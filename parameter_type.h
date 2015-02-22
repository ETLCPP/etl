///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

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

#ifndef __ETL_PARAMETER__
#define __ETL_PARAMETER__

#include "type_traits.h"

namespace etl
{
  //*************************************************************************
  /// Determine how to pass parameters.
  //*************************************************************************
  template <typename T, const bool should_pass_by_value = is_fundamental<T>::value || is_pointer<T>::value>
  struct parameter_type;

  //*************************************************************************
  /// Pass by value.
  //*************************************************************************
  template <typename T>
  struct parameter_type<T, true>
  {
    typedef T type;
  };

  //*************************************************************************
  /// Pass by const reference.
  //*************************************************************************
  template <typename T>
  struct parameter_type<T, false>
  {
    typedef const T& type;
  };
}

#endif
