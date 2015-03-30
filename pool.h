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

#ifndef __ETL_POOL__
#define __ETL_POOL__

#include "alignment.h"
#include "array.h"
#include "bitset.h"
#include "ipool.h"

#include <iterator>

//*****************************************************************************
///\defgroup pool pool
/// A fixed capacity pool.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //*************************************************************************
  /// A templated pool implementation that uses a fixed size pool.
  ///\ingroup pool
  //*************************************************************************
  template <typename T, const size_t SIZE_>
  class pool : public ipool<T>
  {
  public:

    static const size_t SIZE = SIZE_;

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    pool()
      : ipool<T>(reinterpret_cast<T*>(&buffer[0]), in_use, SIZE)
    {
    }

    //*************************************************************************
    /// Destructor
    //*************************************************************************
    ~pool()
    {
      ipool<T>::release_all();
    }

  private:

    ///< The memory for the pool of objects.
    typename etl::aligned_storage<sizeof(T), etl::alignment_of<T>::value>::type buffer[SIZE];

    ///< The set of flags that indicate which items are free in the pool.
    bitset<SIZE> in_use;
  };
}

#endif

