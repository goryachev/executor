#include <iostream>
#include "executor.h"

/* 
 * Based Idea Source: from https://stackoverflow.com/questions/15752659/thread-pooling-in-c11 
 */

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

    executor.finalize();
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
    for (int i = 0; i < num_functions; i++)
    {
       executor.push_func(example_function);
    }

    executor.finalize();
    std::cout << "done" << std::endl;
}

void unit_test_3()
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
    executor.finalize();
    for (int i = 0; i < num_functions; i++)
    {
       executor.push_func(example_function);
    }

    std::cout << "done" << std::endl;
}

int main()
{
  std::cout << std::endl;
  unit_test_1();
  std::cout << std::endl;
  unit_test_2();
  std::cout << std::endl;
  unit_test_3();
  return 0;
}
