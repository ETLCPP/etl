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

#ifndef ETL_U32STRING_STREAM_INCLUDED
#define ETL_U32STRING_STREAM_INCLUDED

///\ingroup string

#include "platform.h"
#include "u32string.h"
#include "u32format_spec.h"
#include "to_u32string.h"
#include "string_view.h"

namespace etl
{
  class u32string_stream
  {
  public:

    typedef etl::u32format_spec         format_spec_type;
    typedef etl::iu32string             istring_type;
    typedef etl::iu32string::value_type value_type;

    //*************************************************************************
    /// Construct from text.
    //*************************************************************************
    explicit u32string_stream(etl::iu32string& text_)
      : text(text_)
    {
    }

    //*************************************************************************
    /// Construct from text and format spec.
    //*************************************************************************
    u32string_stream(etl::iu32string& text_, const etl::u32format_spec& spec_)
      : text(text_)
      , spec(spec_)
    {
    }

    //*************************************************************************
    /// Set the format spec.
    //*************************************************************************
    void set_format(const etl::u32format_spec& spec_)
    {
      spec = spec_;
    }

    //*************************************************************************
    /// Get a const reference to the format spec.
    //*************************************************************************
    const etl::u32format_spec& get_format() const
    {
      return spec;
    }

    //*************************************************************************
    /// Get a reference to the current string.
    //*************************************************************************
    etl::iu32string& str()
    {
      return text;
    }

    //*************************************************************************
    /// Get a const reference to the current string.
    //*************************************************************************
    const etl::iu32string& str() const
    {
      return text;
    }

    //*************************************************************************
    /// Resets the stream to the supplied string.
    //*************************************************************************
    void str(const etl::iu32string::value_type* p)
    {
      text.assign(p);
    }

    //*************************************************************************
    /// Resets the stream to the supplied string.
    //*************************************************************************
    void str(const etl::iu32string& is)
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
    friend u32string_stream& operator <<(u32string_stream& ss, const etl::u32format_spec& spec)
    {
      ss.spec = spec;
      return ss;
    }

    //*********************************
    friend u32string_stream& operator <<(u32string_stream& ss, etl::u32string_view view)
    {
      etl::to_string(view, ss.text, ss.spec, true);
      return ss;
    }

    //*********************************
    friend u32string_stream& operator <<(u32string_stream& ss, etl::iu32string::const_pointer p)
    {
      etl::u32string_view view(p);
      ss << view;
      return ss;
    }

    //*********************************
    friend u32string_stream& operator <<(u32string_stream& ss, const etl::iu32string& text)
    {
      etl::to_string(text, ss.text, ss.spec, true);
      return ss;
    }

    //*********************************
    template <size_t SIZE>
    friend u32string_stream& operator <<(u32string_stream& ss, const etl::u32string<SIZE>& text)
    {
      const etl::iu32string& itext = text;
      etl::to_string(itext, ss.text, ss.spec, true);
      return ss;
    }

    //*********************************
    template <typename T>
    friend u32string_stream& operator <<(u32string_stream& ss, const T& value)
    {
      etl::to_string(value, ss.text, ss.spec, true);
      return ss;
    }

  private:

    etl::iu32string&    text;
    etl::u32format_spec spec;

    u32string_stream(const u32string_stream&) ETL_DELETE;
    u32string_stream& operator =(const u32string_stream&) ETL_DELETE;
  };
}

#endif
