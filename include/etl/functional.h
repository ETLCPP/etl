///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2014 jwellbelove

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

#ifndef ETL_FUNCTIONAL_INCLUDED
#define ETL_FUNCTIONAL_INCLUDED

#include "platform.h"

///\defgroup functional functional
///\ingroup utilities

///\defgroup reference_wrapper reference_wrapper
///\ingroup functional

namespace etl
{
  //***************************************************************************
  /// A definition of reference_wrapper for those that don't have C++ 0x11 support.
  ///\ingroup reference
  //***************************************************************************
  template <typename T>
  class reference_wrapper
  {
  public:

    typedef T type;

    explicit reference_wrapper(T& t_)
      : t(&t_)
    {
    }

    operator T& () const
    {
      return *t;
    }

    reference_wrapper<T>& operator = (T value)
    {
      *t = value;
      return *this;
    }
    
    T& get() const
    {
      return *t;
    }

  private:

    T* t;
  };

  //***************************************************************************
  template <typename T>
  reference_wrapper<T> ref(T& t)
  {
    return reference_wrapper<T>(t);
  }

  //***************************************************************************
  template <typename T>
  reference_wrapper<T> ref(reference_wrapper<T> t)
  {
    return reference_wrapper<T>(t.get());
  }

  //***************************************************************************
  template <typename T>
  reference_wrapper<const T> cref(const T& t)
  {
    return reference_wrapper<const T>(t);
  }

  //***************************************************************************
  template <typename T>
  reference_wrapper<const T> cref(reference_wrapper<T> t)
  {
    return reference_wrapper<const T>(t.get());
  }
}

#endif

