///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 jwellbelove

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

#ifndef ETL_OBJECT_POOL_INCLUDED
#define ETL_OBJECT_POOL_INCLUDED

#include "platform.h"
#include "alignment.h"
#include "intrusive_forward_list.h"
#include "utility.h"
#include "memory.h"

namespace etl
{
  //***************************************************************************
  /// Object Pool implementation for all sizes.
  //***************************************************************************
  class iobject_pool
  {
  public:

#if ETL_USING_CPP11
    //*************************************************************************
    /// 
    //*************************************************************************
    template <typename T, typename... TArgs>
    T* allocate(TArgs... args)
    {
      // This is the size of the header + data.
      header* p_item = allocate_block(sizeof(header_item<T>), etl::alignment_of<header_item<T>>::value);

      if (p_item != nullptr)
      {
        p_item->destructor = &destruct<T>;

        return ::new (p_item->data()) T(etl::forward<TArgs>(args)...);
      }
      else
      {
        return nullptr;
      }
    }

    //*************************************************************************
    /// 
    //*************************************************************************
    template <typename T, typename... TArgs>
    void release(T&& object)
    {
      release_block(reinterpret_cast<char*>(&object));
    }
#endif

    //*************************************************************************
    /// 
    //*************************************************************************
    void release_all()
    {
      // Destruct all remaining allocated objects.
      for (header* p_item = head; p_item != tail; p_item = p_item->next)
      {
        // Destruct the object.
        p_item->destruct();
      }
    }

  protected:

    //*************************************************************************
    /// 
    //*************************************************************************
    iobject_pool(char* buffer_, size_t buffer_size_)
      : pbuffer(buffer_)
      , buffer_size(buffer_size_)
      , head(reinterpret_cast<header*>(buffer_))
      , tail(reinterpret_cast<header*>(buffer_ + buffer_size_))
    {
    }

    //*************************************************************************
    /// 
    //*************************************************************************
    ~iobject_pool()
    {
      release_all();
    }

    //*************************************************************************
    /// 
    //*************************************************************************
    struct header
    {
      //*******************************
      header()
        : destructor(nullptr)
        , next(nullptr)
        , offset(0U)
      {
      }

      //*******************************
      size_t data_size() const
      {
        return size_t(reinterpret_cast<const char*>(next) - data());
      }

      //*******************************
      char* data()
      {
        return (reinterpret_cast<char*>(this) + offset);
      }

      //*******************************
      const char* data() const
      {
        return (reinterpret_cast<const char*>(this) + offset);
      }

      //*******************************
      bool in_use() const
      {
        return (destructor != nullptr);
      }

      //*******************************
      void destruct()
      {
        if (in_use())
        {
          destructor(data()); // Get the address of the actual object and call its destructor.
          destructor = nullptr;
          offset = 0U;
        }
      }

      void(*destructor)(char*);
      header* next;
      unsigned char offset;
    };

    template<typename T>
    struct header_item : public header
    {
      etl::uninitialized_buffer_of<T, 1U> buffer;
    };

  private:

    //*************************************************************************
    /// This function will destruct an object.
    /// A pointer to it is stored in the allocated item's header.
    //*************************************************************************
    template <typename T>
    static void destruct(char* p)
    {
      reinterpret_cast<T*>(p)->~T();
    }

    //*************************************************************************
    /// 
    //*************************************************************************
    header* allocate_block(size_t size, size_t alignment)
    {
      for (header* p_item = head; p_item != tail; p_item = p_item->next)
      {
        if (!p_item->in_use())
        {
          // Calculate the space.
          uintptr_t p_header_begin = reinterpret_cast<uintptr_t>(p_item);
          uintptr_t p_header_end   = p_header_begin + sizeof(p_item);
          uintptr_t p_header_next  = reinterpret_cast<uintptr_t>(p_item->next);

          // Align the data pointer.
          uintptr_t p_data = p_header_end;
          p_data = (p_data + alignment - 1U) & -intptr_t(alignment);

          p_item->offset = p_data - p_header_begin;

          size_t space_including_header = p_header_next - p_data;

          if (space_including_header >= size)
          {

          }
        }
      }

      // No space available.
      return nullptr;
    }

    char*   pbuffer;
    size_t  buffer_size;
    header* head;
    header* tail;
  };

  //***************************************************************************
  /// Object Pool
  //***************************************************************************
  template <size_t Size, size_t Buffer_Alignment = 8U>
  class object_pool : public etl::iobject_pool
  {
  public:

    //*************************************************************************
    /// 
    //*************************************************************************
    object_pool()
      : iobject_pool(static_cast<char*>(buffer), Size)
    {
    }

  private:

    object_pool(const object_pool&) ETL_DELETE;
    object_pool& operator =(const object_pool&) ETL_DELETE;

    etl::uninitialized_buffer<sizeof(char) + sizeof(header), Size, Buffer_Alignment> buffer;
  };

  //***************************************************************************
  /// Object Pool with external buffer
  //***************************************************************************
  class object_pool_ext : public etl::iobject_pool
  {
  public:

    //*************************************************************************
    /// 
    //*************************************************************************
    object_pool_ext(char* pbuffer_, size_t size_)
      : iobject_pool(pbuffer_, size_)
    {
    }

  private:

    object_pool_ext(const object_pool_ext&) ETL_DELETE;
    object_pool_ext& operator =(const object_pool_ext&) ETL_DELETE;
  };
}

#endif
