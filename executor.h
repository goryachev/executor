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
/**
 * Based Idea Source from https://stackoverflow.com/questions/15752659/thread-pooling-in-c11
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
    std::atomic<int64_t> num_functions=0;

    void init();
    bool isFinalized=false;
public:
    Executor();
    Executor(const int64_t N);
    // delete all copy posibilities to avoid the increasing life time of joinable thread 
    Executor(Executor const&) = delete;
    Executor& operator=(Executor const&) = delete;
    void done();
    void infinite_loop_func();
    void push_func(std::function<void()> func);
    void synchronize();
    void finalize();
    void join_all();
    int64_t get_num_threads();
    ~Executor();
};