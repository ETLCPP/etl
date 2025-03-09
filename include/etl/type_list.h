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
#include "nth_type.h"
#include "integral_limits.h"
#include "static_assert.h"
#include "type_traits.h"
#include "utility.h"

#if ETL_USING_CPP11
namespace etl
{

  static ETL_CONSTEXPR size_t type_list_npos = etl::integral_limits<size_t>::max;

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
    using type = THead;
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
    using type = THead;
    using tail = typename private_type_list::recursion_helper<>::type;

    static constexpr size_t size = 1U;

    using index_sequence_type = etl::make_index_sequence<1>; ///< The index_sequence type for this type_list.

  private:

    type_list() ETL_DELETE;
    type_list(const type_list&) ETL_DELETE;
    type_list& operator =(const type_list&) ETL_DELETE;
  };

  //***************************************************************************
  /// Specialisation of etl::nth_type for etl::type_list
  //***************************************************************************
  template <size_t N, typename THead, typename... TTail>
  struct nth_type<N, type_list<THead, TTail...>>
  {
    ETL_STATIC_ASSERT(N <= sizeof...(TTail), "etl::nth_type out of range for etl::type_list");

    using type = typename nth_type<N - 1, type_list<TTail...>>::type;
  };

  //***************************************************************************
  /// Specialisation of etl::nth_type for etl::type_list with index of 0
  //***************************************************************************
  template <typename THead, typename... TTail>
  struct nth_type<0, type_list<THead, TTail...>>
  {
    using type = THead;
  };

  //***************************************************************************
  /// Specialisation of etl::nth_type for empty etl::type_list 
  //***************************************************************************
  template <size_t N>
  struct nth_type<N, type_list<>>
  {
  };

  //***************************************************************************
  /// Declares a new type_list by selecting types from a given type_list, according to an index sequence.
  //***************************************************************************
  template <typename TTypeList, size_t... Indices>
  struct type_list_select
  {
    using type = type_list<nth_type_t<Indices, TTypeList>...>;
  };

  template <typename TTypeList, size_t... Indices>
  using type_list_select_t = typename type_list_select<TTypeList, Indices...>::type;

  //***************************************************************************
  /// Type list size.
  //***************************************************************************
  template <typename... TTypes>
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
  /// Concatenates two or more type_lists.
  //***************************************************************************
  template <typename... TypeLists>
  struct type_list_cat;

  //***************************************************************************
  /// Concatenates two or more type_lists.
  /// Specialisation for a single type_list (base case)
  //***************************************************************************
  template <typename TypeList>
  struct type_list_cat<TypeList>
  {
    using type = TypeList;
  };

  //***************************************************************************
  /// Concatenates two or more type_lists.
  /// Specialisation for two or more type_lists
  //***************************************************************************
  template <typename... TTypes1, typename... TTypes2, typename... TTail>
  struct type_list_cat<etl::type_list<TTypes1...>, etl::type_list<TTypes2...>, TTail...>
  {
    using type = typename type_list_cat<etl::type_list<TTypes1..., TTypes2...>, TTail...>::type;
  };

  template<typename TypeList, typename T>
  struct type_list_contains
    : public etl::integral_constant<bool, etl::is_same<typename TypeList::type, T>::value ? true : type_list_contains<typename TypeList::tail, T>::value>
  {
  };

  template<typename T>
  struct type_list_contains<type_list<>, T>
    : public etl::integral_constant<bool, false>
  {
  };

#if ETL_USING_CPP17
  template<typename TypeList, typename T>
  inline constexpr bool type_list_contains_v = etl::type_list_contains<TypeList, T>::value;
#endif

  template<typename TypeList, typename T>
  struct type_list_index_of
    : public etl::integral_constant<size_t, etl::is_same<typename TypeList::type, T>::value ? 0 :
    (type_list_index_of<typename TypeList::tail, T>::value == type_list_npos ?
     type_list_npos : type_list_index_of<typename TypeList::tail, T>::value + 1)>
  {
  };

  template<typename T>
  struct type_list_index_of<type_list<>, T>
    : public etl::integral_constant<size_t, type_list_npos>
  {
  };

#if ETL_USING_CPP17
  template<typename TypeList, typename T>
  inline constexpr size_t type_list_index_of_v = etl::type_list_index_of<TypeList, T>::value;
#endif

  template<typename TypeList>
  struct type_list_max_sizeof_type
    : public etl::integral_constant<size_t, etl::max(sizeof(typename TypeList::type), type_list_max_sizeof_type<typename TypeList::tail>::value)>
  {
  };

  template<>
  struct type_list_max_sizeof_type<type_list<>>
    : public etl::integral_constant<size_t, 0>
  {
  };

#if ETL_USING_CPP17
  template<typename TypeList>
  inline constexpr size_t type_list_max_sizeof_type_v = etl::type_list_max_sizeof_type<TypeList>::value;
#endif

  template<typename TypeList>
  struct type_list_max_alignment
    : public etl::integral_constant<size_t, etl::max(etl::alignment_of<typename TypeList::type>::value,
        type_list_max_alignment<typename TypeList::tail>::value)>
  {
  };

  template<>
  struct type_list_max_alignment<type_list<>>
    : public etl::integral_constant<size_t, 1>
  {
  };

#if ETL_USING_CPP17
  template<typename TypeList>
  inline constexpr size_t type_list_max_alignment_v = etl::type_list_max_alignment<TypeList>::value;
#endif
}
#endif

#endif
