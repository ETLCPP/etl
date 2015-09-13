///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

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

#include <iterator>

#include "pool_base.h"
#include "nullptr.h"
#include "ibitset.h"

#ifndef ETL_THROW_EXCEPTIONS
#include "error_handler.h"
#endif

namespace etl
{
  //***************************************************************************
  ///\ingroup pool
  //***************************************************************************
  template <typename T>
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
    /// iterator
    //*************************************************************************
    class iterator : public std::iterator<std::forward_iterator_tag, const T>
    {
    public:

      friend class ipool;
      friend class const_iterator;

      //*******************************
      iterator()
        : index(0),
          p_buffer(nullptr),
          p_in_use_flags(nullptr)
      {
      }

      //*******************************
      iterator(const iterator& other)
        : index(other.index),
          p_buffer(other.p_buffer),
          p_in_use_flags(other.p_in_use_flags)
      {
      }

      //*******************************
      iterator& operator ++()
      {
        index = p_in_use_flags->find_next(true, index + 1);
        return *this;
      }

      //*******************************
      iterator operator ++(int)
      {
        iterator temp(*this);
        index = p_in_use_flags->find_next(true, index + 1);
        return temp;
      }

      //*******************************
      iterator operator =(const iterator& other)
      {
        index          = other.index;
        p_buffer       = other.p_buffer;
        p_in_use_flags = other.p_in_use_flags;
        return *this;
      }

      //*******************************
      const_reference operator *() const
      {
        return p_buffer[index];
      }

      //*******************************
      const_pointer operator ->() const
      {
        return &p_buffer[index];
      }

      //*******************************
      friend bool operator == (const iterator& lhs, const iterator& rhs)
      {
        return (lhs.p_buffer == rhs.p_buffer) && (lhs.index == rhs.index);
      }

      //*******************************
      friend bool operator != (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      //*******************************
      //*******************************
      iterator(size_t         index,
               pointer        p_buffer,
               const ibitset* p_in_use_flags)
        : index(index),
          p_buffer(p_buffer),
          p_in_use_flags(p_in_use_flags)
      {
      }

      size_t         index;
      pointer        p_buffer;
      const ibitset* p_in_use_flags;
    };


    //*************************************************************************
    /// const_iterator
    //*************************************************************************
    class const_iterator : public std::iterator<std::forward_iterator_tag, const T>
    {
    public:

      friend class ipool;

      //*******************************
      const_iterator()
        : index(0),
          p_buffer(nullptr),
          p_in_use_flags(nullptr)
      {
      }

      //*******************************
      const_iterator(const const_iterator& other)
        : index(other.index),
          p_buffer(other.p_buffer),
          p_in_use_flags(other.p_in_use_flags)
      {
      }

      //*******************************
      const_iterator(const typename ipool::iterator& other)
        : index(other.index),
          p_buffer(other.p_buffer),
          p_in_use_flags(other.p_in_use_flags)
      {
      }

      //*******************************
      const_iterator& operator ++()
      {
        index = p_in_use_flags->find_next(true, index + 1);
        return *this;
      }

      //*******************************
      const_iterator operator ++(int)
      {
        const_iterator temp(*this);
        index = p_in_use_flags->find_next(true, index + 1);
        return temp;
      }

      //*******************************
      const_iterator operator =(const const_iterator& other)
      {
        index          = other.index;
        p_buffer       = other.p_buffer;
        p_in_use_flags = other.p_in_use_flags;
        return *this;
      }

      //*******************************
      const_reference operator *() const
      {
        return p_buffer[index];
      }

      //*******************************
      const_pointer operator ->() const
      {
        return &p_buffer[index];
      }

      //*******************************
      friend bool operator == (const const_iterator& lhs, const const_iterator& rhs)
      {
        return (lhs.p_buffer == rhs.p_buffer) && (lhs.index == rhs.index);
      }

      //*******************************
      friend bool operator != (const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      //*******************************
      //*******************************
      const_iterator(size_t         index,
                     const_pointer  p_buffer,
                     const ibitset* p_in_use_flags)
        : index(index),
          p_buffer(p_buffer),
          p_in_use_flags(p_in_use_flags)
      {
      }

      size_t         index;
      const_pointer  p_buffer;
      const ibitset* p_in_use_flags;
    };

    //*************************************************************************
    /// Get an iterator to the first allocated item in the pool.
    //*************************************************************************
    iterator begin()
    {
      size_t index = in_use_flags.find_first(true);

      if (index != ibitset::npos)
      {
        return iterator(index, p_buffer, &in_use_flags);
      }
      else
      {
        return end();
      }
    }

    //*************************************************************************
    /// Get a const iterator to the first allocated item in the pool.
    //*************************************************************************
    const_iterator begin() const
    {
      size_t index = in_use_flags.find_first(true);

      if (index != ibitset::npos)
      {
        return const_iterator(index, p_buffer, &in_use_flags);
      }
      else
      {
        return end();
      }
    }

    //*************************************************************************
    /// Get a const iterator to the first allocated item in the pool.
    //*************************************************************************
    const_iterator cbegin() const
    {
      return begin();
    }

    //*************************************************************************
    /// Get an iterator to the end of the pool.
    //*************************************************************************
    iterator end()
    {
      return iterator(ibitset::npos, p_buffer, &in_use_flags);
    }

    //*************************************************************************
    /// Get a const iterator to the end of the pool.
    //*************************************************************************
    const_iterator end() const
    {
      return const_iterator(ibitset::npos, p_buffer, &in_use_flags);
    }

    //*************************************************************************
    /// Get a const iterator to the end of the pool.
    //*************************************************************************
    const_iterator cend() const
    {
      return end();
    }

	  //*************************************************************************
	  /// Allocate an object from the pool.
	  /// Uses the default constructor.
	  /// If ETL_THROW_EXCEPTIONS is defined and there are no more free items an
	  /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
	  /// \note The state of the object returned is undefined.
	  //*************************************************************************
	  T* allocate()
	  {
#if defined(_DEBUG) || defined(DEBUG)
      if (items_allocated < MAX_SIZE && !in_use_flags.test(next_free))
#else
      if (items_allocated < MAX_SIZE)
#endif
	    {
		    T* result = new(&p_buffer[next_free]) T();
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
    /// Allocate an object from the pool from an ititial value.
    /// If ETL_THROW_EXCEPTIONS is defined and there are no more free items an
    /// etl::pool_no_allocation if thrown, otherwise a nullptr is returned.
    /// \note The state of the object returned is undefined.
    //*************************************************************************
    T* allocate(const T& initial)
    {
#if defined(_DEBUG) || defined(DEBUG)
      if (items_allocated < MAX_SIZE && !in_use_flags.test(next_free))
#else
      if (items_allocated < MAX_SIZE)
#endif
      {
        T* result = new(&p_buffer[next_free]) T(initial);
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
    /// Release an object in the pool.
    /// If ETL_THROW_EXCEPTIONS is defined and the object does not belong to this
    /// pool then an etl::pool_object_not_in_pool is thrown.
    /// \param p_object A pointer to the object to be released.
    //*************************************************************************
    void release(const T& object)
    {
      release(&object);
    }

    //*************************************************************************
    /// Release an object in the pool.
    /// If ETL_THROW_EXCEPTIONS is defined and the object does not belong to this
    /// pool then an etl::pool_object_not_in_pool is thrown.
    /// \param p_object A pointer to the object to be released.
    //*************************************************************************
    void release(const T* const p_object)
    {
      // Does it belong to me?
      if (is_in_pool(p_object))
      {
    	// Where is it in the buffer?
        typename std::iterator_traits<T*>::difference_type distance = p_object - p_buffer;
        size_t index = static_cast<size_t>(distance);

        // Check that it hasn't already been released.
        if (in_use_flags.test(index))
        {
          // Destroy the object and mark as available.
          p_object->~T();
          in_use_flags.reset(index);
          --items_allocated;
          next_free = index;
        }
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
    /// Releases all objects in the pool.
    //*************************************************************************
    void release_all()
    {
      const_iterator i_object = begin();

      while (i_object != end())
      {
        i_object->~T();
        ++i_object;
      }

      in_use_flags.reset();
      items_allocated = 0;
      next_free       = 0;
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
      typename std::iterator_traits<T*>::difference_type distance = p_object - p_buffer;

      // Within the range of the buffer?
      return ((distance >= 0) && (distance < static_cast<typename std::iterator_traits<T*>::difference_type>(MAX_SIZE)));
    }

    //*************************************************************************
    /// Gets the iterator to the object.
    /// If the object is not in the pool then end() is returned.
    /// \param object A const reference to the object to be checked.
    /// \return An iterator to the object or end().
    //*************************************************************************
    iterator get_iterator(T& object)
    {
      if (is_in_pool(object))
      {
        iterator i_object = begin();

        while (i_object != end())
        {
          // Same one?
          if (&object == &*i_object)
          {
            return i_object;
          }

          ++i_object;
        }
      }

      return end();
    }

    //*************************************************************************
    /// Gets the const_iterator to the object.
    /// If the object is not in the pool then end() is returned.
    /// \param object A const reference to the object to be checked.
    /// \return An const_iterator to the object or end().
    //*************************************************************************
    const_iterator get_iterator(const T& object) const
    {
      if (is_in_pool(object))
      {
        const_iterator i_object = begin();

        while (i_object != end())
        {
          // Same one?
          if (&object == &*i_object)
          {
            return i_object;
          }

          ++i_object;
        }
      }

      return end();
    }


  protected:

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    ipool(T* p_buffer, ibitset& in_use_flags, size_t size)
      : pool_base(size),
        p_buffer(p_buffer),
        in_use_flags(in_use_flags)
    {
    }

    T*       p_buffer;
    ibitset& in_use_flags;
  };
}

#undef __ETL_IN_IPOOL_H__

#endif

