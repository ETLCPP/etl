///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 jwellbelove

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

#ifndef ETL_STRING_STREAM_INCLUDED
#define ETL_STRING_STREAM_INCLUDED

///\ingroup string

#include "platform.h"
#include "cstring.h"
#include "format_spec.h"
#include "to_string.h"
#include "string_view.h"

namespace etl
{
  class string_stream
  {
  public:

    typedef etl::format_spec         format_spec_type;
    typedef etl::istring             istring_type;
    typedef etl::istring::value_type value_type;

    //*************************************************************************
    /// Construct from text.
    //*************************************************************************
    explicit string_stream(etl::istring& text_)
      : text(text_)
    {
    }

    //*************************************************************************
    /// Construct from text and format spec.
    //*************************************************************************
    string_stream(etl::istring& text_, const etl::format_spec& spec_)
      : text(text_)
      , spec(spec_)
    {
    }

    //*************************************************************************
    /// Set the format spec.
    //*************************************************************************
    void set_format(const etl::format_spec& spec_)
    {
      spec = spec_;
    }

    //*************************************************************************
    /// Get a const reference to the format spec.
    //*************************************************************************
    const etl::format_spec& get_format() const
    {
      return spec;
    }

    //*************************************************************************
    /// Get a reference to the current string.
    //*************************************************************************
    etl::istring& str()
    {
      return text;
    }

    //*************************************************************************
    /// Get a const reference to the current string.
    //*************************************************************************
    const etl::istring& str() const
    {
      return text;
    }

    //*************************************************************************
    /// Resets the stream to the supplied string.
    //*************************************************************************
    void str(const etl::istring::value_type* p)
    {
      text.assign(p);
    }

    //*************************************************************************
    /// Resets the stream to the supplied string.
    //*************************************************************************
    void str(const etl::istring& is)
    {
      text.assign(is);
    }

    //*************************************************************************
    /// Get the current size of the string.
    //*************************************************************************
    size_t size() const
    {
      return text.size();
    }

    //*************************************************************************
    /// Stream operators.
    //*************************************************************************
    friend string_stream& operator <<(string_stream& ss, const etl::format_spec& spec)
    {
      ss.spec = spec;
      return ss;
    }

    //*********************************
    friend string_stream& operator <<(string_stream& ss, etl::private_basic_format_spec::base_spec spec)
    {
      ss.spec.base(spec.base);
      return ss;
    }

    //*********************************
    friend string_stream& operator <<(string_stream& ss, etl::private_basic_format_spec::width_spec spec)
    {
      ss.spec.width(spec.width);
      return ss;
    }

    //*********************************
    template <typename TChar>
    friend string_stream& operator <<(string_stream& ss, etl::private_basic_format_spec::fill_spec<TChar> spec)
    {
      ss.spec.fill(spec.fill);
      return ss;
    }

    //*********************************
    friend string_stream& operator <<(string_stream& ss, etl::private_basic_format_spec::precision_spec spec)
    {
      ss.spec.precision(spec.precision);
      return ss;
    }

    //*********************************
    friend string_stream& operator <<(string_stream& ss, etl::private_basic_format_spec::boolalpha_spec spec)
    {
      ss.spec.boolalpha(spec.boolalpha);
      return ss;
    }

    //*********************************
    friend string_stream& operator <<(string_stream& ss, etl::private_basic_format_spec::upper_case_spec spec)
    {
      ss.spec.upper_case(spec.upper_case);
      return ss;
    }

    //*********************************
    friend string_stream& operator <<(string_stream& ss, etl::private_basic_format_spec::left_spec spec)
    {
      ss.spec.left();
      return ss;
    }

    //*********************************
    friend string_stream& operator <<(string_stream& ss, etl::private_basic_format_spec::right_spec spec)
    {
      ss.spec.right();
      return ss;
    }

    //*********************************
    friend string_stream& operator <<(string_stream& ss, etl::string_view view)
    {
      etl::to_string(view, ss.text, ss.spec, true);
      return ss;
    }

    //*********************************
    friend string_stream& operator <<(string_stream& ss, etl::istring::const_pointer p)
    {
      etl::string_view view(p);
      ss << view;
      return ss;
    }

    //*********************************
    friend string_stream& operator <<(string_stream& ss, const etl::istring& text)
    {
      etl::to_string(text, ss.text, ss.spec, true);
      return ss;
    }

    //*********************************
    template <size_t SIZE>
    friend string_stream& operator <<(string_stream& ss, const etl::string<SIZE>& text)
    {
      const etl::istring& itext = text;
      etl::to_string(itext, ss.text, ss.spec, true);
      return ss;
    }

    //*********************************
    template <typename T>
    friend string_stream& operator <<(string_stream& ss, const T& value)
    {
      etl::to_string(value, ss.text, ss.spec, true);
      return ss;
    }

  private:

    etl::istring&    text;
    etl::format_spec spec;

    string_stream(const string_stream&) ETL_DELETE;
    string_stream& operator =(const string_stream&) ETL_DELETE;
  };
}

#endif
