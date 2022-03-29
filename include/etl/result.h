///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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

#ifndef ETL_RESULT_INCLUDED
#define ETL_RESULT_INCLUDED

///\defgroup result result
///\ingroup utilities

#include "platform.h"
#include "variant.h"

#if ETL_CPP11_NOT_SUPPORTED
  #if !defined(ETL_IN_UNIT_TEST)
  #error NOT SUPPORTED FOR C++03 OR BELOW
  #endif
#else

namespace etl
{
  //*****************************************************************************
  /// Result type.
  //*****************************************************************************
  template <typename TValue, typename TError>
  class result
  {
  public:

    //*******************************************
    /// Cannot be default constructed
    //*******************************************
    result() = delete;

    //*******************************************
    /// Copy constructor
    //*******************************************
    result(const result& other)
      : data(other.data)
    {
    }

    //*******************************************
    /// Move constructor
    //*******************************************
    result(result&& other)
      : data(etl::move(other.data))
    {
    }

    //*******************************************
    // Construct from a value
    //*******************************************
    result(const TValue& value)
      : data(value)
    {
    }

    //*******************************************
    // Move construct from a value
    //*******************************************
    result(TValue&& value)
      : data(etl::move(value))
    {
    }

    //*******************************************
    /// Construct from error
    //*******************************************
    result(const TError& err)
      : data(err)
    {
    }

    //*******************************************
    /// Move construct from error
    //*******************************************
    result(TError&& err)
      : data(etl::move(err))
    {
    }

    //*******************************************
    /// Copy assign
    //*******************************************
    result& operator =(const result& other)
    {
      data = other.data;
      return *this;
    }

    //*******************************************
    /// Move assign 
    //*******************************************
    result& operator =(result&& other)
    {
      data = etl::move(other.data);
      return *this;
    }

    //*******************************************
    /// Copy assign from value
    //*******************************************
    result& operator =(const TValue& value)
    {
      data = value;
      return *this;
    }

    //*******************************************
    /// Move assign from value
    //*******************************************
    result& operator =(TValue&& value)
    {
      data = etl::move(value);
      return *this;
    }

    //*******************************************
    /// Copy assign from error
    //*******************************************
    result& operator =(const TError& err)
    {
      data = err;
      return *this;
    }

    //*******************************************
    /// Move assign from error
    //*******************************************
    result& operator =(TError&& err)
    {
      data = etl::move(err);
      return *this;
    }

    //*******************************************
    /// <b>true</b> if result contains a value
    //*******************************************
    bool is_value() const
    {
      return (data.index() == 0U);
    }

    //*******************************************
    /// <b>true</b> if result contains an error
    //*******************************************
    bool is_error() const
    {
      return (data.index() == 1U);
    }

    //*******************************************
    /// Returns a const reference to the value.
    /// Undefined if the result does not contain an value.
    //*******************************************
    const TValue& value() const
    {
      return etl::get<TValue>(data);
    }

    //*******************************************
    /// Returns an rvalue reference to the value.
    /// Undefined if the result does not contain an value.
    //*******************************************
    TValue&& value()
    {
      return etl::move(etl::get<TValue>(etl::move(data)));
    }

    //*******************************************
    /// Returns a const reference to the error.
    /// Undefined if the result does not contain an error.
    //*******************************************
    const TError& error() const
    {
      return etl::get<TError>(data);
    }

    //*******************************************
    /// Returns an rvalue reference to the error.
    /// Undefined if the result does not contain an error.
    //*******************************************
    TError&& error()
    {
      return etl::move(etl::get<TError>(etl::move(data)));
    }

  private:

    etl::variant<TValue, TError> data;
  };

  //*****************************************************************************
  /// Result type.
  /// Specialisation for void value type.
  //*****************************************************************************
  template<typename TError>
  class result<void, TError>
  {
  public:

    //*******************************************
    /// Cannot be default constructed
    //*******************************************
    result() = delete;

    //*******************************************
    /// Copy constructor
    //*******************************************
    result(const result& other)
      : data(other.data)
    {
    }

    //*******************************************
    /// Move constructor
    //*******************************************
    result(result&& other)
      : data(etl::move(other.data))
    {
    }

    //*******************************************
    /// Construct from error
    //*******************************************
    result(const TError& err)
      : data(err)
    {
    }

    //*******************************************
    /// Move construct from error
    //*******************************************
    result(TError&& err)
      : data(etl::move(err))
    {
    }

    //*******************************************
    /// Copy assign from error
    //*******************************************
    result& operator =(const TError& err)
    {
      data = err;
      return *this;
    }

    //*******************************************
    /// Move assign from error
    //*******************************************
    result& operator =(TError&& err)
    {
      data = etl::move(err);
      return *this;
    }

    //*******************************************
    /// <b>true</b> if result contains a value
    //*******************************************
    bool is_value() const
    {
      return false;
    }

    //*******************************************
    /// <b>true</b> if result contains an error
    //*******************************************
    bool is_error() const
    {
      return true;
    }

    //*******************************************
    /// Returns a const reference to the error.
    /// Undefined if the result does not contain an error.
    //*******************************************
    const TError& error() const
    {
      return etl::get<TError>(data);
    }

    //*******************************************
    /// Returns an rvalue reference to the error.
    /// Undefined if the result does not contain an error.
    //*******************************************
    TError&& error()
    {
      return etl::get<TError>(etl::move(data));
    }

  private:

    etl::variant<etl::monostate, TError> data;
  };
}

#endif
#endif
