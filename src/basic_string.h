///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2016 jwellbelove

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

#ifndef __ETL_BASIC_STRING__
#define __ETL_BASIC_STRING__

#include <stddef.h>
#include <stdint.h>
#include <iterator>

#include "ibasic_string.h"
#include "char_traits.h"
#include "container.h"
#include "alignment.h"
#include "array.h"

//*****************************************************************************
///\defgroup basic_string basic_string
/// A basic_string with the capacity defined at compile time.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// A basic_string implementation that uses a fixed size buffer.
  ///\tparam T The element type.
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup basic_string
  //***************************************************************************
  template <typename T, const size_t MAX_SIZE_>
  class basic_string : public ibasic_string<T>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    basic_string()
      : ibasic_string<T>(reinterpret_cast<T*>(&buffer), MAX_SIZE)
    {
      ibasic_string<T>::initialise();
    }

    //*************************************************************************
    /// Constructor, from null terminated text.
    ///\param text The initial text of the basic_string.
    //*************************************************************************
    basic_string(const T* text)
      : ibasic_string<T>(reinterpret_cast<T*>(&buffer), MAX_SIZE)
    {
      ibasic_string<T>::initialise();
      ibasic_string<T>::assign(text, text + etl::char_traits<T>::length(text));
    }

    //*************************************************************************
    /// Constructor, from null terminated text and count.
    ///\param text  The initial text of the basic_string.
    ///\param count The number of characters to copy.
    //*************************************************************************
    basic_string(const T* text, size_t count)
      : ibasic_string<T>(reinterpret_cast<T*>(&buffer), MAX_SIZE)
    {
      ibasic_string<T>::initialise();
      ibasic_string<T>::assign(text, text + count);
    }
    
    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initialSize  The initial size of the basic_string.
    ///\param value        The value to fill the basic_string with.
    //*************************************************************************
    basic_string(size_t count, T c)
      : ibasic_string<T>(reinterpret_cast<T*>(&buffer), MAX_SIZE)
    {
      ibasic_string<T>::initialise();
      ibasic_string<T>::resize(count, c);
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    basic_string(TIterator first, TIterator last)
      : ibasic_string<T>(reinterpret_cast<T*>(&buffer), MAX_SIZE)
    {
      ibasic_string<T>::assign(first, last);
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    basic_string& operator = (const basic_string& rhs)
    {
      if (&rhs != this)
      {
        ibasic_string<T>::assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

  private:

    T buffer[MAX_SIZE + 1];
  };
}

#endif
