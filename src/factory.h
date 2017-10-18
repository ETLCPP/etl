/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

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

#include "platform.h"
#include "error_handler.h"
#include "exception.h"
#include "largest.h"
#include "type_traits.h"
#include "alignment.h"
#include "static_assert.h"
#include "type_lookup.h"

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
  template <const size_t MAX_SIZE_,
            typename T1,
            typename T2  = etl::type_id_pair<>, 
            typename T3  = etl::type_id_pair<>, 
            typename T4  = etl::type_id_pair<>,
            typename T5  = etl::type_id_pair<>,
            typename T6  = etl::type_id_pair<>,
            typename T7  = etl::type_id_pair<>,
            typename T8  = etl::type_id_pair<>,
            typename T9  = etl::type_id_pair<>,
            typename T10 = etl::type_id_pair<>,
            typename T11 = etl::type_id_pair<>,
            typename T12 = etl::type_id_pair<>,
            typename T13 = etl::type_id_pair<>,
            typename T14 = etl::type_id_pair<>,
            typename T15 = etl::type_id_pair<>,
            typename T16 = etl::type_id_pair<>>
  class factory
  {
  private:

    typedef typename T1::type  TT1;
    typedef typename T2::type  TT2;
    typedef typename T3::type  TT3;
    typedef typename T4::type  TT4;
    typedef typename T5::type  TT5;
    typedef typename T6::type  TT6;
    typedef typename T7::type  TT7;
    typedef typename T8::type  TT8;
    typedef typename T9::type  TT9;
    typedef typename T10::type TT10;
    typedef typename T11::type TT11;
    typedef typename T12::type TT12;
    typedef typename T13::type TT13;
    typedef typename T14::type TT14;
    typedef typename T15::type TT15;
    typedef typename T16::type TT16;

    typedef etl::type_id_lookup<T1, T2,  T3,  T4,  T5,  T6,  T7,  T8,
                                T9, T10, T11, T12, T13, T14, T15, T16> lookup_t;

  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    factory()
      : p_next(reinterpret_cast<char*>(&buffer[0])),
        items_allocated(0),
        items_initialised(0)
    {
    }

    //*************************************************************************
    /// Creates the object. Default constructor.
    //*************************************************************************
    template <typename T>
    T* create_from_type()
    {
      STATIC_ASSERT((etl::is_one_of<T, TT1, TT2, TT3, TT4, TT5, TT6, TT7, TT8, TT9, TT10, TT11, TT12, TT13, TT14, TT15, TT16>::value), "Unsupported type");

      T* p = reinterpret_cast<T*>(allocate_item());

      if (p != nullptr)
      {
        new (p) T();
      }

      return p;
    }

    //*************************************************************************
    /// Creates the object. One parameter constructor.
    //*************************************************************************
    template <typename T, typename TP1>
    T* create_from_type(const TP1& p1)
    {
      STATIC_ASSERT((etl::is_one_of<T, TT1, TT2, TT3, TT4, TT5, TT6, TT7, TT8, TT9, TT10, TT11, TT12, TT13, TT14, TT15, TT16>::value), "Unsupported type");

      T* p = reinterpret_cast<T*>(allocate_item());

      if (p != nullptr)
      {
        new (p) T(p1);
      }

      return p;
    }

    //*************************************************************************
    /// Creates the object. Two parameter constructor.
    //*************************************************************************
    template <typename T, typename TP1, typename TP2>
    T* create_from_type(const TP1& p1, const TP2& p2)
    {
      STATIC_ASSERT((etl::is_one_of<T, TT1, TT2, TT3, TT4, TT5, TT6, TT7, TT8, TT9, TT10, TT11, TT12, TT13, TT14, TT15, TT16>::value), "Unsupported type");

      T* p = reinterpret_cast<T*>(allocate_item());

      if (p != nullptr)
      {
        new (p) T(p1, p2);
      }

      return p;
    }

    //*************************************************************************
    /// Creates the object. Three parameter constructor.
    //*************************************************************************
    template <typename T, typename TP1, typename TP2, typename TP3>
    T* create_from_type(const TP1& p1, const TP2& p2, const TP3& p3)
    {
      STATIC_ASSERT((etl::is_one_of<T, TT1, TT2, TT3, TT4, TT5, TT6, TT7, TT8, TT9, TT10, TT11, TT12, TT13, TT14, TT15, TT16>::value), "Unsupported type");

      T* p = reinterpret_cast<T*>(allocate_item());

      if (p != nullptr)
      {
        new (p) T(p1, p2, p3);
      }

      return p;
    }

    //*************************************************************************
    /// Creates the object. Four parameter constructor.
    //*************************************************************************
    template <typename T, typename TP1, typename TP2, typename TP3, typename TP4>
    T* create_from_type(const TP1& p1, const TP2& p2, const TP3& p3, const TP4& p4)
    {
      STATIC_ASSERT((etl::is_one_of<T, TT1, TT2, TT3, TT4, TT5, TT6, TT7, TT8, TT9, TT10, TT11, TT12, TT13, TT14, TT15, TT16>::value), "Unsupported type");

      T* p = reinterpret_cast<T*>(allocate_item());

      if (p != nullptr)
      {
        new (p) T(p1, p2, p3, p4);
      }

      return p;
    }

    //*************************************************************************
    /// Creates the object from an index. Default constructor.
    //*************************************************************************
    template <size_t ID>
    typename lookup_t::template type_from_id<ID>::type* create_from_id()
    {
      typedef typename lookup_t::template type_from_id<ID>::type type;
      STATIC_ASSERT((!etl::is_same<void, type>::value), "Invalid index");
      return create_from_type<type>();
    }

    //*************************************************************************
    /// Creates the object from an index. One parameter constructor.
    //*************************************************************************
    template <size_t ID, typename TP1>
    typename lookup_t::template type_from_id<ID>::type* create_from_id(const TP1& p1)
    {
      typedef typename lookup_t::template type_from_id<ID>::type type;
      STATIC_ASSERT((!etl::is_same<void, type>::value), "Invalid index");
      return create_from_type<type>(p1);
    }

    //*************************************************************************
    /// Creates the object from an index. Two parameter constructor.
    //*************************************************************************
    template <size_t ID, typename TP1, typename TP2>
    typename lookup_t::template type_from_id<ID>::type* create_from_id(const TP1& p1, const TP2& p2)
    {
      typedef typename lookup_t::template type_from_id<ID>::type type;
      STATIC_ASSERT((!etl::is_same<void, type>::value), "Invalid index");
      return create_from_type<type>(p1, p2);
    }

    //*************************************************************************
    /// Creates the object from an index. Three parameter constructor.
    //*************************************************************************
    template <size_t ID, typename TP1, typename TP2, typename TP3>
    typename lookup_t::template type_from_id<ID>::type* create_from_id(const TP1& p1, const TP2& p2, const TP3& p3)
    {
      typedef typename lookup_t::template type_from_id<ID>::type type;
      STATIC_ASSERT((!etl::is_same<void, type>::value), "Invalid index");
      return create_from_type<type>(p1, p2, p3);
    }

    //*************************************************************************
    /// Creates the object from an index. Three parameter constructor.
    //*************************************************************************
    template <size_t ID, typename TP1, typename TP2, typename TP3, typename TP4>
    typename lookup_t::template type_from_id<ID>::type* create_from_id(const TP1& p1, const TP2& p2, const TP3& p3, const TP4& p4)
    {
      typedef typename lookup_t::template type_from_id<ID>::type type;
      STATIC_ASSERT((!etl::is_same<void, type>::value), "Invalid index");
      return create_from_type<type>(p1, p2, p3, p4);
    }

    //*************************************************************************
    /// Destroys the object.
    //*************************************************************************
    template <typename T>
    bool destroy(const T* const p)
    {
      STATIC_ASSERT((etl::is_one_of<T, TT1, TT2, TT3, TT4, TT5, TT6, TT7, TT8, TT9, TT10, TT11, TT12, TT13, TT14, TT15, TT16>::value ||
                     etl::is_base_of<T, TT1>::value  || 
                     etl::is_base_of<T, TT2>::value  || 
                     etl::is_base_of<T, TT3>::value  || 
                     etl::is_base_of<T, TT4>::value  ||
                     etl::is_base_of<T, TT5>::value  || 
                     etl::is_base_of<T, TT6>::value  || 
                     etl::is_base_of<T, TT7>::value  || 
                     etl::is_base_of<T, TT8>::value  || 
                     etl::is_base_of<T, TT9>::value  || 
                     etl::is_base_of<T, TT10>::value || 
                     etl::is_base_of<T, TT11>::value || 
                     etl::is_base_of<T, TT12>::value || 
                     etl::is_base_of<T, TT13>::value || 
                     etl::is_base_of<T, TT14>::value || 
                     etl::is_base_of<T, TT15>::value || 
                     etl::is_base_of<T, TT16>::value), "Invalid type");
  
      p->~T();

      return release_item(reinterpret_cast<char*>(const_cast<T*>(p)));
    }

    //*************************************************************************
    /// Returns the maximum number of items in the factory.
    //*************************************************************************
    size_t max_size() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Returns the number of free items in the factory.
    //*************************************************************************
    size_t available() const
    {
      return MAX_SIZE - items_allocated;
    }

    //*************************************************************************
    /// Returns the number of allocated items in the factory.
    //*************************************************************************
    size_t size() const
    {
      return items_allocated;
    }

    //*************************************************************************
    /// Checks to see if there are no allocated items in the factory.
    /// \return <b>true</b> if there are none allocated.
    //*************************************************************************
    bool empty() const
    {
      return items_allocated == 0;
    }

    //*************************************************************************
    /// Checks to see if there are no free items in the factory.
    /// \return <b>true</b> if there are none free.
    //*************************************************************************
    bool full() const
    {
      return items_allocated == MAX_SIZE;;
    }

  private:

    factory(const factory&);
    factory& operator =(const factory&);

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
          uintptr_t p = reinterpret_cast<uintptr_t>(reinterpret_cast<char*>(&buffer[0]) + (items_initialised * ITEM_SIZE));
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
        ETL_ASSERT(false, ETL_ERROR(etl::factory_cannot_create));
      }

      return p_value;
    }

    //*************************************************************************
    /// Release an item back to the pool.
    //*************************************************************************
    bool release_item(char* p_value)
    {
      if (is_item_in_pool(p_value))
      {
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

        return true;
      }
      else
      {
        ETL_ASSERT(false, ETL_ERROR(factory_did_not_create));
      }

      return false;
    }

    //*************************************************************************
    /// Check if the item belongs to this pool.
    //*************************************************************************
    bool is_item_in_pool(char* p) const
    {
      if (empty() || (p == nullptr))
      {
        return false;
      }

      // Within the range of the buffer?
      intptr_t distance = p - reinterpret_cast<const char*>(&buffer[0]);
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

    // The pool element.
    union Element
    {
      uintptr_t next; ///< Pointer to the next free element.
      char      value[etl::largest<TT1, TT2, TT3, TT4, TT5, TT6, TT7, TT8, TT9, TT10, TT11, TT12, TT13, TT14, TT15, TT16>::size]; ///< Storage for value type.
      typename  etl::type_with_alignment<etl::largest<TT1, TT2, TT3, TT4, TT5, TT6, TT7, TT8, TT9, TT10, TT11, TT12, TT13, TT14, TT15, TT16>::alignment>::type dummy; ///< Dummy item to get correct alignment.
    };

    ///< The memory for the pool of objects.
    typename etl::aligned_storage<sizeof(Element), etl::alignment_of<Element>::value>::type buffer[MAX_SIZE];

    static const uint32_t ITEM_SIZE = sizeof(Element);

    char* p_next;

    uint32_t items_allocated;   ///< The number of items allocated.
    uint32_t items_initialised; ///< The number of items initialised.
  };
}

#undef ETL_FILE

#endif
