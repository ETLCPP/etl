///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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
  template <size_t VTypeSize, size_t VAlignment, size_t VSize>
  class generic_pool : public etl::ipool
  {
  public:

    static ETL_CONSTANT size_t SIZE      = VSize;
    static ETL_CONSTANT size_t ALIGNMENT = VAlignment;
    static ETL_CONSTANT size_t TYPE_SIZE = VTypeSize;

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    generic_pool()
      : etl::ipool(reinterpret_cast<char*>(&buffer[0]), Element_Size, VSize)
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
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= VAlignment, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= VTypeSize, "Type too large for pool");
      return ipool::allocate<U>();
    }

#if ETL_CPP11_NOT_SUPPORTED || ETL_POOL_CPP03_CODE || ETL_USING_STLPORT
    //*************************************************************************
    /// Allocate storage for an object from the pool and create with default.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a null pointer is returned.
    //*************************************************************************
    template <typename U>
    U* create()
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= VAlignment, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= VTypeSize, "Type too large for pool");
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
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= VAlignment, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= VTypeSize, "Type too large for pool");
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
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= VAlignment, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= VTypeSize, "Type too large for pool");
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
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= VAlignment, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= VTypeSize, "Type too large for pool");
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
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= VAlignment, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= VTypeSize, "Type too large for pool");
      return ipool::create<U>(value1, value2, value3, value4);
    }
#else
    //*************************************************************************
    /// Emplace with variadic constructor parameters.
    //*************************************************************************
    template <typename U, typename... Args>
    U* create(Args&&... args)
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= VAlignment, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= VTypeSize, "Type too large for pool");
      return ipool::create<U>(etl::forward<Args>(args)...);
    }
#endif

    //*************************************************************************
    /// Destroys the object.
    /// Undefined behaviour if the pool does not contain a 'U'.
    /// \param p_object A pointer to the object to be destroyed.
    //*************************************************************************
    template <typename U>
    void destroy(const U* const p_object)
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= VAlignment, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= VTypeSize, "Type too large for pool");
      p_object->~U();
      ipool::release(p_object);
    }

  private:

    // The pool element.
    union Element
    {
      char* next;              ///< Pointer to the next free element.
      char      value[VTypeSize]; ///< Storage for value type.
      typename  etl::type_with_alignment<VAlignment>::type dummy; ///< Dummy item to get correct alignment.
    };

    ///< The memory for the pool of objects.
    typename etl::aligned_storage<sizeof(Element), etl::alignment_of<Element>::value>::type buffer[VSize];

    static ETL_CONSTANT uint32_t Element_Size = sizeof(Element);

    // Should not be copied.
    generic_pool(const generic_pool&) ETL_DELETE;
    generic_pool& operator =(const generic_pool&) ETL_DELETE;
  };

  template <size_t VTypeSize, size_t VAlignment, size_t VSize>
  ETL_CONSTANT size_t generic_pool<VTypeSize, VAlignment, VSize>::SIZE;
  
  template <size_t VTypeSize, size_t VAlignment, size_t VSize>
  ETL_CONSTANT size_t generic_pool<VTypeSize, VAlignment, VSize>::ALIGNMENT;
  
  template <size_t VTypeSize, size_t VAlignment, size_t VSize>
  ETL_CONSTANT size_t generic_pool<VTypeSize, VAlignment, VSize>::TYPE_SIZE;

  //*************************************************************************
  /// A templated abstract pool implementation that uses a fixed size pool.
  /// The storage for the pool is supplied externally.
  ///\ingroup pool
  //*************************************************************************
  template <size_t VTypeSize, size_t VAlignment>
  class generic_pool_ext : public etl::ipool 
  {
  private:
    // The pool element.
    union element_internal 
    {
      char* next;                                                 ///< Pointer to the next free element.
      char value[VTypeSize];                                      ///< Storage for value type.
      typename etl::type_with_alignment<VAlignment>::type dummy;  ///< Dummy item to get correct alignment.
    };

    static const size_t ELEMENT_INTERNAL_SIZE = sizeof(element_internal);

  public:
    static ETL_CONSTANT size_t ALIGNMENT = VAlignment;
    static ETL_CONSTANT size_t TYPE_SIZE = VTypeSize;

    typedef typename etl::aligned_storage<sizeof(element_internal), etl::alignment_of<element_internal>::value>::type element;

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    generic_pool_ext(element* buffer, size_t size) 
      : etl::ipool(reinterpret_cast<char*>(&buffer[0]), ELEMENT_INTERNAL_SIZE, size) 
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
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= VAlignment, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= VTypeSize, "Type too large for pool");
      return ipool::allocate<U>();
    }

#if ETL_CPP11_NOT_SUPPORTED || ETL_POOL_CPP03_CODE || ETL_USING_STLPORT
    //*************************************************************************
    /// Allocate storage for an object from the pool and create with default.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a null pointer is returned.
    //*************************************************************************
    template <typename U>
    U* create()
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= VAlignment, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= VTypeSize, "Type too large for pool");
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
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= VAlignment, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= VTypeSize, "Type too large for pool");
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
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= VAlignment, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= VTypeSize, "Type too large for pool");
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
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= VAlignment, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= VTypeSize, "Type too large for pool");
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
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= VAlignment, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= VTypeSize, "Type too large for pool");
      return ipool::create<U>(value1, value2, value3, value4);
    }
#else
    //*************************************************************************
    /// Emplace with variadic constructor parameters.
    //*************************************************************************
    template <typename U, typename... Args>
    U* create(Args&&... args)
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= VAlignment, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= VTypeSize, "Type too large for pool");
      return ipool::create<U>(etl::forward<Args>(args)...);
    }
#endif

    //*************************************************************************
    /// Destroys the object.
    /// Undefined behaviour if the pool does not contain a 'U'.
    /// \param p_object A pointer to the object to be destroyed.
    //*************************************************************************
    template <typename U>
    void destroy(const U* const p_object)
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= VAlignment, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= VTypeSize, "Type too large for pool");
      p_object->~U();
      ipool::release(p_object);
    }

  private:
    // Should not be copied.
    generic_pool_ext(const generic_pool_ext&) ETL_DELETE;
    generic_pool_ext& operator=(const generic_pool_ext&) ETL_DELETE;
  };

  template <size_t VTypeSize, size_t VAlignment>
  ETL_CONSTANT size_t generic_pool_ext<VTypeSize, VAlignment>::ALIGNMENT;

  template <size_t VTypeSize, size_t VAlignment>
  ETL_CONSTANT size_t generic_pool_ext<VTypeSize, VAlignment>::TYPE_SIZE;
}

#endif

