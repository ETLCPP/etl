///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2014 jwellbelove

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

//*****************************************************************************
// The algorithm for this implementation is based on this paper.
//
// Fast Efficient Fixed-Size Memory Pool
// https://www.thinkmind.org/index.php?view=article&articleid=computation_tools_2012_1_10_80006
//
// Ben Kenwright
// School of Computer Science
// Newcastle University
// Newcastle, United Kingdom,
// b.kenwright@ncl.ac.uk
//*****************************************************************************

#ifndef __ETL_IPOOL__
#define __ETL_IPOOL__
#define __ETL_IN_IPOOL_H__

#include <iterator>

#include "private/pool_base.h"
#include "nullptr.h"
//#include "ibitset.h"
#include "error_handler.h"

namespace etl
{
  //***************************************************************************
  ///\ingroup pool
  //***************************************************************************
  template <typename T, typename TIndex = int32_t>
  class ipool : public pool_base
  {
  public:

    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef size_t   size_type;

    //*************************************************************************
    /// Allocate an object from the pool.
    /// Uses the default constructor.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
    //*************************************************************************
    T* allocate()
    {
      return allocate(T());
    }

    //*************************************************************************
    /// Allocate an object from the pool from an initial value.
    /// If asserts or exceptions are enabled and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
    //*************************************************************************
    T* allocate(const T& initial)
    {
      ETL_ASSERT(items_allocated < MAX_SIZE, ETL_ERROR(pool_no_allocation));

      T* p_value = nullptr;

      if (available() > 0)
      {
        p_value = new(&p_next->value) T(initial);
        p_next->index = -p_next->index;
        ++items_allocated;

        if (available() != 0)
        {
          p_next = AddressFromIndex(p_next->index);          
        }
        else
        {
          p_next = nullptr;
        }
      }

      return p_value;
    }

    //*************************************************************************
    /// Release an object in the pool.
    /// If asserts or exceptions are enabled and the object does not belong to this
    /// pool then an etl::pool_object_not_in_pool is thrown.
    /// \param p_object A pointer to the object to be released.
    //*************************************************************************
    void release(const T& object)
    {
      release(&object);
    }

    //*************************************************************************
    /// Release an object in the pool.
    /// If asserts or exceptions are enabled and the object does not belong to this
    /// pool then an etl::pool_object_not_in_pool is thrown.
    /// \param p_object A pointer to the object to be released.
    //*************************************************************************
    void release(const T* const p_object)
    {
      // Does it belong to me?
      ETL_ASSERT(is_in_pool(p_object), ETL_ERROR(pool_object_not_in_pool));

      // Get the pointer to the element by adjusting for the index.
      uintptr_t p = (uintptr_t)p_object - offsetof(Element, value);

      Element* p_element = reinterpret_cast<Element*>(p);

      if (p_next != nullptr)
      {
        p_element->index = -IndexFromAddress(p_next);
      }
      else
      {
        p_element->index = -TIndex(MAX_SIZE);
      }

      p_object->~T();

      p_next = p_element;
      --items_allocated;
    }

    //*************************************************************************
    /// Releases all objects in the pool.
    //*************************************************************************
    void release_all()
    {
      for (size_t i = 0; i < MAX_SIZE; ++i)
      {
        if (p_buffer[i].index > 0)
        {
          p_buffer[i].value.~T();
        }

        p_buffer[i].index = -int32_t(i + 1);
      }

      items_allocated = 0;
      p_next = p_buffer;
    }

    //*************************************************************************
    /// Check to see if the object belongs to the pool.
    /// \param p_object A pointer to the object to be checked.
    /// \return <b>true<\b> if it does, otherwise <b>false</b>
    //*************************************************************************
    bool is_in_pool(const T& object) const
    {
      return is_in_pool(&object);
    }

    //*************************************************************************
    /// Check to see if the object belongs to the pool.
    /// \param p_object A pointer to the object to be checked.
    /// \return <b>true<\b> if it does, otherwise <b>false</b>
    //*************************************************************************
    bool is_in_pool(const T* p_object) const
    {
      // Does this object belong to this pool?
      // Get the pointer to the element by adjusting for the index.
      uintptr_t p = (uintptr_t)p_object - sizeof(TIndex);

      Element* p_element = reinterpret_cast<Element*>(p);

      // Within the range of the buffer?      
      intptr_t distance = p_element - p_buffer;
      return ((distance >= 0) && (distance < static_cast<intptr_t>(MAX_SIZE)));
    }

  protected:

    struct Element
    {
      TIndex index;
      T      value;
    };

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    ipool(Element* p_buffer, size_t size)
      : pool_base(size),
        p_buffer(p_buffer),
        p_next(p_buffer)
        
    {
      for (int32_t i = 0; i < MAX_SIZE; ++i)
      {
        p_buffer[i].index = -(i + 1);
      }
    }

    //*************************************************************************
    /// Destructor
    //*************************************************************************
    ~ipool()
    {
      release_all();
    }

  private:

    Element* AddressFromIndex(TIndex i) const
    {
      i = (i < 0) ? -i : i;
      return &p_buffer[i];
    }

    TIndex IndexFromAddress(const Element* p_element) const
    {
      return TIndex(p_element - p_buffer);
    }
    
    // Disable copy construction and assignment.
    ipool(const ipool&);
    ipool& operator =(const ipool&);

    Element* p_buffer;
    Element* p_next;
  };
}

#undef __ETL_IN_IPOOL_H__

#endif

