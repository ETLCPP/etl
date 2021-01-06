///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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

namespace etl
{
  //*****************************************************************************
  /// The interface for a memory block pool.
  //*****************************************************************************
  class imemory_block_allocator
  {
  public:

    //*****************************************************************************
    /// Default constructor.
    //*****************************************************************************
    imemory_block_allocator()
      : p_successor(ETL_NULLPTR)
    {
    }

    //*****************************************************************************
    /// Try to allocate a memory block of the required size.
    /// If this allocator cannot, then pass the request on the the successor, if configured.
    //*****************************************************************************
    void* allocate(size_t required_size)
    {
      // Call the derived implementation.
      void* p = allocate_block(required_size);

      // If that failed...
      if (p == ETL_NULLPTR)
      {
        /// ...and we have a successor...
        if (has_successor())
        {
          // Try to allocate from the next one in the chain.
          return get_successor().allocate(required_size);
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
      bool successful = release_block(p);

      // Call the derived implementation to try to release.
      if (!successful)
      {
        // If it failed and we have a successor...
        if (has_successor())
        {
          // Try to release from the next one in the chain.
          successful = get_successor().release(p);
        }
      }

      return successful;
    }

    //*****************************************************************************
    /// Set the sucessor allocator.
    //*****************************************************************************
    void set_successor(etl::imemory_block_allocator& successor)
    {
      p_successor = &successor;
    }

    //*****************************************************************************
    /// Get the sucessor allocator.
    //*****************************************************************************
    etl::imemory_block_allocator& get_successor() const
    {
      return *p_successor;
    }

    //*****************************************************************************
    /// Do we have a successor allocator.
    //*****************************************************************************
    bool has_successor() const
    {
      return (p_successor != ETL_NULLPTR);
    }

  protected:

    virtual void* allocate_block(size_t required_size) = 0;
    virtual bool release_block(const void* const) = 0;

  private:

    etl::imemory_block_allocator* p_successor;
  };
}

#endif
