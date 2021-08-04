#include "function_pool.h"
#include <string>
#include <iostream>
#include <mutex>
#include <functional>
#include <thread>
#include <vector>

/* 
 * 	Source: from https://stackoverflow.com/questions/15752659/thread-pooling-in-c11 
 */

Function_pool func_pool;

class quit_worker_exception : public std::exception {};

void example_function()
{
    std::cout << "GearTech4ever!" << std::endl;
}

int main()
{
    std::cout << "stating operation" << std::endl;
    int num_threads = std::thread::hardware_concurrency();
    std::cout << "number of threads = " << num_threads << std::endl;
    std::vector<std::thread> thread_pool;
    for (int i = 0; i < num_threads; i++)
    {
        thread_pool.push_back(std::thread(&Function_pool::infinite_loop_func, &func_pool));
    }

    //here we should send our functions
    for (int i = 0; i < 50; i++)
    {
        func_pool.push(example_function);
    }
    func_pool.done();
    for (unsigned int i = 0; i < thread_pool.size(); i++)
    {
        thread_pool.at(i).join();
    }
}
