///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove, Robin S�derholm

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

#include "../platform.h"
#include "../alignment.h"
#include "../compare.h"
#include "../error_handler.h"
#include "../exception.h"
#include "../initializer_list.h"
#include "../integral_limits.h"
#include "../largest.h"
#include "../memory.h"
#include "../monostate.h"
#include "../nth_type.h"
#include "../placement_new.h"
#include "../static_assert.h"
#include "../type_list.h"
#include "../type_traits.h"
#include "../utility.h"
#include "../visitor.h"

#include <stdint.h>
#include <string.h>

#if defined(ETL_COMPILER_KEIL)
  #pragma diag_suppress 940
  #pragma diag_suppress 111
#endif

#if ETL_CPP11_NOT_SUPPORTED
  #if !defined(ETL_IN_UNIT_TEST)
    #error NOT SUPPORTED FOR C++03 OR BELOW
  #endif
#else
//*****************************************************************************
///\defgroup variant variant
/// A class that can contain one a several specified types in a type safe manner.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  namespace private_variant
  {
    //*******************************************
    /// Switch-based dispatch for destroy/copy/move.
    /// Replaces the per-instance function pointer with
    /// an inline if-else chain the compiler can optimise
    /// into a switch / jump-table, enabling inlining and
    /// dead-code elimination.
    //*******************************************
    template <size_t Index, typename... TTypes>
    struct variant_operations;

    // Base case: no types left.
    template <size_t Index>
    struct variant_operations<Index>
    {
      static void destroy(char*, size_t) {}
      static void copy(char*, const char*, size_t) {}
      static void move(char*, const char*, size_t) {}
    };

    // Recursive case.
    template <size_t Index, typename THead, typename... TRest>
    struct variant_operations<Index, THead, TRest...>
    {
      static void destroy(char* data, size_t type_id)
      {
        if (type_id == Index)
        {
          reinterpret_cast<const THead*>(data)->~THead();
        }
        else
        {
          variant_operations<Index + 1, TRest...>::destroy(data, type_id);
        }
      }

      static void copy(char* dst, const char* src, size_t type_id)
      {
        if (type_id == Index)
        {
          copy_impl(dst, src, etl::integral_constant<bool, etl::is_copy_constructible<THead>::value>{});
        }
        else
        {
          variant_operations<Index + 1, TRest...>::copy(dst, src, type_id);
        }
      }

      static void move(char* dst, const char* src, size_t type_id)
      {
        if (type_id == Index)
        {
          move_impl(dst, src, etl::integral_constant<bool, etl::is_move_constructible<THead>::value>{});
        }
        else
        {
          variant_operations<Index + 1, TRest...>::move(dst, src, type_id);
        }
      }

    private:

      static void copy_impl(char* dst, const char* src, etl::true_type)
      {
        ::new (dst) THead(*reinterpret_cast<const THead*>(src));
      }

      static void copy_impl(char*, const char*, etl::false_type) {}

      static void move_impl(char* dst, const char* src, etl::true_type)
      {
        ::new (dst) THead(etl::move(*reinterpret_cast<THead*>(const_cast<char*>(src))));
      }

      static void move_impl(char*, const char*, etl::false_type) {}
    };

    //*******************************************
    /// Trait: are all types trivially destructible?
    //*******************************************
    template <typename... TTypes>
    struct are_all_trivially_destructible : etl::conjunction<etl::is_trivially_destructible<TTypes>...>
    {
    };

    //*******************************************
    /// Recursive variadic union for constexpr-friendly storage.
    /// Used when all variant types are trivially destructible.
    //*******************************************
    template <typename... TTypes>
    union variadic_union;

    /// Base case: empty union.
    template <>
    union variadic_union<>
    {
      constexpr variadic_union() ETL_NOEXCEPT {}
    };

    /// Recursive case: union of head type and tail union.
    template <typename THead, typename... TRest>
    union variadic_union<THead, TRest...>
    {
      THead                    head;
      variadic_union<TRest...> tail;

      constexpr variadic_union() ETL_NOEXCEPT
        : tail()
      {
      }

      // Constructor for head element (index 0).
      template <typename T>
      constexpr variadic_union(etl::in_place_index_t<0>, T&& value)
        : head(etl::forward<T>(value))
      {
      }

      // Constructor for tail elements (index > 0).
      template <size_t Index, typename T>
      constexpr variadic_union(etl::in_place_index_t<Index>, T&& value)
        : tail(etl::in_place_index_t<Index - 1>{}, etl::forward<T>(value))
      {
      }
    };

    //*******************************************
    /// Constexpr get by index from variadic_union.
    //*******************************************
    // Non-const lvalue reference
    template <size_t Index, typename THead, typename... TRest>
    ETL_CONSTEXPR14 typename etl::enable_if_t<(Index == 0), THead&> variadic_union_get(variadic_union<THead, TRest...>& u) ETL_NOEXCEPT
    {
      return u.head;
    }

    template <size_t Index, typename THead, typename... TRest>
    ETL_CONSTEXPR14 typename etl::enable_if_t<(Index != 0), etl::nth_type_t<Index, THead, TRest...>&>
      variadic_union_get(variadic_union<THead, TRest...>& u) ETL_NOEXCEPT
    {
      return variadic_union_get<Index - 1>(u.tail);
    }

    // Const lvalue reference
    template <size_t Index, typename THead, typename... TRest>
    constexpr typename etl::enable_if_t<(Index == 0), const THead&> variadic_union_get(const variadic_union<THead, TRest...>& u) ETL_NOEXCEPT
    {
      return u.head;
    }

    template <size_t Index, typename THead, typename... TRest>
    constexpr typename etl::enable_if_t<(Index != 0), const etl::nth_type_t<Index, THead, TRest...>&>
      variadic_union_get(const variadic_union<THead, TRest...>& u) ETL_NOEXCEPT
    {
      return variadic_union_get<Index - 1>(u.tail);
    }

    // Rvalue reference
    template <size_t Index, typename THead, typename... TRest>
    ETL_CONSTEXPR14 typename etl::enable_if_t<(Index == 0), THead&&> variadic_union_get(variadic_union<THead, TRest...>&& u) ETL_NOEXCEPT
    {
      return etl::move(u.head);
    }

    template <size_t Index, typename THead, typename... TRest>
    ETL_CONSTEXPR14 typename etl::enable_if_t<(Index != 0), etl::nth_type_t<Index, THead, TRest...>&&>
      variadic_union_get(variadic_union<THead, TRest...>&& u) ETL_NOEXCEPT
    {
      return variadic_union_get<Index - 1>(etl::move(u.tail));
    }

    // Const rvalue reference
    template <size_t Index, typename THead, typename... TRest>
    constexpr typename etl::enable_if_t<(Index == 0), const THead&&> variadic_union_get(const variadic_union<THead, TRest...>&& u) ETL_NOEXCEPT
    {
      return etl::move(u.head);
    }

    template <size_t Index, typename THead, typename... TRest>
    constexpr typename etl::enable_if_t<(Index != 0), const etl::nth_type_t<Index, THead, TRest...>&&>
      variadic_union_get(const variadic_union<THead, TRest...>&& u) ETL_NOEXCEPT
    {
      return variadic_union_get<Index - 1>(etl::move(u.tail));
    }
  } // namespace private_variant

  /// Definition of variant_npos.
  constexpr size_t variant_npos = etl::integral_limits<size_t>::max;

  //***********************************
  // variant. Forward declaration
  template <typename... TTypes>
  class variant;

  //***************************************************************************
  /// variant_alternative
  //***************************************************************************
  template <size_t Index, typename T>
  struct variant_alternative;

  template <size_t Index, typename... TTypes>
  struct variant_alternative<Index, etl::variant<TTypes...> >
  {
    using type = etl::nth_type_t<Index, TTypes...>;
  };

  template <size_t Index, typename T>
  struct variant_alternative<Index, const T>
  {
    using type = typename variant_alternative<Index, T>::type;
  };

  template <size_t Index, typename T>
  using variant_alternative_t = typename variant_alternative<Index, T>::type;

  //***********************************
  // holds_alternative. Forward declaration
  template <typename T, typename... TTypes>
  ETL_CONSTEXPR14 bool holds_alternative(const etl::variant<TTypes...>& v) ETL_NOEXCEPT;

  //***********************************
  // get. Forward declarations
  template <size_t Index, typename... VTypes>
  ETL_CONSTEXPR14 etl::variant_alternative_t<Index, etl::variant<VTypes...> >& get(etl::variant<VTypes...>& v);

  template <size_t Index, typename... VTypes>
  ETL_CONSTEXPR14 etl::variant_alternative_t<Index, etl::variant<VTypes...> >&& get(etl::variant<VTypes...>&& v);

  template <size_t Index, typename... VTypes>
  ETL_CONSTEXPR14 const etl::variant_alternative_t<Index, const etl::variant<VTypes...> >& get(const etl::variant<VTypes...>& v);

  template <size_t Index, typename... VTypes>
  ETL_CONSTEXPR14 const etl::variant_alternative_t<Index, const etl::variant<VTypes...> >&& get(const etl::variant<VTypes...>&& v);

  template <typename T, typename... VTypes>
  ETL_CONSTEXPR14 T& get(etl::variant<VTypes...>& v);

  template <typename T, typename... VTypes>
  ETL_CONSTEXPR14 T&& get(etl::variant<VTypes...>&& v);

  template <typename T, typename... VTypes>
  ETL_CONSTEXPR14 const T& get(const etl::variant<VTypes...>& v);

  template <typename T, typename... VTypes>
  ETL_CONSTEXPR14 const T&& get(const etl::variant<VTypes...>&& v);

  #if ETL_NOT_USING_CPP17
    #include "variant_select_do_operator.h"
    #include "variant_select_do_visitor.h"
  #endif

  constexpr bool operator>(etl::monostate, etl::monostate) ETL_NOEXCEPT
  {
    return false;
  }
  constexpr bool operator<(etl::monostate, etl::monostate) ETL_NOEXCEPT
  {
    return false;
  }
  constexpr bool operator!=(etl::monostate, etl::monostate) ETL_NOEXCEPT
  {
    return false;
  }
  constexpr bool operator<=(etl::monostate, etl::monostate) ETL_NOEXCEPT
  {
    return true;
  }
  constexpr bool operator>=(etl::monostate, etl::monostate) ETL_NOEXCEPT
  {
    return true;
  }
  constexpr bool operator==(etl::monostate, etl::monostate) ETL_NOEXCEPT
  {
    return true;
  }
  #if ETL_USING_CPP20 && ETL_USING_STL && !(defined(ETL_DEVELOPMENT_OS_APPLE) && defined(ETL_COMPILER_CLANG))
  constexpr std::strong_ordering operator<=>(monostate, monostate) ETL_NOEXCEPT
  {
    return std::strong_ordering::equal;
  }
  #endif

  #if ETL_NOT_USING_STL && !defined(ETL_USE_TYPE_TRAITS_BUILTINS)
  template <>
  struct is_copy_constructible<etl::monostate> : public etl::true_type
  {
  };

  template <>
  struct is_move_constructible<etl::monostate> : public etl::true_type
  {
  };
  #endif

  //***************************************************************************
  /// Base exception for the variant class.
  ///\ingroup variant
  //***************************************************************************
  class variant_exception : public exception
  {
  public:

    variant_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// 'Unsupported type' exception for the variant class.
  ///\ingroup variant
  //***************************************************************************
  class variant_incorrect_type_exception : public variant_exception
  {
  public:

    variant_incorrect_type_exception(string_type file_name_, numeric_type line_number_)
      : variant_exception(ETL_ERROR_TEXT("variant:unsupported type", ETL_VARIANT_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// 'Bad variant access' exception for the variant class.
  ///\ingroup variant
  //***************************************************************************
  class bad_variant_access : public variant_exception
  {
  public:

    bad_variant_access(string_type file_name_, numeric_type line_number_)
      : variant_exception(ETL_ERROR_TEXT("variant:bad variant access", ETL_VARIANT_FILE_ID"B"), file_name_, line_number_)
    {
    }
  };

  namespace private_variant
  {
    //***************************************************************************
    /// variant_base for non-trivially destructible types.
    /// Uses uninitialized_buffer (char array) storage and switch-based
    /// dispatch for copy/move/destroy (no per-instance function pointer).
    //***************************************************************************
    template <bool IsAllTriviallyDestructible, typename... TTypes>
    struct variant_base
    {
      using largest_t               = typename largest_type<TTypes...>::type;
      static const size_t Size      = sizeof(largest_t);
      static const size_t Alignment = etl::largest_alignment<TTypes...>::value;

      etl::uninitialized_buffer<Size, 1U, Alignment> data;
      size_t                                         type_id;

      ETL_CONSTEXPR14 variant_base() noexcept
        : type_id(variant_npos)
      {
      }

      ETL_CONSTEXPR14 variant_base(size_t id) noexcept
        : type_id(id)
      {
      }

      ~variant_base()
      {
        if (type_id != variant_npos)
        {
          variant_operations<0, TTypes...>::destroy(data, type_id);
        }
        type_id = variant_npos;
      }
    };

    //***************************************************************************
    /// variant_base specialisation for trivially destructible types.
    /// Uses variadic_union storage. Destructor is trivial (defaulted), making
    /// the variant a literal type eligible for constexpr / ROM placement.
    /// No operation function pointer is needed since destroy/copy/move are
    /// all handled without indirection for trivially destructible types.
    //***************************************************************************
    template <typename... TTypes>
    struct variant_base<true, TTypes...>
    {
      variadic_union<TTypes...> data;
      size_t                    type_id;

      constexpr variant_base() noexcept
        : data()
        , type_id(variant_npos)
      {
      }

      constexpr variant_base(size_t id) noexcept
        : data()
        , type_id(id)
      {
      }

      template <size_t Index, typename T>
      constexpr variant_base(etl::in_place_index_t<Index>, T&& value,
                             size_t id) noexcept(etl::is_nothrow_constructible<etl::nth_type_t<Index, TTypes...>, T>::value)
        : data(etl::in_place_index_t<Index>{}, etl::forward<T>(value))
        , type_id(id)
      {
      }

      ~variant_base() = default;
    };
  } // namespace private_variant

  //***************************************************************************
  /// A template class that can store any of the types defined in the template parameter list.
  ///\ingroup variant
  //***************************************************************************
  template <typename... TTypes>
  class variant : private private_variant::variant_base<private_variant::are_all_trivially_destructible<TTypes...>::value, TTypes...>
  {
    using base_type = private_variant::variant_base<private_variant::are_all_trivially_destructible<TTypes...>::value, TTypes...>;

    static constexpr bool Is_Trivially_Destructible_Suite = private_variant::are_all_trivially_destructible<TTypes...>::value;

  public:

    using type_list = etl::type_list<TTypes...>;

    //***************************************************************************
    /// get() is a friend function.
    //***************************************************************************
    template <size_t Index, typename... VTypes>
    friend ETL_CONSTEXPR14 etl::variant_alternative_t<Index, etl::variant<VTypes...> >& get(etl::variant<VTypes...>& v);

    template <size_t Index, typename... VTypes>
    friend ETL_CONSTEXPR14 etl::variant_alternative_t<Index, etl::variant<VTypes...> >&& get(etl::variant<VTypes...>&& v);

    template <size_t Index, typename... VTypes>
    friend ETL_CONSTEXPR14 const etl::variant_alternative_t<Index, const etl::variant<VTypes...> >& get(const etl::variant<VTypes...>& v);

    template <size_t Index, typename... VTypes>
    friend ETL_CONSTEXPR14 const etl::variant_alternative_t<Index, const etl::variant<VTypes...> >&& get(const etl::variant<VTypes...>&& v);

    template <typename T, typename... VTypes>
    friend ETL_CONSTEXPR14 T& get(etl::variant<VTypes...>& v);

    template <typename T, typename... VTypes>
    friend ETL_CONSTEXPR14 T&& get(etl::variant<VTypes...>&& v);

    template <typename T, typename... VTypes>
    friend ETL_CONSTEXPR14 const T& get(const etl::variant<VTypes...>& v);

    template <typename T, typename... VTypes>
    friend ETL_CONSTEXPR14 const T&& get(const etl::variant<VTypes...>&& v);

    template < class T, typename... VTypes >
    friend ETL_CONSTEXPR14 etl::add_pointer_t<T> get_if(etl::variant<VTypes...>* pv) ETL_NOEXCEPT;

    template < class T, typename... VTypes >
    friend ETL_CONSTEXPR14 etl::add_pointer_t<const T> get_if(const etl::variant<VTypes...>* pv) ETL_NOEXCEPT;

  private:

    //*******************************************
    // Get the index of a type.
    //*******************************************
    template <typename T>
    using index_of_type = etl::type_list_index_of_type<etl::type_list<TTypes...>, etl::remove_cvref_t<T> >;

    //*******************************************
    // Get the type from the index.
    //*******************************************
    template <size_t Index>
    using type_from_index = typename etl::type_list_type_at_index<etl::type_list<TTypes...>, Index>::type;

    //*******************************************
    // Bring base members into scope.
    //*******************************************
    using base_type::data;
    using base_type::type_id;

  public:

    //***************************************************************************
    /// Default constructor.
    /// Constructs a variant holding the value-initialized value of the first alternative (index() is zero).
    //***************************************************************************
  #include "diagnostic_uninitialized_push.h"
    template <bool Trivial = Is_Trivially_Destructible_Suite, etl::enable_if_t<!Trivial, int> = 0>
    ETL_CONSTEXPR14 variant() noexcept(etl::is_nothrow_default_constructible<type_from_index<0U> >::value)
    {
      using type = type_from_index<0U>;

      default_construct_in_place<type>(data);
      type_id = 0U;
    }

    template <bool Trivial = Is_Trivially_Destructible_Suite, etl::enable_if_t<Trivial, int> = 0>
    constexpr variant() noexcept(etl::is_nothrow_default_constructible<type_from_index<0U> >::value)
      : base_type(etl::in_place_index_t<0>{}, type_from_index<0U>{}, 0U)
    {
    }
  #include "diagnostic_pop.h"

    //***************************************************************************
    /// Construct from a value.
    //***************************************************************************
  #include "diagnostic_uninitialized_push.h"
    template <typename T, bool Trivial_ = Is_Trivially_Destructible_Suite,
              etl::enable_if_t<!etl::is_same<etl::remove_cvref_t<T>, variant>::value && !Trivial_, int> = 0>
    ETL_CONSTEXPR14 variant(T&& value)
      : base_type(index_of_type<T>::value)
    {
      static_assert(etl::is_one_of<etl::remove_cvref_t<T>, TTypes...>::value, "Unsupported type");

      construct_in_place<etl::remove_cvref_t<T> >(data, etl::forward<T>(value));
    }

    template <typename T, bool Trivial_ = Is_Trivially_Destructible_Suite,
              etl::enable_if_t<!etl::is_same<etl::remove_cvref_t<T>, variant>::value && Trivial_, int> = 0>
    constexpr variant(T&& value)
      : base_type(etl::in_place_index_t<index_of_type<T>::value>{}, etl::forward<T>(value), index_of_type<T>::value)
    {
      static_assert(etl::is_one_of<etl::remove_cvref_t<T>, TTypes...>::value, "Unsupported type");
    }
  #include "diagnostic_pop.h"

    //***************************************************************************
    /// Construct from arguments.
    //***************************************************************************
  #include "diagnostic_uninitialized_push.h"
    template <typename T, typename... TArgs, bool Trivial_ = Is_Trivially_Destructible_Suite, etl::enable_if_t<!Trivial_, int> = 0>
    ETL_CONSTEXPR14 explicit variant(etl::in_place_type_t<T>, TArgs&&... args)
      : base_type(index_of_type<T>::value)
    {
      static_assert(etl::is_one_of<etl::remove_cvref_t<T>, TTypes...>::value, "Unsupported type");

      construct_in_place_args<etl::remove_cvref_t<T> >(data, etl::forward<TArgs>(args)...);
    }

    template <typename T, typename... TArgs, bool Trivial_ = Is_Trivially_Destructible_Suite, etl::enable_if_t<Trivial_, int> = 0>
    constexpr explicit variant(etl::in_place_type_t<T>, TArgs&&... args)
      : base_type(etl::in_place_index_t<index_of_type<T>::value>{}, etl::remove_cvref_t<T>(etl::forward<TArgs>(args)...), index_of_type<T>::value)
    {
      static_assert(etl::is_one_of<etl::remove_cvref_t<T>, TTypes...>::value, "Unsupported type");
    }
  #include "diagnostic_pop.h"

    //***************************************************************************
    /// Construct from arguments.
    //***************************************************************************
  #include "diagnostic_uninitialized_push.h"
    template <size_t Index, typename... TArgs, bool Trivial_ = Is_Trivially_Destructible_Suite, etl::enable_if_t<!Trivial_, int> = 0>
    ETL_CONSTEXPR14 explicit variant(etl::in_place_index_t<Index>, TArgs&&... args)
      : base_type(Index)
    {
      using type = type_from_index<Index>;
      static_assert(etl::is_one_of<type, TTypes...>::value, "Unsupported type");

      construct_in_place_args<type>(data, etl::forward<TArgs>(args)...);
    }

    template <size_t Index, typename... TArgs, bool Trivial_ = Is_Trivially_Destructible_Suite, etl::enable_if_t<Trivial_, int> = 0>
    constexpr explicit variant(etl::in_place_index_t<Index>, TArgs&&... args)
      : base_type(etl::in_place_index_t<Index>{}, type_from_index<Index>(etl::forward<TArgs>(args)...), Index)
    {
      using type = type_from_index<Index>;
      static_assert(etl::is_one_of<type, TTypes...>::value, "Unsupported type");
    }
  #include "diagnostic_pop.h"

  #if ETL_HAS_INITIALIZER_LIST
      //***************************************************************************
      /// Construct from type, initializer_list and arguments.
      //***************************************************************************
    #include "diagnostic_uninitialized_push.h"
    template <typename T, typename U, typename... TArgs >
    ETL_CONSTEXPR14 explicit variant(etl::in_place_type_t<T>, std::initializer_list<U> init, TArgs&&... args)
      : base_type(index_of_type<T>::value)
    {
      static_assert(etl::is_one_of<etl::remove_cvref_t<T>, TTypes...>::value, "Unsupported type");

      construct_in_place_args<etl::remove_cvref_t<T> >(data, init, etl::forward<TArgs>(args)...);
    }
    #include "diagnostic_pop.h"

      //***************************************************************************
      /// Construct from index, initializer_list and arguments.
      //***************************************************************************
    #include "diagnostic_uninitialized_push.h"
    template <size_t Index, typename U, typename... TArgs >
    ETL_CONSTEXPR14 explicit variant(etl::in_place_index_t<Index>, std::initializer_list<U> init, TArgs&&... args)
      : base_type(Index)
    {
      using type = type_from_index<Index>;
      static_assert(etl::is_one_of<type, TTypes...>::value, "Unsupported type");

      construct_in_place_args<type>(data, init, etl::forward<TArgs>(args)...);
    }
    #include "diagnostic_pop.h"
  #endif

    //***************************************************************************
    /// Copy constructor.
    ///\param other The other variant object to copy.
    //***************************************************************************
  #include "diagnostic_uninitialized_push.h"
    ETL_CONSTEXPR14 variant(const variant& other) noexcept(etl::conjunction<etl::is_nothrow_copy_constructible<TTypes>...>::value)
      : base_type(other.type_id)
    {
      if (other.index() != variant_npos)
      {
        do_copy_from(other, etl::integral_constant<bool, Is_Trivially_Destructible_Suite>{});
      }
    }
  #include "diagnostic_pop.h"

    //***************************************************************************
    /// Move constructor.
    ///\param other The other variant object to copy.
    //***************************************************************************
  #include "diagnostic_uninitialized_push.h"
    ETL_CONSTEXPR14 variant(variant&& other) noexcept(etl::conjunction<etl::is_nothrow_move_constructible<TTypes>...>::value)
      : base_type(other.type_id)
    {
      if (other.index() != variant_npos)
      {
        do_move_from(other, etl::integral_constant<bool, Is_Trivially_Destructible_Suite>{});
      }
    }
  #include "diagnostic_pop.h"

    //***************************************************************************
    /// Destructor.
    /// Handled by variant_base (trivial for trivially destructible types,
    /// non-trivial otherwise).
    //***************************************************************************
    // ~variant() is provided by base_type

    //***************************************************************************
    /// Emplace by type with variadic constructor parameters.
    //***************************************************************************
    template <typename T, typename... TArgs>
    T& emplace(TArgs&&... args) ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<etl::remove_cvref_t<T>, TArgs...>::value))
    {
      static_assert(etl::is_one_of<T, TTypes...>::value, "Unsupported type");

      using type = etl::remove_cvref_t<T>;

      do_destroy();
      do_emplace<type>(etl::forward<TArgs>(args)...);

      type_id = index_of_type<T>::value;

      return get_value<index_of_type<T>::value>();
    }

  #if ETL_HAS_INITIALIZER_LIST
    //***************************************************************************
    /// Emplace by type with variadic constructor parameters.
    //***************************************************************************
    template <typename T, typename U, typename... TArgs>
    T& emplace(std::initializer_list<U> il, TArgs&&... args)
      ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<etl::remove_cvref_t<T>, std::initializer_list<U>, TArgs...>::value))
    {
      static_assert(etl::is_one_of<T, TTypes...>::value, "Unsupported type");

      using type = etl::remove_cvref_t<T>;

      do_destroy();
      do_emplace<type>(il, etl::forward<TArgs>(args)...);

      type_id = index_of_type<T>::value;

      return get_value<index_of_type<T>::value>();
    }
  #endif

    //***************************************************************************
    /// Emplace by index with variadic constructor parameters.
    //***************************************************************************
    template <size_t Index, typename... TArgs>
    typename etl::variant_alternative_t<Index, variant<TTypes...> >& emplace(TArgs&&... args)
      ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<type_from_index<Index>, TArgs...>::value))
    {
      static_assert(Index < sizeof...(TTypes), "Index out of range");

      using type = type_from_index<Index>;

      do_destroy();
      do_emplace<type>(etl::forward<TArgs>(args)...);

      type_id = Index;

      return get_value<Index>();
    }

  #if ETL_HAS_INITIALIZER_LIST
    //***************************************************************************
    /// Emplace by index with variadic constructor parameters.
    //***************************************************************************
    template <size_t Index, typename U, typename... TArgs>
    typename etl::variant_alternative_t<Index, variant<TTypes...> >& emplace(std::initializer_list<U> il, TArgs&&... args)
      ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<type_from_index<Index>, std::initializer_list<U>, TArgs...>::value))
    {
      static_assert(Index < sizeof...(TTypes), "Index out of range");

      using type = type_from_index<Index>;

      do_destroy();
      do_emplace<type>(il, etl::forward<TArgs>(args)...);

      type_id = Index;

      return get_value<Index>();
    }
  #endif

    //***************************************************************************
    /// Move assignment operator for type.
    ///\param value The value to assign.
    //***************************************************************************
    template <typename T, etl::enable_if_t<!etl::is_same<etl::remove_cvref_t<T>, variant>::value, int> = 0>
    variant& operator=(T&& value)
    {
      using type = etl::remove_cvref_t<T>;

      static_assert(etl::is_one_of<type, TTypes...>::value, "Unsupported type");

      do_destroy();
      do_construct<type>(etl::forward<T>(value));

      type_id = index_of_type<type>::value;

      return *this;
    }

    //***************************************************************************
    /// Assignment operator for variant type.
    ///\param other The variant to assign.
    //***************************************************************************
    variant& operator=(const variant& other) ETL_NOEXCEPT_IF((etl::conjunction<etl::is_nothrow_copy_constructible<TTypes>...>::value))
    {
      if (this != &other)
      {
        if (other.index() == variant_npos)
        {
          type_id = variant_npos;
        }
        else
        {
          do_destroy();

          do_copy_from(other, etl::integral_constant<bool, Is_Trivially_Destructible_Suite>{});

          type_id = other.type_id;
        }
      }

      return *this;
    }

    //***************************************************************************
    /// Assignment operator for variant type.
    ///\param other The variant to assign.
    //***************************************************************************
    variant& operator=(variant&& other) ETL_NOEXCEPT_IF((etl::conjunction<etl::is_nothrow_move_constructible<TTypes>...>::value))
    {
      if (this != &other)
      {
        if (other.index() == variant_npos)
        {
          type_id = variant_npos;
        }
        else
        {
          do_destroy();

          do_move_from(other, etl::integral_constant<bool, Is_Trivially_Destructible_Suite>{});

          type_id = other.type_id;
        }
      }

      return *this;
    }

    //***************************************************************************
    /// Checks whether the variant doesn't contain a valid value.
    ///\return <b>true</b> if the value is invalid, otherwise <b>false</b>.
    //***************************************************************************
    constexpr bool valueless_by_exception() const ETL_NOEXCEPT
    {
      return type_id == variant_npos;
    }

    //***************************************************************************
    /// Gets the index of the type currently stored or variant_npos
    //***************************************************************************
    constexpr size_t index() const ETL_NOEXCEPT
    {
      return type_id;
    }

    //***************************************************************************
    /// Checks to see if the type T is one of the variant's supported types.
    /// For compatibility with legacy variant API.
    ///\return <b>true</b> if it is, otherwise <b>false</b>.
    //***************************************************************************
    template <typename T>
    static constexpr bool is_supported_type()
    {
      return etl::is_one_of<etl::remove_cvref_t<T>, TTypes...>::value;
    }

    //***************************************************************************
    /// Checks to see if the type currently stored is the same as that specified in the template parameter.
    /// For compatibility with legacy variant API.
    ///\return <b>true</b> if it is the specified type, otherwise <b>false</b>.
    //***************************************************************************
    template <typename T, etl::enable_if_t<is_supported_type<T>(), int> = 0>
    constexpr bool is_type() const ETL_NOEXCEPT
    {
      return (type_id == index_of_type<T>::value);
    }

    //***************************************************************************
    template <typename T, etl::enable_if_t<!is_supported_type<T>(), int> = 0>
    constexpr bool is_type() const ETL_NOEXCEPT
    {
      return false;
    }

    //***************************************************************************
    /// Checks if the other variant holds the same type as the current stored type.
    /// For variants with the same type declarations.
    /// For compatibility with legacy variant API.
    ///\return <b>true</b> if the types are the same, otherwise <b>false</b>.
    //***************************************************************************
    constexpr bool is_same_type(const variant& other) const
    {
      return type_id == other.type_id;
    }

    //***************************************************************************
    /// Swaps this variant with another.
    //***************************************************************************
    void swap(variant& rhs) ETL_NOEXCEPT
    {
      variant temp(etl::move(*this));
      *this = etl::move(rhs);
      rhs   = etl::move(temp);
    }

    //***************************************************************************
    /// Accept an etl::visitor.
    //***************************************************************************
    template <typename TVisitor>
    etl::enable_if_t<etl::is_visitor<TVisitor>::value, void> accept(TVisitor& v)
    {
  #if ETL_USING_CPP17 && !defined(ETL_VARIANT_FORCE_CPP11)
      do_visitor(v, etl::make_index_sequence<sizeof...(TTypes)>{});
  #else
      do_visitor<sizeof...(TTypes)>(v);
  #endif
    }

    //***************************************************************************
    /// Accept an etl::visitor.
    //***************************************************************************
    template <typename TVisitor>
    etl::enable_if_t<etl::is_visitor<TVisitor>::value, void> accept(TVisitor& v) const
    {
  #if ETL_USING_CPP17 && !defined(ETL_VARIANT_FORCE_CPP11)
      do_visitor(v, etl::make_index_sequence<sizeof...(TTypes)>{});
  #else
      do_visitor<sizeof...(TTypes)>(v);
  #endif
    }

    //***************************************************************************
    /// Accept a generic functor.
    //***************************************************************************
    template <typename TVisitor>
    etl::enable_if_t<!etl::is_visitor<TVisitor>::value, void> accept(TVisitor& v)
    {
  #if ETL_USING_CPP17 && !defined(ETL_VARIANT_FORCE_CPP11)
      do_operator(v, etl::make_index_sequence<sizeof...(TTypes)>{});
  #else
      do_operator<sizeof...(TTypes)>(v);
  #endif
    }

    //***************************************************************************
    /// Accept a generic functor.
    //***************************************************************************
    template <typename TVisitor>
    etl::enable_if_t<!etl::is_visitor<TVisitor>::value, void> accept(TVisitor& v) const
    {
  #if ETL_USING_CPP17 && !defined(ETL_VARIANT_FORCE_CPP11)
      do_operator(v, etl::make_index_sequence<sizeof...(TTypes)>{});
  #else
      do_operator<sizeof...(TTypes)>(v);
  #endif
    }

    //***************************************************************************
    /// Accept an etl::visitor.
    /// Deprecated.
    //***************************************************************************
    template <typename TVisitor>
  #if !defined(ETL_IN_UNIT_TEST)
    ETL_DEPRECATED_REASON("Replace with accept()")
  #endif
    void
      accept_visitor(TVisitor& v)
    {
  #if ETL_USING_CPP17 && !defined(ETL_VARIANT_FORCE_CPP11)
      do_visitor(v, etl::make_index_sequence<sizeof...(TTypes)>{});
  #else
      do_visitor<sizeof...(TTypes)>(v);
  #endif
    }

    //***************************************************************************
    /// Accept an etl::visitor.
    /// Deprecated.
    //***************************************************************************
    template <typename TVisitor>
  #if !defined(ETL_IN_UNIT_TEST)
    ETL_DEPRECATED_REASON("Replace with accept()")
  #endif
    void
      accept_visitor(TVisitor& v) const
    {
  #if ETL_USING_CPP17 && !defined(ETL_VARIANT_FORCE_CPP11)
      do_visitor(v, etl::make_index_sequence<sizeof...(TTypes)>{});
  #else
      do_visitor<sizeof...(TTypes)>(v);
  #endif
    }

    //***************************************************************************
    /// Accept a generic functor.
    /// Deprecated.
    //***************************************************************************
    template <typename TVisitor>
  #if !defined(ETL_IN_UNIT_TEST)
    ETL_DEPRECATED_REASON("Replace with accept()")
  #endif
    void
      accept_functor(TVisitor& v)
    {
  #if ETL_USING_CPP17 && !defined(ETL_VARIANT_FORCE_CPP11)
      do_operator(v, etl::make_index_sequence<sizeof...(TTypes)>{});
  #else
      do_operator<sizeof...(TTypes)>(v);
  #endif
    }

    //***************************************************************************
    /// Accept a generic functor.
    /// Deprecated.
    //***************************************************************************
    template <typename TVisitor>
  #if !defined(ETL_IN_UNIT_TEST)
    ETL_DEPRECATED_REASON("Replace with accept()")
  #endif
    void
      accept_functor(TVisitor& v) const
    {
  #if ETL_USING_CPP17 && !defined(ETL_VARIANT_FORCE_CPP11)
      do_operator(v, etl::make_index_sequence<sizeof...(TTypes)>{});
  #else
      do_operator<sizeof...(TTypes)>(v);
  #endif
    }

  private:

    //***************************************************************************
    /// Construct the type in-place via perfect forwarding.
    //***************************************************************************
    template <typename T, typename U>
    static void construct_in_place(char* pstorage, U&& value)
    {
      using type = etl::remove_cvref_t<T>;

      ::new (pstorage) type(etl::forward<U>(value));
    }

    //***************************************************************************
    /// Construct the type in-place. Variadic args.
    //***************************************************************************
    template <typename T, typename... TArgs>
    static void construct_in_place_args(char* pstorage, TArgs&&... args)
    {
      using type = etl::remove_cvref_t<T>;

      ::new (pstorage) type(etl::forward<TArgs>(args)...);
    }

    //***************************************************************************
    /// Default construct the type in-place.
    //***************************************************************************
    template <typename T>
    static void default_construct_in_place(char* pstorage) ETL_NOEXCEPT_IF((etl::is_nothrow_default_constructible<etl::remove_cvref_t<T> >::value))
    {
      using type = etl::remove_cvref_t<T>;

      ::new (pstorage) type();
    }

    //***************************************************************************
    /// Destroy the currently held value (only for non-trivially destructible).
    //***************************************************************************
    void do_destroy()
    {
      do_destroy_impl(etl::integral_constant<bool, Is_Trivially_Destructible_Suite>{});
    }

    void do_destroy_impl(etl::integral_constant<bool, true>)
    {
      // Trivially destructible: no-op.
    }

    void do_destroy_impl(etl::integral_constant<bool, false>)
    {
      private_variant::variant_operations<0, TTypes...>::destroy(data, type_id);
    }

    //***************************************************************************
    /// Emplace-construct the alternative in place from forwarded args.
    /// No temporary, so no move/copy is required (works for move-hostile types).
    //***************************************************************************
    template <typename T, typename... TArgs>
    void do_emplace(TArgs&&... args)
    {
      do_emplace_impl<T>(etl::integral_constant<bool, Is_Trivially_Destructible_Suite>{}, etl::forward<TArgs>(args)...);
    }

    // Trivially destructible suite: storage is a variadic_union. The old member is
    // trivially destructible, so placement-new of the new member is well-defined at
    // runtime (emplace is not constexpr, so placement-new is permitted here).
    template <typename T, typename... TArgs>
    void do_emplace_impl(etl::integral_constant<bool, true>, TArgs&&... args)
    {
      ::new (static_cast<void*>(etl::addressof(private_variant::variadic_union_get<index_of_type<T>::value>(data)))) T(etl::forward<TArgs>(args)...);
    }

    // Non-trivially destructible suite: storage is an uninitialized_buffer.
    template <typename T, typename... TArgs>
    void do_emplace_impl(etl::integral_constant<bool, false>, TArgs&&... args)
    {
      ::new (static_cast<char*>(data)) T(etl::forward<TArgs>(args)...);
    }

    //***************************************************************************
    /// Construct a value in the union or buffer storage.
    //***************************************************************************
    template <typename T, typename U>
    void do_construct(U&& value)
    {
      do_construct_impl<T>(etl::forward<U>(value), etl::integral_constant<bool, Is_Trivially_Destructible_Suite>{});
    }

    template <typename T, typename U>
    void do_construct_impl(U&& value, etl::integral_constant<bool, true>)
    {
      // Trivially destructible: assign directly into the variadic_union.
      private_variant::variadic_union_get<index_of_type<T>::value>(data) = etl::forward<U>(value);
    }

    template <typename T, typename U>
    void do_construct_impl(U&& value, etl::integral_constant<bool, false>)
    {
      // Non-trivially destructible: use placement new.
      ::new (static_cast<char*>(data)) T(etl::forward<U>(value));
    }

    //***************************************************************************
    /// Copy from another variant.
    //***************************************************************************
    void do_copy_from(const variant& other, etl::integral_constant<bool, true>)
    {
      // Trivially destructible: copy via memcpy to avoid issues with non-trivial copy assignment in union.
      memcpy(static_cast<void*>(&data), static_cast<const void*>(&other.data), sizeof(data));
    }

    void do_copy_from(const variant& other, etl::integral_constant<bool, false>)
    {
      // Non-trivially destructible: use switch-based dispatch.
      private_variant::variant_operations<0, TTypes...>::copy(data, other.data, other.type_id);
    }

    //***************************************************************************
    /// Move from another variant.
    //***************************************************************************
    void do_move_from(variant& other, etl::integral_constant<bool, true>)
    {
      // Trivially destructible: copy via memcpy (trivial move == copy).
      memcpy(static_cast<void*>(&data), static_cast<const void*>(&other.data), sizeof(data));
    }

    void do_move_from(variant& other, etl::integral_constant<bool, false>)
    {
      // Non-trivially destructible: use switch-based dispatch.
      private_variant::variant_operations<0, TTypes...>::move(data, other.data, other.type_id);
    }

  #if ETL_USING_CPP17 && !defined(ETL_VARIANT_FORCE_CPP11)
    //***************************************************************************
    /// Call the relevant visitor by attempting each one.
    //***************************************************************************
    template <typename TVisitor, size_t... I>
    void do_visitor(TVisitor& visitor, etl::index_sequence<I...>)
    {
      (attempt_visitor<I>(visitor) || ...);
    }

    //***************************************************************************
    /// Call the relevant visitor by attempting each one.
    //***************************************************************************
    template <typename TVisitor, size_t... I>
    void do_visitor(TVisitor& visitor, etl::index_sequence<I...>) const
    {
      (attempt_visitor<I>(visitor) || ...);
    }
  #else
    //***************************************************************************
    /// Call the relevant visitor.
    //***************************************************************************
    template <size_t NTypes, typename TVisitor>
    void do_visitor(TVisitor& visitor)
    {
      etl::private_variant::select_do_visitor<NTypes>::do_visitor(*this, visitor);
    }

    //***************************************************************************
    /// Call the relevant visitor.
    //***************************************************************************
    template <size_t NTypes, typename TVisitor>
    void do_visitor(TVisitor& visitor) const
    {
      etl::private_variant::select_do_visitor<NTypes>::do_visitor(*this, visitor);
    }
  #endif

    //***************************************************************************
    /// Attempt to call a visitor.
    //***************************************************************************
    template <size_t Index, typename TVisitor>
    bool attempt_visitor(TVisitor& visitor)
    {
      if (Index == index())
      {
        // Workaround for MSVC (2023/05/13)
        // It doesn't compile 'visitor.visit(etl::get<Index>(*this))' correctly for C++17 & C++20.
        // Changed all of the instances for consistency.
        auto& v = etl::get<Index>(*this);
        visitor.visit(v);
        return true;
      }
      else
      {
        return false;
      }
    }

    //***************************************************************************
    /// Attempt to call a visitor.
    //***************************************************************************
    template <size_t Index, typename TVisitor>
    bool attempt_visitor(TVisitor& visitor) const
    {
      if (Index == index())
      {
        // Workaround for MSVC (2023/05/13)
        // It doesn't compile 'visitor.visit(etl::get<Index>(*this))' correctly for C++17 & C++20.
        // Changed all of the instances for consistency.
        auto& v = etl::get<Index>(*this);
        visitor.visit(v);
        return true;
      }
      else
      {
        return false;
      }
    }

  #if ETL_USING_CPP17 && !defined(ETL_VARIANT_FORCE_CPP11)
    //***************************************************************************
    /// Call the relevant visitor by attempting each one.
    //***************************************************************************
    template <typename TVisitor, size_t... I>
    void do_operator(TVisitor& visitor, etl::index_sequence<I...>)
    {
      (attempt_operator<I>(visitor) || ...);
    }

    //***************************************************************************
    /// Call the relevant visitor by attempting each one.
    //***************************************************************************
    template <typename TVisitor, size_t... I>
    void do_operator(TVisitor& visitor, etl::index_sequence<I...>) const
    {
      (attempt_operator<I>(visitor) || ...);
    }
  #else
    //***************************************************************************
    /// Call the relevant operator.
    //***************************************************************************
    template <size_t NTypes, typename TVisitor>
    void do_operator(TVisitor& visitor)
    {
    #if defined(ETL_VARIANT_CPP11_MAX_8_TYPES)
      ETL_STATIC_ASSERT(sizeof...(TTypes) <= 8U, "ETL_VARIANT_CPP11_MAX_8_TYPES - Only a maximum of 8 types are allowed in this variant");
    #endif

    #if defined(ETL_VARIANT_CPP11_MAX_16_TYPES)
      ETL_STATIC_ASSERT(sizeof...(TTypes) <= 16U, "ETL_VARIANT_CPP11_MAX_16_TYPES - Only a maximum of 16 types are allowed in this variant");
    #endif

    #if defined(ETL_VARIANT_CPP11_MAX_24_TYPES)
      ETL_STATIC_ASSERT(sizeof...(TTypes) <= 24U, "ETL_VARIANT_CPP11_MAX_24_TYPES - Only a maximum of 24 types are allowed in this variant");
    #endif

      ETL_STATIC_ASSERT(sizeof...(TTypes) <= 32U, "A maximum of 32 types are allowed in this variant");

      etl::private_variant::select_do_operator<NTypes>::do_operator(*this, visitor);
    }

    //***************************************************************************
    /// Call the relevant operator.
    //***************************************************************************
    template <size_t NTypes, typename TVisitor>
    void do_operator(TVisitor& visitor) const
    {
    #if defined(ETL_VARIANT_CPP11_MAX_8_TYPES)
      ETL_STATIC_ASSERT(sizeof...(TTypes) <= 8U, "ETL_VARIANT_CPP11_MAX_8_TYPES - Only a maximum of 8 types are allowed in this variant");
    #endif

    #if defined(ETL_VARIANT_CPP11_MAX_16_TYPES)
      ETL_STATIC_ASSERT(sizeof...(TTypes) <= 16U, "ETL_VARIANT_CPP11_MAX_16_TYPES - Only a maximum of 16 types are allowed in this variant");
    #endif

    #if defined(ETL_VARIANT_CPP11_MAX_24_TYPES)
      ETL_STATIC_ASSERT(sizeof...(TTypes) <= 24U, "ETL_VARIANT_CPP11_MAX_24_TYPES - Only a maximum of 24 types are allowed in this variant");
    #endif

      ETL_STATIC_ASSERT(sizeof...(TTypes) <= 32U, "A maximum of 32 types are allowed in this variant");

      etl::private_variant::select_do_operator<NTypes>::do_operator(*this, visitor);
    }
  #endif

    //***************************************************************************
    /// Attempt to call a visitor.
    //***************************************************************************
    template <size_t Index, typename TVisitor>
    bool attempt_operator(TVisitor& visitor)
    {
      if (Index == index())
      {
        auto& v = etl::get<Index>(*this);
        visitor(v);
        return true;
      }
      else
      {
        return false;
      }
    }

    //***************************************************************************
    /// Attempt to call a visitor.
    //***************************************************************************
    template <size_t Index, typename TVisitor>
    bool attempt_operator(TVisitor& visitor) const
    {
      if (Index == index())
      {
        auto& v = etl::get<Index>(*this);
        visitor(v);
        return true;
      }
      else
      {
        return false;
      }
    }

    //***************************************************************************
    /// Get a reference to the stored value by index.
    /// For trivially destructible types, accesses the variadic_union directly.
    /// For non-trivially destructible types, uses pointer cast on uninitialized_buffer.
    //***************************************************************************
    template <size_t Index>
    ETL_CONSTEXPR14 type_from_index<Index>& get_value() ETL_NOEXCEPT
    {
      return get_value_impl<Index>(etl::integral_constant<bool, Is_Trivially_Destructible_Suite>{});
    }

    template <size_t Index>
    constexpr const type_from_index<Index>& get_value() const ETL_NOEXCEPT
    {
      return get_value_impl<Index>(etl::integral_constant<bool, Is_Trivially_Destructible_Suite>{});
    }

    // Trivially destructible: use variadic_union accessor
    template <size_t Index>
    ETL_CONSTEXPR14 type_from_index<Index>& get_value_impl(etl::integral_constant<bool, true>) ETL_NOEXCEPT
    {
      return private_variant::variadic_union_get<Index>(data);
    }

    template <size_t Index>
    constexpr const type_from_index<Index>& get_value_impl(etl::integral_constant<bool, true>) const ETL_NOEXCEPT
    {
      return private_variant::variadic_union_get<Index>(data);
    }

    // Non-trivially destructible: use pointer cast on uninitialized_buffer
    template <size_t Index>
    ETL_CONSTEXPR14 type_from_index<Index>& get_value_impl(etl::integral_constant<bool, false>) ETL_NOEXCEPT
    {
      return *static_cast<type_from_index<Index>*>(data);
    }

    template <size_t Index>
    ETL_CONSTEXPR14 const type_from_index<Index>& get_value_impl(etl::integral_constant<bool, false>) const ETL_NOEXCEPT
    {
      return *static_cast<const type_from_index<Index>*>(data);
    }

    //***************************************************************************
    /// Get a pointer to the stored value by type.
    //***************************************************************************
    template <typename T>
    ETL_CONSTEXPR14 T* get_value_ptr() ETL_NOEXCEPT
    {
      return get_value_ptr_impl<T>(etl::integral_constant<bool, Is_Trivially_Destructible_Suite>{});
    }

    template <typename T>
    constexpr const T* get_value_ptr() const ETL_NOEXCEPT
    {
      return get_value_ptr_impl<T>(etl::integral_constant<bool, Is_Trivially_Destructible_Suite>{});
    }

    // Trivially destructible: use variadic_union accessor
    template <typename T>
    ETL_CONSTEXPR14 T* get_value_ptr_impl(etl::integral_constant<bool, true>) ETL_NOEXCEPT
    {
      return &private_variant::variadic_union_get<index_of_type<T>::value>(data);
    }

    template <typename T>
    constexpr const T* get_value_ptr_impl(etl::integral_constant<bool, true>) const ETL_NOEXCEPT
    {
      return &private_variant::variadic_union_get<index_of_type<T>::value>(data);
    }

    // Non-trivially destructible: use pointer cast on uninitialized_buffer
    template <typename T>
    ETL_CONSTEXPR14 T* get_value_ptr_impl(etl::integral_constant<bool, false>) ETL_NOEXCEPT
    {
      return static_cast<T*>(data);
    }

    template <typename T>
    ETL_CONSTEXPR14 const T* get_value_ptr_impl(etl::integral_constant<bool, false>) const ETL_NOEXCEPT
    {
      return static_cast<const T*>(data);
    }

    // data and type_id are inherited from base_type.
    // operation is inherited only for non-trivially destructible variants.
  };

  namespace private_variant
  {
    //***************************************************************************
    /// is_same_type_in.
    /// Checks if specified type T is at specified index in given type list
    //***************************************************************************
    template <typename T, typename T0, typename T1, typename... Ts>
    typename etl::enable_if_t<etl::is_same<T, T0>::value, bool> ETL_CONSTEXPR14 is_same_type_in(size_t index) ETL_NOEXCEPT;

    template <typename T, typename T0>
    typename etl::enable_if_t<etl::is_same<T, T0>::value, bool> ETL_CONSTEXPR14 is_same_type_in(size_t index) ETL_NOEXCEPT;

    template <typename T, typename T0, typename T1, typename... Ts>
    typename etl::enable_if_t<!etl::is_same<T, T0>::value, bool> ETL_CONSTEXPR14 is_same_type_in(size_t index) ETL_NOEXCEPT;

    template <typename T, typename T0>
    typename etl::enable_if_t<!etl::is_same<T, T0>::value, bool> ETL_CONSTEXPR14 is_same_type_in(size_t index) ETL_NOEXCEPT;

    template <typename T, typename T0, typename T1, typename... Ts>
    typename etl::enable_if_t<etl::is_same<T, T0>::value, bool> ETL_CONSTEXPR14 is_same_type_in(size_t index) ETL_NOEXCEPT
    {
      if (index == 0)
      {
        return true;
      }
      else
      {
        return is_same_type_in<T, T1, Ts...>(index - 1);
      }
    }

    template <typename T, typename T0>
    typename etl::enable_if_t<etl::is_same<T, T0>::value, bool> ETL_CONSTEXPR14 is_same_type_in(size_t index) ETL_NOEXCEPT
    {
      return index == 0;
    }

    template <typename T, typename T0, typename T1, typename... Ts>
    typename etl::enable_if_t<!etl::is_same<T, T0>::value, bool> ETL_CONSTEXPR14 is_same_type_in(size_t index) ETL_NOEXCEPT
    {
      if (index == 0)
      {
        return false;
      }
      else
      {
        return is_same_type_in<T, T1, Ts...>(index - 1);
      }
    }

    template <typename T, typename T0>
    typename etl::enable_if_t<!etl::is_same<T, T0>::value, bool> ETL_CONSTEXPR14 is_same_type_in(size_t) ETL_NOEXCEPT
    {
      return false;
    }
  } // namespace private_variant

  //***************************************************************************
  /// Checks if the variant v holds the alternative T.
  //***************************************************************************
  template <typename T, typename... TTypes>
  ETL_CONSTEXPR14 bool holds_alternative(const etl::variant<TTypes...>& v) ETL_NOEXCEPT
  {
    return private_variant::is_same_type_in<T, TTypes...>(v.index());
  }

  //***************************************************************************
  /// Checks if the variant v holds the alternative Index.
  //***************************************************************************
  template <size_t Index, typename... TTypes>
  ETL_CONSTEXPR14 bool holds_alternative(const etl::variant<TTypes...>& v) ETL_NOEXCEPT
  {
    return (Index == v.index());
  }

  //***************************************************************************
  /// Checks if the variant v holds the alternative Index. (Runtime)
  //***************************************************************************
  template <typename... TTypes>
  ETL_CONSTEXPR14 bool holds_alternative(size_t index, const etl::variant<TTypes...>& v) ETL_NOEXCEPT
  {
    return (index == v.index());
  }

  //***************************************************************************
  /// get
  //***************************************************************************
  template <size_t Index, typename... TTypes>
  ETL_CONSTEXPR14 etl::variant_alternative_t<Index, etl::variant<TTypes...> >& get(etl::variant<TTypes...>& v)
  {
  #if ETL_USING_CPP17 && !defined(ETL_VARIANT_FORCE_CPP11)
    static_assert(Index < sizeof...(TTypes), "Index out of range");
  #endif

    ETL_ASSERT(Index == v.index(), ETL_ERROR(etl::variant_incorrect_type_exception));

    return v.template get_value<Index>();
  }

  //***********************************
  template <size_t Index, typename... TTypes>
  ETL_CONSTEXPR14 etl::variant_alternative_t<Index, etl::variant<TTypes...> >&& get(etl::variant<TTypes...>&& v)
  {
  #if ETL_USING_CPP17 && !defined(ETL_VARIANT_FORCE_CPP11)
    static_assert(Index < sizeof...(TTypes), "Index out of range");
  #endif

    ETL_ASSERT(Index == v.index(), ETL_ERROR(etl::variant_incorrect_type_exception));

    return etl::move(v.template get_value<Index>());
  }

  //***********************************
  template <size_t Index, typename... TTypes>
  ETL_CONSTEXPR14 const etl::variant_alternative_t<Index, const etl::variant<TTypes...> >& get(const etl::variant<TTypes...>& v)
  {
  #if ETL_USING_CPP17 && !defined(ETL_VARIANT_FORCE_CPP11)
    static_assert(Index < sizeof...(TTypes), "Index out of range");
  #endif

    ETL_ASSERT(Index == v.index(), ETL_ERROR(etl::variant_incorrect_type_exception));

    return v.template get_value<Index>();
  }

  //***********************************
  template <size_t Index, typename... TTypes>
  ETL_CONSTEXPR14 const etl::variant_alternative_t<Index, const etl::variant<TTypes...> >&& get(const etl::variant<TTypes...>&& v)
  {
  #if ETL_USING_CPP17 & !defined(ETL_VARIANT_FORCE_CPP11)
    static_assert(Index < sizeof...(TTypes), "Index out of range");
  #endif

    ETL_ASSERT(Index == v.index(), ETL_ERROR(etl::variant_incorrect_type_exception));

    return etl::move(v.template get_value<Index>());
  }

  //***********************************
  template <typename T, typename... TTypes>
  ETL_CONSTEXPR14 T& get(etl::variant<TTypes...>& v)
  {
    ETL_ASSERT((private_variant::is_same_type_in<T, TTypes...>(v.index())), ETL_ERROR(etl::variant_incorrect_type_exception));

    return *v.template get_value_ptr<T>();
  }

  //***********************************
  template <typename T, typename... TTypes>
  ETL_CONSTEXPR14 T&& get(etl::variant<TTypes...>&& v)
  {
    ETL_ASSERT((private_variant::is_same_type_in<T, TTypes...>(v.index())), ETL_ERROR(etl::variant_incorrect_type_exception));

    return etl::move(*v.template get_value_ptr<T>());
  }

  //***********************************
  template <typename T, typename... TTypes>
  ETL_CONSTEXPR14 const T& get(const etl::variant<TTypes...>& v)
  {
    ETL_ASSERT((private_variant::is_same_type_in<T, TTypes...>(v.index())), ETL_ERROR(etl::variant_incorrect_type_exception));

    return *v.template get_value_ptr<T>();
  }

  //***********************************
  template <typename T, typename... TTypes>
  ETL_CONSTEXPR14 const T&& get(const etl::variant<TTypes...>&& v)
  {
    ETL_ASSERT((private_variant::is_same_type_in<T, TTypes...>(v.index())), ETL_ERROR(etl::variant_incorrect_type_exception));

    return etl::move(*v.template get_value_ptr<T>());
  }

  //***************************************************************************
  /// get_if
  //***************************************************************************
  template < size_t Index, typename... TTypes >
  ETL_CONSTEXPR14 etl::add_pointer_t<etl::variant_alternative_t<Index, etl::variant<TTypes...> > > get_if(etl::variant<TTypes...>* pv) ETL_NOEXCEPT
  {
    if ((pv != nullptr) && (pv->index() == Index))
    {
      return &etl::get<Index>(*pv);
    }
    else
    {
      return nullptr;
    }
  }

  //***********************************
  template < size_t Index, typename... TTypes >
  ETL_CONSTEXPR14 etl::add_pointer_t<const etl::variant_alternative_t<Index, etl::variant<TTypes...> > > get_if(const etl::variant<TTypes...>* pv)
    ETL_NOEXCEPT
  {
    if ((pv != nullptr) && (pv->index() == Index))
    {
      return &etl::get<Index>(*pv);
    }
    else
    {
      return nullptr;
    }
  }

  //***********************************
  template < class T, typename... TTypes >
  ETL_CONSTEXPR14 etl::add_pointer_t<T> get_if(etl::variant<TTypes...>* pv) ETL_NOEXCEPT
  {
    if ((pv != nullptr) && (private_variant::is_same_type_in<T, TTypes...>(pv->index())))
    {
      return pv->template get_value_ptr<T>();
    }
    else
    {
      return nullptr;
    }
  }

  //***********************************
  template < typename T, typename... TTypes >
  ETL_CONSTEXPR14 etl::add_pointer_t<const T> get_if(const etl::variant<TTypes...>* pv) ETL_NOEXCEPT
  {
    if ((pv != nullptr) && (private_variant::is_same_type_in<T, TTypes...>(pv->index())))
    {
      return pv->template get_value_ptr<T>();
    }
    else
    {
      return nullptr;
    }
  }

  //***************************************************************************
  /// swap
  //***************************************************************************
  template <typename... TTypes>
  void swap(etl::variant<TTypes...>& lhs, etl::variant<TTypes...>& rhs)
  {
    lhs.swap(rhs);
  }

  //***************************************************************************
  /// variant_size
  //***************************************************************************
  template <typename T>
  struct variant_size;

  template <typename... TTypes>
  struct variant_size<etl::variant<TTypes...> > : etl::integral_constant<size_t, sizeof...(TTypes)>
  {
  };

  template <typename T>
  struct variant_size<const T> : etl::integral_constant<size_t, variant_size<T>::value>
  {
  };

  #if ETL_USING_CPP17
  template <typename... TTypes>
  inline constexpr size_t variant_size_v = variant_size<TTypes...>::value;
  #endif

  //***************************************************************************
  /// visit
  //***************************************************************************
  namespace private_variant
  {
    template <typename TRet, typename TCallable, typename TVariant, size_t tIndex, typename TNext, typename... TVariants>
    static ETL_CONSTEXPR14 TRet do_visit_single(TCallable&& f, TVariant&& v, TNext&&, TVariants&&... vs);

    //***************************************************************************
    /// Dummy-struct used to indicate that the return type should be auto-deduced
    /// from the callable object and the alternatives in the variants passed to
    /// a visit. Should never explicitly be used by an user.
    //***************************************************************************
    struct visit_auto_return
    {
    };

    //***************************************************************************
    /// Deduces return type of a call to TCallable with arguments Ts.
    /// A lite version of std::invoke_result.
    //***************************************************************************
    template <typename TCallable, typename... Ts>
    struct single_visit_result_type
    {
      using type = decltype(declval<TCallable>()(declval<Ts>()...));
    };

    template <typename TCallable, typename... Ts>
    using single_visit_result_type_t = typename single_visit_result_type<TCallable, Ts...>::type;

    //***************************************************************************
    /// Used to copy r/l value reference qualifier from a variant type to an
    /// element.
    //***************************************************************************
    template <typename TVar, typename T>
    using rlref_copy = conditional_t<is_reference<TVar>::value, T&, T&&>;

    //***************************************************************************
    /// Evaluates all permutations of calls to a callable object that can be done
    /// based upon the variants input. Need a `index_sequence<...>` as second
    /// argument that contains all possible indices of the first following variant.
    /// The first argument is essentially a `single_visit_result_type`-prototype
    /// in which every recursive instantiation of `visit_result_helper` appends
    /// more elements and give it a pass through `common_type_t`.
    //***************************************************************************
    template <template <typename...> class, typename...>
    struct visit_result_helper;

    template <template <typename...> class TToInject, size_t... tAltIndices, typename TCur>
    struct visit_result_helper<TToInject, index_sequence<tAltIndices...>, TCur>
    {
      template <size_t tIndex>
      using var_type = rlref_copy<TCur, variant_alternative_t<tIndex, remove_reference_t<TCur> > >;

      using type = common_type_t<TToInject<var_type<tAltIndices> >...>;
    };

    template <template <typename...> class TToInject, size_t... tAltIndices, typename TCur, typename TNext, typename... TVs>
    struct visit_result_helper<TToInject, index_sequence<tAltIndices...>, TCur, TNext, TVs...>
    {
      template <size_t tIndex>
      using var_type = rlref_copy<TCur, variant_alternative_t<tIndex, remove_reference_t<TCur> > >;

      template <size_t tIndex>
      struct next_inject_wrap
      {
        template <typename... TNextInj>
        using next_inject = TToInject<var_type<tIndex>, TNextInj...>;
        using recursive_result =
          typename visit_result_helper<next_inject, make_index_sequence<variant_size<remove_reference_t<TNext> >::value>, TNext, TVs...>::type;
      };

      using type = common_type_t<typename next_inject_wrap<tAltIndices>::recursive_result...>;
    };

    //***************************************************************************
    /// Generates the result type for visit by applying 'common_type' on the return
    /// type from calls to function object with all possible permutations of variant
    /// alternatives. Shortcuts to first argument unless it is 'visit_auto_return'.
    //***************************************************************************
    template <typename TRet, typename...>
    struct visit_result
    {
      using type = TRet;
    };

    template <typename TCallable, typename T1, typename... Ts>
    struct visit_result<visit_auto_return, TCallable, T1, Ts...>
    {
      // bind TCallable to the first argument in this variadic alias.
      template <typename... Ts2>
      using single_res = single_visit_result_type_t<TCallable, Ts2...>;
      using type       = typename visit_result_helper<single_res, make_index_sequence<variant_size<remove_reference_t<T1> >::value>, T1, Ts...>::type;
    };

    template <typename... Ts>
    using visit_result_t = typename visit_result<Ts...>::type;

    //***************************************************************************
    /// Makes a call to TCallable using tIndex alternative to the variant.
    /// Instantiated as function pointer in the `do_visit` function.
    //***************************************************************************
    template <typename TRet, typename TCallable, typename TVariant, size_t tIndex>
    constexpr TRet do_visit_single(TCallable&& f, TVariant&& v)
    {
      return static_cast<TCallable&&>(f)(etl::get<tIndex>(static_cast<TVariant&&>(v)));
    }

    //***************************************************************************
    /// Helper to instantiate the function pointers needed for the "jump table".
    /// Embeds the 'TVarRest' (remaining variants) into its type to come around
    /// the "double expansion" otherwise needed in "do_visit".
    //***************************************************************************
    template <typename TRet, typename TCallable, typename TCurVariant, typename... TVarRest>
    struct do_visit_helper
    {
      using function_pointer = add_pointer_t<TRet(TCallable&&, TCurVariant&&, TVarRest&&...)>;

      template <size_t tIndex>
      static constexpr function_pointer fptr() ETL_NOEXCEPT
      {
        return &do_visit_single<TRet, TCallable, TCurVariant, tIndex, TVarRest...>;
      }
    };

    //***************************************************************************
    /// Dispatch current variant into recursive calls to dispatch the rest.
    //***************************************************************************
    template <typename TRet, typename TCallable, typename TVariant, size_t... tIndices, typename... TVarRest>
    static ETL_CONSTEXPR14 TRet do_visit(TCallable&& f, TVariant&& v, index_sequence<tIndices...>, TVarRest&&... variants)
    {
      ETL_ASSERT(!v.valueless_by_exception(), ETL_ERROR(bad_variant_access));

      using helper_t = do_visit_helper<TRet, TCallable, TVariant, TVarRest...>;
      using func_ptr = typename helper_t::function_pointer;

      constexpr func_ptr jmp_table[]{helper_t::template fptr<tIndices>()...};

      return jmp_table[v.index()](static_cast<TCallable&&>(f), static_cast<TVariant&&>(v), static_cast<TVarRest&&>(variants)...);
    }

    template <typename TRet, typename TCallable, typename TVariant, typename... TVs>
    static ETL_CONSTEXPR14 TRet visit(TCallable&& f, TVariant&& v, TVs&&... vs)
    {
      constexpr size_t variants = etl::variant_size<typename remove_reference<TVariant>::type>::value;
      return private_variant::do_visit<TRet>(static_cast<TCallable&&>(f), static_cast<TVariant&&>(v), make_index_sequence<variants>{},
                                             static_cast<TVs&&>(vs)...);
    }

    //***************************************************************************
    /// Allows constexpr operation in c++14, otherwise acts like a lambda to
    /// bind a variant "get" to an argument for "TCallable".
    //***************************************************************************
    template <typename TRet, typename TCallable, typename TVariant, size_t tIndex>
    class constexpr_visit_closure
    {
      add_pointer_t<TCallable> callable_;
      add_pointer_t<TVariant>  variant_;

    public:

      constexpr constexpr_visit_closure(TCallable&& c, TVariant&& v)
        : callable_(&c)
        , variant_(&v)
      {
      }

      template <typename... Ts>
      ETL_CONSTEXPR14 TRet operator()(Ts&&... args) const
      {
        return static_cast<TCallable&&>(*callable_)(get<tIndex>(static_cast<TVariant&&>(*variant_)), static_cast<Ts&&>(args)...);
      }
    };

    template <typename TRet, typename TCallable, typename TVariant, size_t tIndex, typename TNext, typename... TVariants>
    static ETL_CONSTEXPR14 TRet do_visit_single(TCallable&& f, TVariant&& v, TNext&& next, TVariants&&... vs)
    {
      return private_variant::visit<TRet>(
        constexpr_visit_closure<TRet, TCallable, TVariant, tIndex>(static_cast<TCallable&&>(f), static_cast<TVariant&&>(v)),
        static_cast<TNext&&>(next), static_cast<TVariants&&>(vs)...);
    }

  } // namespace private_variant

  //***************************************************************************
  /// C++11/14 compatible etl::visit for etl::variant. Supports both c++17
  /// "auto return type" signature and c++20 explicit template return type.
  //***************************************************************************
  template <typename TRet           = private_variant::visit_auto_return, typename... TVariants, typename TCallable,
            typename TDeducedReturn = private_variant::visit_result_t<TRet, TCallable, TVariants...> >
  static ETL_CONSTEXPR14 TDeducedReturn visit(TCallable&& f, TVariants&&... vs)
  {
    return private_variant::visit<TDeducedReturn>(static_cast<TCallable&&>(f), static_cast<TVariants&&>(vs)...);
  }

  namespace private_variant
  {
    //***************************************************************************
    /// C++11 compatible visitor function for testing variant equality.
    /// Assumes that the two variants are already known to contain the same type.
    //***************************************************************************
    template <typename TVariant>
    struct equality_visitor
    {
      equality_visitor(const TVariant& rhs_)
        : rhs(rhs_)
      {
      }

      template <typename TValue>
      bool operator()(const TValue& lhs_downcasted)
      {
        return lhs_downcasted == etl::get<TValue>(rhs);
      }

      const TVariant& rhs;
    };

    //***************************************************************************
    /// C++11 compatible visitor function for testing variant '<' (less than).
    /// Assumes that the two variants are already known to contain the same type.
    //***************************************************************************
    template <typename TVariant>
    struct less_than_visitor
    {
      less_than_visitor(const TVariant& rhs_)
        : rhs(rhs_)
      {
      }

      template <typename TValue>
      bool operator()(const TValue& lhs_downcasted)
      {
        return lhs_downcasted < etl::get<TValue>(rhs);
      }

      const TVariant& rhs;
    };
  } // namespace private_variant

  //***************************************************************************
  /// Checks if the variants are equal.
  /// https://en.cppreference.com/w/cpp/utility/variant/operator_cmp
  //***************************************************************************
  template <typename... TTypes>
  ETL_CONSTEXPR14 bool operator==(const etl::variant<TTypes...>& lhs, const etl::variant<TTypes...>& rhs)
  {
    // If both variants are valueless, they are considered equal
    if (lhs.valueless_by_exception() && rhs.valueless_by_exception())
    {
      return true;
    }

    // If one variant is valueless and the other is not, they are not equal
    if (lhs.valueless_by_exception() || rhs.valueless_by_exception())
    {
      return false;
    }

    // If variants have different types, they are not equal
    if (lhs.index() != rhs.index())
    {
      return false;
    }

    // Variants have the same type, apply the equality operator for the contained values
    private_variant::equality_visitor<etl::variant<TTypes...> > visitor(rhs);

    return etl::visit(visitor, lhs);
  }

  //***************************************************************************
  /// Checks if the variants not equal.
  /// https://en.cppreference.com/w/cpp/utility/variant/operator_cmp
  //***************************************************************************
  template <typename... TTypes>
  ETL_CONSTEXPR14 bool operator!=(const etl::variant<TTypes...>& lhs, const etl::variant<TTypes...>& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Checks if the lhs variant is less than rhs.
  /// https://en.cppreference.com/w/cpp/utility/variant/operator_cmp
  //***************************************************************************
  template <typename... TTypes>
  ETL_CONSTEXPR14 bool operator<(const etl::variant<TTypes...>& lhs, const etl::variant<TTypes...>& rhs)
  {
    // If both variants are valueless, they are considered equal, so not less than
    if (lhs.valueless_by_exception() && rhs.valueless_by_exception())
    {
      return false;
    }

    // A valueless variant is always less than a variant with a value
    if (lhs.valueless_by_exception())
    {
      return true;
    }

    // A variant with a value is never less than a valueless variant
    if (rhs.valueless_by_exception())
    {
      return false;
    }

    // If variants have different types, compare the type index
    if (lhs.index() != rhs.index())
    {
      return lhs.index() < rhs.index();
    }

    // Variants have the same type, apply the less than operator for the contained values
    private_variant::less_than_visitor<etl::variant<TTypes...> > visitor(rhs);

    return etl::visit(visitor, lhs);
  }

  //***************************************************************************
  /// Checks if the lhs variant is greater than rhs.
  /// https://en.cppreference.com/w/cpp/utility/variant/operator_cmp
  //***************************************************************************
  template <typename... TTypes>
  ETL_CONSTEXPR14 bool operator>(const etl::variant<TTypes...>& lhs, const etl::variant<TTypes...>& rhs)
  {
    return (rhs < lhs);
  }

  //***************************************************************************
  /// Checks if the lhs variant is less than or equal to rhs.
  /// https://en.cppreference.com/w/cpp/utility/variant/operator_cmp
  //***************************************************************************
  template <typename... TTypes>
  ETL_CONSTEXPR14 bool operator<=(const etl::variant<TTypes...>& lhs, const etl::variant<TTypes...>& rhs)
  {
    return !(lhs > rhs);
  }

  //***************************************************************************
  /// Checks if the lhs variant is greater than or equal to rhs.
  /// https://en.cppreference.com/w/cpp/utility/variant/operator_cmp
  //***************************************************************************
  template <typename... TTypes>
  ETL_CONSTEXPR14 bool operator>=(const etl::variant<TTypes...>& lhs, const etl::variant<TTypes...>& rhs)
  {
    return !(lhs < rhs);
  }

  namespace private_variant
  {
  #if ETL_USING_CPP20 && ETL_USING_STL && !(defined(ETL_DEVELOPMENT_OS_APPLE) && defined(ETL_COMPILER_CLANG))
    //***************************************************************************
    /// C++20 compatible visitor function for testing variant '<=>'.
    /// Assumes that the two variants are already known to contain the same type.
    //***************************************************************************
    template <typename TVariant>
    struct compare_visitor
    {
      compare_visitor(const TVariant& rhs_)
        : rhs(rhs_)
      {
      }

      template <typename TValue>
      std::strong_ordering operator()(const TValue& lhs_downcasted)
      {
        return lhs_downcasted <=> etl::get<TValue>(rhs);
      }

      const TVariant& rhs;
    };
  #endif
  } // namespace private_variant

  //***************************************************************************
  /// Defines the 'spaceship' <=> operator for comparing variants.
  /// Only defined if using C++20 and STL.
  /// https://en.cppreference.com/w/cpp/utility/variant/operator_cmp
  //***************************************************************************
  #if ETL_USING_CPP20 && ETL_USING_STL && !(defined(ETL_DEVELOPMENT_OS_APPLE) && defined(ETL_COMPILER_CLANG))
  template <typename... TTypes>
  ETL_CONSTEXPR14 std::common_comparison_category_t<std::compare_three_way_result_t<TTypes>...> operator<=>(const etl::variant<TTypes...>& lhs,
                                                                                                      const etl::variant<TTypes...>& rhs)
  {
    if (lhs.valueless_by_exception() && rhs.valueless_by_exception())
    {
      return std::strong_ordering::equal;
    }
    else if (lhs.valueless_by_exception())
    {
      return std::strong_ordering::less;
    }
    else if (rhs.valueless_by_exception())
    {
      return std::strong_ordering::greater;
    }
    else if (lhs.index() != rhs.index())
    {
      return lhs.index() <=> rhs.index();
    }
    else
    {
      // Variants have the same type, apply the equality operator for the contained values
      private_variant::compare_visitor<etl::variant<TTypes...> > visitor(rhs);

      return etl::visit(visitor, lhs);
    }
  }
  #endif

  //***************************************************************************
  /// Helper to turn etl::type_list<TTypes...> into etl::variant<TTypes...>
  template <typename TList>
  struct variant_from_type_list;

  template <typename... TTypes>
  struct variant_from_type_list<etl::type_list<TTypes...> >
  {
    using type = etl::variant<TTypes...>;
  };

  template <typename TTypeList>
  using variant_from_type_list_t = typename variant_from_type_list<TTypeList>::type;
} // namespace etl
#endif
