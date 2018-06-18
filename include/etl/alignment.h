///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2014 jwellbelove

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

#ifndef ETL_ALIGNEMENT_INCLUDED
#define ETL_ALIGNEMENT_INCLUDED

#include <stdint.h>

#include "platform.h"
#include "type_traits.h"
#include "static_assert.h"

///\defgroup alignment alignment
/// Creates a variable of the specified type at the specified alignment.
/// \ingroup utilities

namespace etl
{
  namespace private_alignment
  {
    //***************************************************************************
    // Matcher.
    //***************************************************************************
    template <const bool IS_MATCH, const size_t ALIGNMENT, typename T1 = void, typename T2 = void, typename T3 = void, typename T4 = void, typename T5 = void, typename T6 = void, typename T7 = void, typename T8 = void>
    class type_with_alignment_matcher;

    // Matching alignment.
    template <const size_t ALIGNMENT, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    class type_with_alignment_matcher <true, ALIGNMENT, T1, T2, T3, T4, T5, T6, T7, T8>
    {
    public:

      typedef T1 type;
    };

    // Non-matching alignment.
    template <const size_t ALIGNMENT, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    class type_with_alignment_matcher <false, ALIGNMENT, T1, T2, T3, T4, T5, T6, T7, T8>
    {
    public:

      typedef typename type_with_alignment_matcher<ALIGNMENT <= etl::alignment_of<T2>::value, ALIGNMENT, T2, T3, T4, T5, T6, T7, T8, void>::type type;
    };

    // Non-matching alignment, none left.
    template <const size_t ALIGNMENT>
    class type_with_alignment_matcher <false, ALIGNMENT, void, void, void, void, void, void, void, void>
    {
    public:

      typedef char type;
    };

    //***************************************************************************
    // Helper.
    //***************************************************************************
    template <const size_t ALIGNMENT, typename T1, typename T2 = void, typename T3 = void, typename T4 = void, typename T5 = void, typename T6 = void, typename T7 = void, typename T8 = void>
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
  template <const size_t ALIGNMENT>
  class type_with_alignment
  {
  public:

    typedef typename private_alignment::type_with_alignment_helper<ALIGNMENT, int_least8_t, int_least16_t, int32_t, int64_t, float, double, void*>::type type;
  };

  //***************************************************************************
  /// Aligned storage
  /// LENGTH should be determined in terms of sizeof()
  ///\ingroup alignment
  //***************************************************************************
  template <const size_t LENGTH, const size_t ALIGNMENT>
  struct aligned_storage
  {
    struct type
    {
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

      union
      {
        char data[LENGTH];
        typename etl::type_with_alignment<ALIGNMENT>::type etl_alignment_type; // A POD type that has the same alignment as ALIGNMENT.
      };
    };
  };

  //***************************************************************************
  /// Aligned storage as
  ///\ingroup alignment
  //***************************************************************************
  template <const size_t LENGTH, typename T>
  struct aligned_storage_as : public etl::aligned_storage<LENGTH, etl::alignment_of<T>::value>
  {
  };
}

#endif
