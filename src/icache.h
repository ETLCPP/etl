///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2017 jwellbelove, rlindeman

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
#error  This header is in development. Do not use.
//*****************************************************************************

#ifndef __ETL_ICACHE__
#define __ETL_ICACHE__

#include "platform.h"
#include "function.h"
#include "nullptr.h"

#include <utility>

namespace etl
{
  ///**************************************************************************
  /// The base class for all caches.
  ///**************************************************************************
  template <typename TKey, typename TValue>
  class icache
  {
  public:

    ///************************************************************************
    /// Constructor.
    /// By default, 'write_through' is set to true.
    ///************************************************************************
    icache()
      : write_through(true),
        p_read_store(nullptr),
        p_write_store(nullptr)
    {
    }

    ///************************************************************************
    /// Destructor.
    /// Flushes the cache if necessary.
    ///************************************************************************
    virtual ~icache()
    {
      if (!write_through)
      {
        flush();
      }
    }

    ///************************************************************************
    /// Sets the function that reads from the store.
    ///************************************************************************
    void set_read_function(etl::ifunction<key_value_t&>* p_read)
    {
      p_read_store = p_read;
    }

    ///************************************************************************
    /// Sets the function that writes to the store.
    ///************************************************************************
    void set_write_function(etl::ifunction<const key_value_t&>* p_write)
    {
      p_write_store = p_write;
    }

    ///************************************************************************
    /// Sets the 'write through'' flag.
    ///************************************************************************
    void set_write_through(bool write_through_)
    {
      write_through = write_through_;
    }

    virtual const T& read(const TKey& key) const = 0;             ///< Reads from the cache. May read from the store using p_read_store.
    virtual void write(const TKey& key, const TValue& value) = 0; ///< Writes to the cache. May write to the store using p_write_store.
    virtual void flush() = 0;                                     ///< The overridden function should write all changed values to the store.

  protected:

    typedef std::pair<TKey, TValue> key_value_t;

    bool write_through; ///< If true, the cache should write changed items back to the store immediately. If false then a flush() or destruct will be required.

    etl::ifunction<key_value_t&>*       p_read_store;  ///< A pointer to the function that will read a value from the store into the cache.
    etl::ifunction<const key_value_t&>* p_write_store; ///< A pointer to the function that will write a value from the cache into the store.
  }
}

#endif
