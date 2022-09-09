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
#include "algorithm.h"
#include "enum_type.h"
#include "memory.h"
#include "char_traits.h"
#include "optional.h"

#include <ctype.h>
#include <stdint.h>

#include "private/minmax_push.h"

namespace etl
{
  //***************************************************************************
  /// string_pad_direction
  //***************************************************************************
  struct string_pad_direction
  {
    enum enum_type
    {
      LEFT,
      RIGHT,
    };

    ETL_DECLARE_ENUM_TYPE(string_pad_direction, int)
    ETL_ENUM_TYPE(LEFT,  "left")
    ETL_ENUM_TYPE(RIGHT, "right")
    ETL_END_ENUM_TYPE
  };

  //***************************************************************************
  /// whitespace
  //***************************************************************************
  template <typename TChar>
  struct whitespace;

  template <>
  struct whitespace<char>
  {
    static ETL_CONSTEXPR const char* value()
    {
      return " \t\n\r\f\v";
    }
  };

  template <>
  struct whitespace<wchar_t>
  {
    static ETL_CONSTEXPR const wchar_t* value()
    {
      return L" \t\n\r\f\v";
    }
  };

#if ETL_USING_CPP11
  template <>
  struct whitespace<char16_t>
  {
    static ETL_CONSTEXPR const char16_t* value()
    {
      return u" \t\n\r\f\v";
    }
  };

  template <>
  struct whitespace<char32_t>
  {
    static ETL_CONSTEXPR const char32_t* value()
    {
      return U" \t\n\r\f\v";
    }
  };
#endif

#if ETL_USING_CPP17
  template <typename TChar>
  inline constexpr const TChar* whitespace_v = whitespace<TChar>::value();
#endif

  //***************************************************************************
  /// trim_from_left
  /// Trim left of trim_characters
  //***************************************************************************
  template <typename TIString>
  void trim_from_left(TIString& s, typename TIString::const_pointer trim_characters)
  {
    typename TIString::size_type position = s.find_first_not_of(trim_characters);
    s.erase(0U, position);
  }

  //***************************************************************************
  /// trim_whitespace_left
  /// Trim left of whitespace
  //***************************************************************************
  template <typename TIString>
  void trim_whitespace_left(TIString& s)
  {
    trim_from_left(s, whitespace<typename TIString::value_type>::value());
  }

  //***************************************************************************
  /// trim_from_view_left
  /// Trim left of trim_characters
  //***************************************************************************
  template <typename TStringView>
  TStringView trim_from_view_left(const TStringView& view, typename TStringView::const_pointer trim_characters)
  {
    typename TStringView::size_type first = view.find_first_not_of(trim_characters);

    typename TStringView::const_pointer pbegin = view.data() + view.size();

    if (first != TStringView::npos)
    {
      pbegin = view.data() + first;
    }

    return TStringView(pbegin, etl::distance(pbegin, view.data() + view.size()));
  }

  //***************************************************************************
  /// trim_view_whitespace_left
  /// Trim left of whitespace
  //***************************************************************************
  template <typename TStringView>
  TStringView trim_view_whitespace_left(TStringView& s)
  {
    return trim_from_view_left(s, whitespace<typename TStringView::value_type>::value());
  }

  //***************************************************************************
  /// trim_left
  /// Trim left, up to, but not including, delimiters.
  //***************************************************************************
  template <typename TIString>
  void trim_left(TIString& s, typename TIString::const_pointer delimiters)
  {
    typename TIString::size_type p = s.find_first_of(delimiters);

    if (p == TIString::npos)
    {
      s.clear();
    }
    else
    {
      s.erase(0, p);
    }
  }

  //***************************************************************************
  /// trim_view_left
  /// View trim left, up to, but not including, delimiters.
  //***************************************************************************
  template <typename TStringView>
  TStringView trim_view_left(const TStringView& view, typename TStringView::const_pointer delimiters)
  {
    typename TStringView::size_type first = view.find_first_of(delimiters);

    typename TStringView::const_pointer pbegin = view.data();

    if (first != TStringView::npos)
    {
      pbegin += first;
      return TStringView(pbegin, view.size() - first);
    }
    else
    {
      return TStringView(pbegin, typename TStringView::size_type(0U));
    }
  }

  //***************************************************************************
  /// trim_from_right
  /// Trim right of trim_characters
  //***************************************************************************
  template <typename TIString>
  void trim_from_right(TIString& s, typename TIString::const_pointer trim_characters)
  {
    s.erase(s.find_last_not_of(trim_characters) + 1);
  }

  //***************************************************************************
  /// trim_whitespace_right
  /// Trim from right of whitespace
  //***************************************************************************
  template <typename TIString>
  void trim_whitespace_right(TIString& s)
  {
    trim_from_right(s, whitespace<typename TIString::value_type>::value());
  }

  //***************************************************************************
  /// trim_from_view_right
  /// Trim right of trim_characters
  //***************************************************************************
  template <typename TStringView>
  TStringView trim_from_view_right(const TStringView& view, typename TStringView::const_pointer trim_characters)
  {
    typename TStringView::size_type last = view.find_last_not_of(trim_characters) + 1;

    typename TStringView::const_pointer pend = view.data();

    if (last != TStringView::npos)
    {
      pend += last;
    }

    return TStringView(view.data(), etl::distance(view.data(), pend));
  }

  //***************************************************************************
  /// trim_view_whitespace_right
  /// Trim right of whitespace
  //***************************************************************************
  template <typename TStringView>
  TStringView trim_view_whitespace_right(TStringView& view)
  {
    return trim_from_view_right(view, whitespace<typename TStringView::value_type>::value());
  }

  //***************************************************************************
  /// trim_right
  //***************************************************************************
  template <typename TIString>
  void trim_right(TIString& s, typename TIString::const_pointer delimiters)
  {
    typename TIString::size_type p = s.find_last_of(delimiters);

    if (p == TIString::npos)
    {
      s.clear();
    }
    else
    {
      ++p;

      if (p != s.size())
      {
        s.erase(p);
      }
    }
  }

  //***************************************************************************
  /// trim_view_right
  //***************************************************************************
  template <typename TStringView>
  TStringView trim_view_right(const TStringView& view, typename TStringView::const_pointer delimiters)
  {
    typename TStringView::size_type last = view.find_last_of(delimiters) + 1;

    typename TStringView::const_pointer pend = view.data();

    if (last != TStringView::npos)
    {
      pend += last;
      return TStringView(view.data(), etl::distance(view.data(), pend));
    }
    else
    {
      return TStringView(view.data(), typename TStringView::size_type(0U));
    }
  }

  //***************************************************************************
  /// trim_from
  /// Trim left and right of trim_characters
  //***************************************************************************
  template <typename TIString>
  void trim_from(TIString& s, typename TIString::const_pointer trim_characters)
  {
    trim_from_left(s, trim_characters);
    trim_from_right(s, trim_characters);
  }

  //***************************************************************************
  /// trim_whitespace
  /// Trim both ends of whitespace
  //***************************************************************************
  template <typename TIString>
  void trim_whitespace(TIString& s)
  {
    trim_from(s, whitespace<typename TIString::value_type>::value());
  }

  //***************************************************************************
  /// trim_from_view
  /// Trim left and right of trim_characters
  //***************************************************************************
  template <typename TStringView>
  TStringView trim_from_view(const TStringView& view, typename TStringView::const_pointer trim_characters)
  {
    typename TStringView::size_type first = view.find_first_not_of(trim_characters);
    typename TStringView::size_type last  = view.find_last_not_of(trim_characters) + 1;

    typename TStringView::const_pointer pbegin = view.data();
    typename TStringView::const_pointer pend   = view.data();

    if (first != TStringView::npos)
    {
      pbegin += first;
    }

    if (last != TStringView::npos)
    {
      pend += last;
    }

    return TStringView(pbegin, etl::distance(pbegin, pend));
  }

  //***************************************************************************
  /// trim_view_whitespace
  /// Trim both ends of whitespace
  //***************************************************************************
  template <typename TStringView>
  TStringView trim_view_whitespace(const TStringView& view)
  {
    return trim_from_view(view, whitespace<typename TStringView::value_type>::value());
  }

  //***************************************************************************
  /// trim_delimiters
  /// Trim left and right of trim_characters
  //***************************************************************************
  template <typename TIString>
  void trim(TIString& s, typename TIString::const_pointer delimiters)
  {
    trim_left(s, delimiters);
    trim_right(s, delimiters);
  }

  //***************************************************************************
  /// trim_view
  /// Trim left and right of trim_characters
  //***************************************************************************
  template <typename TStringView>
  TStringView trim_view(const TStringView& view, typename TStringView::const_pointer delimiters)
  {
    typename TStringView::size_type first = view.find_first_of(delimiters);
    typename TStringView::size_type last  = view.find_last_of(delimiters) + 1;

    typename TStringView::const_pointer pbegin = view.data();
    typename TStringView::const_pointer pend   = view.data();

    if (first != TStringView::npos)
    {
      pbegin += first;
    }

    if (last != TStringView::npos)
    {
      pend += last;
    }

    return TStringView(pbegin, etl::distance(pbegin, pend));
  }

  //***************************************************************************
  /// Get up to the first n characters.
  //***************************************************************************
  template <typename TIString>
  void left_n(TIString& s, typename TIString::size_type n)
  {
    n = (n > s.size()) ? s.size() : n;

    s.erase(s.begin() + n, s.end());
  }

  //***************************************************************************
  /// Get a view of up to the first n characters.
  //***************************************************************************
  template <typename TStringView>
  TStringView left_n_view(const TStringView& view, typename TStringView::size_type n)
  {
    n = (n > view.size()) ? view.size() : n;

    return TStringView(etl::addressof(*view.begin()), n);
  }

  //***************************************************************************
  /// Get up to the last n characters.
  //***************************************************************************
  template <typename TIString>
  void right_n(TIString& s, typename TIString::size_type n)
  {
    n = (n > s.size()) ? s.size() : n;

    s.erase(s.begin(), s.end() - n);
  }

  //***************************************************************************
  /// Get a view of up to the last n characters.
  //***************************************************************************
  template <typename TStringView>
  TStringView right_n_view(const TStringView& view, typename TStringView::size_type n)
  {
    n = (n > view.size()) ? view.size() : n;

    return TStringView(view.data() + view.size() - n, n);
  }

  //***************************************************************************
  /// reverse
  /// Reverse a string
  //***************************************************************************
  template <typename TIString>
  void reverse(TIString& s)
  {
    etl::reverse(s.begin(), s.end());
  }

  //***************************************************************************
  /// replace_characters
  //***************************************************************************
  template <typename TIString, typename TPair>
  void replace_characters(TIString& s,
                          const TPair* pairsbegin,
                          const TPair* pairsend)
  {
    while (pairsbegin != pairsend)
    {
      etl::replace(s.begin(), s.end(), pairsbegin->first, pairsbegin->second);
      ++pairsbegin;
    }
  }

  //***************************************************************************
  /// replace_strings
  //***************************************************************************
  template <typename TIString, typename TPair>
  void replace_strings(TIString& s,
                        const TPair* pairsbegin,
                        const TPair* pairsend)
  {
    while (pairsbegin != pairsend)
    {
      const typename TIString::value_type* p_old = pairsbegin->first;
      const typename TIString::value_type* p_new = pairsbegin->second;

      typename TIString::size_type position = 0U;

      do
      {
        position = s.find(p_old, position);
        if (position != TIString::npos)
        {
          s.replace(position, typename TIString::size_type(etl::strlen(p_old)), p_new, typename TIString::size_type(etl::strlen(p_new)));
          position += typename TIString::size_type(etl::strlen(p_new));
        }
      } while (position != TIString::npos);

      ++pairsbegin;
    }
  }

  //*********************************************************************
  /// Find first of any of delimiters within the string
  //*********************************************************************
  template <typename TIterator, typename TPointer>
  TIterator find_first_of(TIterator first, TIterator last, TPointer delimiters)
  {
    TIterator itr(first);

    while (itr != last)
    {
      TPointer pd = delimiters;

      while (*pd != 0)
      {
        if (*itr == *pd)
        {
          return itr;
        }

        ++pd;
      }

      ++itr;
    }

    return last;
  }

  //*********************************************************************
  /// Find first of any of delimiters within the string
  //*********************************************************************
  template <typename TIString, typename TPointer>
  typename TIString::iterator find_first_of(TIString& s, TPointer delimiters)
  {
    return find_first_of(s.begin(), s.end(), delimiters);
  }

  //*********************************************************************
  /// Find first of any of delimiters within the string
  //*********************************************************************
  template <typename TIString, typename TPointer>
  typename TIString::const_iterator find_first_of(const TIString& s, TPointer delimiters)
  {
    return find_first_of(s.begin(), s.end(), delimiters);
  }

  //*********************************************************************
  /// Find first not of any of delimiters within the string
  //*********************************************************************
  template <typename TIterator, typename TPointer>
  TIterator find_first_not_of(TIterator first, TIterator last, TPointer delimiters)
  {
    TIterator itr(first);

    while (itr != last)
    {
      TPointer pd = delimiters;

      bool found = false;

      while (*pd != 0)
      {
        if (*itr == *pd)
        {
          found = true;
          break;
        }

        ++pd;
      }

      if (!found)
      {
        return itr;
      }

      ++itr;
    }

    return last;
  }

  //*********************************************************************
  /// Find first not of any of delimiters within the string
  //*********************************************************************
  template <typename TIString, typename TPointer>
  typename TIString::iterator find_first_not_of(TIString& s, TPointer delimiters)
  {
    return find_first_not_of(s.begin(), s.end(), delimiters);
  }

  //*********************************************************************
  /// Find first not of any of delimiters within the string
  //*********************************************************************
  template <typename TIString, typename TPointer>
  typename TIString::const_iterator find_first_not_of(const TIString& s, TPointer delimiters)
  {
    return find_first_not_of(s.begin(), s.end(), delimiters);
  }

  //*********************************************************************
  /// Find last of any of delimiters within the string
  //*********************************************************************
  template <typename TIterator, typename TPointer>
  TIterator find_last_of(TIterator first, TIterator last, TPointer delimiters)
  {
    if (first == last)
    {
      return last;
    }

    TIterator itr(last);
    TIterator end(first);

    do
    {
      --itr;

      TPointer pd = delimiters;

      while (*pd != 0)
      {
        if (*itr == *pd)
        {
          return itr;
        }

        ++pd;
      }
    } while (itr != end);

    return last;
  }

  //*********************************************************************
  /// Find last of any of delimiters within the string
  //*********************************************************************
  template <typename TIString, typename TPointer>
  typename TIString::iterator find_last_of(TIString& s, TPointer delimiters)
  {
    return find_last_of(s.begin(), s.end(), delimiters);
  }

  //*********************************************************************
  /// Find last of any of delimiters within the string
  //*********************************************************************
  template <typename TIString, typename TPointer>
  typename TIString::const_iterator find_last_of(const TIString& s, TPointer delimiters)
  {
    return find_last_of(s.begin(), s.end(), delimiters);
  }

  //*********************************************************************
  /// Find last not of any of delimiters within the string
  //*********************************************************************
  template <typename TIterator, typename TPointer>
  TIterator find_last_not_of(TIterator first, TIterator last, TPointer delimiters)
  {
    if (first == last)
    {
      return last;
    }

    TIterator itr(last);
    TIterator end(first);

    do
    {
      --itr;

      TPointer pd = delimiters;

      bool found = false;

      while (*pd != 0)
      {
        if (*itr == *pd)
        {
          found = true;
          break;
        }

        ++pd;
      }

      if (!found)
      {
        return itr;
      }
    } while (itr != end);

    return last;
  }

  //*********************************************************************
  /// Find last not of any of delimiters within the string
  //*********************************************************************
  template <typename TIString, typename TPointer>
  typename TIString::iterator find_last_not_of(TIString& s, TPointer delimiters)
  {
    return find_last_not_of(s.begin(), s.end(), delimiters);
  }

  //*********************************************************************
  /// Find last not of any of delimiters within the string
  //*********************************************************************
  template <typename TIString, typename TPointer>
  typename TIString::const_iterator find_last_not_of(const TIString& s, TPointer delimiters)
  {
    return find_last_not_of(s.begin(), s.end(), delimiters);
  }

  //***************************************************************************
  /// get_token
  //***************************************************************************
  template <typename TInput, typename TStringView>
  etl::optional<TStringView> get_token(const TInput& input, typename TInput::const_pointer delimiters, const etl::optional<TStringView>& last_view, bool ignore_empty_tokens)
  {
    typedef typename TInput::const_pointer const_pointer;

    bool token_found = false;
    typename TStringView::size_type position  = 0U;
    TStringView view = last_view.value_or(TStringView());
    const_pointer begin_ptr = input.data();

    if (begin_ptr == ETL_NULLPTR)
    {
      return etl::optional<TStringView>();
    }

    const_pointer end_ptr   = begin_ptr + input.size();

    while (!token_found)
    {
      // Does the last view have valid data?
      if (view.data() != ETL_NULLPTR)
      {
        position = etl::distance(begin_ptr, view.data() + view.size() + 1U);

        // Have we reached the end of the string?
        if (position > input.size())
        {
          return etl::optional<TStringView>();
        }
      }

      // Look for the next token.
      const_pointer first_ptr = begin_ptr + position;
      const_pointer last_ptr  = find_first_of(first_ptr, end_ptr, delimiters);

      view = TStringView(first_ptr, etl::distance(first_ptr, last_ptr));

      token_found = ((view.size() != 0U) || !ignore_empty_tokens);
    }

    return etl::optional<TStringView>(view);
  }

  //***************************************************************************
  /// pad_left
  //***************************************************************************
  template <typename TIString>
  void pad_left(TIString& s, typename TIString::size_type required_size, typename TIString::value_type pad_char)
  {
    required_size = etl::min(required_size, s.max_size());

    if (required_size > s.size())
    {
      required_size -= s.size();
      s.insert(typename TIString::size_type(0U), required_size, pad_char);
    }
  }

  //***************************************************************************
  /// pad_right
  //***************************************************************************
  template <typename TIString>
  void pad_right(TIString& s, typename TIString::size_type required_size, typename TIString::value_type pad_char)
  {
    required_size = etl::min(required_size, s.max_size());

    if (required_size > s.size())
    {
      required_size -= s.size();
      s.insert(s.size(), required_size, pad_char);
    }
  }

  //***************************************************************************
  /// pad
  //***************************************************************************
  template <typename TIString>
  void pad(TIString& s, typename TIString::size_type required_size, string_pad_direction pad_direction, typename TIString::value_type pad_char)
  {
    switch (int(pad_direction))
    {
      case string_pad_direction::LEFT:
      {
        pad_left(s, required_size, pad_char);
        break;
      }

      case string_pad_direction::RIGHT:
      {
        pad_right(s, required_size, pad_char);
        break;
      }

      default:
      {
        break;
      }
    }
  }

  //***************************************************************************
  /// to_upper_case
  //***************************************************************************
  template <typename TString>
  void to_upper_case(TString& s)
  {
    etl::transform(s.begin(), s.end(), s.begin(), ::toupper);
  }

  //***************************************************************************
  /// to_lower_case
  //***************************************************************************
  template <typename TString>
  void to_lower_case(TString& s)
  {
    etl::transform(s.begin(), s.end(), s.begin(), ::tolower);
  }

  //***************************************************************************
  /// to_sentence_case
  //***************************************************************************
  template <typename TString>
  void to_sentence_case(TString& s)
  {
    typename TString::iterator itr = s.begin();

    *itr = typename TString::value_type(::toupper(*itr));
    ++itr;

    etl::transform(itr, s.end(), itr, ::tolower);
  }
}

#include "private/minmax_pop.h"

#endif
