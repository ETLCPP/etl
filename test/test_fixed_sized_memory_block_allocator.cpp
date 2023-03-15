/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 John Wellbelove

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

#include "unit_test_framework.h"

#include "etl/fixed_sized_memory_block_allocator.h"

namespace
{
  using Allocator8  = etl::fixed_sized_memory_block_allocator<sizeof(int8_t),  alignof(int8_t),  4>;
  using Allocator16 = etl::fixed_sized_memory_block_allocator<sizeof(int16_t), alignof(int16_t), 4>;
  using Allocator32 = etl::fixed_sized_memory_block_allocator<sizeof(int32_t), alignof(int32_t), 4>;

  class CustomAllocator8 : public Allocator8
  {
  public:

    bool allocate_block_called = false;
    bool release_block_called  = false;
    mutable bool is_owner_of_block_called = false;

  protected:

    //*************************************************************************
    /// The overridden virtual function to allocate a block.
    //*************************************************************************
    void* allocate_block(size_t required_size, size_t required_alignment) override
    {
      allocate_block_called = true;

      return Allocator8::allocate_block(required_size, required_alignment);
    }

    //*************************************************************************
    /// The overridden virtual function to release a block.
    //*************************************************************************
    bool release_block(const void* const pblock) override
    {
      release_block_called = true;

      return Allocator8::release_block(pblock);
    }

    //*************************************************************************
    /// Returns true if the allocator is the owner of the block.
    //*************************************************************************
    bool is_owner_of_block(const void* const pblock) const override
    {
      is_owner_of_block_called = true;

      return Allocator8::is_owner_of_block(pblock);
    }
  };

  SUITE(test_fixed_sized_memory_block_allocator)
  {
    //*************************************************************************
    TEST(test_allocator_no_successor_use_all_allocation)
    {
      Allocator16 allocator16;

      int16_t* p1 = static_cast<int16_t*>(allocator16.allocate(sizeof(int16_t), alignof(int16_t)));
      int16_t* p2 = static_cast<int16_t*>(allocator16.allocate(sizeof(int16_t), alignof(int16_t)));
      int16_t* p3 = static_cast<int16_t*>(allocator16.allocate(sizeof(int16_t), alignof(int16_t)));
      int16_t* p4 = static_cast<int16_t*>(allocator16.allocate(sizeof(int16_t), alignof(int16_t)));
      int16_t* p5 = static_cast<int16_t*>(allocator16.allocate(sizeof(int16_t), alignof(int16_t)));

      CHECK(p1 != nullptr);
      CHECK(p2 != nullptr);
      CHECK(p3 != nullptr);
      CHECK(p4 != nullptr);
      CHECK(p5 == nullptr);

      CHECK(allocator16.release(p1));
      CHECK(allocator16.release(p2));
      CHECK(allocator16.release(p3));
      CHECK(allocator16.release(p4));
      CHECK(!allocator16.release(p5));
    }

    //*************************************************************************
    TEST(test_allocator_has_successors)
    {
      Allocator16 allocator16;
      Allocator16 allocator16s;
      Allocator16 allocator16ss;

      allocator16.set_successor(allocator16s);
      allocator16s.set_successor(allocator16ss);

      CHECK(allocator16.has_successor()   == true);
      CHECK(allocator16s.has_successor()  == true);
      CHECK(allocator16ss.has_successor() == false);
    }

    //*************************************************************************
    TEST(test_allocator_with_successors_use_all_allocation)
    {          
      Allocator16 allocator16;
      Allocator16 allocator16s;
      Allocator16 allocator16ss;
      
      allocator16.set_successor(allocator16s);
      allocator16s.set_successor(allocator16ss);

      int16_t* p1 = static_cast<int16_t*>(allocator16.allocate(sizeof(int16_t), alignof(uint16_t)));
      int16_t* p2 = static_cast<int16_t*>(allocator16.allocate(sizeof(int16_t), alignof(uint16_t)));
      int16_t* p3 = static_cast<int16_t*>(allocator16.allocate(sizeof(int16_t), alignof(uint16_t)));
      int16_t* p4 = static_cast<int16_t*>(allocator16.allocate(sizeof(int16_t), alignof(uint16_t)));
      int16_t* p5 = static_cast<int16_t*>(allocator16.allocate(sizeof(int16_t), alignof(uint16_t)));
      int16_t* p6 = static_cast<int16_t*>(allocator16.allocate(sizeof(int16_t), alignof(uint16_t)));
      int16_t* p7 = static_cast<int16_t*>(allocator16.allocate(sizeof(int16_t), alignof(uint16_t)));
      int16_t* p8 = static_cast<int16_t*>(allocator16.allocate(sizeof(int16_t), alignof(uint16_t)));
      int16_t* p9 = static_cast<int16_t*>(allocator16.allocate(sizeof(int16_t), alignof(uint16_t)));
      int16_t* p10 = static_cast<int16_t*>(allocator16.allocate(sizeof(int16_t), alignof(uint16_t)));
      int16_t* p11 = static_cast<int16_t*>(allocator16.allocate(sizeof(int16_t), alignof(uint16_t)));
      int16_t* p12 = static_cast<int16_t*>(allocator16.allocate(sizeof(int16_t), alignof(uint16_t)));
      int16_t* p13 = static_cast<int16_t*>(allocator16.allocate(sizeof(int16_t), alignof(uint16_t)));

      CHECK(p1  != nullptr);
      CHECK(p2  != nullptr);
      CHECK(p3  != nullptr);
      CHECK(p4  != nullptr);
      CHECK(p5  != nullptr);
      CHECK(p6  != nullptr);
      CHECK(p7  != nullptr);
      CHECK(p8  != nullptr);
      CHECK(p9  != nullptr);
      CHECK(p10 != nullptr);
      CHECK(p11 != nullptr);
      CHECK(p12 != nullptr);
      CHECK(p13 == nullptr);

      CHECK(allocator16.release(p1));
      CHECK(allocator16.release(p2));
      CHECK(allocator16.release(p3));
      CHECK(allocator16.release(p4));
      CHECK(allocator16.release(p5));
      CHECK(allocator16.release(p6));
      CHECK(allocator16.release(p7));
      CHECK(allocator16.release(p8));
      CHECK(allocator16.release(p9));
      CHECK(allocator16.release(p10));
      CHECK(allocator16.release(p11));
      CHECK(allocator16.release(p12));
      CHECK(!allocator16.release(p13));
    }

    //*************************************************************************
    TEST(test_allocator_with_different_block_sized_successors)
    {
      Allocator8  allocator8;
      Allocator16 allocator16;
      Allocator32 allocator32;

      allocator8.set_successor(allocator16, allocator32);

      int8_t*  p1  = static_cast<int8_t*>(allocator8.allocate(sizeof(int8_t),   alignof(uint8_t)));  // Take from allocator8
      int16_t* p2  = static_cast<int16_t*>(allocator8.allocate(sizeof(int16_t), alignof(uint16_t))); // Take from allocator16
      int32_t* p3  = static_cast<int32_t*>(allocator8.allocate(sizeof(int32_t), alignof(uint32_t))); // Take from allocator32
      int64_t* p4  = static_cast<int64_t*>(allocator8.allocate(sizeof(int64_t), alignof(uint64_t))); // Unable to allocate
      int8_t*  p5  = static_cast<int8_t*>(allocator8.allocate(sizeof(int8_t),   alignof(uint8_t)));  // Take from allocator8
      int8_t*  p6  = static_cast<int8_t*>(allocator8.allocate(sizeof(int8_t),   alignof(uint8_t)));  // Take from allocator8
      int8_t*  p7  = static_cast<int8_t*>(allocator8.allocate(sizeof(int8_t),   alignof(uint8_t)));  // Take from allocator8. allocator8 is now full.
      int8_t*  p8  = static_cast<int8_t*>(allocator8.allocate(sizeof(int8_t),   alignof(uint8_t)));  // Take from allocator16
      int8_t*  p9  = static_cast<int8_t*>(allocator8.allocate(sizeof(int8_t),   alignof(uint8_t)));  // Take from allocator16
      int8_t*  p10 = static_cast<int8_t*>(allocator8.allocate(sizeof(int8_t),   alignof(uint8_t)));  // Take from allocator16. allocator16 now is full.
      int8_t*  p11 = static_cast<int8_t*>(allocator8.allocate(sizeof(int8_t),   alignof(uint8_t)));  // Take from allocator32

      CHECK(p1  != nullptr);
      CHECK(p2  != nullptr);
      CHECK(p3  != nullptr);
      CHECK(p4  == nullptr);
      CHECK(p5  != nullptr);
      CHECK(p6  != nullptr);
      CHECK(p7  != nullptr);
      CHECK(p8  != nullptr);
      CHECK(p9  != nullptr);
      CHECK(p10 != nullptr);
      CHECK(p11 != nullptr);

      // Take from allocator8
      CHECK(allocator8.is_owner_of(p1));
      CHECK(!allocator16.is_owner_of(p1));
      CHECK(!allocator32.is_owner_of(p1));

      // Take from allocator16
      CHECK(allocator8.is_owner_of(p2));
      CHECK(allocator16.is_owner_of(p2));
      CHECK(!allocator32.is_owner_of(p2));

      // Take from allocator32
      CHECK(allocator8.is_owner_of(p3));
      CHECK(allocator16.is_owner_of(p3));
      CHECK(allocator32.is_owner_of(p3));
            
      // Unable to allocate
      CHECK(!allocator8.is_owner_of(p4));
      CHECK(!allocator16.is_owner_of(p4));
      CHECK(!allocator32.is_owner_of(p4));

      // Take from allocator8
      CHECK(allocator8.is_owner_of(p5));
      CHECK(!allocator16.is_owner_of(p5));
      CHECK(!allocator32.is_owner_of(p5));

      // Take from allocator8
      CHECK(allocator8.is_owner_of(p6));
      CHECK(!allocator16.is_owner_of(p6));
      CHECK(!allocator32.is_owner_of(p6));

      // Take from allocator8.
      CHECK(allocator8.is_owner_of(p7));
      CHECK(!allocator16.is_owner_of(p7));
      CHECK(!allocator32.is_owner_of(p7));

      // Take from allocator16
      CHECK(allocator8.is_owner_of(p8));
      CHECK(allocator16.is_owner_of(p8));
      CHECK(!allocator32.is_owner_of(p8));

      // Take from allocator16
      CHECK(allocator8.is_owner_of(p9));
      CHECK(allocator16.is_owner_of(p9));
      CHECK(!allocator32.is_owner_of(p9));

      // Take from allocator16.
      CHECK(allocator8.is_owner_of(p10));
      CHECK(allocator16.is_owner_of(p10));
      CHECK(!allocator32.is_owner_of(p10));

      // Take from allocator32
      CHECK(allocator8.is_owner_of(p11));
      CHECK(allocator16.is_owner_of(p11));
      CHECK(allocator32.is_owner_of(p11));

      CHECK(allocator8.release(p1));
      CHECK(allocator8.release(p2));
      CHECK(allocator8.release(p3));
      CHECK(!allocator8.release(p4));
      CHECK(allocator8.release(p5));
      CHECK(allocator8.release(p6));
      CHECK(allocator8.release(p7));
      CHECK(allocator8.release(p8));
      CHECK(allocator8.release(p9));
      CHECK(allocator8.release(p10));
      CHECK(allocator8.release(p11));
    }

    //*************************************************************************
    TEST(test_custom_allocator)
    {
      CustomAllocator8 allocator8;

      int8_t* p1 = static_cast<int8_t*>(allocator8.allocate(sizeof(int8_t), alignof(int8_t)));
      CHECK(allocator8.allocate_block_called);
      CHECK(p1 != nullptr);
      CHECK(allocator8.is_owner_of(p1));
      CHECK(allocator8.is_owner_of_block_called);
      CHECK(allocator8.release(p1));
      CHECK(allocator8.release_block_called);
    }
  }
}
