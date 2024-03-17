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

namespace etl
{
  #define ETL_TYPEDEF(T, name) class name##_tag; typedef etl::type_def<name##_tag, T> name

  //*************************************************************************
  /// A template type to define strong typedefs.
  /// Usage:
  ///\code
  /// // Short form.
  /// ETL_TYPEDEF(int, mytype);
  ///
  /// // Long form.
  /// class mytype_t_tag;
  /// typedef etl::type_def<mytype_t_tag, int> mytype_t_tag;
  ///\endcode
  //*************************************************************************
  template <typename TIdType, typename TValue>
  class type_def
  {
  public:

    typedef TValue  value_type;
    typedef TIdType id_type;

    //*********************************************************************
    ETL_CONSTEXPR type_def()
      : value(TValue())
    {
    }

    //*********************************************************************
    ETL_CONSTEXPR type_def(TValue value_)
      : value(value_)
    {
    }

    //*********************************************************************
    ETL_CONSTEXPR type_def(const type_def& other)
      : value(other.value)
    {
    }

    //*********************************************************************
    ETL_CONSTEXPR operator TValue() const
    {
      return value;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator ++()
    {
      ++value;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def operator ++(int)
    {
      type_def temp(*this);
      type_def::operator ++();
      return temp;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator --()
    {
      --value;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def operator --(int)
    {
      type_def temp(*this);
      type_def::operator --();
      return temp;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator +=(TValue rhs)
    {
      value += rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator +=(const type_def& rhs)
    {
      value += rhs.value;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator -=(TValue rhs)
    {
      value -= rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator -=(const type_def& rhs)
    {
      value -= rhs.value;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator *=(TValue rhs)
    {
      value *= rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator *=(const type_def& rhs)
    {
      value *= rhs.value;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator /=(TValue rhs)
    {
      value /= rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator /=(const type_def& rhs)
    {
      value /= rhs.value;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator %=(TValue rhs)
    {
      value %= rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator %=(const type_def& rhs)
    {
      value %= rhs.value;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator &=(TValue rhs)
    {
      value &= rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator &=(const type_def& rhs)
    {
      value &= rhs.value;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator |=(TValue rhs)
    {
      value |= rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator |=(const type_def& rhs)
    {
      value |= rhs.value;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator ^=(TValue rhs)
    {
      value ^= rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator ^=(const type_def& rhs)
    {
      value ^= rhs.value;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator <<=(TValue rhs)
    {
      value <<= rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator >>=(TValue rhs)
    {
      value >>= rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator =(TValue rhs)
    {
      value = rhs;
      return *this;
    }

    //*********************************************************************
    ETL_CONSTEXPR14 type_def& operator =(const type_def& rhs)
    {
      value = rhs.value;
      return *this;
    }

    //*********************************************************************
    TValue& get()
    {
      return value;
    }

    //*********************************************************************
    ETL_CONSTEXPR const TValue& get() const
    {
      return value;
    }

    //*********************************************************************
    friend ETL_CONSTEXPR bool operator >(const type_def& lhs, const type_def& rhs)
    {
      return lhs.value > rhs.value;
    }

    //*********************************************************************
    friend ETL_CONSTEXPR bool operator >=(const type_def& lhs, const type_def& rhs)
    {
      return lhs.value >= rhs.value;
    }

    //*********************************************************************
    friend ETL_CONSTEXPR bool operator ==(const type_def& lhs, const type_def& rhs)
    {
      return lhs.value == rhs.value;
    }

    //*********************************************************************
    friend ETL_CONSTEXPR bool operator !=(const type_def& lhs, const type_def& rhs)
    {
      return lhs.value != rhs.value;
    }

  private:

      TValue value;
  };
}

#endif
