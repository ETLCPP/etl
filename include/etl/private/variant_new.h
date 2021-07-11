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

#include <stdint.h>

#include "../platform.h"
#include "../utility.h"
#include "../array.h"
#include "../largest.h"
#include "../exception.h"
#include "../type_traits.h"
#include "../integral_limits.h"
#include "../static_assert.h"
#include "../alignment.h"
#include "../error_handler.h"
#include "../parameter_pack.h"
#include "../placement_new.h"
#include "../visitor.h"
#include "../memory.h"

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
    //***************************************************************************
    // This is a copy of the normal etl::parameter_pack, but without the static_assert
    // so that the C++11 versions of do_accept() & do_operator() do not throw a compile time error.
    //***************************************************************************
    template <typename... TTypes>
    class parameter_pack
    {
    public:

      static constexpr size_t size = sizeof...(TTypes);

      //***************************************************************************
      /// index_of_type
      //***************************************************************************
      template <typename T>
      class index_of_type
      {
      private:

        using type = etl::remove_reference_t<T>;

        //***********************************
        template <typename Type, typename T1, typename... TRest>
        struct index_of_type_helper
        {
          static constexpr size_t value = etl::is_same<Type, T1>::value ? 1 : 1 + index_of_type_helper<Type, TRest...>::value;
        };

        //***********************************
        template <typename Type, typename T1>
        struct index_of_type_helper<Type, T1>
        {
          static constexpr size_t value = 1;
        };

      public:

        static_assert(etl::is_one_of_v<type, TTypes...>, "T is not in parameter pack");

        /// The index value.
        static constexpr size_t value = index_of_type_helper<type, TTypes...>::value - 1;
      };

      //***************************************************************************
      /// type_from_index
      //***************************************************************************
      template <size_t I>
      class type_from_index
      {
      private:

        //***********************************
        template <size_t II, size_t N, typename T1, typename... TRest>
        struct type_from_index_helper
        {
          using type = typename etl::conditional<II == N, T1, typename type_from_index_helper<II, N + 1, TRest...>::type>::type;
        };

        //***********************************
        template <size_t II, size_t N, typename T1>
        struct type_from_index_helper<II, N, T1>
        {
          using type = T1;
        };

      public:

        /// Template alias
        using type = typename type_from_index_helper<I, 0, TTypes...>::type;
      };

      //***********************************
      template <size_t I>
      using type_from_index_t = typename type_from_index<I>::type;
    };
  }

  /// Definition of variant_npos.
  constexpr size_t variant_npos = etl::integral_limits<size_t>::max;

  // Forward declarations.
  template <typename... TTypes>
  class variant;

  template <typename T, typename... TTypes>
  ETL_CONSTEXPR14 bool holds_alternative(const etl::variant<TTypes...>& v) noexcept;

  //***************************************************************************
  /// Monostate for variants.
  ///\ingroup variant
  //***************************************************************************
  struct monostate 
  {
  };

  constexpr bool operator >(etl::monostate, etl::monostate) noexcept { return false; }
	constexpr bool operator <(etl::monostate, etl::monostate) noexcept { return false; }
	constexpr bool operator !=(etl::monostate, etl::monostate) noexcept { return false; }
	constexpr bool operator <=(etl::monostate, etl::monostate) noexcept { return true; }
	constexpr bool operator >=(etl::monostate, etl::monostate) noexcept { return true; }
	constexpr bool operator ==(etl::monostate, etl::monostate) noexcept { return true; }

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
  /// A template class that can store any of the types defined in the template parameter list.
  /// Supports up to 8 types.
  ///\ingroup variant
  //***************************************************************************
  template <typename... TTypes>
  class variant
  {
  public:

    //***************************************************************************
    /// The type used for ids.
    //***************************************************************************
    using type_id_t = uint_least8_t ;

  private:

    // The type of actions we can perform.
    enum class action_type : char
    {
      Create,
      Destroy
    };

    // All types of variant are friends.
    template <typename... UTypes>
    friend class variant;

    //***************************************************************************
    /// The largest type.
    //***************************************************************************
    using largest_t = typename largest_type<TTypes...>::type;

    //***************************************************************************
    /// The largest size.
    //***************************************************************************
    static const size_t Size = sizeof(largest_t);

    //***************************************************************************
    /// The largest alignment.
    //***************************************************************************
    static const size_t Alignment = etl::largest_alignment<TTypes...>::value;

  public:

    //***************************************************************************
    /// The internal storage.
    /// Aligned on a suitable boundary, which should be good for all types.
    //***************************************************************************
    etl::uninitialized_buffer<Size, 1U, Alignment> data;

    //***************************************************************************
    /// Default constructor.
    /// Sets the state of the instance to containing no valid data.
    //***************************************************************************
    ETL_CONSTEXPR14 variant()
      : data()
    {
      using type = typename etl::private_variant::parameter_pack<TTypes...>::template type_from_index<0>::type;

      operation = do_operation_default_construct<type>;
      operation(action_type::Create, data, nullptr);
      type_id = 0;
    }

    //***************************************************************************
    /// Constructor from a value.
    //***************************************************************************
    template <typename T, etl::enable_if_t<!etl::is_same<etl::remove_reference_t<T>, variant>::value, int> = 0>
    ETL_CONSTEXPR14 variant(T&& value)
      : data()
      , operation(operation_type<T, etl::is_rvalue_reference<decltype(value)>::value>::do_operation)
      , type_id(etl::private_variant::parameter_pack<TTypes...>::template index_of_type<etl::remove_reference_t<T>>::value)
    {
      static_assert(etl::is_one_of<etl::remove_reference_t<T>, TTypes...>::value, "Unsupported type");

      operation(action_type::Create, data, &value);
    }

    //***************************************************************************
    /// Construct from arguments.
    //***************************************************************************
    template <typename T, typename... TArgs>
    ETL_CONSTEXPR14 explicit variant(etl::in_place_type_t<T>, TArgs&&... args)
      : data()
      , type_id(etl::private_variant::parameter_pack<TTypes...>::template index_of_type<etl::remove_reference_t<T>>::value)
    {
      T temp(std::forward<TArgs>(args)...);

      operation = operation_type<T, etl::is_rvalue_reference<decltype(temp)>::value>::do_operation;
      operation(action_type::Create, data, &temp);
    }

    //***************************************************************************
    /// Construct from arguments.
    //***************************************************************************
    template <size_t Index, typename... TArgs>
    ETL_CONSTEXPR14 explicit variant(etl::in_place_index_t<Index>, TArgs&&... args)
      : data()
    {
      using type = private_variant::parameter_pack<TTypes...>::type_from_index_t<Index>;

      type temp(std::forward<TArgs>(args)...);

      operation = operation_type<type, etl::is_rvalue_reference<decltype(temp)>::value>::do_operation;
      operation(action_type::Create, data, &temp);

      type_id = Index;
    }

    //***************************************************************************
    /// Copy constructor.
    ///\param other The other variant object to copy.
    //***************************************************************************
    ETL_CONSTEXPR14 variant(const variant& other)
      : data()
      , operation(other.operation)
      , type_id(other.type_id)
    {
      if (this != &other)
      {
        if (other.index() == variant_npos)
        {
          type_id = variant_npos;
        }
        else
        {
          operation(action_type::Create, data, other.data);
        }
      }
    }

    //***************************************************************************
    /// Move constructor.
    ///\param other The other variant object to copy.
    //***************************************************************************
    ETL_CONSTEXPR14 variant(variant&& other)
      : data()
      , operation(other.operation)
      , type_id(other.type_id)
    {
      if (this != &other)
      {
        if (other.index() == variant_npos)
        {
          type_id = variant_npos;
        }
        else
        {
          operation(action_type::Create, data, other.data);
        }
      }
      else
      {
        type_id = variant_npos;
      }
    }

    //***************************************************************************
    /// Destructor.
    //***************************************************************************
    ~variant()
    {
      if (index() != variant_npos)
      {
        operation(action_type::Destroy, data, nullptr);        
      }

      operation = null_operation;
      type_id = variant_npos;
    }

    //***************************************************************************
    /// Emplace with variadic constructor parameters.
    //***************************************************************************
    template <typename T, typename... TArgs>
    T& emplace(TArgs&&... args)
    {
      static_assert(etl::is_one_of<T, TTypes...>::value, "Unsupported type");

      operation(action_type::Destroy, data, nullptr);
     
      T temp(etl::forward<TArgs>(args)...);
      operation = operation_type<T, etl::is_rvalue_reference<decltype(temp)>::value>::do_operation;
      operation(action_type::Create, data, &temp);

      type_id = etl::private_variant::parameter_pack<TTypes...>::template index_of_type<T>::value;

      return *static_cast<T*>(data);
    }

    //***************************************************************************
    /// Move assignment operator for type.
    ///\param value The value to assign.
    //***************************************************************************
    template <typename T, etl::enable_if_t<!etl::is_same_v<etl::remove_reference_t<T>, variant>, int> = 0>
    variant& operator =(T&& value)
    {
      static_assert(etl::is_one_of<etl::remove_reference_t<T>, TTypes...>::value, "Unsupported type");

      operation(action_type::Destroy, data, nullptr);

      operation = operation_type<T, etl::is_rvalue_reference<decltype(value)>::value>::do_operation;
      operation(action_type::Create, data, &value);

      type_id = etl::private_variant::parameter_pack<TTypes...>::template index_of_type<T>::value;

      return *this;
    }

    //***************************************************************************
    /// Assignment operator for variant type.
    ///\param other The variant to assign.
    //***************************************************************************
    variant& operator =(const variant& other)
    {
      if (this != &other)
      {
        if (other.index() == variant_npos)
        {
          type_id = variant_npos;
        }
        else
        {
          operation(action_type::Destroy, data, nullptr);

          operation = other.operation;
          operation(action_type::Create, data, other.data);

          type_id = other.type_id;
        }
      }

      return *this;
    }

    //***************************************************************************
    /// Assignment operator for variant type.
    ///\param other The variant to assign.
    //***************************************************************************
    variant& operator =(variant&& other)
    {
      if (this != &other)
      {
        if (other.index() == variant_npos)
        {
          type_id = variant_npos;
        }
        else
        {
          operation(action_type::Destroy, data, nullptr);

          operation = other.operation;
          operation(action_type::Create, data, other.data);

          type_id = other.type_id;
        }
      }

      return *this;
    }

    //***************************************************************************
    /// Checks whether a valid value is currently stored.
    ///\return <b>true</b> if the value is valid, otherwise <b>false</b>.
    //***************************************************************************
    constexpr bool valueless_by_exception() const noexcept
    {
      return type_id == variant_npos;
    }

    //***************************************************************************
    /// Gets the index of the type currently stored or variant_npos
    //***************************************************************************
    constexpr size_t index() const noexcept
    {
      return type_id;
    }

    //***************************************************************************
    /// Swaps this variant with another.
    //***************************************************************************
    void swap(variant& rhs) noexcept
    {
      variant temp(*this);
      *this = rhs;
      rhs = temp;
    }

    //***************************************************************************
    /// Accept an etl::visitor.
    //***************************************************************************
    template <typename... Types>
    void accept(etl::visitor<TTypes...>& v)
    {
#if ETL_CPP17_SUPPORTED && !defined(ETL_VARIANT_FORCE_CPP11)
      do_accept(v, etl::make_index_sequence<sizeof...(TTypes)>{});
#else
      do_accept(v);
#endif
    }

    //***************************************************************************
    /// Accept a generic visitor.
    //***************************************************************************
    template <typename TVisitor>
    void operator()(TVisitor& v)
    {
#if ETL_CPP17_SUPPORTED && !defined(ETL_VARIANT_FORCE_CPP11)
      do_operator(v, etl::make_index_sequence<sizeof...(TTypes)>{});
#else
      do_operator(v);
#endif
    }

  private:

    using operation_function = void(*)(action_type, void*, const void*);

    //***************************************************************************
    /// Do an operation determined by type.
    //***************************************************************************
    template <typename T>
    static void do_operation_default_construct(action_type action, void* pstorage, const void* pvalue)
    {
      switch (action)
      {
      case action_type::Create:
      {
        ::new (pstorage) T();
        break;
      }

      case action_type::Destroy:
      {
        reinterpret_cast<const T*>(pstorage)->~T();
        break;
      }

      default:
      {
        break;
      }
      }
    }

    // Declaration.
    template <typename T, bool IsRValueRef> 
    struct operation_type;

    // Specialisation for lvalue rreferences
    template <typename T> 
    struct operation_type<T, false>
    {
      static void do_operation(variant::action_type action, void* pstorage, const void* pvalue)
      {
        using type = etl::remove_reference_t<T>;

        switch (action)
        {
          case variant::action_type::Create:
          {
            ::new (pstorage) type(*reinterpret_cast<const type*>(pvalue));
            break;
          }

          case variant::action_type::Destroy:
          {
            reinterpret_cast<const type*>(pstorage)->~type();
            break;
          }

          default:
          {
            break;
          }
        }
      }
    };

    // Specialisation for rvalue rreferences
    template <typename T>
    struct operation_type<T, true>
    {
      static void do_operation(variant::action_type action, void* pstorage, const void* pvalue)
      {
        using type = etl::remove_reference_t<T>;

        switch (action)
        {
          case variant::action_type::Create:
          {
            ::new (pstorage) type(etl::move(*reinterpret_cast<type*>(const_cast<void*>(pvalue))));
            break;
          }

          case variant::action_type::Destroy:
          {
            reinterpret_cast<const type*>(pstorage)->~type();
            break;
          }

          default:
          {
            break;
          }
        }
      }
    };

#if ETL_CPP17_SUPPORTED && !defined(ETL_VARIANT_FORCE_CPP11)
    //***************************************************************************
    /// Call the relevent visitor by attemptng each one.
    //***************************************************************************
    template <typename TVisitor, size_t... I>
    void do_accept(TVisitor& visitor, etl::index_sequence<I...>)
    {
      (attempt_visitor<I>(visitor) || ...);
    }
#else
    //***************************************************************************
    /// /// Call the relevent visitor.
    //***************************************************************************
    template <typename TVisitor>
    void do_accept(TVisitor& visitor)
    {
      switch (index())
      {
        case 0: visitor.visit(etl::get<0>(*this)); break;
        case 1: visitor.visit(etl::get<1>(*this)); break;
        case 2: visitor.visit(etl::get<2>(*this)); break;
        case 3: visitor.visit(etl::get<3>(*this)); break;
        case 4: visitor.visit(etl::get<4>(*this)); break;
        case 5: visitor.visit(etl::get<5>(*this)); break;
        case 6: visitor.visit(etl::get<6>(*this)); break;
        case 7: visitor.visit(etl::get<7>(*this)); break;
#if !defined(ETL_VARIANT_CPP11_MAX_8_TYPES)
        case 8: visitor.visit(etl::get<8>(*this)); break;
        case 9: visitor.visit(etl::get<9>(*this)); break;
        case 10: visitor.visit(etl::get<10>(*this)); break;
        case 11: visitor.visit(etl::get<11>(*this)); break;
        case 12: visitor.visit(etl::get<12>(*this)); break;
        case 13: visitor.visit(etl::get<13>(*this)); break;
        case 14: visitor.visit(etl::get<14>(*this)); break;
        case 15: visitor.visit(etl::get<15>(*this)); break;
#if !defined(ETL_VARIANT_CPP11_MAX_16_TYPES)
        case 16: visitor.visit(etl::get<16>(*this)); break;
        case 17: visitor.visit(etl::get<17>(*this)); break;
        case 18: visitor.visit(etl::get<18>(*this)); break;
        case 19: visitor.visit(etl::get<19>(*this)); break;
        case 20: visitor.visit(etl::get<20>(*this)); break;
        case 21: visitor.visit(etl::get<21>(*this)); break;
        case 22: visitor.visit(etl::get<22>(*this)); break;
        case 23: visitor.visit(etl::get<23>(*this)); break;
#if !defined(ETL_VARIANT_CPP11_MAX_24_TYPES)
        case 24: visitor.visit(etl::get<24>(*this)); break;
        case 25: visitor.visit(etl::get<25>(*this)); break;
        case 26: visitor.visit(etl::get<26>(*this)); break;
        case 27: visitor.visit(etl::get<27>(*this)); break;
        case 28: visitor.visit(etl::get<28>(*this)); break;
        case 29: visitor.visit(etl::get<29>(*this)); break;
        case 30: visitor.visit(etl::get<30>(*this)); break;
        case 31: visitor.visit(etl::get<31>(*this)); break;
#endif
#endif
#endif
        default: break;
      }
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
        visitor.visit(etl::get<Index>(*this));
        return true;
      }
      else
      {
        return false;
      }
    }

#if ETL_CPP17_SUPPORTED && !defined(ETL_VARIANT_FORCE_CPP11)
    //***************************************************************************
    /// Call the relevent visitor by attemptng each one.
    //***************************************************************************
    template <typename TVisitor, size_t... I>
    void do_operator(TVisitor& visitor, etl::index_sequence<I...>)
    {
      (attempt_operator<I>(visitor) || ...);
    }
#else
    //***************************************************************************
    /// Call the relevent visitor.
    //***************************************************************************
    template <typename TVisitor>
    void do_operator(TVisitor& visitor)
    {
#if defined(ETL_VARIANT_CPP11_MAX_8_TYPES)
      ETL_STATIC_ASSERT(sizeof...(TTypes) <= 8U, "???");
#endif

#if defined(ETL_VARIANT_CPP11_MAX_16_TYPES)
      ETL_STATIC_ASSERT(sizeof...(TTypes) <= 16U, "???");
#endif

#if defined(ETL_VARIANT_CPP11_MAX_24_TYPES)
      ETL_STATIC_ASSERT(sizeof...(TTypes) <= 24U, "???");
#endif

      switch (index())
      {
        case 0: visitor(etl::get<0>(*this)); break;
        case 1: visitor(etl::get<1>(*this)); break;
        case 2: visitor(etl::get<2>(*this)); break;
        case 3: visitor(etl::get<3>(*this)); break;
        case 4: visitor(etl::get<4>(*this)); break;
        case 5: visitor(etl::get<5>(*this)); break;
        case 6: visitor(etl::get<6>(*this)); break;
        case 7: visitor(etl::get<7>(*this)); break;
#if !defined(ETL_VARIANT_CPP11_MAX_8_TYPES)
        case 8: visitor(etl::get<8>(*this)); break;
        case 9: visitor(etl::get<9>(*this)); break;
        case 10: visitor(etl::get<10>(*this)); break;
        case 11: visitor(etl::get<11>(*this)); break;
        case 12: visitor(etl::get<12>(*this)); break;
        case 13: visitor(etl::get<13>(*this)); break;
        case 14: visitor(etl::get<14>(*this)); break;
        case 15: visitor(etl::get<15>(*this)); break;
#if !defined(ETL_VARIANT_CPP11_MAX_16_TYPES)
        case 16: visitor(etl::get<16>(*this)); break;
        case 17: visitor(etl::get<17>(*this)); break;
        case 18: visitor(etl::get<18>(*this)); break;
        case 19: visitor(etl::get<19>(*this)); break;
        case 20: visitor(etl::get<20>(*this)); break;
        case 21: visitor(etl::get<21>(*this)); break;
        case 22: visitor(etl::get<22>(*this)); break;
        case 23: visitor(etl::get<23>(*this)); break;
#if !defined(ETL_VARIANT_CPP11_MAX_24_TYPES)
        case 24: visitor(etl::get<24>(*this)); break;
        case 25: visitor(etl::get<25>(*this)); break;
        case 26: visitor(etl::get<26>(*this)); break;
        case 27: visitor(etl::get<27>(*this)); break;
        case 28: visitor(etl::get<28>(*this)); break;
        case 29: visitor(etl::get<29>(*this)); break;
        case 30: visitor(etl::get<30>(*this)); break;
        case 31: visitor(etl::get<31>(*this)); break;
#endif
#endif
#endif
        default: break;
      }
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
        visitor(etl::get<Index>(*this));
        return true;
      }
      else
      {
        return false;
      }
    }

    //***************************************************************************
    /// Default operation.
    //***************************************************************************
    template <typename... TArgs>
    static void null_operation(action_type action, void* pstorage, TArgs... args)
    {
    }

    //***************************************************************************
    /// The operation function.
    //***************************************************************************
    operation_function operation;

    //***************************************************************************
    /// The id of the current stored type.  
    //***************************************************************************
    size_t type_id;
  };

  //***************************************************************************
  /// Checks if the variant v holds the alternative T.
  //***************************************************************************
	template <typename T, typename... TTypes>
	ETL_CONSTEXPR14 bool holds_alternative(const etl::variant<TTypes...>& v) noexcept
	{
    constexpr size_t Index = etl::private_variant::parameter_pack<TTypes...>::template index_of_type<T>::value;
		
    return (Index == variant_npos) ? false : (v.index() == Index);
	}

  //***************************************************************************
  /// variant_alternative
  //***************************************************************************
  template <size_t Index, typename T>
  struct variant_alternative;

  template <size_t Index, typename... TTypes>
  struct variant_alternative<Index, etl::variant<TTypes...>>
  {
    using type = typename etl::private_variant::parameter_pack<TTypes...>::template type_from_index<Index>::type;
  };

  template <size_t Index, typename T> 
  struct variant_alternative<Index, const T>
  {
    using type = typename variant_alternative<Index, T>::type;
  };

  template <size_t Index, typename T>
  using variant_alternative_t = typename variant_alternative<Index, T>::type;

  //***************************************************************************
  /// get
  //***************************************************************************
  template <size_t Index, typename... TTypes>
  ETL_CONSTEXPR14 etl::variant_alternative_t<Index, etl::variant<TTypes...>>&
    get(etl::variant<TTypes...>& v)
  {
#if ETL_CPP17_SUPPORTED && !defined(ETL_VARIANT_FORCE_CPP11)
    static_assert(Index < sizeof...(TTypes), "Index out of range");
#endif

    ETL_ASSERT(Index == v.index(), ETL_ERROR(etl::variant_incorrect_type_exception));

		using type = etl::variant_alternative_t<Index, etl::variant<TTypes...>>;
		
    return *static_cast<type*>(v.data);
  }

  //***********************************
  template <size_t Index, typename... TTypes>
  ETL_CONSTEXPR14 etl::variant_alternative_t<Index, etl::variant<TTypes...>>&&
    get(etl::variant<TTypes...>&& v)
  {
#if ETL_CPP17_SUPPORTED && !defined(ETL_VARIANT_FORCE_CPP11)
    static_assert(Index < sizeof...(TTypes), "Index out of range");
#endif

		using type = etl::variant_alternative_t<Index, etl::variant<TTypes...>>;
		
    return etl::move(*static_cast<type*>(v.data));
  }

  //***********************************
  template <size_t Index, typename... TTypes>
  ETL_CONSTEXPR14 const etl::variant_alternative_t<Index, const etl::variant<TTypes...>>&
    get(const etl::variant<TTypes...>& v)
  {
#if ETL_CPP17_SUPPORTED && !defined(ETL_VARIANT_FORCE_CPP11)
    static_assert(Index < sizeof...(TTypes), "Index out of range");
#endif

    ETL_ASSERT(Index == v.index(), ETL_ERROR(etl::variant_incorrect_type_exception));

		using type = etl::variant_alternative_t<Index, etl::variant<TTypes...>>;
		
    return *static_cast<type*>(v.data);
  }

  //***********************************
  template <size_t Index, typename... TTypes>
  ETL_CONSTEXPR14 const etl::variant_alternative_t<Index, const etl::variant<TTypes...>>&&
    get(const etl::variant<TTypes...>&& v)
  {
#if ETL_CPP17_SUPPORTED & !defined(ETL_VARIANT_FORCE_CPP11)
    static_assert(Index < sizeof...(TTypes), "Index out of range");
#endif

    ETL_ASSERT(Index == v.index(), ETL_ERROR(etl::variant_incorrect_type_exception));

		using type = etl::variant_alternative_t<Index, etl::variant<TTypes...>>;
		
    return etl::move(*static_cast<type*>(v.data));
  }

  //***********************************
  template <typename T, typename... TTypes>
  ETL_CONSTEXPR14 T& get(etl::variant<TTypes...>& v)
  {
    constexpr size_t Index = etl::private_variant::parameter_pack<TTypes...>::template index_of_type<T>::value;

    return get<Index>(v);
  }

  //***********************************
  template <typename T, typename... TTypes>
  ETL_CONSTEXPR14 T&& get(etl::variant<TTypes...>&& v)
  {
    constexpr size_t Index = etl::private_variant::parameter_pack<TTypes...>::template index_of_type<T>::value;

    return get<Index>(v);
  }

  //***********************************
  template <typename T, typename... TTypes>
  ETL_CONSTEXPR14 const T& get(const etl::variant<TTypes...>& v)
  {
    constexpr size_t Index = etl::private_variant::parameter_pack<TTypes...>::template index_of_type<T>::value;

    return get<Index>(v);
  }

  //***********************************
  template <typename T, typename... TTypes>
  ETL_CONSTEXPR14 const T&& get(const etl::variant<TTypes...>&& v)
  {
    constexpr size_t Index = etl::private_variant::parameter_pack<TTypes...>::template index_of_type<T>::value;

    return get<Index>(v);
  }

  //***************************************************************************
  /// get_if (pointer parameter)
  //***************************************************************************
  template < size_t Index, typename... TTypes >
  ETL_CONSTEXPR14 etl::add_pointer_t<etl::variant_alternative_t<Index, etl::variant<TTypes...>>>
    get_if(etl::variant<TTypes...>* pv) noexcept
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
  template< size_t Index, typename... TTypes >
  ETL_CONSTEXPR14 etl::add_pointer_t<const etl::variant_alternative_t<Index, etl::variant<TTypes...>>>
    get_if(const etl::variant<TTypes...>* pv) noexcept
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
  template< class T, typename... TTypes >
  ETL_CONSTEXPR14 etl::add_pointer_t<T> get_if(etl::variant<TTypes...>* pv) noexcept
  {
    constexpr size_t Index = etl::private_variant::parameter_pack<TTypes...>::template index_of_type<T>::value;

    return etl::get_if<Index>(pv);
  }

  //***********************************
  template< class T, typename... TTypes >
  ETL_CONSTEXPR14 etl::add_pointer_t<const T> get_if(const etl::variant<TTypes...>* pv) noexcept
  {
    constexpr size_t Index = etl::private_variant::parameter_pack<TTypes...>::template index_of_type<T>::value;

    return etl::get_if<Index>(pv);
  }

  //***************************************************************************
  /// get_if (reference parameter)
  //***************************************************************************
  template < size_t Index, typename... TTypes >
  ETL_CONSTEXPR14 etl::add_pointer_t<etl::variant_alternative_t<Index, etl::variant<TTypes...>>>
    get_if(etl::variant<TTypes...>& v) noexcept
  {
    if (v.index() == Index)
    {
      return &etl::get<Index>(v);
    }
    else
    {
      return nullptr;
    }
  }

  //***********************************
  template< size_t Index, typename... TTypes >
  ETL_CONSTEXPR14 etl::add_pointer_t<const etl::variant_alternative_t<Index, etl::variant<TTypes...>>>
    get_if(const etl::variant<TTypes...>& v) noexcept
  {
    if (v.index() == Index)
    {
      return &etl::get<Index>(v);
    }
    else
    {
      return nullptr;
    }
  }

  //***********************************
  template< size_t Index, typename... TTypes >
  ETL_CONSTEXPR14 etl::add_pointer_t<const etl::variant_alternative_t<Index, etl::variant<TTypes...>>>
    get_if(etl::variant<TTypes...>&& v) noexcept
  {
    if (v.index() == Index)
    {
      return &etl::get<Index>(v);
    }
    else
    {
      return nullptr;
    }
  }

  //***********************************
  template< class T, typename... TTypes >
  ETL_CONSTEXPR14 etl::add_pointer_t<T> get_if(etl::variant<TTypes...>& v) noexcept
  {
    constexpr size_t Index = etl::private_variant::parameter_pack<TTypes...>::template index_of_type<T>::value;

    return etl::get_if<Index>(v);
  }

  //***********************************
  template< class T, typename... TTypes >
  ETL_CONSTEXPR14 etl::add_pointer_t<const T> get_if(const etl::variant<TTypes...>& v) noexcept
  {
    constexpr size_t Index = etl::private_variant::parameter_pack<TTypes...>::template index_of_type<T>::value;

    return etl::get_if<Index>(v);
  }

  //***********************************
  template< class T, typename... TTypes >
  ETL_CONSTEXPR14 etl::add_pointer_t<const T> get_if(etl::variant<TTypes...>&& v) noexcept
  {
    constexpr size_t Index = etl::private_variant::parameter_pack<TTypes...>::template index_of_type<T>::value;

    return etl::get_if<Index>(v);
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
  struct variant_size<etl::variant<TTypes...>>
    : etl::integral_constant<size_t, sizeof...(TTypes)>
  {
  };

  template <typename T> 
  struct variant_size<const T>
    : etl::integral_constant<size_t, variant_size<T>::value>
  {
  };

#if ETL_CPP17_SUPPORTED
  template <typename... TTypes>
  inline constexpr size_t variant_size_v = variant_size<TTypes...>::value;
#endif
}
#endif
