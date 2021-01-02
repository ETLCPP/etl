///******************************************************************************
//The MIT License(MIT)
//
//Embedded Template Library.
//https://github.com/ETLCPP/etl
//https://www.etlcpp.com
//
//Copyright(c) 2020 jwellbelove
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

#include <stdint.h>

#include "platform.h"
#include "atomic.h"

namespace etl
{
  //***************************************************************************
  /// The base of all reference counted objects.
  //***************************************************************************
  class ireference_counted_object
  {
  public:

    virtual ~ireference_counted_object() {};
    virtual void set_reference_count(uint32_t value) = 0;
    virtual void increment_reference_count() = 0;
    ETL_NODISCARD virtual uint32_t decrement_reference_count() = 0;
    ETL_NODISCARD virtual uint32_t get_reference_count() const = 0;
  };

  //***************************************************************************
  /// Class for creating reference counted objects.
  /// \tparam TObject  The type to be reference counted.
  /// \tparam TCounter The type to use as the counter.
  //***************************************************************************
  template <typename TObject, typename TCounter = void>
  class reference_counted_object : public etl::ireference_counted_object
  {
  public:

    typedef TObject  value_type;
    typedef TCounter counter_type;

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    reference_counted_object(const TObject& object_)
      : object(object_)
    {
      reference_count = 0;
    }
   
    //***************************************************************************
    /// Get a const reference to the counted object.
    //***************************************************************************
    const value_type& get_object() const
    {
      return object;
    }

    //***************************************************************************
    /// Set the reference count.
    //***************************************************************************
    virtual void set_reference_count(uint32_t value) ETL_OVERRIDE
    {
      reference_count = value;
    }

    //***************************************************************************
    /// Increment the refernce count.
    //***************************************************************************
    virtual void increment_reference_count() ETL_OVERRIDE
    {
      ++reference_count;
    }

    //***************************************************************************
    /// Decrement the refernce count.
    //***************************************************************************
    ETL_NODISCARD virtual uint32_t decrement_reference_count() ETL_OVERRIDE
    {
      return uint32_t(--reference_count);
    }

    //***************************************************************************
    /// Get the current reference count.
    //***************************************************************************
    ETL_NODISCARD virtual uint32_t get_reference_count() const ETL_OVERRIDE
    {
      return uint32_t(reference_count);
    }

  private:

    // This class must not be default contructed, copy constructed or assigned.
    reference_counted_object() ETL_DELETE;
    reference_counted_object(const reference_counted_object&) ETL_DELETE;
    reference_counted_object& operator =(const reference_counted_object&) ETL_DELETE;

    TCounter reference_count; // The reference counter
    const TObject object;     // The object being reference counted.
  };

#if ETL_HAS_ATOMIC
  //***************************************************************************
  /// Class for creating reference counted objects using an atomic counter.
  /// \tparam TObject  The type to be reference counted.
  //***************************************************************************
  template <typename TObject>
  class atomic_counted_object : public etl::reference_counted_object<TObject, etl::atomic_uint32_t>
  {
  public:

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    atomic_counted_object(const TObject& object_)
      : reference_counted_object<TObject, etl::atomic_uint32_t>(object_)
    {
    }

    typedef typename reference_counted_object<TObject, etl::atomic_uint32_t>::value_type   value_type;
    typedef typename reference_counted_object<TObject, etl::atomic_uint32_t>::counter_type counter_type;
  };
#endif

  //***************************************************************************
  /// Specialisation creating objects that have the ireference_counted_object interface
  /// but do not actually count.
  /// \tparam TObject  The type stored in the object.
  //***************************************************************************
  template <typename TObject>
  class reference_counted_object<TObject, void> : public etl::ireference_counted_object
  {
  public:

    typedef TObject  value_type;
    typedef void     counter_type;

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
    value_type& get_object()
    {
      return object;
    }

    //***************************************************************************
    /// Get a const reference to the counted object.
    //***************************************************************************
    const value_type& get_object() const
    {
      return object;
    }

    //***************************************************************************
    /// Set the reference count.
    //***************************************************************************
    virtual void set_reference_count(uint32_t value) ETL_OVERRIDE
    {
    }

    //***************************************************************************
    /// Increment the refernce count.
    //***************************************************************************
    virtual void increment_reference_count() ETL_OVERRIDE
    {
    }

    //***************************************************************************
    /// Decrement the refernce count.
    //***************************************************************************
    ETL_NODISCARD virtual uint32_t decrement_reference_count() ETL_OVERRIDE
    {
      return 1;
    }

    //***************************************************************************
    /// Get the current reference count.
    //***************************************************************************
    ETL_NODISCARD virtual uint32_t get_reference_count() const ETL_OVERRIDE
    {
      return 1;
    }

  private:

    TObject object; // The object being reference counted.
  };
}

#endif
