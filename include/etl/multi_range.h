///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 John Wellbelove

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

namespace etl
{
  //***************************************************************************
  /// Exception for the multi_range.
  //***************************************************************************
  class multi_range_exception : public etl::exception
  {
  public:

    multi_range_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Circular reference exception.
  //***************************************************************************
  class multi_range_circular_reference : public etl::multi_range_exception
  {
  public:

    multi_range_circular_reference(string_type file_name_, numeric_type line_number_)
      : etl::multi_range_exception(ETL_ERROR_TEXT("multi_range:circular reference", ETL_MULTI_LOOP_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The base class for multi_range.
  //***************************************************************************
  class imulti_range
  {
  public:

    //***************************************************************************
    /// Insert after this range.
    //***************************************************************************
    imulti_range& insert(imulti_range& inner_range)
    {
      ETL_ASSERT(is_valid(inner_range), ETL_ERROR(multi_range_circular_reference));

      // Remember what the next range was.
      imulti_range* next = inner;

      // Append the new range
      inner = &inner_range;

      // Link to the original next range.
      inner_range.set_last(next);

      return *this;
    }

    //***************************************************************************
    /// Append to the most inner range.
    //***************************************************************************
    imulti_range& append(imulti_range& inner_range)
    {
      ETL_ASSERT(is_valid(inner_range), ETL_ERROR(multi_range_circular_reference));

      if (inner != ETL_NULLPTR)
      {
        inner->append(inner_range);
      }
      else
      {
        inner = &inner_range;
      }

      return *this;
    }

    //***************************************************************************
    /// Unlinks this range from its inner.
    //***************************************************************************
    void detach()
    {
      inner = ETL_NULLPTR;
    }

    //***************************************************************************
    /// Unlinks this and all inner ranges.
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
    /// Gets the total number of ranges, from this range inclusive.
    //***************************************************************************
    size_t number_of_ranges() const
    {
      size_t count = 1UL;

      imulti_range* p_range = inner;

      while (p_range != ETL_NULLPTR)
      {
        ++count;
        p_range = p_range->inner;
      }

      return count;
    }

    //***************************************************************************
    /// Gets the total number of iterations over all ranges, from this range inclusive.
    //***************************************************************************
    size_t number_of_iterations()
    {
      size_t count = 0UL;

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
    imulti_range()
      : has_completed(true)
      , inner(ETL_NULLPTR)
    {
    }

    //***************************************************************************
    /// Checks that there are no circular references.
    //***************************************************************************
    bool is_valid(imulti_range& inner_range)
    {
      imulti_range* range = &inner_range;

      while (range != ETL_NULLPTR)
      {
        if (range == this)
        {
          return false;
        }

        range = range->inner;
      }

      return true;
    }

    //***************************************************************************
    /// Set the inner range of the last linked range.
    //***************************************************************************
    void set_last(imulti_range* next)
    {
      // Find the last range.
      imulti_range* range = this;

      while (range->inner != ETL_NULLPTR)
      {
        range = range->inner;
      }

      range->inner = next;
    }

    bool has_completed;
    imulti_range* inner;
  };

  //***************************************************************************
  /// multi_range
  /// \tparam T The type to range over.
  //***************************************************************************
  template <typename T>
  class multi_range : public imulti_range
  {
  public:

    typedef T        value_type;
    typedef const T& const_reference;

    //***************************************************************************
    ///
    //***************************************************************************
    struct step_type
    {
      typedef T value_type;

      virtual void operator()(value_type& value) = 0;
    };

    //***************************************************************************
    ///
    //***************************************************************************
    struct forward_step : public step_type
    {
      typedef T value_type;

      virtual void operator()(value_type& value)
      {
        ++value;
      }
    };

    //***************************************************************************
    ///
    //***************************************************************************
    struct forward_step_by : public step_type
    {
      typedef T value_type;

      explicit forward_step_by(const value_type& step_value_)
        : step_value(step_value_)
      {
      }

      virtual void operator()(value_type& value)
      {
        value += step_value;
      }

      const value_type step_value;
    };

    //***************************************************************************
    ///
    //***************************************************************************
    struct reverse_step : public step_type
    {
      typedef T value_type;

      virtual void operator()(value_type& value)
      {
        --value;
      }
    };

    //***************************************************************************
    ///
    //***************************************************************************
    struct reverse_step_by : public step_type
    {
      typedef T value_type;

      explicit reverse_step_by(const value_type& step_value_)
        : step_value(step_value_)
      {
      }

      virtual void operator()(value_type& value)
      {
        value -= step_value;
      }

      const value_type step_value;
    };

    //***************************************************************************
    ///
    //***************************************************************************
    struct compare_type
    {
      typedef T value_type;

      virtual bool operator()(const value_type& current, const value_type& last) const = 0;
    };

    //***************************************************************************
    ///
    //***************************************************************************
    struct not_equal_compare : public compare_type
    {
      typedef T value_type;

      virtual bool operator()(const value_type& current, const value_type& last) const ETL_OVERRIDE
      {
        return etl::not_equal_to<value_type>()(current, last);
      }
    };

    //***************************************************************************
    ///
    //***************************************************************************
    struct less_than_compare : public compare_type
    {
      typedef T value_type;

      virtual bool operator()(const value_type& current, const value_type& last) const ETL_OVERRIDE
      {
        return etl::less<value_type>()(current, last);
      }
    };

    //***************************************************************************
    ///
    //***************************************************************************
    struct greater_than_compare : public compare_type
    {
      typedef T value_type;

      virtual bool operator()(const value_type& current, const value_type& last) const ETL_OVERRIDE
      {
        return etl::greater<value_type>()(current, last);
      }
    };

    //***************************************************************************
    /// Constructor
    /// \param first The starting value of the range.
    /// \param last  The terminating value of the range. Equal to the last required value + 1.
    //***************************************************************************
    multi_range(value_type first_,
                value_type last_)
      : first(first_)
      , last(last_)
      , current(first_)
      , p_stepper(&default_stepper)
      , p_compare(&default_compare)
    {
    }

    //***************************************************************************
    /// Constructor
    /// \param first The starting value of the range.
    /// \param last  The terminating value of the range. Equal to the last required value + 1.
    //***************************************************************************
    multi_range(value_type first_,
                value_type last_,
                step_type& stepper_)
      : first(first_)
      , last(last_)
      , current(first_)
      , p_stepper(&stepper_)
      , p_compare(&default_compare)
    {
    }

    //***************************************************************************
    /// Constructor
    /// \param first The starting value of the range.
    /// \param last  The terminating value of the range.
    //***************************************************************************
    multi_range(value_type    first_,
                value_type    last_,
                compare_type& compare_)
      : first(first_)
      , last(last_)
      , current(first_)
      , p_stepper(&default_stepper)
      , p_compare(&compare_)
    {
    }

    //***************************************************************************
    /// Constructor
    /// \param first The starting value of the range.
    /// \param last  The terminating value of the range. Equal to the last required value + 1.
    //***************************************************************************
    multi_range(value_type    first_,
                value_type    last_,
                step_type&    stepper_,
                compare_type& compare_)
      : first(first_)
      , last(last_)
      , current(first_)
      , p_stepper(&stepper_)
      , p_compare(&compare_)
    {
    }

    //***************************************************************************
    /// Get a const reference to the starting value of the range.
    //***************************************************************************
    const_reference begin()
    {
      return first;
    }

    //***************************************************************************
    /// Get a const reference to the terminating value of the range.
    /// Equal to the last required value + 1.
    //***************************************************************************
    const_reference end()
    {
      return last;
    }

    //***************************************************************************
    /// Initialises the ranges to the starting values.
    //***************************************************************************
    void start() ETL_OVERRIDE
    {
      if (inner != ETL_NULLPTR)
      {
        inner->start();
      }

      current = first;
      has_completed = !(*p_compare)(current, last); // Check for null range.
    }

    //***************************************************************************
    /// Step to the next logical values in the ranges.
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
    /// Returns a const reference to the current range value.
    //***************************************************************************
    const_reference value() const
    {
      return current;
    }

  private:

    //***************************************************************************
    /// Increments the current range value.
    //***************************************************************************
    bool step()
    {
      (*p_stepper)(current);

      const bool has_rolled_over = !(*p_compare)(current, last);

      if (has_rolled_over)
      {
        current = first;
      }

      return has_rolled_over;
    }

    multi_range() ETL_DELETE;
    multi_range(const multi_range&) ETL_DELETE;
    multi_range& operator =(const multi_range&) ETL_DELETE;

    value_type first;   ///< The first value of the range.
    value_type last;    ///< The terminating value of the range.
    value_type current; ///< The current value of the range.

    step_type*   p_stepper;
    forward_step default_stepper;

    compare_type*     p_compare;
    not_equal_compare default_compare;
  };
}

#endif
