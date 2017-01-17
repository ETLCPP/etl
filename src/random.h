
#ifndef __ETL_RANDOM_INCLUDED_
#define __ETL_RANDOM_INCLUDED_

#include <stdint.h>

namespace etl
{
  //***************************************************************************
  /// A 32 bit Mersenne Twister random number generator.
  /// Based on C/C++ code on Wikipedia.
  /// https://en.wikipedia.org/wiki/Mersenne_Twister#C.2FC.2B.2B_implementation
  //***************************************************************************
  class random
  {
    public:

      random();
      random(uint32_t seed);
      void initialise(uint32_t seed);
      uint32_t get() const;

  private:

    void twist();

    uint32_t  mt[N];
    uint16_t  index;
  };
}

#endif