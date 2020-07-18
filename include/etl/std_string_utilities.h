///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 John Wellbelove, John Lagerquist

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

#ifndef ETL_STD_STRING_UTILITIES_INCLUDED
#define ETL_STD_STRING_UTILITIES_INCLUDED

#include "platform.h"

#if ETL_USING_STL

#include "algorithm.h"
#include "private/string_utilities_helper.h"

#include <string>
#include <string_view>

#include <ctype.h>

namespace etl
{
  //***************************************************************************
  /// trim_whitespace_left
  /// Trim left of whitespace
  //***************************************************************************
  inline void trim_whitespace_left(std::string& s)
  {
    etl::private_string_utilities::trim_from_left<std::string>(s, " \t\n\r\f\v");
  }

  //***************************************************************************
  /// trim_whitespace_left
  /// View trim left of whitespace
  //***************************************************************************
  inline std::string_view trim_whitespace_left(const std::string_view& view)
  {
    return etl::private_string_utilities::view_trim_from_left<std::string_view>(view, " \t\n\r\f\v");
  }

  //***************************************************************************
  /// trim_from_left
  /// Trim left of specified characters
  //***************************************************************************
  inline void trim_from_left(std::string& s, std::string::const_pointer trim_characters)
  {
    etl::private_string_utilities::trim_from_left<std::string>(s, trim_characters);
  }

  //***************************************************************************
  /// trim_from_left
  /// View trim left of specified characters
  //***************************************************************************
  inline std::string_view trim_from_left(const std::string_view& view, std::string::const_pointer trim_characters)
  {
    return etl::private_string_utilities::view_trim_from_left<std::string_view>(view, trim_characters);
  }

  //***************************************************************************
  /// trim_left
  /// Trim left, up to, but not including, delimiters.
  //***************************************************************************
  inline void trim_left(std::string& s, std::string::const_pointer delimiters)
  {
    etl::private_string_utilities::trim_left_delimiters<std::string>(s, delimiters);
  }

  //***************************************************************************
  /// trim_left
  /// Trim left, up to, but not including, delimiters.
  //***************************************************************************
  inline std::string_view trim_left(std::string_view& s, std::string_view::const_pointer delimiters)
  {
    return etl::private_string_utilities::view_trim_left_delimiters<std::string_view>(s, delimiters);
  }

  //***************************************************************************
  /// trim_whitespace_right
  /// Trim right of whitespace
  //***************************************************************************
  inline void trim_whitespace_right(std::string& s)
  {
    etl::private_string_utilities::trim_from_right<std::string>(s, " \t\n\r\f\v");
  }

  //***************************************************************************
  /// trim_whitespace_right
  /// Trim right of whitespace
  //***************************************************************************
  inline std::string_view trim_whitespace_right(const std::string_view& view)
  {
    return etl::private_string_utilities::view_trim_from_right<std::string_view>(view, " \t\n\r\f\v");
  }

  //***************************************************************************
  /// trim_from_right
  /// Trim right of specified characters
  //***************************************************************************
  inline void trim_from_right(std::string& s, std::string::const_pointer trim_characters)
  {
    etl::private_string_utilities::trim_from_right<std::string>(s, trim_characters);
  }

  //***************************************************************************
  /// trim_from_right
  /// Trim right of specified characters
  //***************************************************************************
  inline std::string_view trim_from_right(const std::string_view& view, std::string::const_pointer trim_characters)
  {
    return etl::private_string_utilities::view_trim_from_right<std::string_view>(view, trim_characters);
  }

  //***************************************************************************
  /// trim_right
  //***************************************************************************
  inline void trim_right(std::string& s, std::string::const_pointer delimiters)
  {
    etl::private_string_utilities::trim_right_delimiters<std::string>(s, delimiters);
  }

  //***************************************************************************
  /// trim_right
  //***************************************************************************
  inline std::string_view trim_right(const std::string_view& view, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::view_trim_right_delimiters<std::string_view>(view, delimiters);
  }

  //***************************************************************************
  /// trim_whitespace
  /// Trim both ends of whitespace
  //***************************************************************************
  inline void trim_whitespace(std::string& s)
  {
    etl::private_string_utilities::trim_from<std::string>(s, " \t\n\r\f\v");
  }

  //***************************************************************************
  /// trim_whitespace
  /// Trim both ends of whitespace
  //***************************************************************************
  inline std::string_view trim_whitespace(const std::string_view& view)
  {
    return etl::private_string_utilities::view_trim_from<std::string_view>(view, " \t\n\r\f\v");
  }

  //***************************************************************************
  /// trim_from
  /// Trim right of specified characters
  //***************************************************************************
  inline void trim_from(std::string& s, std::string::const_pointer trim_characters)
  {
    etl::private_string_utilities::trim_from<std::string>(s, trim_characters);
  }

  //***************************************************************************
  /// trim_from
  /// Trim right of specified characters
  //***************************************************************************
  inline std::string_view trim_from(const std::string_view& view, std::string::const_pointer trim_characters)
  {
    return etl::private_string_utilities::view_trim_from<std::string_view>(view, trim_characters);
  }

  //***************************************************************************
  /// trim
  //***************************************************************************
  inline void trim(std::string& s, std::string::const_pointer delimiters)
  {
    etl::private_string_utilities::trim_delimiters<std::string>(s, delimiters);
  }

  //***************************************************************************
  /// trim
  //***************************************************************************
  inline std::string_view trim(const std::string_view& view, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::view_trim_delimiters<std::string_view>(view, delimiters);
  }

  //***************************************************************************
  /// reverse
  /// Reverse the string
  //***************************************************************************
  inline void reverse(std::string& s)
  {
    etl::private_string_utilities::reverse<std::string>(s);
  }

  //***************************************************************************
  /// Get up to the first n characters.
  //***************************************************************************
  inline void left_n(std::string& s, size_t n)
  {
    n = (n > s.size()) ? s.size() : n;

    s.erase(s.begin() + n, s.end());
  }

  //***************************************************************************
  /// Get a view of up to the first n characters.
  //***************************************************************************
  inline std::string_view left_n(std::string_view view, size_t n)
  {
    n = (n > view.size()) ? view.size() : n;

    return std::string_view(view.data(), n);
  }

  //***************************************************************************
  /// Get up to the last n characters.
  //***************************************************************************
  inline void right_n(std::string& s, size_t n)
  {
    n = (n > s.size()) ? s.size() : n;

    s.erase(s.begin(), s.end() - n);
  }

  //***************************************************************************
  /// Get a view of up to the last n characters.
  //***************************************************************************
  inline std::string_view right_n(std::string_view view, size_t n)
  {
    n = (n > view.size()) ? view.size() : n;

    return std::string_view(view.data() + view.size() - n, n);
  }

  //***************************************************************************
  /// pad_left
  //***************************************************************************
  inline void pad_left(std::string& s, size_t required_size, std::string::value_type pad_char)
  {
    etl::private_string_utilities::pad_left(s, required_size, pad_char);
  }

  //***************************************************************************
  /// pad_right
  //***************************************************************************
  inline void pad_right(std::string& s, size_t required_size, std::string::value_type pad_char)
  {
    etl::private_string_utilities::pad_right(s, required_size, pad_char);
  }

  //***************************************************************************
  /// pad
  //***************************************************************************
  void pad(std::string& s, size_t required_size, string_pad_direction pad_direction, std::string::value_type pad_char)
  {
    etl::private_string_utilities::pad(s, required_size, pad_direction, pad_char);
  }

  //***************************************************************************
  /// to_upper_case
  //***************************************************************************
  inline void to_upper_case(std::string& s)
  {
    std::string::iterator itr = s.begin();

    while (itr != s.end())
    {
      *itr = std::string::value_type(::toupper(*itr));
      ++itr;
    }
  }

  //***************************************************************************
  /// to_lower_case
  //***************************************************************************
  inline void to_lower_case(std::string& s)
  {
    std::string::iterator itr = s.begin();

    while (itr != s.end())
    {
      *itr = std::string::value_type(::tolower(*itr));
      ++itr;
    }
  }

  //***************************************************************************
  /// to_sentence_case
  //***************************************************************************
  inline void to_sentence_case(std::string& s)
  {
    std::string::iterator itr = s.begin();

    *itr = std::string::value_type(::toupper(*itr));
    ++itr;

    while (itr != s.end())
    {
      *itr = std::string::value_type(::tolower(*itr));
    }
  }

  //***************************************************************************
  /// replace
  //***************************************************************************
  inline void replace(std::string& s,
                      const std::pair<std::string::value_type, std::string::value_type>* pairsbegin, 
                      const std::pair<std::string::value_type, std::string::value_type>* pairsend)
  {
    etl::private_string_utilities::replace_characters<std::string>(s, pairsbegin, pairsend);
  }

  //***************************************************************************
  /// replace
  //***************************************************************************
  inline void replace(std::string& s,
                      const std::pair<const std::string::value_type*, const std::string::value_type*>* pairsbegin,
                      const std::pair<const std::string::value_type*, const std::string::value_type*>* pairsend)
  {
    etl::private_string_utilities::replace_strings<std::string>(s, pairsbegin, pairsend);
  }

  //***************************************************************************
  /// get_token
  //***************************************************************************
  inline std::string_view get_token(const std::string& s, std::string::const_pointer delimiters, const std::string_view& last_view)
  {
    return etl::private_string_utilities::get_token(s, delimiters, last_view);
  }

  //*********************************************************************
  /// Find first of any of delimiters within the string
  //*********************************************************************
  inline std::string::iterator find_first_of(std::string::iterator first, std::string::iterator last, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_first_of(first, last, delimiters);
  }

  //*********************************************************************
  /// Find first of any of delimiters within the string
  //*********************************************************************
  inline std::string::const_iterator find_first_of(std::string::const_iterator first, std::string::const_iterator last, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_first_of(first, last, delimiters);
  }

  //*********************************************************************
  /// Find first of any of delimiters within the string
  //*********************************************************************
  inline std::string::iterator find_first_of(std::string& s, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_first_of(s.begin(), s.end(), delimiters);
  }

  //*********************************************************************
  /// Find first of any of delimiters within the string
  //*********************************************************************
  inline std::string::const_iterator find_first_of(const std::string& s, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_first_of(s.cbegin(), s.cend(), delimiters);
  }

  //*********************************************************************
  /// Find first of any of delimiters within the string
  //*********************************************************************
  inline std::string_view::const_iterator find_first_of(const std::string_view& s, std::string_view::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_first_of(s.cbegin(), s.cend(), delimiters);
  }

  //*********************************************************************
  /// Find first not of delimiters within the string
  //*********************************************************************
  inline std::string::iterator find_first_not_of(std::string::iterator first, std::string::iterator last, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_first_not_of(first, last, delimiters);
  }

  //*********************************************************************
  /// Find first not of delimiters within the string
  //*********************************************************************
  inline std::string::const_iterator find_first_not_of(std::string::const_iterator first, std::string::const_iterator last, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_first_not_of(first, last, delimiters);
  }

  //*********************************************************************
  /// Find first not of delimiters within the string
  //*********************************************************************
  inline std::string::iterator find_first_not_of(std::string& s, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_first_not_of(s.begin(), s.end(), delimiters);
  }

  //*********************************************************************
  /// Find first not of delimiters within the string
  //*********************************************************************
  inline std::string::const_iterator find_first_not_of(const std::string& s, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_first_not_of(s.cbegin(), s.cend(), delimiters);
  }

  //*********************************************************************
  /// Find first not of delimiters within the string
  //*********************************************************************
  inline std::string_view::const_iterator find_first_not_of(const std::string_view& view, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_first_not_of(view.cbegin(), view.cend(), delimiters);
  }

  //*********************************************************************
  /// Find last of any of delimiters within the string
  //*********************************************************************
  inline std::string::iterator find_last_of(std::string::iterator first, std::string::iterator last, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_last_of(first, last, delimiters);
  }

  //*********************************************************************
  /// Find last of any of delimiters within the string
  //*********************************************************************
  inline std::string::const_iterator find_last_of(std::string::const_iterator first, std::string::const_iterator last, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_last_of(first, last, delimiters);
  }

  //*********************************************************************
  /// Find last of any of delimiters within the string
  //*********************************************************************
  inline std::string::iterator find_last_of(std::string& s, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_last_of(s.begin(), s.end(), delimiters);
  }

  //*********************************************************************
  /// Find last of any of delimiters within the string
  //*********************************************************************
  inline std::string::const_iterator find_last_of(const std::string& s, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_last_of(s.cbegin(), s.cend(), delimiters);
  }

  //*********************************************************************
  /// Find last of any of delimiters within the string
  //*********************************************************************
  inline std::string_view::const_iterator find_last_of(const std::string_view& view, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_last_of(view.cbegin(), view.cend(), delimiters);
  }

  //*********************************************************************
  /// Find last of any of delimiters within the string
  //*********************************************************************
  inline std::string::iterator find_last_not_of(std::string::iterator first, std::string::iterator last, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_last_not_of(first, last, delimiters);
  }

  //*********************************************************************
  /// Find last of any of delimiters within the string
  //*********************************************************************
  inline std::string::const_iterator find_last_not_of(std::string::const_iterator first, std::string::const_iterator last, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_last_not_of(first, last, delimiters);
  }

  //*********************************************************************
  /// Find last of any of delimiters within the string
  //*********************************************************************
  inline std::string::iterator find_last_not_of(std::string& s, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_last_not_of(s.begin(), s.end(), delimiters);
  }

  //*********************************************************************
  /// Find last of any of delimiters within the string
  //*********************************************************************
  inline std::string::const_iterator find_last_not_of(const std::string& s, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_last_not_of(s.cbegin(), s.cend(), delimiters);
  }

  //*********************************************************************
  /// Find last of any of delimiters within the string
  //*********************************************************************
  inline std::string_view::const_iterator find_last_not_of(const std::string_view& view, std::string::const_pointer delimiters)
  {
    return etl::private_string_utilities::find_last_not_of(view.cbegin(), view.cend(), delimiters);
  }
}

#endif

#endif