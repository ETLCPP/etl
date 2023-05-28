///******************************************************************************
//The MIT License(MIT)
//
//Embedded Template Library.
//https://github.com/ETLCPP/etl
//https://www.etlcpp.com
//
//Copyright(c) 2021 John Wellbelove
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//******************************************************************************/

#ifndef ETL_REFERENCE_COUNTED_OBJECT_INCLUDED
#define ETL_REFERENCE_COUNTED_OBJECT_INCLUDED

#include "platform.h"
#include "atomic.h"
#include "exception.h"
#include "error_handler.h"

#include <stdint.h>

namespace etl
{

  //***************************************************************************
  /// Exceptions for reference counting
  ///\ingroup reference_counting
  //***************************************************************************
  class reference_counting_exception : public etl::exception
  {
  public:
    reference_counting_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Reference counter overrun exception
  ///\ingroup reference_counting
  //***************************************************************************
  class reference_count_overrun : public etl::reference_counting_exception
  {
  public:
    reference_count_overrun(string_type file_name_, numeric_type line_number_)
      : etl::reference_counting_exception(ETL_ERROR_TEXT("reference_counting:overrun", ETL_REFERENCE_COUNTED_OBJECT_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The base of all reference counters.
  //***************************************************************************
  class ireference_counter
  {
  public:

    virtual ~ireference_counter() {};
    virtual void set_reference_count(int32_t value) = 0;
    virtual void increment_reference_count() = 0;
    ETL_NODISCARD virtual int32_t decrement_reference_count() = 0;
    ETL_NODISCARD virtual int32_t get_reference_count() const = 0;
  };

  //***************************************************************************
  /// A specific type of reference counter.
  //***************************************************************************
  template <typename TCounter>
  class reference_counter : public ireference_counter
  {
  public:

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    reference_counter()
      : reference_count(0)
    {
    }

    //***************************************************************************
    /// Set the reference count.
    //***************************************************************************
    virtual void set_reference_count(int32_t value) ETL_OVERRIDE
    {
      reference_count = value;
    }

    //***************************************************************************
    /// Increment the reference count.
    //***************************************************************************
    virtual void increment_reference_count() ETL_OVERRIDE
    {
      ++reference_count;
    }

    //***************************************************************************
    /// Decrement the reference count.
    //***************************************************************************
    ETL_NODISCARD virtual int32_t decrement_reference_count() ETL_OVERRIDE
    {
      ETL_ASSERT(reference_count > 0, ETL_ERROR(reference_count_overrun));

      return int32_t(--reference_count);
    }

    //***************************************************************************
    /// Get the current reference count.
    //***************************************************************************
    ETL_NODISCARD virtual int32_t get_reference_count() const ETL_OVERRIDE
    {
      return int32_t(reference_count);
    }

  private:

    TCounter reference_count; // The reference count object.
  };

  //***************************************************************************
  /// A specialisation for a counter type of void.
  //***************************************************************************
  template <>
  class reference_counter<void> : public ireference_counter
  {
  public:

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    reference_counter()
    {
      // Do nothing.
    }

    //***************************************************************************
    /// Set the reference count.
    //***************************************************************************
    virtual void set_reference_count(int32_t /*value*/) ETL_OVERRIDE
    {
      // Do nothing.
    }

    //***************************************************************************
    /// Increment the reference count.
    //***************************************************************************
    virtual void increment_reference_count() ETL_OVERRIDE
    {
      // Do nothing.
    }

    //***************************************************************************
    /// Decrement the reference count.
    //***************************************************************************
    ETL_NODISCARD virtual int32_t decrement_reference_count() ETL_OVERRIDE
    {
      return int32_t(1);
    }

    //***************************************************************************
    /// Get the current reference count.
    //***************************************************************************
    ETL_NODISCARD virtual int32_t get_reference_count() const ETL_OVERRIDE
    {
      return int32_t(1);
    }
  };

  //***************************************************************************
  /// Base for all reference counted objects.
  //***************************************************************************
  class ireference_counted_object
  {
  public:

    virtual ~ireference_counted_object() {}
    ETL_NODISCARD virtual ireference_counter& get_reference_counter() = 0;
    ETL_NODISCARD virtual const ireference_counter& get_reference_counter() const = 0;
  };

  //***************************************************************************
  /// Class for creating reference counted objects.
  /// \tparam TObject  The type to be reference counted.
  /// \tparam TCounter The type to use as the counter.
  //***************************************************************************
  template <typename TObject, typename TCounter>
  class reference_counted_object : public etl::ireference_counted_object
  {
  public:

    typedef TObject  value_type;
    typedef TCounter counter_type;

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    reference_counted_object()
    {
    }

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    reference_counted_object(const TObject& object_)
      : object(object_)
    {
    }

    //***************************************************************************
    /// Get a reference to the counted object.
    //***************************************************************************
    ETL_NODISCARD value_type& get_object()
    {
      return object;
    }


    //***************************************************************************
    /// Get a const reference to the counted object.
    //***************************************************************************
    ETL_NODISCARD const value_type& get_object() const
    {
      return object;
    }

    //***************************************************************************
    /// Get a reference to the reference counter.
    //***************************************************************************
    ETL_NODISCARD virtual ireference_counter& get_reference_counter() ETL_OVERRIDE
    {
      return reference_counter;
    }

    //***************************************************************************
    /// Get a const reference to the reference counter.
    //***************************************************************************
    ETL_NODISCARD virtual const ireference_counter& get_reference_counter() const ETL_OVERRIDE
    {
      return reference_counter;
    }

  private:

    // This class must not be copy constructed or assigned.
    reference_counted_object(const reference_counted_object&) ETL_DELETE;
    reference_counted_object& operator =(const reference_counted_object&) ETL_DELETE;
        
    TObject object;                                     ///< The object being reference counted.
    etl::reference_counter<TCounter> reference_counter; ///< The reference counter.
  };

#if ETL_USING_CPP11 && ETL_HAS_ATOMIC
  //***************************************************************************
  /// Class for creating reference counted objects using an atomic counter.
  /// \tparam TObject  The type to be reference counted.
  //***************************************************************************
  template <typename TObject>
  using atomic_counted_object = etl::reference_counted_object<TObject, etl::atomic_int32_t>;
#endif
}

#endif
