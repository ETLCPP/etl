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

#ifndef ETL_STRING_UTILITIES_HELPER_INCLUDED
#define ETL_STRING_UTILITIES_HELPER_INCLUDED

#include "../platform.h"
#include "../basic_string.h"
#include "../string_view.h"
#include "../algorithm.h"

#include <stdint.h>

namespace etl
{
  namespace private_string_utilities
  {
    //***************************************************************************
    /// trim_left
    /// Trim left of whitespace
    //***************************************************************************
    template <typename TIString>
    void trim_left_of(TIString& s, typename TIString::const_pointer trim_characters)
    {
      size_t position = s.find_first_not_of(trim_characters);
      s.erase(0U, position);
    }

    //***************************************************************************
    /// trim_left
    /// Trim left, up to, but not including, delimiters.
    //***************************************************************************
    template <typename TIString>
    void trim_left_delimiters(TIString& s, typename TIString::const_pointer delimiters)
    {
      size_t p = s.find_first_of(delimiters);

      if (p != TIString::npos)
      {
        s.erase(0, p);
      }
    }

    //***************************************************************************
    /// trim_left
    /// Trim left, up to, but not including, delimiters.
    //***************************************************************************
    template <typename TIString>
    void trim_left_delimiters(TIString& s, typename TIString::const_pointer delimiters, size_t length)
    {
      size_t p = s.find_first_of(delimiters, 0, length);

      if (p != TIString::npos)
      {
        s.erase(0, p);
      }
    }

    //***************************************************************************
    /// trim_left
    /// Trim left, up to, but not including, delimiters.
    //***************************************************************************
    template <typename TIString>
    void trim_left_delimiters(TIString& s, const TIString& delimiters)
    {
      trim_left_delimiters(s, delimiters.c_str());
    }

    //***************************************************************************
    /// trim_right
    /// Trim right of whitespace
    //***************************************************************************
    template <typename TIString>
    void trim_right_of(TIString& s, typename TIString::const_pointer trim_characters)
    {
      s.erase(s.find_last_not_of(trim_characters) + 1);
    }

    //***************************************************************************
    /// trim_right
    //***************************************************************************
    template <typename TIString>
    void trim_right_delimiters(TIString& s, typename TIString::const_pointer delimiters)
    {
      size_t p = s.find_last_of(delimiters);

      if (p != TIString::npos)
      {
        ++p;

        if (p != s.size())
        {
          s.erase(p);
        }
      }
    }

    //***************************************************************************
    /// trim_right
    /// Trim right, up to, but not including, delimiters.
    //***************************************************************************
    template <typename TIString>
    void trim_right_delimiters(TIString& s, typename TIString::const_pointer delimiters, size_t length)
    {
      size_t p = s.find_last_of(delimiters, TIString::npos, length);

      if (p != TIString::npos)
      {
        ++p;

        if (p != s.size())
        {
          s.erase(p);
        }
      }
    }

    //***************************************************************************
    /// trim_right
    /// Trim right, up to, but not including, delimiters.
    //***************************************************************************
    template <typename TIString>
    void trim_right_delimiters(TIString& s, const TIString& delimiters)
    {
      trim_right(s, delimiters.c_str());
    }

    //***************************************************************************
    /// trim
    /// Trim left and right of whitespace
    //***************************************************************************
    template <typename TIString>
    void trim_of(TIString& s, typename TIString::const_pointer trim_characters)
    {
      trim_left_of(s, trim_characters);
      trim_right_of(s, trim_characters);
    }

    ////***************************************************************************
    ///// trim_view
    ///// return a string view trimmed left and right of whitespace
    ////***************************************************************************
    //static TStringView trim_view(TIString& s)
    //{
    //  size_t first = s.find_first_not_of(whitespace<value_type>());
    //  size_t last  = s.find_last_not_of(whitespace<value_type>());

    //  typename TIString::const_pointer pbegin;
    //  typename TIString::const_pointer pend;

    //  if (first == TIString::npos)
    //  {
    //    pbegin = s.end();
    //  }
    //  else
    //  {
    //    pbegin = s.c_str() + first;
    //  }

    //  if (last == TIString::npos)
    //  {
    //    pend = s.end();
    //  }
    //  else
    //  {
    //    pend = s.c_str() + last + 1;
    //  }

    //  return TStringView(pbegin, pend);
    //}

    ////***************************************************************************
    ///// reverse
    ///// Reverse a string
    ////***************************************************************************
    //void reverse(TIString& s)
    //{
    //  etl::reverse(s.begin(), s.end());
    //}

    ////***************************************************************************
    ///// 
    ////***************************************************************************
    //template<size_t max_len>
    //etl::string<max_len> left(etl::string<max_len> const& s, size_t n_chars)
    //{
    //  return s.substr(0, n_chars);
    //}

    ////***************************************************************************
    ///// 
    ////***************************************************************************
    //template<size_t max_len>
    //etl::string<max_len> right(etl::string<max_len> const& s, size_t start)
    //{
    //  return s.substr(start);
    //}

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
    //float to_float(etl::string<max_len> const& s)
    //{
    //  return strtof(s.c_str(), 0);
    //}

    //template<size_t max_len>
    //unsigned to_unsigned(etl::string<max_len> const& s, int radix)
    //{
    //  return strtoul(s.c_str(), 0, radix);
    //}

    //template<size_t max_len>
    //int to_int(etl::string<max_len> const& s, int radix)
    //{
    //  return strtol(s.c_str(), 0, radix);
    //}

    //template<size_t max_len>
    //bool to_bool(etl::string<max_len> const& s)
    //{
    //  return s[0] == 't' || s[0] == 'T';
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

    //template<size_t max_len>
    //int array_subscript(etl::string<max_len> const& s)
    //{
    //  etl::string<max_len> n = substr_between(s);
    //  return n.empty() ? -1 : to_int(n);
    //}

    //template<size_t max_len>
    //etl::string<max_len> from_float(float f, int digits)
    //{
    //  char buf[32];
    //  char format[] = { '%','9','.',char('0' + char(digits)),'f',0 };
    //  sprintf(buf, format, f);
    //  return etl::string<max_len>(buf);
    //}

    //template<size_t max_len>
    //etl::string<max_len> from_float(float f, char const* format_string)
    //{
    //  char buf[32];
    //  sprintf(buf, format_string, f);
    //  return etl::string<max_len>(buf);
    //}

    //template<size_t max_len>
    //etl::string<max_len> from_unsigned(unsigned u, bool hex = false)
    //{
    //  char buf[16];
    //  char format[] = { '%',hex ? 'x' : 'u',0 };
    //  sprintf(buf, format, u);
    //  return etl::string<max_len>(buf);
    //}

    //template<size_t max_len>
    //etl::string<max_len> from_int(int i, bool hex = false)
    //{
    //  char buf[16];
    //  char format[] = { '%',hex ? 'x' : 'i',0 };
    //  sprintf(buf, format, i);
    //  return etl::string<max_len>(buf);
    //}

    //template<size_t max_len>
    //etl::string<max_len> from_bool(bool b, bool verbose)
    //{
    //  if (verbose)
    //    return b ? "true" : "false";
    //  else
    //    return b ? "t" : "f";
    //}


    //template<size_t max_len>
    //void reverse(etl::string<max_len>& s)
    //{
    //  std::reverse(s.begin(), s.end());
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

    //template<size_t max_len>
    //void to_uppercase(etl::string<max_len>& s)
    //{
    //  for (typename etl::string<max_len>::iterator iter = s.begin(); iter != s.end(); ++iter)
    //    *iter = char(toupper(*iter));
    //}

    //template<size_t max_len>
    //void to_lowercase(etl::string<max_len>& s)
    //{
    //  for (typename etl::string<max_len>::iterator iter = s.begin(); iter != s.end(); ++iter)
    //    *iter = char(tolower(*iter));
    //}

    //template<size_t max_len>
    //void namify(etl::string<max_len>& s)
    //{
    //  replace(s.begin(), s.end(), ' ', '_');
    //  replace(s.begin(), s.end(), '/', '_');
    //  replace(s.begin(), s.end(), '-', '_');
    //  replace(s.begin(), s.end(), '.', '_');
    //  replace(s.begin(), s.end(), '(', '_');
    //  replace(s.begin(), s.end(), ')', '_');
    //  replace(s.begin(), s.end(), ',', '_');
    //  replace(s.begin(), s.end(), '[', '_');
    //  replace(s.begin(), s.end(), ']', '_');
    //  while (!s.empty() && s.back() == '_')
    //    s.pop_back();
    //}
  };
}

#endif