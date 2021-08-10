#include "executor.h"

/**
 * @brief Helper method for thread pool initialization in constructor
 */
void Executor::init()
{
    std::cout << "number of threads = " << m_num_threads << std::endl;
    for (int i = 0; i < m_num_threads; i++)
    {
        promises.push_back(std::promise<void>());
        futures.push_back(promises.back().get_future());
        thread_pool.push_back(std::thread(&Executor::infinite_loop_func, this));
    }
}

/**
 * @brief Construct with default (system) number of threads
 */
Executor::Executor() : m_function_queue(), m_lock(), m_data_condition(), m_accept_functions(true)
{
    m_num_threads = std::thread::hardware_concurrency();
    init();
};

/**
 * @brief Construct with number of threads equals N
 */
Executor::Executor(const int64_t N) : m_function_queue(), m_lock(), m_data_condition(), m_accept_functions(true)
{ 
    m_num_threads = N;
    init();
};

/**
 * @brief Add function to queue for execution  
 */
void Executor::push_func(std::function<void()> func)
{
    
    std::unique_lock<std::mutex> lock(m_lock);
    m_function_queue.push(func);
    m_total_functions++;
    m_current_num_functions++;
    // when we send the notification immediately, the consumer will try to get the lock , so unlock asap
    lock.unlock();
    m_data_condition.notify_one();
}

/**
 * @brief Helper method for safety thread pool finalization
 */
void Executor::done()
{
    std::unique_lock<std::mutex> lock(m_lock);
    m_accept_functions = false;
    lock.unlock();
    // when we send the notification immediately, the consumer will try to get the lock , so unlock asap
    m_data_condition.notify_all();
    //notify all waiting threads.
}

/**
 * @brief Infinite loop with function pop from the queue and further execution 
 */
void Executor::infinite_loop_func()
{
    std::function<void()> func;
    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(m_lock);
            m_data_condition.wait(lock, [this]() {return !m_function_queue.empty() || !m_accept_functions; });
            if (!m_accept_functions && m_function_queue.empty())
            {
                //lock will be release automatically.
                //finish the thread loop and let it join in the main thread.
                return;
            }
            func = m_function_queue.front();
            m_function_queue.pop();
            //release the lock
        }
        func();
        m_current_num_functions--;
    }
}

/**
 * @brief Allow parent thread to wait for remaining tasks to complete
 */
void Executor::synchronize() 
{
    do {
        std::this_thread::yield();
    } while (m_current_num_functions > 0);
    std::cout << "Synchronization\n";
 }

/**
 * @brief Finalize the execution of thread pool
 */
void Executor::finalize()
{
    done();
    join_all();
    isFinalized = true;
    std::cout << "total number of executed functions = " << m_total_functions << std::endl;
};

/**
 * @brief Returns the current number of worker threads in the pool
 */
uint64_t Executor::get_num_threads()
{
    return thread_pool.size();
}
uint64_t Executor::get_num_threads() const
{
    return thread_pool.size();
}

/**
 * @brief Returns the total number of function tasks that already have been executed and must be executed
 */
uint64_t Executor::get_total_num_functions() 
{
    std::unique_lock<std::mutex> lock(m_lock);
    return m_total_functions;
}

/**
 * @brief Returns the current number of function tasks that must be executed
 */
uint64_t Executor::get_current_num_functions()
{
    std::unique_lock<std::mutex> lock(m_lock);
    return m_current_num_functions;
}

/**
 * @brief Destruct and check if the thread pool execution was finalized manually. 
 * @details Finalize, if it wasn't done manually.
 */
Executor::~Executor()
{
    if(!isFinalized)
    { 
        finalize();
    }
    thread_pool.clear();
}

/**
 * @brief Join all threads of thread pool to the parent
 */
void Executor::join_all()
{
    for (auto& thread : thread_pool)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}
