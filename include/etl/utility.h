///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 jwellbelove

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

#ifndef ETL_UTILITY_INCLUDED
#define ETL_UTILITY_INCLUDED

#include "platform.h"
#include "type_traits.h"

#if !defined(ETL_NO_STL)
  #include <utility>
#endif

///\defgroup utility utility
///\ingroup utilities

namespace etl
{
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

  // We can't have std::swap and etl::swap templates coexisting in the unit tests
  // as the compiler will be unable to decide of which one to use, due to ADL.
#if defined(ETL_NO_STL) && !defined(ETL_IN_UNIT_TEST)
  //***************************************************************************
  // swap
#if ETL_CPP11_SUPPORTED
  template <typename T>
  void swap(T& a, T& b) ETL_NOEXCEPT
  {
    T temp(etl::move(a));
    a = etl::move(b);
    b = etl::move(temp);
  }
#else
  template <typename T>
  void swap(T& a, T& b) ETL_NOEXCEPT
  {
    T temp(a);
    a = b;
    b = temp;
  }
#endif

  template< class T, size_t N >
  void swap(T(&a)[N], T(&b)[N]) ETL_NOEXCEPT
  {
    for (size_t i = 0; i < N; ++i)
    {
      swap(a[i], b[i]);
    }
  }
#endif

  //******************************************************************************
  template <typename T1, typename T2>
  struct pair
  {
    typedef T1 first_type;
    typedef T2 second_type;

    T1 first;
    T2 second;

    /// Default constructor
    ETL_CONSTEXPR pair()
      : first(T1())
      , second(T2())
    {
    }

    /// Constructor from parameters
    ETL_CONSTEXPR14 pair(const T1& a, const T2& b)
      : first(a)
      , second(b)
    {
    }

#if ETL_CPP11_SUPPORTED
    /// Move constructor from parameters
    template <typename U1, typename U2>
    ETL_CONSTEXPR14 pair(U1&& a, U2&& b)
      : first(etl::forward<U1>(a))
      , second(etl::forward<U2>(b))
    {
    }
#endif

    /// Copy constructor
    template <typename U1, typename U2>
    ETL_CONSTEXPR14 pair(const pair<U1, U2>& other)
      : first(other.first)
      , second(other.second)
    {
    }

    /// Copy constructor
    pair(const pair<T1, T2>& other)
      : first(other.first)
      , second(other.second)
    {
    }

#if ETL_CPP11_SUPPORTED
    /// Move constructor
    template <typename U1, typename U2>
    ETL_CONSTEXPR14 pair(pair<U1, U2>&& other)
      : first(etl::move(other.first))
      , second(etl::move(other.second))
    {
    }
#endif

    void swap(pair<T1, T2>& other)
    {
      using ETL_OR_STD::swap;

      swap(first,  other.first);
      swap(second, other.second);
    }

    pair<T1, T2>& operator =(const pair<T1, T2>& other)
    {
      first  = other.first;
      second = other.second;

      return *this;
    }

    template <typename U1, typename U2>
    pair<U1, U2>& operator =(const pair<U1, U2>& other)
    {
      first  = other.first;
      second = other.second;

      return *this;
    }

#if ETL_CPP11_SUPPORTED
    pair<T1, T2>& operator =(pair<T1, T2>&& other)
    {
      first  = etl::move(other.first);
      second = etl::move(other.second);

      return *this;
    }

    template <typename U1, typename U2>
    pair<U1, U2>& operator =(pair<U1, U2>&& other)
    {
      first  = etl::move(other.first);
      second = etl::move(other.second);

      return *this;
    }
#endif
  };

  //******************************************************************************
#if ETL_CPP11_SUPPORTED
  template <typename T1, typename T2>
  inline pair<T1, T2> make_pair(T1&& a, T2&& b)
  {
    return pair<T1, T2>(etl::move(a), etl::move(b));
  }
#else
  template <typename T1, typename T2>
  inline pair<T1, T2> make_pair(T1 a, T2 b)
  {
    return pair<T1, T2>(a, b);
  }
#endif

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

#if defined(ETL_NO_STL) || !ETL_CPP14_SUPPORTED
  //***************************************************************************
  /// exchange (const)
  //***************************************************************************
  template <typename T, typename U = T>
  T exchange(T& object, const U& new_value)
  {
    T old_value = object;
    object = new_value;
    return old_value;
  }
#else
  //***************************************************************************
  /// exchange (const)
  //***************************************************************************
  template <typename T, typename U = T>
  T exchange(T& object, const U& new_value)
  {
    return std::exchange(object, new_value);
  }
#endif

  //***************************************************************************
  /// as_const
  //***************************************************************************
  template <typename T>
  typename etl::add_const<T>::type& as_const(T& t)
  {
    return t;
  }
}

#endif

