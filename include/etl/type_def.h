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

#ifndef ETL_TYPE_DEF_INCLUDED
#define ETL_TYPE_DEF_INCLUDED

#include "platform.h"
#include "type_traits.h"

namespace etl
{
  #define ETL_TYPEDEF(T, name) class name##_tag; typedef etl::type_def<name##_tag, T> name
  #define ETL_USING(name, T)   class name##_tag; typedef etl::type_def<name##_tag, T> name

  //*************************************************************************
  /// A template type to define strong typedefs.
  /// Usage:
  ///\code
  /// // Short form.
  /// ETL_TYPEDEF(int, mytype);
  /// or
  /// ETL_USING(mytype, int);
  ///
  /// // Long form.
  /// class mytype_t_tag;
  /// typedef etl::type_def<mytype_t_tag, int> mytype;
  ///\endcode
  //*************************************************************************
  template <typename TIdType, typename TValue>
  class type_def
  {
  public:

    typedef TValue  type;
    typedef TValue  value_type;
    typedef TIdType id_type;

    //*********************************************************************
    ETL_CONSTEXPR type_def() ETL_NOEXCEPT
      : value(TValue())
    {
    }

    //*********************************************************************
#if ETL_USING_CPP11
    template <typename T, typename = typename etl::enable_if<etl::is_convertible<T, TValue>::value, void>::type>
#else
    template <typename T>
#endif
    ETL_CONSTEXPR type_def(T value_) ETL_NOEXCEPT
      : value(value_)
    {
    }

    //*********************************************************************
#if ETL_USING_CPP11
    ETL_CONSTEXPR type_def(const type_def& other) = default;
#endif

    //*********************************************************************
    ETL_CONSTEXPR operator TValue() const ETL_NOEXCEPT
    {
      return value;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator ++() ETL_NOEXCEPT
    {
      ++value;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def operator ++(int) ETL_NOEXCEPT
    {
      type_def temp(*this);
      type_def::operator ++();
      return temp;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator --() ETL_NOEXCEPT
    {
      --value;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def operator --(int) ETL_NOEXCEPT
    {
      type_def temp(*this);
      type_def::operator --();
      return temp;
    }

    //*********************************************************************
    template <typename T>
    ETL_CONSTEXPR14 
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def&>::type
#else
      type_def&
#endif
      operator +=(T rhs) ETL_NOEXCEPT
    {
      value += rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 
    type_def& operator +=(const type_def& rhs) ETL_NOEXCEPT
    {
      value += rhs.value;
      return *this;
    }

    //*********************************************************************
    template <typename T>
    ETL_CONSTEXPR14 
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def&>::type
#else
      type_def&
#endif
      operator -=(T rhs) ETL_NOEXCEPT
    {
      value -= rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator -=(const type_def& rhs) ETL_NOEXCEPT
    {
      value -= rhs.value;
      return *this;
    }

    //*********************************************************************
    template <typename T>
    ETL_CONSTEXPR14 
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def&>::type
#else
      type_def&
#endif
      operator *=(T rhs) ETL_NOEXCEPT
    {
      value *= rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator *=(const type_def& rhs) ETL_NOEXCEPT
    {
      value *= rhs.value;
      return *this;
    }

    //*********************************************************************
    template <typename T>
    ETL_CONSTEXPR14 
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def&>::type
#else
      type_def&
#endif
      operator /=(T rhs) ETL_NOEXCEPT
    {
      value /= rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator /=(const type_def& rhs) ETL_NOEXCEPT
    {
      value /= rhs.value;
      return *this;
    }

    //*********************************************************************
    template <typename T>
    ETL_CONSTEXPR14 
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def&>::type
#else
      type_def&
#endif
      operator %=(T rhs) ETL_NOEXCEPT
    {
      value %= rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator %=(const type_def& rhs) ETL_NOEXCEPT
    {
      value %= rhs.value;
      return *this;
    }

    //*********************************************************************
    template <typename T>
    ETL_CONSTEXPR14 
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def&>::type
#else
      type_def&
#endif
      operator &=(T rhs) ETL_NOEXCEPT
    {
      value &= rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator &=(const type_def& rhs) ETL_NOEXCEPT
    {
      value &= rhs.value;
      return *this;
    }

    //*********************************************************************
    template <typename T>
    ETL_CONSTEXPR14 
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def&>::type
#else
      type_def&
#endif
      operator |=(T rhs) ETL_NOEXCEPT
    {
      value |= rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator |=(const type_def& rhs) ETL_NOEXCEPT
    {
      value |= rhs.value;
      return *this;
    }

    //*********************************************************************
    template <typename T>
    ETL_CONSTEXPR14 
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def&>::type
#else
      type_def&
#endif
      operator ^=(T rhs) ETL_NOEXCEPT
    {
      value ^= rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator ^=(const type_def& rhs) ETL_NOEXCEPT
    {
      value ^= rhs.value;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator <<=(int rhs) ETL_NOEXCEPT
    {
      value <<= rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator >>=(int rhs) ETL_NOEXCEPT
    {
      value >>= rhs;
      return *this;
    }

    //*********************************************************************
#if ETL_USING_CPP11
    ETL_CONSTEXPR14 type_def& operator =(const type_def& rhs) = default;
#endif

    //*********************************************************************
    TValue& get() ETL_NOEXCEPT
    {
      return value;
    }

    //*********************************************************************
    ETL_CONSTEXPR const TValue& get() const ETL_NOEXCEPT
    {
      return value;
    }

    //*********************************************************************
    // + operator
    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def>::type
#else
      type_def
#endif
      operator +(const type_def& lhs, T rhs) ETL_NOEXCEPT
    {
      return type_def(lhs.value + rhs);
    }

    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR type_def operator +(T lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return type_def(lhs + rhs.value);
    }

    //*********************************************************************
    friend ETL_CONSTEXPR type_def operator +(const type_def& lhs, const type_def& rhs)
    {
      return type_def(lhs.value + rhs.value);
    }

    //*********************************************************************
    // - operator
    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR 
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def>::type
#else
      type_def
#endif
      operator -(const type_def& lhs, T rhs) ETL_NOEXCEPT
    {
      return type_def(lhs.value - rhs);
    }

    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def>::type
#else
      type_def
#endif
      operator -(T lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return type_def(lhs - rhs.value);
    }

    //*********************************************************************
    friend ETL_CONSTEXPR type_def operator -(const type_def& lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return type_def(lhs.value - rhs.value);
    }

    //*********************************************************************
    // * operator
    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def>::type
#else
      type_def
#endif
      operator *(const type_def& lhs, T rhs) ETL_NOEXCEPT
    {
      return type_def(lhs.value * rhs);
    }

    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def>::type
#else
      type_def
#endif
      operator *(T lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return type_def(lhs * rhs.value);
    }

    //*********************************************************************
    friend ETL_CONSTEXPR type_def operator *(const type_def& lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return type_def(lhs.value * rhs.value);
    }

    //*********************************************************************
    // / operator
    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def>::type
#else
      type_def
#endif
      operator /(const type_def& lhs, T rhs) ETL_NOEXCEPT
    {
      return type_def(lhs.value / rhs);
    }

    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def>::type
#else
      type_def
#endif
      operator /(T lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return type_def(lhs / rhs.value);
    }

    //*********************************************************************
    friend ETL_CONSTEXPR type_def operator /(const type_def& lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return type_def(lhs.value / rhs.value);
    }

    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def>::type
#else
      type_def
#endif
      operator %(const type_def& lhs, T rhs) ETL_NOEXCEPT
    {
      return type_def(lhs.value % rhs);
    }

    //*********************************************************************
    // % operator
    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def>::type
#else
      type_def
#endif
      operator %(T lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return type_def(lhs % rhs.value);
    }

    //*********************************************************************
    friend ETL_CONSTEXPR type_def operator %(const type_def& lhs, const type_def& rhs)
    {
      return type_def(lhs.value % rhs.value);
    }

    //*********************************************************************
    // & operator
    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR 
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def>::type
#else
      type_def
#endif
      operator &(const type_def& lhs, T rhs) ETL_NOEXCEPT
    {
      return type_def(lhs.value & rhs);
    }

    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def>::type
#else
      type_def
#endif
      operator &(T lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return type_def(lhs & rhs.value);
    }

    //*********************************************************************
    friend ETL_CONSTEXPR type_def operator &(const type_def& lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return type_def(lhs.value & rhs.value);
    }

    //*********************************************************************
    // | operator
    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def>::type
#else
      type_def
#endif
      operator |(const type_def& lhs, T rhs) ETL_NOEXCEPT
    {
      return type_def(lhs.value | rhs);
    }

    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def>::type
#else
      type_def
#endif
      operator |(T lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return type_def(lhs | rhs.value);
    }

    //*********************************************************************
    friend ETL_CONSTEXPR type_def operator |(const type_def& lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return type_def(lhs.value | rhs.value);
    }

    //*********************************************************************
    // ^ operator
    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def>::type
#else
      type_def
#endif
      operator ^(const type_def& lhs, T rhs) ETL_NOEXCEPT
    {
      return type_def(lhs.value ^ rhs);
    }

    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, type_def>::type
#else
      type_def
#endif
      operator ^(T lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return type_def(lhs ^ rhs.value);
    }

    //*********************************************************************
    friend ETL_CONSTEXPR type_def operator ^(const type_def& lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return type_def(lhs.value ^ rhs.value);
    }

    //*********************************************************************
    // << operator
    //*********************************************************************
    friend ETL_CONSTEXPR type_def operator <<(const type_def& lhs, int rhs) ETL_NOEXCEPT
    {
      return type_def(lhs.value << rhs);
    }

    //*********************************************************************
    // >> operator
    //*********************************************************************
    friend ETL_CONSTEXPR type_def operator >>(const type_def& lhs, int rhs) ETL_NOEXCEPT
    {
      return type_def(lhs.value >> rhs);
    }

    //*********************************************************************
    // < operator
    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, bool>::type
#else
      bool
#endif
      operator <(const type_def& lhs, T rhs) ETL_NOEXCEPT
    {
      return lhs.value < rhs;
    }

    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, bool>::type
#else
      bool
#endif
      operator <(T lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return lhs < rhs.value;
    }

    //*********************************************************************
    friend ETL_CONSTEXPR bool operator <(const type_def& lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return lhs.value < rhs.value;
    }

    //*********************************************************************
    // <= operator
    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, bool>::type
#else
      bool
#endif
      operator <=(const type_def& lhs, T rhs) ETL_NOEXCEPT
    {
      return lhs.value <= rhs;
    }

    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, bool>::type
#else
      bool
#endif
      operator <=(T lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return lhs <= rhs.value;
    }

    //*********************************************************************
    friend ETL_CONSTEXPR bool operator <=(const type_def& lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return lhs.value <= rhs.value;
    }

    //*********************************************************************
    // > operator
    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, bool>::type
#else
      bool
#endif
      operator >(const type_def& lhs, T rhs) ETL_NOEXCEPT
    {
      return lhs.value > rhs;
    }

    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, bool>::type
#else
      bool
#endif
      operator >(T lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return lhs > rhs.value;
    }

    //*********************************************************************
    friend ETL_CONSTEXPR bool operator >(const type_def& lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return lhs.value > rhs.value;
    }

    //*********************************************************************
    // >= operator
    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, bool>::type
#else
      bool
#endif
      operator >=(const type_def& lhs, T rhs) ETL_NOEXCEPT
    {
      return lhs.value >= rhs;
    }

    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, bool>::type
#else
      bool
#endif
      operator >=(T lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return lhs >= rhs.value;
    }

    //*********************************************************************
    friend ETL_CONSTEXPR bool operator >=(const type_def& lhs, const type_def& rhs)
    {
      return lhs.value >= rhs.value;
    }

    //*********************************************************************
    // == operator
    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, bool>::type
#else
      bool
#endif
      operator ==(const type_def& lhs, T rhs) ETL_NOEXCEPT
    {
      return lhs.value == rhs;
    }

    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, bool>::type
#else
      bool
#endif
      operator ==(T lhs, const type_def& rhs)
    {
      return lhs == rhs.value;
    }

    //*********************************************************************
    friend ETL_CONSTEXPR bool operator ==(const type_def& lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return lhs.value == rhs.value;
    }

    //*********************************************************************
    // != operator
    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, bool>::type
#else
      bool
#endif
      operator !=(const type_def& lhs, T rhs) ETL_NOEXCEPT
    {
      return lhs.value != rhs;
    }

    //*********************************************************************
    template <typename T>
    friend ETL_CONSTEXPR
#if ETL_USING_CPP11
      typename etl::enable_if<etl::is_convertible<T, TValue>::value, bool>::type
#else
      bool
#endif
      operator !=(T lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return lhs != rhs.value;
    }

    //*********************************************************************
    friend ETL_CONSTEXPR bool operator !=(const type_def& lhs, const type_def& rhs) ETL_NOEXCEPT
    {
      return lhs.value != rhs.value;
    }

  private:

      TValue value;
  };
}

#endif
