//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

#ifndef _MURMURHASH3_H_
#define _MURMURHASH3_H_

//-----------------------------------------------------------------------------
// Platform-specific functions and macros

// Microsoft Visual Studio

#include "etl/platform.h"

#if defined(ETL_COMPILER_MICROSOFT) && (_MSC_VER < 1600)

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
#if ETL_USING_64BIT_TYPES
typedef unsigned __int64 uint64_t;
#endif

// Other compilers

#else   // defined(ETL_COMPILER_MICROSOFT)

#include <stdint.h>

#endif // !defined(ETL_COMPILER_MICROSOFT)

//-----------------------------------------------------------------------------

void MurmurHash3_x86_32(const void * key, int len, uint32_t seed, void * out);

void MurmurHash3_x86_128(const void * key, int len, uint32_t seed, void * out);

void MurmurHash3_x64_128(const void * key, int len, uint32_t seed, void * out);

//-----------------------------------------------------------------------------

#endif // _MURMURHASH3_H_

