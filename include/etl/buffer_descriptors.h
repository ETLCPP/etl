///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 jwellbelove

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

#include <cstring>

#undef ETL_FILE
#define ETL_FILE "57"

namespace etl
{
  //***************************************************************************
  /// buffer_descriptors
  //***************************************************************************
  template <typename TBuffer, typename TSize, TSize BUFFER_SIZE_, size_t N_BUFFERS_, typename TFlag = bool>
  class buffer_descriptors
  {
  public:

    typedef TBuffer           value_type;
    typedef value_type*       pointer;
    typedef TSize             size_type;
    typedef TFlag             flag_type;
    
    ETL_STATIC_ASSERT(etl::is_unsigned<TSize>::value, "TSize must be unsigned");
    ETL_STATIC_ASSERT(etl::is_integral<TSize>::value, "TSize must be integral");

    static ETL_CONSTANT size_t    N_BUFFERS   = N_BUFFERS_;
    static ETL_CONSTANT size_type BUFFER_SIZE = BUFFER_SIZE_;

    //*********************************
    class descriptor
    {
    public:

      friend class buffer_descriptors;

      static ETL_CONSTANT const size_type MAX_SIZE = buffer_descriptors::BUFFER_SIZE;

      //*********************************
      descriptor()
        : pbuffer(ETL_NULLPTR)
        , in_use(false)
      {
      }

      //*********************************
      ETL_CONSTEXPR pointer data() const
      {
        return pbuffer;
      }

      //*********************************
      ETL_CONSTEXPR size_type max_size() const
      {
        return BUFFER_SIZE;
      }

      //*********************************
      bool is_valid() const
      {
        return pbuffer != ETL_NULLPTR;
      }

      //*********************************
      bool is_allocated() const
      {
        return bool(in_use);
      }

      //*********************************
      bool is_released() const
      {
        return !bool(in_use);
      }

      //*********************************
      void release()
      {
        in_use = false;
      }

    private:

      //*********************************
      ETL_CONSTEXPR descriptor(TBuffer* pbuffer_)
        : pbuffer(pbuffer_)
        , in_use(false)
      {
      }

      //*********************************
      void allocate()
      {
        in_use = true;
      }

      //*********************************
      descriptor(const descriptor&) ETL_DELETE;
      descriptor& operator =(const descriptor&) ETL_DELETE;

      pointer   pbuffer;
      volatile flag_type in_use;
    };

    typedef etl::delegate<void(descriptor&, size_type)> callback_type;

    //*********************************
    buffer_descriptors(TBuffer* pbuffers_)
    {
      for (size_t i = 0U; i < N_BUFFERS; ++i)
      {
        descriptors[i].pbuffer = pbuffers_ + (i * BUFFER_SIZE);
      }
    }

    //*********************************
    buffer_descriptors(TBuffer* pbuffers_, const callback_type& callback_)
      : callback(callback_)
    {
      for (size_t i = 0U; i < N_BUFFERS; ++i)
      {
        descriptors[i].pbuffer = pbuffers_ + (i * BUFFER_SIZE);
      }
    }

    //*********************************
    void set_callback(const callback_type& callback_)
    {
      callback = callback_;
    }

    //*********************************
    bool is_valid() const
    {
      return callback.is_valid();
    }

    //*********************************
    void notify(descriptor& desc_, size_type count_)
    {
      // We have a valid callback?
      if (callback.is_valid())
      {
        // Set the relevant data.
        callback(desc_, count_);
       }
    }

    //*********************************
    descriptor& allocate()
    {    
      descriptor& desc = descriptors[next];

      if (desc.is_released())
      {
        ++next;
        
        desc.allocate();

        return desc;
      }
      else
      {
        static descriptor null_descriptor;
        return null_descriptor;
      }
    }

  private:

    callback_type callback;
    etl::array<descriptor, N_BUFFERS> descriptors;
    etl::cyclic_value<uint_least8_t, 0U, N_BUFFERS - 1> next;
  };
}

#undef ETL_FILE

#endif
