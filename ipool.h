///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

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

#ifndef __ETL_IPOOL__
#define __ETL_IPOOL__
#define __ETL_IN_IPOOL_H__

#include "pool_base.h"
#include "nullptr.h"

#ifndef ETL_THROW_EXCEPTIONS
#include "error_handler.h"
#endif

namespace etl
{
  //*************************************************************************
  ///\ingroup pool
  //*************************************************************************
  template <typename T>
  class ipool : public pool_base
  {
  public:

    //*************************************************************************
    /// Allocate an object from the pool.
    /// If ETL_THROW_EXCEPTIONS is defined and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
    /// \note The state of the object returned is undefined.
    //*************************************************************************
    T* allocate()
    {
      if (next_free != MAX_SIZE)
      {
        T* result = &p_buffer[next_free];
        in_use_flags.set(next_free);
        next_free = in_use_flags.find_first(false);
        ++items_allocated;
        return result;
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw pool_no_allocation();
#else
        error_handler::error(pool_no_allocation());
#endif
        return nullptr;
      }
    }
    
    //*************************************************************************
    /// Free an object in the pool.
    /// If ETL_THROW_EXCEPTIONS is defined and the object does not belong to this
    /// pool then an etl::pool_object_not_in_pool is thrown.
    /// \param p_object A pointer to the object to be released.
    //*************************************************************************
    void release(const T& object)
    {
      release(&object);
    }

    //*************************************************************************
    /// Free an object in the pool.
    /// If ETL_THROW_EXCEPTIONS is defined and the object does not belong to this
    /// pool then an etl::pool_object_not_in_pool is thrown.
    /// \param p_object A pointer to the object to be released.
    //*************************************************************************
    void release(const T* const p_object)
    {
      // Does it belong to me?
      if (is_in_pool(p_object))
      {
        typename std::iterator_traits<T*>::difference_type distance = p_object - p_buffer;

        // Mark the object as available.
        next_free = static_cast<size_t>(distance);
        in_use_flags.reset(next_free);
        --items_allocated;
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw pool_object_not_in_pool();
#else
        error_handler::error(pool_object_not_in_pool());
#endif
      }
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
    bool is_in_pool(const T* const p_object) const
    {
      // Does this object belong to this pool?
      typename std::iterator_traits<T*>::difference_type distance = p_object - p_buffer;

      // Within the range of the buffer?
      return ((distance >= 0) && (distance < static_cast<typename std::iterator_traits<T*>::difference_type>(MAX_SIZE)));
    }

  protected:

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    ipool(T* p_buffer, ibitset& in_use_flags, size_t size)
      : pool_base(size),
        p_buffer(nullptr),
        in_use_flags(in_use_flags)
    {
    }

    T* p_buffer;
    ibitset& in_use_flags;
  };
}
#endif

