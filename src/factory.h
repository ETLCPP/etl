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

#ifndef __ETL_FACTORY__
#define __ETL_FACTORY__

#include <stdint.h>

#include "error_handler.h"
#include "exception.h"
#include "largest.h"
#include "pool.h"
#include "type_traits.h"
#include "alignment.h"
#include "static_assert.h"

#undef ETL_FILE
#define ETL_FILE "40"

namespace etl
{
  //***************************************************************************
  class factory_exception : public etl::exception
  {
  public:

    factory_exception(string_type what, string_type file_name, numeric_type line_number)
      : exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  class factory_cannot_create : public etl::factory_exception
  {
  public:

    factory_cannot_create(string_type file_name, numeric_type line_number)
      : factory_exception(ETL_ERROR_TEXT("factory:cannot create", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  class factory_did_not_create : public etl::factory_exception
  {
  public:

    factory_did_not_create(string_type file_name, numeric_type line_number)
      : factory_exception(ETL_ERROR_TEXT("factory:did not create", ETL_FILE"B"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  template <const size_t MAX_ITEMS_,
            typename TBase,
            typename T1,         typename T2  = void, typename T3  = void, typename T4  = void,
            typename T5  = void, typename T6  = void, typename T7  = void, typename T8  = void,
            typename T9  = void, typename T10 = void, typename T11 = void, typename T12 = void,
            typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
  class factory
  {
  public:

    static const size_t MAX_ITEMS = MAX_ITEMS_;

    //*************************************************************************
    /// Creates the object. Default constructor.
    //*************************************************************************
    template <typename T>
    T* create()
    {
      STATIC_ASSERT((etl::is_one_of<T, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>::value), "Unsupported type");

      ETL_ASSERT(!full(), ETL_ERROR(etl::factory_cannot_create));

      T* p = pool.template allocate<T>();
      new (p) T();

      return p;
    }

    //*************************************************************************
    /// Creates the object. One parameter constructor.
    //*************************************************************************
    template <typename T, typename TP1>
    T* create(const TP1& p1)
    {
      STATIC_ASSERT((etl::is_one_of<T, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>::value), "Unsupported type");

      ETL_ASSERT(!full(), ETL_ERROR(etl::factory_cannot_create));

      T* p = pool.template allocate<T>();
      new (p) T(p1);

      return p;
    }

    //*************************************************************************
    /// Creates the object. Two parameter constructor.
    //*************************************************************************
    template <typename T, typename TP1, typename TP2>
    T* create(const TP1& p1, const TP2& p2)
    {
      STATIC_ASSERT((etl::is_one_of<T, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>::value), "Unsupported type");

      ETL_ASSERT(!full(), ETL_ERROR(etl::factory_cannot_create));

      T* p = pool.template allocate<T>();
      new (p) T(p1, p2);

      return p;
    }

    //*************************************************************************
    /// Creates the object. Three parameter constructor.
    //*************************************************************************
    template <typename T, typename TP1, typename TP2, typename TP3>
    T* create(const TP1& p1, const TP2& p2, const TP3& p3)
    {
      STATIC_ASSERT((etl::is_one_of<T, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>::value), "Unsupported type");

      ETL_ASSERT(!full(), ETL_ERROR(etl::factory_cannot_create));

      T* p = pool.template allocate<T>();
      new (p) T(p1, p2, p3);

      return p;
    }

    //*************************************************************************
    /// Creates the object. Four parameter constructor.
    //*************************************************************************
    template <typename T, typename TP1, typename TP2, typename TP3, typename TP4>
    T* create(const TP1& p1, const TP2& p2, const TP3& p3, const TP4& p4)
    {
      STATIC_ASSERT((etl::is_one_of<T, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>::value), "Unsupported type");

      ETL_ASSERT(!full(), ETL_ERROR(etl::factory_cannot_create));

      T* p = pool.template allocate<T>();
      new (p) T(p1, p2, p3, p4);

      return p;
    }

    //*************************************************************************
    /// Destroys the object.
    //*************************************************************************
    template <typename T>
    void destroy(T* p)
    {
      STATIC_ASSERT((etl::is_one_of<T, TBase, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>::value), "Unsupported type");

      if (pool.is_in_pool(p))
      {
        p->~T();
        pool.release(p);
      }
      else
      {
        ETL_ASSERT(false, ETL_ERROR(etl::factory_did_not_create));
      }
    }

    //*************************************************************************
    /// Returns the maximum number of items in the factory.
    //*************************************************************************
    size_t max_items() const
    {
      return pool.max_items();
    }

    //*************************************************************************
    /// Returns the number of free items in the factory.
    //*************************************************************************
    size_t available() const
    {
      return pool.available();
    }

    //*************************************************************************
    /// Returns the number of allocated items in the factory.
    //*************************************************************************
    size_t size() const
    {
      return pool.size();
    }

    //*************************************************************************
    /// Checks to see if there are no allocated items in the factory.
    /// \return <b>true</b> if there are none allocated.
    //*************************************************************************
    bool empty() const
    {
      return pool.empty();
    }

    //*************************************************************************
    /// Checks to see if there are no free items in the factory.
    /// \return <b>true</b> if there are none free.
    //*************************************************************************
    bool full() const
    {
      return pool.full();
    }

  private:

    // The pool element.
    union element
    {
      char     value[etl::largest<T1, T2, T3, T4, T5, T6, T7, T8>::size]; ///< Storage for value type.
      typename etl::type_with_alignment<etl::largest<T1, T2, T3, T4, T5, T6, T7, T8>::alignment>::type dummy; ///< Dummy item to get correct alignment.
    };

    etl::pool<element, MAX_ITEMS_> pool;
  };
}

#undef ETL_FILE

#endif
