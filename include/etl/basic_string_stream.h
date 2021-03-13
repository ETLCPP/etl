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

#ifndef ETL_BASIC_STRING_STREAM_INCLUDED
#define ETL_BASIC_STRING_STREAM_INCLUDED

///\ingroup string

#include "platform.h"

namespace etl
{
  template <typename TFormat, typename TIString, typename TStringView>
  class basic_string_stream
  {
  public:

    typedef TFormat                          format_spec_type;
    typedef TIString                         istring_type;
    typedef typename TIString::value_type    value_type;
    typedef typename TIString::const_pointer const_pointer;

    //*************************************************************************
    /// Construct from text.
    //*************************************************************************
    explicit basic_string_stream(TIString& text_)
      : text(text_)
    {
    }

    //*************************************************************************
    /// Construct from text and format spec.
    //*************************************************************************
    basic_string_stream(TIString& text_, const TFormat& spec_)
      : text(text_)
      , spec(spec_)
    {
    }

    //*************************************************************************
    /// Set the format spec.
    //*************************************************************************
    void set_format(const TFormat& spec_)
    {
      spec = spec_;
    }

    //*************************************************************************
    /// Get a const reference to the format spec.
    //*************************************************************************
    const TFormat& get_format() const
    {
      return spec;
    }

    //*************************************************************************
    /// Get a reference to the current string.
    //*************************************************************************
    TIString& str()
    {
      return text;
    }

    //*************************************************************************
    /// Get a const reference to the current string.
    //*************************************************************************
    const TIString& str() const
    {
      return text;
    }

    //*************************************************************************
    /// Resets the stream to the supplied string.
    //*************************************************************************
    void str(const value_type* p)
    {
      text.assign(p);
    }

    //*************************************************************************
    /// Resets the stream to the supplied string.
    //*************************************************************************
    void str(const TIString& is)
    {
      text.assign(is);
    }

    //*************************************************************************
    /// Stream operators.
    //*************************************************************************

    //*********************************
    /// TFormat
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, const TFormat& spec)
    {
      ss.spec = spec;
      return ss;
    }

    //*********************************
    /// etl::base_spec from etl::setbase, etl::bin, etl::oct, etl::dec & etl::hex stream manipulators
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, etl::private_basic_format_spec::base_spec spec)
    {
      ss.spec.base(spec.base);
      return ss;
    }

    //*********************************
    /// etl::width_spec from etl::setw stream manipulator
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, etl::private_basic_format_spec::width_spec spec)
    {
      ss.spec.width(spec.width);
      return ss;
    }

    //*********************************
    /// etl::fill_spec from etl::setfill stream manipulator
    //*********************************
    template <typename TChar>
    friend basic_string_stream& operator <<(basic_string_stream& ss, etl::private_basic_format_spec::fill_spec<TChar> spec)
    {
      ss.spec.fill(spec.fill);
      return ss;
    }

    //*********************************
    /// etl::precision_spec from etl::setprecision stream manipulator
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, etl::private_basic_format_spec::precision_spec spec)
    {
      ss.spec.precision(spec.precision);
      return ss;
    }

    //*********************************
    /// etl::boolalpha_spec from etl::boolalpha & etl::noboolalpha stream manipulators
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, etl::private_basic_format_spec::boolalpha_spec spec)
    {
      ss.spec.boolalpha(spec.boolalpha);
      return ss;
    }

    //*********************************
    /// etl::uppercase_spec from etl::uppercase & etl::nouppercase stream manipulators
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, etl::private_basic_format_spec::uppercase_spec spec)
    {
      ss.spec.upper_case(spec.upper_case);
      return ss;
    }

    //*********************************
    /// etl::showbase_spec from etl::showbase & etl::noshowbase stream manipulators
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, etl::private_basic_format_spec::showbase_spec spec)
    {
      ss.spec.show_base(spec.show_base);
      return ss;
    }

    //*********************************
    /// etl::left_spec from etl::left stream manipulator
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, etl::private_basic_format_spec::left_spec spec)
    {
      ss.spec.left();
      return ss;
    }

    //*********************************
    /// etl::right_spec from etl::left stream manipulator
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, etl::private_basic_format_spec::right_spec spec)
    {
      ss.spec.right();
      return ss;
    }

    //*********************************
    /// From a string view
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, TStringView view)
    {
      etl::to_string(view, ss.text, ss.spec, true);
      return ss;
    }

    //*********************************
    /// From a const pointer to a string
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, const_pointer p)
    {
      TStringView view(p);
      ss << view;
      return ss;
    }

    //*********************************
    /// From a string interface
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, const TIString& text)
    {
      etl::to_string(text, ss.text, ss.spec, true);
      return ss;
    }

    //*********************************
    /// From a string
    //*********************************
    template <template <size_t> class TString, size_t SIZE>
    friend basic_string_stream& operator <<(basic_string_stream& ss, const TString<SIZE>& text)
    {
      const TIString& itext = text;
      etl::to_string(itext, ss.str(), ss.get_format(), true);
      return ss;
    }

    //*********************************
    /// From anything else
    //*********************************
    template <typename T>
    friend basic_string_stream& operator <<(basic_string_stream& ss, const T& value)
    {
      etl::to_string(value, ss.text, ss.spec, true);
      return ss;
    }

  private:

    TIString&    text;
    TFormat spec;

    basic_string_stream(const basic_string_stream&) ETL_DELETE;
    basic_string_stream& operator =(const basic_string_stream&) ETL_DELETE;
  };
}

#endif
