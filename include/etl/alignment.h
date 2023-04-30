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

  //*****************************************************************************
  /// Check that 'p' has 'required_alignment'.
  //*****************************************************************************
  inline bool is_aligned(void* p, size_t required_alignment)
  {
    uintptr_t alignment = static_cast<uintptr_t>(required_alignment);
    uintptr_t address = reinterpret_cast<uintptr_t>(p);
    return (address % alignment) == 0U;
  }

  //*****************************************************************************
  /// Check that 'p' has 'Alignment'.
  //*****************************************************************************
  template <size_t Alignment>
  bool is_aligned(void* p)
  {
    uintptr_t address = reinterpret_cast<uintptr_t>(p);
    return (address % static_cast<uintptr_t>(Alignment)) == 0U;
  }

  //*****************************************************************************
  /// Check that 'p' has the alignment of 'T'.
  //*****************************************************************************
  template <typename T>
  bool is_aligned(void* p)
  {
    return is_aligned<etl::alignment_of<T>::value>(p);
  }

  namespace private_alignment
  {
    //***************************************************************************
    // Matcher.
    //***************************************************************************
    template <bool IS_MATCH, const size_t ALIGNMENT, typename T1 = void, typename T2 = void, typename T3 = void, typename T4 = void, 
                                                     typename T5 = void, typename T6 = void, typename T7 = void, typename T8 = void>
    class type_with_alignment_matcher;

    // Matching alignment.
    template <size_t ALIGNMENT, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    class type_with_alignment_matcher <true, ALIGNMENT, T1, T2, T3, T4, T5, T6, T7, T8>
    {
    public:

      typedef T1 type;
    };

    // Non-matching alignment.
    template <size_t ALIGNMENT, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    class type_with_alignment_matcher <false, ALIGNMENT, T1, T2, T3, T4, T5, T6, T7, T8>
    {
    public:

      typedef typename type_with_alignment_matcher<ALIGNMENT <= etl::alignment_of<T2>::value, ALIGNMENT, T2, T3, T4, T5, T6, T7, T8, void>::type type;
    };

    // Non-matching alignment, none left.
    template <size_t ALIGNMENT>
    class type_with_alignment_matcher <false, ALIGNMENT, void, void, void, void, void, void, void, void>
    {
    public:

      typedef char type;
    };

    //***************************************************************************
    // Helper.
    //***************************************************************************
    template <size_t ALIGNMENT, typename T1,        typename T2 = void, typename T3 = void, typename T4 = void,
                                      typename T5 = void, typename T6 = void, typename T7 = void, typename T8 = void>
    class type_with_alignment_helper
    {
    public:

      typedef typename type_with_alignment_matcher<ALIGNMENT <= etl::alignment_of<T1>::value, ALIGNMENT, T1, T2, T3, T4, T5, T6, T7, T8>::type type;
    };
  }

  //***************************************************************************
  /// Gets a type that has the same as the specified alignment.
  ///\ingroup alignment
  //***************************************************************************
  template <size_t ALIGNMENT>
  class type_with_alignment
  {
  public:

#if ETL_NOT_USING_64BIT_TYPES
    typedef typename private_alignment::type_with_alignment_helper<ALIGNMENT, int_least8_t, int_least16_t, int32_t, float, double, void*>::type type;
#else
    typedef typename private_alignment::type_with_alignment_helper<ALIGNMENT, int_least8_t, int_least16_t, int32_t, int64_t, float, double, void*>::type type;
#endif
  };

  //***************************************************************************
  /// Aligned storage
  /// LENGTH should be determined in terms of sizeof()
  ///\ingroup alignment
  //***************************************************************************
  template <size_t LENGTH, const size_t ALIGNMENT>
  struct aligned_storage
  {
    struct type
    {
      //type() 
      //  : data()
      //{
      //}

      /// Convert to T reference.
      template <typename T>
      operator T& ()
      {
        ETL_STATIC_ASSERT((etl::is_same<T*, void*>:: value || ((ALIGNMENT % etl::alignment_of<T>::value) == 0)), "Incompatible alignment");
        T* t = *this;
        return *t;
      }

      /// Convert to const T reference.
      template <typename T>
      operator const T& () const
      {
        ETL_STATIC_ASSERT((etl::is_same<T*, void*>:: value || ((ALIGNMENT % etl::alignment_of<T>::value) == 0)), "Incompatible alignment");
        const T* t = *this;
        return *t;
      }

      /// Convert to T pointer.
      template <typename T>
      operator T* ()
      {
        ETL_STATIC_ASSERT((etl::is_same<T*, void*>:: value || ((ALIGNMENT % etl::alignment_of<T>::value) == 0)), "Incompatible alignment");
        return reinterpret_cast<T*>(data);
      }

      /// Convert to const T pointer.
      template <typename T>
      operator const T* () const
      {
        ETL_STATIC_ASSERT((etl::is_same<T*, void*>:: value || ((ALIGNMENT % etl::alignment_of<T>::value) == 0)), "Incompatible alignment");
        return reinterpret_cast<const T*>(data);
      }

      /// Get address as T reference.
      template <typename T>
      T& get_reference()
      {
        ETL_STATIC_ASSERT((etl::is_same<T*, void*>:: value || ((ALIGNMENT % etl::alignment_of<T>::value) == 0)), "Incompatible alignment");
        T* t = *this;
        return *t;
      }

      /// Get address as const T reference.
      template <typename T>
      const T& get_reference() const
      {
        ETL_STATIC_ASSERT((etl::is_same<T*, void*>:: value || ((ALIGNMENT % etl::alignment_of<T>::value) == 0)), "Incompatible alignment");
        const T* t = *this;
        return *t;
      }

      /// Get address as T pointer.
      template <typename T>
      T* get_address()
      {
        ETL_STATIC_ASSERT((etl::is_same<T*, void*>:: value || ((ALIGNMENT % etl::alignment_of<T>::value) == 0)), "Incompatible alignment");
        return reinterpret_cast<T*>(data);
      }

      /// Get address as const T pointer.
      template <typename T>
      const T* get_address() const
      {
        ETL_STATIC_ASSERT((etl::is_same<T*, void*>:: value || ((ALIGNMENT % etl::alignment_of<T>::value) == 0)), "Incompatible alignment");
        return reinterpret_cast<const T*>(data);
      }

#if ETL_USING_CPP11 && !defined(ETL_COMPILER_ARM5)
      alignas(ALIGNMENT) char data[LENGTH];
#else
      union
      {
        char data[LENGTH];
        typename etl::type_with_alignment<ALIGNMENT>::type etl_alignment_type; // A POD type that has the same alignment as ALIGNMENT.
      };
#endif
    };
  };

#if ETL_USING_CPP11
  template <size_t LENGTH, const size_t ALIGNMENT>
  using aligned_storage_t = typename aligned_storage<LENGTH, ALIGNMENT>::type;
#endif

  //***************************************************************************
  /// Aligned storage as
  ///\ingroup alignment
  //***************************************************************************
  template <size_t LENGTH, typename T>
  struct aligned_storage_as : public etl::aligned_storage<LENGTH, etl::alignment_of<T>::value>
  {
  };

#if ETL_USING_CPP11
  template <size_t LENGTH, typename T>
  using aligned_storage_as_t = typename aligned_storage_as<LENGTH, T>::type;
#endif
}

#endif
