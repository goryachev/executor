#include "function_pool.h"
#include <string>
#include <iostream>
#include <mutex>
#include <functional>
#include <thread>
#include <vector>

/* 
 * Based Idea Source: from https://stackoverflow.com/questions/15752659/thread-pooling-in-c11 
 */

class Executor
{
private:
    Function_pool func_pool;
    int64_t num_threads=0;
    int64_t num_functions=0; 
    std::vector<std::thread> thread_pool;
    void init()
    {

    for (int i = 0; i < num_threads; i++)
    {
        thread_pool.push_back(std::thread(&Function_pool::infinite_loop_func, &func_pool));
    }
    std::cout << "number of threads = " << num_threads << std::endl;
    }

public:
    Executor(){num_threads = std::thread::hardware_concurrency();init();};
    Executor(int64_t N): num_threads(N){init();};
    void push_func(std::function<void()> func){num_functions++; func_pool.push(func);}
    void run()
    {
    func_pool.done();
    for (unsigned int i = 0; i < thread_pool.size(); i++)
    {
        thread_pool.at(i).join();
    }
    std::cout << "number of executed functions = " << num_functions << std::endl;
    };
    int64_t get_num_threads()
    {
       return thread_pool.size();
    }
    int64_t get_num_threads() const
    {
       return thread_pool.size();
    }
};
//class quit_worker_exception : public std::exception {};

void example_function()
{
    std::cout << "GearTech4ever!\n";
}

void unit_test_1()
{
    std::cout << "starting operation" << std::endl;
    Executor executor;
    const int64_t num_functions = executor.get_num_threads();

    //here we should send our functions
    for (int i = 0; i < num_functions; i++)
    {
       executor.push_func(example_function);
    }

    executor.run();
    std::cout << "done" << std::endl;
}

void unit_test_2()
{
    std::cout << "starting operation" << std::endl;
    const int64_t N = 6;
    Executor executor(6);
    const int64_t num_functions = 12;

    //here we should send our functions
    for (int i = 0; i < num_functions; i++)
    {
       executor.push_func(example_function);
    }

    executor.run();
    std::cout << "done" << std::endl;
}

int main()
{
  std::cout << std::endl;
  unit_test_1();
  std::cout << std::endl;
  unit_test_2();
  return 0;
}
