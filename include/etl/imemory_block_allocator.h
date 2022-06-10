///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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

#ifndef ETL_MEMORY_BLOCK_ALLOCATOR_INCLUDED
#define ETL_MEMORY_BLOCK_ALLOCATOR_INCLUDED

#include "platform.h"
#include "nullptr.h"
#include "successor.h"

namespace etl
{
  //*****************************************************************************
  /// The interface for a memory block pool.
  //*****************************************************************************
  class imemory_block_allocator : public successor<imemory_block_allocator>
  {
  public:

    //*****************************************************************************
    /// Default constructor.
    //*****************************************************************************
    imemory_block_allocator()
    {
    }

    //*****************************************************************************
    /// Try to allocate a memory block of the required size.
    /// If this allocator cannot, then pass the request on the the successor, if configured.
    //*****************************************************************************
    void* allocate(size_t required_size, size_t required_alignment)
    {
      // Call the derived implementation.
      void* p = allocate_block(required_size, required_alignment);

      // If that failed...
      if (p == ETL_NULLPTR)
      {
        /// ...and we have a successor...
        if (has_successor())
        {
          // ...try to allocate from the next one in the chain.
          return get_successor().allocate(required_size, required_alignment);
        }
      }

      return p;
    }

    //*****************************************************************************
    /// Try to release a memory block of the required size.
    /// If this allocator cannot, then pass the request on the the successor, if configured.
    //*****************************************************************************
    bool release(const void* const p)
    {
      bool was_released = release_block(p);

      // If that failed...
      if (!was_released)
      {
        /// ...and we have a successor...
        if (has_successor())
        {
          // ...try to release from the next one in the chain.
          was_released = get_successor().release(p);
        }
      }

      return was_released;
    }

    //*****************************************************************************
    /// Check if the memory block is owned by this allocator.
    /// If this allocator does not own it, then pass the request on the the successor, if configured.
    //*****************************************************************************
    bool is_owner_of(const void* const p) const
    {
      bool is_owner = is_owner_of_block(p);

      // If that failed...
      if (!is_owner)
      {
        /// ...and we have a successor...
        if (has_successor())
        {
          // ...check with the next one in the chain.
          is_owner = get_successor().is_owner_of(p);
        }
      }

      return is_owner;
    }

  protected:

    virtual void* allocate_block(size_t required_size, size_t required_alignment) = 0;
    virtual bool release_block(const void* const) = 0;
    virtual bool is_owner_of_block(const void* const) const = 0;

  private:

    // No copying allowed.
    imemory_block_allocator(const etl::imemory_block_allocator&) ETL_DELETE;
    imemory_block_allocator& operator =(const imemory_block_allocator&) ETL_DELETE;
  };
}

#endif
