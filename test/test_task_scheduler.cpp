/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

#include "unit_test_framework.h"

#include <stdint.h>
#include <string>
#include <vector>

#include "etl/task.h"
#include "etl/scheduler.h"
#include "etl/container.h"

typedef std::vector<std::string> WorkList_t;

//*****************************************************************************
struct Common
{
  //*********************************************
  Common()
    : idle_callback(*this, &Common::IdleCallback),
      watchdog_callback(*this, &Common::WatchdogCallback),
      watchdog_called(false)
  {
  }

  //*********************************************
  void Clear()
  {
    workList.clear();
  }

  //*********************************************
  void IdleCallback()
  {
    pScheduler->exit_scheduler();
  }

  //*********************************************
  void WatchdogCallback()
  {
    watchdog_called = true;
  }

  WorkList_t workList;
  etl::function<Common, void> idle_callback;
  etl::function<Common, void> watchdog_callback;
  etl::ischeduler* pScheduler;
  bool watchdog_called;
};

//*****************************************************************************
class Task : public etl::task
{
public:

  //*********************************************
  Task(etl::task_priority_t priority_, WorkList_t& work_, Common& common_)
    : task(priority_)
    , task_added(false)
    , work(work_)
    , common(common_)
    , workIndex(0)
    , addAtIndex(0)
    , workToAdd("")
    , pTaskToAddTo(nullptr)
  {
    workCopy = work;
  }

  //*********************************************
  void Reset()
  {
    workIndex    = 0;
    addAtIndex   = 0;
    workToAdd    = "";
    pTaskToAddTo = nullptr;
    work         = workCopy;
    task_added   = false;
  }

  //*********************************************
  void WorkToAdd(int addAtIndex_, const std::string& workToAdd_, Task& taskToAddTo_)
  {
    addAtIndex   = addAtIndex_;
    workToAdd    = workToAdd_;
    pTaskToAddTo = &taskToAddTo_;
  }

  //*********************************************
  virtual uint32_t task_request_work() const ETL_OVERRIDE
  {
    return uint_least8_t(work.size() - workIndex);
  }

  //*********************************************
  virtual void task_process_work() ETL_OVERRIDE
  {
    common.workList.push_back(work[workIndex]);
    ++workIndex;

    if (workIndex == addAtIndex)
    {
      pTaskToAddTo->work.push_back(workToAdd);
    }
  }

  //*********************************************
  virtual void on_task_added() ETL_OVERRIDE
  {
    task_added = true;
  }

  bool task_added;

private:

  WorkList_t work;
  WorkList_t workCopy;
  Common &common;
  uint_least8_t workIndex;
  int addAtIndex;
  std::string workToAdd;
  Task* pTaskToAddTo;
};

Common common;

WorkList_t work1 = { "T1W1", "T1W2", "T1W3" };
WorkList_t work2 = { "T2W1", "T2W2", "T2W3", "T2W4" };
WorkList_t work3 = { "T3W1", "T3W2" };

Task task1(1, work1, common);
Task task2(2, work2, common);
Task task3(3, work3, common);

etl::task* taskList[] = { &task1, &task2, &task3 };

typedef etl::scheduler<etl::scheduler_policy_sequential_single,   sizeof(etl::array_size(taskList))> SchedulerSequentialSingle;
typedef etl::scheduler<etl::scheduler_policy_sequential_multiple, sizeof(etl::array_size(taskList))> SchedulerSequentialMultiple;
typedef etl::scheduler<etl::scheduler_policy_highest_priority,    sizeof(etl::array_size(taskList))> SchedulerHighestPriority;
typedef etl::scheduler<etl::scheduler_policy_most_work,           sizeof(etl::array_size(taskList))> SchedulerMostWork;

namespace
{
  SUITE(test_task_scheduler)
  {
    //*************************************************************************
    TEST(test_task_added)
    {
      SchedulerSequentialSingle s;

      task1.Reset();
      task2.Reset();
      task3.Reset();

      common.Clear();

      s.add_task_list(taskList, ETL_OR_STD17::size(taskList));

      CHECK(task1.task_added);
      CHECK(task2.task_added);
      CHECK(task3.task_added);
    }

    //*************************************************************************
    TEST(test_scheduler_sequencial_single)
    {
      SchedulerSequentialSingle s;

      task1.Reset();
      task2.Reset();
      task3.Reset();

      task2.WorkToAdd(2, "T3W3", task3);

      common.Clear();
      common.pScheduler = &s;

      s.set_idle_callback(common.idle_callback);
      s.set_watchdog_callback(common.watchdog_callback);
      s.add_task_list(taskList, ETL_OR_STD17::size(taskList));
      s.start(); // If 'start' returns then the idle callback was successfully called.

      WorkList_t expected = { "T3W1", "T2W1", "T1W1", "T3W2", "T2W2", "T1W2", "T3W3", "T2W3", "T1W3", "T2W4" };

      CHECK(expected == common.workList);
      CHECK(common.watchdog_called);
    }

    //*************************************************************************
    TEST(test_scheduler_sequencial_multiple)
    {
      SchedulerSequentialMultiple s;

      task1.Reset();
      task2.Reset();
      task3.Reset();

      task2.WorkToAdd(2, "T3W3", task3);

      common.Clear();
      common.pScheduler = &s;

      s.set_idle_callback(common.idle_callback);
      s.set_watchdog_callback(common.watchdog_callback);
      s.add_task_list(taskList, ETL_OR_STD17::size(taskList));
      s.start(); // If 'start' returns then the idle callback was successfully called.

      WorkList_t expected = { "T3W1", "T3W2", "T2W1", "T2W2", "T2W3", "T2W4", "T1W1", "T1W2", "T1W3", "T3W3" };

      CHECK(expected == common.workList);
      CHECK(common.watchdog_called);
    }

    //*************************************************************************
    TEST(test_scheduler_highest_priority)
    {
      SchedulerHighestPriority s;

      task1.Reset();
      task2.Reset();
      task3.Reset();

      task2.WorkToAdd(2, "T3W3", task3);

      common.Clear();
      common.pScheduler = &s;

      s.set_idle_callback(common.idle_callback);
      s.set_watchdog_callback(common.watchdog_callback);
      s.add_task_list(taskList, ETL_OR_STD17::size(taskList));
      s.start(); // If 'start' returns then the idle callback was successfully called.

      WorkList_t expected = { "T3W1", "T3W2", "T2W1", "T2W2", "T3W3", "T2W3", "T2W4", "T1W1", "T1W2", "T1W3" };

      CHECK(expected == common.workList);
      CHECK(common.watchdog_called);
    }

    //*************************************************************************
    TEST(test_scheduler_most_work)
    {
      SchedulerMostWork s;

      task1.Reset();
      task2.Reset();
      task3.Reset();

      task2.WorkToAdd(3, "T3W3", task3);

      common.Clear();
      common.pScheduler = &s;

      s.set_idle_callback(common.idle_callback);
      s.set_watchdog_callback(common.watchdog_callback);
      s.add_task_list(taskList, ETL_OR_STD17::size(taskList));
      s.start(); // If 'start' returns then the idle callback was successfully called.

      WorkList_t expected = { "T2W1", "T2W2", "T1W1", "T3W1", "T2W3", "T3W2", "T1W2", "T3W3", "T2W4", "T1W3" };

      CHECK(expected == common.workList);
      CHECK(common.watchdog_called);
    }
  };
}
