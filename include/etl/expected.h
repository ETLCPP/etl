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
#include "initializer_list.h"
#include "type_traits.h"
#include "invoke.h"

namespace etl
{
  // Forward declaration for is_expected
  template <typename TValue, typename TError> class expected;
  
  template <typename T>
  struct is_expected : etl::false_type {};
  
  template <typename TValue, typename TError>
  struct is_expected<expected<TValue,TError>> : etl::true_type {};

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
  class expected_invalid : public etl::expected_exception
  {
  public:

    expected_invalid(string_type file_name_, numeric_type line_number_)
      : expected_exception(ETL_ERROR_TEXT("expected:invalid", ETL_EXPECTED_FILE_ID"A"), file_name_, line_number_)
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

    typedef TError error_type;

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

    //*******************************************
    /// Construct from an lvalue.
    //*******************************************
    ETL_CONSTEXPR explicit unexpected(const TError& e)
      : error_value(e)
    {
    }

#if ETL_USING_CPP11
    //*******************************************
    /// Construct from an rvalue.
    //*******************************************
    ETL_CONSTEXPR explicit unexpected(TError&& e)
      : error_value(etl::forward<TError>(e))
    {
    }

    //*******************************************
    /// Construct from arguments.
    //*******************************************
    template <typename... Args >
    ETL_CONSTEXPR explicit unexpected(etl::in_place_t, Args&&... args)
      : error_value(etl::forward<Args>(args)...)
    {
    }
#endif

#if ETL_HAS_INITIALIZER_LIST
    //*******************************************
    /// Construct from initializer_list and arguments.
    //*******************************************
    template <typename U, typename... Args>
    ETL_CONSTEXPR explicit unexpected(etl::in_place_t, std::initializer_list<U> init, Args&&... args)
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
#if ETL_USING_CPP11
      ETL_STATIC_ASSERT(etl::is_copy_constructible<TError>::value, "Error not copy assignable");
#endif

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
      ETL_STATIC_ASSERT(etl::is_move_constructible<TError>::value, "Error not move assignable");

      error_value = etl::move(rhs.error_value);
      return *this;
    }
#endif

#if ETL_USING_CPP11
    //*******************************************
    /// Get the error.
    //*******************************************
    ETL_CONSTEXPR14 TError& error()& ETL_NOEXCEPT
    {
      return error_value;
    }

    //*******************************************
    /// Get the error.
    //*******************************************
    ETL_CONSTEXPR14 const TError& error() const& ETL_NOEXCEPT
    {
      return error_value;
    }

    //*******************************************
    /// Get the error.
    //*******************************************
    ETL_CONSTEXPR14 TError&& error()&& ETL_NOEXCEPT
    {
      return etl::move(error_value);
    }

    //*******************************************
    /// Get the error.
    //*******************************************
    ETL_CONSTEXPR14 TError&& error() const&& ETL_NOEXCEPT
    {
      return etl::move(error_value);
    }
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

  //*****************************************************************************
  /// unexpect_t
  //*****************************************************************************
  struct unexpect_t
  {
    ETL_CONSTEXPR14 explicit unexpect_t()
    {
    }
  };

#if ETL_USING_CPP17
  inline ETL_CONSTEXPR unexpect_t unexpect{};
#else
  static const unexpect_t unexpect;
#endif

  //*****************************************************************************
  /// Expected type.
  //*****************************************************************************
  template <typename TValue, typename TError>
  class expected
  {
  public:

    typedef etl::expected<TValue, TError> this_type;
    typedef TValue                        value_type;
    typedef TError                        error_type;
    typedef etl::unexpected<TError>       unexpected_type;

#if ETL_USING_CPP11
    template <typename U>
    using rebind = expected<U, TError>;
#endif

    //*******************************************
    /// Default constructor
    //*******************************************
    ETL_CONSTEXPR14 expected() ETL_NOEXCEPT
      : storage(etl::in_place_index_t<Value_Type>(), value_type())
    {
    }

    //*******************************************
    /// Constructor
    //*******************************************
    ETL_CONSTEXPR14 expected(const value_type& value_) ETL_NOEXCEPT
      : storage(etl::in_place_index_t<Value_Type>(), value_)
    {
    }

#if ETL_USING_CPP11
    //*******************************************
    /// Constructor
    //*******************************************
    ETL_CONSTEXPR14 expected(value_type&& value_) ETL_NOEXCEPT
      : storage(etl::in_place_index_t<Value_Type>(), etl::move(value_))
    {
    }
#endif

    //*******************************************
    /// Copy constructor
    //*******************************************
    ETL_CONSTEXPR14 expected(const expected& other) ETL_NOEXCEPT
      : storage(other.storage)
    {
    }

#if ETL_USING_CPP11
    //*******************************************
    /// Move constructor
    //*******************************************
    ETL_CONSTEXPR14 expected(expected&& other) ETL_NOEXCEPT
      : storage(etl::move(other.storage))
    {
    }
#endif

#if ETL_USING_CPP11
    //*******************************************
    /// Copy construct from unexpected type.
    //*******************************************
    template <typename G, typename etl::enable_if<!etl::is_convertible<const G&, TError>::value, bool>::type = false>
    ETL_CONSTEXPR14 explicit expected(const etl::unexpected<G>& ue)
      : storage(etl::in_place_index_t<Error_Type>(), ue.error())
    {
    }

    template <typename G, typename etl::enable_if<etl::is_convertible<const G&, TError>::value, bool>::type = false>
    ETL_CONSTEXPR14 expected(const etl::unexpected<G>& ue)
      : storage(etl::in_place_index_t<Error_Type>(), ue.error())
    {
    }
#else
    template <typename G>
    explicit expected(const etl::unexpected<G>& ue)
      : storage(etl::in_place_index_t<Error_Type>(), ue.error())
    {
    }
#endif

#if ETL_USING_CPP11
    //*******************************************
    /// Move construct from unexpected type.
    //*******************************************
    template <typename G, typename etl::enable_if<!etl::is_convertible<const G&, TError>::value, bool>::type = false>
    ETL_CONSTEXPR14 explicit expected(etl::unexpected<G>&& ue)
      : storage(etl::in_place_index_t<Error_Type>(), etl::move(ue.error()))
    {
    }

    template <typename G, typename etl::enable_if<etl::is_convertible<const G&, TError>::value, bool>::type = false>
    ETL_CONSTEXPR14 expected(etl::unexpected<G>&& ue)
      : storage(etl::in_place_index_t<Error_Type>(), etl::move(ue.error()))
    {
    }
#endif

    //*******************************************
    /// Construct with default value type.
    //*******************************************
    ETL_CONSTEXPR14 explicit expected(etl::in_place_t) ETL_NOEXCEPT
      : storage(value_type())
    {
    }

#if ETL_USING_CPP11
    //*******************************************
    /// Construct value type from arguments.
    //*******************************************
    template <typename... Args>
    ETL_CONSTEXPR14 explicit expected(etl::in_place_t, Args&&... args)
      : storage(etl::in_place_index_t<Value_Type>(), etl::forward<Args>(args)...)
    {
    }

#if ETL_HAS_INITIALIZER_LIST
    //*******************************************
    /// Construct value type from initializser_list and arguments.
    //*******************************************
    template <typename U, typename... Args>
    ETL_CONSTEXPR14 explicit expected(etl::in_place_t, std::initializer_list<U> il, Args&&... args)
      : storage(etl::in_place_index_t<Value_Type>(), il, etl::forward<Args>(args)...)
    {
    }
#endif

    //*******************************************
    /// Construct error type from arguments.
    //*******************************************
    template <typename... Args>
    ETL_CONSTEXPR14 explicit expected(etl::unexpect_t, Args&&... args)
      : storage(error_type(etl::forward<Args>(args)...))
    {
    }

#if ETL_HAS_INITIALIZER_LIST
    //*******************************************
    /// Construct error type from initializser_list and arguments.
    //*******************************************
    template <typename U, typename... Args>
    ETL_CONSTEXPR14 explicit expected(etl::unexpect_t, std::initializer_list<U> il, Args&&... args)
      : storage(error_type(il, etl::forward<Args>(args)...))
    {
    }
#endif
#endif

    //*******************************************
    /// Copy assign from etl::expected.
    //*******************************************
    this_type& operator =(const this_type& other)
    {
      ETL_STATIC_ASSERT(etl::is_copy_constructible<TValue>::value && etl::is_copy_constructible<TError>::value, "Not copy assignable");

      storage = other.storage;

      return *this;
    }

#if ETL_USING_CPP11
    //*******************************************
    /// Move assign from etl::expected.
    //*******************************************
    this_type& operator =(this_type&& other)
    {
      ETL_STATIC_ASSERT(etl::is_move_constructible<TValue>::value && etl::is_move_constructible<TError>::value, "Not move assignable");

      storage = etl::move(other.storage);

      return *this;
    }
#endif

    //*******************************************
    /// Copy assign from value
    //*******************************************
    expected& operator =(const value_type& value)
    {
      ETL_STATIC_ASSERT(etl::is_copy_constructible<TValue>::value, "Value not copy assignable");

      storage.template emplace<Value_Type>(value);

      return *this;
    }

#if ETL_USING_CPP11
    //*******************************************
    /// Move assign from value
    //*******************************************
    expected& operator =(value_type&& value)
    {
      ETL_STATIC_ASSERT(etl::is_move_constructible<TValue>::value, "Value not move assignable");

      storage.template emplace<Value_Type>(etl::move(value));

      return *this;
    }
#endif

    //*******************************************
    /// Copy assign from unexpected
    //*******************************************
    expected& operator =(const unexpected_type& ue)
    {
#if ETL_USING_CPP11
      ETL_STATIC_ASSERT(etl::is_copy_constructible<TError>::value, "Error not copy assignable");
#endif

      storage.template emplace<Error_Type>(ue.error());

      return *this;
    }

#if ETL_USING_CPP11
    //*******************************************
    /// Move assign from unexpected
    //*******************************************
    expected& operator =(unexpected_type&& ue)
    {
      ETL_STATIC_ASSERT(etl::is_move_constructible<TError>::value, "Error not move assignable");

      storage.template emplace<Error_Type>(etl::move(ue.error()));

      return *this;
    }
#endif

#if ETL_USING_CPP11
    //*******************************************
    /// Get the value.
    //*******************************************
    ETL_CONSTEXPR14 value_type& value()&
    {
      return etl::get<Value_Type>(storage);
    }

    //*******************************************
    /// Get the value.
    //*******************************************
    ETL_CONSTEXPR14 const value_type& value() const&
    {
      return etl::get<Value_Type>(storage);
    }

    //*******************************************
    /// Get the value.
    //*******************************************
    ETL_CONSTEXPR14 value_type&& value()&&
    {
      return etl::move(etl::get<Value_Type>(storage));
    }

    //*******************************************
    /// Get the value.
    //*******************************************
    ETL_CONSTEXPR14 const value_type&& value() const&&
    {
      return etl::move(etl::get<Value_Type>(storage));
    }
#else
    //*******************************************
    /// Get the value.
    //*******************************************
    const value_type& value() const
    {
      return etl::get<Value_Type>(storage);
    }
#endif

    //*******************************************
    ///
    //*******************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    bool has_value() const ETL_NOEXCEPT
    {
      return (storage.index() == Value_Type);
    }

    //*******************************************
    ///
    //*******************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    ETL_EXPLICIT
    operator bool() const ETL_NOEXCEPT
    {
      return has_value();
    }

#if ETL_USING_CPP11
    //*******************************************
    ///
    //*******************************************
    template <typename U>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    etl::enable_if_t<etl::is_convertible<U, value_type>::value, value_type>
      value_or(U&& default_value) const&
    {
      if (has_value())
      {
        return value();
      }
      else
      {
        return static_cast<value_type>(etl::forward<U>(default_value));
      }
    }

    //*******************************************
    ///
    //*******************************************
    template <typename U>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    etl::enable_if_t<etl::is_convertible<U, value_type>::value, value_type>
      value_or(U&& default_value)&&
    {
      if (has_value())
      {
        return etl::move(value());
      }
      else
      {
        return static_cast<value_type>(etl::forward<U>(default_value));
      }
    }

    //*******************************************
    ///
    //*******************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    error_type& error()& ETL_NOEXCEPT
    {
      return etl::get<Error_Type>(storage);
    }

    //*******************************************
    ///
    //*******************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    const error_type& error() const& ETL_NOEXCEPT
    {
      return etl::get<Error_Type>(storage);
    }

    //*******************************************
    ///
    //*******************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    error_type&& error()&& ETL_NOEXCEPT
    {
      return etl::move(etl::get<Error_Type>(storage));
    }

    //*******************************************
    ///
    //*******************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    const error_type&& error() const&& ETL_NOEXCEPT
    {
      return etl::move(etl::get<Error_Type>(storage));
    }


    //*******************************************
    /// Swap with another etl::expected.
    //*******************************************
    void swap(this_type& other)
    {
      using ETL_OR_STD::swap;

      swap(storage, other.storage);
    }

    //*******************************************
    ///
    //*******************************************
    template <typename... Args>
    ETL_CONSTEXPR14 value_type& emplace(Args&&... args) ETL_NOEXCEPT
    {
      storage.template emplace<value_type>(etl::forward<Args>(args)...);

      return value();
    }

    //*******************************************
    ///
    //*******************************************
#if ETL_HAS_INITIALIZER_LIST
    template <typename U, typename... Args>
    ETL_CONSTEXPR14 value_type& emplace(std::initializer_list<U> il, Args&&... args) ETL_NOEXCEPT
    {
      storage.template emplace<value_type>(il, etl::forward<Args>(args)...);

      return value();
    }
#endif
#else
    //*******************************************
    ///
    //*******************************************
    template <typename U>
    value_type value_or(const U& default_value) const
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
    const error_type& error() const
    {
      return etl::get<Error_Type>(storage);
    }
#endif

    //*******************************************
    ///
    //*******************************************
    value_type* operator ->()
    {
      ETL_ASSERT_OR_RETURN_VALUE(has_value(), ETL_ERROR(expected_invalid), ETL_NULLPTR);

      return etl::addressof(etl::get<value_type>(storage));
    }

    //*******************************************
    ///
    //*******************************************
    const value_type* operator ->() const
    {
      ETL_ASSERT_OR_RETURN_VALUE(has_value(), ETL_ERROR(expected_invalid), ETL_NULLPTR);

      return etl::addressof(etl::get<value_type>(storage));
    }

    //*******************************************
    ///
    //*******************************************
    value_type& operator *() ETL_LVALUE_REF_QUALIFIER
    {
      ETL_ASSERT(has_value(), ETL_ERROR(expected_invalid));

      return etl::get<value_type>(storage);
    }

    //*******************************************
    ///
    //*******************************************
    const value_type& operator *() const ETL_LVALUE_REF_QUALIFIER
    {
      ETL_ASSERT_OR_RETURN_VALUE(has_value(), ETL_ERROR(expected_invalid), ETL_NULLPTR);

      return etl::get<value_type>(storage);
    }

#if ETL_USING_CPP11
    //*******************************************
    ///
    //*******************************************
    value_type&& operator *()&&
    {
      ETL_ASSERT_OR_RETURN_VALUE(has_value(), ETL_ERROR(expected_invalid), ETL_NULLPTR);

      return etl::move(etl::get<value_type>(storage));
    }

    //*******************************************
    ///
    //*******************************************
    const value_type&& operator *() const&&
    {
      ETL_ASSERT(has_value(), ETL_ERROR(expected_invalid));

      return etl::move(etl::get<value_type>(storage));
    }
#endif

#if ETL_USING_CPP11
    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, TValue&>::type>::type>
    auto transform(F&& f) & -> expected<U, TError>
    {
      return transform_impl<F, this_type&, U, TValue&>(etl::forward<F>(f), *this);
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, const TValue&>::type>::type>
    auto transform(F&& f) const& -> expected<U, TError>
    {
      return transform_impl<F, const this_type&, U, const TValue&>(etl::forward<F>(f), *this);
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, TValue&&>::type>::type>
    auto transform(F&& f) && -> expected<U, TError>
    {
      return transform_impl<F, this_type&&, U, TValue&&>(etl::forward<F>(f), etl::move(*this));
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, const TValue&&>::type>::type>
    auto transform(F&& f) const&& -> expected<U, TError>
    {
      return transform_impl<F, const this_type&&, U, const TValue&&>(etl::forward<F>(f), etl::move(*this));
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, TValue&>::type>::type>
    auto and_then(F&& f) & -> U
    {
      return and_then_impl<F, this_type&, U, TValue&>(etl::forward<F>(f), *this);
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, const TValue&>::type>::type>
    auto and_then(F&& f) const& -> U
    {
      return and_then_impl<F, const this_type&, U, const TValue&>(etl::forward<F>(f), *this);
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, TValue&&>::type>::type>
    auto and_then(F&& f) && -> U
    {
      return and_then_impl<F, this_type&&, U, TValue&&>(etl::forward<F>(f), etl::move(*this));
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, const TValue&&>::type>::type>
    auto and_then(F&& f) const&& -> U
    {
      return and_then_impl<F, const this_type&&, U, const TValue&&>(etl::forward<F>(f), etl::move(*this));
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, TError&>::type>::type>
    auto or_else(F&& f) & -> U
    {
      return or_else_impl<F, this_type&, U, TError&>(etl::forward<F>(f), *this);
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, const TError&>::type>::type>
    auto or_else(F&& f) const & -> U
    {
      return or_else_impl<F, const this_type&, U, const TError&>(etl::forward<F>(f), *this);
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, TError&&>::type>::type>
    auto or_else(F&& f) && -> U
    {
      return or_else_impl<F, this_type&&, U, TError&&>(etl::forward<F>(f), etl::move(*this));
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, const TError&&>::type>::type>
    auto or_else(F&& f) const && -> U
    {
      return or_else_impl<F, const this_type&&, U, const TError&&>(etl::forward<F>(f), etl::move(*this));
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, TError&>::type>::type>
    auto transform_error(F&& f) & -> expected<TValue, U>
    {
      return transform_error_impl<F, this_type&, U, TError&>(etl::forward<F>(f), *this);
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, const TError&>::type>::type>
    auto transform_error(F&& f) const & -> expected<TValue, U>
    {
      return transform_error_impl<F, const this_type&, U, const TError&>(etl::forward<F>(f), *this);
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, TError&&>::type>::type>
    auto transform_error(F&& f) && -> expected<TValue, U>
    {
      return transform_error_impl<F, this_type&&, U, TError&&>(etl::forward<F>(f), etl::move(*this));
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, const TError&&>::type>::type>
    auto transform_error(F&& f) const&& -> expected<TValue, U>
    {
      return transform_error_impl<F, const this_type&&, U, const TError&&>(etl::forward<F>(f), etl::move(*this));
    }
#endif

  private:

    enum
    {
      Uninitialised,
      Value_Type,
      Error_Type
    };

    typedef etl::variant<etl::monostate, value_type, error_type> storage_type;
    storage_type storage;

    template <typename F, typename TExp, typename TRet, typename TValueRef, typename = typename etl::enable_if<!etl::is_void<TRet>::value>::type>
    auto transform_impl(F&& f, TExp&& exp) const -> expected<TRet, TError>
    {
      if (exp.has_value())
      {
        return expected<TRet, TError>(etl::invoke(etl::forward<F>(f), etl::forward<TValueRef>(etl::get<Value_Type>(exp.storage))));
      }
      else
      {
        return expected<TRet, TError>(unexpected<TError>(etl::forward<TExp>(exp).error()));
      }
    }

    template <typename F, typename TExp, typename TRet, typename TValueRef, typename = typename etl::enable_if<etl::is_void<TRet>::value>::type>
    auto transform_impl(F&& f, TExp&& exp) const -> expected<void, TError>
    {
      if (exp.has_value())
      {
        etl::invoke(etl::forward<F>(f), etl::forward<TValueRef>(etl::get<Value_Type>(exp.storage)));
        return expected<void, TError>();
      }
      else
      {
        return expected<void, TError>(unexpected<TError>(etl::forward<TExp>(exp).error()));
      }
    }

    template <typename F, typename TExp, typename TRet, typename TValueRef, typename = typename etl::enable_if<!etl::is_void<TRet>::value && etl::is_expected<TRet>::value && etl::is_same<typename TRet::error_type, TError>::value>::type>
    auto and_then_impl(F&& f, TExp&& exp) const -> TRet
    {
      if (exp.has_value())
      {
        return etl::invoke(etl::forward<F>(f), etl::forward<TValueRef>(etl::get<Value_Type>(exp.storage)));
      }
      else
      {
        return TRet(unexpected<TError>(etl::forward<TExp>(exp).error()));
      }
    }

    template <typename F, typename TExp, typename TRet, typename TErrorRef, typename = typename etl::enable_if<!etl::is_void<TRet>::value && etl::is_expected<TRet>::value && etl::is_same<typename TRet::value_type, TValue>::value>::type>
    auto or_else_impl(F&& f, TExp&& exp) const -> TRet
    {
      if (exp.has_value())
      {
        return TRet(etl::forward<TExp>(exp).value());
      }
      else
      {
        return etl::invoke(etl::forward<F>(f), etl::forward<TErrorRef>(etl::get<Error_Type>(exp.storage)));
      }
    }

    template <typename F, typename TExp, typename TRet, typename TErrorRef, typename = typename etl::enable_if<!etl::is_void<TRet>::value>::type>
    auto transform_error_impl(F&& f, TExp&& exp) const -> expected<TValue, TRet>
    {
      if (exp.has_value())
      {
        return expected<TValue, TRet>(etl::forward<TExp>(exp).value());
      }
      else
      {
        return expected<TValue, TRet>(unexpected<TRet>(etl::invoke(etl::forward<F>(f), etl::forward<TErrorRef>(etl::get<Error_Type>(exp.storage)))));
      }
    }
  };

  //*****************************************************************************
  /// Specialisation for void value type.
  //*****************************************************************************
  template<typename TError>
  class expected<void, TError>
  {
  public:

    typedef etl::expected<void, TError> this_type;
    typedef void                        value_type;
    typedef TError                      error_type;
    typedef etl::unexpected<TError>     unexpected_type;

    //*******************************************
    /// Default constructor
    //*******************************************
    ETL_CONSTEXPR14
      expected()
    {
    }

    //*******************************************
    /// Copy construct from unexpected
    //*******************************************
    ETL_CONSTEXPR14
      expected(const unexpected_type& ue_)
      : storage(ue_.error())
    {
    }

#if ETL_USING_CPP11
    //*******************************************
    /// Move construct from unexpected
    //*******************************************
    ETL_CONSTEXPR14
      expected(unexpected_type&& ue_)
      : storage(etl::move(ue_.error()))
    {
    }
#endif

    //*******************************************
    /// Copy construct
    //*******************************************
    ETL_CONSTEXPR14
      expected(const this_type& other)
      : storage(other.storage)
    {
    }

#if ETL_USING_CPP11
    //*******************************************
    /// Move construct
    //*******************************************
    ETL_CONSTEXPR14
      expected(this_type&& other)
      : storage(etl::move(other.storage))
    {
    }
#endif

    //*******************************************
    /// Copy assign
    //*******************************************
    this_type& operator =(const this_type& other)
    {
      ETL_STATIC_ASSERT(etl::is_copy_constructible<TError>::value, "Not copy assignable");

      storage = other.storage;
      return *this;
    }

#if ETL_USING_CPP11
    //*******************************************
    /// Move assign
    //*******************************************
    this_type& operator =(this_type&& other)
    {
      ETL_STATIC_ASSERT(etl::is_move_constructible<TError>::value, "Not move assignable");

      storage = etl::move(other.storage);
      return *this;
    }
#endif

    //*******************************************
    /// Copy assign from unexpected
    //*******************************************
    expected& operator =(const unexpected_type& ue)
    {
#if ETL_USING_CPP11
      ETL_STATIC_ASSERT(etl::is_copy_constructible<TError>::value, "Error not copy assignable");
#endif

      storage.template emplace<Error_Type>(ue.error());
      return *this;
    }

#if ETL_USING_CPP11
    //*******************************************
    /// Move assign from unexpected
    //*******************************************
    expected& operator =(unexpected_type&& ue)
    {
      ETL_STATIC_ASSERT(etl::is_move_constructible<TError>::value, "Error not move assignable");

      storage.template emplace<Error_Type>(etl::move(ue.error()));
      return *this;
    }
#endif

    //*******************************************
    /// Returns true if expected has a value
    //*******************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    bool has_value() const ETL_NOEXCEPT
    {
      return (storage.index() != Error_Type);
    }

    //*******************************************
    /// Returns true if expected has a value
    //*******************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    ETL_EXPLICIT
    operator bool() const ETL_NOEXCEPT
    {
      return has_value();
    }

#if ETL_USING_CPP11
    //*******************************************
    /// Returns the error
    /// Undefined behaviour if an error has not been set.
    //*******************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    error_type& error()& ETL_NOEXCEPT
    {
      return etl::get<Error_Type>(storage);
    }

    //*******************************************
    /// Returns the error
    /// Undefined behaviour if an error has not been set.
    //*******************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    const error_type& error() const& ETL_NOEXCEPT
    {
      return etl::get<Error_Type>(storage);
    }

    //*******************************************
    /// Returns the error
    /// Undefined behaviour if an error has not been set.
    //*******************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    error_type&& error() && ETL_NOEXCEPT
    {
      return etl::move(etl::get<Error_Type>(storage));
    }

    //*******************************************
    /// Returns the error
    /// Undefined behaviour if an error has not been set.
    //*******************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    const error_type&& error() const&& ETL_NOEXCEPT
    {
      return etl::move(etl::get<Error_Type>(storage));
    }
#else
    //*******************************************
    /// Returns the error
    /// Undefined behaviour if an error has not been set.
    //*******************************************
    const error_type& error() const
    {
      return etl::get<Error_Type>(storage);
    }
#endif

    //*******************************************
    /// Swap with another etl::expected.
    //*******************************************
    void swap(this_type& other)
    {
      using ETL_OR_STD::swap;

      swap(storage, other.storage);
    }

#if ETL_USING_CPP11
    template<typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void>::type>::type>
    auto transform(F&& f) & -> expected<U, TError>
    {
      return transform_impl<F, this_type&, U>(etl::forward<F>(f), *this);
    }

    template<typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void>::type>::type>
    auto transform(F&& f) const & -> expected<U, TError>
    {
      return transform_impl<F, const this_type&, U>(etl::forward<F>(f), *this);
    }

    template<typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void>::type>::type>
    auto transform(F&& f) && -> expected<U, TError>
    {
      return transform_impl<F, this_type&&, U>(etl::forward<F>(f), etl::move(*this));
    }

    template<typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void>::type>::type>
    auto transform(F&& f) const && -> expected<U, TError>
    {
      return transform_impl<F, const this_type&&, U>(etl::forward<F>(f), etl::move(*this));
    }

    template<typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void>::type>::type>
    auto and_then(F&& f) & -> U
    {
      return and_then_impl<F, this_type&, U>(etl::forward<F>(f), *this);
    }

    template<typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void>::type>::type>
    auto and_then(F&& f) const & -> U
    {
      return and_then_impl<F, const this_type&, U>(etl::forward<F>(f), *this);
    }

    template<typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void>::type>::type>
    auto and_then(F&& f) && -> U
    {
      return and_then_impl<F, this_type&&, U>(etl::forward<F>(f), etl::move(*this));
    }

    template<typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void>::type>::type>
    auto and_then(F&& f) const && -> U
    {
      return and_then_impl<F, const this_type&&, U>(etl::forward<F>(f), etl::move(*this));
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, TError&>::type>::type>
    auto or_else(F&& f) & -> U
    {
      return or_else_impl<F, this_type&, U, TError&>(etl::forward<F>(f), *this);
    }
    
    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, const TError&>::type>::type>
    auto or_else(F&& f) const & -> U
    {
      return or_else_impl<F, const this_type&, U, const TError&>(etl::forward<F>(f), *this);
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, TError&&>::type>::type>
    auto or_else(F&& f) && -> U
    {
      return or_else_impl<F, this_type&&, U, TError&&>(etl::forward<F>(f), etl::move(*this));
    }
    
    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, const TError&&>::type>::type>
    auto or_else(F&& f) const && -> U
    {
      return or_else_impl<F, const this_type&&, U, const TError&&>(etl::forward<F>(f), etl::move(*this));
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, TError&>::type>::type>
    auto transform_error(F&& f) & -> expected<void, U>
    {
      return transform_error_impl<F, this_type&, U, TError&>(etl::forward<F>(f), *this);
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, const TError&>::type>::type>
    auto transform_error(F&& f) const & -> expected<void, U>
    {
      return transform_error_impl<F, const this_type&, U, const TError&>(etl::forward<F>(f), *this);
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, TError&&>::type>::type>
    auto transform_error(F&& f) && -> expected<void, U>
    {
      return transform_error_impl<F, this_type&&, U, TError&&>(etl::forward<F>(f), etl::move(*this));
    }

    template <typename F, typename U = typename etl::remove_cvref<typename etl::invoke_result<F, void, const TError&&>::type>::type>
    auto transform_error(F&& f) const && -> expected<void, U>
    {
      return transform_error_impl<F, const this_type&&, U, const TError&&>(etl::forward<F>(f), *this);
    }
#endif
  
    
  private:

    enum
    {
      Uninitialised,
      Error_Type
    };

    etl::variant<etl::monostate, error_type> storage;

    template <typename F, typename TExp, typename TRet, typename = typename etl::enable_if<!etl::is_void<TRet>::value>::type>
    auto transform_impl(F&& f, TExp&& exp) const -> expected<TRet, TError>
    {
      if (exp.has_value())
      {
        return expected<TRet, TError>(etl::invoke(etl::forward<F>(f)));
      }
      else
      {
        return expected<TRet, TError>(unexpected<TError>(etl::forward<TExp>(exp).error()));
      }
    }

    template <typename F, typename TExp, typename TRet, typename = typename etl::enable_if<etl::is_void<TRet>::value>::type>
    auto transform_impl(F&& f, TExp&& exp) const -> expected<void, TError>
    {
      if (exp.has_value())
      {
        etl::invoke(etl::forward<F>(f));
        return expected<void, TError>();
      }
      else
      {
        return expected<void, TError>(unexpected<TError>(etl::forward<TExp>(exp).error()));
      }
    }

    template <typename F, typename TExp, typename TRet, typename = typename etl::enable_if<!etl::is_void<TRet>::value && etl::is_expected<TRet>::value && etl::is_same<typename TRet::error_type, TError>::value>::type>
    auto and_then_impl(F&& f, TExp&& exp) const -> TRet
    {
      if (exp.has_value())
      {
        return etl::invoke(etl::forward<F>(f));
      }
      else
      {
        return TRet(unexpected<TError>(etl::forward<TExp>(exp).error()));
      }
    }

    template <typename F, typename TExp, typename TRet, typename TErrorRef, typename = typename etl::enable_if<!etl::is_void<TRet>::value && etl::is_expected<TRet>::value && etl::is_same<typename TRet::value_type, void>::value>::type>
    auto or_else_impl(F&& f, TExp&& exp) const -> TRet
    {
      if (exp.has_value())
      {
        return TRet();
      }
      else
      {
        return etl::invoke(etl::forward<F>(f), etl::forward<TErrorRef>(etl::get<Error_Type>(exp.storage)));
      }
    }

    template <typename F, typename TExp, typename TRet, typename TErrorRef, typename = typename etl::enable_if<!etl::is_void<TRet>::value>::type>
    auto transform_error_impl(F&& f, TExp&& exp) const -> expected<void, TRet>
    {
      if (exp.has_value())
      {
        return expected<void, TRet>();
      }
      else
      {
        return expected<void, TRet>(unexpected<TRet>(etl::invoke(etl::forward<F>(f), etl::forward<TErrorRef>(etl::get<Error_Type>(exp.storage)))));
      }
    }
  };
}

//*******************************************
/// Equivalence operators.
//*******************************************
template <typename TValue, typename TError, typename TValue2, typename TError2>
ETL_CONSTEXPR14
bool operator ==(const etl::expected<TValue, TError>& lhs, const etl::expected<TValue2, TError2>& rhs)
{
  if (lhs.has_value() != rhs.has_value())
  {
    return false;
  }
  if (lhs.has_value())
  {
    return lhs.value() == rhs.value();
  }
  return lhs.error() == rhs.error();
}

//*******************************************
template <typename TValue, typename TError, typename TValue2>
ETL_CONSTEXPR14
bool operator ==(const etl::expected<TValue, TError>& lhs, const TValue2& rhs)
{
  if (!lhs.has_value())
  {
    return false;
  }
  return lhs.value() == rhs;
}

//*******************************************
template <typename TValue, typename TError, typename TError2>
ETL_CONSTEXPR14
bool operator ==(const etl::expected<TValue, TError>& lhs, const etl::unexpected<TError2>& rhs)
{
  if (lhs.has_value())
  {
    return false;
  }
  return lhs.error() == rhs.error();
}

//*******************************************
template <typename TError, typename TError2>
ETL_CONSTEXPR14
bool operator ==(const etl::expected<void, TError>& lhs, const etl::expected<void, TError2>& rhs)
{
  if (lhs.has_value() != rhs.has_value())
  {
    return false;
  }
  if (lhs.has_value())
  {
    return true;
  }
  return lhs.error() == rhs.error();
}

//*******************************************
template <typename TError, typename TError2>
ETL_CONSTEXPR14
bool operator ==(const etl::expected<void, TError>& lhs, const etl::unexpected<TError2>& rhs)
{
  if (lhs.has_value())
  {
    return false;
  }
  return lhs.error() == rhs.error();
}

//*******************************************
template <typename TError, typename TError2>
ETL_CONSTEXPR14
bool operator ==(const etl::unexpected<TError>& lhs, const etl::unexpected<TError2>& rhs)
{
  return lhs.error() == rhs.error();
}

//*******************************************
template <typename TValue, typename TError, typename TValue2, typename TError2>
ETL_CONSTEXPR14
bool operator !=(const etl::expected<TValue, TError>& lhs, const etl::expected<TValue2, TError2>& rhs)
{
  return !(lhs == rhs);
}

//*******************************************
template <typename TValue, typename TError, typename TValue2>
ETL_CONSTEXPR14
bool operator !=(const etl::expected<TValue, TError>& lhs, const TValue2& rhs)
{
  return !(lhs == rhs);
}

//*******************************************
template <typename TValue, typename TError, typename TError2>
ETL_CONSTEXPR14
bool operator !=(const etl::expected<TValue, TError>& lhs, const etl::unexpected<TError2>& rhs)
{
  return !(lhs == rhs);
}

//*******************************************
template <typename TError, typename TError2>
ETL_CONSTEXPR14
bool operator !=(const etl::expected<void, TError>& lhs, const etl::expected<void, TError2>& rhs)
{
  return !(lhs == rhs);
}

//*******************************************
template <typename TError, typename TError2>
ETL_CONSTEXPR14
bool operator !=(const etl::expected<void, TError>& lhs, const etl::unexpected<TError2>& rhs)
{
  return !(lhs == rhs);
}

//*******************************************
template <typename TError, typename TError2>
ETL_CONSTEXPR14
bool operator !=(const etl::unexpected<TError>& lhs, const etl::unexpected<TError2>& rhs)
{
  return !(lhs == rhs);
}

//*******************************************
/// Swap etl::expected.
//*******************************************
template <typename TValue, typename TError>
ETL_CONSTEXPR14
void swap(etl::expected<TValue, TError>& lhs, etl::expected<TValue, TError>& rhs)
{
  lhs.swap(rhs);
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

#endif
