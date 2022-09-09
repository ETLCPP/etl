///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

#include "platform.h"
#include "binary.h"

#include <stdint.h>

namespace etl
{
#if defined(ETL_POLYMORPHIC_RANDOM)
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
#else
  //***************************************************************************
  /// The base for all 32 bit random number generators.
  /// Empty, non-virtual.
  //***************************************************************************
  class random
  {
  protected:
    random()
    {
    }

    ~random()
    {
    }
  };
#endif

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
        n ^= n << 11U;
        n ^= n >> 8U;
        state[3] = state[2];
        state[2] = state[1];
        state[1] = state[0];
        n ^= state[0];
        n ^= state[0] >> 19U;
        state[0] = n;

        return n;
      }

      //***************************************************************************
      /// Get the next random_xorshift number in a specified inclusive range.
      //***************************************************************************
      uint32_t range(uint32_t low, uint32_t high)
      {
        uint32_t r = high - low + 1UL;
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
      uint32_t r = high - low + 1UL;
      uint32_t n = operator()();
      n %= r;
      n += low;

      return n;
    }

  private:

    static ETL_CONSTANT uint32_t a = 40014U;
    static ETL_CONSTANT uint32_t m = 2147483563UL;

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
        static ETL_CONSTANT uint32_t m = ((m1 > m2) ? m1 : m2);

        value1 = (a1 * value1) % m1;
        value2 = (a2 * value2) % m2;

        return (value1 + value2) % m;
      }

      //***************************************************************************
      /// Get the next random_clcg number in a specified inclusive range.
      //***************************************************************************
      uint32_t range(uint32_t low, uint32_t high)
      {
        uint32_t r = high - low + 1UL;
        uint32_t n = operator()();
        n %= r;
        n += low;

        return n;
      }

    private:

      static ETL_CONSTANT uint32_t a1 = 40014U;
      static ETL_CONSTANT uint32_t m1 = 2147483563UL;

      static ETL_CONSTANT uint32_t a2 = 40692U;
      static ETL_CONSTANT uint32_t m2 = 2147483399UL;

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
        static ETL_CONSTANT uint32_t polynomial = 0x80200003UL;

        value >>= 1U;

        if ((value & 1UL) == 1UL)
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
        uint32_t r = high - low + 1UL;
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
      value1 = 36969UL * (value1 & 0xFFFFUL) + (value1 >> 16U);
      value2 = 18000UL * (value2 & 0xFFFFUL) + (value2 >> 16U);

      return (value1 << 16U) + value2;
    }

    //***************************************************************************
    /// Get the next random_lsfr number in a specified inclusive range.
    //***************************************************************************
    uint32_t range(uint32_t low, uint32_t high)
    {
      uint32_t r = high - low + 1UL;
      uint32_t n = operator()();
      n %= r;
      n += low;

      return n;
    }

  private:

    uint32_t value1;
    uint32_t value2;
  };

#if ETL_USING_64BIT_TYPES
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
      value = uint64_t(seed) | (uint64_t(seed) << 32U);
    }

    //***************************************************************************
    /// Get the next random_lsfr number.
    //***************************************************************************
    uint32_t operator()()
    {
      uint64_t x = value;
      unsigned count = (unsigned)(value >> 59U);

      value = (x * multiplier) + increment;
      x ^= x >> 18U;
      return etl::rotate_right((uint32_t)(x >> 27U), count);
    }

    //***************************************************************************
    /// Get the next random_lsfr number in a specified inclusive range.
    //***************************************************************************
    uint32_t range(uint32_t low, uint32_t high)
    {
      uint32_t r = high - low + 1UL;
      uint32_t n = operator()();
      n %= r;
      n += low;

      return n;
    }

  private:

    static ETL_CONSTANT uint64_t multiplier = 6364136223846793005ULL;
    static ETL_CONSTANT uint64_t increment  = 1ULL;

    uint64_t value;
  };
#endif

#if ETL_USING_8BIT_TYPES
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
      uint32_t r = high - low + 1UL;
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
