#pragma once
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <future>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cassert>


/**
*  Chapter 3 "C++ Concurrency in Action", A. Williams
*/

class Executor
{
private:
    std::queue<std::function<void()>> m_function_queue;
    std::vector<std::thread> thread_pool;
    std::vector<std::promise<void>> promises;
    std::vector<std::future<void>> futures;

    std::mutex m_lock;
    std::condition_variable m_data_condition;
    std::atomic<bool> m_accept_functions;
 
    int64_t num_threads=0;
    int64_t num_functions=0; 

    void init();
    bool isFinalized=false;
public:
    Executor();
    Executor(const int64_t N);
    void done();
    void infinite_loop_func();
    void push_func(std::function<void()> func);
    void synchronize();
    void finalize();
    int64_t get_num_threads();
    int64_t get_num_threads() const;
    ~Executor();
};