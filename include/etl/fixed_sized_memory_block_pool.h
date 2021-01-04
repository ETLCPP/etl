///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

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

#ifndef ETL_FIXED_MEMORY_BLOCK_POOL_INCLUDED
#define ETL_FIXED_MEMORY_BLOCK_POOL_INCLUDED

#include "platform.h"
#include "memory.h"
#include "pool.h"
#include "alignment.h"

namespace etl
{
  //*************************************************************************
  /// The fixed sized memory block pool.
  /// The allocated memory blocks are all the same size.
  //*************************************************************************
  template <size_t BLOCK_SIZE, size_t ALIGNMENT, size_t SIZE>
  struct fixed_sized_memory_block_pool : public etl::ipool
  {
  public:

    fixed_sized_memory_block_pool()
      : ipool(buffer.get_address<char>(), BLOCK_SIZE, SIZE)
    {
    }

  private:

    // No copying allowed.
    fixed_sized_memory_block_pool(const etl::fixed_sized_memory_block_pool<BLOCK_SIZE, ALIGNMENT, SIZE>&) ETL_DELETE;
    fixed_sized_memory_block_pool& operator =(const etl::fixed_sized_memory_block_pool<BLOCK_SIZE, ALIGNMENT, SIZE>&) ETL_DELETE;

    typename etl::aligned_storage<SIZE * BLOCK_SIZE, ALIGNMENT>::type buffer;
  };
}

#endif
