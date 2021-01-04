///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 jwellbelove

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

#ifndef ETL_GENERIC_POOL_INCLUDED
#define ETL_GENERIC_POOL_INCLUDED

#include "platform.h"
#include "ipool.h"
#include "type_traits.h"
#include "static_assert.h"
#include "alignment.h"

#define ETL_POOL_CPP03_CODE 0

//*****************************************************************************
///\defgroup pool pool
/// A fixed capacity pool.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //*************************************************************************
  /// A templated abstract pool implementation that uses a fixed size pool.
  ///\ingroup pool
  //*************************************************************************
  template <const size_t TYPE_SIZE_, const size_t ALIGNMENT_, const size_t SIZE_>
  class generic_pool : public etl::ipool
  {
  public:

    static const size_t SIZE      = SIZE_;
    static const size_t ALIGNMENT = ALIGNMENT_;
    static const size_t TYPE_SIZE = TYPE_SIZE_;

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    generic_pool()
      : etl::ipool(reinterpret_cast<char*>(&buffer[0]), ELEMENT_SIZE, SIZE)
    {
    }

    //*************************************************************************
    /// Allocate an object from the pool.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a null pointer is returned.
    /// Static asserts if the specified type is too large for the pool.
    //*************************************************************************
    template <typename U>
    U* allocate()
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT_, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      return ipool::allocate<U>();
    }

#if !ETL_CPP11_SUPPORTED || ETL_POOL_CPP03_CODE || defined(ETL_STLPORT)
    //*************************************************************************
    /// Allocate storage for an object from the pool and create with default.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a null pointer is returned.
    //*************************************************************************
    template <typename U>
    U* create()
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT_, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      return ipool::create<U>();
    }

    //*************************************************************************
    /// Allocate storage for an object from the pool and create with 1 parameter.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a null pointer is returned.
    //*************************************************************************
    template <typename U, typename T1>
    U* create(const T1& value1)
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT_, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      return ipool::create<U>(value1);
    }

    //*************************************************************************
    /// Allocate storage for an object from the pool and create with 2 parameters.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a null pointer is returned.
    //*************************************************************************
    template <typename U, typename T1, typename T2>
    U* create(const T1& value1, const T2& value2)
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT_, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      return ipool::create<U>(value1, value2);
    }

    //*************************************************************************
    /// Allocate storage for an object from the pool and create with 3 parameters.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a null pointer is returned.
    //*************************************************************************
    template <typename U, typename T1, typename T2, typename T3>
    U* create(const T1& value1, const T2& value2, const T3& value3)
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT_, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      return ipool::create<U>(value1, value2, value3);
    }

    //*************************************************************************
    /// Allocate storage for an object from the pool and create with 4 parameters.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a null pointer is returned.
    //*************************************************************************
    template <typename U, typename T1, typename T2, typename T3, typename T4>
    U* create(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT_, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      return ipool::create<U>(value1, value2, value3, value4);
    }
#else
    //*************************************************************************
    /// Emplace with variadic constructor parameters.
    //*************************************************************************
    template <typename U, typename... Args>
    U* create(Args&&... args)
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT_, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      return ipool::create<U>(etl::forward<Args>(args)...);
    }
#endif

    //*************************************************************************
    /// Releases the object.
    /// \param p_object A pointer to the object to be destroyed.
    //*************************************************************************
    template <typename U>
    void release(const U* const p_object)
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT_, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      ipool::release(p_object);
    }

    //*************************************************************************
    /// Destroys the object.
    /// \param p_object A pointer to the object to be destroyed.
    //*************************************************************************
    template <typename U>
    void destroy(const U* const p_object)
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT_, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      ipool::destroy(p_object);
    }

  private:

    // The pool element.
    union Element
    {
      char*     next;              ///< Pointer to the next free element.
      char      value[TYPE_SIZE_]; ///< Storage for value type.
      typename  etl::type_with_alignment<ALIGNMENT_>::type dummy; ///< Dummy item to get correct alignment.
    };

    ///< The memory for the pool of objects.
    typename etl::aligned_storage<sizeof(Element), etl::alignment_of<Element>::value>::type buffer[SIZE];

    static ETL_CONSTANT uint32_t ELEMENT_SIZE = sizeof(Element);

    // Should not be copied.
    generic_pool(const generic_pool&) ETL_DELETE;
    generic_pool& operator =(const generic_pool&) ETL_DELETE;
  };
}

#endif

