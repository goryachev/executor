#include "executor.h"

void Executor::init()
{
    std::cout << "number of threads = " << num_threads << std::endl;
    for (int i = 0; i < num_threads; i++)
    {
        promises.push_back(std::promise<void>());
        futures.push_back(promises.back().get_future());
        thread_pool.push_back(std::thread(&Executor::infinite_loop_func, this));
    }
}

Executor::Executor() : m_function_queue(), m_lock(), m_data_condition(), m_accept_functions(true)
{
    num_threads = std::thread::hardware_concurrency();
    init();
};

Executor::Executor(const int64_t N) : m_function_queue(), m_lock(), m_data_condition(), m_accept_functions(true)
{ 
    num_threads = N;
    init();
};

void Executor::push_func(std::function<void()> func)
{
    num_functions++; 
    std::unique_lock<std::mutex> lock(m_lock);
    m_function_queue.push(func);
    // when we send the notification immediately, the consumer will try to get the lock , so unlock asap
    lock.unlock();
    m_data_condition.notify_one();
}

void Executor::done()
{
    std::unique_lock<std::mutex> lock(m_lock);
    m_accept_functions = false;
    lock.unlock();
    // when we send the notification immediately, the consumer will try to get the lock , so unlock asap
    m_data_condition.notify_all();
    //notify all waiting threads.
}

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
    }
}

void Executor::synchronize() 
{
    for (const auto& future : futures) future.wait();
    std::cout << "Synchronization\n";
}

void Executor::finalize()
{
    done();
    for (unsigned int i = 0; i < thread_pool.size(); i++)
    {
      thread_pool.at(i).join();
    }
    isFinalized = true;
    std::cout << "number of executed functions = " << num_functions << std::endl;
};

int64_t Executor::get_num_threads()
{
    return thread_pool.size();
}

int64_t Executor::get_num_threads() const
{
    return thread_pool.size();
}

Executor::~Executor()
{
    if(!isFinalized)
    { 
        finalize();
    }
    thread_pool.clear();
}