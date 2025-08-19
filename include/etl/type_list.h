/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove

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

#ifndef ETL_TYPE_LIST_INCLUDED
#define ETL_TYPE_LIST_INCLUDED

#include "platform.h"

#include "algorithm.h"
#include "index_of_type.h"
#include "integral_limits.h"
#include "static_assert.h"
#include "type_traits.h"
#include "utility.h"
#include "largest.h"

#if ETL_USING_CPP11
namespace etl
{
  //***************************************************************************
  /// Defines a no-position constant.
  //***************************************************************************
  static ETL_CONSTANT size_t type_list_npos = etl::integral_limits<size_t>::max;

  //***************************************************************************
  /// Type list forward declaration.
  //***************************************************************************
  template <typename... TTypes>
  struct type_list;

  //***************************************************************************
  /// The empty type list.
  //***************************************************************************
  template <>
  struct type_list<>
  {
    static constexpr size_t size = 0U;

    using index_sequence_type = etl::make_index_sequence<0>; ///< The index_sequence type for this type_list.

  private:

    type_list() ETL_DELETE;
    type_list(const type_list&) ETL_DELETE;
    type_list& operator =(const type_list&) ETL_DELETE;
  };

  namespace private_type_list
  {
    // helper to solve the issue that recursed-rest can't be put directly in type_list::tail definition
    template <typename... TTypes>
    struct recursion_helper
    {
      using type = type_list<TTypes...>;
    };
  }

  //***************************************************************************
  /// Recursive type list implementation for multiple types.
  //***************************************************************************
  template <typename THead, typename... TTail>
  struct type_list<THead, TTail...> : type_list<TTail...>
  {
    using head = THead;
    using tail = typename private_type_list::recursion_helper<TTail...>::type;

    static constexpr size_t size = sizeof...(TTail) + 1U;

    using index_sequence_type = etl::make_index_sequence<sizeof...(TTail) + 1U>; ///< The index_sequence type for this type_list.

  private:

    type_list() ETL_DELETE;
    type_list(const type_list&) ETL_DELETE;
    type_list& operator =(const type_list&) ETL_DELETE;
  };

  //***************************************************************************
  /// Type list implementation for one type.
  //***************************************************************************
  template <typename THead>
  struct type_list<THead> : type_list<>
  {
    using head = THead;
    using tail = typename private_type_list::recursion_helper<>::type;

    static constexpr size_t size = 1U;

    using index_sequence_type = etl::make_index_sequence<1>; ///< The index_sequence type for this type_list.

  private:

    type_list() ETL_DELETE;
    type_list(const type_list&) ETL_DELETE;
    type_list& operator =(const type_list&) ETL_DELETE;
  };

  //***************************************************************************
  /// Type list size.
  //***************************************************************************
  template <typename TTypes>
  struct type_list_size;

  template <typename... TTypes>
  struct type_list_size<etl::type_list<TTypes...>> : public etl::integral_constant<size_t, sizeof...(TTypes)>
  {
  };

#if ETL_USING_CPP17
  template <typename... TTypes>
  inline constexpr size_t type_list_size_v = type_list_size<etl::type_list<TTypes...>>::value;
#endif

  //***************************************************************************
  /// Defines type as the type found at Index in the type_list.
  /// Static asserts if Index is out of range.
  //***************************************************************************
  template <typename TTypeList, size_t Index>
  struct type_list_type_at_index
  {
    ETL_STATIC_ASSERT(Index < type_list_size<TTypeList>::value,              "etl::type_list_type_at_index out of range");
    ETL_STATIC_ASSERT((etl::is_base_of<etl::type_list<>, TTypeList>::value), "TTypeList must be an etl::type_list");

    using type = typename type_list_type_at_index<typename TTypeList::tail, Index - 1>::type;
  };

  template <typename TTypeList>
  struct type_list_type_at_index<TTypeList, 0>
  {
    using type = typename TTypeList::head;
  };

  template <typename TTypeList, size_t Index>
  using type_list_type_at_index_t = typename type_list_type_at_index<TTypeList, Index>::type;

  //***************************************************************************
  /// Defines an integral constant that is the index of the specified type in the type_list.
  /// If the type is not in the type_list, then defined as etl::type_list_npos.
  //***************************************************************************
  template <typename TTypeList, typename T>
  struct type_list_index_of_type
    : public etl::integral_constant<size_t, etl::is_same<typename TTypeList::head, T>::value ? 0 :
                                            (type_list_index_of_type<typename TTypeList::tail, T>::value == etl::type_list_npos ? etl::type_list_npos : 
                                                                                                                                  type_list_index_of_type<typename TTypeList::tail, T>::value + 1)>
  {
    ETL_STATIC_ASSERT((etl::is_base_of<etl::type_list<>, TTypeList>::value), "TTypeList must be an etl::type_list");
  };

  template <typename T>
  struct type_list_index_of_type<type_list<>, T>
    : public etl::integral_constant<size_t, etl::type_list_npos>
  {
  };

#if ETL_USING_CPP17
  template <typename TTypeList, typename T>
  inline constexpr size_t type_list_index_of_v = etl::type_list_index_of_type<TTypeList, T>::value;
#endif

  //***************************************************************************
  /// Defines a bool constant that is true if the type_list contains the specified type, otherwise false.
  //***************************************************************************
  template <typename T, typename TTypes>
  struct type_list_contains;

  template <typename T, typename... TTypes>
  struct type_list_contains<etl::type_list<TTypes...>, T>
    : public etl::integral_constant<bool, etl::is_one_of<T, TTypes...>::value>
  {
  };

  template <typename T>
  struct type_list_contains<type_list<>, T>
    : public etl::integral_constant<bool, false>
  {
  };

#if ETL_USING_CPP17
  template <typename TTypeList, typename T>
  inline constexpr bool type_list_contains_v = etl::type_list_contains<TTypeList, T>::value;
#endif

  //***************************************************************************
  /// Defines an integral constant that is maximum sizeof all types in the type_list.
  /// If the type_list is empty, then defined as 0.
  //***************************************************************************
  template <typename T>
  struct type_list_max_size;

  template <typename... TTypes>
  struct type_list_max_size<etl::type_list<TTypes...>>
    : public etl::integral_constant<size_t, etl::largest<TTypes...>::size>
  {
  };

  template <>
  struct type_list_max_size<type_list<>>
    : public etl::integral_constant<size_t, 0>
  {
  };

#if ETL_USING_CPP17
  template <typename TTypeList>
  inline constexpr size_t type_list_max_size_v = etl::type_list_max_size<TTypeList>::value;
#endif

  //***************************************************************************
  /// Defines an integral constant that is maximum alignment all types in the type_list.
  /// If the type_list is empty, then defined as 1.
  //***************************************************************************
  template <typename T>
  struct type_list_max_alignment;

  template <typename... TTypes>
  struct type_list_max_alignment<etl::type_list<TTypes...>>
    : public etl::integral_constant<size_t, etl::largest<TTypes...>::alignment>
  {
  };

  template <>
  struct type_list_max_alignment<type_list<>>
    : public etl::integral_constant<size_t, 1>
  {
  };

#if ETL_USING_CPP17
  template <typename TTypeList>
  inline constexpr size_t type_list_max_alignment_v = etl::type_list_max_alignment<TTypeList>::value;
#endif

  //***************************************************************************
  /// Declares a new type_list by selecting types from a given type_list, according to an index sequence.
  //***************************************************************************
  template <typename TTypeList, size_t... Indices>
  struct type_list_select
  {
    ETL_STATIC_ASSERT((etl::is_base_of<etl::type_list<>, TTypeList>::value), "TTypeList must be an etl::type_list");

    using type = type_list<type_list_type_at_index_t<TTypeList, Indices>...>;
  };

  template <typename TTypeList, size_t... Indices>
  using type_list_select_t = typename type_list_select<TTypeList, Indices...>::type;

  //***************************************************************************
  /// Concatenates two or more type_lists.
  //***************************************************************************
  template <typename... TTypes>
  struct type_list_cat;

  template <typename... TTypes1, typename... TTypes2, typename... TTail>
  struct type_list_cat<etl::type_list<TTypes1...>, etl::type_list<TTypes2...>, TTail...>
  {
    using type = typename type_list_cat<etl::type_list<TTypes1..., TTypes2...>, TTail...>::type;
  };

  template <typename T>
  struct type_list_cat<T>
  {
    using type = T;
  };

  template <typename... TypeLists>
  using type_list_cat_t = typename type_list_cat<TypeLists...>::type;

  //***************************************************************************
  /// Checks that two type lists are convertible.
  /// Static asserts if the type lists are not the same length.
  //***************************************************************************
  // Primary template
  template <typename TFromList, typename TToList>
  struct type_lists_are_convertible;

  // Specialization: both lists empty, convertible
  template <>
  struct type_lists_are_convertible<etl::type_list<>, etl::type_list<>> 
    : public etl::true_type
  {
  };

  // Recursive case: check head types, then recurse
  template <typename TFromHead, typename... TFromTail, typename TToHead, typename... TToTail>
  struct type_lists_are_convertible<etl::type_list<TFromHead, TFromTail...>, etl::type_list<TToHead, TToTail...>> 
    : public etl::bool_constant<etl::is_convertible<TFromHead, TToHead>::value && 
                                etl::type_lists_are_convertible<etl::type_list<TFromTail...>, etl::type_list<TToTail...>>::value>
  {
    static_assert(sizeof...(TFromTail) == sizeof...(TToTail), "Type lists are not the same length");
  };

#if ETL_USING_CPP17
  template <typename TFromList, typename TToList>
  inline constexpr bool type_lists_are_convertible_v = etl::type_lists_are_convertible<TFromList, TToList>::value;
#endif
}
#endif

#endif
