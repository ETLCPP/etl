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

#include <assert.h>

#include "etl/initializer_list.h"

#if ETL_NOT_USING_STL

namespace
{
  template <typename T>
  class Container
  {
  public:

    constexpr Container(std::initializer_list<T> init)
      : buffer()
    {
      typename std::initializer_list<T>::const_iterator itr = std::begin(init);
      T* p = buffer;

      while (itr != std::end(init))
      {
        *p++ = *itr++;
      }
    }

    const T& operator [](int i) const
    {
      return buffer[i];
    }

  private:

    T buffer[10];
  };

  //***************************************************************************
  // Test constexpr.
  constexpr Container<int> container = { 1, 2, 3, 4, 5 };

  //***************************************************************************
  // Test non-constexpr.
  static Container<int> TestInitializerList()
  {
    static Container<int> container = { 1, 2, 3, 4, 5 };

    assert(container[0] == 1);
    assert(container[1] == 2);
    assert(container[2] == 3);
    assert(container[3] == 4);
    assert(container[4] == 5);

    return container;
  }
 
  static Container<int> c = TestInitializerList();
}

#endif
