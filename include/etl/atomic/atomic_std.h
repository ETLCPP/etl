/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 jwellbelove

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

  typedef std::memory_order memory_order;

  static const etl::memory_order memory_order_relaxed = std::memory_order_relaxed;
  static const etl::memory_order memory_order_consume = std::memory_order_consume;
  static const etl::memory_order memory_order_acquire = std::memory_order_acquire;
  static const etl::memory_order memory_order_release = std::memory_order_release;
  static const etl::memory_order memory_order_acq_rel = std::memory_order_acq_rel;
  static const etl::memory_order memory_order_seq_cst = std::memory_order_seq_cst;

  template <typename T>
  class atomic
  {
  public:

    atomic()
      : value(0)
    {
    }

    atomic(T v)
      : value(v)
    {
    }

    // Assignment
    T operator =(T v)
    {
      return value = v;
    }

    T operator =(T v) volatile
    {
      return value = v;
    }

    // Pre-increment
    T operator ++()
    {
      return ++value;
    }

    T operator ++() volatile
    {
      return ++value;
    }

    // Post-increment
    T operator ++(int)
    {
      return value++;
    }

    T operator ++(int) volatile
    {
      return value++;
    }

    // Pre-decrement
    T operator --()
    {
      return --value;
    }

    T operator --() volatile
    {
      return --value;
    }

    // Post-decrement
    T operator --(int)
    {
      return value--;
    }

    T operator --(int) volatile
    {
      return value--;
    }

    // Add
    T operator +=(T v)
    {
      return value += v;
    }

    T operator +=(T v) volatile
    {
      return value += v;
    }

    // Subtract
    T operator -=(T v)
    {
      return value -= v;
    }

    T operator -=(T v) volatile
    {
      return value -= v;
    }

    // And
    T operator &=(T v)
    {
      return value &= v;
    }

    T operator &=(T v) volatile
    {
      return value &= v;
    }

    // Or
    T operator |=(T v)
    {
      return value |= v;
    }

    T operator |=(T v) volatile
    {
      return value |= v;
    }

    // Exclusive or
    T operator ^=(T v)
    {
      return value ^= v;
    }

    T operator ^=(T v) volatile
    {
      return value ^= v;
    }

    // Conversion operator
    operator T () const
    {
      return T(value);
    }

    operator T() volatile const
    {
      return T(value);
    }

    // Is lock free?
    bool is_lock_free() const
    {
      return value.is_lock_free();
    }

    bool is_lock_free() const volatile
    {
      return value.is_lock_free();
    }

    // Store
    void store(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      value.store(v, order);
    }

    void store(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      value.store(v, order);
    }

    // Load
    T load(etl::memory_order order = etl::memory_order_seq_cst) const
    {
      return value.load(order);
    }

    T load(etl::memory_order order = etl::memory_order_seq_cst) const volatile
    {
      return value.load(order);
    }

    // Fetch add
    T fetch_add(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return value.fetch_add(v, order);
    }

    T fetch_add(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return value.fetch_add(v, order);
    }

    // Fetch subtract
    T fetch_sub(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return value.fetch_sub(v, order);
    }

    T fetch_sub(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return value.fetch_sub(v, order);
    }

    // Fetch or
    T fetch_or(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return value.fetch_or(v, order);
    }

    T fetch_or(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return value.fetch_or(v, order);
    }

    // Fetch and
    T fetch_and(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return value.fetch_and(v, order);
    }

    T fetch_and(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return value.fetch_and(v, order);
    }

    // Fetch exclusive or
    T fetch_xor(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return value.fetch_xor(v, order);
    }

    T fetch_xor(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return value.fetch_xor(v, order);
    }

    // Exchange
    T exchange(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return value.exchange(v, order);
    }

    T exchange(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return value.exchange(v, order);
    }

    // Compare exchange weak
    bool compare_exchange_weak(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return value.compare_exchange_weak(expected, desired, order);
    }

    bool compare_exchange_weak(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return value.compare_exchange_weak(expected, desired, order);
    }

    bool compare_exchange_weak(T& expected, T desired, etl::memory_order success, etl::memory_order failure)
    {
      return value.compare_exchange_weak(expected, desired, success, failure);
    }

    bool compare_exchange_weak(T& expected, T desired, etl::memory_order success, etl::memory_order failure) volatile
    {
      return value.compare_exchange_weak(expected, desired, success, failure);
    }

    // Compare exchange strong
    bool compare_exchange_strong(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return value.compare_exchange_strong(expected, desired, order);
    }

    bool compare_exchange_strong(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return value.compare_exchange_strong(expected, desired, order);
    }

    bool compare_exchange_strong(T& expected, T desired, etl::memory_order success, etl::memory_order failure)
    {
      return value.compare_exchange_strong(expected, desired, success, failure);
    }

    bool compare_exchange_strong(T& expected, T desired, etl::memory_order success, etl::memory_order failure) volatile
    {
      return value.compare_exchange_strong(expected, desired, success, failure);
    }

  private:

    atomic& operator =(const atomic&);
    //atomic& operator =(const atomic&) volatile;

    std::atomic<T> value;
  };

  template <typename T>
  class atomic<T*>
  {
  public:

    atomic()
      : value(nullptr)
    {
    }

    atomic(T* v)
      : value(v)
    {
    }

    // Assignment
    T* operator =(T* v)
    {
      return value = v;
    }

    T* operator =(T* v) volatile
    {
      return value = v;
    }

    // Pre-increment
    T* operator ++()
    {
      return ++value;
    }

    T* operator ++() volatile
    {
      return ++value;
    }

    // Post-increment
    T* operator ++(int)
    {
      return value++;
    }

    T* operator ++(int) volatile
    {
      return value++;
    }

    // Pre-decrement
    T* operator --()
    {
      return --value;
    }

    T* operator --() volatile
    {
      return --value;
    }

    // Post-decrement
    T* operator --(int)
    {
      return value--;
    }

    T* operator --(int) volatile
    {
      return value--;
    }

    // Add
    T* operator +=(ptrdiff_t v)
    {
      return value += v;
    }

    T* operator +=(ptrdiff_t v) volatile
    {
      return value += v;
    }

    // Subtract
    T* operator -=(ptrdiff_t v)
    {
      return value -= v;
    }

    T* operator -=(ptrdiff_t v) volatile
    {
      return value -= v;
    }

    // Conversion operator
    operator T* () const
    {
      return (T*)value;
    }

    operator T*() volatile const
    {
      return (T*)value;
    }

    // Is lock free?
    bool is_lock_free() const
    {
      return value.is_lock_free();
    }

    bool is_lock_free() const volatile
    {
      return value.is_lock_free();
    }

    // Store
    void store(T* v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      value.store(v, order);
    }

    void store(T* v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      value.store(v, order);
    }

    // Load
    T* load(etl::memory_order order = etl::memory_order_seq_cst)
    {
      return value.load(order);
    }

    T* load(etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return value.load(order);
    }

    // Fetch add
    T* fetch_add(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return value.fetch_add(v, order);
    }

    T* fetch_add(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return value.fetch_add(v, order);
    }

    // Fetch subtract
    T* fetch_sub(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return value.fetch_sub(v, order);
    }

    T* fetch_sub(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return value.fetch_sub(v, order);
    }

    // Exchange
    T* exchange(T* v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return value.exchange(v, order);
    }

    T* exchange(T* v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return value.exchange(v, order);
    }

    // Compare exchange weak
    bool compare_exchange_weak(T*& expected, T* desired, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return value.compare_exchange_weak(expected, desired, order);
    }

    bool compare_exchange_weak(T*& expected, T* desired, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return value.compare_exchange_weak(expected, desired, order);
    }

    bool compare_exchange_weak(T*& expected, T* desired, etl::memory_order success, etl::memory_order failure)
    {
      return value.compare_exchange_weak(expected, desired, success, failure);
    }

    bool compare_exchange_weak(T*& expected, T* desired, etl::memory_order success, etl::memory_order failure) volatile
    {
      return value.compare_exchange_weak(expected, desired, success, failure);
    }

    // Compare exchange strong
    bool compare_exchange_strong(T*& expected, T* desired, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return value.compare_exchange_strong(expected, desired, order);
    }

    bool compare_exchange_strong(T*& expected, T* desired, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return value.compare_exchange_strong(expected, desired, order);
    }

    bool compare_exchange_strong(T*& expected, T* desired, etl::memory_order success, etl::memory_order failure)
    {
      return value.compare_exchange_strong(expected, desired, success, failure);
    }

    bool compare_exchange_strong(T*& expected, T* desired, etl::memory_order success, etl::memory_order failure) volatile
    {
      return value.compare_exchange_strong(expected, desired, success, failure);
    }

  private:

    atomic & operator =(const atomic&);
    //atomic& operator =(const atomic&) volatile;

    std::atomic<T*> value;
  };

  typedef std::atomic<char>                atomic_char;
  typedef std::atomic<signed char>         atomic_schar;
  typedef std::atomic<unsigned char>       atomic_uchar;
  typedef std::atomic<short>               atomic_short;
  typedef std::atomic<unsigned short>      atomic_ushort;
  typedef std::atomic<int>                 atomic_int;
  typedef std::atomic<unsigned int>        atomic_uint;
  typedef std::atomic<long>                atomic_long;
  typedef std::atomic<unsigned long>       atomic_ulong;
  typedef std::atomic<long long>           atomic_llong;
  typedef std::atomic<unsigned long long>  atomic_ullong;
  typedef std::atomic<wchar_t>             atomic_wchar_t;
  typedef std::atomic<char16_t>            atomic_char16_t;
  typedef std::atomic<char32_t>            atomic_char32_t;
  typedef std::atomic<uint8_t>             atomic_uint8_t;
  typedef std::atomic<int8_t>              atomic_int8_t;
  typedef std::atomic<uint16_t>            atomic_uint16_t;
  typedef std::atomic<int16_t>             atomic_int16_t;
  typedef std::atomic<uint32_t>            atomic_uint32_t;
  typedef std::atomic<int32_t>             atomic_int32_t;
  typedef std::atomic<uint64_t>            atomic_uint64_t;
  typedef std::atomic<int64_t>             atomic_int64_t;
  typedef std::atomic<int_least8_t>        atomic_int_least8_t;
  typedef std::atomic<uint_least8_t>       atomic_uint_least8_t;
  typedef std::atomic<int_least16_t>       atomic_int_least16_t;
  typedef std::atomic<uint_least16_t>      atomic_uint_least16_t;
  typedef std::atomic<int_least32_t>       atomic_int_least32_t;
  typedef std::atomic<uint_least32_t>      atomic_uint_least32_t;
  typedef std::atomic<int_least64_t>       atomic_int_least64_t;
  typedef std::atomic<uint_least64_t>      atomic_uint_least64_t;
  typedef std::atomic<int_fast8_t>         atomic_int_fast8_t;
  typedef std::atomic<uint_fast8_t>        atomic_uint_fast8_t;
  typedef std::atomic<int_fast16_t>        atomic_int_fast16_t;
  typedef std::atomic<uint_fast16_t>       atomic_uint_fast16_t;
  typedef std::atomic<int_fast32_t>        atomic_int_fast32_t;
  typedef std::atomic<uint_fast32_t>       atomic_uint_fast32_t;
  typedef std::atomic<int_fast64_t>        atomic_int_fast64_t;
  typedef std::atomic<uint_fast64_t>       atomic_uint_fast64_t;
  typedef std::atomic<intptr_t>            atomic_intptr_t;
  typedef std::atomic<uintptr_t>           atomic_uintptr_t;
  typedef std::atomic<size_t>              atomic_size_t;
  typedef std::atomic<ptrdiff_t>           atomic_ptrdiff_t;
  typedef std::atomic<intmax_t>            atomic_intmax_t;
  typedef std::atomic<uintmax_t>           atomic_uintmax_t;
}

#endif
