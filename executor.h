#pragma once
#include <string>
#include <iostream>
#include <mutex>
#include <functional>
#include <thread>
#include <vector>
#include "function_pool.h"

class Executor
{
private:
    Function_pool func_pool;
    int64_t num_threads=0;
    int64_t num_functions=0; 
    std::vector<std::thread> thread_pool;
    void init();

public:
    Executor();
    Executor(int64_t N);
    void push_func(std::function<void()> func);
    void run();
    int64_t get_num_threads();
    int64_t get_num_threads() const;
};
