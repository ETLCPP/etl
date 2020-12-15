///\file

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

#ifndef ETL_MULTI_LOOP_INCLUDED
#define ETL_MULTI_LOOP_INCLUDED

#include "vector.h"
#include "algorithm.h"
#include "error_handler.h"
#include "exception.h"

#undef ETL_FILE
#define ETL_FILE "57"

namespace etl
{
  struct multi_loop_exception : etl::exception
  {
    multi_loop_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  struct multi_loop_parameter_error : multi_loop_exception
  {
    multi_loop_parameter_error(string_type file_name_, numeric_type line_number_)
      : multi_loop_exception(ETL_ERROR_TEXT("multi_loop:paramters", ETL_FILE"A"), file_name_, line_number_)
    {
    }
  };

  struct multi_loop_index_error : multi_loop_exception
  {
    multi_loop_index_error(string_type file_name_, numeric_type line_number_)
      : multi_loop_exception(ETL_ERROR_TEXT("multi_loop:index", ETL_FILE"B"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// imulti_loop
  //***************************************************************************
  template<typename T>
  class imulti_loop
  {
  public:

    //***************************************************************************
    void add_inner(T begin_value, T end_value)
    {
      ETL_ASSERT(begin_value < end_value, ETL_ERROR(multi_loop_parameter_error));

      loops.push_back(loop_info(begin_value, end_value));
    }

    //***************************************************************************
    void add_outer(T begin_value, T end_value)
    {
      ETL_ASSERT(begin_value < end_value, ETL_ERROR(multi_loop_parameter_error));

      loops.insert(loops.begin(), loop_info(begin_value, end_value));
    }

    //***************************************************************************
    void start()
    {
      etl::for_each(loops.begin(), loops.end(), reset_loop);
    }

    //***************************************************************************
    bool completed() const
    {
      // We have completed all nested loops if the outer-most loop has completed
      // or we have configured no loops.
      return (loops[0].completed()) ||
             (number_of_loops() == 0);
    }

    //***************************************************************************
    void next()
    {
      if (!completed())
      {
        size_t i = number_of_loops() - 1U;

        while (true)
        {
          // Increment the loop.
          ++loops[i].current;

          // Has the loop not completed or is it the outer loop?
          if (!loops[i].completed() || (i == 0U))
          {
            return;
          }

          // Reset it and move on to the next outer loop.
          loops[i].reset();
          --i;
        }
      }
    }

    //***************************************************************************
    T begin(size_t i) const
    {
      ETL_ASSERT(i < number_of_loops(), ETL_ERROR(multi_loop_index_error));

      return loops[i].begin;
    }

    //***************************************************************************
    T end(size_t i) const
    {
      ETL_ASSERT(i < number_of_loops(), ETL_ERROR(multi_loop_index_error));

      return loops[i].end;
    }

    //***************************************************************************
    T index(size_t i) const
    {
      ETL_ASSERT(i < number_of_loops(), ETL_ERROR(multi_loop_index_error));

      return loops[i].current;
    }

    //***************************************************************************
    const T operator [](int i) const
    {
      ETL_ASSERT(i < number_of_loops(), ETL_ERROR(multi_loop_index_error));

      return (loops[i].current);
    }

    //***************************************************************************
    size_t number_of_loops() const
    {
      return loops.size();
    }

    //***************************************************************************
    size_t max_number_of_loops() const
    {
      return max_loops;
    }

  protected:

    //***************************************************************************
    struct loop_info
    {
      loop_info(T begin_, T end_)
        : begin(begin_)
        , end(end_)
        , current(begin_)
      {
      }

      void reset()
      {
        current = begin;
      }

      bool completed() const
      {
        return current == end;
      }

      T begin;
      T end;
      T current;
    };

    //***************************************************************************
    imulti_loop(etl::ivector<loop_info>& loops_, size_t max_loops_)
      : loops(loops_)
      , max_loops(max_loops_)
    {
    }

    //***************************************************************************
    static void reset_loop(loop_info& loop)
    {
      loop.reset();
    }

    etl::ivector<loop_info>& loops; ///< The list of loops.
    size_t max_loops;               ///< The maximum supported number of loops.
  };

  //***************************************************************************
  /// multi_loop
  //***************************************************************************
  template<typename T, size_t MAX_LOOPS_>
  class multi_loop : public imulti_loop<T>
  {
  public:

    static ETL_CONSTANT size_t MAX_LOOPS = MAX_LOOPS_;

    //***************************************************************************
    multi_loop()
      : imulti_loop(loops, MAX_LOOPS)
    {
    }

  private:

    etl::vector<typename etl::imulti_loop<T>::loop_info, MAX_LOOPS> loops;
  };
}

#undef ETL_FILE

#endif