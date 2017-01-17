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

namespace etl
{
  //***************************************************************************
  /// Default constructor.
  /// Attempts to come up with a reasonable non-zero seed.
  //***************************************************************************
  random::random()
  {
    // An attempt to come up with a reasonable non-zero seed,
    // based on the address of the instance.
    uintptr_t n    = reinterpret_cast<uintptr_t>(this);
    uint32_t  seed = static_cast<uint32_t>(n);
    initialise(seed);
  }

  //***************************************************************************
  /// Constructor with seed value.
  ///\param seed The new seed value.
  //***************************************************************************
  random::random(uint32_t seed)
  {
    initialise(seed);
  }

  //***************************************************************************
  /// Initialises the sequence with a new seed value.
  ///\param seed The new seed value.
  //***************************************************************************
  void random::initialise(uint32_t seed)
  {
    state[0] = seed;
    state[1] = seed + 3;
    state[2] = seed + 5;
    state[3] = seed + 7;
  }

  //***************************************************************************
  /// Get the next random number.
  //***************************************************************************
  uint32_t random::operator()()
  {
    uint32_t n = state[3];
    n ^= n << 11;
    n ^= n >> 8;
    state[3] = state[2];
    state[2] = state[1];
    state[1] = state[0];
    n ^= state[0];
    n ^= state[0] >> 19;
    state[0] = n;

    return n;
  }
}