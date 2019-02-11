 ///\file

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

#ifndef ETL_STL_ALTERNATE_UTILITY_INCLUDED
#define ETL_STL_ALTERNATE_UTILITY_INCLUDED

#include "../../platform.h"
#include "../../type_traits.h"

#if defined(ETL_IN_UNIT_TEST)
  #if !defined(ETLSTD)
    #define ETLSTD etlstd
  #endif
  namespace etlstd
#else
  #if !defined(ETLSTD)
    #define ETLSTD std
  #endif
  namespace std
#endif
{
#if !defined(ETL_COMPILER_ARM6)
  //******************************************************************************
  template <typename T1, typename T2>
  struct pair
  {
    typedef T1 first_type;
    typedef T2 second_type;

    T1 first;
    T2 second;

    pair()
      : first(T1()),
        second(T2())
    {
    }

    pair(const T1& a, const T2& b)
      : first(a),
        second(b)
    {
    }

    template <typename U1, typename U2>
    pair(const pair<U1, U2>& other)
      : first(other.first),
        second(other.second)
    {
    }

    pair(const pair<T1, T2>& other)
      : first(other.first),
        second(other.second)
    {
    }

    void swap(pair<T1, T2>& other)
    {
      T1 temp1 = first;
      T2 temp2 = second;
      first  = other.first;
      second = other.second;
      other.first  = temp1;
      other.second = temp2;
    }
  };
#endif
  //******************************************************************************
  template <typename T1, typename T2>
  inline pair<T1, T2> make_pair(T1 a, T2 b)
  {
    return pair<T1, T2>(a, b);
  }

#if !defined(ETL_COMPILER_ARM6)
  //******************************************************************************
  template <typename T1, typename T2>
  inline void swap(pair<T1, T2>& a, pair<T1, T2>& b)
  {
    a.swap(b);
  }

  //******************************************************************************
  template <typename T1, typename T2>
    inline bool operator ==(const pair<T1, T2>& a, const pair<T1, T2>& b)
  {
    return (a.first == b.first) && (a.second == b.second);
  }

  template <typename T1, typename T2>
  inline bool operator !=(const pair<T1, T2>& a, const pair<T1, T2>& b)
  {
    return !(a == b);
  }

  template <typename T1, typename T2>
  inline bool operator <(const pair<T1, T2>& a, const pair<T1, T2>& b)
  {
    return (a.first < b.first) ||
           (!(b.first < a.first) && (a.second < b.second));
  }

  template <typename T1, typename T2>
  inline bool operator >(const pair<T1, T2>& a, const pair<T1, T2>& b)
  {
    return (b < a);
  }

  template <typename T1, typename T2>
  inline bool operator <=(const pair<T1, T2>& a, const pair<T1, T2>& b)
  {
    return !(b < a);
  }

  template <typename T1, typename T2>
  inline bool operator >=(const pair<T1, T2>& a, const pair<T1, T2>& b)
  {
    return !(a < b);
  }
#endif

#if ETL_CPP11_SUPPORTED
  //******************************************************************************
  template <typename T>
  constexpr typename etl::remove_reference<T>::type&& move(T&& t) noexcept
  {
    return static_cast<typename etl::remove_reference<T>::type&&>(t);
  }

  //******************************************************************************
  template <typename T>
  constexpr T&& forward(typename etl::remove_reference<T>::type& t) noexcept
  {
    return static_cast<T&&>(t);
  }

  template <typename T>
  constexpr T&& forward(typename etl::remove_reference<T>::type&& t) noexcept
  {
    return static_cast<T&&>(t);
  }
#endif
}

#endif
