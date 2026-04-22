/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 John Wellbelove

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

#ifndef ETL_ATOMIC_STD_INCLUDED
#define ETL_ATOMIC_STD_INCLUDED

#include "../platform.h"
#include "../char_traits.h"
#include "../nullptr.h"

#include <atomic>
#include <stdint.h>

namespace etl
{
  //***************************************************************************
  // ETL Atomic type for compilers that support std::atomic.
  // etl::atomic is a wrapper around std::atomic that adds fetch_max and
  // fetch_min if the standard library does not yet provide them.
  //***************************************************************************

#if ETL_HAS_STD_ATOMIC_MIN_MAX
  // The standard library already provides fetch_max/fetch_min.
  template <typename T>
  using atomic = std::atomic<T>;
#else
  template <typename T>
  class atomic : public std::atomic<T>
  {
    using base_type = std::atomic<T>;

  public:

    atomic() ETL_NOEXCEPT = default;

    ETL_CONSTEXPR atomic(T desired) ETL_NOEXCEPT
      : base_type(desired)
    {
    }

    atomic(const atomic&)                     = delete;
    atomic& operator=(const atomic&)          = delete;
    atomic& operator=(const atomic&) volatile = delete;

    using base_type::operator=;

    // Fetch max
    T fetch_max(T v, std::memory_order order = std::memory_order_seq_cst)
    {
      T old = this->load(order);

      while (v > old)
      {
        if (this->compare_exchange_weak(old, v, order))
        {
          break;
        }
      }

      return old;
    }

    T fetch_max(T v, std::memory_order order = std::memory_order_seq_cst) volatile
    {
      T old = this->load(order);

      while (v > old)
      {
        if (this->compare_exchange_weak(old, v, order))
        {
          break;
        }
      }

      return old;
    }

    // Fetch min
    T fetch_min(T v, std::memory_order order = std::memory_order_seq_cst)
    {
      T old = this->load(order);

      while (v < old)
      {
        if (this->compare_exchange_weak(old, v, order))
        {
          break;
        }
      }

      return old;
    }

    T fetch_min(T v, std::memory_order order = std::memory_order_seq_cst) volatile
    {
      T old = this->load(order);

      while (v < old)
      {
        if (this->compare_exchange_weak(old, v, order))
        {
          break;
        }
      }

      return old;
    }
  };
#endif

  using memory_order = std::memory_order;

  static ETL_CONSTANT etl::memory_order memory_order_relaxed = std::memory_order_relaxed;
  static ETL_CONSTANT etl::memory_order memory_order_consume = std::memory_order_consume;
  static ETL_CONSTANT etl::memory_order memory_order_acquire = std::memory_order_acquire;
  static ETL_CONSTANT etl::memory_order memory_order_release = std::memory_order_release;
  static ETL_CONSTANT etl::memory_order memory_order_acq_rel = std::memory_order_acq_rel;
  static ETL_CONSTANT etl::memory_order memory_order_seq_cst = std::memory_order_seq_cst;

  using atomic_bool    = etl::atomic<bool>;
  using atomic_char    = etl::atomic<char>;
  using atomic_schar   = etl::atomic<signed char>;
  using atomic_uchar   = etl::atomic<unsigned char>;
  using atomic_short   = etl::atomic<short>;
  using atomic_ushort  = etl::atomic<unsigned short>;
  using atomic_int     = etl::atomic<int>;
  using atomic_uint    = etl::atomic<unsigned int>;
  using atomic_long    = etl::atomic<long>;
  using atomic_ulong   = etl::atomic<unsigned long>;
  using atomic_llong   = etl::atomic<long long>;
  using atomic_ullong  = etl::atomic<unsigned long long>;
  using atomic_wchar_t = etl::atomic<wchar_t>;
#if ETL_HAS_NATIVE_CHAR8_T
  using atomic_char8_t = etl::atomic<char8_t>;
#endif
#if ETL_HAS_NATIVE_CHAR16_T
  using atomic_char16_t = etl::atomic<char16_t>;
#endif
#if ETL_HAS_NATIVE_CHAR32_T
  using atomic_char32_t = etl::atomic<char32_t>;
#endif
#if ETL_USING_8BIT_TYPES
  using atomic_uint8_t = etl::atomic<uint8_t>;
  using atomic_int8_t  = etl::atomic<int8_t>;
#endif
  using atomic_uint16_t = etl::atomic<uint16_t>;
  using atomic_int16_t  = etl::atomic<int16_t>;
  using atomic_uint32_t = etl::atomic<uint32_t>;
  using atomic_int32_t  = etl::atomic<int32_t>;
#if ETL_USING_64BIT_TYPES
  using atomic_uint64_t = etl::atomic<uint64_t>;
  using atomic_int64_t  = etl::atomic<int64_t>;
#endif
  using atomic_int_least8_t   = etl::atomic<int_least8_t>;
  using atomic_uint_least8_t  = etl::atomic<uint_least8_t>;
  using atomic_int_least16_t  = etl::atomic<int_least16_t>;
  using atomic_uint_least16_t = etl::atomic<uint_least16_t>;
  using atomic_int_least32_t  = etl::atomic<int_least32_t>;
  using atomic_uint_least32_t = etl::atomic<uint_least32_t>;
#if ETL_USING_64BIT_TYPES
  using atomic_int_least64_t  = etl::atomic<int_least64_t>;
  using atomic_uint_least64_t = etl::atomic<uint_least64_t>;
#endif
  using atomic_int_fast8_t   = etl::atomic<int_fast8_t>;
  using atomic_uint_fast8_t  = etl::atomic<uint_fast8_t>;
  using atomic_int_fast16_t  = etl::atomic<int_fast16_t>;
  using atomic_uint_fast16_t = etl::atomic<uint_fast16_t>;
  using atomic_int_fast32_t  = etl::atomic<int_fast32_t>;
  using atomic_uint_fast32_t = etl::atomic<uint_fast32_t>;
#if ETL_USING_64BIT_TYPES
  using atomic_int_fast64_t  = etl::atomic<int_fast64_t>;
  using atomic_uint_fast64_t = etl::atomic<uint_fast64_t>;
#endif
  using atomic_intptr_t  = etl::atomic<intptr_t>;
  using atomic_uintptr_t = etl::atomic<uintptr_t>;
  using atomic_size_t    = etl::atomic<size_t>;
  using atomic_ptrdiff_t = etl::atomic<ptrdiff_t>;
  using atomic_intmax_t  = etl::atomic<intmax_t>;
  using atomic_uintmax_t = etl::atomic<uintmax_t>;
} // namespace etl

#endif
