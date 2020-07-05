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

#ifndef ETL_U16STRING_STREAM_INCLUDED
#define ETL_U16STRING_STREAM_INCLUDED

///\ingroup string

#include "platform.h"
#include "u16string.h"
#include "u16format_spec.h"
#include "to_u16string.h"
#include "string_view.h"

namespace etl
{
  class u16string_stream
  {
  public:

    typedef etl::u16format_spec         format_spec_type;
    typedef etl::iu16string             istring_type;
    typedef etl::iu16string::value_type value_type;

    //*************************************************************************
    /// Construct from text.
    //*************************************************************************
    explicit u16string_stream(etl::iu16string& text_)
      : text(text_)
    {
    }

    //*************************************************************************
    /// Construct from text and format spec.
    //*************************************************************************
    u16string_stream(etl::iu16string& text_, etl::u16format_spec spec_)
      : text(text_)
      , spec(spec_)
    {
    }

    //*************************************************************************
    /// Set the format spec.
    //*************************************************************************
    void set_format(etl::u16format_spec spec_)
    {
      spec = spec_;
    }

    //*************************************************************************
    /// Get a const reference to the format spec.
    //*************************************************************************
    const etl::u16format_spec& get_format() const
    {
      return spec;
    }

    //*************************************************************************
    /// Get a reference to the current string.
    //*************************************************************************
    etl::iu16string& str()
    {
      return text;
    }

    //*************************************************************************
    /// Get a const reference to the current string.
    //*************************************************************************
    const etl::iu16string& str() const
    {
      return text;
    }

    //*************************************************************************
    /// Resets the stream to the supplied string.
    //*************************************************************************
    void str(const etl::iu16string::value_type* p)
    {
      text.assign(p);
    }

    //*************************************************************************
    /// Resets the stream to the supplied string.
    //*************************************************************************
    void str(const etl::iu16string& is)
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
    friend u16string_stream& operator <<(u16string_stream& ss, etl::u16format_spec spec)
    {
      ss.spec = spec;
      return ss;
    }

    //*********************************
    friend u16string_stream& operator <<(u16string_stream& ss, etl::u16string_view view)
    {
      etl::to_string(view, ss.text, ss.spec, true);
      return ss;
    }

    //*********************************
    friend u16string_stream& operator <<(u16string_stream& ss, etl::iu16string::const_pointer p)
    {
      etl::u16string_view view(p);
      ss << view;
      return ss;
    } 

    //*********************************
    friend u16string_stream& operator <<(u16string_stream& ss, const etl::iu16string& text)
    {
      etl::to_string(text, ss.text, ss.spec, true);
      return ss;
    }

    //*********************************
    template <size_t SIZE>
    friend u16string_stream& operator <<(u16string_stream& ss, const etl::u16string<SIZE>& text)
    {
      const etl::iu16string& itext = text;
      etl::to_string(itext, ss.text, ss.spec, true);
      return ss;
    }

    //*********************************
    template <typename T>
    friend u16string_stream& operator <<(u16string_stream& ss, const T& value)
    {
      etl::to_string(value, ss.text, ss.spec, true);
      return ss;
    }

  private:

    etl::iu16string&    text;
    etl::u16format_spec spec;
  };
}

#endif
