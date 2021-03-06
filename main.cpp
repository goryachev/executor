#include <iostream>
#include "executor.h"


//class quit_worker_exception : public std::exception {};

void example_function()
{
    std::cout << ("GearTech4ever!\tthreadID: " + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())) + "\n");
}

void example_function_inversely()
{
    std::cout << ("!reve4hceTraeG\tthreadID: " + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())) + "\n");
}

/**
 *  Simple test for check of correct launch and finalize thread executions
 */
void unit_test_1()
{
    std::cout << "starting <unit_test_1> operation" << std::endl;
    Executor executor;
    const int64_t num_functions = executor.get_num_threads();

    // here we should send our functions
    for (int i = 0; i < num_functions; i++)
    {
       executor.push_func(example_function);
    }
    // finalize thread executions for correct finish
    executor.finalize();
    std::cout << "done" << std::endl;
}

/**
 *  Here we will check asynchronous execution of two function types  
 */
void unit_test_2()
{
    std::cout << "starting <unit_test_2> operation" << std::endl;
    const int64_t N = 6;
    const int64_t num_functions = 12;

    try
    {
        Executor executor(6);

        // here we should send two type of our functions
        for (int i = 0; i < num_functions; i++)
        {// type One
            executor.push_func(example_function);
        }
        for (int i = 0; i < num_functions; i++)
        {// type Two
            executor.push_func(example_function_inversely);
        }
        // threads will be finalize in destructor
    }
    catch (quit_worker_exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    std::cout << "done" << std::endl;
}

/**
 *  Here we will check synchronization between executions of two function types
 */
void unit_test_3()
{
    std::cout << "starting <unit_test_3> operation" << std::endl;
    const int64_t N = 6;
    Executor executor(6);
    const int64_t num_functions = 12;

    // here we should send first portion of our functions
    for (int i = 0; i < num_functions; i++)
    {// function type One
       executor.push_func(example_function);
    }
    // synchronize thread executions
    executor.synchronize();
    // here we should send second portion of our functions
    for (int i = 0; i < num_functions; i++)
    {// function type Two
       executor.push_func(example_function_inversely);
    }
    // finalize thread executions for correct finish
    executor.finalize();
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
#ifdef _WIN64
// Windows systems only for console info keeping on screen 
  std::cout << "\n(PRESS ENTER)\n";
  getchar();
#endif
  return 0;
}
