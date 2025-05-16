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

#ifndef ETL_IPOOL_INCLUDED
#define ETL_IPOOL_INCLUDED

#include "platform.h"
#include "error_handler.h"
#include "exception.h"
#include "iterator.h"
#include "static_assert.h"
#include "utility.h"
#include "memory.h"
#include "placement_new.h"

#define ETL_POOL_CPP03_CODE 0

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
      : pool_exception(ETL_ERROR_TEXT("pool:allocation", ETL_POOL_FILE_ID"A"), file_name_, line_number_)
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
      : pool_exception(ETL_ERROR_TEXT("pool:not in pool", ETL_POOL_FILE_ID"B"), file_name_, line_number_)
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
      : pool_exception(ETL_ERROR_TEXT("pool:element size", ETL_POOL_FILE_ID"C"), file_name_, line_number_)
    {}
  };

  //***************************************************************************
  ///\ingroup pool
  //***************************************************************************
  class ipool
  {
  public:

    typedef size_t size_type;

  private:

    //***************************************************************************
    /// Iterator helper functions
    //***************************************************************************

    const char* buffer_end() const
    {
      return p_buffer + Item_Size * items_initialised;
    }

    //***************************************************************************
    /// Optimization: a candidate for being in free list, but no guarantee
    /// if false, cannot be part of free list, and therefore is allocated
    /// if true, possibly in free list, but still needs to be checked via free list
    //***************************************************************************
    bool is_pointing_into_pool_or_end_or_nullptr(const char *address) const
    {
      return address == ETL_NULLPTR || (p_buffer <= address && address <= buffer_end());
    }

    //***************************************************************************
    /// Iterate free list to confirm specified address is included or not
    //***************************************************************************
    bool is_in_free_list(const char* address) const
    {
      const char* i = p_next;
      while (i != ETL_NULLPTR)
      {
        if (address == i)
        {
          return true;
        }
        i = *reinterpret_cast<const char* const*>(i);
      }
      return false;
    }

  public:

    //***************************************************************************
    template<bool is_const>
    class ipool_iterator
    {
    public:

      friend class ipool;

      typedef typename etl::conditional<is_const, const char*, char*>::type         value_type;
      typedef typename etl::conditional<is_const, const char*&, char*&>::type       reference;
      typedef typename etl::conditional<is_const, const char**, char**>::type       pointer;
      typedef ptrdiff_t                                                             difference_type;
      typedef ETL_OR_STD::forward_iterator_tag                                      iterator_category;
      typedef typename etl::conditional<is_const, const void*, void*>::type         void_type;
      typedef typename etl::conditional<is_const, const ipool, ipool>::type         pool_type;
      typedef typename etl::conditional<is_const, const char* const*, char**>::type pointer_type;

      //***************************************************************************
      ipool_iterator(const ipool_iterator& other)
        : p_current(other.p_current)
        , p_pool(other.p_pool)
      {
        find_allocated();
      }

      //***************************************************************************
      ipool_iterator& operator ++()
      {
        p_current = p_current + p_pool->Item_Size;
        find_allocated();
        return *this;
      }

      //***************************************************************************
      ipool_iterator operator ++(int)
      {
        ipool_iterator temp(*this);
        p_current = p_current + p_pool->Item_Size;
        find_allocated();
        return temp;
      }

      //***************************************************************************
      ipool_iterator& operator =(const ipool_iterator& other)
      {
        p_current = other.p_current;
        p_pool = other.p_pool;
        return *this;
      }

      //***************************************************************************
      void_type operator *() const
      {
        return p_current;
      }

      //***************************************************************************
      template <typename T>
      T& get() const
      {
        return *reinterpret_cast<T*>(p_current);
      }

      //***************************************************************************
      friend bool operator == (const ipool_iterator& lhs, const ipool_iterator& rhs)
      {
        return lhs.p_current == rhs.p_current;
      }

      //***************************************************************************  
      friend bool operator != (const ipool_iterator& lhs, const ipool_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      //***************************************************************************
      /// Find allocated item by increasing p_current, starting at p_current.
      /// Leave p_current at buffer_end() if no further allocated item found.
      //***************************************************************************
      void find_allocated()
      {
        while (p_current < p_pool->buffer_end())
        {
          value_type value = *reinterpret_cast<pointer_type>(p_current);
          if (!p_pool->is_pointing_into_pool_or_end_or_nullptr(value))
          {
            return;
          }
          if (!p_pool->is_in_free_list(p_current))
          {
            return;
          }
          p_current += p_pool->Item_Size;
        }
      }

      //***************************************************************************
      /// Constructor
      //***************************************************************************
      ipool_iterator(value_type p, pool_type* pool_)
        : p_current(p)
        , p_pool(pool_)
      {
        find_allocated();
      }

      value_type p_current;
      pool_type* p_pool;
    };

    template<bool is_const>
    friend class ipool_iterator;

    typedef ipool_iterator<false> iterator;

    //***************************************************************************
    class const_iterator : public ipool_iterator<true>
    {
    public:
      const_iterator(const ipool_iterator& other) : ipool_iterator(other) {}
      const_iterator(const ipool_iterator<false>& other) : ipool_iterator(other.p_current, other.p_pool) {}
      const_iterator(value_type p, pool_type* pool_) : ipool_iterator<true>(p, pool_) {}
    };

    //***************************************************************************
    iterator begin()
    {
      return iterator(p_buffer, this);
    }

    //***************************************************************************
    iterator end()
    {
      return iterator(p_buffer + Item_Size * items_initialised, this);
    }

    //***************************************************************************
    const_iterator begin() const
    {
      return const_iterator(p_buffer, this);
    }

    //***************************************************************************
    const_iterator end() const
    {
      return const_iterator(p_buffer + Item_Size * items_initialised, this);
    }

    //***************************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(p_buffer, this);
    }

    //***************************************************************************
    const_iterator cend() const
    {
      return const_iterator(p_buffer + Item_Size * items_initialised, this);
    }

    //*************************************************************************
    /// Allocate storage for an object from the pool.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a null pointer is returned.
    //*************************************************************************
    template <typename T>
    T* allocate()
    {
      if (sizeof(T) > Item_Size)
      {
        ETL_ASSERT(false, ETL_ERROR(etl::pool_element_size));
      }

      return reinterpret_cast<T*>(allocate_item());
    }

#if ETL_CPP11_NOT_SUPPORTED || ETL_POOL_CPP03_CODE || ETL_USING_STLPORT
    //*************************************************************************
    /// Allocate storage for an object from the pool and create default.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a null pointer is returned.
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
    /// etl::pool_no_allocation if thrown, otherwise a null pointer is returned.
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
        ::new (p) T(etl::forward<Args>(args)...);
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
    void destroy(const T* const p_object)
    {
      if (sizeof(T) > Item_Size)
      {
        ETL_ASSERT(false, ETL_ERROR(etl::pool_element_size));
      }

      p_object->~T();
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
      const uintptr_t p = uintptr_t(p_object);
      release_item((char*)p);
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
    bool is_in_pool(const void* const p_object) const
    {
      const uintptr_t p = uintptr_t(p_object);
      return is_item_in_pool((const char*)p);
    }

    //*************************************************************************
    /// Returns the maximum number of items in the pool.
    //*************************************************************************
    size_t max_size() const
    {
      return Max_Size;
    }

    //*************************************************************************
    /// Returns the maximum size of an item in the pool.
    //*************************************************************************
    size_t max_item_size() const
    {
      return Item_Size;
    }

    //*************************************************************************
    /// Returns the maximum number of items in the pool.
    //*************************************************************************
    size_t capacity() const
    {
      return Max_Size;
    }

    //*************************************************************************
    /// Returns the number of free items in the pool.
    //*************************************************************************
    size_t available() const
    {
      return Max_Size - items_allocated;
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
      return items_allocated == Max_Size;
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
      Item_Size(item_size_),
      Max_Size(max_size_)
    {
    }

  private:
    
    static ETL_CONSTANT uintptr_t invalid_item_ptr = 1;

    //*************************************************************************
    /// Allocate an item from the pool.
    //*************************************************************************
    char* allocate_item()
    {
      char* p_value = ETL_NULLPTR;

      // Any free space left?
      if (items_allocated < Max_Size)
      {
        // Initialise another one if necessary.
        if (items_initialised < Max_Size)
        {
          char* p = p_buffer + (items_initialised * Item_Size);
          char* np = p + Item_Size;
          *reinterpret_cast<char**>(p) = np;
          ++items_initialised;
        }

        // Get the address of new allocated item.
        p_value = p_next;

        ++items_allocated;
        if (items_allocated < Max_Size)
        {
          // Set up the pointer to the next free item
          p_next = *reinterpret_cast<char**>(p_next);
        }
        else
        {
          // No more left!
          p_next = ETL_NULLPTR;
        }

        // invalid pointer, outside pool
        // needs to be different from ETL_NULLPTR since ETL_NULLPTR is used
        // as list endmarker
        *reinterpret_cast<uintptr_t*>(p_value) = invalid_item_ptr;
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

      if (items_allocated > 0) 
      {
        // Point it to the current free item.
        *(uintptr_t*)p_value = reinterpret_cast<uintptr_t>(p_next);

        p_next = p_value;

        --items_allocated;
      }
      else 
      {
        ETL_ASSERT_FAIL(ETL_ERROR(pool_no_allocation));
      }
    }

    //*************************************************************************
    /// Check if the item belongs to this pool.
    //*************************************************************************
    bool is_item_in_pool(const char* p) const
    {
      // Within the range of the buffer?
      intptr_t distance = p - p_buffer;
      bool is_within_range = (distance >= 0) && (distance <= intptr_t((Item_Size * Max_Size) - Item_Size));

      // Modulus and division can be slow on some architectures, so only do this in debug.
#if ETL_IS_DEBUG_BUILD
      // Is the address on a valid object boundary?
      bool is_valid_address = ((distance % Item_Size) == 0);
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

    const uint32_t Item_Size;    ///< The size of allocated items.
    const uint32_t Max_Size;     ///< The maximum number of objects that can be allocated.

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
}

#endif

