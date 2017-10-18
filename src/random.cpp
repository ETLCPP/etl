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

#include "platform.h"
#include "random.h"

namespace etl
{
  //***************************************************************************
  // XOR Shift
  //***************************************************************************

  //***************************************************************************
  /// Default constructor.
  /// Attempts to come up with a unique non-zero seed.
  //***************************************************************************
  random_xorshift::random_xorshift()
  {
    // An attempt to come up with a unique non-zero seed,
    // based on the address of the instance.
    uintptr_t n    = reinterpret_cast<uintptr_t>(this);
    uint32_t  seed = static_cast<uint32_t>(n);
    initialise(seed);
  }

  //***************************************************************************
  /// Constructor with seed value.
  ///\param seed The new seed value.
  //***************************************************************************
  random_xorshift::random_xorshift(uint32_t seed)
  {
    initialise(seed);
  }

  //***************************************************************************
  /// Initialises the sequence with a new seed value.
  ///\param seed The new seed value.
  //***************************************************************************
  void random_xorshift::initialise(uint32_t seed)
  {
    // Add the first four primes to ensure that the seed isn't zero.
    state[0] = seed + 3;
    state[1] = seed + 5;
    state[2] = seed + 7;
    state[3] = seed + 11;
  }

  //***************************************************************************
  /// Get the next random_xorshift number.
  //***************************************************************************
  uint32_t random_xorshift::operator()()
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

  //***************************************************************************
  /// Get the next random_xorshift number in a specified inclusive range.
  //***************************************************************************
  uint32_t random_xorshift::range(uint32_t low, uint32_t high)
  {
    uint32_t r = high - low + 1;
    uint32_t n = operator()();
    n %= r;
    n += low;

    return n;
  }

  //***************************************************************************
  // Linear Congruential Generator
  //***************************************************************************

  //***************************************************************************
  /// Default constructor.
  /// Attempts to come up with a unique non-zero seed.
  //***************************************************************************
  random_lcg::random_lcg()
  {
    // An attempt to come up with a unique non-zero seed,
    // based on the address of the instance.
    uintptr_t n    = reinterpret_cast<uintptr_t>(this);
    uint32_t  seed = static_cast<uint32_t>(n);
    initialise(seed);
  }

  //***************************************************************************
  /// Constructor with seed value.
  ///\param seed The new seed value.
  //***************************************************************************
  random_lcg::random_lcg(uint32_t seed)
  {
    initialise(seed);
  }

  //***************************************************************************
  /// Initialises the sequence with a new seed value.
  ///\param seed The new seed value.
  //***************************************************************************
  void random_lcg::initialise(uint32_t seed)
  {
    seed = (seed == 0) ? 1 : seed;
    value = (seed > m) ? m : seed;
  }

  //***************************************************************************
  /// Get the next random_clcg number.
  //***************************************************************************
  uint32_t random_lcg::operator()()
  {
    value = (a * value) % m;

    return value;
  }

  //***************************************************************************
  /// Get the next random_clcg number in a specified inclusive range.
  //***************************************************************************
  uint32_t random_lcg::range(uint32_t low, uint32_t high)
  {
    uint32_t r = high - low + 1;
    uint32_t n = operator()();
    n %= r;
    n += low;

    return n;
  }

  //***************************************************************************
  // Combined Linear Congruential Generator
  //***************************************************************************

  //***************************************************************************
  /// Default constructor.
  /// Attempts to come up with a unique non-zero seed.
  //***************************************************************************
  random_clcg::random_clcg()
  {
    // An attempt to come up with a unique non-zero seed,
    // based on the address of the instance.
    uintptr_t n = reinterpret_cast<uintptr_t>(this);
    uint32_t  seed = static_cast<uint32_t>(n);
    initialise(seed);
  }

  //***************************************************************************
  /// Constructor with seed value.
  ///\param seed The new seed value.
  //***************************************************************************
  random_clcg::random_clcg(uint32_t seed)
  {
    initialise(seed);
  }

  //***************************************************************************
  /// Initialises the sequence with a new seed value.
  ///\param seed The new seed value.
  //***************************************************************************
  void random_clcg::initialise(uint32_t seed)
  {
    seed = (seed == 0) ? 1 : seed;
    value1 = (seed > m1) ? m1 : seed;
    value2 = (seed > m1) ? m1 : seed;
  }

  //***************************************************************************
  /// Get the next random_clcg number.
  //***************************************************************************
  uint32_t random_clcg::operator()()
  {
    static const uint32_t m = ((m1 > m2) ? m1 : m2);

    value1 = (a1 * value1) % m1;
    value2 = (a2 * value2) % m2;

    return (value1 + value2) % m;
  }

  //***************************************************************************
  /// Get the next random_clcg number in a specified inclusive range.
  //***************************************************************************
  uint32_t random_clcg::range(uint32_t low, uint32_t high)
  {
    uint32_t r = high - low + 1;
    uint32_t n = operator()();
    n %= r;
    n += low;

    return n;
  }

  //***************************************************************************
  // Linear Shift Feedback Register
  //***************************************************************************

  //***************************************************************************
  /// Default constructor.
  /// Attempts to come up with a unique non-zero seed.
  //***************************************************************************
  random_lsfr::random_lsfr()
  {
    // An attempt to come up with a unique non-zero seed,
    // based on the address of the instance.
    uintptr_t n    = reinterpret_cast<uintptr_t>(this);
    uint32_t  seed = static_cast<uint32_t>(n);
    initialise(seed);
  }

  //***************************************************************************
  /// Constructor with seed value.
  ///\param seed The new seed value.
  //***************************************************************************
  random_lsfr::random_lsfr(uint32_t seed)
  {
    initialise(seed);
  }

  //***************************************************************************
  /// Initialises the sequence with a new seed value.
  ///\param seed The new seed value.
  //***************************************************************************
  void random_lsfr::initialise(uint32_t seed)
  {
    value = seed;
  }

  //***************************************************************************
  /// Get the next random_lsfr number.
  //***************************************************************************
  uint32_t random_lsfr::operator()()
  {
    static const uint32_t polynomial = 0x80200003;

    value >>= 1;

    if ((value & 1) == 1)
    {
      value ^= polynomial;
    }

    return value;
  }

  //***************************************************************************
  /// Get the next random_lsfr number in a specified inclusive range.
  //***************************************************************************
  uint32_t random_lsfr::range(uint32_t low, uint32_t high)
  {
    uint32_t r = high - low + 1;
    uint32_t n = operator()();
    n %= r;
    n += low;

    return n;
  }

  //***************************************************************************
  // Multiply with carry random number generator.
  //***************************************************************************

  //***************************************************************************
  /// Default constructor.
  /// Attempts to come up with a unique non-zero seed.
  //***************************************************************************
  random_mwc::random_mwc()
  {
    // An attempt to come up with a unique non-zero seed,
    // based on the address of the instance.
    uintptr_t n = reinterpret_cast<uintptr_t>(this);
    uint32_t  seed = static_cast<uint32_t>(n);
    initialise(seed);
  }

  //***************************************************************************
  /// Constructor with seed value.
  ///\param seed The new seed value.
  //***************************************************************************
  random_mwc::random_mwc(uint32_t seed)
  {
    initialise(seed);
  }

  //***************************************************************************
  /// Initialises the sequence with a new seed value.
  ///\param seed The new seed value.
  //***************************************************************************
  void random_mwc::initialise(uint32_t seed)
  {
    value1 = seed;
    value2 = seed;
  }

  //***************************************************************************
  /// Get the next random_lsfr number.
  //***************************************************************************
  uint32_t random_mwc::operator()()
  {
    value1 = 36969 * (value1 & 0xFFFF) + (value1 >> 16);
    value2 = 18000 * (value2 & 0xFFFF) + (value2 >> 16);

    return (value1 << 16) + value2;
  }

  //***************************************************************************
  /// Get the next random_lsfr number in a specified inclusive range.
  //***************************************************************************
  uint32_t random_mwc::range(uint32_t low, uint32_t high)
  {
    uint32_t r = high - low + 1;
    uint32_t n = operator()();
    n %= r;
    n += low;

    return n;
  }
}
