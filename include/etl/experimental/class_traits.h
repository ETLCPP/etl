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

#ifndef ETL_CLASS_TRAITS_INCLUDED
#define ETL_CLASS_TRAITS_INCLUDED

#include <stddef.h>
#include <stdint.h>
#include <utility>

#include "platform.h"

#if ETL_CPP11_SUPPORTED

namespace etl
{
#if ETL_CPP11_SUPPORTED
  //***************************************************************************
  /// has_member_function_begin
  //***************************************************************************
  template <typename T>
  class has_begin
  {
    typedef char one;
    struct two { char x[2]; };

    template <typename C> static constexpr one test(decltype(&C::begin)*);
    template <typename C> static constexpr two test(...);

  public:

    static constexpr bool value = (sizeof(test<T>(0)) == sizeof(char));
  };

#if ETL_CPP17_SUPPORTED
  template <typename T>
  static constexpr bool has_begin_v = has_begin<T>::value;
#endif

  //***************************************************************************
  /// has_member_function_end
  //***************************************************************************
  template <typename T>
  class has_end
  {
    typedef char one;
    struct two { char x[2]; };

    template <typename C> static constexpr one test(decltype(std::declval<C>().end()));
    template <typename C> static constexpr two test(...);

  public:

    static constexpr bool value = (sizeof(test<T>(0)) == sizeof(char));
  };

#if ETL_CPP17_SUPPORTED
  template <typename T>
  static constexpr bool has_end_v = hasend<T>::value;
#endif

  //***************************************************************************
  /// has_member_function_size
  //***************************************************************************
  template <typename T>
  class has_size
  {
    typedef char one;
    struct two { char x[2]; };

    template <typename C> static one test(decltype(std::declval<C>().size()));
    template <typename C> static two test(...);

  public:

    static constexpr bool value = (sizeof(test<T>(0)) == sizeof(char));
  };

#if ETL_CPP17_SUPPORTED
  template <typename T>
  static constexpr bool has_size_v = has_size<T>::value;
#endif

  //***************************************************************************
/// has_member_function_max_size
//***************************************************************************
  template <typename T>
  class has_max_size
  {
    typedef char one;
    struct two { char x[2]; };

    template <typename C> static one test(decltype(std::declval<C>().max_size()));
    template <typename C> static two test(...);

  public:

    static constexpr bool value = (sizeof(test<T>(0)) == sizeof(char));
  };

#if ETL_CPP17_SUPPORTED
  template <typename T>
  static constexpr bool has_max_size_v = has_max_size<T>::value;
#endif

  //***************************************************************************
  /// has_member_function_empty
  //***************************************************************************
  template <typename T>
  class has_empty
  {
    typedef char one;
    struct two { char x[2]; };

    template <typename C> static constexpr one test(decltype(std::declval<C>().empty()));
    template <typename C> static constexpr two test(...);

  public:

    static constexpr bool value = (sizeof(test<T>(0)) == sizeof(char));
  };

#if ETL_CPP17_SUPPORTED
  template <typename T>
  static constexpr bool has_empty_v = has_empty<T>::value;
#endif

  //***************************************************************************
  /// has_member_function_data
  //***************************************************************************
  template <typename T>
  class has_data
  {
    typedef char one;
    struct two { char x[2]; };

    template <typename C> static constexpr one test(decltype(std::declval<C>().data()));
    template <typename C> static constexpr two test(...);

  public:

    static constexpr bool value = (sizeof(test<T>(0)) == sizeof(char));
  };

#if ETL_CPP17_SUPPORTED
  template <typename T>
  static constexpr bool has_data_v = has_data<T>::value;
#endif
#endif
}

#endif

#endif // ETL_CLASS_TRAITS_INCLUDED
