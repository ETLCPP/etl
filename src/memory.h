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
  /// Gets the address of an object.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
  T* addressof(T& t)
  {
      return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(t)));
  }

  //*****************************************************************************
  /// Fills uninitialised memory range with a value.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename T>
  TOutputIterator uninitialized_fill(TOutputIterator o_begin, TOutputIterator o_end, const T& value)
  {
    typedef typename std::iterator_traits<TOutputIterator>::value_type value_type;

    while (o_begin != o_end)
    {
      ::new (static_cast<void*>(etl::addressof(*o_begin))) value_type(value);
      ++o_begin;
    }

    return o_begin;
  }

  //*****************************************************************************
  /// Fills uninitialised memory range with a value.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename T, typename TCounter>
  TOutputIterator uninitialized_fill(TOutputIterator o_begin, TOutputIterator o_end, const T& value, TCounter& count)
  {
    typedef typename std::iterator_traits<TOutputIterator>::value_type value_type;

    while (o_begin != o_end)
    {
      ::new (static_cast<void*>(etl::addressof(*o_begin))) value_type(value);
      ++o_begin;
      ++count;
    }

    return o_begin;
  }

  //*****************************************************************************
  /// Fills uninitialised memory with N values.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize, typename T>
  TOutputIterator uninitialized_fill_n(TOutputIterator o_begin, TSize n, const T& value)
  {
    return etl::uninitialized_fill(o_begin, o_begin + n, value);
  }

  //*****************************************************************************
  /// Fills uninitialised memory with N values.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize, typename T, typename TCounter>
  TOutputIterator uninitialized_fill_n(TOutputIterator o_begin, TSize n, const T& value, TCounter& count)
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
      ::new (static_cast<void*>(etl::addressof(*o_begin))) value_type(*i_begin);
      ++i_begin;
      ++o_begin;
    }

    return o_begin;
  }

  //*****************************************************************************
  /// Copies a range of objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TInputIterator, typename TOutputIterator, typename TCounter>
  TOutputIterator uninitialized_copy(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin, TCounter& count)
  {
    typedef typename std::iterator_traits<TOutputIterator>::value_type value_type;

    while (i_begin != i_end)
    {
      ::new (static_cast<void*>(etl::addressof(*o_begin))) value_type(*i_begin);
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
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TInputIterator, typename TSize, typename TOutputIterator, typename TCounter>
  TOutputIterator uninitialized_copy_n(TInputIterator i_begin, TSize n, TOutputIterator o_begin, TCounter& count)
  {
    return etl::uninitialized_copy(i_begin, i_begin + n, o_begin, count);
  }

  //*****************************************************************************
  /// Default initialises a range of objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator>
  void uninitialized_default_construct(TOutputIterator o_begin, TOutputIterator o_end)
  {
    typedef typename std::iterator_traits<TOutputIterator>::value_type value_type;

    while (o_begin != o_end)
    {
      ::new (static_cast<void*>(etl::addressof(*o_begin))) value_type;
      ++o_begin;
    }
  }

  //*****************************************************************************
  /// Default initialises a range of objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TCounter>
  void uninitialized_default_construct(TOutputIterator o_begin, TOutputIterator o_end, TCounter& count)
  {
    typedef typename std::iterator_traits<TOutputIterator>::value_type value_type;

    while (o_begin != o_end)
    {
      ::new (static_cast<void*>(etl::addressof(*o_begin))) value_type;
      ++o_begin;
      ++count;
    }
  }

  //*****************************************************************************
  /// Default initialises N objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize>
  TOutputIterator uninitialized_default_construct_n(TOutputIterator o_begin, TSize n)
  {
    TOutputIterator o_end = o_begin + n;

    etl::uninitialized_default_construct(o_begin, o_end);

    return o_end;
  }

  //*****************************************************************************
  /// Default initialises N objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize, typename TCounter>
  TOutputIterator uninitialized_default_construct_n(TOutputIterator o_begin, TSize n, TCounter& count)
  {
    TOutputIterator o_end = o_begin + n;

    etl::uninitialized_default_construct(o_begin, o_end, count);

    return o_end;
  }

  //*****************************************************************************
  /// Default initialises a range of objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator>
  void uninitialized_value_construct(TOutputIterator o_begin, TOutputIterator o_end)
  {
    typedef typename std::iterator_traits<TOutputIterator>::value_type value_type;

    while (o_begin != o_end)
    {
      ::new (static_cast<void*>(etl::addressof(*o_begin))) value_type();
      ++o_begin;
    }
  }

  //*****************************************************************************
  /// Default initialises a range of objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TCounter>
  void uninitialized_value_construct(TOutputIterator o_begin, TOutputIterator o_end, TCounter& count)
  {
    typedef typename std::iterator_traits<TOutputIterator>::value_type value_type;

    while (o_begin != o_end)
    {
      ::new (static_cast<void*>(etl::addressof(*o_begin))) value_type();
      ++o_begin;
      ++count;
    }
  }

  //*****************************************************************************
  /// Default initialises N objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize>
  TOutputIterator uninitialized_value_construct_n(TOutputIterator o_begin, TSize n)
  {
    TOutputIterator o_end = o_begin + n;

    etl::uninitialized_value_construct(o_begin, o_end);

    return o_end;
  }

  //*****************************************************************************
  /// Default initialises N objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize, typename TCounter>
  TOutputIterator uninitialized_value_construct_n(TOutputIterator o_begin, TSize n, TCounter& count)
  {
    TOutputIterator o_end = o_begin + n;

    etl::uninitialized_value_construct(o_begin, o_end, count);

    return o_end;
  }

  //*****************************************************************************
  /// Destroys an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
  inline void destroy_at(T* p)
  {
    p->~T();
  }

  //*****************************************************************************
  /// Destroys an item at address p.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T, typename TCounter>
  inline void destroy_at(T* p, TCounter& count)
  {
    p->~T();
    --count;
  }

  //*****************************************************************************
  /// Destroys a range of items.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator>
  void destroy(TIterator i_begin, TIterator i_end)
  {
    while (i_begin != i_end)
    {
      etl::destroy_at(etl::addressof(*i_begin));
      ++i_begin;
    }
  }

  //*****************************************************************************
  /// Destroys a range of items.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator, typename TCounter>
  void destroy(TIterator i_begin, TIterator i_end, TCounter& count)
  {
    while (i_begin != i_end)
    {
      etl::destroy_at(etl::addressof(*i_begin));
      ++i_begin;
      --count;
    }
  }

  //*****************************************************************************
  /// Destroys a number of items.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator, typename TSize>
  TIterator destroy_n(TIterator i_begin, TSize n)
  {
    while (n > 0)
    {
      etl::destroy_at(etl::addressof(*i_begin));
      ++i_begin;
      --n;
    }

    return i_begin;
  }

  //*****************************************************************************
  /// Destroys a number of items.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator, typename TSize, typename TCounter>
  TIterator destroy_n(TIterator i_begin, TSize n, TCounter& count)
  {
    while (n > 0)
    {
      etl::destroy_at(etl::addressof(*i_begin));
      ++i_begin;
      --n;
      --count;
    }

    return i_begin;
  }
}

#endif
