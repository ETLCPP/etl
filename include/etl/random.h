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

#ifndef ETL_RANDOM_INCLUDED
#define ETL_RANDOM_INCLUDED

#include <stdint.h>

#include "platform.h"
#include "binary.h"

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
    virtual uint32_t range(uint32_t low, uint32_t high) = 0;
  };

  //***************************************************************************
  /// A 32 bit random number generator.
  /// Uses a 128 bit XOR shift algorithm.
  /// https://en.wikipedia.org/wiki/Xorshift
  //***************************************************************************
  class random_xorshift : public random
  {
    public:

      //***************************************************************************
      /// Default constructor.
      /// Attempts to come up with a unique non-zero seed.
      //***************************************************************************
      random_xorshift()
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
      random_xorshift(uint32_t seed)
      {
        initialise(seed);
      }

      //***************************************************************************
      /// Initialises the sequence with a new seed value.
      ///\param seed The new seed value.
      //***************************************************************************
      void initialise(uint32_t seed)
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
      uint32_t operator()()
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
      uint32_t range(uint32_t low, uint32_t high)
      {
        uint32_t r = high - low + 1;
        uint32_t n = operator()();
        n %= r;
        n += low;

        return n;
      }

    private:

      uint32_t state[4];
  };

  //***************************************************************************
  /// A 32 bit random number generator.
  /// Uses a linear congruential generator.
  /// https://cs.adelaide.edu.au/~paulc/teaching/montecarlo/node107.html
  //***************************************************************************
  class random_lcg : public random
  {
  public:

    //***************************************************************************
    /// Default constructor.
    /// Attempts to come up with a unique non-zero seed.
    //***************************************************************************
    random_lcg()
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
    random_lcg(uint32_t seed)
    {
      initialise(seed);
    }

    //***************************************************************************
    /// Initialises the sequence with a new seed value.
    ///\param seed The new seed value.
    //***************************************************************************
    void initialise(uint32_t seed)
    {
      seed = (seed == 0) ? 1 : seed;
      value = (seed > m) ? m : seed;
    }

    //***************************************************************************
    /// Get the next random_clcg number.
    //***************************************************************************
    uint32_t operator()()
    {
      value = (a * value) % m;

      return value;
    }

    //***************************************************************************
    /// Get the next random_clcg number in a specified inclusive range.
    //***************************************************************************
    uint32_t range(uint32_t low, uint32_t high)
    {
      uint32_t r = high - low + 1;
      uint32_t n = operator()();
      n %= r;
      n += low;

      return n;
    }

  private:

    static const uint32_t a = 40014;
    static const uint32_t m = 2147483563;

    uint32_t value;
  };

  //***************************************************************************
  /// A 32 bit random number generator.
  /// Uses a combined linear congruential generator.
  /// https://cs.adelaide.edu.au/~paulc/teaching/montecarlo/node107.html
  //***************************************************************************
  class random_clcg : public random
  {
    public:

      //***************************************************************************
      /// Default constructor.
      /// Attempts to come up with a unique non-zero seed.
      //***************************************************************************
      random_clcg()
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
      random_clcg(uint32_t seed)
      {
        initialise(seed);
      }

      //***************************************************************************
      /// Initialises the sequence with a new seed value.
      ///\param seed The new seed value.
      //***************************************************************************
      void initialise(uint32_t seed)
      {
        seed = (seed == 0) ? 1 : seed;
        value1 = (seed > m1) ? m1 : seed;
        value2 = (seed > m1) ? m1 : seed;
      }

      //***************************************************************************
      /// Get the next random_clcg number.
      //***************************************************************************
      uint32_t operator()()
      {
        static const uint32_t m = ((m1 > m2) ? m1 : m2);

        value1 = (a1 * value1) % m1;
        value2 = (a2 * value2) % m2;

        return (value1 + value2) % m;
      }

      //***************************************************************************
      /// Get the next random_clcg number in a specified inclusive range.
      //***************************************************************************
      uint32_t range(uint32_t low, uint32_t high)
      {
        uint32_t r = high - low + 1;
        uint32_t n = operator()();
        n %= r;
        n += low;

        return n;
      }

    private:

      static const uint32_t a1 = 40014;
      static const uint32_t m1 = 2147483563;

      static const uint32_t a2 = 40692;
      static const uint32_t m2 = 2147483399;

      uint32_t value1;
      uint32_t value2;
  };

  //***************************************************************************
  /// A 32 bit random number generator.
  /// Uses a linear shift feedback register.
  /// Polynomial 0x80200003
  /// https://en.wikipedia.org/wiki/Linear-feedback_shift_register
  //***************************************************************************
  class random_lsfr : public random
  {
    public:

      //***************************************************************************
      /// Default constructor.
      /// Attempts to come up with a unique non-zero seed.
      //***************************************************************************
      random_lsfr()
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
      random_lsfr(uint32_t seed)
      {
        initialise(seed);
      }

      //***************************************************************************
      /// Initialises the sequence with a new seed value.
      ///\param seed The new seed value.
      //***************************************************************************
      void initialise(uint32_t seed)
      {
        value = seed;
      }

      //***************************************************************************
      /// Get the next random_lsfr number.
      //***************************************************************************
      uint32_t operator()()
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
      uint32_t range(uint32_t low, uint32_t high)
      {
        uint32_t r = high - low + 1;
        uint32_t n = operator()();
        n %= r;
        n += low;

        return n;
      }

    private:

      uint32_t value;
  };

  //***************************************************************************
  /// A 32 bit random number generator.
  /// Uses a multiply with carry calculation.
  //***************************************************************************
  class random_mwc : public random
  {
  public:

    //***************************************************************************
    /// Default constructor.
    /// Attempts to come up with a unique non-zero seed.
    //***************************************************************************
    random_mwc()
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
    random_mwc(uint32_t seed)
    {
      initialise(seed);
    }

    //***************************************************************************
    /// Initialises the sequence with a new seed value.
    ///\param seed The new seed value.
    //***************************************************************************
    void initialise(uint32_t seed)
    {
      value1 = seed;
      value2 = seed;
    }

    //***************************************************************************
    /// Get the next random_lsfr number.
    //***************************************************************************
    uint32_t operator()()
    {
      value1 = 36969 * (value1 & 0xFFFF) + (value1 >> 16);
      value2 = 18000 * (value2 & 0xFFFF) + (value2 >> 16);

      return (value1 << 16) + value2;
    }

    //***************************************************************************
    /// Get the next random_lsfr number in a specified inclusive range.
    //***************************************************************************
    uint32_t range(uint32_t low, uint32_t high)
    {
      uint32_t r = high - low + 1;
      uint32_t n = operator()();
      n %= r;
      n += low;

      return n;
    }

  private:

    uint32_t value1;
    uint32_t value2;
  };

  //***************************************************************************
  /// A 32 bit random number generator.
  /// Uses a permuted congruential generator calculation.
  /// https://en.wikipedia.org/wiki/Permuted_congruential_generator
  //***************************************************************************
  class random_pcg : public random
  {
  public:

    random_pcg()
    {
      // An attempt to come up with a unique non-zero seed,
      // based on the address of the instance.
      uintptr_t n = reinterpret_cast<uintptr_t>(this);
      value = static_cast<uint64_t>(n);
    }

    //***************************************************************************
    /// Constructor with seed value.
    ///\param seed The new seed value.
    //***************************************************************************
    random_pcg(uint32_t seed)
    {
      initialise(seed);
    }

    //***************************************************************************
    /// Initialises the sequence with a new seed value.
    ///\param seed The new seed value.
    //***************************************************************************
    void initialise(uint32_t seed)
    {
      value = uint64_t(seed) | (uint64_t(seed) << 32);
    }

    //***************************************************************************
    /// Get the next random_lsfr number.
    //***************************************************************************
    uint32_t operator()()
    {
      uint64_t x = value;
      unsigned count = (unsigned)(value >> 59);

      value = (x * multiplier) + increment;
      x ^= x >> 18;
      return etl::rotate_right((uint32_t)(x >> 27), count);
    }

    //***************************************************************************
    /// Get the next random_lsfr number in a specified inclusive range.
    //***************************************************************************
    uint32_t range(uint32_t low, uint32_t high)
    {
      uint32_t r = high - low + 1;
      uint32_t n = operator()();
      n %= r;
      n += low;

      return n;
    }

  private:

    static const uint64_t multiplier = 6364136223846793005ULL;
    static const uint64_t increment  = 1ULL;

    uint64_t value;
  };

#if ETL_8BIT_SUPPORT
  //***************************************************************************
  /// A 32 bit random number generator.
  /// Applies a user supplied 32bit hash to a counter.
  /// The hash must implement 'void add(uint8_t)' and 'uint8_t value()' member functions.
  //***************************************************************************
  template <typename THash>
  class random_hash : public random
  {
  public:

    random_hash()
    {
      // An attempt to come up with a unique non-zero seed,
      // based on the address of the instance.
      uintptr_t n = reinterpret_cast<uintptr_t>(this);
      value = static_cast<uint32_t>(n);
    }

    //***************************************************************************
    /// Constructor with seed value.
    ///\param seed The new seed value.
    //***************************************************************************
    random_hash(uint32_t seed)
    {
      initialise(seed);
    }

    //***************************************************************************
    /// Initialises the sequence with a new seed value.
    ///\param seed The new seed value.
    //***************************************************************************
    void initialise(uint32_t seed)
    {
      value = seed;
    }

    //***************************************************************************
    /// Get the next random_lsfr number.
    //***************************************************************************
    uint32_t operator()()
    {
      ++value;
      hash.add(value);
      return hash.value();
    }

    //***************************************************************************
    /// Get the next random_lsfr number in a specified inclusive range.
    //***************************************************************************
    uint32_t range(uint32_t low, uint32_t high)
    {
      uint32_t r = high - low + 1;
      uint32_t n = operator()();
      n %= r;
      n += low;

      return n;
    }

  private:

    THash   hash;
    uint8_t value;
  };
#endif
}

#endif
