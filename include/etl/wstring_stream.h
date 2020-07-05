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

#ifndef ETL_WSTRING_STREAM_INCLUDED
#define ETL_WSTRING_STREAM_INCLUDED

///\ingroup string

#include "platform.h"
#include "cstring.h"
#include "wformat_spec.h"
#include "to_wstring.h"
#include "string_view.h"

namespace etl
{
  class wstring_stream
  {
  public:

    typedef etl::wformat_spec         format_spec_type;
    typedef etl::iwstring             istring_type;
    typedef etl::iwstring::value_type value_type;

    //*************************************************************************
    /// Construct from text.
    //*************************************************************************
    explicit wstring_stream(etl::iwstring& text_)
      : text(text_)
    {
    }

    //*************************************************************************
    /// Construct from text and format spec.
    //*************************************************************************
    wstring_stream(etl::iwstring& text_, etl::wformat_spec spec_)
      : text(text_)
      , spec(spec_)
    {
    }

    //*************************************************************************
    /// Set the format spec.
    //*************************************************************************
    void set_format(etl::wformat_spec spec_)
    {
      spec = spec_;
    }

    //*************************************************************************
    /// Get a const reference to the format spec.
    //*************************************************************************
    const etl::wformat_spec& get_format() const
    {
      return spec;
    }

    //*************************************************************************
    /// Get a reference to the current string.
    //*************************************************************************
    etl::iwstring& str()
    {
      return text;
    }

    //*************************************************************************
    /// Get a const reference to the current string.
    //*************************************************************************
    const etl::iwstring& str() const
    {
      return text;
    }

    //*************************************************************************
    /// Resets the stream to the supplied string.
    //*************************************************************************
    void str(const etl::iwstring::value_type* p)
    {
      text.assign(p);
    }

    //*************************************************************************
    /// Resets the stream to the supplied string.
    //*************************************************************************
    void str(const etl::iwstring& is)
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
    friend wstring_stream& operator <<(wstring_stream& ss, etl::wformat_spec spec)
    {
      ss.spec = spec;
      return ss;
    }

    //*********************************
    friend wstring_stream& operator <<(wstring_stream& ss, etl::wstring_view view)
    {
      etl::to_string(view, ss.text, ss.spec, true);
      return ss;
    }
    
    //*********************************
    friend wstring_stream& operator <<(wstring_stream& ss, etl::iwstring::const_pointer p)
    {
      etl::wstring_view view(p);
      ss << view;
      return ss;
    }

    //*********************************
    friend wstring_stream& operator <<(wstring_stream& ss, const etl::iwstring& text)
    {
      etl::to_string(text, ss.text, ss.spec, true);
      return ss;
    }

    //*********************************
    template <size_t SIZE>
    friend wstring_stream& operator <<(wstring_stream& ss, const etl::wstring<SIZE>& text)
    {
      const etl::iwstring& itext = text;
      etl::to_string(itext, ss.text, ss.spec, true);
      return ss;
    }

    //*********************************
    template <typename T>
    friend wstring_stream& operator <<(wstring_stream& ss, const T& value)
    {
      etl::to_string(value, ss.text, ss.spec, true);
      return ss;
    }

  private:

    etl::iwstring&    text;
    etl::wformat_spec spec;
  };
}

#endif
