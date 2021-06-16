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

#ifndef ETL_ATOMIC_GCC_SYNC_INCLUDED
#define ETL_ATOMIC_GCC_SYNC_INCLUDED

#include "../platform.h"
#include "../type_traits.h"
#include "../static_assert.h"
#include "../nullptr.h"
#include "../char_traits.h"

#include <stdint.h>
#include <string.h>

#if defined(ETL_COMPILER_GCC)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-value"
#endif

namespace etl
{
  //***************************************************************************
  // Atomic type for pre C++11 GCC compilers that support the builtin '__sync' functions.
  // Only integral and pointer types are supported.
  //***************************************************************************

  typedef enum memory_order
  {
    memory_order_relaxed,
    memory_order_consume,
    memory_order_acquire,
    memory_order_release,
    memory_order_acq_rel,
    memory_order_seq_cst
  } memory_order;

  //***************************************************************************
  /// For all types except bool and pointers
  //***************************************************************************
  template <typename T>
  class atomic
  {
  public:

    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Only integral types are supported");

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
      store(v);

      return v;
    }

    T operator =(T v) volatile
    {
      store(v);

      return v;
    }

    // Pre-increment
    T operator ++()
    {
      return __sync_add_and_fetch(&value, 1);
    }

    T operator ++() volatile
    {
      return __sync_add_and_fetch(&value, 1);
    }

    // Post-increment
    T operator ++(int)
    {
      return __sync_fetch_and_add(&value, 1);
    }

    T operator ++(int) volatile
    {
      return __sync_fetch_and_add(&value, 1);
    }

    // Pre-decrement
    T operator --()
    {
      return __sync_sub_and_fetch(&value, 1);
    }

    T operator --() volatile
    {
      return __sync_sub_and_fetch(&value, 1);
    }

    // Post-decrement
    T operator --(int)
    {
      return __sync_fetch_and_sub(&value, 1);
    }

    T operator --(int) volatile
    {
      return __sync_fetch_and_sub(&value, 1);
    }

    // Add
    T operator +=(T v)
    {
      return __sync_fetch_and_add(&value, v);
    }

    T operator +=(T v) volatile
    {
      return __sync_fetch_and_add(&value, v);
    }

    // Subtract
    T operator -=(T v)
    {
      return __sync_fetch_and_sub(&value, v);
    }

    T operator -=(T v) volatile
    {
      return __sync_fetch_and_sub(&value, v);
    }

    // And
    T operator &=(T v)
    {
      return __sync_fetch_and_and(&value, v);
    }

    T operator &=(T v) volatile
    {
      return __sync_fetch_and_and(&value, v);
    }

    // Or
    T operator |=(T v)
    {
      return __sync_fetch_and_or(&value, v);
    }

    T operator |=(T v) volatile
    {
      return __sync_fetch_and_or(&value, v);
    }

    // Exclusive or
    T operator ^=(T v)
    {
      return __sync_fetch_and_xor(&value, v);
    }

    T operator ^=(T v) volatile
    {
      return __sync_fetch_and_xor(&value, v);
    }

    // Conversion operator
    operator T () const
    {
      return __sync_fetch_and_add(&value, 0);
    }

    operator T() volatile const
    {
      return __sync_fetch_and_add(&value, 0);
    }

    // Is lock free?
    bool is_lock_free() const
    {
      return true;
    }

    bool is_lock_free() const volatile
    {
      return true;
    }

    // Store
    void store(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      (void)__sync_lock_test_and_set(&value, v);
    }

    void store(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      (void)__sync_lock_test_and_set(&value, v);
    }

    // Load
    T load(etl::memory_order order = etl::memory_order_seq_cst) const
    {
      return __sync_fetch_and_add(&value, 0);
    }

    T load(etl::memory_order order = etl::memory_order_seq_cst) const volatile
    {
      return __sync_fetch_and_add(&value, 0);
    }

    // Fetch add
    T fetch_add(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return __sync_fetch_and_add(&value, v);
    }

    T fetch_add(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return __sync_fetch_and_add(&value, v);
    }

    // Fetch subtract
    T fetch_sub(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return __sync_fetch_and_sub(&value, v);
    }

    T fetch_sub(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return __sync_fetch_and_sub(&value, v);
    }

    // Fetch or
    T fetch_or(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return __sync_fetch_and_or(&value, v);
    }

    T fetch_or(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return __sync_fetch_and_or(&value, v);
    }

    // Fetch and
    T fetch_and(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return __sync_fetch_and_and(&value, v);
    }

    T fetch_and(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return __sync_fetch_and_and(&value, v);
    }

    // Fetch exclusive or
    T fetch_xor(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return __sync_fetch_and_xor(&value, v);
    }

    T fetch_xor(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return __sync_fetch_and_xor(&value, v);
    }

    // Exchange
    T exchange(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return __sync_lock_test_and_set(&value, v);
    }

    T exchange(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return __sync_lock_test_and_set(&value, v);
    }

    // Compare exchange weak
    bool compare_exchange_weak(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst)
    {
      T old = __sync_val_compare_and_swap(&value, expected, desired);

      if (old == expected)
      {
        return true;
      }
      else
      {
        expected = old;
        return false;
      }
    }

    bool compare_exchange_weak(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      T old = __sync_val_compare_and_swap(&value, expected, desired);

      if (old == expected)
      {
        return true;
      }
      else
      {
        expected = old;
        return false;
      }
    }

    bool compare_exchange_weak(T& expected, T desired, etl::memory_order success, etl::memory_order failure)
    {
      T old = __sync_val_compare_and_swap(&value, expected, desired);

      if (old == expected)
      {
        return true;
      }
      else
      {
        expected = old;
        return false;
      }
    }

    bool compare_exchange_weak(T& expected, T desired, etl::memory_order success, etl::memory_order failure) volatile
    {
      T old = __sync_val_compare_and_swap(&value, expected, desired);

      if (old == expected)
      {
        return true;
      }
      else
      {
        expected = old;
        return false;
      }
    }

    // Compare exchange strong
    bool compare_exchange_strong(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst)
    {
      T old = expected;

      while (!compare_exchange_weak(old, desired))
      {
        if (memcmp(&old, &expected, sizeof(T)))
        {
          expected = old;
          return false;
        }
      }

      return true;
    }

    bool compare_exchange_strong(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      T old = expected;

      while (!compare_exchange_weak(old, desired))
      {
        if (memcmp(&old, &expected, sizeof(T)))
        {
          expected = old;
          return false;
        }
      }

      return true;
    }

    bool compare_exchange_strong(T& expected, T desired, etl::memory_order success, etl::memory_order failure)
    {
      T old = expected;

      while (!compare_exchange_weak(old, desired))
      {
        if (memcmp(&old, &expected, sizeof(T)))
        {
          expected = old;
          return false;
        }
      }

      return true;
    }

    bool compare_exchange_strong(T& expected, T desired, etl::memory_order success, etl::memory_order failure) volatile
    {
      T old = expected;

      while (!compare_exchange_weak(old, desired))
      {
        if (memcmp(&old, &expected, sizeof(T)))
        {
          expected = old;
          return false;
        }
      }

      return true;
    }

  private:

    atomic& operator =(const atomic&) ETL_DELETE;
    atomic& operator =(const atomic&) volatile ETL_DELETE;

    mutable volatile T value;
  };

  //***************************************************************************
  /// Specialisation for pointers
  //***************************************************************************
  template <typename T>
  class atomic<T*>
  {
  public:

    atomic()
      : value(0U)
    {
    }

    atomic(T* v)
      : value(uintptr_t(v))
    {
    }

    // Assignment
    T* operator =(T* v)
    {
      store(v);

      return v;
    }

    T* operator =(T* v) volatile
    {
      store(v);

      return v;
    }

    // Pre-increment
    T* operator ++()
    {
      return (T*)__sync_add_and_fetch(&value, sizeof(T));
    }

    T* operator ++() volatile
    {
      return (T*)__sync_add_and_fetch(&value, sizeof(T));
    }

    // Post-increment
    T* operator ++(int)
    {
      return (T*)__sync_fetch_and_add(&value, sizeof(T));
    }

    T* operator ++(int) volatile
    {
      return (T*)__sync_fetch_and_add(&value, sizeof(T));
    }

    // Pre-decrement
    T* operator --()
    {
      return (T*)__sync_sub_and_fetch(&value, sizeof(T));
    }

    T* operator --() volatile
    {
      return (T*)__sync_sub_and_fetch(&value, sizeof(T));
    }

    // Post-decrement
    T* operator --(int)
    {
      return (T*)__sync_fetch_and_sub(&value, sizeof(T));
    }

    T* operator --(int) volatile
    {
      return (T*)__sync_fetch_and_sub(&value, sizeof(T));
    }

    // Add
    T* operator +=(ptrdiff_t v)
    {
      return (T*)__sync_fetch_and_add(&value, v * sizeof(T));
    }

    T* operator +=(ptrdiff_t v) volatile
    {
      return (T*)__sync_fetch_and_add(&value, v * sizeof(T));
    }

    // Subtract
    T* operator -=(ptrdiff_t v)
    {
      return (T*)__sync_fetch_and_sub(&value, v * sizeof(T));
    }

    T* operator -=(ptrdiff_t v) volatile
    {
      return (T*)__sync_fetch_and_sub(&value, v * sizeof(T));
    }

    // Conversion operator
    operator T* () const
    {
      return (T*)__sync_fetch_and_add(&value, 0);
    }

    operator T*() volatile const
    {
      return (T*)__sync_fetch_and_add(&value, 0);
    }

    // Is lock free?
    bool is_lock_free() const
    {
      return true;
    }

    bool is_lock_free() const volatile
    {
      return true;
    }

    // Store
    void store(T* v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      __sync_lock_test_and_set(&value, uintptr_t(v));
    }

    void store(T* v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      __sync_lock_test_and_set(&value, uintptr_t(v));
    }

    // Load
    T* load(etl::memory_order order = etl::memory_order_seq_cst) const
    {
      return (T*)__sync_fetch_and_add(&value, 0);
    }

    T* load(etl::memory_order order = etl::memory_order_seq_cst) const volatile
    {
      return (T*)__sync_fetch_and_add(&value, 0);
    }

    // Fetch add
    T* fetch_add(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return (T*)__sync_fetch_and_add(&value, v);
    }

    T* fetch_add(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return (T*)__sync_fetch_and_add(&value, v);
    }

    // Fetch subtract
    T* fetch_sub(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return (T*)__sync_fetch_and_sub(&value, v);
    }

    T* fetch_sub(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return (T*)__sync_fetch_and_sub(&value, v);
    }

    // Exchange
    T* exchange(T* v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return (T*)__sync_lock_test_and_set(&value, uintptr_t(v));
    }

    T* exchange(T* v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return (T*)__sync_lock_test_and_set(&value, uintptr_t(v));
    }

    // Compare exchange weak
    bool compare_exchange_weak(T*& expected, T* desired, etl::memory_order order = etl::memory_order_seq_cst)
    {
      T* old = (T*)__sync_val_compare_and_swap(&value, uintptr_t(expected), uintptr_t(desired));

      if (old == expected)
      {
        return true;
      }
      else
      {
        expected = old;
        return false;
      }
    }

    bool compare_exchange_weak(T*& expected, T* desired, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      T* old = (T*)__sync_val_compare_and_swap(&value, uintptr_t(expected), uintptr_t(desired));

      if (old == expected)
      {
        return true;
      }
      else
      {
        expected = old;
        return false;
      }
    }

    bool compare_exchange_weak(T*& expected, T* desired, etl::memory_order success, etl::memory_order failure)
    {
      T* old = (T*)__sync_val_compare_and_swap(&value, uintptr_t(expected), uintptr_t(desired));

      if (old == expected)
      {
        return true;
      }
      else
      {
        expected = old;
        return false;
      }
    }

    bool compare_exchange_weak(T*& expected, T* desired, etl::memory_order success, etl::memory_order failure) volatile
    {
      T* old = (T*)__sync_val_compare_and_swap(&value, uintptr_t(expected), uintptr_t(desired));

      if (old == expected)
      {
        return true;
      }
      else
      {
        expected = old;
        return false;
      }
    }

    // Compare exchange strong
    bool compare_exchange_strong(T*& expected, T* desired, etl::memory_order order = etl::memory_order_seq_cst)
    {
      T* old = expected;

      while (!compare_exchange_weak(old, desired))
      {
        if (memcmp(&old, &expected, sizeof(T*)))
        {
          expected = old;
          return false;
        }
      }

      return true;
    }

    bool compare_exchange_strong(T*& expected, T* desired, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      T* old = expected;

      while (!compare_exchange_weak(old, desired))
      {
        if (memcmp(&old, &expected, sizeof(T*)))
        {
          expected = old;
          return false;
        }
      }

      return true;
    }

    bool compare_exchange_strong(T*& expected, T* desired, etl::memory_order success, etl::memory_order failure)
    {
      T* old = expected;

      while (!compare_exchange_weak(old, desired))
      {
        if (memcmp(&old, &expected, sizeof(T*)))
        {
          expected = old;
          return false;
        }
      }

      return true;
    }

    bool compare_exchange_strong(T*& expected, T* desired, etl::memory_order success, etl::memory_order failure) volatile
    {
      T* old = expected;

      while (!compare_exchange_weak(old, desired))
      {
        if (memcmp(&old, &expected, sizeof(T*)))
        {
          expected = old;
          return false;
        }
      }

      return true;
    }

  private:

    atomic& operator =(const atomic&) ETL_DELETE;
    atomic& operator =(const atomic&) volatile ETL_DELETE;

    mutable uintptr_t value;
  };

  //***************************************************************************
  /// Specialisation for bool
  //***************************************************************************
  template <>
  class atomic<bool>
  {
  public:

    atomic()
      : value(0U)
    {
    }

    atomic(bool v)
      : value(char(v))
    {
    }

    // Assignment
    bool operator =(bool v)
    {
      store(v);

      return v;
    }

    bool operator =(bool v) volatile
    {
      store(v);

      return v;
    }

    // Pre-increment
    bool operator ++()
    {
      return (bool)__sync_add_and_fetch(&value, sizeof(char));
    }

    bool operator ++() volatile
    {
      return (bool)__sync_add_and_fetch(&value, sizeof(char));
    }

    // Post-increment
    bool operator ++(int)
    {
      return (bool)__sync_fetch_and_add(&value, sizeof(char));
    }

    bool operator ++(int) volatile
    {
      return (bool)__sync_fetch_and_add(&value, sizeof(char));
    }

    // Pre-decrement
    bool operator --()
    {
      return (bool)__sync_sub_and_fetch(&value, sizeof(char));
    }

    bool operator --() volatile
    {
      return (bool)__sync_sub_and_fetch(&value, sizeof(char));
    }

    // Post-decrement
    bool operator --(int)
    {
      return (bool)__sync_fetch_and_sub(&value, sizeof(char));
    }

    bool operator --(int) volatile
    {
      return (bool)__sync_fetch_and_sub(&value, sizeof(char));
    }

    // Add
    bool operator +=(ptrdiff_t v)
    {
      return (bool)__sync_fetch_and_add(&value, v * sizeof(char));
    }

    bool operator +=(ptrdiff_t v) volatile
    {
      return (bool)__sync_fetch_and_add(&value, v * sizeof(char));
    }

    // Subtract
    bool operator -=(ptrdiff_t v)
    {
      return (bool)__sync_fetch_and_sub(&value, v * sizeof(char));
    }

    bool operator -=(ptrdiff_t v) volatile
    {
      return (bool)__sync_fetch_and_sub(&value, v * sizeof(char));
    }

    // Conversion operator
    operator bool () const
    {
      return (bool)__sync_fetch_and_add(&value, 0);
    }

    operator bool() volatile const
    {
      return (bool)__sync_fetch_and_add(&value, 0);
    }

    // Is lock free?
    bool is_lock_free() const
    {
      return true;
    }

    bool is_lock_free() const volatile
    {
      return true;
    }

    // Store
    void store(bool v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      __sync_lock_test_and_set(&value, char(v));
    }

    void store(bool v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      __sync_lock_test_and_set(&value, char(v));
    }

    // Load
    bool load(etl::memory_order order = etl::memory_order_seq_cst) const
    {
      return (bool)__sync_fetch_and_add(&value, 0);
    }

    bool load(etl::memory_order order = etl::memory_order_seq_cst) const volatile
    {
      return (bool)__sync_fetch_and_add(&value, 0);
    }

    // Fetch add
    bool fetch_add(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return (bool)__sync_fetch_and_add(&value, v);
    }

    bool fetch_add(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return (bool)__sync_fetch_and_add(&value, v);
    }

    // Fetch subtract
    bool fetch_sub(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return (bool)__sync_fetch_and_sub(&value, v);
    }

    bool fetch_sub(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return (bool)__sync_fetch_and_sub(&value, v);
    }

    // Exchange
    bool exchange(bool v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return (bool)__sync_lock_test_and_set(&value, char(v));
    }

    bool exchange(bool v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return (bool)__sync_lock_test_and_set(&value, char(v));
    }

    // Compare exchange weak
    bool compare_exchange_weak(bool& expected, bool desired, etl::memory_order order = etl::memory_order_seq_cst)
    {
      bool old = (bool)__sync_val_compare_and_swap(&value, char(expected), char(desired));

      if (old == expected)
      {
        return true;
      }
      else
      {
        expected = old;
        return false;
      }
    }

    bool compare_exchange_weak(bool& expected, bool desired, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      bool old = (bool)__sync_val_compare_and_swap(&value, char(expected), char(desired));

      if (old == expected)
      {
        return true;
      }
      else
      {
        expected = old;
        return false;
      }
    }

    bool compare_exchange_weak(bool& expected, bool desired, etl::memory_order success, etl::memory_order failure)
    {
      bool old = (bool)__sync_val_compare_and_swap(&value, char(expected), char(desired));

      if (old == expected)
      {
        return true;
      }
      else
      {
        expected = old;
        return false;
      }
    }

    bool compare_exchange_weak(bool& expected, bool desired, etl::memory_order success, etl::memory_order failure) volatile
    {
      bool old = (bool)__sync_val_compare_and_swap(&value, char(expected), char(desired));

      if (old == expected)
      {
        return true;
      }
      else
      {
        expected = old;
        return false;
      }
    }

    // Compare exchange strong
    bool compare_exchange_strong(bool& expected, bool desired, etl::memory_order order = etl::memory_order_seq_cst)
    {
      bool old = expected;

      while (!compare_exchange_weak(old, desired))
      {
        if (memcmp(&old, &expected, sizeof(bool)))
        {
          expected = old;
          return false;
        }
      }

      return true;
    }

    bool compare_exchange_strong(bool& expected, bool desired, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      bool old = expected;

      while (!compare_exchange_weak(old, desired))
      {
        if (memcmp(&old, &expected, sizeof(bool)))
        {
          expected = old;
          return false;
        }
      }

      return true;
    }

    bool compare_exchange_strong(bool& expected, bool desired, etl::memory_order success, etl::memory_order failure)
    {
      bool old = expected;

      while (!compare_exchange_weak(old, desired))
      {
        if (memcmp(&old, &expected, sizeof(bool)))
        {
          expected = old;
          return false;
        }
      }

      return true;
    }

    bool compare_exchange_strong(bool& expected, bool desired, etl::memory_order success, etl::memory_order failure) volatile
    {
      bool old = expected;

      while (!compare_exchange_weak(old, desired))
      {
        if (memcmp(&old, &expected, sizeof(bool)))
        {
          expected = old;
          return false;
        }
      }

      return true;
    }

  private:

    atomic& operator =(const atomic&) ETL_DELETE;
    atomic& operator =(const atomic&) volatile ETL_DELETE;

    mutable char value;
  };

  typedef etl::atomic<bool>                atomic_bool;
  typedef etl::atomic<char>                atomic_char;
  typedef etl::atomic<signed char>         atomic_schar;
  typedef etl::atomic<unsigned char>       atomic_uchar;
  typedef etl::atomic<short>               atomic_short;
  typedef etl::atomic<unsigned short>      atomic_ushort;
  typedef etl::atomic<int>                 atomic_int;
  typedef etl::atomic<unsigned int>        atomic_uint;
  typedef etl::atomic<long>                atomic_long;
  typedef etl::atomic<unsigned long>       atomic_ulong;
  typedef etl::atomic<long long>           atomic_llong;
  typedef etl::atomic<unsigned long long>  atomic_ullong;
  typedef etl::atomic<wchar_t>             atomic_wchar_t;
  typedef etl::atomic<char16_t>            atomic_char16_t;
  typedef etl::atomic<char32_t>            atomic_char32_t;
#if ETL_USING_8BIT_TYPES
  typedef etl::atomic<uint8_t>             atomic_uint8_t;
  typedef etl::atomic<int8_t>              atomic_int8_t;
#endif
  typedef etl::atomic<uint16_t>            atomic_uint16_t;
  typedef etl::atomic<int16_t>             atomic_int16_t;
  typedef etl::atomic<uint32_t>            atomic_uint32_t;
  typedef etl::atomic<int32_t>             atomic_int32_t;
#if ETL_USING_64BIT_TYPES
  typedef etl::atomic<uint64_t>            atomic_uint64_t;
  typedef etl::atomic<int64_t>             atomic_int64_t;
#endif
  typedef etl::atomic<int_least8_t>        atomic_int_least8_t;
  typedef etl::atomic<uint_least8_t>       atomic_uint_least8_t;
  typedef etl::atomic<int_least16_t>       atomic_int_least16_t;
  typedef etl::atomic<uint_least16_t>      atomic_uint_least16_t;
  typedef etl::atomic<int_least32_t>       atomic_int_least32_t;
  typedef etl::atomic<uint_least32_t>      atomic_uint_least32_t;
#if ETL_USING_64BIT_TYPES
  typedef etl::atomic<int_least64_t>       atomic_int_least64_t;
  typedef etl::atomic<uint_least64_t>      atomic_uint_least64_t;
#endif
  typedef etl::atomic<int_fast8_t>         atomic_int_fast8_t;
  typedef etl::atomic<uint_fast8_t>        atomic_uint_fast8_t;
  typedef etl::atomic<int_fast16_t>        atomic_int_fast16_t;
  typedef etl::atomic<uint_fast16_t>       atomic_uint_fast16_t;
  typedef etl::atomic<int_fast32_t>        atomic_int_fast32_t;
  typedef etl::atomic<uint_fast32_t>       atomic_uint_fast32_t;
#if ETL_USING_64BIT_TYPES
  typedef etl::atomic<int_fast64_t>        atomic_int_fast64_t;
  typedef etl::atomic<uint_fast64_t>       atomic_uint_fast64_t;
#endif
  typedef etl::atomic<intptr_t>            atomic_intptr_t;
  typedef etl::atomic<uintptr_t>           atomic_uintptr_t;
  typedef etl::atomic<size_t>              atomic_size_t;
  typedef etl::atomic<ptrdiff_t>           atomic_ptrdiff_t;
  typedef etl::atomic<intmax_t>            atomic_intmax_t;
  typedef etl::atomic<uintmax_t>           atomic_uintmax_t;
}

#if defined(ETL_COMPILER_GCC)
#pragma GCC diagnostic pop
#endif

#endif
