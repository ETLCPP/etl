///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 John Wellbelove

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

#ifndef ETL_NO_STL_TEST_ITERATORS_INCLUDED
#define ETL_NO_STL_TEST_ITERATORS_INCLUDED

#include "etl/platform.h"
#include "etl/iterator.h"

template <typename T>
struct non_random_iterator : public etl::iterator<ETL_OR_STD::bidirectional_iterator_tag, T>
{
  non_random_iterator()
    : ptr(nullptr)
  {
  }

  non_random_iterator(T* v)
    : ptr(v)
  {
  }

  non_random_iterator(const non_random_iterator& other)
  {
    ptr = other.ptr;
  }

  non_random_iterator& operator =(const non_random_iterator& other)
  {
    ptr = other.ptr;
    return *this;
  }

  T& operator *()
  {
    return *ptr;
  }

  const T& operator *() const
  {
    return *ptr;
  }

  non_random_iterator& operator ++()
  {
    ++ptr;
    return *this;
  }

  non_random_iterator operator ++(int)
  {
    T* temp = ptr;
    ++ptr;
    return non_random_iterator(temp);
  }

  non_random_iterator& operator --()
  {
    --ptr;
    return *this;
  }

  non_random_iterator operator --(int)
  {
    T* temp = ptr;
    --ptr;
    return non_random_iterator(temp);
  }

  non_random_iterator& operator =(T* other)
  {
    ptr = other;
    return *this;
  }

  operator T*()
  {
    return ptr;
  }

  operator const T*()
  {
    return ptr;
  }

  T* ptr;
};

template <typename T>
bool operator !=(const non_random_iterator<T>& lhs, const non_random_iterator<T>& rhs)
{
  return lhs.ptr != rhs.ptr;
}

template <typename T>
struct random_iterator : public etl::iterator<ETL_OR_STD::random_access_iterator_tag, T>
{
  random_iterator()
    : ptr(nullptr)
  {
  }

  random_iterator(T* v)
    : ptr(v)
  {
  }

  random_iterator(const random_iterator& other)
  {
    ptr = other.ptr;
  }

  random_iterator& operator =(const random_iterator& other)
  {
    ptr = other.ptr;
    return *this;
  }

  T& operator *()
  {
    return *ptr;
  }

  const T& operator *() const
  {
    return *ptr;
  }

  random_iterator& operator ++()
  {
    ++ptr;
    return *this;
  }

  random_iterator operator ++(int)
  {
    T* temp = ptr;
    ++ptr;
    return random_iterator(temp);
  }

  random_iterator& operator --()
  {
    --ptr;
    return *this;
  }

  random_iterator operator --(int)
  {
    T* temp = ptr;
    --ptr;
    return random_iterator(temp);
  }

  random_iterator& operator +=(int n)
  {
    ptr += n;
    return *this;
  }

  random_iterator& operator -=(int n)
  {
    ptr -= n;
    return *this;
  }

  random_iterator& operator =(T* other)
  {
    ptr = other;
    return *this;
  }

  operator T*()
  {
    return ptr;
  }

  operator const T*()
  {
    return ptr;
  }

  T* ptr;
};

template <typename T>
ptrdiff_t operator -(const random_iterator<T>& lhs, const random_iterator<T>& rhs)
{
  return lhs.ptr - rhs.ptr;
}

#endif
