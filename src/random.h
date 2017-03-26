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

#ifndef __ETL_RANDOM__
#define __ETL_RANDOM__

#include <stdint.h>

namespace etl
{
  //***************************************************************************
  /// The base for all 32 bit random number generators.
  //***************************************************************************
  class random
  {
  public:

    virtual ~random()
    {
    }

    virtual void initialise(uint32_t seed) = 0;
    virtual uint32_t operator()() = 0;
  };

  //***************************************************************************
  /// A 32 bit random number generator.
  /// Uses a 128 bit XOR shift algorithm.
  /// https://en.wikipedia.org/wiki/Xorshift
  //***************************************************************************
  class random_xorshift : public random
  {
    public:

      random_xorshift();
      random_xorshift(uint32_t seed);
      void initialise(uint32_t seed);
      uint32_t operator()();

    private:

      uint32_t state[4];
  };
}

#endif