//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

// Note - The x86 and x64 versions do _not_ produce the same results, as the
// algorithms are optimized for their respective platforms. You can still
// compile and run any of them on any platform, but your performance with the
// non-native version will be less than optimal.

#include "etl/platform.h"

#ifdef ETL_COMPILER_GCC
  #pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#endif

#include "murmurhash3.h"

//-----------------------------------------------------------------------------
// Platform-specific functions and macros

// Microsoft Visual Studio

#if defined(ETL_COMPILER_MICROSOFT)

#define FORCE_INLINE    __forceinline

#include <stdlib.h>

#define ROTL32(x,y)     _rotl(x,y)
#define ROTL64(x,y)     _rotl64(x,y)

#define BIG_CONSTANT(x) (x##UI64)

// Other compilers

#else   // defined(ETL_COMPILER_MICROSOFT)

#define FORCE_INLINE inline __attribute__((always_inline))

inline uint32_t rotl32(uint32_t x, int8_t r)
{
  return (x << r) | (x >> (32U - r));
}

inline uint64_t rotl64(uint64_t x, int8_t r)
{
  return (x << r) | (x >> (64U - r));
}

#define ROTL32(x,y)     rotl32(x,y)
#define ROTL64(x,y)     rotl64(x,y)

#define BIG_CONSTANT(x) (x##LLU)

#endif // !defined(ETL_COMPILER_MICROSOFT)

//-----------------------------------------------------------------------------
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here

FORCE_INLINE uint32_t getblock32(const uint32_t * p, int i)
{
  return p[i];
}

#if ETL_USING_64BIT_TYPES
FORCE_INLINE uint64_t getblock64(const uint64_t * p, int i)
{
  return p[i];
}
#endif

//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche

FORCE_INLINE uint32_t fmix32(uint32_t h)
{
  h ^= h >> 16U;
  h *= 0x85ebca6bUL;
  h ^= h >> 13U;
  h *= 0xc2b2ae35UL;
  h ^= h >> 16U;

  return h;
}

//----------

#if ETL_USING_64BIT_TYPES
FORCE_INLINE uint64_t fmix64(uint64_t k)
{
  k ^= k >> 33U;
  k *= BIG_CONSTANT(0xff51afd7ed558ccd);
  k ^= k >> 33U;
  k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
  k ^= k >> 33U;

  return k;
}
#endif

//-----------------------------------------------------------------------------

void MurmurHash3_x86_32(const void * key, int len, uint32_t seed, void * out)
{
  const uint8_t * data = (const uint8_t*)key;
  const int nblocks = len / 4;

  uint32_t h1 = seed;

  const uint32_t c1 = 0xcc9e2d51UL;
  const uint32_t c2 = 0x1b873593UL;

  //----------
  // body

  const uint32_t * blocks = (const uint32_t *)(data + nblocks * 4);

  for (int i = -nblocks; i; i++)
  {
    uint32_t k1 = getblock32(blocks, i);

    k1 *= c1;
    k1 = ROTL32(k1, 15);
    k1 *= c2;

    h1 ^= k1;
    h1 = ROTL32(h1, 13);
    h1 = h1 * 5 + 0xe6546b64UL;
  }

  //----------
  // tail

  const uint8_t * tail = data + nblocks * 4;

  uint32_t k1 = 0U;

  switch (len & 3)
  {
  case 3: k1 ^= tail[2] << 16U;
  case 2: k1 ^= tail[1] << 8U;
  case 1: k1 ^= tail[0];
    k1 *= c1; k1 = ROTL32(k1, 15); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= len;

  h1 = fmix32(h1);

  *(uint32_t*)out = h1;
}

//-----------------------------------------------------------------------------

void MurmurHash3_x86_128(const void * key, const int len,
  uint32_t seed, void * out)
{
  const uint8_t * data = (const uint8_t*)key;
  const int nblocks = len / 16;

  uint32_t h1 = seed;
  uint32_t h2 = seed;
  uint32_t h3 = seed;
  uint32_t h4 = seed;

  const uint32_t c1 = 0x239b961bUL;
  const uint32_t c2 = 0xab0e9789UL;
  const uint32_t c3 = 0x38b34ae5UL;
  const uint32_t c4 = 0xa1e38b93UL;

  //----------
  // body

  const uint32_t * blocks = (const uint32_t *)(data + nblocks * 16);

  for (int i = -nblocks; i; i++)
  {
    uint32_t k1 = getblock32(blocks, i * 4 + 0);
    uint32_t k2 = getblock32(blocks, i * 4 + 1);
    uint32_t k3 = getblock32(blocks, i * 4 + 2);
    uint32_t k4 = getblock32(blocks, i * 4 + 3);

    k1 *= c1; k1 = ROTL32(k1, 15); k1 *= c2; h1 ^= k1;

    h1 = ROTL32(h1, 19); h1 += h2; h1 = h1 * 5 + 0x561ccd1bUL;

    k2 *= c2; k2 = ROTL32(k2, 16); k2 *= c3; h2 ^= k2;

    h2 = ROTL32(h2, 17); h2 += h3; h2 = h2 * 5 + 0x0bcaa747UL;

    k3 *= c3; k3 = ROTL32(k3, 17); k3 *= c4; h3 ^= k3;

    h3 = ROTL32(h3, 15); h3 += h4; h3 = h3 * 5 + 0x96cd1c35UL;

    k4 *= c4; k4 = ROTL32(k4, 18); k4 *= c1; h4 ^= k4;

    h4 = ROTL32(h4, 13); h4 += h1; h4 = h4 * 5 + 0x32ac3b17UL;
  }

  //----------
  // tail

  const uint8_t * tail = data + nblocks * 16;

  uint32_t k1 = 0U;
  uint32_t k2 = 0U;
  uint32_t k3 = 0U;
  uint32_t k4 = 0U;

  switch (len & 15)
  {
  case 15: k4 ^= tail[14] << 16U;
  case 14: k4 ^= tail[13] << 8U;
  case 13: k4 ^= tail[12] << 0U;
    k4 *= c4; k4 = ROTL32(k4, 18); k4 *= c1; h4 ^= k4;

  case 12: k3 ^= tail[11] << 24U;
  case 11: k3 ^= tail[10] << 16U;
  case 10: k3 ^= tail[9] << 8U;
  case  9: k3 ^= tail[8] << 0U;
    k3 *= c3; k3 = ROTL32(k3, 17); k3 *= c4; h3 ^= k3;

  case  8: k2 ^= tail[7] << 24U;
  case  7: k2 ^= tail[6] << 16U;
  case  6: k2 ^= tail[5] << 8U;
  case  5: k2 ^= tail[4] << 0U;
    k2 *= c2; k2 = ROTL32(k2, 16); k2 *= c3; h2 ^= k2;

  case  4: k1 ^= tail[3] << 24U;
  case  3: k1 ^= tail[2] << 16U;
  case  2: k1 ^= tail[1] << 8U;
  case  1: k1 ^= tail[0] << 0U;
    k1 *= c1; k1 = ROTL32(k1, 15); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= len; h2 ^= len; h3 ^= len; h4 ^= len;

  h1 += h2; h1 += h3; h1 += h4;
  h2 += h1; h3 += h1; h4 += h1;

  h1 = fmix32(h1);
  h2 = fmix32(h2);
  h3 = fmix32(h3);
  h4 = fmix32(h4);

  h1 += h2; h1 += h3; h1 += h4;
  h2 += h1; h3 += h1; h4 += h1;

  ((uint32_t*)out)[0] = h1;
  ((uint32_t*)out)[1] = h2;
  ((uint32_t*)out)[2] = h3;
  ((uint32_t*)out)[3] = h4;
}

//-----------------------------------------------------------------------------

#if ETL_USING_64BIT_TYPES
void MurmurHash3_x64_128(const void * key, const int len,
  const uint32_t seed, void * out)
{
  const uint8_t * data = (const uint8_t*)key;
  const int nblocks = len / 16;

  uint64_t h1 = seed;
  uint64_t h2 = seed;

  const uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
  const uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);

  //----------
  // body

  const uint64_t * blocks = (const uint64_t *)(data);

  for (int i = 0; i < nblocks; i++)
  {
    uint64_t k1 = getblock64(blocks, i * 2 + 0);
    uint64_t k2 = getblock64(blocks, i * 2 + 1);

    k1 *= c1; k1 = ROTL64(k1, 31); k1 *= c2; h1 ^= k1;

    h1 = ROTL64(h1, 27); h1 += h2; h1 = h1 * 5 + 0x52dce729UL;

    k2 *= c2; k2 = ROTL64(k2, 33); k2 *= c1; h2 ^= k2;

    h2 = ROTL64(h2, 31); h2 += h1; h2 = h2 * 5 + 0x38495ab5UL;
  }

  //----------
  // tail

  const uint8_t * tail = data + nblocks * 16;

  uint64_t k1 = 0ULL;
  uint64_t k2 = 0ULL;

  switch (len & 15)
  {
  case 15: k2 ^= ((uint64_t)tail[14]) << 48U;
  case 14: k2 ^= ((uint64_t)tail[13]) << 40U;
  case 13: k2 ^= ((uint64_t)tail[12]) << 32U;
  case 12: k2 ^= ((uint64_t)tail[11]) << 24U;
  case 11: k2 ^= ((uint64_t)tail[10]) << 16U;
  case 10: k2 ^= ((uint64_t)tail[9]) << 8U;
  case  9: k2 ^= ((uint64_t)tail[8]) << 0U;
    k2 *= c2; k2 = ROTL64(k2, 33); k2 *= c1; h2 ^= k2;

  case  8: k1 ^= ((uint64_t)tail[7]) << 56U;
  case  7: k1 ^= ((uint64_t)tail[6]) << 48U;
  case  6: k1 ^= ((uint64_t)tail[5]) << 40U;
  case  5: k1 ^= ((uint64_t)tail[4]) << 32U;
  case  4: k1 ^= ((uint64_t)tail[3]) << 24U;
  case  3: k1 ^= ((uint64_t)tail[2]) << 16U;
  case  2: k1 ^= ((uint64_t)tail[1]) << 8U;
  case  1: k1 ^= ((uint64_t)tail[0]) << 0U;
    k1 *= c1; k1 = ROTL64(k1, 31); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= len; h2 ^= len;

  h1 += h2;
  h2 += h1;

  h1 = fmix64(h1);
  h2 = fmix64(h2);

  h1 += h2;
  h2 += h1;

  ((uint64_t*)out)[0] = h1;
  ((uint64_t*)out)[1] = h2;
}
#endif
