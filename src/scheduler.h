/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#ifndef __ETL_SCHEDULER__
#define __ETL_SCHEDULER__

#include <stdint.h>

#include "vector.h"
#include "nullptr.h"
#include "error_handler.h"
#include "exception.h"
#include "task.h"
#include "type_traits.h"
#include "function.h"

#undef ETL_FILE
#define ETL_FILE "36"

namespace etl
{
  //***************************************************************************
  /// Base exception class for scheduler.
  //***************************************************************************
  class scheduler_exception : public etl::exception
  {
  public:

    scheduler_exception(string_type what, string_type file_name, numeric_type line_number)
      : etl::exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// 'No tasks' exception.
  //***************************************************************************
  class scheduler_no_tasks_exception : public etl::scheduler_exception
  {
  public:

    scheduler_no_tasks_exception(string_type file_name, numeric_type line_number)
      : etl::scheduler_exception(ETL_ERROR_TEXT("scheduler:no tasks", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// 'Null tasks' exception.
  //***************************************************************************
  class scheduler_null_task_exception : public etl::scheduler_exception
  {
  public:

    scheduler_null_task_exception(string_type file_name, numeric_type line_number)
      : etl::scheduler_exception(ETL_ERROR_TEXT("scheduler:null task", ETL_FILE"B"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Sequencial Single.
  /// A policy the scheduler can use to decide what to do next.
  /// Only calls the task to process work once, if it has work to do.
  //***************************************************************************
  struct scheduler_policy_sequencial_single
  {
    bool schedule_tasks(etl::ivector<etl::task*>& task_list)
    {
      bool idle = true;

      for (size_t index = 0; index < task_list.size(); ++index)
      {
        etl::task& task = *(task_list[index]);

        if (task.task_request_work() > 0)
        {
          task.task_process_work();
          idle = false;
        }
      }

      return idle;
    }
  };

  //***************************************************************************
  /// Sequencial Multiple.
  /// A policy the scheduler can use to decide what to do next.
  /// Calls the task to process work until it reports that it has no more.
  //***************************************************************************
  struct scheduler_policy_sequencial_multiple
  {
    bool schedule_tasks(etl::ivector<etl::task*>& task_list)
    {
      bool idle = true;

      for (size_t index = 0; index < task_list.size(); ++index)
      {
        etl::task& task = *(task_list[index]);

        while (task.task_request_work() > 0)
        {
          task.task_process_work();
          idle = false;
        }
      }

      return idle;
    }
  };
  
  //***************************************************************************
  /// Highest Priority.
  /// A policy the scheduler can use to decide what to do next.
  /// Calls the highest priority task that has work.
  //***************************************************************************
  struct scheduler_policy_highest_priority
  {
    bool schedule_tasks(etl::ivector<etl::task*>& task_list)
    {
      bool idle = true;

      size_t index = 0;
      while (index < task_list.size())
      {
        etl::task& task = *(task_list[index]);

        if (task.task_request_work() > 0)
        {
          task.task_process_work();
          idle = false;
          index = 0; // Restart from the top of the list.
        }
        else
        {
          ++index;
        }
      }

      return idle;
    }
  };

  //***************************************************************************
  /// Most Work.
  /// A policy the scheduler can use to decide what to do next.
  /// Calls the task that has the most work.
  /// Starts looking from the task with the highest priority.
  //***************************************************************************
  struct scheduler_policy_most_work
  {
    bool schedule_tasks(etl::ivector<etl::task*>& task_list)
    {
      bool idle = true;

      size_t most_index = 0;
      uint_least8_t most_work = 0;

      for (size_t index = 0; index < task_list.size(); ++index)
      {
        etl::task& task = *(task_list[index]);

        uint_least8_t n_work = task.task_request_work();

        if (n_work > most_work)
        {
          most_index = index;
          most_work = n_work;
          idle = false;
        }
      }

      if (!idle)
      {
        task_list[most_index]->task_process_work();
      }

      return idle;
    }
  };

  //***************************************************************************
  /// Scheduler base.
  //***************************************************************************
  class ischeduler
  {
  public:

    //*******************************************
    /// Constructor.
    //*******************************************
    ischeduler()
      : scheduler_running(false),
        scheduler_exit(false),
        p_idle_callback(nullptr)
    {
    }

    //*******************************************
    /// Set the idle callback.
    //*******************************************
    void set_idle_callback(etl::ifunction<void>& callback)
    {
      p_idle_callback = &callback;
    }

    //*******************************************
    /// Set the running state for the scheduler.
    //*******************************************
    void set_scheduler_running(bool scheduler_running_)
    {
      scheduler_running = scheduler_running_;
    }

    //*******************************************
    /// Get the running state for the scheduler.
    //*******************************************
    bool scheduler_is_running() const
    {
      return scheduler_running;
    }

    //*******************************************
    /// Force the scheduler to exit.
    //*******************************************
    void exit_scheduler()
    {
      scheduler_exit = true;
    }

  protected:

    bool scheduler_running;
    bool scheduler_exit;
    etl::ifunction<void>* p_idle_callback;
  };

  //***************************************************************************
  /// Scheduler.
  //***************************************************************************
  template <typename TSchedulerPolicy, size_t MAX_TASKS_>
  class scheduler : public etl::ischeduler, protected TSchedulerPolicy
  {
  public:

    enum
    {
      MAX_TASKS = MAX_TASKS_,
    };

    //*******************************************
    /// Add a task.
    /// Add to the task list in priority order.
    //*******************************************
    void add_task(etl::task& task)
    {
      typename task_list_t::iterator itask = std::lower_bound(task_list.begin(),
                                                              task_list.end(),
                                                              task.get_task_priority(),
                                                              compare_priority());

      task_list.insert(itask, &task);
    }

    //*******************************************
    /// Add a task list.
    /// Adds to the tasks to the internal task list in priority order.
    /// Input order is ignored.
    //*******************************************
    template <typename TSize>
    void add_task_list(etl::task** p_tasks, TSize size)
    {
      for (TSize i = 0; i < size; ++i)
      {
        ETL_ASSERT((p_tasks[i] != nullptr), ETL_ERROR(etl::scheduler_null_task_exception));
        add_task(*(p_tasks[i]));
      }
    }

    //*******************************************
    /// Start the scheduler. SEQUENCIAL_SINGLE
    /// Only calls the task to process work once, if it has work to do.
    //*******************************************
    void start()
    {
      ETL_ASSERT(task_list.size() > 0, ETL_ERROR(etl::scheduler_no_tasks_exception));

      const size_t task_list_size = task_list.size();

      scheduler_running = true;

      while (!scheduler_exit)
      {
        if (scheduler_running)
        {
          bool idle = TSchedulerPolicy::schedule_tasks(task_list);

          if (idle && p_idle_callback)
          {
            (*p_idle_callback)();
          }
        }
      }
    }

  private:

    //*******************************************
    // Used to order tasks in descending priority.
    //*******************************************
    struct compare_priority
    {
      bool operator()(etl::task* ptask, etl::task_priority_t priority) const
      {
        return ptask->get_task_priority() > priority;
      }
    };

    typedef etl::vector<etl::task*, MAX_TASKS> task_list_t;
    task_list_t task_list;
  };
}

#undef ETL_FILE

#endif
