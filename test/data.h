/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#ifndef ETL_TEST_DATA_INCLUDED
#define ETL_TEST_DATA_INCLUDED

#include <ostream>
#include <utility>

#include "etl/instance_count.h"

//*****************************************************************************
// Default constructor.
//*****************************************************************************
template <typename T>
class TestDataDC : public etl::instance_count<TestDataDC<T>>
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

  virtual ~TestDataDC()
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

  bool operator <= (const TestDataDC& other) const
  {
    return !(value > other.value);
  }

  bool operator >= (const TestDataDC& other) const
  {
    return !(value < other.value);
  }

  static bool are_identical(const TestDataDC& lhs, const TestDataDC& rhs)
  {
#include "etl/private/diagnostic_float_equal_push.h"
    return (lhs.value == rhs.value) && (lhs.index == rhs.index);
#include "etl/private/diagnostic_pop.h"
  }

  T   value;
  int index;
};

template <typename T>
bool operator == (const TestDataDC<T>& lhs, const TestDataDC<T>& rhs)
{
#include "etl/private/diagnostic_float_equal_push.h"
  return lhs.value == rhs.value;
#include "etl/private/diagnostic_pop.h"
}

template <typename T>
bool operator != (const TestDataDC<T>& lhs, const TestDataDC<T>& rhs)
{
#include "etl/private/diagnostic_float_equal_push.h"
  return lhs.value != rhs.value;
#include "etl/private/diagnostic_pop.h"
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
class TestDataNDC : public etl::instance_count<TestDataNDC<T>>
{
public:

  explicit TestDataNDC(const T& value_, int index_ = 0)
    : value(value_),
      index(index_)
  {}

  virtual ~TestDataNDC()
  {
  }

  TestDataNDC(const TestDataNDC&) = default;
  TestDataNDC& operator =(const TestDataNDC&) = default;

  bool operator < (const TestDataNDC& other) const
  {
    return value < other.value;
  }

  bool operator > (const TestDataNDC& other) const
  {
    return value > other.value;
  }

  bool operator <= (const TestDataNDC& other) const
  {
    return !(value > other.value);
  }

  bool operator >= (const TestDataNDC& other) const
  {
    return !(value < other.value);
  }

  static bool are_identical(const TestDataNDC& lhs, const TestDataNDC& rhs)
  {
#include "etl/private/diagnostic_float_equal_push.h"
    return (lhs.value == rhs.value) && (lhs.index == rhs.index);
#include "etl/private/diagnostic_pop.h"
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

//*****************************************************************************
// Movable.
//*****************************************************************************
template <typename T>
class TestDataM : public etl::instance_count<TestDataM<T>>
{
public:

  explicit TestDataM(const T& value_)
    : value(value_)
    , valid(true)
  {
  }

  explicit TestDataM(T&& value_)
    : value(std::move(value_))
    , valid(true)
  {
  }

  TestDataM(TestDataM&& other) noexcept
    : value(std::move(other.value))
    , valid(true)
  {
    other.valid = false;
  }

  virtual ~TestDataM()
  {
    valid = false;
  }

  TestDataM& operator =(TestDataM&& other) noexcept
  {
    value = std::move(other.value);
    valid = true;

    other.valid = false;

    return *this;
  }

  bool operator < (const TestDataM& other) const
  {
    return value < other.value;
  }

  bool operator > (const TestDataM& other) const
  {
    return other.value < value;
  }

  bool operator <= (const TestDataM& other) const
  {
    return !(other.value < value);
  }

  bool operator >= (const TestDataM& other) const
  {
    return !(value < other.value);
  }

  operator bool() const
  {
    return valid;
  }

  T    value;
  bool valid;

private:

  TestDataM(const TestDataM& other) = delete;
  TestDataM& operator =(const TestDataM& other) = delete;
};

template <typename T>
bool operator == (const TestDataM<T>& lhs, const TestDataM<T>& rhs)
{
#include "etl/private/diagnostic_float_equal_push.h"
  return lhs.value == rhs.value;
#include "etl/private/diagnostic_pop.h"
}

template <typename T>
bool operator != (const TestDataM<T>& lhs, const TestDataM<T>& rhs)
{
#include "etl/private/diagnostic_float_equal_push.h"
  return lhs.value != rhs.value;
#include "etl/private/diagnostic_pop.h"
}

template <typename T>
std::ostream& operator << (std::ostream& s, const TestDataM<T>& rhs)
{
  s << rhs.value;
  return s;
}

#endif
