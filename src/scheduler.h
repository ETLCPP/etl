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

    scheduler_no_tasks_exception(string_type what, string_type file_name, numeric_type line_number)
      : etl::scheduler_exception(ETL_ERROR_TEXT("scheduler:no tasks", ETL_FILE"A"), file_name, line_number))
    {
    }
  };

  //***************************************************************************
  /// 'Null tasks' exception.
  //***************************************************************************
  class scheduler_null_task_exception : public etl::scheduler_exception
  {
  public:

    scheduler_null_task_exception(string_type what, string_type file_name, numeric_type line_number)
      : etl::scheduler_exception(ETL_ERROR_TEXT("scheduler:null task", ETL_FILE"B"), file_name, line_number))
    {
    }
  };

  //***************************************************************************
  /// The policy the scheduler uses to decide what to do next.
  //***************************************************************************
  struct scheduler_policy
  {
    enum
    {
      SEQUENTIAL_SINGLE,
      SEQUENTIAL_MULTIPLE,
      HIGHEST_PRIORITY
    };
  };

  //***************************************************************************
  /// Scheduler.
  //***************************************************************************
  template <const size_t MAX_TASKS_, const size_t POLICY_>
  class scheduler
  {
  public:

    enum
    {
      MAX_TASKS = MAX_TASKS_,
      POLICY    = POLICY_
    };

    //*******************************************
    /// Destructor.
    //*******************************************
    virtual ~scheduler()
    {
    }

    //*******************************************
    /// Add a task.
    //*******************************************
    void add_task(etl::task& task)
    {
      task_list_t::iterator itask = std::lower_bound(task_list.begin(),
                                                     task_list.end(),
                                                     task.get_task_priority(),
                                                     compare_priority());

      task_list.insert(itask, &task);
    }

    //*******************************************
    /// Add a task list.
    //*******************************************
    template <typename TSize>
    void add_task_list(etl::task** p_tasks, TSize size)
    {
      for (TSize i = 0; i < size; ++i)
      {
        ETL_ASSERT((p_tasks[i] != nullptr), ETL_ERROR(etl::scheduler_null_task_exception));
        add_task(*(p_tasks[i]);
      }
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
    /// Start the scheduler. SEQUENCIAL_SINGLE
    //*******************************************
    etl::enable_if<POLICY == scheduler_policy::SEQUENCIAL_SINGLE, void>::type
     start()
    {
      ETL_ASSERT(task_list.size() > 0, ETL_ERROR(etl::scheduler_no_tasks_exception));

      const size_t task_list_size = task_list.size();

      scheduler_running = true;

      while (true)
      {
        if (scheduler_running)
        {
          bool idle = true;

          for (size_t index = 0; index < task_list_size; ++index)
          {
            etl::task& task = *(task_list[index]);

            if (task.task_request_work())
            {
              task.task_process_work();
              idle = false;
            }
          }

          if (idle)
          {
            on_scheduler_idle();
          }
        }
      }
    }

    //*******************************************
    /// Start the scheduler. SEQUENCIAL_MULTIPLE
    //*******************************************
    etl::enable_if<POLICY == scheduler_policy::SEQUENCIAL_MULTIPLE, void>::type
     start()
    {
      ETL_ASSERT((task_list.size() > 0), ETL_ERROR(etl::scheduler_no_tasks_exception));

      const size_t task_list_size = task_list.size();

      scheduler_running = true;

      while (true)
      {
        if (scheduler_running)
        {
          bool idle = true;

          for (size_t index = 0; index < task_list_size; ++index)
          {
            etl::task& task = *(task_list[index]);

            while (task.task_request_work())
            {
              task.task_process_work();
              idle = false;
            }
          }

          if (idle)
          {
            on_scheduler_idle();
          }
        }
      }
    }

    //*******************************************
    /// Start the scheduler. HIGHEST_PRIORITY
    //*******************************************
    etl::enable_if<POLICY == scheduler_policy::HIGHEST_PRIORITY, void>::type
     start()
    {
      ETL_ASSERT(task_list.size() > 0, ETL_ERROR(etl::scheduler_no_tasks_exception));

      const size_t task_list_size = task_list.size();

      scheduler_running = true;

      while (true)
      {
        if (scheduler_running)
        {
          bool idle = true;

          for (size_t index = 0; index < task_list_size; ++index)
          {
            etl::task& task = *(task_list[index]);

            if (task.task_request_work())
            {
              task.task_process_work();
              idle = false;
              index = 0; // Restart from the top of the list.
            }
          }

          if (idle)
          {
            on_scheduler_idle();
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
    }

    //*******************************************
    /// Called when no work has been done in one cycle.
    //*******************************************
    virtual void on_scheduler_idle()
    {
      // By default do nothing.
    }

    typedef etl::vector<etl::task*, MAX_TASKS> task_list_t;
    task_list_t task_list;
    bool scheduler_running;
  };
}

#undef ETL_FILE

#endif
