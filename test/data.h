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

#ifndef __test_data__
#define __test_data__

#include <ostream>

//*****************************************************************************
// Default constructor.
//*****************************************************************************
template <typename T>
class TestDataDC
{
public:

  TestDataDC()
  : value(T()),
    index(0)
  {
  }

  explicit TestDataDC(const T& value_, int index_ = 0)
    : value(value_),
      index(index_)
  {
  }

  bool operator < (const TestDataDC& other) const
  {
    return value < other.value;
  }

  bool operator > (const TestDataDC& other) const
  {
    return value > other.value;
  }

  T   value;
  int index;
};

template <typename T>
bool operator == (const TestDataDC<T>& lhs, const TestDataDC<T>& rhs)
{
  return lhs.value == rhs.value;
}

template <typename T>
bool operator != (const TestDataDC<T>& lhs, const TestDataDC<T>& rhs)
{
  return lhs.value != rhs.value;
}

template <typename T>
std::ostream& operator << (std::ostream& s, const TestDataDC<T>& rhs)
{
  s << rhs.value;
  return s;
}

//*****************************************************************************
// No default constructor.
//*****************************************************************************
template <typename T>
class TestDataNDC
{
public:

  explicit TestDataNDC(const T& value_, int index_ = 0)
    : value(value_),
      index(index_)
  {}

  bool operator < (const TestDataNDC& other) const
  {
    return value < other.value;
  }

  bool operator > (const TestDataNDC& other) const
  {
    return value > other.value;
  }

  T value;
  int index;
};

template <typename T>
bool operator == (const TestDataNDC<T>& lhs, const TestDataNDC<T>& rhs)
{
  return lhs.value == rhs.value;
}

template <typename T>
bool operator != (const TestDataNDC<T>& lhs, const TestDataNDC<T>& rhs)
{
  return lhs.value != rhs.value;
}

template <typename T>
std::ostream& operator << (std::ostream& s, const TestDataNDC<T>& rhs)
{
  s << rhs.value;
  return s;
}

#endif
