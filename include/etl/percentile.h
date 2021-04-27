///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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

#ifndef ETL_PERCENTILE_INCLUDED
#define ETL_PERCENTILE_INCLUDED

#include "platform.h"
#include "iterator.h"
#include "histogram.h"
#include "algorithm.h"

namespace etl
{
  //***************************************************************************
  /// Get the iterator to the histogram element at the specified percentile.
  /// Uses the nearest-rank method.
  //***************************************************************************
  template <typename TIterator, typename TPercentile>
  TIterator percentile(TIterator first, TIterator last, TPercentile percentile)
  {
    typedef typename etl::iterator_traits<TIterator>::difference_type difference_type;

    size_t length = size_t(etl::distance(first, last));
    size_t index  = size_t(((length * percentile) + TPercentile(99)) / TPercentile(100));

    if (index != 0)
    {
      --index;
    }

    // Return the iterator to the value at the percentile.
    etl::advance(first, index);
    return first;
  }

  //***************************************************************************
  /// Get the iterator to the histogram element where the sum of the histogram
  /// values are at least the specified percentage of the total.
  //***************************************************************************
  template <typename TIterator, typename TPercentile>
  typename TIterator histogram_percentile(TIterator first, TIterator last, TPercentile percentile)
  {
    uint32_t total = etl::accumulate(first, last, uint32_t(0U));
    uint32_t sum   = 0U;

    while (first != last)
    {
      sum += *first;

      // Return when we have reached or exceeded the required percentile.
      if (((TPercentile(100) * sum) / total) >= percentile)
      {
        return first;
      }

      ++first;
    }

    return first;
  }
}

#endif