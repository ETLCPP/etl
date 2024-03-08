///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 John Wellbelove

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
#include "to_string.h"

namespace etl
{
  template <typename TFormat, typename TIString, typename TStringView>
  class basic_string_stream
  {
  public:

    typedef TFormat                          format_spec_type;
    typedef TIString                         istring_type;
    typedef typename TIString::value_type    value_type;
    typedef typename TIString::pointer       pointer;
    typedef typename TIString::const_pointer const_pointer;

    //*************************************************************************
    /// Construct from text.
    //*************************************************************************
    explicit basic_string_stream(TIString& text_)
      : text(text_)
    {
    }

    //*************************************************************************
    /// Construct from text and format fmt.
    //*************************************************************************
    basic_string_stream(TIString& text_, const TFormat& spec_)
      : text(text_)
      , format(spec_)
    {
    }

    //*************************************************************************
    /// Set the format fmt.
    //*************************************************************************
    void set_format(const TFormat& spec_)
    {
      format = spec_;
    }

    //*************************************************************************
    /// Get a const reference to the format fmt.
    //*************************************************************************
    const TFormat& get_format() const
    {
      return format;
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
    friend basic_string_stream& operator <<(basic_string_stream& ss, const TFormat& fmt)
    {
      ss.format = fmt;
      return ss;
    }

    //*********************************
    /// etl::base_spec from etl::setbase, etl::bin, etl::oct, etl::dec & etl::hex stream manipulators
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, etl::private_basic_format_spec::base_spec fmt)
    {
      ss.format.base(fmt.base);
      return ss;
    }

    //*********************************
    /// etl::width_spec from etl::setw stream manipulator
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, etl::private_basic_format_spec::width_spec fmt)
    {
      ss.format.width(fmt.width);
      return ss;
    }

    //*********************************
    /// etl::fill_spec from etl::setfill stream manipulator
    //*********************************
    template <typename TChar>
    friend basic_string_stream& operator <<(basic_string_stream& ss, etl::private_basic_format_spec::fill_spec<TChar> fmt)
    {
      ss.format.fill(fmt.fill);
      return ss;
    }

    //*********************************
    /// etl::precision_spec from etl::setprecision stream manipulator
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, etl::private_basic_format_spec::precision_spec fmt)
    {
      ss.format.precision(fmt.precision);
      return ss;
    }

    //*********************************
    /// etl::boolalpha_spec from etl::boolalpha & etl::noboolalpha stream manipulators
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, etl::private_basic_format_spec::boolalpha_spec fmt)
    {
      ss.format.boolalpha(fmt.boolalpha);
      return ss;
    }

    //*********************************
    /// etl::uppercase_spec from etl::uppercase & etl::nouppercase stream manipulators
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, etl::private_basic_format_spec::uppercase_spec fmt)
    {
      ss.format.upper_case(fmt.upper_case);
      return ss;
    }

    //*********************************
    /// etl::showbase_spec from etl::showbase & etl::noshowbase stream manipulators
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, etl::private_basic_format_spec::showbase_spec fmt)
    {
      ss.format.show_base(fmt.show_base);
      return ss;
    }

    //*********************************
    /// etl::left_spec from etl::left stream manipulator
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, etl::private_basic_format_spec::left_spec /*fmt*/)
    {
      ss.format.left();
      return ss;
    }

    //*********************************
    /// etl::right_spec from etl::left stream manipulator
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, etl::private_basic_format_spec::right_spec /*fmt*/)
    {
      ss.format.right();
      return ss;
    }

    //*********************************
    /// From a string view
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, TStringView view)
    {
      etl::to_string(view, ss.text, ss.format, true);
      return ss;
    }

    //*********************************
    /// From a character pointer to a string
    //*********************************
    friend basic_string_stream& operator <<(basic_string_stream& ss, pointer p)
    {
      TStringView view(p);
      ss << view;
      return ss;
    }

    //*********************************
    /// From a const character pointer to a string
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
    friend basic_string_stream& operator <<(basic_string_stream& ss, const TIString& t)
    {
      etl::to_string(t, ss.text, ss.format, true);
      return ss;
    }

    //*********************************
    /// From a string
    //*********************************
    template <template <size_t> class TString, size_t SIZE>
    friend basic_string_stream& operator <<(basic_string_stream& ss, const TString<SIZE>& t)
    {
      const TIString& itext = t;
      etl::to_string(itext, ss.str(), ss.get_format(), true);
      return ss;
    }

    //*********************************
    /// From anything else
    //*********************************
    template <typename T>
    friend basic_string_stream& operator <<(basic_string_stream& ss, const T& value)
    {
      etl::to_string(value, ss.text, ss.format, true);
      return ss;
    }

  private:

    TIString& text;
    TFormat   format;

    basic_string_stream(const basic_string_stream&) ETL_DELETE;
    basic_string_stream& operator =(const basic_string_stream&) ETL_DELETE;
  };
}

#endif
