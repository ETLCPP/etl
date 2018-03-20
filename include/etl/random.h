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

#include "platform.h"

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

      random_xorshift();
      explicit random_xorshift(uint32_t seed);
      void initialise(uint32_t seed);
      uint32_t operator()();
      uint32_t range(uint32_t low, uint32_t high);

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

    random_lcg();
    explicit random_lcg(uint32_t seed);
    void initialise(uint32_t seed);
    uint32_t operator()();
    uint32_t range(uint32_t low, uint32_t high);

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

      random_clcg();
      explicit random_clcg(uint32_t seed);
      void initialise(uint32_t seed);
      uint32_t operator()();
      uint32_t range(uint32_t low, uint32_t high);

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
  /// https://en.wikipedia.org/wiki/Linear-feedback_shift_register
  //***************************************************************************
  class random_lsfr : public random
  {
    public:

      random_lsfr();
      explicit random_lsfr(uint32_t seed);
      void initialise(uint32_t seed);
      uint32_t operator()();
      uint32_t range(uint32_t low, uint32_t high);

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

    random_mwc();
    explicit random_mwc(uint32_t seed);
    void initialise(uint32_t seed);
    uint32_t operator()();
    uint32_t range(uint32_t low, uint32_t high);

  private:

    uint32_t value1;
    uint32_t value2;
  };
}

#endif
