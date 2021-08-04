#**functionPool**
**Gear Tech issue**
##**ISSUE**
Design and implement simple task executor (aka thread pool).

Task is a simple std::function<void()>. Interface should accept such tasks and execute them on a number of threads N. 

Think through which methods are required and useful in such interface, design and implement them. Additionally you can add tests to cover functionality of executor.

##**SOLUTION**
- Executor may be initialized by two ways: default, then number of threads will be choisen by system, or fixd number **N** in constructor.
 
- Executor have method **push_func** to add std::function<void()> to query for futher execution.

- After filling function query launch method run for parallel tasks execution.
