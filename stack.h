///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

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

#ifndef __ETL_STACK__
#define __ETL_STACK__

#include <stddef.h>
#include <stdint.h>
#include <algorithm>

#include "istack.h"
#include "container.h"
#include "alignment.h"
#include "array.h"

//*****************************************************************************
///\defgroup stack stack
/// A First-in / first-out stack with the capacity defined at compile time,
/// written in the STL style.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  ///\ingroup stack
  /// A fixed capacity stack.
  /// This stack does not suppo7rt concurrent access by different threads.
  /// \tparam T        The type this stack should support.
  /// \tparam MAX_SIZE The maximum capacity of the stack.
  //***************************************************************************
  template <typename T, const size_t SIZE>
  class stack : public istack<T>
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    stack()
      : istack<T>(reinterpret_cast<T*>(&buffer[0]), SIZE)
    {
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    stack(const stack& rhs)
      : istack<T>(reinterpret_cast<T*>(&buffer[0]), SIZE)
    {
      istack<T>::clone(rhs);
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~stack()
    {
      istack<T>::clear();
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    stack& operator = (const stack& rhs)
    {
      if (&rhs != this)
      {
        istack<T>::clone(rhs);
      }

      return *this;
    }

  private:

    /// The unititialised buffer of T used in the stack.
    typename etl::aligned_storage<sizeof(T), etl::alignment_of<T>::value>::type buffer[SIZE];
  };
}

#endif
