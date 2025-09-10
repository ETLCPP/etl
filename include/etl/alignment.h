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

#ifndef ETL_ALIGNMENT_INCLUDED
#define ETL_ALIGNMENT_INCLUDED

#include "platform.h"
#include "type_traits.h"
#include "static_assert.h"
#include "error_handler.h"
#include "exception.h"
#include "utility.h"
#include "algorithm.h"

#include <stdint.h>

///\defgroup alignment alignment
/// Creates a variable of the specified type at the specified alignment.
/// \ingroup utilities

namespace etl
{
  //***************************************************************************
  /// Exception base for alignment
  //***************************************************************************
  class alignment_exception : public etl::exception
  {
  public:

    alignment_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Memory misalignment exception.
  //***************************************************************************
  class alignment_error : public alignment_exception
  {
  public:

    alignment_error(string_type file_name_, numeric_type line_number_)
      : alignment_exception(ETL_ERROR_TEXT("alignment:error", ETL_ALIGNMENT_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Typed storage exception.
  //***************************************************************************
  class typed_storage_error : public alignment_exception
  {
  public:

    typed_storage_error(string_type file_name_, numeric_type line_number_)
      : alignment_exception(ETL_ERROR_TEXT("typed_storage:error", ETL_ALIGNMENT_FILE_ID"B"), file_name_, line_number_)
    {
    }
  };

  //*****************************************************************************
  /// Check that 'p' has 'required_alignment'.
  //*****************************************************************************
  inline bool is_aligned(const void* p, size_t required_alignment)
  {
    uintptr_t address = reinterpret_cast<uintptr_t>(p);
    return (address % required_alignment) == 0U;
  }

  //*****************************************************************************
  /// Check that 'p' has 'Alignment'.
  //*****************************************************************************
  template <size_t Alignment>
  bool is_aligned(const void* p)
  {
    uintptr_t address = reinterpret_cast<uintptr_t>(p);
    return (address % Alignment) == 0U;
  }

  //*****************************************************************************
  /// Check that 'p' has the alignment of 'T'.
  //*****************************************************************************
  template <typename T>
  bool is_aligned(const void* p)
  {
    return is_aligned<etl::alignment_of<T>::value>(p);
  }

  namespace private_alignment
  {
#if ETL_USING_CPP11
    //***************************************************************************
    // Matcher.
    //***************************************************************************
    template <bool Is_Match, size_t Alignment, typename... TRest>
    class type_with_alignment_matcher;

    // Matching alignment.
    template <size_t Alignment, typename T1, typename... TRest>
    class type_with_alignment_matcher<true, Alignment, T1, TRest...> 
    {
    public:
    
      typedef T1 type;
    };

    // Non-matching alignment
    template <size_t Alignment, typename T1, typename T2, typename... TRest>
    class type_with_alignment_matcher <false, Alignment, T1, T2, TRest...> 
    {
    public:
    
      typedef typename type_with_alignment_matcher < Alignment <= etl::alignment_of<T2>::value , Alignment, T2, TRest... > ::type type;
    };

    // Non-matching alignment, none left.
    template <size_t Alignment, typename T1>
    class type_with_alignment_matcher <false, Alignment, T1> 
    {
    public:
    
      typedef char type;
    };

    //***************************************************************************
    // Helper.
    //***************************************************************************
    template <size_t Alignment, typename T1, typename... T>
    class type_with_alignment_helper 
    {
    public:
    
      typedef typename type_with_alignment_matcher<Alignment <= etl::alignment_of<T1>::value, Alignment, T1, T...>::type type;
    };
#else
    //***************************************************************************
    // Matcher.
    //***************************************************************************
    template <bool Is_Match, const size_t Alignment, typename T1 = void, typename T2 = void, typename T3 = void, typename T4 = void, 
                                                     typename T5 = void, typename T6 = void, typename T7 = void, typename T8 = void>
    class type_with_alignment_matcher;

    // Matching alignment.
    template <size_t Alignment, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    class type_with_alignment_matcher <true, Alignment, T1, T2, T3, T4, T5, T6, T7, T8>
    {
    public:

      typedef T1 type;
    };

    // Non-matching alignment.
    template <size_t Alignment, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    class type_with_alignment_matcher <false, Alignment, T1, T2, T3, T4, T5, T6, T7, T8>
    {
    public:

      typedef typename type_with_alignment_matcher<Alignment <= etl::alignment_of<T2>::value, Alignment, T2, T3, T4, T5, T6, T7, T8, void>::type type;
    };

    // Non-matching alignment, none left.
    template <size_t Alignment>
    class type_with_alignment_matcher <false, Alignment, void, void, void, void, void, void, void, void>
    {
    public:

      typedef char type;
    };

    //***************************************************************************
    // Helper.
    //***************************************************************************
    template <size_t Alignment, typename T1,        typename T2 = void, typename T3 = void, typename T4 = void,
                                typename T5 = void, typename T6 = void, typename T7 = void, typename T8 = void>
    class type_with_alignment_helper
    {
    public:

      typedef typename type_with_alignment_matcher<Alignment <= etl::alignment_of<T1>::value, Alignment, T1, T2, T3, T4, T5, T6, T7, T8>::type type;
    };
#endif
  }

  //***************************************************************************
  /// Gets a type that has the same as the specified alignment.
  ///\ingroup alignment
  //***************************************************************************
  template <size_t Alignment>
  class type_with_alignment
  {
  public:

#if ETL_USING_CPP11
    typedef struct { alignas(Alignment) char dummy; } type;
#else
  #if ETL_NOT_USING_64BIT_TYPES
      typedef typename private_alignment::type_with_alignment_helper<Alignment, int_least8_t, int_least16_t, int32_t, float, double, void*>::type type;
  #else
      typedef typename private_alignment::type_with_alignment_helper<Alignment, int_least8_t, int_least16_t, int32_t, int64_t, float, double, void*>::type type;
  #endif
#endif

    ETL_STATIC_ASSERT(etl::alignment_of<type>::value == Alignment, "Unable to create the type with the specified alignment");
  };

#if ETL_USING_CPP11
  template <size_t Alignment>
  using type_with_alignment_t = typename type_with_alignment<Alignment>::type;
#endif

  //***************************************************************************
  /// Aligned storage
  /// Length should be determined in terms of sizeof()
  ///\ingroup alignment
  //***************************************************************************
  template <size_t Length, const size_t Alignment>
  struct aligned_storage
  {
    struct type
    {
      /// Convert to T reference.
      template <typename T>
      operator T& ()
      {
        ETL_STATIC_ASSERT((etl::is_same<T*, void*>:: value || ((Alignment % etl::alignment_of<T>::value) == 0)), "Incompatible alignment");
        T* t = *this;
        return *t;
      }

      /// Convert to const T reference.
      template <typename T>
      operator const T& () const
      {
        ETL_STATIC_ASSERT((etl::is_same<T*, void*>:: value || ((Alignment % etl::alignment_of<T>::value) == 0)), "Incompatible alignment");
        const T* t = *this;
        return *t;
      }

      /// Convert to T pointer.
      template <typename T>
      operator T* ()
      {
        ETL_STATIC_ASSERT((etl::is_same<T*, void*>:: value || ((Alignment % etl::alignment_of<T>::value) == 0)), "Incompatible alignment");
        return reinterpret_cast<T*>(data);
      }

      /// Convert to const T pointer.
      template <typename T>
      operator const T* () const
      {
        ETL_STATIC_ASSERT((etl::is_same<T*, void*>:: value || ((Alignment % etl::alignment_of<T>::value) == 0)), "Incompatible alignment");
        return reinterpret_cast<const T*>(data);
      }

      /// Get address as T reference.
      template <typename T>
      T& get_reference()
      {
        ETL_STATIC_ASSERT((etl::is_same<T*, void*>:: value || ((Alignment % etl::alignment_of<T>::value) == 0)), "Incompatible alignment");
        T* t = *this;
        return *t;
      }

      /// Get address as const T reference.
      template <typename T>
      const T& get_reference() const
      {
        ETL_STATIC_ASSERT((etl::is_same<T*, void*>:: value || ((Alignment % etl::alignment_of<T>::value) == 0)), "Incompatible alignment");
        const T* t = *this;
        return *t;
      }

      /// Get address as T pointer.
      template <typename T>
      T* get_address()
      {
        ETL_STATIC_ASSERT((etl::is_same<T*, void*>:: value || ((Alignment % etl::alignment_of<T>::value) == 0)), "Incompatible alignment");
        return reinterpret_cast<T*>(data);
      }

      /// Get address as const T pointer.
      template <typename T>
      const T* get_address() const
      {
        ETL_STATIC_ASSERT((etl::is_same<T*, void*>:: value || ((Alignment % etl::alignment_of<T>::value) == 0)), "Incompatible alignment");
        return reinterpret_cast<const T*>(data);
      }

#if ETL_USING_CPP11 && !defined(ETL_COMPILER_ARM5)
      alignas(Alignment) char data[Length];
#else
      union
      {
        char data[Length];
        typename etl::type_with_alignment<Alignment>::type etl_alignment_type; // A POD type that has the same alignment as Alignment.
      };
#endif
    };
  };

#if ETL_USING_CPP11
  template <size_t Length, const size_t Alignment>
  using aligned_storage_t = typename aligned_storage<Length, Alignment>::type;
#endif

  //***************************************************************************
  /// Aligned storage as
  ///\ingroup alignment
  //***************************************************************************
  template <size_t Length, typename T>
  struct aligned_storage_as : public etl::aligned_storage<Length, etl::alignment_of<T>::value>
  {
  };

#if ETL_USING_CPP11
  template <size_t Length, typename T>
  using aligned_storage_as_t = typename aligned_storage_as<Length, T>::type;
#endif

  //***************************************************************************
  /// Wrapper class that provides a memory area and lets the user create an
  /// instance of T in this memory at runtime. This class also erases the
  /// destructor call of T, i.e. if typed_storage goes out of scope, the
  /// destructor if the wrapped type will not be called. This can be done
  /// explicitly by calling destroy().
  /// \tparam T    Type of element stored in this instance of typed_storage.
  //***************************************************************************
  template <typename T>
  class typed_storage
  {
  public:

    typedef T        value_type;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef T*       pointer;
    typedef const T* const_pointer;

    //***************************************************************************
    // Default constructor
    //***************************************************************************
    typed_storage() ETL_NOEXCEPT
      : valid(false)
    {
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Constructs the instance of T forwarding the given \p args to its constructor.
    //***************************************************************************
    template <typename... TArgs>
    typed_storage(TArgs&&... args) ETL_NOEXCEPT_EXPR(ETL_NOT_USING_EXCEPTIONS)
      : valid(false)
    {
      create(etl::forward<TArgs>(args)...);
    }
#else
    //***************************************************************************
    /// Constructs the instance of T with type T1
    //***************************************************************************
    template <typename T1>
    typed_storage(const T1& t1)
      : valid(false)
    {
      create(t1);
    }

    //***************************************************************************
    /// Constructs the instance of T with types T1, T2
    //***************************************************************************
    template <typename T1, typename T2>
    typed_storage(const T1& t1, const T2& t2)
      : valid(false)
    {
      create(t1, t2);
    }

    //***************************************************************************
    /// Constructs the instance of T with types T1, T2, T3
    //***************************************************************************
    template <typename T1, typename T2, typename T3>
    typed_storage(const T1& t1, const T2& t2, const T3& t3)
      : valid(false)
    {
      create(t1, t2, t3);
    }

    //***************************************************************************
    /// Constructs the instance of T with types T1, T2, T3, T4
    //***************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    typed_storage(const T1& t1, const T2& t2, const T3& t3, const T4& t4)
      : valid(false)
    {
      create(t1, t2, t3, t4);
    }
#endif

    //***************************************************************************
    /// Default destructor which will NOT call the destructor of the object which
    /// was created by calling create().
    //***************************************************************************
    ~typed_storage() ETL_NOEXCEPT
    {
      // Intentionally empty.
    }

    //***************************************************************************
    /// \returns <b>true</b> if object has been constructed using create().
    /// \returns <b>false</b> otherwise.
    //***************************************************************************
    bool has_value() const ETL_NOEXCEPT
    {
      return valid;
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Constructs the instance of T forwarding the given \p args to its constructor.
    /// \returns the instance of T which has been constructed in the internal byte array.
    //***************************************************************************
    template <typename... TArgs>
    reference create(TArgs&&... args) ETL_NOEXCEPT_EXPR(ETL_NOT_USING_EXCEPTIONS)
    {
      ETL_ASSERT(!has_value(), ETL_ERROR(etl::typed_storage_error));
      pointer p = ::new (&storage.value) value_type(etl::forward<TArgs>(args)...);
      valid = true;
      return *p;
    }
#else
    //***************************************************************************
    /// Constructs the instance of T with type T1
    /// \returns the instance of T which has been constructed in the internal byte array.
    //***************************************************************************
    template <typename T1>
    reference create(const T1& t1)
    {
      ETL_ASSERT(!has_value(), ETL_ERROR(etl::typed_storage_error));
      pointer p = ::new (&storage.value) value_type(t1);
      valid = true;
      return *p;
    }

    //***************************************************************************
    /// Constructs the instance of T with types T1, T2
    /// \returns the instance of T which has been constructed in the internal byte array.
    //***************************************************************************
    template <typename T1, typename T2>
    reference create(const T1& t1, const T2& t2)
    {
      ETL_ASSERT(!has_value(), ETL_ERROR(etl::typed_storage_error));
      pointer p = ::new (&storage.value) value_type(t1, t2);
      valid = true;
      return *p;
    }

    //***************************************************************************
    /// Constructs the instance of T with types T1, T2, T3
    /// \returns the instance of T which has been constructed in the internal byte array.
    //***************************************************************************
    template <typename T1, typename T2, typename T3>
    reference create(const T1& t1, const T2& t2, const T3& t3)
    {
      ETL_ASSERT(!has_value(), ETL_ERROR(etl::typed_storage_error));
      pointer p = ::new (&storage.value) value_type(t1, t2, t3);
      valid = true;
      return *p;
    }

    //***************************************************************************
    /// Constructs the instance of T with types T1, T2, T3, T4
    /// \returns the instance of T which has been constructed in the internal byte array.
    //***************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    reference create(const T1& t1, const T2& t2, const T3& t3, const T4& t4)
    {
      ETL_ASSERT(!has_value(), ETL_ERROR(etl::typed_storage_error));
      pointer p = ::new (&storage.value) value_type(t1, t2, t3, t4);
      valid = true;
      return *p;
    }
#endif

    //***************************************************************************
    /// Calls the destructor of the stored object, if created.
    //***************************************************************************
    void destroy() ETL_NOEXCEPT
    {
      if (has_value())
      {
        storage.value.~T();
        valid = false;
      }
    }

    //***************************************************************************
    /// \returns a pointer of type T and asserts if has_value() is false.
    //***************************************************************************
    pointer operator->() ETL_NOEXCEPT_EXPR(ETL_NOT_USING_EXCEPTIONS)
    {
      ETL_ASSERT(has_value(), ETL_ERROR(etl::typed_storage_error));

      return &storage.value;
    }

    //***************************************************************************
    /// \returns a const pointer of type T and asserts if has_value() is false.
    //***************************************************************************
    const_pointer operator->() const ETL_NOEXCEPT_EXPR(ETL_NOT_USING_EXCEPTIONS)
    {
      ETL_ASSERT(has_value(), ETL_ERROR(etl::typed_storage_error));
      
      return &storage.value;
    }

    //***************************************************************************
    /// \returns reference of type T and asserts if has_value() is false.
    //***************************************************************************
    reference operator*() ETL_NOEXCEPT_EXPR(ETL_NOT_USING_EXCEPTIONS)
    {
      return *operator->();
    }

    //***************************************************************************
    /// \returns const_reference of type T and asserts if has_value() is false.
    //***************************************************************************
    const_reference operator*() const ETL_NOEXCEPT_EXPR(ETL_NOT_USING_EXCEPTIONS)
    {
      return *operator->();
    }

  private:

    typed_storage(etl::typed_storage<T>&) ETL_DELETE;
    typed_storage& operator =(etl::typed_storage<T>&) ETL_DELETE;

    struct dummy_t {};

   //*******************************
    union union_type
    {
      union_type() ETL_NOEXCEPT
        : dummy()
      {
      }

      ~union_type() ETL_NOEXCEPT
      {
      }

      dummy_t    dummy;
      value_type value;
    } storage;

    bool valid;
  };

  //***************************************************************************
  /// Wrapper class wraps a supplied memory area and lets the user create an
  /// instance of T in this memory at runtime. This class also erases the
  /// destructor call of T, i.e. if typed_storage goes out of scope, the
  /// destructor if the wrapped type will not be called. This can be done
  /// explicitly by calling destroy().
  /// \tparam T    Type of element stored in this instance of typed_storage.
  //***************************************************************************
  template <typename T>
  class typed_storage_ext
  {
  public:

    typedef T        value_type;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef T*       pointer;
    typedef const T* const_pointer;
    
    template <typename U>
    friend ETL_CONSTEXPR14 void swap(typed_storage_ext<U>& lhs, typed_storage_ext<U>& rhs) ETL_NOEXCEPT;

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    typed_storage_ext(void* pbuffer_) ETL_NOEXCEPT_EXPR(ETL_NOT_USING_EXCEPTIONS)
      : pbuffer(reinterpret_cast<T*>(pbuffer_)),
        valid(false)
    {
      ETL_ASSERT(etl::is_aligned(pbuffer_, etl::alignment_of<T>::value), ETL_ERROR(etl::alignment_error));
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Constructs the instance of T forwarding the given \p args to its constructor.
    //***************************************************************************
    template <typename... TArgs>
    typed_storage_ext(void* pbuffer_, TArgs&&... args) ETL_NOEXCEPT_EXPR(ETL_NOT_USING_EXCEPTIONS)
      : pbuffer(reinterpret_cast<T*>(pbuffer_))
      , valid(false)
    {
      ETL_ASSERT(etl::is_aligned(pbuffer_, etl::alignment_of<T>::value), ETL_ERROR(etl::alignment_error));
      create(etl::forward<TArgs>(args)...);
    }

    //***************************************************************************
    /// Move constructor.
    /// Transfers ownership of the buffer from \p other to this.
    //***************************************************************************
    typed_storage_ext(typed_storage_ext<T>&& other) ETL_NOEXCEPT_EXPR(ETL_NOT_USING_EXCEPTIONS)
      : pbuffer(other.pbuffer)
      , valid(other.valid)
    {
      other.pbuffer = ETL_NULLPTR;
      other.valid   = false;
    }
#else
    //***************************************************************************
    /// Constructs the instance of T with type T1
    //***************************************************************************
    template <typename T1>
    typed_storage_ext(void* pbuffer_, const T1& t1)
      : pbuffer(reinterpret_cast<T*>(pbuffer_))
      , valid(false)
    {
      ETL_ASSERT(etl::is_aligned(pbuffer_, etl::alignment_of<T>::value), ETL_ERROR(etl::alignment_error));
      create(t1);
    }

    //***************************************************************************
    /// Constructs the instance of T with types T1, T2
    //***************************************************************************
    template <typename T1, typename T2>
    typed_storage_ext(void* pbuffer_, const T1& t1, const T2& t2)
      : pbuffer(reinterpret_cast<T*>(pbuffer_))
      , valid(false)
    {
      ETL_ASSERT(etl::is_aligned(pbuffer_, etl::alignment_of<T>::value), ETL_ERROR(etl::alignment_error));
      create(t1, t2);
    }

    //***************************************************************************
    /// Constructs the instance of T with types T1, T2, T3
    //***************************************************************************
    template <typename T1, typename T2, typename T3>
    typed_storage_ext(void* pbuffer_, const T1& t1, const T2& t2, const T3& t3)
      : pbuffer(reinterpret_cast<T*>(pbuffer_))
      , valid(false)
    {
      ETL_ASSERT(etl::is_aligned(pbuffer_, etl::alignment_of<T>::value), ETL_ERROR(etl::alignment_error));
      create(t1, t2, t3);
    }

    //***************************************************************************
    /// Constructs the instance of T with types T1, T2, T3, T4
    //***************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    typed_storage_ext(void* pbuffer_, const T1& t1, const T2& t2, const T3& t3, const T4& t4)
      : pbuffer(reinterpret_cast<T*>(pbuffer_))
      , valid(false)
    {
      ETL_ASSERT(etl::is_aligned(pbuffer_, etl::alignment_of<T>::value), ETL_ERROR(etl::alignment_error));
      create(t1, t2, t3, t4);
    }
#endif

    //***************************************************************************
    /// Default destructor which will NOT call the destructor of the object which
    /// was created by calling create().
    //***************************************************************************
    ~typed_storage_ext() ETL_NOEXCEPT
    {
      // Intentionally empty.
    }

    //***************************************************************************
    /// \returns <b>true</b> if object has been constructed using create().
    /// \returns <b>false</b> otherwise.
    //***************************************************************************
    bool has_value() const ETL_NOEXCEPT
    {
      return valid;
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Constructs the instance of T forwarding the given \p args to its constructor.
    /// \returns the instance of T which has been constructed in the external buffer.
    //***************************************************************************
    template <typename... TArgs>
    reference create(TArgs&&... args) ETL_NOEXCEPT_EXPR(ETL_NOT_USING_EXCEPTIONS)
    {
      ETL_ASSERT(!has_value(), ETL_ERROR(etl::typed_storage_error));
      pointer p = ::new (pbuffer) value_type(etl::forward<TArgs>(args)...);
      valid = true;
      return *p;
    }
#else
    //***************************************************************************
    /// Constructs the instance of T with type T1
    /// \returns the instance of T which has been constructed in the external buffer.
    //***************************************************************************
    template <typename T1>
    reference create(const T1& t1)
    {
      ETL_ASSERT(!has_value(), ETL_ERROR(etl::typed_storage_error));
      pointer p = ::new (pbuffer) value_type(t1);
      valid = true;
      return *p;
    }

    //***************************************************************************
    /// Constructs the instance of T with types T1, T2
    /// \returns the instance of T which has been constructed in the external buffer.
    //***************************************************************************
    template <typename T1, typename T2>
    reference create(const T1& t1, const T2& t2)
    {
      ETL_ASSERT(!has_value(), ETL_ERROR(etl::typed_storage_error));
      pointer p = ::new (pbuffer) value_type(t1, t2);
      valid = true;
      return *p;
    }

    //***************************************************************************
    /// Constructs the instance of T with types T1, T2, T3
    /// \returns the instance of T which has been constructed in the external buffer.
    //***************************************************************************
    template <typename T1, typename T2, typename T3>
    reference create(const T1& t1, const T2& t2, const T3& t3)
    {
      ETL_ASSERT(!has_value(), ETL_ERROR(etl::typed_storage_error));
      pointer p = ::new (pbuffer) value_type(t1, t2, t3);
      valid = true;
      return *p;
    }

    //***************************************************************************
    /// Constructs the instance of T with types T1, T2, T3, T4
    /// \returns the instance of T which has been constructed in the external buffer.
    //***************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    reference create(const T1& t1, const T2& t2, const T3& t3, const T4& t4)
    {
      ETL_ASSERT(!has_value(), ETL_ERROR(etl::typed_storage_error));
      pointer p = ::new (pbuffer) value_type(t1, t2, t3, t4);
      valid = true;
      return *p;
    }
#endif

    //***************************************************************************
    /// Calls the destructor of the stored object, if created.
    //***************************************************************************
    void destroy() ETL_NOEXCEPT
    {
      if (has_value())
      {
        pbuffer->~T();
        valid = false;
      }
    }

    //***************************************************************************
    /// \returns a pointer of type T and asserts if has_value() is false.
    //***************************************************************************
    pointer operator->() ETL_NOEXCEPT_EXPR(ETL_NOT_USING_EXCEPTIONS)
    {
      ETL_ASSERT(has_value(), ETL_ERROR(etl::typed_storage_error));

      return pbuffer;
    }

    //***************************************************************************
    /// \returns a const pointer of type T and asserts if has_value() is false.
    //***************************************************************************
    const_pointer operator->() const ETL_NOEXCEPT_EXPR(ETL_NOT_USING_EXCEPTIONS)
    {
      ETL_ASSERT(has_value(), ETL_ERROR(etl::typed_storage_error));

      return pbuffer;
    }

    //***************************************************************************
    /// \returns reference of type T and asserts if has_value() is false.
    //***************************************************************************
    reference operator*() ETL_NOEXCEPT_EXPR(ETL_NOT_USING_EXCEPTIONS)
    {
      return *operator->();
    }

    //***************************************************************************
    /// \returns const_reference of type T and asserts if has_value() is false.
    //***************************************************************************
    const_reference operator*() const ETL_NOEXCEPT_EXPR(ETL_NOT_USING_EXCEPTIONS)
    {
      return *operator->();
    }

  private:

    typed_storage_ext(etl::typed_storage_ext<T>&) ETL_DELETE;
    typed_storage_ext& operator =(etl::typed_storage_ext<T>&) ETL_DELETE;

    pointer pbuffer;
    bool valid;
  };

  //***************************************************************************
  /// Swap two etl::typed_storage_ext
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 void swap(etl::typed_storage_ext<T>& lhs, etl::typed_storage_ext<T>& rhs) ETL_NOEXCEPT
  {
    using ETL_OR_STD::swap;

    swap(lhs.pbuffer, rhs.pbuffer);
    swap(lhs.valid,   rhs.valid);
  }
}

#endif
