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

#include "platform.h"
#include "nullptr.h"
#include "functional.h"
#include "exception.h"
#include "error_handler.h"

#undef ETL_FILE
#define ETL_FILE "57"

namespace etl
{
  //***************************************************************************
  /// Exception for the multi_loop.
  //***************************************************************************
  class multi_loop_exception : public etl::exception
  {
  public:

    multi_loop_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Circular reference exception.
  //***************************************************************************
  class multi_loop_circular_reference : public etl::multi_loop_exception
  {
  public:

    multi_loop_circular_reference(string_type file_name_, numeric_type line_number_)
      : etl::multi_loop_exception(ETL_ERROR_TEXT("multi_loop:circular reference", ETL_FILE"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The base class for multi_loop.
  //***************************************************************************
  class imulti_loop
  {
  public:

    //***************************************************************************
    /// Insert after this loop.
    //***************************************************************************
    imulti_loop& insert(imulti_loop& inner_loop)
    {
      ETL_ASSERT(is_valid(inner_loop), ETL_ERROR(multi_loop_circular_reference));

      // Remember what the next loop was.
      imulti_loop* next = inner;

      // Append the new loop
      inner = &inner_loop;

      // Link to the original next loop.
      inner_loop.set_last(next);

      return *this;
    }

    //***************************************************************************
    /// Append to the most inner loop.
    //***************************************************************************
    imulti_loop& append(imulti_loop& inner_loop)
    {
      ETL_ASSERT(is_valid(inner_loop), ETL_ERROR(multi_loop_circular_reference));

      if (inner != nullptr)
      {
        inner->append(inner_loop);
      }
      else
      {
        inner = &inner_loop;
      }

      return *this;
    }

    //***************************************************************************
    /// Unlinks this loop from its inner.
    //***************************************************************************
    void detach()
    {
      inner = ETL_NULLPTR;  
    }

    //***************************************************************************
    /// Unlinks this and all inner loops.
    //***************************************************************************
    void detach_all()
    {
      if (inner != ETL_NULLPTR)
      {
        inner->detach_all();
      }

      detach();
    }

    //***************************************************************************
    ///
    //***************************************************************************
    bool completed() const
    {
      return has_completed;
    }

    //***************************************************************************
    /// Gets the total number of loops, from this loop inclusive.
    //***************************************************************************
    size_t number_of_loops() const
    {
      size_t count = 1U;

      imulti_loop* p_loop = inner;

      while (p_loop != ETL_NULLPTR)
      {
        ++count;
        p_loop = p_loop->inner;
      }

      return count;
    }

    //***************************************************************************
    /// Gets the total number of iterations over all loops, from this loop inclusive.
    //***************************************************************************
    size_t number_of_iterations()
    {
      size_t count = 0U;

      for (start(); !completed(); next())
      {
        ++count;
      }

      return count;
    }

    //***************************************************************************
    /// Pure virtual functions.
    //***************************************************************************
    virtual void next() = 0;
    virtual void start() = 0;

  protected:

    //***************************************************************************
    /// Constructor
    //***************************************************************************
    imulti_loop()
      : has_completed(true)
      , inner(ETL_NULLPTR)
    {
    }

    //***************************************************************************
    /// Checks that there are no circular references by making sure that the
    /// inner loop does not link to this loop.
    //***************************************************************************
    bool is_valid(imulti_loop& inner_loop)
    {
      imulti_loop* loop = &inner_loop;

      while (loop != ETL_NULLPTR)
      {
        if (loop == this)
        {
          return false;
        }

        loop = loop->inner;
      }

      return true;
    }

    //***************************************************************************
    /// Set the inner loop of the last linked loop.
    //***************************************************************************
    void set_last(imulti_loop* next)
    {
      // Find the last loop.
      imulti_loop* loop = this;

      while (loop->inner != ETL_NULLPTR)
      {
        loop = loop->inner;
      }

      loop->inner = next;
    }

    bool has_completed;
    imulti_loop* inner;
  };

  //***************************************************************************
  /// multi_loop
  /// \tparam T The type to loop over.
  /// \tparam TStepper The type that implements operator() to step.
  /// Default = etl::increment, which calls operator ++() for the type.
  /// \tparam TContinue The type that implements operator() to test for loop continuation.
  /// Default = etl::not_equal_to, which calls operator !=() for the type.
  //***************************************************************************
  template <typename T, 
            typename TStepper  = etl::increment<T>, 
            typename TContinue = etl::not_equal_to<T> >
  class multi_loop : public imulti_loop
  {
  public:

    typedef T        value_type;
    typedef const T& const_reference;

    //***************************************************************************
    /// Constructor
    /// \param first The starting value of the loop.
    /// \param last  The terminating value of the loop. Equal to the last required value + 1.
    //***************************************************************************
    multi_loop(value_type first_, 
               value_type last_, 
               TStepper   stepper_       = TStepper(),
               TContinue  continue_loop_ = TContinue())
      : first(first_)
      , last(last_)
      , current(first_)
      , stepper(stepper_)
      , continue_loop(continue_loop_)
    {
    }

    //***************************************************************************
    /// Get a const reference to the starting value of the loop.
    //***************************************************************************
    const_reference begin()
    {
      return first;
    }

    //***************************************************************************
    /// Get a const reference to the terminating value of the loop.
    /// Equal to the last required value + 1.
    //***************************************************************************
    const_reference end()
    {
      return last;
    }

    //***************************************************************************
    /// Initialises the loops to the starting values.
    //***************************************************************************
    void start() ETL_OVERRIDE
    {
      if (inner != ETL_NULLPTR)
      {
        inner->start();
      }

      current = first;
      has_completed = !continue_loop(current, last); // Check for null loop.
    }

    //***************************************************************************
    /// Step to the next logical values in the loops.
    //***************************************************************************
    void next() ETL_OVERRIDE
    {
      has_completed = false;

      if (inner != ETL_NULLPTR)
      {
        inner->next();

        if (inner->completed())
        {
          has_completed = step();
        }
      }
      else
      {
        has_completed = step();
      }
    }

    //***************************************************************************
    /// Returns a const reference to the current loop value.
    //***************************************************************************
    const_reference value() const
    {
      return current;
    }

  private:

    //***************************************************************************
    /// Increments the current loop value.
    //***************************************************************************
    bool step()
    {
      current = stepper(current);

      const bool has_rolled_over = !continue_loop(current, last);

      if (has_rolled_over)
      {
        current = first;
      }

      return has_rolled_over;
    }
       
    value_type first;   ///< The first value of the loop.
    value_type last;    ///< The terminating value of the loop.
    value_type current; ///< The current value of the loop.

    TStepper  stepper;       ///< The type to step the value to the next.
    TContinue continue_loop; ///< The type to determine if the loop should continue.
  };
}

#undef ETL_FILE

#endif