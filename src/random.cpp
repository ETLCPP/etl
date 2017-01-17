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

#include "random.h"

namespace
{
  enum
  {
      W = 32
      N = 624,
      M = 397,
      R = 31,
      A = 0x9908B0DF,
      F = 1812433253,
      U = 11,
      S = 7,
      B = 0x9D2C5680,
      T = 15,
      C = 0xEFC60000,
      L = 18,

      MASK_LOWER = (1ull << R) - 1,
      MASK_UPPER = (1ull << R)
  };
}

namespace etl
{
  //***************************************************************************
  /// Default constructor.
  //***************************************************************************
  random::random()
  {
    // An attempt to come up with a reasonable non-zero seed.
    uintptr_t n = reinterpret_cast<uintptr_t>(this);
    uint32_t seed = static_cast<uint32_t>(n);
    initialise(seed);
  }

  //***************************************************************************
  /// Constructor with seed value.
  //***************************************************************************
  random::random(uint32_t seed)
  {
    initialise(seed);
  }

  //***************************************************************************
  /// Get the next random number.
  //***************************************************************************
  uint32_t random::get() const
  {
    uint32_t n;
    uint16_t i = index;

    if (index >= N)
    {
      twist();
      i = index;
    }

    n = mt[i];
    index = i + 1;

    n ^= (mt[i] >> U);
    n ^= (n << S) & B;
    n ^= (n << T) & C;
    n ^= (n >> L);

    return n;
  }

  //***************************************************************************
  /// Initialises the sequence with a new seed value.
  //***************************************************************************
  void random::initialize(uint32_t seed)
  {
    uint32_t i;

    mt[0] = seed;

    for (i = 1; i < N; i++)
    {
      mt[i] = (F * (mt[i - 1] ^ (mt[i - 1] >> (W - 2))) + i);
    }

    index = N;
  }

  //***************************************************************************
  /// Creates the next iteration.
  //***************************************************************************
  void random::twist()
  {
    for (uint32_t i = 0; i < N; i++ )
    {
      uint32_t x = (mt[i] & MASK_UPPER) + (mt[(i + 1) % N] & MASK_LOWER);

      uint32_t xA = x >> 1;

      if (x & 0x01)
      {
        xA ^= A;
      }

      mt[i] = mt[(i + M) % N] ^ xA;
    }

    index = 0;
  }
}