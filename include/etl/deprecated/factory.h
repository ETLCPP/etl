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
#include <utility>

#include "platform.h"
#include "error_handler.h"
#include "exception.h"
#include "largest.h"
#include "type_traits.h"
#include "alignment.h"
#include "static_assert.h"
#include "type_lookup.h"
#include "pool.h"

#if defined(ETL_COMPILER_GCC)
  #warning THIS CLASS IS DEPRECATED!USE VARIANT_POOL INSTEAD.
#elif defined(ETL_COMPILER_MICROSOFT)
  #pragma message ("THIS CLASS IS DEPRECATED! USE VARIANT_POOL INSTEAD.")
#endif

#undef ETL_FILE
#define ETL_FILE "40"

namespace etl
{
  //***************************************************************************
  class factory_exception : public etl::exception
  {
  public:

    factory_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  class factory_cannot_create : public etl::factory_exception
  {
  public:

    factory_cannot_create(string_type file_name_, numeric_type line_number_)
      : factory_exception(ETL_ERROR_TEXT("factory:cannot create", ETL_FILE"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  class factory_did_not_create : public etl::factory_exception
  {
  public:

    factory_did_not_create(string_type file_name_, numeric_type line_number_)
      : factory_exception(ETL_ERROR_TEXT("factory:did not create", ETL_FILE"B"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  template <const size_t MAX_SIZE_,
            typename T1,
            typename T2  = etl::type_id_pair<etl::null_type, -2>,
            typename T3  = etl::type_id_pair<etl::null_type, -3>,
            typename T4  = etl::type_id_pair<etl::null_type, -4>,
            typename T5  = etl::type_id_pair<etl::null_type, -5>,
            typename T6  = etl::type_id_pair<etl::null_type, -6>,
            typename T7  = etl::type_id_pair<etl::null_type, -7>,
            typename T8  = etl::type_id_pair<etl::null_type, -8>,
            typename T9  = etl::type_id_pair<etl::null_type, -9>,
            typename T10 = etl::type_id_pair<etl::null_type, -10>,
            typename T11 = etl::type_id_pair<etl::null_type, -11>,
            typename T12 = etl::type_id_pair<etl::null_type, -12>,
            typename T13 = etl::type_id_pair<etl::null_type, -13>,
            typename T14 = etl::type_id_pair<etl::null_type, -14>,
            typename T15 = etl::type_id_pair<etl::null_type, -15>,
            typename T16 = etl::type_id_pair<etl::null_type, -16> >
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
    /// Default constructor.
    //*************************************************************************
    factory()
    {
    }

#if !ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Creates the object. Default constructor.
    //*************************************************************************
    template <typename T>
    T* create_from_type()
    {
      STATIC_ASSERT((etl::is_one_of<T, TT1, TT2, TT3, TT4, TT5, TT6, TT7, TT8, TT9, TT10, TT11, TT12, TT13, TT14, TT15, TT16>::value), "Unsupported type");

      T* p = nullptr;

      if (pool.full())
      {
        ETL_ASSERT(false, ETL_ERROR(etl::factory_cannot_create));
      }
      else
      {
        p = pool.template allocate<T>();

        if (p != nullptr)
        {
          new (p) T();
        }
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

      T* p = nullptr;

      if (pool.full())
      {
        ETL_ASSERT(false, ETL_ERROR(etl::factory_cannot_create));
      }
      else
      {
        p = pool.template allocate<T>();

        if (p != nullptr)
        {
          new (p) T(p1);
        }
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

      T* p = nullptr;

      if (pool.full())
      {
        ETL_ASSERT(false, ETL_ERROR(etl::factory_cannot_create));
      }
      else
      {
        p = pool.template allocate<T>();

        if (p != nullptr)
        {
          new (p) T(p1, p2);
        }
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

      T* p = nullptr;

      if (pool.full())
      {
        ETL_ASSERT(false, ETL_ERROR(etl::factory_cannot_create));
      }
      else
      {
        p = pool.template allocate<T>();

        if (p != nullptr)
        {
          new (p) T(p1, p2, p3);
        }
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

      T* p = nullptr;

      if (pool.full())
      {
        ETL_ASSERT(false, ETL_ERROR(etl::factory_cannot_create));
      }
      else
      {
        p = pool.template allocate<T>();

        if (p != nullptr)
        {
          new (p) T(p1, p2, p3, p4);
        }
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
#else
    //*************************************************************************
    /// Creates the object from a type. Variadic parameter constructor.
    //*************************************************************************
    template <typename T, typename... Args>
    T* create_from_type(Args&&... args)
    {
      STATIC_ASSERT((etl::is_one_of<T, TT1, TT2, TT3, TT4, TT5, TT6, TT7, TT8, TT9, TT10, TT11, TT12, TT13, TT14, TT15, TT16>::value), "Unsupported type");

      T* p = nullptr;

      if (pool.full())
      {
        ETL_ASSERT(false, ETL_ERROR(etl::factory_cannot_create));
      }
      else
      {
        p = pool.template allocate<T>();

        if (p != nullptr)
        {
          new (p) T(std::forward<Args>(args)...);
        }
      }

      return p;
    }

    //*************************************************************************
    /// Creates the object from an index. Variadic parameter constructor.
    //*************************************************************************
    template <size_t ID, typename... Args>
    typename lookup_t::template type_from_id<ID>::type* create_from_id(Args&&... args)
    {
      typedef typename lookup_t::template type_from_id<ID>::type type;
      STATIC_ASSERT((!etl::is_same<void, type>::value), "Invalid index");
      return create_from_type<type>(std::forward<Args>(args)...);
    }
#endif

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

      void* vp = reinterpret_cast<char*>(const_cast<T*>(p));

      if (pool.is_in_pool(vp))
      {
        pool.release(vp);
        return true;
      }
      else
      {
        ETL_ASSERT(false, ETL_ERROR(factory_did_not_create));
        return false;
      }
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

    factory(const factory&);
    factory& operator =(const factory&);

    // The pool.
    etl::generic_pool<etl::largest<TT1, TT2, TT3, TT4, TT5, TT6, TT7, TT8, TT9, TT10, TT11, TT12, TT13, TT14, TT15, TT16>::size,
                      etl::largest<TT1, TT2, TT3, TT4, TT5, TT6, TT7, TT8, TT9, TT10, TT11, TT12, TT13, TT14, TT15, TT16>::alignment,
                      MAX_SIZE> pool;
  };
}

#undef ETL_FILE

#endif
