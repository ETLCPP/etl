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
  // Type list forward declaration.
  //***************************************************************************
  template <typename... TTypes>
  struct type_list;

  //***************************************************************************
  /// Check if a type is an etl::type_list.
  //***************************************************************************
  template <typename T>
  struct is_type_list : etl::false_type {};

  template <typename... TTypes>
  struct is_type_list<etl::type_list<TTypes...>> : etl::true_type {};

#if ETL_USING_CPP17
  template <typename T>
  inline constexpr bool is_type_list_v = is_type_list<T>::value;
#endif

  //***************************************************************************
  /// The empty type list.
  //***************************************************************************
  template <>
  struct type_list<>
  {
    static constexpr size_t size = 0U;

    using index_sequence_type = etl::make_index_sequence<0>; ///< The index_sequence type for this type_list.

  private:

    // A type_list cannot be instantiated, so delete the constructor and assignment operators.
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

    // A type_list cannot be instantiated, so delete the constructor and assignment operators.
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
    ETL_STATIC_ASSERT(Index < TTypeList::size,               "etl::type_list_type_at_index out of range");
    ETL_STATIC_ASSERT((etl::is_type_list<TTypeList>::value), "TTypeList must be an etl::type_list");

    using type = typename type_list_type_at_index<typename TTypeList::tail, Index - 1>::type;
  };

  template <typename TTypeList>
  struct type_list_type_at_index<TTypeList, 0>
  {
    ETL_STATIC_ASSERT((etl::is_type_list<TTypeList>::value), "TTypeList must be an etl::type_list");

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
    ETL_STATIC_ASSERT((etl::is_type_list<TTypeList>::value),    "TTypeList must be an etl::type_list");
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
  template <typename TTypeList, typename T>
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
  /// Defines a bool constant that is true if the type_list has duplicates of the specified type, otherwise false.
  //***************************************************************************
  template <typename TTypeList, typename T>
  struct type_list_has_duplicates_of;

  template <typename T, typename... TTypes>
  struct type_list_has_duplicates_of<etl::type_list<TTypes...>, T>
    : public etl::has_duplicates_of<T, TTypes...>
  {
  };

  template <typename T>
  struct type_list_has_duplicates_of<type_list<>, T>
    : public etl::integral_constant<bool, false>
  {
  };

#if ETL_USING_CPP17
  template <typename TTypeList, typename T>
  inline constexpr bool type_list_has_duplicates_of_v = etl::type_list_has_duplicates_of<TTypeList, T>::value;
#endif

  //***************************************************************************
  /// Defines an integral constant that is the count of the number of times a type is in the type list.
  //***************************************************************************
  template <typename TTypeList, typename T>
  struct type_list_count_of;

  template <typename T, typename... TTypes>
  struct type_list_count_of<etl::type_list<TTypes...>, T>
    : public etl::count_of<T, TTypes...>
  {
  };

  template <typename T>
  struct type_list_count_of<type_list<>, T>
    : public etl::integral_constant<size_t, 0>
  {
  };

#if ETL_USING_CPP17
  template <typename TTypeList, typename T>
  inline constexpr size_t type_list_count_of_v = etl::type_list_count_of<TTypeList, T>::value;
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
    ETL_STATIC_ASSERT((etl::is_type_list<TTypeList>::value), "TTypeList must be an etl::type_list");

    using type = type_list<type_list_type_at_index_t<TTypeList, Indices>...>;
  };

  template <typename TTypeList, size_t... Indices>
  using type_list_select_t = typename type_list_select<TTypeList, Indices...>::type;

  //***************************************************************************
  /// Declares a new type_list by selecting types from a given type_list, according to an index sequence.
  //***************************************************************************
  template <typename TTypeList, typename TIndexSequence>
  struct type_list_select_from_index_sequence;

  template <typename TTypeList, size_t... Indices>
  struct type_list_select_from_index_sequence<TTypeList, etl::index_sequence<Indices...>>
  {
    using type = etl::type_list_select_t<TTypeList, Indices...>;
  };

  template <typename TTypeList, typename TIndexSequence>
  using type_list_select_from_index_sequence_t = typename type_list_select_from_index_sequence<TTypeList, TIndexSequence>::type;

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
  /// Prepend a type to a type_list.
  //***************************************************************************
  template <typename T, typename... TTypes>
  struct type_list_prepend;

  template <typename T, typename... TTypes>
  struct type_list_prepend<etl::type_list<TTypes...>, T>
  {
    using type = type_list<T, TTypes...>;
  };

  template <typename T>
  struct type_list_prepend<T>
  {
    using type = etl::type_list<T>;
  };

  template <typename TypeList, typename T>
  using type_list_prepend_t = typename type_list_prepend<TypeList, T>::type;

  //***************************************************************************
  /// Append a type to a type_list.
  //***************************************************************************
  template <typename T, typename... TTypes>
  struct type_list_append;

  template <typename T, typename... TTypes>
  struct type_list_append<etl::type_list<TTypes...>, T>
  {
    using type = type_list<TTypes..., T>;
  };

  template <typename T>
  struct type_list_append<T>
  {
    using type = etl::type_list<T>;
  };

  template <typename TypeList, typename T>
  using type_list_append_t = typename type_list_append<TypeList, T>::type;

  //***************************************************************************
  /// Insert a type at an index in a type_list.
  /// Inserts before the type currently at Index.
  /// If Index == size of the type_list, the type is appended.
  //***************************************************************************
  template <typename TTypeList, typename T, size_t Index>
  struct type_list_insert
  {
  private:

    ETL_STATIC_ASSERT((etl::is_type_list<TTypeList>::value), "TTypeList must be an etl::type_list");
    ETL_STATIC_ASSERT(Index <= TTypeList::size,         "Index out of range");

    using index_sequence_for_prefix = etl::make_index_sequence<Index>;
    using index_sequence_for_suffix = etl::make_index_sequence_with_offset<Index, TTypeList::size - Index>;

    using prefix = etl::type_list_select_from_index_sequence_t<TTypeList, index_sequence_for_prefix>;
    using suffix = etl::type_list_select_from_index_sequence_t<TTypeList, index_sequence_for_suffix>;

  public:

    // Concatenate the prefix, new type, and suffix to create the new type list with T inserted at the correct position.
    using type = etl::type_list_cat_t<prefix, etl::type_list<T>, suffix>;
  };

#if ETL_USING_CPP11
  template <typename TTypeList, typename T, size_t Index>
  using type_list_insert_t = typename etl::type_list_insert<TTypeList, T, Index>::type;
#endif

  //***************************************************************************
  /// Remove a type at an index in a type_list.
  //***************************************************************************
  template <typename TTypeList, size_t Index>
  struct type_list_remove
  {
  private:

    ETL_STATIC_ASSERT((etl::is_type_list<TTypeList>::value), "TTypeList must be an etl::type_list");
    ETL_STATIC_ASSERT(Index < TTypeList::size,               "Index out of range");

    using index_sequence_for_prefix = etl::make_index_sequence<Index>;
    using index_sequence_for_suffix = etl::make_index_sequence_with_offset<Index + 1, TTypeList::size - Index - 1>;

    using prefix = etl::type_list_select_from_index_sequence_t<TTypeList, index_sequence_for_prefix>;
    using suffix = etl::type_list_select_from_index_sequence_t<TTypeList, index_sequence_for_suffix>;

  public:

    // Concatenate the prefix and suffix to create the new type list with the Index element removed.
    using type = etl::type_list_cat_t<prefix, suffix>;
  };

#if ETL_USING_CPP11
  template <typename TTypeList, size_t Index>
  using type_list_remove_t = typename etl::type_list_remove<TTypeList, Index>::type;
#endif

  //***************************************************************************
  // Remove types that satisfy a predicate from a type_list.
  //***************************************************************************
  namespace private_type_list
  {
    template <typename TTypeList, template <typename> class TPredicate>
    struct type_list_remove_if_impl;

    template <template <typename> class TPredicate>
    struct type_list_remove_if_impl<etl::type_list<>, TPredicate>
    {
      using type = etl::type_list<>;
    };

    template <typename Head, typename... Tail, template <typename> class TPredicate>
    struct type_list_remove_if_impl<etl::type_list<Head, Tail...>, TPredicate>
    {
    private:

      using rest = typename type_list_remove_if_impl<etl::type_list<Tail...>, TPredicate>::type;

    public:

      using type = typename etl::conditional<TPredicate<Head>::value,
                                             rest,
                                             etl::type_list_prepend_t<rest, Head>>::type;
    };
  }

  //***************************************************************************
  /// Remove types that satisfy a predicate from a type_list.
  /// Predicate must be: template <typename T> struct Pred : etl::bool_constant<...> {};
  //***************************************************************************
  template <typename TTypeList, template <typename> class TPredicate>
  struct type_list_remove_if
  {
  private:

    ETL_STATIC_ASSERT((etl::is_type_list<TTypeList>::value), "TTypeList must be an etl::type_list");

  public:

    using type = typename private_type_list::type_list_remove_if_impl<TTypeList, TPredicate>::type;
  };

#if ETL_USING_CPP11
  template <typename TTypeList, template <typename> class TPredicate>
  using type_list_remove_if_t = typename etl::type_list_remove_if<TTypeList, TPredicate>::type;
#endif

  //***************************************************************************
  /// Removes the first type from a type_list.
  //***************************************************************************
  template <typename TTypeList>
  struct type_list_pop_front
  {
  private:

    ETL_STATIC_ASSERT((etl::is_type_list<TTypeList>::value), "TTypeList must be an etl::type_list");
    ETL_STATIC_ASSERT(TTypeList::size > 0U,                  "Cannot pop_front from an empty type_list");

  public:

    using type = typename TTypeList::tail;
  };

#if ETL_USING_CPP11
  template <typename TTypeList>
  using type_list_pop_front_t = typename etl::type_list_pop_front<TTypeList>::type;
#endif

  //***************************************************************************
  /// Removes the last type from a type_list.
  //***************************************************************************
  template <typename TTypeList>
  struct type_list_pop_back
  {
  private:

    ETL_STATIC_ASSERT((etl::is_type_list<TTypeList>::value), "TTypeList must be an etl::type_list");
    ETL_STATIC_ASSERT(TTypeList::size > 0U,                  "Cannot pop_back from an empty type_list");

  public:

    using type = typename etl::type_list_remove<TTypeList, TTypeList::size - 1U>::type;
  };

#if ETL_USING_CPP11
  template <typename TTypeList>
  using type_list_pop_back_t = typename etl::type_list_pop_back<TTypeList>::type;
#endif

  //***************************************************************************
  // Remove duplicate types from a type_list, preserving the first occurrence.
  //***************************************************************************
  namespace private_type_list
  {
    template <typename TTypeList, typename TResult>
    struct type_list_unique_impl;

    // Base case: empty list, return the result.
    template <typename TResult>
    struct type_list_unique_impl<etl::type_list<>, TResult>
    {
      using type = TResult;
    };

    // Recursive case: Check if the head is already in the result, if not add it, then recurse on the tail.
    template <typename Head, typename... Tail, typename TResult>
    struct type_list_unique_impl<etl::type_list<Head, Tail...>, TResult>
    {
    private:

      using next_result = etl::conditional_t<etl::type_list_contains<TResult, Head>::value,
                                             TResult,
                                             etl::type_list_append_t<TResult, Head>>;

    public:

      using type = typename type_list_unique_impl<etl::type_list<Tail...>, next_result>::type;
    };
  }

  //***************************************************************************
  /// Defines a new type_list by removing duplicate types from a given type_list, preserving the first occurrence.
  //***************************************************************************
  template <typename TTypeList>
  struct type_list_unique
  {
    ETL_STATIC_ASSERT((etl::is_type_list<TTypeList>::value), "TTypeList must be an etl::type_list");

    using type = typename private_type_list::type_list_unique_impl<TTypeList, etl::type_list<>>::type;
  };

#if ETL_USING_CPP11
  template <typename TTypeList>
  using type_list_unique_t = typename etl::type_list_unique<TTypeList>::type;
#endif

  //***************************************************************************
  /// Checks that all of the types in a type_list are unique.
  //***************************************************************************
  template <typename TTypeList>
  struct type_list_is_unique
    // Create a unique version of the type list, and check if it is the same as the original list.
    // If they are the same, then all types in the original list are unique.
    : etl::bool_constant<std::is_same<TTypeList, typename type_list_unique<TTypeList>::type>::value>
  {
    ETL_STATIC_ASSERT((etl::is_type_list<TTypeList>::value), "TTypeList must be an etl::type_list");
  };

#if ETL_USING_CPP17
  template <typename TTypeList>
  inline constexpr bool type_list_is_unique_v = etl::type_list_is_unique<TTypeList>::value;
#endif

  //***************************************************************************
  /// Checks that all types in a type_list satisfy a unary predicate.
  /// Predicate must be: template <typename T> struct Pred : etl::bool_constant<...> {};
  //***************************************************************************
  template <typename TTypeList, template <typename> class TPredicate>
  struct type_list_all_of;

  template <template <typename> class TPredicate, typename... TTypes>
  struct type_list_all_of<etl::type_list<TTypes...>, TPredicate>
    : etl::conjunction<TPredicate<TTypes>...>
  {
  };

  template <template <typename> class TPredicate>
  struct type_list_all_of<etl::type_list<>, TPredicate>
    : etl::bool_constant<false>
  {
  };

#if ETL_USING_CPP17
  template <typename TTypeList, template <typename> class TPredicate>
  inline constexpr bool type_list_all_of_v = type_list_all_of<TTypeList, TPredicate>::value;
#endif

  //***************************************************************************
  /// Checks that any type in a type_list satisfies a unary predicate.
  /// Predicate must be: template <typename T> struct Pred : etl::bool_constant<...> {};
  //***************************************************************************
  template <typename TTypeList, template <typename> class TPredicate>
  struct type_list_any_of;

  template <template <typename> class TPredicate, typename... TTypes>
  struct type_list_any_of<etl::type_list<TTypes...>, TPredicate>
    : etl::disjunction<TPredicate<TTypes>...>
  {
  };

  template <template <typename> class TPredicate>
  struct type_list_any_of<etl::type_list<>, TPredicate>
    : etl::bool_constant<false>
  {
  };

#if ETL_USING_CPP17
  template <typename TTypeList, template <typename> class TPredicate>
  inline constexpr bool type_list_any_of_v = type_list_any_of<TTypeList, TPredicate>::value;
#endif

  //***************************************************************************
  /// Checks that no types in a type_list satisfy a unary predicate.
  /// Predicate must be: template <typename T> struct Pred : etl::bool_constant<...> {};
  //***************************************************************************
  template <typename TTypeList, template <typename> class TPredicate>
  struct type_list_none_of;

  template <template <typename> class TPredicate, typename... TTypes>
  struct type_list_none_of<etl::type_list<TTypes...>, TPredicate>
    : etl::negation<etl::disjunction<TPredicate<TTypes>...>>
  {
  };

  template <template <typename> class TPredicate>
  struct type_list_none_of<etl::type_list<>, TPredicate>
    : etl::bool_constant<true>
  {
  };

#if ETL_USING_CPP17
  template <typename TTypeList, template <typename> class TPredicate>
  inline constexpr bool type_list_none_of_v = type_list_none_of<TTypeList, TPredicate>::value;
#endif

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

  namespace private_type_list
  {
    //*********************************
    template <typename TTypeList, template <typename, typename> class TCompare>
    struct type_list_is_sorted_impl;

    //*********************************
    // Empty list is sorted
    template <template <typename, typename> class TCompare>
    struct type_list_is_sorted_impl<etl::type_list<>, TCompare>
      : etl::true_type
    {
    };

    //*********************************
    // Single element list is sorted
    template <typename T0, template <typename, typename> class TCompare>
    struct type_list_is_sorted_impl<etl::type_list<T0>, TCompare>
      : etl::true_type
    {
    };

    //*********************************
    // Ensure that the list is sorted.
    // Recursively the head to the next to ensure that the list is sorted.
    template <typename Head, typename Next, typename... Tail, template <typename, typename> class TCompare>
    struct type_list_is_sorted_impl<etl::type_list<Head, Next, Tail...>, TCompare>
      : etl::bool_constant<!TCompare<Next, Head>::value &&
                           type_list_is_sorted_impl<etl::type_list<Next, Tail...>, TCompare>::value>
    {
    };
  }

  //*****************************************************************************
  /// Checks if a type_list is sorted according to TCompare
  /// Static asserts if TTypeList is not an etl::type_list.
  /// Comparator must be: template <typename A, typename B> struct Compare : etl::bool_constant<...> {};
  //*****************************************************************************
  template <typename TTypeList, template <typename, typename> class TCompare>
  struct type_list_is_sorted : public private_type_list::type_list_is_sorted_impl<TTypeList, TCompare>
  {
    ETL_STATIC_ASSERT((etl::is_type_list<TTypeList>::value), "TTypeList must be an etl::type_list");
  };

#if ETL_USING_CPP17
  template <typename TTypeList, template <typename, typename> class TCompare>
  inline constexpr bool type_list_is_sorted_v = etl::type_list_is_sorted<TTypeList, TCompare>::value;
#endif

  //*****************************************************************************
  namespace private_type_list
  {
    //*********************************
    template <bool InsertBefore, typename Head, typename T, template <typename, typename> class TCompare, typename... Tail>
    struct insert_sorted_impl;

    //*********************************
    template <typename TSortedList, typename T, template <typename, typename> class TCompare>
    struct type_list_insert_sorted_impl;

    //*********************************
    // Insert T into sorted list using TCompare
    // Empty list -> T becomes the list
    template <typename T, template <typename, typename> class TCompare>
    struct type_list_insert_sorted_impl<etl::type_list<>, T, TCompare>
    {
      using type = etl::type_list<T>;
    };

    //*********************************
    // Insert T into the correct position in the sorted list, as determined by TCompare
    template <typename Head, typename... Tail, typename T, template <typename, typename> class TCompare>
    struct type_list_insert_sorted_impl<etl::type_list<Head, Tail...>, T, TCompare>
    {
      using type = typename insert_sorted_impl<TCompare<T, Head>::value,
        Head,
        T,
        TCompare,
        Tail...>::type;
    };

    //*********************************
    // If InsertBefore is true, then T should be inserted before Head
    template <typename Head, typename T, template <typename, typename> class TCompare, typename... Tail>
    struct insert_sorted_impl<true, Head, T, TCompare, Tail...>
    {
      using type = etl::type_list<T, Head, Tail...>;
    };

    //*********************************
    // If InsertBefore is false, then T should be inserted after Head, so we recursively call insert_sorted_impl on the tail of the list
    template <typename Head, typename T, template <typename, typename> class TCompare, typename... Tail>
    struct insert_sorted_impl<false, Head, T, TCompare, Tail...>
    {
      using type = etl::type_list_prepend_t<typename type_list_insert_sorted_impl<etl::type_list<Tail...>, T, TCompare>::type, Head>;
    };
  }

  //*****************************************************************************
  /// Insert T into the correct position in the sorted list, as determined by TCompare
  /// Static asserts if TTypeList is not sorted according to TCompare.
  /// Comparator must be: template <typename A, typename B> struct Compare : etl::bool_constant<...> {};
  //*****************************************************************************
  template <typename TTypeList, typename T, template <typename, typename> class TCompare>
  struct type_list_insert_sorted : public private_type_list::type_list_insert_sorted_impl<TTypeList, T, TCompare>
  {
    ETL_STATIC_ASSERT((etl::is_type_list<TTypeList>::value),                  "TTypeList must be an etl::type_list");
    ETL_STATIC_ASSERT((etl::type_list_is_sorted<TTypeList, TCompare>::value), "Cannot insert into a non-sorted type list");
  };

#if ETL_USING_CPP11
  template <typename TTypeList, typename T, template <typename, typename> class TCompare>
  using type_list_insert_sorted_t = typename etl::type_list_insert_sorted<TTypeList, T, TCompare>::type;
#endif

  //***************************************************************************
  namespace private_type_list
  {
    //*********************************
    // Forward declaration
    template <typename TTypeList, template <typename, typename> class TCompare>
    struct type_list_sort_impl;

    //*********************************
    // Empty list
    template <template <typename, typename> class TCompare>
    struct type_list_sort_impl<etl::type_list<>, TCompare>
    {
      using type = etl::type_list<>;
    };

    //*********************************
    // Single element list
    template <typename T0, template <typename, typename> class TCompare>
    struct type_list_sort_impl<etl::type_list<T0>, TCompare>
    {
      using type = etl::type_list<T0>;
    };

    //*********************************
    // Define a new type_list by sorting the types according to TCompare
    template <typename Head, typename... Tail, template <typename, typename> class TCompare>
    struct type_list_sort_impl<etl::type_list<Head, Tail...>, TCompare>
    {
    private:

      using sorted_tail = typename type_list_sort_impl<etl::type_list<Tail...>, TCompare>::type;

    public:

      using type = typename etl::type_list_insert_sorted<sorted_tail, Head, TCompare>::type;
    };
  }

  //*****************************************************************************
  /// etl::type_list sorting by a user-supplied type comparator
  /// Comparator must be: template <typename A, typename B> struct Compare : etl::bool_constant<...> {};
  //*****************************************************************************
  template <typename TTypeList, template <typename, typename> class TCompare>
  struct type_list_sort : public private_type_list::type_list_sort_impl<TTypeList, TCompare>
  {
    ETL_STATIC_ASSERT((etl::is_type_list<TTypeList>::value), "TTypeList must be an etl::type_list");
  };

#if ETL_USING_CPP11
  template <typename TTypeList, template <typename, typename> class TCompare>
  using type_list_sort_t = typename etl::type_list_sort<TTypeList, TCompare>::type;
#endif
}
#endif

#endif
