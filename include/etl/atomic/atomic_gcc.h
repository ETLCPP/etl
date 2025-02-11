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

#ifndef ETL_ATOMIC_GCC_SYNC_INCLUDED
#define ETL_ATOMIC_GCC_SYNC_INCLUDED

#include "../platform.h"
#include "../type_traits.h"
#include "../static_assert.h"
#include "../nullptr.h"
#include "../char_traits.h"
#include "../bit.h"
#include "../memory.h"

#include <stdint.h>
#include <string.h>

namespace etl
{
  //*************************************************************************
  // Atomic type for pre C++11 GCC compilers that support the builtin '__atomic' functions.
  // Only integral and pointer types are supported.
  //*************************************************************************

  typedef enum memory_order
  {
    memory_order_relaxed = __ATOMIC_RELAXED,
    memory_order_consume = __ATOMIC_CONSUME,
    memory_order_acquire = __ATOMIC_ACQUIRE,
    memory_order_release = __ATOMIC_RELEASE,
    memory_order_acq_rel = __ATOMIC_ACQ_REL,
    memory_order_seq_cst = __ATOMIC_SEQ_CST
  } memory_order;

  namespace private_atomic
  {
    //*************************************************************************
    template <typename T>
    struct storage_type
    {
      typedef
#if ETL_USING_8BIT_TYPES
        typename etl::conditional<sizeof(T) == sizeof(uint8_t), uint8_t,
#endif        
#if ETL_USING_64BIT_TYPES
        typename etl::conditional<sizeof(T) == sizeof(uint16_t), uint16_t,
        typename etl::conditional<sizeof(T) == sizeof(uint32_t), uint32_t, uint64_t>::type>::type>::type type;
#else
        typename etl::conditional<sizeof(T) == sizeof(uint16_t), uint16_t, uint32_t>::type>::type type;
#endif
    };

    template <typename TStore>
    class atomic_common
    {
    public:

      // Is always lock free?
      static ETL_CONSTANT bool is_always_lock_free = __atomic_always_lock_free(sizeof(TStore), nullptr);
    };

    template <typename TStore>
    ETL_CONSTANT bool atomic_common<TStore>::is_always_lock_free;
  }

  template <typename T, bool Is_Integral_Type = etl::is_integral<T>::value>
  class atomic;

  //*************************************************************************
  /// Specialisation for integral types.
  //*************************************************************************
  template <typename T>
  class atomic<T, true> : public etl::private_atomic::atomic_common<T>
  {
  public:

    //*************************************************************************
    /// Default constructor
    //*************************************************************************
    atomic()
      : value(T())
    {
    }

    //*************************************************************************
    /// Contruct from a value
    //*************************************************************************
    atomic(T v)
      : value(v)
    {
    }

    //*************************************************************************
    /// Assignment
    //*************************************************************************
    T operator =(T v)
    {
      store(v);

      return v;
    }

    //*************************************************************************
    /// Assignment
    /// Volatile
    //*************************************************************************
    T operator =(T v) volatile
    {
      store(v);

      return v;
    }

    //*************************************************************************
    /// Pre-increment
    //*************************************************************************
    T operator ++()
    {
      return static_cast<T>(__atomic_add_fetch(&value, 1, etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Pre-increment
    /// Volatile 
    //*************************************************************************
    T operator ++() volatile
    {
      return static_cast<T>(__atomic_add_fetch(&value, 1, etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Post-increment
    //*************************************************************************
    T operator ++(int)
    {
      return static_cast<T>(__atomic_fetch_add(&value, 1, etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Post-increment
    /// Volatile
    //*************************************************************************
    T operator ++(int) volatile
    {
      return static_cast<T>(__atomic_fetch_add(&value, 1, etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Pre-decrement
    //*************************************************************************
    T operator --()
    {
      return static_cast<T>(__atomic_sub_fetch(&value, 1, etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Pre-decrement
    /// Volatile 
    //*************************************************************************
    T operator --() volatile
    {
      return static_cast<T>(__atomic_sub_fetch(&value, 1, etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Post-decrement
    //*************************************************************************
    T operator --(int)
    {
      return static_cast<T>(__atomic_fetch_sub(&value, 1, etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Post-decrement
    /// Volatile
    //*************************************************************************
    T operator --(int) volatile
    {
      return static_cast<T>(__atomic_fetch_sub(&value, 1, etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Add
    //*************************************************************************
    T operator +=(T v)
    {
      return static_cast<T>(__atomic_fetch_add(&value, v, etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Add
    /// Volatile
    //*************************************************************************
    T operator +=(T v) volatile
    {
      return static_cast<T>(__atomic_fetch_add(&value, v, etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Subtract
    //*************************************************************************
    T operator -=(T v)
    {
      return static_cast<T>(__atomic_fetch_sub(&value, v, etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Subtract
    /// Volatile
    //*************************************************************************
    T operator -=(T v) volatile
    {
      return static_cast<T>(__atomic_fetch_sub(&value, v, etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// And
    //*************************************************************************
    T operator &=(T v)
    {
      return static_cast<T>(__atomic_fetch_and(&value, v, etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// And
    /// Volatile
    //*************************************************************************
    T operator &=(T v) volatile
    {
      return static_cast<T>(__atomic_fetch_and(&value, v, etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Or
    //*************************************************************************
    T operator |=(T v)
    {
      return static_cast<T>(__atomic_fetch_or(&value, v, etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Or
    /// Volatile
    //*************************************************************************
    T operator |=(T v) volatile
    {
      return static_cast<T>(__atomic_fetch_or(&value, v, etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Exclusive or
    //*************************************************************************
    T operator ^=(T v)
    {
      return static_cast<T>(__atomic_fetch_xor(&value, v, etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Exclusive or
    /// Volatile
    //*************************************************************************
    T operator ^=(T v) volatile
    {
      return static_cast<T>(__atomic_fetch_xor(&value, v, etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Conversion operator
    //*************************************************************************
    operator T() const
    {
      return load();
    }

    //*************************************************************************
    /// Conversion operator
    /// Volatile
    //*************************************************************************
    operator T() volatile const
    {
      return load();
    }

    //*************************************************************************
    /// Is lock free?
    //*************************************************************************
    bool is_lock_free() const
    {
      return __atomic_is_lock_free(sizeof(T), &value);
    }

    //*************************************************************************
    /// Is lock free?
    /// Volatile
    //*************************************************************************
    bool is_lock_free() const volatile
    {
      return __atomic_is_lock_free(sizeof(T), &value);
    }

    //*************************************************************************
    /// Store
    //*************************************************************************
    void store(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      __atomic_store_n(&value, v, order);
    }

    //*************************************************************************
    /// Store
    /// Volatile
    //*************************************************************************
    void store(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      __atomic_store_n(&value, v, order);
    }

    //*************************************************************************
    /// Load
    //*************************************************************************
    T load(etl::memory_order order = etl::memory_order_seq_cst) const
    {
      return static_cast<T>(__atomic_load_n(&value, order));
    }

    //*************************************************************************
    /// Load
    /// Volatile
    //*************************************************************************
    T load(etl::memory_order order = etl::memory_order_seq_cst) const volatile
    {
      return static_cast<T>(__atomic_load_n(&value, order));
    }

    //*************************************************************************
    /// Fetch add
    //*************************************************************************
    T fetch_add(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return static_cast<T>(__atomic_fetch_add(&value, v, order));
    }

    T fetch_add(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return static_cast<T>(__atomic_fetch_add(&value, v, order));
    }

    //*************************************************************************
    /// Fetch subtract
    //*************************************************************************
    T fetch_sub(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return static_cast<T>(__atomic_fetch_sub(&value, v, order));
    }

    T fetch_sub(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return static_cast<T>(__atomic_fetch_sub(&value, v, order));
    }

    //*************************************************************************
    /// Fetch or
    //*************************************************************************
    T fetch_or(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return static_cast<T>(__atomic_fetch_or(&value, v, order));
    }

    T fetch_or(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return static_cast<T>(__atomic_fetch_or(&value, v, order));
    }

    //*************************************************************************
    /// Fetch and
    //*************************************************************************
    T fetch_and(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return static_cast<T>(__atomic_fetch_and(&value, v, order));
    }

    T fetch_and(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return static_cast<T>(__atomic_fetch_and(&value, v, order));
    }

    //*************************************************************************
    /// Fetch exclusive or
    //*************************************************************************
    T fetch_xor(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return static_cast<T>(__atomic_fetch_xor(&value, v, order));
    }

    T fetch_xor(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return __atomic_fetch_xor(&value, v, order);
    }

    //*************************************************************************
    /// Exchange
    //*************************************************************************
    T exchange(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return static_cast<T>(__atomic_exchange_n(&value, v, order));
    }

    T exchange(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return static_cast<T>(__atomic_exchange_n(&value, v, order));
    }

    //*************************************************************************
    /// Compare exchange weak
    //*************************************************************************
    bool compare_exchange_weak(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return __atomic_compare_exchange_n(&value, &expected, desired, true, order, order);
    }

    bool compare_exchange_weak(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return __atomic_compare_exchange_n(&value, &expected, desired, true, order, order);
    }

    bool compare_exchange_weak(T& expected, T desired, etl::memory_order success, etl::memory_order failure)
    {
      return __atomic_compare_exchange_n(&value, &expected, desired, true, success, failure);
    }

    bool compare_exchange_weak(T& expected, T desired, etl::memory_order success, etl::memory_order failure) volatile
    {
      return __atomic_compare_exchange_n(&value, &expected, desired, true, success, failure);
    }

    //*************************************************************************
    /// Compare exchange strong
    //*************************************************************************
    bool compare_exchange_strong(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return __atomic_compare_exchange_n(&value, &expected, desired, false, order, order);
    }

    bool compare_exchange_strong(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return __atomic_compare_exchange_n(&value, &expected, desired, false, order, order);
    }

    bool compare_exchange_strong(T& expected, T desired, etl::memory_order success, etl::memory_order failure)
    {
      return __atomic_compare_exchange_n(&value, &expected, desired, false, success, failure);
    }

    bool compare_exchange_strong(T& expected, T desired, etl::memory_order success, etl::memory_order failure) volatile
    {
      return __atomic_compare_exchange_n(&value, &expected, desired, false, success, failure);
    }

  private:

    atomic& operator =(const atomic&) ETL_DELETE;
    atomic& operator =(const atomic&) volatile ETL_DELETE;

    mutable T value;
  };

  //*************************************************************************
  /// Specialisation for pointer types.
  //*************************************************************************
  template <typename T>
  class atomic<T*, false> : public etl::private_atomic::atomic_common<uintptr_t>
  {
  public:

    //*************************************************************************
    /// Default constructor
    //*************************************************************************
    atomic()
      : value(0U)
    {
    }

    //*************************************************************************
    /// Contruct from a value
    //*************************************************************************
    atomic(T* v)
      : value(uintptr_t(v))
    {
    }

    //*************************************************************************
    /// Assignment
    //*************************************************************************
    T* operator =(T* v)
    {
      store(v);

      return v;
    }

    //*************************************************************************
    /// Assignment
    /// Volatile
    //*************************************************************************
    T* operator =(T* v) volatile
    {
      store(v);

      return v;
    }

    //*************************************************************************
    /// Pre-increment
    //*************************************************************************
    T* operator ++()
    {
      return reinterpret_cast<T*>(__atomic_add_fetch(&value, sizeof(T), etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Pre-increment
    /// Volatile
    //*************************************************************************
    T* operator ++() volatile
    {
      return reinterpret_cast<T*>(__atomic_add_fetch(&value, sizeof(T), etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Post-increment
    //*************************************************************************
    T* operator ++(int)
    {
      return reinterpret_cast<T*>(__atomic_fetch_add(&value, sizeof(T), etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Post-increment
    /// Volatile
    //*************************************************************************
    T* operator ++(int) volatile
    {
      return reinterpret_cast<T*>(__atomic_fetch_add(&value, sizeof(T), etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Pre-decrement
    //*************************************************************************
    T* operator --()
    {
      return reinterpret_cast<T*>(__atomic_sub_fetch(&value, sizeof(T), etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Pre-decrement
    /// Volatile
    //*************************************************************************
    T* operator --() volatile
    {
      return reinterpret_cast<T*>(__atomic_sub_fetch(&value, sizeof(T), etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Post-decrement
    //*************************************************************************
    T* operator --(int)
    {
      return reinterpret_cast<T*>(__atomic_fetch_sub(&value, sizeof(T), etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Post-decrement
    /// Volatile
    //*************************************************************************
    T* operator --(int) volatile
    {
      return reinterpret_cast<T*>(__atomic_fetch_sub(&value, sizeof(T), etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Add
    //*************************************************************************
    T* operator +=(ptrdiff_t v)
    {
      return reinterpret_cast<T*>(__atomic_fetch_add(&value, v * sizeof(T), etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Add
    /// Volatile
    //*************************************************************************
    T* operator +=(ptrdiff_t v) volatile
    {
      return reinterpret_cast<T*>(__atomic_fetch_add(&value, v * sizeof(T), etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Subtract
    //*************************************************************************
    T* operator -=(ptrdiff_t v)
    {
      return reinterpret_cast<T*>(__atomic_fetch_sub(&value, v * sizeof(T), etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Subtract
    /// Volatile
    //*************************************************************************
    T* operator -=(ptrdiff_t v) volatile
    {
      return reinterpret_cast<T*>(__atomic_fetch_sub(&value, v * sizeof(T), etl::memory_order_seq_cst));
    }

    //*************************************************************************
    /// Conversion operator
    //*************************************************************************
    operator T* () const
    {
      return load();
    }

    //*************************************************************************
    /// Conversion operator
    /// Volatile
    //*************************************************************************
    operator T*() volatile const
    {
      return load();
    }

    //*************************************************************************
    /// Is lock free?
    //*************************************************************************
    bool is_lock_free() const
    {
      return __atomic_is_lock_free(sizeof(uintptr_t), &value);
    }

    //*************************************************************************
    /// Is lock free?
    /// Volatile
    //*************************************************************************
    bool is_lock_free() const volatile
    {
      return __atomic_is_lock_free(sizeof(uintptr_t), &value);
    }

    //*************************************************************************
    /// Store
    //*************************************************************************
    void store(T* v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      __atomic_store_n(&value, uintptr_t(v), order);
    }

    //*************************************************************************
    /// Store
    /// Volatile
    //*************************************************************************
    void store(T* v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      __atomic_store_n(&value, uintptr_t(v), order);
    }

    //*************************************************************************
    /// Load
    //*************************************************************************
    T* load(etl::memory_order order = etl::memory_order_seq_cst) const
    {
      return reinterpret_cast<T*>(__atomic_load_n(&value, order));
    }

    //*************************************************************************
    /// Load
    /// Volatile
    //*************************************************************************
    T* load(etl::memory_order order = etl::memory_order_seq_cst) const volatile
    {
      return reinterpret_cast<T*>(__atomic_load_n(&value, order));
    }

    //*************************************************************************
    /// Fetch add
    //*************************************************************************
    T* fetch_add(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return reinterpret_cast<T*>(__atomic_fetch_add(&value, v, order));
    }

    //*************************************************************************
    /// Fetch add
    /// Volatile
    //*************************************************************************
    T* fetch_add(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return reinterpret_cast<T*>(__atomic_fetch_add(&value, v, order));
    }

    //*************************************************************************
    /// Fetch subtract
    //*************************************************************************
    T* fetch_sub(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return reinterpret_cast<T*>(__atomic_fetch_sub(&value, v, order));
    }

    //*************************************************************************
    /// Fetch subtract
    /// Volatile
    //*************************************************************************
    T* fetch_sub(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return reinterpret_cast<T*>(__atomic_fetch_sub(&value, v, order));
    }

    //*************************************************************************
    /// Exchange
    //*************************************************************************
    T* exchange(T* v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      return reinterpret_cast<T*>(__atomic_exchange_n(&value, uintptr_t(v), order));
    }

    //*************************************************************************
    /// Exchange
    /// Volatile
    //*************************************************************************
    T* exchange(T* v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      return reinterpret_cast<T*>(__atomic_exchange_n(&value, uintptr_t(v), order));
    }

    //*************************************************************************
    /// Compare exchange weak
    //*************************************************************************
    bool compare_exchange_weak(T*& expected, T* desired, etl::memory_order order = etl::memory_order_seq_cst)
    {
      uintptr_t expected_v = uintptr_t(expected);

      return __atomic_compare_exchange_n(&value, &expected_v, uintptr_t(desired), true, order, order);
    }

    //*************************************************************************
    /// Compare exchange weak
    /// Volatile
    //*************************************************************************
    bool compare_exchange_weak(T*& expected, T* desired, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      uintptr_t expected_v = uintptr_t(expected);

      return __atomic_compare_exchange_n(&value, &expected_v, uintptr_t(desired), true, order, order);
    }

    //*************************************************************************
    /// Compare exchange weak
    /// Success/failure
    //*************************************************************************
    bool compare_exchange_weak(T*& expected, T* desired, etl::memory_order success, etl::memory_order failure)
    {
      uintptr_t expected_v = uintptr_t(expected);

      return __atomic_compare_exchange_n(&value, &expected_v, uintptr_t(desired), true, success, failure);
    }

    //*************************************************************************
    /// Compare exchange weak
    /// Success/failure
    /// Volatile
    //*************************************************************************
    bool compare_exchange_weak(T*& expected, T* desired, etl::memory_order success, etl::memory_order failure) volatile
    {
      uintptr_t expected_v = uintptr_t(expected);

      return __atomic_compare_exchange_n(&value, &expected_v, uintptr_t(desired), true, success, failure);
    }

    //*************************************************************************
    /// Compare exchange strong
    //*************************************************************************
    bool compare_exchange_strong(T*& expected, T* desired, etl::memory_order order = etl::memory_order_seq_cst)
    {
      uintptr_t expected_v = uintptr_t(expected);

      return __atomic_compare_exchange_n(&value, &expected_v, uintptr_t(desired), false, order, order);
    }

    //*************************************************************************
    /// Compare exchange strong
    /// Volatile    
    //*************************************************************************
    bool compare_exchange_strong(T*& expected, T* desired, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      uintptr_t expected_v = uintptr_t(expected);

      return __atomic_compare_exchange_n(&value, &expected_v, uintptr_t(desired), false, order, order);
    }

    //*************************************************************************
    /// Compare exchange strong
    /// Success/failure
    //*************************************************************************
    bool compare_exchange_strong(T*& expected, T* desired, etl::memory_order success, etl::memory_order failure)
    {
      uintptr_t expected_v = uintptr_t(expected);

      return __atomic_compare_exchange_n(&value, &expected_v, uintptr_t(desired), false, success, failure);
    }

    //*************************************************************************
    /// Compare exchange strong
    /// Success/failure
    /// Volatile
    //*************************************************************************
    bool compare_exchange_strong(T*& expected, T* desired, etl::memory_order success, etl::memory_order failure) volatile
    {
      uintptr_t expected_v = uintptr_t(expected);

      return __atomic_compare_exchange_n(&value, &expected_v, uintptr_t(desired), false, success, failure);
    }

  private:

    atomic& operator =(const atomic&) ETL_DELETE;
    atomic& operator =(const atomic&) volatile ETL_DELETE;

    mutable uintptr_t value;
  };

  //*************************************************************************
  /// Specialisation for other types that can fit in an integral sized storage.
  //*************************************************************************
  template <typename T>
  class atomic<T, false> : public etl::private_atomic::atomic_common<typename etl::private_atomic::storage_type<T>::type>
  {
  private:

    /// Define an integral storage type that can hold the value.
    typedef typename etl::private_atomic::storage_type<T>::type storage_t;

    ETL_STATIC_ASSERT(sizeof(storage_t) >= sizeof(T), "Cannot assign atomic storage for T");

#if defined(ETL_COMPILER_GCC)
    ETL_STATIC_ASSERT(__has_trivial_copy(T), "Type is not trivially copyable");
#else
    ETL_STATIC_ASSERT(__is_trivially_copyable(T), "Type is not trivially copyable");
#endif

  public:

    //*************************************************************************
    /// Default constructor
    //*************************************************************************
    atomic()
      : value(storage_t())
    { 
      T v = T();
      memcpy(&value, &v, sizeof(v));
    }

    //*************************************************************************
    /// Contruct from a value
    //*************************************************************************
    atomic(T v)
      : value(storage_t())
    {
      memcpy(&value, &v, sizeof(v));
    }

    //*************************************************************************
    /// Assignment
    //*************************************************************************
    T operator =(T v)
    {
      store(v);

      return v;
    }

    //*************************************************************************
    /// Assignment
    /// Volatile
    //*************************************************************************
    T operator =(T v) volatile
    {
      store(v);

      return v;
    }

    //*************************************************************************
    /// Conversion operator to T
    //*************************************************************************
    operator T() const
    {
      return load();
    }

    //*************************************************************************
    /// Conversion operator to T
    /// Volatile
    //*************************************************************************
    operator T() volatile const
    {
      return load();
    }

    //*************************************************************************
    /// Is lock free?
    //*************************************************************************
    bool is_lock_free() const
    {
      return __atomic_is_lock_free(sizeof(T), &value);
    }

    //*************************************************************************
    /// Is lock free?
    /// Volatile
    //*************************************************************************
    bool is_lock_free() const volatile
    {
      return __atomic_is_lock_free(sizeof(T), &value);
    }

    //*************************************************************************
    /// Store
    //*************************************************************************
    void store(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      storage_t s = 0;

      memcpy(&s, &v, sizeof(T));

      __atomic_store_n(&value, s, order);
    }

    //*************************************************************************
    /// Store
    /// Volatile
    //*************************************************************************
    void store(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      storage_t s = 0;

      memcpy(&s, &v, sizeof(T));

      __atomic_store_n(&value, s, order);
    }

    //*************************************************************************
    /// Load
    //*************************************************************************
    T load(etl::memory_order order = etl::memory_order_seq_cst) const
    {
      storage_t s = __atomic_load_n(&value, order);
      etl::uninitialized_buffer_of<T, 1> buffer;

      memcpy(buffer.data(), &s, sizeof(T));

      return buffer; // Implicit conversion to T.
    }

    //*************************************************************************
    /// Load
    /// Volatile
    //*************************************************************************
    T load(etl::memory_order order = etl::memory_order_seq_cst) const volatile
    {
      storage_t s = __atomic_load_n(&value, order);
      etl::uninitialized_buffer_of<T, 1> buffer;

      memcpy(buffer.data(), &s, sizeof(T));

      return buffer; // Implicit conversion to T.
    }

    //*************************************************************************
    /// Exchange
    //*************************************************************************
    T exchange(T v, etl::memory_order order = etl::memory_order_seq_cst)
    {
      storage_t next;
      memcpy(&next, &v, sizeof(T));

      storage_t prev = __atomic_exchange_n(&value, next, order);

      etl::uninitialized_buffer_of<T, 1> buffer;
      memcpy(buffer.data(), &prev, sizeof(T));

      return buffer; // Implicit conversion to T.
    }

    //*************************************************************************
    /// Exchange
    /// Volatile
    //*************************************************************************
    T exchange(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      storage_t next;
      memcpy(&next, &v, sizeof(T));

      storage_t prev = __atomic_exchange_n(&value, next, order);

      etl::uninitialized_buffer_of<T, 1> buffer;
      memcpy(buffer.data(), &prev, sizeof(T));

      return buffer; // Implicit conversion to T.
    }

    //*************************************************************************
    /// Compare exchange weak
    //*************************************************************************
    bool compare_exchange_weak(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst)
    {
      storage_t expected_s;
      memcpy(&expected_s, &expected, sizeof(T));

      storage_t desired_s;
      memcpy(&desired_s, &desired, sizeof(T));

      bool pass = __atomic_compare_exchange_n(&value, &expected_s, desired_s, true, order, order);

      if (pass)
      {
        memcpy(reinterpret_cast<char*>(&expected), &expected_s, sizeof(T));
      }

      return pass;
    }

    //*************************************************************************
    /// Compare exchange weak
    /// Volatile
    //*************************************************************************
    bool compare_exchange_weak(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      storage_t expected_s;
      memcpy(&expected_s, &expected, sizeof(T));

      storage_t desired_s;
      memcpy(&desired_s, &desired, sizeof(T));

      bool pass = __atomic_compare_exchange_n(&value, &expected_s, desired_s, true, order, order);

      if (pass)
      {
        memcpy(reinterpret_cast<char*>(&expected), &expected_s, sizeof(T));
      }

      return pass;
    }

    //*************************************************************************
    /// Compare exchange weak
    /// Success/Failure
    //*************************************************************************
    bool compare_exchange_weak(T& expected, T desired, etl::memory_order success, etl::memory_order failure)
    {
      storage_t expected_s;
      memcpy(&expected_s, &expected, sizeof(T));

      storage_t desired_s;
      memcpy(&desired_s, &desired, sizeof(T));

      bool pass = __atomic_compare_exchange_n(&value, &expected_s, desired_s, true, success, failure);

      if (pass)
      {
        memcpy(reinterpret_cast<char*>(&expected), &expected_s, sizeof(T));
      }

      return pass;
    }

    //*************************************************************************
    /// Compare exchange weak
    /// Success/Failure
    /// Volatile
    //*************************************************************************
    bool compare_exchange_weak(T& expected, T desired, etl::memory_order success, etl::memory_order failure) volatile
    {
      storage_t expected_s;
      memcpy(&expected_s, &expected, sizeof(T));

      storage_t desired_s;
      memcpy(&desired_s, &desired, sizeof(T));

      bool pass = __atomic_compare_exchange_n(&value, &expected_s, desired_s, true, success, failure);

      if (pass)
      {
        memcpy(reinterpret_cast<char*>(&expected), &expected_s, sizeof(T));
      }

      return pass;
    }

    //*************************************************************************
    /// Compare exchange strong
    //*************************************************************************
    bool compare_exchange_strong(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst)
    {
      storage_t expected_s;
      memcpy(&expected_s, &expected, sizeof(T));

      storage_t desired_s;
      memcpy(&desired_s, &desired, sizeof(T));

      bool pass = __atomic_compare_exchange_n(&value, &expected_s, desired_s, false, order, order);

      if (pass)
      {
        memcpy(reinterpret_cast<char*>(&expected), &expected_s, sizeof(T));
      }

      return pass;
    }

    //*************************************************************************
    /// Compare exchange strong
    /// Volatile
    //*************************************************************************
    bool compare_exchange_strong(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst) volatile
    {
      storage_t expected_s;
      memcpy(&expected_s, &expected, sizeof(T));

      storage_t desired_s;
      memcpy(&desired_s, &desired, sizeof(T));

      bool pass = __atomic_compare_exchange_n(&value, &expected_s, desired_s, false, order, order);

      if (pass)
      {
        memcpy(reinterpret_cast<char*>(&expected), &expected_s, sizeof(T));
      }

      return pass;
    }

    //*************************************************************************
    /// Compare exchange strong
    /// Success/Failure
    //*************************************************************************
    bool compare_exchange_strong(T& expected, T desired, etl::memory_order success, etl::memory_order failure)
    {
      storage_t expected_s;
      memcpy(&expected_s, &expected, sizeof(T));

      storage_t desired_s;
      memcpy(&desired_s, &desired, sizeof(T));

      bool pass = __atomic_compare_exchange_n(&value, &expected_s, desired_s, false, success, failure);

      if (pass)
      {
        memcpy(reinterpret_cast<char*>(&expected), &expected_s, sizeof(T));
      }

      return pass;
    }

    //*************************************************************************
    /// Compare exchange strong
    /// Success/Failure
    /// Volatile
    //*************************************************************************
    bool compare_exchange_strong(T& expected, T desired, etl::memory_order success, etl::memory_order failure) volatile
    {
      storage_t expected_s;
      memcpy(&expected_s, &expected, sizeof(T));

      storage_t desired_s;
      memcpy(&desired_s, &desired, sizeof(T));

      bool pass = __atomic_compare_exchange_n(&value, &expected_s, desired_s, false, success, failure);

      if (pass)
      {
        memcpy(reinterpret_cast<char*>(&expected), &expected_s, sizeof(T));
      }

      return pass;
    }

  private:

    atomic& operator =(const atomic&) ETL_DELETE;
    atomic& operator =(const atomic&) volatile ETL_DELETE;

    mutable storage_t value;
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
#if ETL_HAS_NATIVE_CHAR8_T
  typedef etl::atomic<char8_t>             atomic_char8_t;
#endif
#if ETL_HAS_NATIVE_CHAR16_T
  typedef etl::atomic<char16_t>            atomic_char16_t;
#endif
#if ETL_HAS_NATIVE_CHAR32_T
  typedef etl::atomic<char32_t>            atomic_char32_t;
#endif
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

#endif

