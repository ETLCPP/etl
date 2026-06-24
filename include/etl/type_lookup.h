/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

#ifndef ETL_TYPE_LOOKUP_INCLUDED
#define ETL_TYPE_LOOKUP_INCLUDED

#include "platform.h"
#include "integral_limits.h"
#include "null_type.h"
#include "type_traits.h"

#include <stddef.h>

namespace etl
{
  //***************************************************************************
  /// The type/id pair type to use for type/id lookup template parameters.
  //***************************************************************************
  template <typename T, int ID_>
  struct type_id_pair
  {
    typedef T type;

    enum
    {
      Id = ID_
    };
  };

  //***************************************************************************
  /// The type/type pair type to use for type/type lookup template parameters.
  //***************************************************************************
  template <typename T1, typename T2>
  struct type_type_pair
  {
    typedef T1 type1;
    typedef T2 type2;
  };

#if ETL_USING_CPP11 && !defined(ETL_TYPE_SELECT_FORCE_CPP03_IMPLEMENTATION)
  //***************************************************************************
  // type_id_lookup
  //***************************************************************************
  template <typename... TTypes>
  struct type_id_lookup
  {
  private:

    // The type for no match.
    struct nulltype
    {
    };

    // For N type pairs.
    template <int Id, typename T1, typename... TRest>
    struct type_from_id_helper
    {
      using type = typename etl::conditional< Id == T1::Id, typename T1::type, typename type_from_id_helper<Id, TRest...>::type>::type;
    };

    // Specialisation for 1 type pair.
    template <int Id, typename T1>
    struct type_from_id_helper<Id, T1>
    {
      using type = typename etl::conditional<Id == T1::Id, typename T1::type, nulltype>::type;
    };

  public:

    //************************************
    // type_from_id
    //************************************
    template <int Id>
    struct type_from_id
    {
      using type = typename type_from_id_helper<Id, TTypes...>::type;

      static_assert(!(etl::is_same<nulltype, type>::value), "Invalid id");
    };

    template <int Id>
    using type_from_id_t = typename type_from_id<Id>::type;

  private:

    static constexpr size_t UNKNOWN = etl::integral_limits<size_t>::max;

    // For N type pairs.
    template <typename T, typename T1, typename... TRest>
    struct id_from_type_helper
    {
      static constexpr size_t value = etl::is_same<T, typename T1::type>::value ? size_t(T1::Id) : id_from_type_helper<T, TRest...>::value;
    };

    // Specialisation for 1 type pair.
    template <typename T, typename T1>
    struct id_from_type_helper<T, T1>
    {
      static constexpr size_t value = etl::is_same<T, typename T1::type>::value ? size_t(T1::Id) : UNKNOWN;
    };

  public:

    //************************************
    // id_from_type
    //************************************
    template <typename T>
    struct id_from_type
    {
      static constexpr size_t value = id_from_type_helper<T, TTypes...>::value;

      static_assert(value != UNKNOWN, "Invalid type");
    };

  #if ETL_USING_CPP17
    template <typename T>
    static constexpr size_t id_from_type_v = id_from_type<T>::value;
  #endif

    //************************************
    template <typename T>
    static unsigned int get_id_from_type(const T&)
    {
      return get_id_from_type<T>();
    }

    //************************************
    template <typename T>
    static unsigned int get_id_from_type()
    {
      return id_from_type<T>::value;
    }
  };

  //***************************************************************************
  // type_type_lookup
  //***************************************************************************
  template <typename... TTypes>
  class type_type_lookup
  {
  private:

    // The type for no match.
    struct nulltype
    {
    };

    template <typename T, typename T1, typename... TRest>
    struct type_from_type_helper
    {
      using type = typename etl::conditional< etl::is_same<T, typename T1::type1>::value, typename T1::type2,
                                              typename type_from_type_helper<T, TRest...>::type>::type;
    };

    template <typename T, typename T1>
    struct type_from_type_helper<T, T1>
    {
      using type = typename etl::conditional<etl::is_same<T, typename T1::type1>::value, typename T1::type2, nulltype>::type;
    };

  public:

    template <typename T>
    class type_from_type
    {
    public:

      // The matched type or nulltype
      using type = typename type_from_type_helper<T, TTypes...>::type;

      static_assert(!etl::is_same<type, nulltype>::value, "Type match not found");
    };

    // Template alias.
    template <typename T>
    using type_from_type_t = typename type_from_type<T>::type;
  };

#else
  #include "private/type_lookup_cpp03.h"
#endif
} // namespace etl

#endif
