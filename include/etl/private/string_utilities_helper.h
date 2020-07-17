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
#include "../utility.h"
#include "../enum_type.h"

#include <stdint.h>

namespace etl
{
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

  namespace private_string_utilities
  {
    //***************************************************************************
    /// trim_from_left
    /// Trim left of trim_characters
    //***************************************************************************
    template <typename TIString>
    void trim_from_left(TIString& s, typename TIString::const_pointer trim_characters)
    {
      size_t position = s.find_first_not_of(trim_characters);
      s.erase(0U, position);
    }

    //***************************************************************************
    /// view_trim_left_of
    /// Trim left of whitespace
    //***************************************************************************
    template <typename TStringView>
    TStringView view_trim_from_left(const TStringView& view, typename TStringView::const_pointer trim_characters)
    {
      size_t first = view.find_first_not_of(trim_characters);

      typename TStringView::const_pointer pbegin = view.end();

      if (first != TStringView::npos)
      {
        pbegin = view.begin() + first;
      }

      return TStringView(pbegin, view.end());
    }

    //***************************************************************************
    /// trim_left_delimiters
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
    /// view_trim_left_delimiters
    /// View trim left, up to, but not including, delimiters.
    //***************************************************************************
    template <typename TStringView>
    TStringView view_trim_left_delimiters(const TStringView& view, typename TStringView::const_pointer delimiters)
    {
      size_t first = view.find_first_of(delimiters);

      typename TStringView::const_pointer pbegin = view.end();

      if (first != TStringView::npos)
      {
        pbegin = view.begin() + first;
      }

      return TStringView(pbegin, view.end());
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
    /// view_trim_from_right
    /// Trim right of trim_characters
    //***************************************************************************
    template <typename TStringView>
    TStringView view_trim_from_right(const TStringView& view, typename TStringView::const_pointer trim_characters)
    {
      size_t last = view.find_last_not_of(trim_characters) + 1;

      typename TStringView::const_pointer pend = view.begin();

      if (last != TStringView::npos)
      {
        pend += last;
      }

      return TStringView(view.begin(), pend);
    }

    //***************************************************************************
    /// trim_right_delimiters
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
    /// view_trim_right_delimiters
    //***************************************************************************
    template <typename TStringView>
    TStringView view_trim_right_delimiters(const TStringView& view, typename TStringView::const_pointer delimiters)
    {
      size_t last = view.find_last_of(delimiters) + 1;

      typename TStringView::const_pointer pend = view.begin();

      if (last != TStringView::npos)
      {
        pend += last;
      }

      return TStringView(view.begin(), pend);
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
    /// trim_from
    /// Trim left and right of trim_characters
    //***************************************************************************
    template <typename TStringView>
    TStringView view_trim_from(const TStringView& view, typename TStringView::const_pointer trim_characters)
    {
      size_t first = view.find_first_not_of(trim_characters);
      size_t last  = view.find_last_not_of(trim_characters) + 1;

      typename TStringView::const_pointer pbegin = view.begin();
      typename TStringView::const_pointer pend   = view.begin();

      if (first != TStringView::npos)
      {
        pbegin += first;
      }

      if (last != TStringView::npos)
      {
        pend += last;
      }

      return TStringView(pbegin, pend);
    }

    //***************************************************************************
    /// trim_delimiters
    /// Trim left and right of trim_characters
    //***************************************************************************
    template <typename TIString>
    void trim_delimiters(TIString& s, typename TIString::const_pointer delimiters)
    {
      trim_left_delimiters(s, delimiters);
      trim_right_delimiters(s, delimiters);
    }

    //***************************************************************************
    /// trim_delimiters
    /// Trim left and right of trim_characters
    //***************************************************************************
    template <typename TStringView>
    TStringView view_trim_delimiters(const TStringView& view, typename TStringView::const_pointer delimiters)
    {
      size_t first = view.find_first_of(delimiters);
      size_t last  = view.find_last_of(delimiters) + 1;

      typename TStringView::const_pointer pbegin = view.begin();
      typename TStringView::const_pointer pend   = view.begin();

      if (first != TStringView::npos)
      {
        pbegin += first;
      }

      if (last != TStringView::npos)
      {
        pend += last;
      }

      return TStringView(pbegin, pend);
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
    template <typename TIString>
    void replace_characters(TIString& s,
                            const etl::pair<typename TIString::value_type, typename TIString::value_type>* pairsbegin,
                            const etl::pair<typename TIString::value_type, typename TIString::value_type>* pairsend)
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
    template <typename TIString>
    void replace_strings(TIString& s,
                         const etl::pair<const typename TIString::value_type*, const typename TIString::value_type*>* pairsbegin,
                         const etl::pair<const typename TIString::value_type*, const typename TIString::value_type*>* pairsend)
    {
      while (pairsbegin != pairsend)
      {
        const typename TIString::value_type* p_old = pairsbegin->first;
        const typename TIString::value_type* p_new = pairsbegin->second;

        size_t position = 0U;

        do
        {
          position = s.find(p_old, position);
          if (position != TIString::npos)
          {
            s.replace(position, etl::strlen(p_old), p_new, etl::strlen(p_new));
            position += etl::strlen(p_new);
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
    /// Find last of any of delimiters within the string
    //*********************************************************************
    template <typename TIterator, typename TPointer>
    TIterator find_last_of(TIterator first, TIterator last, TPointer delimiters)
    {
      if (first == last)
      {
        return last;
      }

      TIterator itr(last - 1);
      TIterator end(first - 1);

      while (itr != end)
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

        --itr;
      }

      return last;
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

      TIterator itr(last - 1);
      TIterator end(first - 1);

      while (itr != end)
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

        --itr;
      }

      return last;
    }

    //***************************************************************************
    /// get_token
    //***************************************************************************
    template <typename TStringView, typename TIString>
    TStringView get_token(const TIString& s, typename TIString::const_pointer delimiters, const TStringView& last_view)
    {
      typename TIString::const_iterator first = last_view.end();
      typename TIString::const_iterator last  = last_view.end();

      // If the last view was default constructed then we must be looking for the first token.
      if (last_view.data() == ETL_NULLPTR)
      {
        first = s.begin();
      }

      // Look for the start of the next token.
      first = find_first_not_of(first, s.end(), delimiters);
      last  = find_first_of(first, s.end(), delimiters);

      return TStringView(first, last);
    }

    //***************************************************************************
    /// pad_left
    //***************************************************************************
    template <typename TIString>
    void pad_left(TIString& s, size_t required_size, typename TIString::value_type pad_char)
    {
      required_size = etl::min(required_size, s.capacity());

      if (required_size > s.size())
      {
        required_size -= s.size();
        s.insert(0U, required_size, pad_char);
      }
    }

    //***************************************************************************
    /// pad_right
    //***************************************************************************
    template <typename TIString>
    void pad_right(TIString& s, size_t required_size, typename TIString::value_type pad_char)
    {
      required_size = etl::min(required_size, s.capacity());

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
    void pad(TIString& s, size_t required_size, string_pad_direction pad_direction, typename TIString::value_type pad_char)
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
  };
}

#endif