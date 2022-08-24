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
#include "../nullptr.h"
#include "../char_traits.h"

#include <atomic>
#include <stdint.h>

namespace etl
{
  //***************************************************************************
  // ETL Atomic type for compilers that support std::atomic.
  // etl::atomic is a simple wrapper around std::atomic.
  //***************************************************************************

  template <typename T>
  using atomic = std::atomic<T>;

  using memory_order = std::memory_order;

  static ETL_CONSTANT etl::memory_order memory_order_relaxed = std::memory_order_relaxed;
  static ETL_CONSTANT etl::memory_order memory_order_consume = std::memory_order_consume;
  static ETL_CONSTANT etl::memory_order memory_order_acquire = std::memory_order_acquire;
  static ETL_CONSTANT etl::memory_order memory_order_release = std::memory_order_release;
  static ETL_CONSTANT etl::memory_order memory_order_acq_rel = std::memory_order_acq_rel;
  static ETL_CONSTANT etl::memory_order memory_order_seq_cst = std::memory_order_seq_cst;

  using atomic_bool           = std::atomic<bool>;
  using atomic_char           = std::atomic<char>;
  using atomic_schar          = std::atomic<signed char>;
  using atomic_uchar          = std::atomic<unsigned char>;
  using atomic_short          = std::atomic<short>;
  using atomic_ushort         = std::atomic<unsigned short>;
  using atomic_int            = std::atomic<int>;
  using atomic_uint           = std::atomic<unsigned int>;
  using atomic_long           = std::atomic<long>;
  using atomic_ulong          = std::atomic<unsigned long>;
  using atomic_llong          = std::atomic<long long>;
  using atomic_ullong         = std::atomic<unsigned long long>;
  using atomic_wchar_t        = std::atomic<wchar_t>;
#if ETL_HAS_NATIVE_CHAR8_T
  using atomic_char8_t = std::atomic<char8_t>;
#endif
#if ETL_HAS_NATIVE_CHAR16_T
  using atomic_char16_t       = std::atomic<char16_t>;
#endif
#if ETL_HAS_NATIVE_CHAR32_T
  using atomic_char32_t       = std::atomic<char32_t>;
#endif
#if ETL_USING_8BIT_TYPES
  using atomic_uint8_t        = std::atomic<uint8_t>;
  using atomic_int8_t         = std::atomic<int8_t>;
#endif
  using atomic_uint16_t       = std::atomic<uint16_t>;
  using atomic_int16_t        = std::atomic<int16_t>;
  using atomic_uint32_t       = std::atomic<uint32_t>;
  using atomic_int32_t        = std::atomic<int32_t>;
#if ETL_USING_64BIT_TYPES
  using atomic_uint64_t       = std::atomic<uint64_t>;
  using atomic_int64_t        = std::atomic<int64_t>;
#endif
  using atomic_int_least8_t   = std::atomic<int_least8_t>;
  using atomic_uint_least8_t  = std::atomic<uint_least8_t>;
  using atomic_int_least16_t  = std::atomic<int_least16_t>;
  using atomic_uint_least16_t = std::atomic<uint_least16_t>;
  using atomic_int_least32_t  = std::atomic<int_least32_t>;
  using atomic_uint_least32_t = std::atomic<uint_least32_t>;
#if ETL_USING_64BIT_TYPES
  using atomic_int_least64_t  = std::atomic<int_least64_t>;
  using atomic_uint_least64_t = std::atomic<uint_least64_t>;
#endif
  using atomic_int_fast8_t    = std::atomic<int_fast8_t>;
  using atomic_uint_fast8_t   = std::atomic<uint_fast8_t>;
  using atomic_int_fast16_t   = std::atomic<int_fast16_t>;
  using atomic_uint_fast16_t  = std::atomic<uint_fast16_t>;
  using atomic_int_fast32_t   = std::atomic<int_fast32_t>;
  using atomic_uint_fast32_t  = std::atomic<uint_fast32_t>;
#if ETL_USING_64BIT_TYPES
  using atomic_int_fast64_t   = std::atomic<int_fast64_t>;
  using atomic_uint_fast64_t  = std::atomic<uint_fast64_t>;
#endif
  using atomic_intptr_t       = std::atomic<intptr_t>;
  using atomic_uintptr_t      = std::atomic<uintptr_t>;
  using atomic_size_t         = std::atomic<size_t>;
  using atomic_ptrdiff_t      = std::atomic<ptrdiff_t>;
  using atomic_intmax_t       = std::atomic<intmax_t>;
  using atomic_uintmax_t      = std::atomic<uintmax_t>;
}

#endif
