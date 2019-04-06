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
#include "format_spec.h"
#include "private/to_string_helper.h"

namespace etl
{
  //***************************************************************************
  /// For signed integrals less than 64 bits. Default format spec.
  //***************************************************************************
  template <typename T>
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_signed<T>::value &&
                          !etl::is_same<T, int64_t>::value, const etl::iu16string&>::type
    to_string(const T value, etl::iu16string& str, const bool append = false)
  {
    etl::format_spec<etl::iu16string> format;

    return to_string_helper(int32_t(value), str, format, append);
  }

  //***************************************************************************
  /// For signed integrals less than 64 bits. Supplied format spec.
  //***************************************************************************
  template <typename T>
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_signed<T>::value &&
                          !etl::is_same<T, int64_t>::value, const etl::iu16string&>::type
    to_string(const T value, etl::iu16string& str, const etl::format_spec<etl::iu16string>& format,  const bool append = false)
  {
    return to_string_helper(int32_t(value), str, format, append);
  }

  //***************************************************************************
  /// For unsigned integrals less then 64 bits. Default format spec.
  //***************************************************************************
  template <typename T>
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_unsigned<T>::value &&
                          !etl::is_same<T, uint64_t>::value, const etl::iu16string&>::type
    to_string(const T value, etl::iu16string& str, const bool append = false)
  {
    etl::format_spec<etl::iu16string> format;

    return to_string_helper(uint32_t(value), str, format, append);
  }

  //***************************************************************************
  /// For unsigned integrals less than 64 bits. Supplied format spec.
  //***************************************************************************
  template <typename T>
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_unsigned<T>::value &&
                          !etl::is_same<T, uint64_t>::value, const etl::iu16string&>::type
    to_string(const T value, etl::iu16string& str, const etl::format_spec<etl::iu16string>& format, const bool append = false)
  {
    return to_string_helper(uint32_t(value), str, format, append);
  }

  //***************************************************************************
  /// For signed 64 bit integrals. Default format spec.
  //***************************************************************************
  template <typename T>
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_signed<T>::value &&
                          etl::is_same<T, int64_t>::value, const etl::iu16string&>::type
    to_string(const T value, etl::iu16string& str, const bool append = false)
  {
    etl::format_spec<etl::iu16string> format;

    return to_string_helper(int64_t(value), str, format, append);
  }

  //***************************************************************************
  /// For signed 64 bit integrals. Supplied format spec.
  //***************************************************************************
  template <typename T>
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_signed<T>::value &&
                          etl::is_same<T, int64_t>::value, const etl::iu16string&>::type
    to_string(const T value, etl::iu16string& str, const etl::format_spec<etl::iu16string>& format, const bool append = false)
  {
    return to_string_helper(int64_t(value), str, format, append);
  }

  //***************************************************************************
  /// For unsigned 64 bit integrals. Default format spec.
  //***************************************************************************
  template <typename T>
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_unsigned<T>::value &&
                          etl::is_same<T, uint64_t>::value, const etl::iu16string&>::type
    to_string(const T value, etl::iu16string& str, const bool append = false)
  {
    etl::format_spec<etl::iu16string> format;

    return to_string_helper(uint64_t(value), str, format, append);
  }

  //***************************************************************************
  /// For unsigned 64 bit integrals. Supplied format spec.
  //***************************************************************************
  template <typename T>
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_unsigned<T>::value &&
                          etl::is_same<T, uint64_t>::value, const etl::iu16string&>::type
    to_string(const T value, etl::iu16string& str, const etl::format_spec<etl::iu16string>& format, const bool append = false)
  {
    return to_string_helper(uint64_t(value), str, format, append);
  }
}

#endif
