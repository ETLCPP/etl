/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

#ifndef ETL_VARIANT_POOL_INCLUDED
#define ETL_VARIANT_POOL_INCLUDED

#include "platform.h"
#include "largest.h"
#include "pool.h"
#include "static_assert.h"
#include "type_traits.h"

#include <stddef.h>

namespace etl
{
#if ETL_USING_CPP11 && !defined(ETL_VARIANT_POOL_FORCE_CPP03_IMPLEMENTATION)
  //***************************************************************************
  template <size_t MAX_SIZE_, typename... Ts>
  class variant_pool : public etl::generic_pool<etl::largest<Ts...>::size, etl::largest<Ts...>::alignment, MAX_SIZE_>
  {
  public:

    typedef etl::generic_pool<etl::largest<Ts...>::size, etl::largest<Ts...>::alignment, MAX_SIZE_> base_t;

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    variant_pool() {}

    //*************************************************************************
    /// Creates the object from a type. Variadic parameter constructor.
    //*************************************************************************
    template <typename T, typename... Args>
    T* create(Args&&... args)
    {
      ETL_STATIC_ASSERT((etl::is_one_of<T, Ts...>::value), "Unsupported type");

      return base_t::template create<T>(etl::forward<Args>(args)...);
    }

    //*************************************************************************
    /// Destroys the object.
    //*************************************************************************
    template <typename T>
    void destroy(const T* const p)
    {
      ETL_STATIC_ASSERT((etl::is_one_of<T, Ts...>::value || etl::is_base_of_any<T, Ts...>::value), "Invalid type");

      base_t::destroy(p);
    }

    //*************************************************************************
    /// Returns the maximum number of items in the variant_pool.
    //*************************************************************************
    size_t max_size() const
    {
      return MAX_SIZE;
    }

  private:

    variant_pool(const variant_pool&) ETL_DELETE;
    variant_pool& operator=(const variant_pool&) ETL_DELETE;
  };

  //***************************************************************************
  template <typename... Ts>
  class variant_pool_ext : public etl::generic_pool_ext<etl::largest<Ts...>::size, etl::largest<Ts...>::alignment>
  {
  public:

    typedef etl::generic_pool_ext<etl::largest<Ts...>::size, etl::largest<Ts...>::alignment> base_t;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    variant_pool_ext(typename base_t::element* buffer, size_t size)
      : base_t(buffer, size)
    {
    }

    //*************************************************************************
    /// Creates the object from a type. Variadic parameter constructor.
    //*************************************************************************
    template <typename T, typename... Args>
    T* create(Args&&... args)
    {
      ETL_STATIC_ASSERT((etl::is_one_of<T, Ts...>::value), "Unsupported type");

      return base_t::template create<T>(etl::forward<Args>(args)...);
    }

    //*************************************************************************
    /// Destroys the object.
    //*************************************************************************
    template <typename T>
    void destroy(const T* const p)
    {
      ETL_STATIC_ASSERT((etl::is_one_of<T, Ts...>::value || etl::is_base_of_any<T, Ts...>::value), "Invalid type");

      base_t::destroy(p);
    }

    //*************************************************************************
    /// Returns the maximum number of items in the variant_pool.
    //*************************************************************************
    size_t max_size() const
    {
      return base_t::max_size();
    }

  private:

    variant_pool_ext(const variant_pool_ext&) ETL_DELETE;
    variant_pool_ext& operator=(const variant_pool_ext&) ETL_DELETE;
  };
#else
  #include "private/variant_pool_cpp03.h"
#endif
} // namespace etl

#endif
