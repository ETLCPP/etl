///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 jwellbelove

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

#if defined(ETL_COMPILER_KEIL)
  #pragma diag_suppress 940
  #pragma diag_suppress 111
#endif

//*****************************************************************************
///\defgroup variant variant
/// A class that can contain one a several specified types in a type safe manner.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  static_assert(ETL_CPP11_SUPPORTED, "Only supported for C++11 or above");

  constexpr size_t variant_npos = etl::integral_limits<size_t>::max;

  //***************************************************************************
  /// Monostate for variants.
  ///\ingroup variant
  //***************************************************************************
  struct monostate 
  {
  };

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
      : variant_exception(ETL_ERROR_TEXT("variant: unsupported type", ETL_VARIANT_FILE_ID"A"), file_name_, line_number_)
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
      Construct,
      Destruct,
      Move
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
    typename etl::aligned_storage<Size, Alignment>::type data;

    //***************************************************************************
    /// Default constructor.
    /// Sets the state of the instance to containing no valid data.
    //***************************************************************************
    constexpr variant()
      : data()
    {
      using type = typename etl::parameter_pack<TTypes...>::type_from_index<0>::type;

      type temp;

      operation = do_operation<type>;
      operation(action_type::Construct, data, &temp);
      type_id = 0;
    }

    //***************************************************************************
    /// Constructor for lvalues
    //***************************************************************************
    template <typename T>
    constexpr variant(const T& value)
      : data()
      , operation(do_operation<T>)
      , type_id(etl::parameter_pack<TTypes...>::index_of_type<T>::value)
    {
      static_assert(etl::is_one_of<T, TTypes...>::value, "Unsupported type");

      operation(action_type::Construct, data, &const_cast<T&>(value));      
    }

    //***************************************************************************
    /// Copy constructor.
    ///\param other The other variant object to copy.
    //***************************************************************************
    constexpr variant(const variant& other)
      : data()
      , operation(other.operation)
      , type_id(other.type_id)
    {
      operation(action_type::Construct, data, other.data);
    }

    //***************************************************************************
    /// Destructor.
    //***************************************************************************
    ~variant()
    {
      operation(action_type::Destruct, data, nullptr);
      operation = null_operation;

      type_id = variant_npos;
    }

    //*************************************************************************
    /// Emplace with variadic constructor parameters.
    //*************************************************************************
    template <typename T, typename... TArgs>
    T& emplace(TArgs&&... args)
    {
      static_assert(etl::is_one_of<T, TTypes...>::value, "Unsupported type");

      T temp(etl::forward<TArgs>(args)...);

      operation(action_type::Destruct, data, nullptr);
      
      operation = do_operation<T>;
      operation(action_type::Construct, data, &temp);

      type_id = etl::parameter_pack<TTypes...>::index_of_type<T>::value;

      return *static_cast<T*>(data);
    }

    //***************************************************************************
    /// Assignment operator for type.
    ///\param value The value to assign.
    //***************************************************************************
    template <typename T>
    variant& operator =(const T& value)
    {
      static_assert(etl::is_one_of<T, TTypes...>::value, "Unsupported type");

      operation(action_type::Destruct, data, nullptr);

      operation = do_operation<T>;
      operation(action_type::Construct, data, &value);

      type_id = etl::parameter_pack<TTypes...>::index_of_type<T>::value;

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
        operation(action_type::Destruct, data, nullptr);

        operation = other.operation;
        operation(action_type::Construct, data, other.data);

        type_id = other.type_id;
      }

      return *this;
    }

    //***************************************************************************
    /// Checks whether a valid value is currently stored.
    ///\return <b>true</b> if the value is valid, otherwise <b>false</b>.
    //***************************************************************************
    constexpr bool valueless_by_exception() const noexcept
    {
      return type_id != variant_npos;
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
    /// Gets the value stored as the specified template type.
    /// Throws a variant_incorrect_type_exception if the actual type is not that specified.
    ///\return A reference to the value.
    //***************************************************************************
    template <typename T>
    T& get()
    {
      static_assert(etl::is_one_of<T, TTypes...>::value, "Unsupported type");
      ETL_ASSERT(etl::holds_alternative<T>(*this), ETL_ERROR(variant_incorrect_type_exception));

      return static_cast<T&>(data);
    }

    //***************************************************************************
    /// Gets the value stored as the specified template type.
    /// Throws a variant_incorrect_type_exception if the actual type is not that specified.
    ///\return A const reference to the value.
    //***************************************************************************
    template <typename T>
    const T& get() const
    {
      static_assert(etl::is_one_of<T, TTypes...>::value, "Unsupported type");
      ETL_ASSERT(is_type<T>(), ETL_ERROR(variant_incorrect_type_exception));

      return static_cast<const T&>(data);
    }

  private:

    using operation_type = void(*)(action_type, void*, const void*);

    //***************************************************************************
    /// Do an operation determined by type.
    //***************************************************************************
    template <typename T, typename... TArgs>
    static void do_operation(action_type action, void* pstorage, const void* pvalue)
    {
      switch (action)
      {
        case action_type::Construct:
        {
          ::new (pstorage) T(*reinterpret_cast<const T*>(pvalue));         
          break;
        }

        case action_type::Destruct:
        {
          reinterpret_cast<T*>(pstorage)->~T();
          break;
        }

        default:
        {
          break;
        }
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
    operation_type operation;

    //***************************************************************************
    /// The id of the current stored type.  
    //***************************************************************************
    size_t type_id;
  };

  //***************************************************************************
  /// Checks if the variant v holds the alternative T.
  //***************************************************************************
	template <typename T, typename... TTypes>
	constexpr bool holds_alternative(const etl::variant<TTypes...>& v) noexcept
	{
    constexpr size_t Index = etl::parameter_pack<TTypes...>::index_of_type<T>::value;
		
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
    using type = typename etl::parameter_pack<TTypes...>::template type_from_index<Index>::type;
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
  constexpr etl::variant_alternative_t<Index, etl::variant<TTypes...>>&
    get(etl::variant<TTypes...>& v)
  {
    static_assert(Index < sizeof...(TTypes), "Index out of range");
    ETL_ASSERT(Index == v.index(), ETL_ERROR(etl::variant_incorrect_type_exception));

		using type = etl::variant_alternative_t<Index, etl::variant<TTypes...>>;
		
    return *static_cast<type*>(v.data);
  }

  //***********************************
  template <size_t Index, typename... TTypes>
  constexpr etl::variant_alternative_t<Index, etl::variant<TTypes...>>&&
    get(etl::variant<TTypes...>&& v)
  {
    static_assert(Index < sizeof...(TTypes), "Index out of range");

		using type = etl::variant_alternative_t<Index, etl::variant<TTypes...>>;
		
    return etl::move(*static_cast<type*>(v.data));
  }

  //***********************************
  template <size_t Index, typename... TTypes>
  constexpr const etl::variant_alternative_t<Index, const etl::variant<TTypes...>>&
    get(const etl::variant<TTypes...>& v)
  {
    static_assert(Index < sizeof...(TTypes), "Index out of range");
    ETL_ASSERT(Index == v.index(), ETL_ERROR(etl::variant_incorrect_type_exception));

		using type = etl::variant_alternative_t<Index, etl::variant<TTypes...>>;
		
    return *static_cast<type*>(v.data);
  }

  //***********************************
  template <size_t Index, typename... TTypes>
  constexpr const etl::variant_alternative_t<Index, const etl::variant<TTypes...>>&&
    get(const etl::variant<TTypes...>&& v)
  {
    static_assert(Index < sizeof...(TTypes), "Index out of range");
    ETL_ASSERT(Index == v.index(), ETL_ERROR(etl::variant_incorrect_type_exception));

		using type = etl::variant_alternative_t<Index, etl::variant<TTypes...>>;
		
    return etl::move(*static_cast<type*>(v.data));
  }

  //***********************************
  template <typename T, typename... TTypes>
  constexpr T& get(etl::variant<TTypes...>& v)
  {
    constexpr size_t Index = etl::parameter_pack<TTypes...>::index_of_type<T>::value;

    return get<Index>(v);
  }

  //***********************************
  template <typename T, typename... TTypes>
  constexpr T&& get(etl::variant<TTypes...>&& v)
  {
    constexpr size_t Index = etl::parameter_pack<TTypes...>::index_of_type<T>::value;

    return get<Index>(v);
  }

  //***********************************
  template <typename T, typename... TTypes>
  constexpr const T& get(const etl::variant<TTypes...>& v)
  {
    constexpr size_t Index = etl::parameter_pack<TTypes...>::index_of_type<T>::value;

    return get<Index>(v);
  }

  //***********************************
  template <typename T, typename... TTypes>
  constexpr const T&& get(const etl::variant<TTypes...>&& v)
  {
    constexpr size_t Index = etl::parameter_pack<TTypes...>::index_of_type<T>::value;

    return get<Index>(v);
  }

  //***************************************************************************
  /// get_if
  //***************************************************************************
  template < size_t Index, class... Types >
  constexpr etl::add_pointer_t<etl::variant_alternative_t<Index, etl::variant<Types...>>>
    get_if(etl::variant<Types...>* pv) noexcept
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
  template< size_t Index, class... Types >
  constexpr etl::add_pointer_t<const etl::variant_alternative_t<Index, etl::variant<Types...>>>
    get_if(const etl::variant<Types...>* pv) noexcept
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
  template< class T, class... Types >
  constexpr etl::add_pointer_t<T> get_if(etl::variant<Types...>* pv) noexcept
  {
    constexpr size_t Index = etl::parameter_pack<TTypes...>::index_of_type<T>::value;

    return etl::get_if<Index>(pv);
  }

  //***********************************
  template< class T, class... Types >
  constexpr etl::add_pointer_t<const T> get_if(const etl::variant<Types...>* pv) noexcept
  {
    constexpr size_t Index = etl::parameter_pack<TTypes...>::index_of_type<T>::value;

    return etl::get_if<Index>(pv);
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

}
