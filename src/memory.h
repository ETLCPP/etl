///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#ifndef __ETL_MEMORY__
#define __ETL_MEMORY__

#include <iterator>

///\defgroup memory memory
///\ingroup etl
namespace etl
{
  //*****************************************************************************
  /// Fills uninitialised memory range with a value.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator, typename T>
  TIterator uninitialized_fill(TIterator o_begin, TIterator o_end, const T& value)
  {
    typedef typename std::iterator_traits<TIterator>::value_type value_type;

    while (o_begin != o_end)
    {
      ::new (static_cast<void*>(&*o_begin)) value_type(value);
      ++o_begin;
    }

    return o_begin;
  }

  //*****************************************************************************
  /// Fills uninitialised memory range with a value.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator, typename T, typename TCounter>
  TIterator uninitialized_fill(TIterator o_begin, TIterator o_end, const T& value, TCounter& count)
  {
    typedef typename std::iterator_traits<TIterator>::value_type value_type;

    while (o_begin != o_end)
    {
      ::new (static_cast<void*>(&*o_begin)) value_type(value);
      ++o_begin;
      ++count;
    }

    return o_begin;
  }

  //*****************************************************************************
  /// Fills uninitialised memory with N values.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator, typename TSize, typename T>
  TIterator uninitialized_fill_n(TIterator o_begin, TSize n, const T& value)
  {
    return etl::uninitialized_fill(o_begin, o_begin + n, value);
  }

  //*****************************************************************************
  /// Fills uninitialised memory with N values.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator, typename TSize, typename T, typename TCounter>
  TIterator uninitialized_fill_n(TIterator o_begin, TSize n, const T& value, TCounter& count)
  {
    return etl::uninitialized_fill(o_begin, o_begin + n, value, count);
  }

  //*****************************************************************************
  /// Copies a range of objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TInputIterator, typename TOutputIterator>
  TOutputIterator uninitialized_copy(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin)
  {
    typedef typename std::iterator_traits<TOutputIterator>::value_type value_type;

    while (i_begin != i_end)
    {
      ::new (static_cast<void*>(&*o_begin)) value_type(*i_begin);
      ++i_begin;
      ++o_begin;
    }

    return o_begin;
  }

  //*****************************************************************************
  /// Copies a range of objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TInputIterator, typename TOutputIterator, typename TCounter>
  TOutputIterator uninitialized_copy(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin, TCounter& count)
  {
    typedef typename std::iterator_traits<TOutputIterator>::value_type value_type;

    while (i_begin != i_end)
    {
      ::new (static_cast<void*>(&*o_begin)) value_type(*i_begin);
      ++i_begin;
      ++o_begin;
      ++count;
    }

    return o_begin;
  }

  //*****************************************************************************
  /// Copies N objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TInputIterator, typename TSize, typename TOutputIterator>
  TOutputIterator uninitialized_copy_n(TInputIterator i_begin, TSize n, TOutputIterator o_begin)
  {
    return etl::uninitialized_copy(i_begin, i_begin + n, o_begin);
  }

  //*****************************************************************************
  /// Copies N objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TInputIterator, typename TSize, typename TOutputIterator, typename TCounter>
  TOutputIterator uninitialized_copy_n(TInputIterator i_begin, TSize n, TOutputIterator o_begin, TCounter& count)
  {
    return etl::uninitialized_copy(i_begin, i_begin + n, o_begin, count);
  }
}

#endif
