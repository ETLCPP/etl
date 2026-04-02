///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#ifndef ETL_LARGEST_INCLUDED
#define ETL_LARGEST_INCLUDED

///\defgroup largest largest
///\ingroup utilities

#include "platform.h"
#include "smallest.h"
#include "static_assert.h"
#include "type_traits.h"

namespace etl
{
#if ETL_USING_CPP11 && !defined(ETL_LARGEST_TYPE_FORCE_CPP03_IMPLEMENTATION)
  //***************************************************************************
  /// Template to determine the largest type and size.
  /// Defines 'value_type' which is the type of the largest parameter.
  /// Defines 'size' which is the size of the largest parameter.
  ///\ingroup largest
  //***************************************************************************
  template <typename T1, typename... TRest>
  class largest_type
  {
  private:

    // Define 'largest_other' as 'largest_type' with all but the first
    // parameter.
    using largest_other = typename largest_type<TRest...>::type;

  public:

    // Set 'type' to be the largest of the first parameter and any of the
    // others. This is recursive.
    using type = typename etl::conditional< (etl::size_of<T1>::value > etl::size_of<largest_other>::value), // Boolean
                                            T1,                                                             // TrueType
                                            largest_other>                                                  // FalseType
      ::type;                                                                                               // The largest type of the two.

    // The size of the largest type.
    enum
    {
      size = etl::size_of<type>::value
    };
  };

  //***************************************************************************
  // Specialisation for one template parameter.
  //***************************************************************************
  template <typename T1>
  class largest_type<T1>
  {
  public:

    using type = T1;

    enum
    {
      size = etl::size_of<type>::value
    };
  };

  #if ETL_USING_CPP11
  template <typename... T>
  using largest_type_t = typename largest_type<T...>::type;
  #endif

  #if ETL_USING_CPP17
  template <typename... T>
  constexpr size_t largest_type_v = largest_type<T...>::size;
  #endif

#else
  #include "private/largest_type_cpp03.h"
#endif

#if ETL_USING_CPP11 && !defined(ETL_LARGEST_ALIGNMENT_FORCE_CPP03_IMPLEMENTATION)
  //***************************************************************************
  /// Template to determine the largest alignment.
  /// Defines <b>value</b> which is the largest alignment of all the parameters.
  ///\ingroup largest
  //***************************************************************************
  template <typename T1, typename... TRest>
  struct largest_alignment
  {
    // Define 'largest_other' as 'largest_type' with all but the first
    // parameter.
    using largest_other = typename largest_alignment<TRest...>::type;

    // Set 'type' to be the largest of the first parameter and any of the
    // others. This is recursive.
    using type = typename etl::conditional< (etl::alignment_of<T1>::value > etl::alignment_of<largest_other>::value), // Boolean
                                            T1,                                                                       // TrueType
                                            largest_other>                                                            // FalseType
      ::type;                                                                                                         // The largest type of the two.

    // The largest alignment.
    enum
    {
      value = etl::alignment_of<type>::value
    };
  };

  //***************************************************************************
  // Specialisation for one template parameter.
  //***************************************************************************
  template <typename T1>
  struct largest_alignment<T1>
  {
    typedef T1 type;

    enum
    {
      value = etl::alignment_of<type>::value
    };
  };

  #if ETL_USING_CPP17
  template <typename... T>
  inline constexpr size_t largest_alignment_v = largest_alignment<T...>::value;
  #endif

#else
  #include "private/largest_alignment_cpp03.h"
#endif

  //***************************************************************************
  /// Defines a type that is as larger or larger than the specified type.
  /// Will return the specified type is there is not a larger type.
  ///\\ingroup largest
  //***************************************************************************
  template <typename T>
  struct larger_int_type
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Must be an integral type");

    typedef typename etl::smallest_int_for_bits< etl::integral_limits<typename etl::make_signed<T>::type>::bits + 1>::type type;
  };

#if ETL_USING_CPP11
  template <typename T>
  using larger_int_type_t = typename larger_int_type<T>::type;
#endif

  //***************************************************************************
  /// Defines a type that is as larger or larger than the specified type.
  /// Will return the specified type is there is not a larger type.
  ///\ingroup largest
  //***************************************************************************
  template <typename T>
  struct larger_uint_type
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Must be an integral type");

    typedef typename etl::smallest_uint_for_bits< etl::integral_limits<typename etl::make_unsigned<T>::type>::bits + 1>::type type;
  };

#if ETL_USING_CPP11
  template <typename T>
  using larger_uint_type_t = typename larger_uint_type<T>::type;
#endif

  //***************************************************************************
  /// Defines a type that is as larger or larger than the specified type.
  /// Will return the specified type is there is not a larger type.
  /// The returned type will be of the same sign.
  ///\ingroup largest
  //***************************************************************************
  template <typename T, bool IS_SIGNED = etl::is_signed<T>::value>
  struct larger_type;

  template <typename T>
  struct larger_type<T, false>
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Must be an integral type");

    typedef typename etl::smallest_uint_for_bits<etl::integral_limits<T>::bits + 1>::type type;
  };

  template <typename T>
  struct larger_type<T, true>
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Must be an integral type");

    typedef typename etl::smallest_int_for_bits<etl::integral_limits<T>::bits + 1>::type type;
  };

#if ETL_USING_CPP11
  template <typename T>
  using larger_type_t = typename larger_type<T>::type;
#endif

#if ETL_USING_CPP11 && !defined(ETL_LARGEST_FORCE_CPP03_IMPLEMENTATION)
  //***************************************************************************
  /// Template to determine the largest type, size and alignment.
  /// Defines <b>value</b> which is the largest type, size and alignment of all
  /// the parameters.
  ///\ingroup largest
  //***************************************************************************
  template <typename... T>
  struct largest
  {
    using type = typename etl::largest_type<T...>::type;

    enum
    {
      size      = etl::largest_type<T...>::size,
      alignment = etl::largest_alignment<T...>::value
    };
  };

  #if ETL_USING_CPP11
  template <typename... T>
  using largest_t = typename largest<T...>::type;
  #endif

  #if ETL_USING_CPP17
  template <typename... T>
  inline constexpr size_t largest_size = largest<T...>::size;
  #endif

#else
  #include "private/largest_cpp03.h"
#endif
} // namespace etl

#endif
