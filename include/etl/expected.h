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
#include "exception.h"
#include "error_handler.h"
#include "utility.h"
#include "variant.h"

namespace etl
{
  //***************************************************************************
  /// Base exception for et::expected
  //***************************************************************************
  class expected_exception : public etl::exception
  {
  public:

    expected_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// expected_invalid
  //***************************************************************************
  template <typename TError>
  class expected_invalid;

  //*******************************************
  template<>
  class expected_invalid<void> : public etl::expected_exception
  {
  public:

    expected_invalid(string_type file_name_, numeric_type line_number_)
      : expected_exception(ETL_ERROR_TEXT("expected:invalid", ETL_EXPECTED_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //*******************************************
  template <typename TError>
  class expected_invalid : etl::expected_invalid<void>
  {
  public:

    expected_invalid(string_type file_name_, numeric_type line_number_)
      : expected_invalid<void>(file_name_, line_number_)
    {
    }
  };

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

#if ETL_USING_CPP11
    //*******************************************
    /// Move constructor.
    //*******************************************
    ETL_CONSTEXPR unexpected(unexpected&& other)
      : error_value(etl::move(other.error_value))
    {
    }
#endif

#if ETL_CPP11_SUPPORTED
    //*******************************************
    /// Construct from argument.
    //*******************************************
    template <typename TErr = TError, typename = typename etl::enable_if<!etl::is_same<typename etl::remove_cvref<TErr>::type, etl::unexpected>::value &&
                                                                         !etl::is_same<typename etl::remove_cvref<TErr>::type, etl::in_place_t>::value, int>::type>
    constexpr explicit unexpected(TErr&& e)
      : error_value(etl::forward<TErr>(e))
    {
    }
#else
    //*******************************************
    /// Construct from argument.
    //*******************************************
    template <typename TErr>
    explicit unexpected(const TErr& e, typename etl::enable_if<!etl::is_same<typename etl::remove_cvref<TErr>::type, etl::unexpected>::value &&
                                                               !etl::is_same<typename etl::remove_cvref<TErr>::type, etl::in_place_t>::value, int>::type = 0)
      : error_value(e)
    {
    }
#endif

#if ETL_USING_CPP11
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
    ETL_CONSTEXPR14
    etl::unexpected<TError>& operator =(const etl::unexpected<TError>& rhs)
    {
      error_value = rhs.error_value;

      return *this;
    }

#if ETL_USING_CPP11
    //*******************************************
    /// Move assign from etl::unexpected.
    //*******************************************
    ETL_CONSTEXPR14
    etl::unexpected<TError>& operator =(etl::unexpected<TError>&& rhs)
    {
      error_value = etl::move(rhs.error_value);

      return *this;
    }
#endif

#if ETL_USING_CPP11
    //*******************************************
    /// Get the error.
    //*******************************************
    constexpr const TError& error() const& noexcept
    {
      return error_value;
    }

    //*******************************************
    /// Get the error.
    //*******************************************
    constexpr const TError&& error() const&& noexcept
    {
      return etl::move(error_value);
    }

#if ETL_USING_CPP14
    //*******************************************
    /// Get the error.
    //*******************************************
    constexpr TError& error() & noexcept
    {
      return error_value;
    }

    //*******************************************
    /// Get the error.
    //*******************************************    
    constexpr TError&& error() && noexcept
    {
      return etl::move(error_value);
    }
#else
    //*******************************************
    /// Get the error.
    //*******************************************
    TError& error() & noexcept
    {
      return error_value;
    }

    //*******************************************
    /// Get the error.
    //*******************************************    
    TError&& error() && noexcept
    {
      return etl::move(error_value);
    }
#endif

#else
    //*******************************************
    /// Get the error.
    //*******************************************
    const TError& error() const
    {
      return error_value;
    }
#endif

    //*******************************************
    /// Swap with another etl::unexpected.
    //*******************************************
    void swap(etl::unexpected<TError>& other)
    {
      using ETL_OR_STD::swap;

      swap(error_value, other.error_value);
    }

  private:

    TError error_value;
  };

#if ETL_USING_CPP17
  template <typename TError>
  unexpected(TError) -> unexpected<TError>;
#endif

#if ETL_USING_CPP17
  template <typename TError>
  unexpected(TError) -> unexpected<TError>;
#endif
}

//*******************************************
/// Equivalence operator.
//*******************************************
template <typename TError>
ETL_CONSTEXPR14
bool operator ==(const etl::unexpected<TError>& lhs, const etl::unexpected<TError>& rhs)
{
  return lhs.error_value == rhs.error_value;
}

//*******************************************
/// Swap etl::unexpected.
//*******************************************
template <typename TError>
ETL_CONSTEXPR14
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

#if ETL_CPP17_SUPPORTED
inline constexpr unexpect_t unexpect{};
#else
static const unexpect_t unexpect;
#endif

#endif
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
    using rebind = etl::expected<U, TError>;
#endif

    //*******************************************
    /// Default constructor
    //*******************************************
    ETL_CONSTEXPR14 expected() ETL_NOEXCEPT
      : data(TValue())
    {
    }

    //*******************************************
    /// Copy constructor
    //*******************************************
    ETL_CONSTEXPR14 expected(const expected& other) ETL_NOEXCEPT
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

    template <typename F>
    ETL_CONSTEXPR14 explicit expected(const etl::unexpected<F>& ue)
      : data(ue)
    {
    }

#if ETL_CPP11_SUPPORTED
    template <typename F>
    ETL_CONSTEXPR14 explicit expected(etl::unexpected<F>&& ue)
      : data(etl::move(ue))
    {
    }
#endif

    ETL_CONSTEXPR14 explicit expected(etl::in_place_t) ETL_NOEXCEPT
      : data(TValue())
    {
    }

    template <typename... Args>
    ETL_CONSTEXPR14 explicit expected(etl::in_place_t, Args&&... args)
      : data(etl::forward<Args>(args)...)
    {
    }

    template <typename U, typename... Args>
    ETL_CONSTEXPR14 explicit expected(etl::in_place_t, std::initializer_list<U> il, Args&&... args)
      : data(il, etl::forward<Args>(args)...)
    {
    }

    template <typename... Args>
    ETL_CONSTEXPR14 explicit expected(etl::unexpect_t, Args&&... args)
      : data(etl::unexpected<TError>(args...))
    {
    }

    template <typename U, typename... Args>
    ETL_CONSTEXPR14 explicit expected(etl::unexpect_t, std::initializer_list<U> il, Args&&... args)
      : data(etl::unexpected<TError>(il, args...))
    {
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

#if ETL_CPP11_SUPPORTED
    //*******************************************
    /// 
    //*******************************************
    ETL_CONSTEXPR14 T& value() &
    {
      return etl::get<TValue>(data);
    }

    //*******************************************
    /// 
    //*******************************************
    ETL_CONSTEXPR14 const T& value() const&
    {
      return etl::get<TValue>(data);
    }

    //*******************************************
    /// 
    //*******************************************
    ETL_CONSTEXPR14 T&& value() &&
    {
      return etl::move(etl::get<TValue>(data));
    }

    //*******************************************
    /// 
    //*******************************************
    ETL_CONSTEXPR14 const T&& value() const&&
    {
      return etl::move(etl::get<TValue>(data));
    }
#else
    //*******************************************
    /// Returns a const reference to the value.
    /// Undefined if the expected does not contain an value.
    //*******************************************
    ETL_CONSTEXPR14 const TValue& value() const
    {
      return etl::get<TValue>(data);
    }
#endif

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

#if ETL_CPP11_SUPPORTED
    //*******************************************
    /// 
    //*******************************************
    ETL_CONSTEXPR14 const TError& error() const& ETL_NOEXCEPT
    {
      return etl::get<TError>(data);
    }

    //*******************************************
    /// 
    //*******************************************
    ETL_CONSTEXPR14 TError& error() & ETL_NOEXCEPT
    {
      return etl::get<TError>(data);
    }
    
    //*******************************************
    /// 
    //*******************************************
    ETL_CONSTEXPR14 const TError&& error() const&& ETL_NOEXCEPT
    {
      return etl::move(etl::get<TError>(data));
    }
     
    //*******************************************
    /// 
    //*******************************************
    ETL_CONSTEXPR14 TError&& error() && ETL_NOEXCEPT
    {
      return etl::move(etl::get<TError>(data));
    }
#else
    //*******************************************
    /// Returns a const reference to the error.
    /// Undefined if the expected does not contain an error.
    //*******************************************
    ETL_CONSTEXPR14 const TError& error() const
    {
      return etl::get<TError>(data);
    }
#endif

#if ETL_CPP11_SUPPORTED
    //*******************************************
    /// 
    //*******************************************
    template <typename... Args>
    ETL_CONSTEXPR14 T& emplace(Args&&... args) ETL_NOEXCEPT
    {
      data.emplace(args...);
    }

    //*******************************************
    /// 
    //*******************************************
    template <typename U, typename... Args>
    ETL_CONSTEXPR14 T& emplace(std::initializer_list<U>& il, Args&&... args) ETL_NOEXCEPT
    {
      data.emplace(il, args...);
    }
#endif

    //*******************************************
    /// 
    //*******************************************
    TValue* operator ->()
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(valid, ETL_ERROR(expected_invalid));
#endif

      return etl::addressof(data.get<TValue>());
    }

    //*******************************************
    /// 
    //*******************************************
    const TValue* operator ->() const
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(valid, ETL_ERROR(expected_invalid));
#endif

      return etl::addressof(data.get<TValue>());
    }

    //*******************************************
    /// 
    //*******************************************
    TValue& operator *()
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(valid, ETL_ERROR(expected_invalid));
#endif

      return data.get<TValue>();
    }

    //*******************************************
    /// 
    //*******************************************
    const TValue& operator *() const
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(valid, ETL_ERROR(expected_invalid));
#endif

      return data.get<TValue>();
    }

  private:

    etl::variant<TValue, TError> data;
  };

  //*****************************************************************************
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

