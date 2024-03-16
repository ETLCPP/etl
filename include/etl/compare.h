///\file

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

#ifndef ETL_COMPARE_INCLUDED
#define ETL_COMPARE_INCLUDED

#include "platform.h"
#include "parameter_type.h"
#include "functional.h"

//*****************************************************************************
///\defgroup compare compare
/// Comparisons only using less than operator
///\ingroup utilities
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// Defines <=, >, >= in terms of <
  /// Default
  //***************************************************************************
  template <typename T, typename TLess = etl::less<T> >
  struct compare
  {
    typedef typename etl::parameter_type<T>::type first_argument_type;
    typedef typename etl::parameter_type<T>::type second_argument_type;
    typedef bool result_type;

    static result_type lt(first_argument_type lhs, second_argument_type rhs)
    {
      return TLess()(lhs, rhs);
    }

    static result_type gt(first_argument_type lhs, second_argument_type rhs)
    {
      return TLess()(rhs, lhs);
    }

    static result_type lte(first_argument_type lhs, second_argument_type rhs)
    {
      return !gt(lhs, rhs);
    }

    static result_type gte(first_argument_type lhs, second_argument_type rhs)
    {
      return !lt(lhs, rhs);
    }
  };
}

#endif
