///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 jwellbelove

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

#ifndef ETL_TO_U16STRING_INCLUDED
#define ETL_TO_U16STRING_INCLUDED

///\ingroup string

#include "platform.h"
#include "type_traits.h"
#include "u16string.h"
#include "u16format_spec.h"
#include "private/to_string_helper.h"

namespace etl
{
  //***************************************************************************
  /// Default format spec.
  //***************************************************************************
  template <typename T>
  const etl::iu16string& to_string(const T value, etl::iu16string& str, const bool append = false)
  {
    etl::u16format_spec format;

    return private_to_string::to_string(value, str, format, append);
  }

  //***************************************************************************
  /// Supplied format spec.
  //***************************************************************************
  template <typename T>
  const etl::iu16string& to_string(const T value, etl::iu16string& str, const etl::u16format_spec& format, const bool append = false)
  {
    return private_to_string::to_string(value, str, format, append);
  }
}

#endif
