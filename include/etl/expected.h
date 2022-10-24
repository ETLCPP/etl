///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

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

#ifndef ETL_EXPECTED_INCLUDED
#define ETL_EXPECTED_INCLUDED

///\defgroup expected expected
///\ingroup utilities

#include "platform.h"
#include "utility.h"

namespace etl
{
  //***************************************************************************
  /// Unexpected type.
  /// etl::unexpected represents an unexpected value stored in etl::expected.
  //***************************************************************************
  template <typename TError>
  class unexpected
  {
  public:

    //*******************************************
    /// Copy constructor.
    //*******************************************
    ETL_CONSTEXPR unexpected(const unexpected& other)
      : error_value(other.error_value)
    {
    }

    //*******************************************
    /// Move constructor.
    //*******************************************
    ETL_CONSTEXPR unexpected(unexpected&& other)
      : error_value(etl::move(other.error_value))
    {
    }

#if ETL_CPP11_SUPPORTED
    //*******************************************
    /// Construct from argument.
    //*******************************************
    template <typename E = TError, typename = typename etl::enable_if<!etl::is_same<typename etl::remove_cvref<Err>::type, etl::unexpected>::value &&
                                                                      !etl::is_same<typename etl::remove_cvref<Err>::type, etl::in_place_t>::value, int>::type>
    constexpr explicit unexpected(E&& e)
      : error_value(etl::forward<TError>(e))
    {
    }
#else
    //*******************************************
    /// Construct from argument.
    //*******************************************
    template <typename E>
    explicit unexpected(const E& e, typename etl::enable_if<!etl::is_same<typename etl::remove_cvref<Err>::type, etl::unexpected>::value &&
                                                            !etl::is_same<typename etl::remove_cvref<Err>::type, etl::in_place_t>::value, int>::type = 0)
      : error_value(e)
    {
    }
#endif

#if ETL_CPP11_SUPPORTED
    //*******************************************
    /// Construct from arguments.
    //*******************************************
    template <typename... Args >
    constexpr explicit unexpected(etl::in_place_t, Args&&... args)
      : error_value(etl::forward<Args>(args)...)
    {
    }
#endif

#if ETL_HAS_INITIALIZER_LIST
    //*******************************************
    /// Construct from initializer_list and arguments.
    //*******************************************
    template <typename U, typename... Args>
    constexpr explicit unexpected(etl::in_place_t, std::initializer_list<U> init, Args&&... args)
      : error_value(init, etl::forward<Args>(args)...)
    {
    }
#endif

    //*******************************************
    /// Assign from etl::unexpected.
    //*******************************************
    unexpected& operator =(const etl::unexpected& rhs)
    {
      error_value = rhs.error_value;
    }

#if ETL_CPP11_SUPPORTED
    //*******************************************
    /// Move assign from etl::unexpected.
    //*******************************************
    unexpected& operator =(etl::unexpected&& rhs)
    {
      error_value = etl::move(rhs.error_value);
    }
#endif

    //*******************************************
    /// Get the error.
    //*******************************************
    const TError& error() const
    {
      return error_type;
    }

    //*******************************************
    /// Swap with another etl::unexpected.
    //*******************************************
    swap(etl::unexpected& other)
    {
      using ETL_OR_STD::swap;

      swap(error_value, other.error_value);
    }

  private:

    TError error_value;
  };

  //*******************************************
  /// Equivalence operator.
  //*******************************************
  template <typename TError>
  bool operator ==(const etl::unexpected<TError>& lhs, const etl::unexpected<TError>& rhs)
  {
    return lhs.error_value == rhs.error_value;
  }

  //*******************************************
  /// Swap etl::unexpected.
  //*******************************************
  template <typename TError>
  void swap(etl::unexpected<TError>& lhs, etl::unexpected<TError>& rhs)
  {
    lhs.swap(rhs);
  }

  //*****************************************************************************
  /// unexpect_t
  //*****************************************************************************
  struct unexpect_t 
  {
    ETL_CONSTEXPR14 explicit unexpect_t()
    {
    }
  };

#if ETL_CPP14_SUPPORTED
  inline ETL_CONSTEXPR14 unexpect_t unexpect{};
#endif

  //*****************************************************************************
  /// Expected type.
  //*****************************************************************************
  template <typename TValue, typename TError>
  class expected
  {
  public:

    typename TValue                  value_type;
    typename TError                  error_type;
    typename etl::unexpected<TError> unexpected_type;

#if ETL_CPP11_SUPPORTED
    template <typename U>
    using rebind = etl::expected<U, error_type>;
#endif

    //*******************************************
    /// Default constructor
    //*******************************************
    ETL_CONSTEXPR14 expected(const expected& other) ETL_NOEXCEPT
      : data(TValue())
    {
    }

    //*******************************************
    /// Copy constructor
    //*******************************************
    ETL_CONSTEXPR14 expected(const expected& other)
      : data(other.data)
    {
    }

#if ETL_CPP11_SUPPORTED
    //*******************************************
    /// Move constructor
    //*******************************************
    ETL_CONSTEXPR14 expected(expected&& other) ETL_NOEXCEPT
      : data(etl::move(other.data))
    {
    }
#endif

    template <typename U, typename F>
    constexpr explicit() expected(const expected<U, F>& other)
    {
    }

    template <typename U, typename F>
    constexpr explicit(expected(expected<U, F>&& other)
    {
    }

#if ETL_CPP11_SUPPORTED
    template <typename U = T>
    constexpr explicit expected(U&& v)
    {
    }
#endif

    template <typename F>
    constexpr explicit expected(const etl::unexpected<F>& e)
    {
    }

#if ETL_CPP11_SUPPORTED
    template <typename F>
    constexpr explicit expected(etl::unexpected<F>&& e)
    {
    }
#endif

    constexpr explicit expected(etl::in_place_t) noexcept
    {
    }

    template <typename... Args>
    constexpr explicit expected(etl::in_place_t, Args&&... args)
    {
    }

    template <typename U, typename... Args>
    constexpr explicit expected(etl::in_place_t, std::initializer_list<U> il, Args&&... args)
    {
    }

    template <typename... Args>
    constexpr explicit expected(etl::unexpect_t, Args&&... args)
    {
    }

    template <typename U, typename... Args>
    constexpr explicit expected(etl::unexpect_t, std::initializer_list<U> il, Args&&... args)
    {
    }

//    //*******************************************
//    // Construct from a value
//    //*******************************************
//    expected(const TValue& value)
//      : data(value)
//    {
//    }
//
//#if ETL_CPP11_SUPPORTED
//    //*******************************************
//    // Move construct from a value
//    //*******************************************
//    expected(TValue&& value)
//      : data(etl::move(value))
//    {
//    }
//#endif

    ////*******************************************
    ///// Construct from error
    ////*******************************************
    //expected(const TError& error)
    //  : data(error)
    //{
    //}

    ////*******************************************
    ///// Move construct from error
    ////*******************************************
    //expected(TError&& error)
    //  : data(etl::move(error))
    //{
    //}

    //*******************************************
    /// Copy assign
    //*******************************************
    expected& operator =(const expected& other)
    {
      data = other.data;
      return *this;
    }

    //*******************************************
    /// Move assign 
    //*******************************************
    expected& operator =(expected&& other)
    {
      data = etl::move(other.data);
      return *this;
    }

    //*******************************************
    /// Copy assign from value
    //*******************************************
    expected& operator =(const TValue& value)
    {
      data = value;
      return *this;
    }

    //*******************************************
    /// Move assign from value
    //*******************************************
    expected& operator =(TValue&& value)
    {
      data = etl::move(value);
      return *this;
    }

    //*******************************************
    /// Copy assign from error
    //*******************************************
    expected& operator =(const TError& error)
    {
      data = error;
      return *this;
    }

    //*******************************************
    /// Move assign from error
    //*******************************************
    expected& operator =(TError&& error)
    {
      data = etl::move(error);
      return *this;
    }

    //*******************************************
    /// Returns a const reference to the value.
    /// Undefined if the expected does not contain an value.
    //*******************************************
    ETL_CONSTEXPR14 const TValue& value() const
    {
      return etl::get<TValue>(data);
    }

    //*******************************************
    /// Returns an rvalue reference to the value.
    /// Undefined if the expected does not contain an value.
    //*******************************************
    ETL_CONSTEXPR14 TValue&& value()
    {
      return etl::move(etl::get<TValue>(etl::move(data)));
    }

#if ETL_CPP11_SUPPORTED
    //*******************************************
    /// 
    //*******************************************
    template <typename U>
    ETL_CONSTEXPR14 TValue value_or(U&& default_value) const&
    {
      if (has_value())
      {
        return value();
      }
      else
      {
        return default_value;
      }
    }

    //*******************************************
    /// 
    //*******************************************
    template <typename U>
    ETL_CONSTEXPR14 TValue value_or(U&& default_value) &&
    {
      if (has_value())
      {
        return etl::move(value());
      }
      else
      {
        return etl::move(default_value);
      }
    }
#else
    //*******************************************
    /// 
    //*******************************************
    template <typename U>
    TValue value_or(const U& default_value) const
    {
      if (has_value())
      {
        return value();
      }
      else
      {
        return default_value;
      }
    }
#endif

    //*******************************************
    /// Returns a const reference to the error.
    /// Undefined if the expected does not contain an error.
    //*******************************************
    ETL_CONSTEXPR14 const TError& error() const
    {
      return etl::get<TError>(data);
    }

#if (ETL_CPP11_SUPPORTED)
    //*******************************************
    /// Returns an rvalue reference to the error.
    /// Undefined if the expected does not contain an error.
    //*******************************************
    ETL_CONSTEXPR14 TError&& error() &&
    {
      return etl::move(etl::get<TError>(data));
    }

    //*******************************************
    /// Returns an rvalue reference to the error.
    /// Undefined if the expected does not contain an error.
    //*******************************************
    ETL_CONSTEXPR14 TError&& error() const &&
    {
      return etl::move(etl::get<TError>(data));
    }
#endif

  private:

    etl::variant<TValue, TError> data;
  };

  //*****************************************************************************
  /// Result type.
  /// Specialisation for void value type.
  //*****************************************************************************
  template<typename TError>
  class expected<void, TError>
  {
  public:



  private:

    TError error;
  };
}


#endif

