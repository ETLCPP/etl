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

#ifndef ETL_STRING_UTILITIES_INCLUDED
#define ETL_STRING_UTILITIES_INCLUDED

#include "platform.h"
#include "cstring.h"
#include "string_view.h"
#include "algorithm.h"
#include "private/string_utilities_helper.h"

#include <ctype.h>

namespace etl
{
  //***************************************************************************
  /// trim_whitespace_left
  /// Trim left of whitespace
  //***************************************************************************
  inline void trim_whitespace_left(etl::istring& s)
  {
    etl::private_string_utilities::trim_from_left<etl::istring>(s, " \t\n\r\f\v");
  }

  //***************************************************************************
  /// trim_whitespace_left
  /// View trim left of whitespace
  //***************************************************************************
  inline etl::string_view trim_whitespace_left(const etl::string_view& view)
  {
    return etl::private_string_utilities::view_trim_from_left<etl::string_view>(view, " \t\n\r\f\v");
  }

  //***************************************************************************
  /// trim_from_left
  /// Trim left of specified characters
  //***************************************************************************
  inline void trim_from_left(etl::istring& s, etl::istring::const_pointer trim_characters)
  {
    etl::private_string_utilities::trim_from_left<etl::istring>(s, trim_characters);
  }

  //***************************************************************************
  /// trim_from_left
  /// View trim left of specified characters
  //***************************************************************************
  inline etl::string_view trim_from_left(const etl::string_view& view, etl::istring::const_pointer trim_characters)
  {
    return etl::private_string_utilities::view_trim_from_left<etl::string_view>(view, trim_characters);
  }

  //***************************************************************************
  /// trim_from_left
  /// Trim left of specified characters
  //***************************************************************************
  inline void trim_from_left(etl::istring& s, etl::istring::const_pointer trim_characters, size_t length)
  {
    etl::private_string_utilities::trim_from_left<etl::istring>(s, trim_characters, length);
  }

  //***************************************************************************
  /// trim_from_left
  /// View trim left of specified characters
  //***************************************************************************
  inline etl::string_view trim_from_left(const etl::string_view& view, etl::istring::const_pointer trim_characters, size_t length)
  {
    return etl::private_string_utilities::view_trim_from_left<etl::string_view>(view, trim_characters, length);
  }

  //***************************************************************************
  /// trim_from_left
  /// Trim left of specified characters
  //***************************************************************************
  inline void trim_from_left(etl::istring& s, const etl::istring& trim_characters)
  {
    etl::private_string_utilities::trim_from_left<etl::istring>(s, trim_characters.c_str());
  }

  //***************************************************************************
  /// trim_from_left
  /// View trim left of specified characters
  //***************************************************************************
  inline etl::string_view trim_from_left(const etl::string_view& view, const etl::istring& trim_characters)
  {
    return etl::private_string_utilities::view_trim_from_left<etl::string_view>(view, trim_characters.c_str());
  }

  //***************************************************************************
  /// trim_left
  /// Trim left, up to, but not including, delimiters.
  //***************************************************************************
  inline void trim_left(etl::istring& s, etl::istring::const_pointer delimiters)
  {
    etl::private_string_utilities::trim_left_delimiters<etl::istring>(s, delimiters);
  }

  //***************************************************************************
  /// trim_left
  /// Trim left, up to, but not including, delimiters.
  //***************************************************************************
  inline etl::string_view trim_left(etl::string_view& s, etl::string_view::const_pointer delimiters)
  {
    return etl::private_string_utilities::view_trim_left_delimiters<etl::string_view>(s, delimiters);
  }

  //***************************************************************************
  /// trim_left
  /// Trim left, up to, but not including, delimiters.
  //***************************************************************************
  inline void trim_left(etl::istring& s, etl::istring::const_pointer delimiters, size_t length)
  {
    etl::private_string_utilities::trim_left_delimiters<etl::istring>(s, delimiters, length);
  }

  //***************************************************************************
  /// trim_left
  /// Trim left, up to, but not including, delimiters.
  //***************************************************************************
  inline etl::string_view trim_left(const etl::string_view& view, etl::string_view::const_pointer delimiters, size_t length)
  {
    return etl::private_string_utilities::view_trim_left_delimiters<etl::string_view>(view, delimiters, length);
  }

  //***************************************************************************
  /// trim_left
  /// Trim left, up to, but not including, delimiters.
  //***************************************************************************
  inline void trim_left(etl::istring& s, const etl::istring& delimiters)
  {
    etl::private_string_utilities::trim_left_delimiters<etl::istring>(s, delimiters.c_str());
  }

  //***************************************************************************
  /// trim_left
  /// Trim left, up to, but not including, delimiters.
  //***************************************************************************
  inline etl::string_view trim_left(const etl::string_view& view, const etl::istring& delimiters)
  {
    return etl::private_string_utilities::view_trim_left_delimiters<etl::string_view>(view, delimiters.c_str());
  }

  //*********************************************************************************************************************************************************

  //***************************************************************************
  /// trim_whitespace_right
  /// Trim right of whitespace
  //***************************************************************************
  inline void trim_whitespace_right(etl::istring& s)
  {
    etl::private_string_utilities::trim_from_right<etl::istring>(s, " \t\n\r\f\v");
  }

  //***************************************************************************
  /// trim_whitespace_right
  /// Trim right of whitespace
  //***************************************************************************
  inline etl::string_view trim_whitespace_right(const etl::string_view& view)
  {
    return etl::private_string_utilities::view_trim_from_right<etl::string_view>(view, " \t\n\r\f\v");
  }

  //***************************************************************************
  /// trim_from_right
  /// Trim right of specified characters
  //***************************************************************************
  inline void trim_from_right(etl::istring& s, etl::istring::const_pointer trim_characters)
  {
    etl::private_string_utilities::trim_from_right<etl::istring>(s, trim_characters);
  }

  //***************************************************************************
  /// trim_from_right
  /// Trim right of specified characters
  //***************************************************************************
  inline etl::string_view trim_from_right(const etl::string_view& view, etl::istring::const_pointer trim_characters)
  {
    return etl::private_string_utilities::view_trim_from_right<etl::string_view>(view, trim_characters);
  }

  //***************************************************************************
  /// trim_from_right
  /// Trim right of specified characters
  //***************************************************************************
  inline void trim_from_right(etl::istring& s, etl::istring::const_pointer trim_characters, size_t length)
  {
    etl::private_string_utilities::trim_from_right<etl::istring>(s, trim_characters, length);
  }

  //***************************************************************************
  /// trim_from_right
  /// Trim right of specified characters
  //***************************************************************************
  inline etl::string_view trim_from_right(const etl::string_view& view, etl::istring::const_pointer trim_characters, size_t length)
  {
    return etl::private_string_utilities::view_trim_from_right<etl::string_view>(view, trim_characters, length);
  }

  //***************************************************************************
  /// trim_from_right
  /// Trim right of specified characters
  //***************************************************************************
  inline void trim_from_right(etl::istring& s, const etl::istring& trim_characters)
  {
    etl::private_string_utilities::trim_from_right<etl::istring>(s, trim_characters.c_str());
  }

  //***************************************************************************
  /// trim_from_right
  /// Trim right of specified characters
  //***************************************************************************
  inline etl::string_view trim_from_right(const etl::string_view& view, const etl::istring& trim_characters)
  {
    return etl::private_string_utilities::view_trim_from_right<etl::string_view>(view, trim_characters.c_str());
  }

  //***************************************************************************
  /// trim_right
  //***************************************************************************
  inline void trim_right(etl::istring& s, etl::istring::const_pointer delimiters)
  {
    etl::private_string_utilities::trim_right_delimiters<etl::istring>(s, delimiters);
  }

  //***************************************************************************
  /// trim_right
  //***************************************************************************
  inline etl::string_view trim_right(const etl::string_view& view, etl::istring::const_pointer delimiters)
  {
    return etl::private_string_utilities::view_trim_right_delimiters<etl::string_view>(view, delimiters);
  }

  //***************************************************************************
  /// trim_right
  /// Trim right, up to, but not including, delimiters.
  //***************************************************************************
  inline void trim_right(etl::istring& s, etl::istring::const_pointer delimiters, size_t length)
  {
    etl::private_string_utilities::trim_right_delimiters<etl::istring>(s, delimiters, length);
  }

  //***************************************************************************
  /// trim_right
  /// Trim right, up to, but not including, delimiters.
  //***************************************************************************
  inline etl::string_view trim_right(const etl::string_view& view, etl::istring::const_pointer delimiters, size_t length)
  {
    etl::private_string_utilities::view_trim_right_delimiters<etl::string_view>(view, delimiters, length);
  }

  //***************************************************************************
  /// trim_right
  /// Trim right, up to, but not including, delimiters.
  //***************************************************************************
  inline void trim_right(etl::istring& s, const etl::istring& delimiters)
  {
    etl::private_string_utilities::trim_right_delimiters<etl::istring>(s, delimiters.c_str());
  }

  //***************************************************************************
  /// trim_right
  /// Trim right, up to, but not including, delimiters.
  //***************************************************************************
  inline etl::string_view trim_right(const etl::string_view& view, const etl::istring& delimiters)
  {
    return etl::private_string_utilities::view_trim_right_delimiters<etl::string_view>(view, delimiters.c_str());
  }

  //*********************************************************************************************************************************************************

  //***************************************************************************
  /// trim_whitespace
  /// Trim both ends of whitespace
  //***************************************************************************
  inline void trim_whitespace(etl::istring& s)
  {
    etl::private_string_utilities::trim_from<etl::istring>(s, " \t\n\r\f\v");
  }

  //***************************************************************************
  /// trim_whitespace
  /// Trim both ends of whitespace
  //***************************************************************************
  inline etl::string_view trim_whitespace(const etl::string_view& view)
  {
    return etl::private_string_utilities::view_trim_from<etl::string_view>(view, " \t\n\r\f\v");
  }

  //***************************************************************************
  /// trim_from
  /// Trim right of specified characters
  //***************************************************************************
  inline void trim_from(etl::istring& s, etl::istring::const_pointer trim_characters)
  {
    etl::private_string_utilities::trim_from<etl::istring>(s, trim_characters);
  }

  //***************************************************************************
  /// trim_from
  /// Trim right of specified characters
  //***************************************************************************
  inline etl::string_view trim_from(const etl::string_view& view, etl::istring::const_pointer trim_characters)
  {
    return etl::private_string_utilities::view_trim_from<etl::string_view>(view, trim_characters);
  }

  //***************************************************************************
  /// trim_from
  /// Trim right of specified characters
  //***************************************************************************
  inline void trim_from(etl::istring& s, etl::istring::const_pointer trim_characters, size_t length)
  {
    etl::private_string_utilities::trim_from<etl::istring>(s, trim_characters, length);
  }

  //***************************************************************************
  /// trim_from
  /// Trim right of specified characters
  //***************************************************************************
  inline etl::string_view trim_from(const etl::string_view& view, etl::istring::const_pointer trim_characters, size_t length)
  {
    return etl::private_string_utilities::view_trim_from<etl::string_view>(view, trim_characters, length);
  }

  //***************************************************************************
  /// trim_from
  /// Trim right of specified characters
  //***************************************************************************
  inline void trim_from(etl::istring& s, const etl::istring& trim_characters)
  {
    etl::private_string_utilities::trim_from<etl::istring>(s, trim_characters.c_str());
  }

  //***************************************************************************
  /// trim_from
  /// Trim right of specified characters
  //***************************************************************************
  inline etl::string_view trim_from(const etl::string_view& view, const etl::istring& trim_characters)
  {
    return etl::private_string_utilities::view_trim_from<etl::string_view>(view, trim_characters.c_str());
  }

  //***************************************************************************
  /// trim
  //***************************************************************************
  inline void trim(etl::istring& s, etl::istring::const_pointer delimiters)
  {
    etl::private_string_utilities::trim_delimiters<etl::istring>(s, delimiters);
  }

  //***************************************************************************
  /// trim
  //***************************************************************************
  inline etl::string_view trim(const etl::string_view& view, etl::istring::const_pointer delimiters)
  {
    return etl::private_string_utilities::view_trim_delimiters<etl::string_view>(view, delimiters);
  }

  //***************************************************************************
  /// trim
  /// Trim right, up to, but not including, delimiters.
  //***************************************************************************
  inline void trim(etl::istring& s, etl::istring::const_pointer delimiters, size_t length)
  {
    etl::private_string_utilities::trim_delimiters<etl::istring>(s, delimiters, length);
  }

  //***************************************************************************
  /// trim
  /// Trim right, up to, but not including, delimiters.
  //***************************************************************************
  inline etl::string_view trim(const etl::string_view& view, etl::istring::const_pointer delimiters, size_t length)
  {
    etl::private_string_utilities::view_trim_delimiters<etl::string_view>(view, delimiters, length);
  }

  //***************************************************************************
  /// trim
  /// Trim right, up to, but not including, delimiters.
  //***************************************************************************
  inline void trim(etl::istring& s, const etl::istring& delimiters)
  {
    etl::private_string_utilities::trim_delimiters<etl::istring>(s, delimiters.c_str());
  }

  //***************************************************************************
  /// trim
  /// Trim right, up to, but not including, delimiters.
  //***************************************************************************
  inline etl::string_view trim(const etl::string_view& view, const etl::istring& delimiters)
  {
    return etl::private_string_utilities::view_trim_delimiters<etl::string_view>(view, delimiters.c_str());
  }

  //***************************************************************************
  /// reverse
  /// Reverse the string
  //***************************************************************************
  void reverse(etl::istring& s)
  {
    etl::private_string_utilities::reverse<etl::istring>(s);
  }

  //***************************************************************************
  /// Get a view of up to the first n characters.
  //***************************************************************************
  etl::string_view left(const etl::istring& s, size_t n)
  {
    n = (n > s.size()) ? s.size() : n;

    return etl::string_view(s.begin(), s.begin() + n);
  }

  //***************************************************************************
  /// Get a view of up to the last n characters. 
  //***************************************************************************
  etl::string_view right(const etl::istring& s, size_t n)
  {
    n = (n > s.size()) ? s.size() : n;

    return etl::string_view(s.end() - n, s.end());
  }

  //template<size_t max_len, size_t max_num_tokens>
  //void tokenize(etl::string<max_len> const& s, etl::vector<etl::string<max_len>, max_num_tokens>& tokens, char const* delims, bool ignore_empty_strings)
  //{
  //  size_t delim;
  //  size_t O = 0;

  //  tokens.clear();

  //  delim = s.find_first_of(delims, O);

  //  while (delim != etl::string<max_len>::npos)
  //  {
  //    etl::string<max_len> tok(s.substr(O, delim - O));
  //    trim(tok);
  //    if (!tok.empty() || !ignore_empty_strings)
  //      tokens.push_back(tok);
  //    O = delim + 1;

  //    delim = s.find_first_of(delims, O);
  //  }
  //  etl::string<max_len> tok(s.substr(O));
  //  trim(tok);
  //  if (!tok.empty() || !ignore_empty_strings)
  //    tokens.push_back(tok);
  //}

  //template<size_t max_len, size_t max_token_len, size_t max_num_tokens>
  //void tokenize(etl::string<max_len> const& s, etl::vector<etl::string<max_token_len>, max_num_tokens>& tokens, char const* delims, bool ignore_empty_strings)
  //{
  //  size_t delim;
  //  size_t O = 0;

  //  tokens.clear();

  //  delim = s.find_first_of(delims, O);

  //  while (delim != etl::string<max_len>::npos)
  //  {
  //    etl::string<max_token_len> tok(s.substr(O, delim - O).c_str());
  //    trim(tok);
  //    if (!tok.empty() || !ignore_empty_strings)
  //      tokens.push_back(tok);
  //    O = delim + 1;

  //    delim = s.find_first_of(delims, O);
  //  }
  //  etl::string<max_token_len> tok(s.substr(O).c_str());
  //  trim(tok);
  //  if (!tok.empty() || !ignore_empty_strings)
  //    tokens.push_back(tok);
  //}

  //template<size_t max_len>
  //etl::string<max_len> substr_between(etl::string<max_len> const& s, const etl::string<max_len> brackets)
  //{
  //  size_t l = s.find_first_of(brackets[0]);
  //  size_t r = s.find_last_of(brackets[1]);

  //  if (l != etl::string<max_len>::npos && r != etl::string<max_len>::npos && r - l > 1)
  //    return s.substr(l + 1, r - l - 1);
  //  return etl::string<max_len>();
  //}

  //enum pad_dir_t
  //{
  //  pad_dir__right,
  //  pad_dir__left
  //};

  //template<size_t max_len>
  //void pad(etl::string<max_len>& s, size_t size, pad_dir_t dir, char pad_char)
  //{
  //  if (size > s.size())
  //  {
  //    size -= s.size();

  //    if (dir == pad_dir__left)
  //    {
  //      reverse(s);
  //      s.reserve(s.size() + size);
  //      while (size--)
  //        s.push_back(pad_char);
  //      reverse(s);
  //    }
  //    else
  //    {
  //      s.reserve(s.size() + size);
  //      while (size--)
  //        s.push_back(pad_char);
  //    }
  //  }
  //}

  //***************************************************************************
  /// to_upper_case
  //***************************************************************************
  void to_upper_case(etl::istring& s)
  {
    etl::istring::iterator itr = s.begin();

    while (itr != s.end())
    {
      *itr = etl::istring::value_type(::toupper(*itr));
      ++itr;
    }
  }

  //***************************************************************************
  /// to_lower_case
  //***************************************************************************
  void to_lower_case(etl::istring& s)
  {
    etl::istring::iterator itr = s.begin();

    while (itr != s.end())
    {
      *itr = etl::istring::value_type(::tolower(*itr));
      ++itr;
    }
  }

  //***************************************************************************
  /// to_sentence_case
  //***************************************************************************
  void to_sentence_case(etl::istring& s)
  {
    etl::istring::iterator itr = s.begin();

    *itr = etl::istring::value_type(::toupper(*itr));
    ++itr;

    while (itr != s.end())
    {
      *itr = etl::istring::value_type(::tolower(*itr));
    }
  }

  //***************************************************************************
  /// transform
  //***************************************************************************
  void transform(etl::istring& s, 
                 const etl::pair<etl::istring::value_type, etl::istring::value_type>* pairsbegin, 
                 const etl::pair<etl::istring::value_type, etl::istring::value_type>* pairsend)
  {
    etl::private_string_utilities::transform_characters<etl::istring>(s, pairsbegin, pairsend);
  }

  //***************************************************************************
  /// transform
  //***************************************************************************
  void transform(etl::istring& s,
                 const etl::pair<const etl::istring::value_type*, const etl::istring::value_type*>* pairsbegin,
                 const etl::pair<const etl::istring::value_type*, const etl::istring::value_type*>* pairsend)
  {
    etl::private_string_utilities::transform_strings<etl::istring>(s, pairsbegin, pairsend);
  }
}

#endif