#**EXECUTOR**

**Gear Tech issue**

##**ISSUE**

Design and implement simple task executor (aka thread pool).

Task is a simple std::function<void()>. Interface should accept such tasks and execute them on a number of threads N. 

Think through which methods are required and useful in such interface, design and implement them. Additionally you can add tests to cover functionality of executor.

##**SOLUTION INTERFACE**
- Executor may be initialized by two ways: default, then number of threads will be choisen by system, or fixed number **N** in constructor.
 
- Executor have method **push_func** to add std::function<void()> to queue for futher execution.

- Executor can synchronize threads between bunches of pushed tasks by **synchronize** method.

- Executor must be **finalized** after all manually or by default via destructor.
