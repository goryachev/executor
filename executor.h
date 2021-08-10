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
 *   Suitable Sources:
 *   --- A. Williams, "C++ Concurrency in Action"
 *   --- https://stackoverflow.com/questions/15752659/thread-pooling-in-c11 (based idea)
 *   --- https://github.com/amc176/cpp-threadpool
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
 
    uint64_t m_num_threads=0;
    std::atomic<uint64_t> m_total_functions = 0;
    std::atomic<uint64_t> m_current_num_functions = 0;

    void init();
    bool isFinalized=false;

    void infinite_loop_func();
    void join_all();
    void done();


 public:
    Executor();
    Executor(const int64_t N);
    // delete all copy posibilities to avoid the increasing life time of joinable thread 
    Executor(Executor const&) = delete;
    Executor& operator=(Executor const&) = delete;
   
    void push_func(std::function<void()> func);
    void synchronize();
    void finalize();
    uint64_t get_total_num_functions();
    uint64_t get_current_num_functions();
    uint64_t get_num_threads();
    uint64_t get_num_threads() const;
    ~Executor();
};