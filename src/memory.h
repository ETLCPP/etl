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
#include <algorithm>

#include "type_traits.h"

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
  typename etl::enable_if<etl::has_trivial_constructor<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_fill(TOutputIterator o_begin, TOutputIterator o_end, const T& value)
  {
    std::fill(o_begin, o_end, value);

    return o_end;
  }

  //*****************************************************************************
  /// Fills uninitialised memory range with a value.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename T>
  typename etl::enable_if<!etl::has_trivial_constructor<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_fill(TOutputIterator o_begin, TOutputIterator o_end, const T& value)
  {
    typedef typename std::iterator_traits<TOutputIterator>::value_type value_type;

    while (o_begin != o_end)
    {
      ::new (static_cast<void*>(etl::addressof(*o_begin))) value_type(value);
      ++o_begin;
    }

    return o_end;
  }

  //*****************************************************************************
  /// Fills uninitialised memory range with a value.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename T, typename TCounter>
  typename etl::enable_if<etl::has_trivial_constructor<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_fill(TOutputIterator o_begin, TOutputIterator o_end, const T& value, TCounter& count)
  {
    count += std::distance(o_begin, o_end);

    std::fill(o_begin, o_end, value);
    
    return o_end;
  }

  //*****************************************************************************
  /// Fills uninitialised memory range with a value.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename T, typename TCounter>
  typename etl::enable_if<!etl::has_trivial_constructor<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_fill(TOutputIterator o_begin, TOutputIterator o_end, const T& value, TCounter& count)
  {
    count += std::distance(o_begin, o_end);

    etl::uninitialized_fill(o_begin, o_end, value);

    return o_end;
  }

  //*****************************************************************************
  /// Fills uninitialised memory with N values.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize, typename T>
  inline TOutputIterator uninitialized_fill_n(TOutputIterator o_begin, TSize n, const T& value)
  {
    return etl::uninitialized_fill(o_begin, o_begin + n, value);
  }

  //*****************************************************************************
  /// Fills uninitialised memory with N values.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize, typename T, typename TCounter>
  inline TOutputIterator uninitialized_fill_n(TOutputIterator o_begin, TSize n, const T& value, TCounter& count)
  {
    count += n;

    return etl::uninitialized_fill(o_begin, o_begin + n, value);
  }

  //*****************************************************************************
  /// Copies a range of objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TInputIterator, typename TOutputIterator>
  typename etl::enable_if<etl::has_trivial_constructor<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_copy(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin)
  {
    return std::copy(i_begin, i_end, o_begin);
  }

  //*****************************************************************************
  /// Copies a range of objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TInputIterator, typename TOutputIterator>
  typename etl::enable_if<!etl::has_trivial_constructor<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_copy(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin)
  {
    typedef typename std::iterator_traits<TOutputIterator>::value_type value_type;

    TOutputIterator o_end = o_begin;

    while (i_begin != i_end)
    {
      ::new (static_cast<void*>(etl::addressof(*o_end))) value_type(*i_begin);
      ++i_begin;
      ++o_end;
    }

    return o_end;
  }

  //*****************************************************************************
  /// Copies a range of objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TInputIterator, typename TOutputIterator, typename TCounter>
  typename etl::enable_if<etl::has_trivial_constructor<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_copy(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin, TCounter& count)
  {
    TOutputIterator o_end = std::copy(i_begin, i_end, o_begin);
    count += std::distance(o_begin, o_end);

    return o_end;
  }

  //*****************************************************************************
  /// Copies a range of objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TInputIterator, typename TOutputIterator, typename TCounter>
  typename etl::enable_if<!etl::has_trivial_constructor<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_copy(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin, TCounter& count)
  {
    TOutputIterator o_end = etl::uninitialized_copy(i_begin, i_end, o_begin);

    count += std::distance(o_begin, o_end);

    return o_end;
  }

  //*****************************************************************************
  /// Copies N objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TInputIterator, typename TSize, typename TOutputIterator>
  inline TOutputIterator uninitialized_copy_n(TInputIterator i_begin, TSize n, TOutputIterator o_begin)
  {
    return etl::uninitialized_copy(i_begin, i_begin + n, o_begin);
  }

  //*****************************************************************************
  /// Copies N objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TInputIterator, typename TSize, typename TOutputIterator, typename TCounter>
  inline TOutputIterator uninitialized_copy_n(TInputIterator i_begin, TSize n, TOutputIterator o_begin, TCounter& count)
  {
    count += n;

    return etl::uninitialized_copy(i_begin, i_begin + n, o_begin);
  }

  //*****************************************************************************
  /// Default contruct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
  typename etl::enable_if<etl::has_trivial_constructor<T>::value, void>::type
   create_default_at(T* /*p*/)
  {
  }

  //*****************************************************************************
  /// Default contruct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T, typename TCounter>
  typename etl::enable_if<etl::has_trivial_constructor<T>::value, void>::type
   create_default_at(T* /*p*/, TCounter& count)
  {
    ++count;
  }

  //*****************************************************************************
  /// Default contruct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
  typename etl::enable_if<!etl::has_trivial_constructor<T>::value, void>::type
   create_default_at(T* p)
  {
    ::new (p) T;
  }

  //*****************************************************************************
  /// Default contruct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T, typename TCounter>
  typename etl::enable_if<!etl::has_trivial_constructor<T>::value, void>::type
   create_default_at(T* p, TCounter& count)
  {
    ::new (p) T;
    ++count;
  }

  //*****************************************************************************
  /// Default initialises a range of objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator>
  typename etl::enable_if<etl::has_trivial_constructor<typename std::iterator_traits<TOutputIterator>::value_type>::value, void>::type
   uninitialized_default_construct(TOutputIterator /*o_begin*/, TOutputIterator /*o_end*/)
  {
  }

  //*****************************************************************************
  /// Default initialises a range of objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator>
  typename etl::enable_if<!etl::has_trivial_constructor<typename std::iterator_traits<TOutputIterator>::value_type>::value, void>::type
   uninitialized_default_construct(TOutputIterator o_begin, TOutputIterator o_end)
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
  typename etl::enable_if<etl::has_trivial_constructor<typename std::iterator_traits<TOutputIterator>::value_type>::value, void>::type
   uninitialized_default_construct(TOutputIterator o_begin, TOutputIterator o_end, TCounter& count)
  {
    count = std::distance(o_begin, o_end);
  }

  //*****************************************************************************
  /// Default initialises a range of objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TCounter>
  typename etl::enable_if<!etl::has_trivial_constructor<typename std::iterator_traits<TOutputIterator>::value_type>::value, void>::type
   uninitialized_default_construct(TOutputIterator o_begin, TOutputIterator o_end, TCounter& count)
  {
    count += std::distance(o_begin, o_end);

    etl::uninitialized_default_construct(o_begin, o_end);
  }

  //*****************************************************************************
  /// Default initialises N objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize>
  typename etl::enable_if<etl::has_trivial_constructor<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_default_construct_n(TOutputIterator o_begin, TSize n)
  {
    TOutputIterator o_end = o_begin + n;

    return o_end;
  }

  //*****************************************************************************
  /// Default initialises N objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize>  
  typename etl::enable_if<!etl::has_trivial_constructor<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_default_construct_n(TOutputIterator o_begin, TSize n)
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
  typename etl::enable_if<etl::has_trivial_constructor<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_default_construct_n(TOutputIterator o_begin, TSize n, TCounter& count)
  {
    TOutputIterator o_end = o_begin + n;

    count += n;

    return o_end;
  }

  //*****************************************************************************
  /// Default initialises N objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize, typename TCounter>
  typename etl::enable_if<!etl::has_trivial_constructor<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_default_construct_n(TOutputIterator o_begin, TSize n, TCounter& count)
  {
    TOutputIterator o_end = o_begin + n;

    etl::uninitialized_default_construct(o_begin, o_end);

    count += n;

    return o_end;
  }

  //*****************************************************************************
  /// Value construct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
  inline void create_value_at(T* p)
  {
    ::new (p) T();
  }

  //*****************************************************************************
  /// Value construct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T, typename TCounter>
  inline void create_value_at(T* p, TCounter& count)
  {
    ::new (p) T();
    ++count;
  }

  //*****************************************************************************
  /// Copy construct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
  inline void create_copy_at(T* p, const T& value)
  {
    ::new (p) T(value);
  }

  //*****************************************************************************
  /// Copy construct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T, typename TCounter>
  inline void create_copy_at(T* p, const T& value, TCounter& count)
  {
    ::new (p) T(value);
    ++count;
  }

  //*****************************************************************************
  /// Default initialises a range of objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator>
  typename etl::enable_if<etl::has_trivial_constructor<typename std::iterator_traits<TOutputIterator>::value_type>::value, void>::type
   uninitialized_value_construct(TOutputIterator o_begin, TOutputIterator o_end)
  {
    typedef typename std::iterator_traits<TOutputIterator>::value_type value_type;

    std::fill(o_begin, o_end, value_type());
  }

  //*****************************************************************************
  /// Default initialises a range of objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator>
  typename etl::enable_if<!etl::has_trivial_constructor<typename std::iterator_traits<TOutputIterator>::value_type>::value, void>::type
   uninitialized_value_construct(TOutputIterator o_begin, TOutputIterator o_end)
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
    count += std::distance(o_begin, o_end);

    etl::uninitialized_value_construct(o_begin, o_end);
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

    etl::uninitialized_value_construct(o_begin, o_end);

    count += n;

    return o_end;
  }

  //*****************************************************************************
  /// Destroys an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
  typename etl::enable_if<etl::has_trivial_destructor<T>::value, void>::type
   destroy_at(T* /*p*/)
  {
  }

  //*****************************************************************************
  /// Destroys an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
  typename etl::enable_if<!etl::has_trivial_destructor<T>::value, void>::type
   destroy_at(T* p)
  {
    p->~T();
  }

  //*****************************************************************************
  /// Destroys an item at address p.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T, typename TCounter>
  typename etl::enable_if<etl::has_trivial_destructor<T>::value, void>::type
   destroy_at(T* /*p*/, TCounter& count)
  {
    --count;
  }

  //*****************************************************************************
  /// Destroys an item at address p.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T, typename TCounter>
  typename etl::enable_if<!etl::has_trivial_destructor<T>::value, void>::type
   destroy_at(T* p, TCounter& count)
  {
    p->~T();
    --count;
  }

  //*****************************************************************************
  /// Destroys a range of items.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator>
  typename etl::enable_if<etl::has_trivial_destructor<typename std::iterator_traits<TIterator>::value_type>::value, void>::type
   destroy(TIterator /*i_begin*/, TIterator /*i_end*/)
  {
  }

  //*****************************************************************************
  /// Destroys a range of items.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator>
  typename etl::enable_if<!etl::has_trivial_destructor<typename std::iterator_traits<TIterator>::value_type>::value, void>::type
   destroy(TIterator i_begin, TIterator i_end)
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
  typename etl::enable_if<etl::has_trivial_destructor<typename std::iterator_traits<TIterator>::value_type>::value, void>::type
   destroy(TIterator i_begin, TIterator i_end, TCounter& count)
  {
    count -= std::distance(i_begin, i_end);
  }

  //*****************************************************************************
  /// Destroys a range of items.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator, typename TCounter>
  typename etl::enable_if<!etl::has_trivial_destructor<typename std::iterator_traits<TIterator>::value_type>::value, void>::type
   destroy(TIterator i_begin, TIterator i_end, TCounter& count)
  {
    count -= std::distance(i_begin, i_end);

    while (i_begin != i_end)
    {
      etl::destroy_at(etl::addressof(*i_begin));
      ++i_begin;
    }
  }

  //*****************************************************************************
  /// Destroys a number of items.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator, typename TSize>
  typename etl::enable_if<etl::has_trivial_destructor<typename std::iterator_traits<TIterator>::value_type>::value, TIterator>::type
   destroy_n(TIterator i_begin, TSize n)
  {
    return i_begin + n;
  }

  //*****************************************************************************
  /// Destroys a number of items.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator, typename TSize>
  typename etl::enable_if<!etl::has_trivial_destructor<typename std::iterator_traits<TIterator>::value_type>::value, TIterator>::type
   destroy_n(TIterator i_begin, TSize n)
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
  typename etl::enable_if<etl::has_trivial_destructor<typename std::iterator_traits<TIterator>::value_type>::value, TIterator>::type
   destroy_n(TIterator i_begin, TSize n, TCounter& count)
  {
    count -= n;
    return i_begin + n;
  }

  //*****************************************************************************
  /// Destroys a number of items.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator, typename TSize, typename TCounter>
  typename etl::enable_if<!etl::has_trivial_destructor<typename std::iterator_traits<TIterator>::value_type>::value, TIterator>::type
   destroy_n(TIterator i_begin, TSize n, TCounter& count)
  {
    count -= n;

    while (n > 0)
    {
      etl::destroy_at(etl::addressof(*i_begin));
      ++i_begin;
      --n;
    }

    return i_begin;
  }
}

#endif
