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

#ifndef __ETL_POOL__
#define __ETL_POOL__

#include "bitset.h"
#include "nullptr.h"

#include <iterator>

//*****************************************************************************
///\defgroup pool pool
/// A fixed capacity pool.
///\ingroup containers
//*****************************************************************************

namespace etl
{
#ifdef ETL_THROW_EXCEPTIONS
  //***************************************************************************
  /// The base class for pool exceptions.
  ///\ingroup pool
  //***************************************************************************
  class pool_exception : public exception
  {
  public:

    pool_exception(const char* what)
      : exception(what)
    {}
  };

  //***************************************************************************
  /// The exception thrown when the pool has no more free items.
  ///\ingroup pool
  //***************************************************************************
  class pool_no_allocation : public pool_exception
  {
  public:

    pool_no_allocation()
      : pool_exception("pool: no allocation")
    {}
  };

  //***************************************************************************
  /// The exception thrown when an object is released which does not belong to the pool.
  ///\ingroup pool
  //***************************************************************************
  class pool_object_not_in_pool : public pool_exception
  {
  public:

    pool_object_not_in_pool()
      : pool_exception("pool: not in pool")
    {}
  };
#endif

  //*************************************************************************
  /// A templated pool implementation that uses a fixed size pool.
  /// SIZE_ elements will be always be constructed.
  //*************************************************************************
  template <typename T, const size_t SIZE_>
  class pool
  {
  public:

    static const size_t SIZE = SIZE_;

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    pool()
      : next_free(0),
        count(0)
    {
    }

    //*************************************************************************
    /// Allocate an object from the pool.
    /// If ETL_THROW_EXCEPTIONS is defined and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
    /// \note The state of the object returned is undefined.
    //*************************************************************************
    T* allocate()
    {
      if (next_free != SIZE)
      {
        T* result = &buffer[next_free];
        in_use.set(next_free);
        next_free = in_use.find_first(false);
        ++count;
        return result;
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw pool_no_allocation();
#else
        return nullptr;
#endif
      }
    }

    //*************************************************************************
    /// Free an object in the pool.
    /// If ETL_THROW_EXCEPTIONS is defined and the object does not belong to this
    /// pool then an etl::pool_object_not_in_pool is thrown.
    /// \param object A reference to the object to be released.
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
      // Does this object belong to this pool?
      typename std::iterator_traits<T*>::difference_type distance = p_object - buffer;

      // Not within the range of the buffer?
      if ((distance < 0) || (distance >= SIZE))
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw pool_object_not_in_pool();
#endif
      }
      else
      {
        // Mark the object as available.
        next_free = static_cast<size_t>(distance);
        in_use.reset(next_free);
        --count;
      }
    }

    //*************************************************************************
    /// Returns the number of free items in the pool.
    //*************************************************************************
    size_t available() const
    {
      return SIZE - in_use.count();
    }

    //*************************************************************************
    /// Checks to see if there are no free items in the pool.
    /// \return <b>true</b> if there are none free (or 'empty')
    //*************************************************************************
    bool empty() const
    {
      return count == SIZE;
    }

  private:

    size_t       next_free;    ///< The position of the next free item in the pool.
    size_t       count;        ///< The number of objects allocated.
    T            buffer[SIZE]; ///< The pool of objects.
    bitset<SIZE> in_use;       ///< The set of flags that indicate which items are free in the pool.
  };
}
#endif

