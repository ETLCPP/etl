///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#ifndef ETL_POOL_INCLUDED
#define ETL_POOL_INCLUDED

#include "platform.h"

#include <new>

#include "stl/algorithm.h"
#include "stl/iterator.h"

#include "error_handler.h"
#include "alignment.h"
#include "array.h"
#include "container.h"
#include "integral_limits.h"
#include "nullptr.h"
#include "alignment.h"
#include "static_assert.h"
#include "algorithm.h"

#undef ETL_FILE
#define ETL_FILE "11"

#define ETL_POOL_CPP03_CODE 0

//*****************************************************************************
///\defgroup pool pool
/// A fixed capacity pool.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// The base class for pool exceptions.
  ///\ingroup pool
  //***************************************************************************
  class pool_exception : public exception
  {
  public:

    pool_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {}
  };

  //***************************************************************************
  /// The exception thrown when the pool has no more free items.
  ///\ingroup pool
  //***************************************************************************
  class pool_no_allocation : public pool_exception
  {
  public:

    explicit pool_no_allocation(string_type file_name_, numeric_type line_number_)
      : pool_exception(ETL_ERROR_TEXT("pool:allocation", ETL_FILE"A"), file_name_, line_number_)
    {}
  };

  //***************************************************************************
  /// The exception thrown when an object is released which does not belong to the pool.
  ///\ingroup pool
  //***************************************************************************
  class pool_object_not_in_pool : public pool_exception
  {
  public:

    pool_object_not_in_pool(string_type file_name_, numeric_type line_number_)
      : pool_exception(ETL_ERROR_TEXT("pool:not in pool", ETL_FILE"B"), file_name_, line_number_)
    {}
  };

  //***************************************************************************
  /// The exception thrown when an the type requested is larger than the element size.
  ///\ingroup pool
  //***************************************************************************
  class pool_element_size : public pool_exception
  {
  public:

    pool_element_size(string_type file_name_, numeric_type line_number_)
      : pool_exception(ETL_ERROR_TEXT("pool:element size", ETL_FILE"C"), file_name_, line_number_)
    {}
  };

  //***************************************************************************
  ///\ingroup pool
  //***************************************************************************
  class ipool
  {
  public:

    typedef size_t size_type;

    //*************************************************************************
    /// Allocate storage for an object from the pool.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
    //*************************************************************************
    template <typename T>
    T* allocate()
    {
      if (sizeof(T) > ITEM_SIZE)
      {
        ETL_ASSERT(false, ETL_ERROR(etl::pool_element_size));
      }

      return reinterpret_cast<T*>(allocate_item());
    }

#if !ETL_CPP11_SUPPORTED || ETL_POOL_CPP03_CODE || defined(ETL_STLPORT) || defined(ETL_NO_STL)
    //*************************************************************************
    /// Allocate storage for an object from the pool and create default.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
    //*************************************************************************
    template <typename T>
    T* create()
    {
      T* p = allocate<T>();

      if (p)
      {
        ::new (p) T();
      }

      return p;
    }

    //*************************************************************************
    /// Allocate storage for an object from the pool and create with 1 parameter.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
    //*************************************************************************
    template <typename T, typename T1>
    T* create(const T1& value1)
    {
      T* p = allocate<T>();

      if (p)
      {
        ::new (p) T(value1);
      }

      return p;
    }

    template <typename T, typename T1, typename T2>
    T* create(const T1& value1, const T2& value2)
    {
      T* p = allocate<T>();

      if (p)
      {
        ::new (p) T(value1, value2);
      }

      return p;
    }

    template <typename T, typename T1, typename T2, typename T3>
    T* create(const T1& value1, const T2& value2, const T3& value3)
    {
      T* p = allocate<T>();

      if (p)
      {
        ::new (p) T(value1, value2, value3);
      }

      return p;
    }

    template <typename T, typename T1, typename T2, typename T3, typename T4>
    T* create(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      T* p = allocate<T>();

      if (p)
      {
        ::new (p) T(value1, value2, value3, value4);
      }

      return p;
    }
#else
    //*************************************************************************
    /// Emplace with variadic constructor parameters.
    //*************************************************************************
    template <typename T, typename... Args>
    T* create(Args&&... args)
    {
      T* p = allocate<T>();

      if (p)
      {
        ::new (p) T(std::forward<Args>(args)...);
      }

      return p;
    }
#endif

    //*************************************************************************
    /// Destroys the object.
    /// Undefined behaviour if the pool does not contain a 'T'.
    /// \param p_object A pointer to the object to be destroyed.
    //*************************************************************************
    template <typename T>
    void destroy(const void* const p_object)
    {
      if (sizeof(T) > ITEM_SIZE)
      {
        ETL_ASSERT(false, ETL_ERROR(etl::pool_element_size));
      }

      reinterpret_cast<T*>((const_cast<void*>(p_object)))->~T();
      release(p_object);
    }

    //*************************************************************************
    /// Release an object in the pool.
    /// If asserts or exceptions are enabled and the object does not belong to this
    /// pool then an etl::pool_object_not_in_pool is thrown.
    /// \param p_object A pointer to the object to be released.
    //*************************************************************************
    void release(const void* const p_object)
    {
      release_item((char*)p_object);
    }

    //*************************************************************************
    /// Release all objects in the pool.
    //*************************************************************************
    void release_all()
    {
      items_allocated = 0;
      items_initialised = 0;
      p_next = p_buffer;
    }

    //*************************************************************************
    /// Check to see if the object belongs to the pool.
    /// \param p_object A pointer to the object to be checked.
    /// \return <b>true<\b> if it does, otherwise <b>false</b>
    //*************************************************************************
    //template <typename T>
    bool is_in_pool(const void* p_object) const
    {
      return is_item_in_pool((const char*)p_object);
    }

    //*************************************************************************
    /// Returns the maximum number of items in the pool.
    //*************************************************************************
    size_t max_size() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Returns the number of free items in the pool.
    //*************************************************************************
    size_t available() const
    {
      return MAX_SIZE - items_allocated;
    }

    //*************************************************************************
    /// Returns the number of allocated items in the pool.
    //*************************************************************************
    size_t size() const
    {
      return items_allocated;
    }

    //*************************************************************************
    /// Checks to see if there are no allocated items in the pool.
    /// \return <b>true</b> if there are none allocated.
    //*************************************************************************
    bool empty() const
    {
      return items_allocated == 0;
    }

    //*************************************************************************
    /// Checks to see if there are no free items in the pool.
    /// \return <b>true</b> if there are none free.
    //*************************************************************************
    bool full() const
    {
      return items_allocated == MAX_SIZE;
    }

  protected:

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    ipool(char* p_buffer_, uint32_t item_size_, uint32_t max_size_)
      : p_buffer(p_buffer_),
        p_next(p_buffer_),
        items_allocated(0),
        items_initialised(0),
        ITEM_SIZE(item_size_),
        MAX_SIZE(max_size_)
    {
    }

  private:

    //*************************************************************************
    /// Allocate an item from the pool.
    //*************************************************************************
    char* allocate_item()
    {
      char* p_value = nullptr;

      // Any free space left?
      if (items_allocated < MAX_SIZE)
      {
        // Initialise another one if necessary.
        if (items_initialised < MAX_SIZE)
        {
          uintptr_t p = reinterpret_cast<uintptr_t>(p_buffer + (items_initialised * ITEM_SIZE));
          *reinterpret_cast<uintptr_t*>(p) = p + ITEM_SIZE;
          ++items_initialised;
        }

        // Get the address of new allocated item.
        p_value = p_next;

        ++items_allocated;
        if (items_allocated != MAX_SIZE)
        {
          // Set up the pointer to the next free item
          p_next = *reinterpret_cast<char**>(p_next);
        }
        else
        {
          // No more left!
          p_next = nullptr;
        }
      }
      else
      {
        ETL_ASSERT(false, ETL_ERROR(pool_no_allocation));
      }

      return p_value;
    }

    //*************************************************************************
    /// Release an item back to the pool.
    //*************************************************************************
    void release_item(char* p_value)
    {
      // Does it belong to us?
      ETL_ASSERT(is_item_in_pool(p_value), ETL_ERROR(pool_object_not_in_pool));

      if (p_next != nullptr)
      {
        // Point it to the current free item.
        *(uintptr_t*)p_value = reinterpret_cast<uintptr_t>(p_next);
      }
      else
      {
        // This is the only free item.
        *((uintptr_t*)p_value) = 0;
      }

      p_next = p_value;

      --items_allocated;
    }

    //*************************************************************************
    /// Check if the item belongs to this pool.
    //*************************************************************************
    bool is_item_in_pool(const char* p) const
    {
      // Within the range of the buffer?
      intptr_t distance = p - p_buffer;
      bool is_within_range = (distance >= 0) && (distance <= intptr_t((ITEM_SIZE * MAX_SIZE) - ITEM_SIZE));

      // Modulus and division can be slow on some architectures, so only do this in debug.
#if defined(ETL_DEBUG)
      // Is the address on a valid object boundary?
      bool is_valid_address = ((distance % ITEM_SIZE) == 0);
#else
      bool is_valid_address = true;
#endif

      return is_within_range && is_valid_address;
    }

    // Disable copy construction and assignment.
    ipool(const ipool&);
    ipool& operator =(const ipool&);

    char* p_buffer;
    char* p_next;

    uint32_t  items_allocated;   ///< The number of items allocated.
    uint32_t  items_initialised; ///< The number of items initialised.

    const uint32_t ITEM_SIZE;    ///< The size of allocated items.
    const uint32_t MAX_SIZE;    ///< The maximum number of objects that can be allocated.

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_POOL) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~ipool()
    {
    }
#else
  protected:
    ~ipool()
    {
    }
#endif
  };

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
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
    /// Static asserts if the specified type is too large for the pool.
    //*************************************************************************
    template <typename U>
    U* allocate()
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT_, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      return ipool::allocate<U>();
    }

#if !ETL_CPP11_SUPPORTED || ETL_POOL_CPP03_CODE || defined(ETL_STLPORT) || defined(ETL_NO_STL)
    //*************************************************************************
    /// Allocate storage for an object from the pool and create with default.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
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
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
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
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
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
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
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
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
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
      return ipool::create<U>(std::forward<Args>(args)...);
    }
#endif

    //*************************************************************************
    /// Destroys the object.
    /// Undefined behaviour if the pool does not contain a 'U'.
    /// \param p_object A pointer to the object to be destroyed.
    //*************************************************************************
    template <typename U>
    void destroy(const void* const p_object)
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT_, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      reinterpret_cast<U*>((const_cast<void*>(p_object)))->~U();
      ipool::release(p_object);
    }

  private:

    // The pool element.
    union Element
    {
      uintptr_t next;              ///< Pointer to the next free element.
      char      value[TYPE_SIZE_]; ///< Storage for value type.
      typename  etl::type_with_alignment<ALIGNMENT_>::type dummy; ///< Dummy item to get correct alignment.
    };

    ///< The memory for the pool of objects.
    typename etl::aligned_storage<sizeof(Element), etl::alignment_of<Element>::value>::type buffer[SIZE];

    static const uint32_t ELEMENT_SIZE = sizeof(Element);

    // Should not be copied.
    generic_pool(const generic_pool&);
    generic_pool& operator =(const generic_pool&);
  };

  //*************************************************************************
  /// A templated pool implementation that uses a fixed size pool.
  ///\ingroup pool
  //*************************************************************************
  template <typename T, const size_t SIZE_>
  class pool : public etl::generic_pool<sizeof(T), etl::alignment_of<T>::value, SIZE_>
  {
  private:

    typedef etl::generic_pool<sizeof(T), etl::alignment_of<T>::value, SIZE_> base_t;

  public:

    static const size_t SIZE      = base_t::SIZE;
    static const size_t ALIGNMENT = base_t::ALIGNMENT;
    static const size_t TYPE_SIZE = base_t::TYPE_SIZE;

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    pool()
    {
    }

    //*************************************************************************
    /// Allocate an object from the pool.
    /// Uses the default constructor.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
    /// Static asserts if the specified type is too large for the pool.
    //*************************************************************************
    template <typename U>
    U* allocate()
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      return base_t::template allocate<U>();
    }

#if !ETL_CPP11_SUPPORTED || ETL_POOL_CPP03_CODE || defined(ETL_STLPORT) || defined(ETL_NO_STL)
    //*************************************************************************
    /// Allocate storage for an object from the pool and create with default.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
    //*************************************************************************
    template <typename U>
    U* create()
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      return base_t::template create<U>();
    }

    //*************************************************************************
    /// Allocate storage for an object from the pool and create with 1 parameter.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
    //*************************************************************************
    template <typename U, typename T1>
    U* create(const T1& value1)
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      return base_t::template create<U>(value1);
    }

    //*************************************************************************
    /// Allocate storage for an object from the pool and create with 2 parameters.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
    //*************************************************************************
    template <typename U, typename T1, typename T2>
    U* create(const T1& value1, const T2& value2)
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      return base_t::template create<U>(value1, value2);
    }

    //*************************************************************************
    /// Allocate storage for an object from the pool and create with 3 parameters.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
    //*************************************************************************
    template <typename U, typename T1, typename T2, typename T3>
    U* create(const T1& value1, const T2& value2, const T3& value3)
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      return base_t::template create<U>(value1, value2, value3);
    }

    //*************************************************************************
    /// Allocate storage for an object from the pool and create with 4 parameters.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
    //*************************************************************************
    template <typename U, typename T1, typename T2, typename T3, typename T4>
    U* create(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      return base_t::template create<U>(value1, value2, value3, value4);
    }
#else
    //*************************************************************************
    /// Allocate storage for an object from the pool and create with variadic parameters.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
    //*************************************************************************
    template <typename U, typename... Args>
    U* create(Args&&... args)
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      return base_t::template create<U>(std::forward<Args>(args)...);
    }
#endif

    //*************************************************************************
    /// Destroys the object.
    /// Undefined behaviour if the pool does not contain a 'U'.
    /// \param p_object A pointer to the object to be destroyed.
    //*************************************************************************
    template <typename U>
    void destroy(const void* const p_object)
    {
      ETL_STATIC_ASSERT(etl::alignment_of<U>::value <= ALIGNMENT, "Type has incompatible alignment");
      ETL_STATIC_ASSERT(sizeof(U) <= TYPE_SIZE, "Type too large for pool");
      reinterpret_cast<U*>((const_cast<void*>(p_object)))->~U();
      base_t::release(p_object);
    }

  private:

    // Should not be copied.
    pool(const pool&);
    pool& operator =(const pool&);
  };
}

#undef ETL_FILE

#endif

