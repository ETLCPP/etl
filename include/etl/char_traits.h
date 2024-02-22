///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 John Wellbelove

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

#ifndef ETL_CHAR_TRAITS_INCLUDED
#define ETL_CHAR_TRAITS_INCLUDED

#include "platform.h"
#include "algorithm.h"
#include "iterator.h"

#include <stdint.h>

//*****************************************************************************
///\defgroup char_traits char_traits
/// Character traits
///\ingroup string
//*****************************************************************************

namespace etl
{
  template<typename T> struct char_traits_types;

  template<> struct char_traits_types<char>
  {
    typedef char      char_type;
    typedef int       int_type;
    typedef long long off_type;
    typedef size_t    pos_type;
    typedef char      state_type;
  };

  template<> struct char_traits_types<wchar_t>
  {
    typedef wchar_t        char_type;
    typedef uint_least16_t int_type;
    typedef long long      off_type;
    typedef size_t         pos_type;
    typedef char           state_type;
  };

#if ETL_USING_CPP20
  template<> struct char_traits_types<char8_t>
  {
    typedef char8_t       char_type;
    typedef unsigned int  int_type;
    typedef long long     off_type;
    typedef size_t        pos_type;
    typedef char          state_type;
  };
#endif

  template<> struct char_traits_types<char16_t>
  {
    typedef char16_t       char_type;
    typedef uint_least16_t int_type;
    typedef long long      off_type;
    typedef size_t         pos_type;
    typedef char           state_type;
  };

  template<> struct char_traits_types<char32_t>
  {
    typedef char32_t       char_type;
    typedef uint_least32_t int_type;
    typedef long long      off_type;
    typedef size_t         pos_type;
    typedef char           state_type;
  };

  //***************************************************************************
  /// Character traits for any character type.
  //***************************************************************************
  template<typename T>
  struct char_traits : public char_traits_types<T>
  {
    typedef typename char_traits_types<T>::char_type  char_type;
    typedef typename char_traits_types<T>::int_type   int_type;
    typedef typename char_traits_types<T>::off_type   off_type;
    typedef typename char_traits_types<T>::pos_type   pos_type;
    typedef typename char_traits_types<T>::state_type state_type;

    //*************************************************************************
    static ETL_CONSTEXPR bool eq(char_type a, char_type b)
    {
      return a == b;
    }

    //*************************************************************************
    static ETL_CONSTEXPR bool lt(char_type a, char_type b)
    {
      return a < b;
    }

    //*************************************************************************
    static ETL_CONSTEXPR14 size_t length(const char_type* str)
    {
      size_t count = 0UL;

      if (str != 0)
      {
        while (*str++ != 0)
        {
          ++count;
        }
      }

      return count;
    }

    //*************************************************************************
    ETL_CONSTEXPR14 static size_t length(const char_type* str, size_t max_length)
    {
      size_t count = 0UL;

      if (str != 0)
      {
        while ((count < max_length) && (*str++ != 0))
        {
          ++count;
        }
      }

      return count;
    }

    //*************************************************************************
    static ETL_CONSTEXPR14 void assign(char_type& r, const char_type& c)
    {
      r = c;
    }

    //*************************************************************************
    static ETL_CONSTEXPR14 char_type* assign(char_type* p, size_t n, char_type c)
    {
      if (p != ETL_NULLPTR)
      {
        etl::fill_n(p, n, c);
      }

      return p;
    }

    //*************************************************************************
    static ETL_CONSTEXPR14 char_type* move(char_type* dst, const char_type* src, size_t count)
    {
      if ((dst < src) || (dst > (src + count)))
      {
        etl::copy_n(src, count, dst);
      }
      else
      {
        etl::copy_n(ETL_OR_STD::reverse_iterator<const char_type*>(src + count),
                    count,
                    ETL_OR_STD::reverse_iterator<char_type*>(dst + count));
      }

      return dst;
    }

    //*************************************************************************
    static ETL_CONSTEXPR14 char_type* copy(char_type* dst, const char_type* src, size_t count)
    {
      etl::copy_n(src, count, dst);

      return dst;
    }

    //*************************************************************************
    static ETL_CONSTEXPR14 int compare(const char_type* s1, const char_type* s2, size_t count)
    {
      for (size_t i = 0UL; i < count; ++i)
      {
        const char_type c1 = *s1++;
        const char_type c2 = *s2++;

        if (c1 < c2)
        {
          return -1;
        }
        else if (c1 > c2)
        {
          return 1;
        }
      }

      return 0;
    }

    //*************************************************************************
    static ETL_CONSTEXPR14 const char_type* find(const char_type* p, size_t count, const char_type& ch)
    {
      for (size_t i = 0UL; i < count; ++i)
      {
        if (*p == ch)
        {
          return p;
        }

        ++p;
      }

      return 0;
    }

    //*************************************************************************
    static ETL_CONSTEXPR char_type to_char_type(int_type c)
    {
      return static_cast<char_type>(c);
    }

    //*************************************************************************
    static ETL_CONSTEXPR int_type to_int_type(char_type c)
    {
      return static_cast<int_type>(c);
    }

    //*************************************************************************
    static ETL_CONSTEXPR bool eq_int_type(int_type c1, int_type c2)
    {
      return (c1 == c2);
    }

    //*************************************************************************
    static ETL_CONSTEXPR int_type eof()
    {
      return -1;
    }

    //*************************************************************************
    static ETL_CONSTEXPR int_type not_eof(int_type e)
    {
      return (e == eof()) ? eof() - 1 : e;
    }
  };

  //***************************************************************************
  /// Alternative strlen for all character types.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 size_t strlen(const T* t)
  {
    return etl::char_traits<T>::length(t);
  }

  //***************************************************************************
  /// Alternative strlen for all character types, with maximum length.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 size_t strlen(const T* t, size_t max_length)
  {
    return etl::char_traits<T>::length(t, max_length);
  }

  //***************************************************************************
  /// Alternative strcmp for all character types.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 int strcmp(const T* t1, const T* t2)
  {
    while ((*t1 != 0) || (*t2 != 0))
    {
      if (*t1 > *t2)
      {
        return 1;
      }
      
      if (*t1 < *t2)
      {
        return -1;
      }

      ++t1;
      ++t2;
    }

    return 0;
  }

  //***************************************************************************
  /// Alternative strncmp for all character types.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 int strncmp(const T* t1, const T* t2, size_t n)
  {
    while (((*t1 != 0) || (*t2 != 0)) && (n != 0))
    {
      if (*t1 < *t2)
      {
        return -1;
      }
      else if (*t1 > *t2)
      {
        return 1;
      }

      ++t1;
      ++t2;
      --n;
    } 

    return 0;
  }

  //***************************************************************************
  /// Alternative strcpy for all character types.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 T* strcpy(T* dst, const T* src)
  {
    T* result = dst;

    while (*src != 0)
    {
      *dst++ = *src++;
    }

    *dst = 0;

    return result;
  }

  //***************************************************************************
  /// Alternative strncpy for all character types.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 T* strncpy(T* dst, const T* src, size_t n)
  {
    T* result = dst;

    while ((*src != 0) && (n != 0))
    {
      *dst++ = *src++;
      --n;
    }

    *dst = 0;

    return result;
  }
}

#endif
