/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 jwellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

//*****************************************************************************
// This example demonstrates the basic method to schedule tasks.
// Experiment with the different scheduling policies.
//*****************************************************************************

#include "etl/scheduler.h"
#include "etl/task.h"
#include "etl/function.h"

#include <iostream>
#include <string>

//*****************************************************************************
// Task 1. Lowest priority.
//*****************************************************************************
class Task1 : public etl::task
{
public:

  //*************************************
  Task1()
    : task(1)
    , work(3)
  {

  }

  //*************************************
  uint32_t task_request_work() const
  {
    return work; // How much work do we still have to do? This could be a message queue length.
  }

  //*************************************
  void task_process_work()
  {
    std::cout << "Task1 : Process work : " << work << std::endl;
    --work;
  }

private:

  uint32_t work;
};

//*****************************************************************************
// Task 2. Highest priority.
//*****************************************************************************
class Task2 : public etl::task
{
public:

  //*************************************
  Task2()
    : task(2)
    , work(4)
  {

  }

  //*************************************
  uint32_t task_request_work() const
  {
    return work; // How much work do we still have to do? This could be a message queue length.
  }

  //*************************************
  void task_process_work()
  {
    std::cout << "Task2 : Process work : " << work << std::endl;
    --work;
  }

private:

  uint32_t work;
};

//*****************************************************************************
// The idle handler.
//*****************************************************************************
class Idle
{
public:

  //*************************************
  Idle(etl::ischeduler& scheduler_)
    : scheduler(scheduler_)
  {

  }

  //*************************************
  void IdleCallback()
  {
    std::cout << "Idle callback" << std::endl;
    scheduler.exit_scheduler();
    std::cout << "Exiting the scheduler" << std::endl;
  }

private:

  etl::ischeduler& scheduler;
};

//*****************************************************************************
// The watchdog handler.
//*****************************************************************************
void WatchdogCallback()
{
  std::cout << "Watchdog callback" << std::endl;
}

//*****************************************************************************
// The scheduler.
// Maximum of two tasks.
//
// Try the other scheduler policies to see how scheduling differs.
// etl::scheduler_policy_sequential_single
// etl::scheduler_policy_sequential_multiple
// etl::scheduler_policy_highest_priority
// etl::scheduler_policy_most_work
//*****************************************************************************
class Scheduler : public etl::scheduler<etl::scheduler_policy_sequential_single, 2>
{
};

//*****************************************************************************
// The test application.
//*****************************************************************************
int main()
{
  Scheduler scheduler;

  Task1 task1;
  Task2 task2;

  Idle idleHandler(scheduler);

  etl::function_mv<Idle, &Idle::IdleCallback> idleCallback(idleHandler); // Member function, no parameters, returning void.
  etl::function_fv<WatchdogCallback>          watchdogCallback;          // Global function, no parameters, returning void.

  scheduler.add_task(task1);
  scheduler.add_task(task2);
  scheduler.set_idle_callback(idleCallback);
  scheduler.set_watchdog_callback(watchdogCallback);

  std::cout << "Starting the scheduler" << std::endl;

  scheduler.start();

  std::cout << "Exiting the application" << std::endl;

  return 0;
}

