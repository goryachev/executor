#include "executor.h"

void Executor::init()
{
  for (int i = 0; i < num_threads; i++)
  {
    thread_pool.push_back(std::thread(&Function_pool::infinite_loop_func, &func_pool));
  }
  std::cout << "number of threads = " << num_threads << std::endl;
}

Executor::Executor()
{
  num_threads = std::thread::hardware_concurrency();
  init();
};

Executor::Executor(int64_t N)
{ 
  num_threads=N;
  init();
};

void Executor::push_func(std::function<void()> func)
{
  num_functions++; 
  func_pool.push(func);
}

void Executor::run()
{
  func_pool.done();
  for (unsigned int i = 0; i < thread_pool.size(); i++)
  {
    thread_pool.at(i).join();
  }
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
