///\file

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

#ifndef ETL_BUFFER_DESCRIPTORS_INCLUDED
#define ETL_BUFFER_DESCRIPTORS_INCLUDED

#include "platform.h"
#include "array.h"
#include "delegate.h"
#include "type_traits.h"
#include "static_assert.h"
#include "cyclic_value.h"
#include "algorithm.h"

#include <cstring>

#if ETL_USING_CPP11

namespace etl
{
  //***************************************************************************
  /// buffer_descriptors
  //***************************************************************************
  template <typename TBuffer, size_t BUFFER_SIZE_, size_t N_BUFFERS_, typename TFlag = bool>
  class buffer_descriptors
  {
  private:

    struct descriptor_item;

  public:

    typedef TBuffer     value_type;
    typedef value_type* pointer;
    typedef size_t      size_type;
    typedef TFlag       flag_type;

    static ETL_CONSTANT size_type N_BUFFERS   = N_BUFFERS_;
    static ETL_CONSTANT size_type BUFFER_SIZE = BUFFER_SIZE_;

    //*********************************
    /// Describes a buffer.
    //*********************************
    class descriptor
    {
    public:

      friend class buffer_descriptors;

      static ETL_CONSTANT size_type MAX_SIZE = buffer_descriptors::BUFFER_SIZE;

      //*********************************
      descriptor()
        : pdesc_item(ETL_NULLPTR)
      {
      }

      //*********************************
      descriptor(const descriptor& other)
        : pdesc_item(other.pdesc_item)
      {
      }

      //*********************************
      descriptor& operator =(const descriptor& other)
      {
        pdesc_item = other.pdesc_item;
        return *this;
      }

      //*********************************
      pointer data() const
      {
        assert(pdesc_item != ETL_NULLPTR);
        return pdesc_item->pbuffer;
      }

      //*********************************
      ETL_NODISCARD
      ETL_CONSTEXPR size_type max_size() const
      {
        return BUFFER_SIZE;
      }

      //*********************************
      ETL_NODISCARD
      bool is_allocated() const
      {
        return bool(pdesc_item->in_use);
      }

      //*********************************
      ETL_NODISCARD
      bool is_released() const
      {
        return bool(!pdesc_item->in_use);
      }

      //*********************************
      ETL_NODISCARD
      bool is_valid() const
      {
        return pdesc_item != ETL_NULLPTR;
      }

      //*********************************
      void release()
      {
        pdesc_item->in_use = false;
      }

    private:

      //*********************************
      descriptor(descriptor_item* pdesc_item_)
        : pdesc_item(pdesc_item_)
      {
      }

      //*********************************
      void allocate()
      {
        pdesc_item->in_use = true;;
      }

      /// The pointer to the buffer descriptor.
      descriptor_item* pdesc_item;
    };

    //*********************************
    /// Describes a notification.
    //*********************************
    class notification
    {
    public:

      //*********************************
      notification()
        : desc()
        , count(0U)
      {
      }

      //*********************************
      notification(descriptor desc_, size_t count_)
        : desc(desc_)
        , count(count_)
      {
      }

      //*********************************
      ETL_NODISCARD
      descriptor get_descriptor() const
      {
        return desc;
      }

      //*********************************
      ETL_NODISCARD
      size_t get_count() const
      {
        return count;
      }

    private:

      descriptor desc;
      size_t     count;
    };

    // The type of the callback function.
    typedef etl::delegate<void(notification)> callback_type;

    //*********************************
    buffer_descriptors(TBuffer* pbuffers_, callback_type callback_ = callback_type())
      : callback(callback_)
    {
      for (size_t i = 0UL; i < N_BUFFERS; ++i)
      {
        descriptor_items[i].pbuffer = pbuffers_ + (i * BUFFER_SIZE);
        descriptor_items[i].in_use  = false;
      }
    }

    //*********************************
    void set_callback(const callback_type& callback_)
    {
      callback = callback_;
    }

    //*********************************
    void clear()
    {
      for (size_t i = 0UL; i < N_BUFFERS; ++i)
      {
        descriptor_items[i].in_use = false;
      }

      next.to_first();
    }

    //*********************************
    ETL_NODISCARD
    bool is_valid() const
    {
      return callback.is_valid();
    }

    //*********************************
    void notify(notification n)
    {
      // Do we have a valid callback?
      if (callback.is_valid())
      {
        callback(n);
      }
    }

    //*********************************
    ETL_NODISCARD
    descriptor allocate()
    {
      descriptor desc(&descriptor_items[next]);

      if (desc.is_released())
      {
        ++next;

        desc.allocate();

        return desc;
      }
      else
      {
        return descriptor();
      }
    }

    //*********************************
    ETL_NODISCARD
    descriptor allocate(value_type fill_)
    {
      descriptor desc = allocate();

      if (desc.is_valid())
      {
        etl::fill_n(desc.data(), BUFFER_SIZE, fill_);
      }

      return desc;
    }

  private:

    //*********************************
    struct descriptor_item
    {
      pointer  pbuffer;
      volatile flag_type in_use;
    };

    callback_type callback;
    etl::array<descriptor_item, N_BUFFERS> descriptor_items;
    etl::cyclic_value<uint_least8_t, 0U, N_BUFFERS - 1> next;
  };

  template <typename TBuffer, size_t BUFFER_SIZE_, size_t N_BUFFERS_, typename TFlag>
  ETL_CONSTANT typename buffer_descriptors<TBuffer, BUFFER_SIZE_, N_BUFFERS_, TFlag>::size_type buffer_descriptors<TBuffer, BUFFER_SIZE_, N_BUFFERS_, TFlag>::N_BUFFERS;

  template <typename TBuffer, size_t BUFFER_SIZE_, size_t N_BUFFERS_, typename TFlag>
  ETL_CONSTANT typename buffer_descriptors<TBuffer, BUFFER_SIZE_, N_BUFFERS_, TFlag>::size_type buffer_descriptors<TBuffer, BUFFER_SIZE_, N_BUFFERS_, TFlag>::BUFFER_SIZE;
  
  template <typename TBuffer, size_t BUFFER_SIZE_, size_t N_BUFFERS_, typename TFlag>
  ETL_CONSTANT typename buffer_descriptors<TBuffer, BUFFER_SIZE_, N_BUFFERS_, TFlag>::size_type buffer_descriptors<TBuffer, BUFFER_SIZE_, N_BUFFERS_, TFlag>::descriptor::MAX_SIZE;
}
#endif
#endif
