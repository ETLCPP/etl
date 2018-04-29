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

#ifndef ETL_TASK_INCLUDED
#define ETL_TASK_INCLUDED

#include <stdint.h>

#include "platform.h"
#include "error_handler.h"
#include "exception.h"

#undef ETL_FILE
#define ETL_FILE "37"

namespace etl
{
  //***************************************************************************
  /// Base exception class for task.
  //***************************************************************************
  class task_exception : public etl::exception
  {
  public:

    task_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : etl::exception(reason_, file_name_, line_number_)
    {
    }
  };

  typedef uint_least8_t task_priority_t;

  //***************************************************************************
  /// Scheduler.
  //***************************************************************************
  class task
  {
  public:

    //*******************************************
    /// Constructor.
    //*******************************************
    task(task_priority_t priority)
      : task_running(true),
        task_priority(priority)
    {
    }

    //*******************************************
    /// Destructor.
    //*******************************************
    virtual ~task()
    {
    }

    //*******************************************
    /// Called to check if the task has work.
    /// Returns a score as to the amount of work it has to do.
    //*******************************************
    virtual uint32_t task_request_work() const = 0;

    //*******************************************
    /// Called to get the task to do work.
    //*******************************************
    virtual void task_process_work() = 0;

    //*******************************************
    /// Set the running state for the task.
    //*******************************************
    void set_task_running(bool task_running_)
    {
      task_running = task_running_;
    }

    //*******************************************
    /// Get the running state for the task.
    //*******************************************
    bool task_is_running() const
    {
      return task_running;
    }

    //*******************************************
    /// Get the priority of the task.
    /// Higher value = higher priority.
    //*******************************************
    etl::task_priority_t get_task_priority() const
    {
      return task_priority;
    }

  private:

    bool task_running;
    etl::task_priority_t task_priority;
  };
}

#undef ETL_FILE

#endif
