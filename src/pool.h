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
  template <typename T, const size_t SIZE_, typename TIndex = int32_t>
  class pool : public ipool<T, TIndex>
  {
  public:

    static const size_t SIZE = SIZE_;

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    pool()
      : ipool<T, TIndex>(reinterpret_cast<Element*>(&buffer[0]), SIZE)
    {
    }

    //*************************************************************************
    /// Destructor
    //*************************************************************************
    ~pool()
    {
      ipool<T, TIndex>::release_all();
    }

  private:

    typedef typename ipool<T, TIndex>::Element Element;

    ///< The memory for the pool of objects.
    typename etl::aligned_storage<sizeof(Element), etl::alignment_of<Element>::value>::type buffer[SIZE];

    // Should not be copied.
    pool(const pool&);
    pool& operator =(const pool&);
  };
}

#endif

