/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/std
https://www.etlcpp.com

Copyright(c) 2022 jwellbelove

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

#include "etl/initializer_list.h"

#include <stdio.h>

template <typename T>
class Container
{
public:

//#if ETL_USING_INITIALIZER_LIST
  ETL_CONSTEXPR14 Container(std::initializer_list<T> init)
    : length(init.size())
    , buffer()
  {
    typename std::initializer_list<T>::const_iterator itr = std::begin(init);
    T* p = buffer;

    while (itr != std::end(init))
    {
      *p++ = *itr++;
    }
  }
//#endif

  const T& operator [](int i) const
  {
    return buffer[i];
  }

  const size_t length;

private:

  T buffer[10];
};

int main()
{
  int result = 0;

  Container<int> c = { 1, 2, 3, 4, 5 };

  if (c[0] != 1) result =  1;
  if (c[1] != 2) result =  2;
  if (c[2] != 3) result =  3;
  if (c[3] != 4) result =  4;
  if (c[4] != 5) result =  5;

  if (c.length != 5) result =  6;

  ETL_CONSTEXPR14 Container<int> cc = { 1, 2, 3, 4, 5 };

  if (cc[0] != 1) result =  7;
  if (cc[1] != 2) result =  8;
  if (cc[2] != 3) result =  9;
  if (cc[3] != 4) result =  10;
  if (cc[4] != 5) result =  11;

  if (cc.length != 5) result =  12;

  if (result == 0)
  {
    printf("**** All tests passed ****\n");
  }
  else
  {
    printf(">>>> Tests failed at #%d <<<<\n", result);
  }

  return result;
}

