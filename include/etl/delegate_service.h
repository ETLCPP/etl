///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 jwellbelove

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

#ifndef ETL_DELEGATE_SERVICE_INCLUDED
#define ETL_DELEGATE_SERVICE_INCLUDED

#include "platform.h"
#include "nullptr.h"
#include "static_assert.h"
#include "delegate.h"
#include "array.h"

namespace etl
{
  //***************************************************************************
  /// An indexed delegate service.
  /// \tparam RANGE  The number of delegates to handle.
  /// \tparam OFFSET The lowest delegate id value.
  /// The delegate ids must range between OFFSET and OFFSET + RANGE - 1.
  //***************************************************************************
  template <const size_t RANGE, const size_t OFFSET = 0U>
  class delegate_service
  {
  public:

    //*************************************************************************
    /// Reset the delegate service.
    /// Sets all delegates to the internal default.
    //*************************************************************************
    delegate_service()
    {
      etl::delegate<void(size_t)> default_delegate = etl::delegate<void(size_t)>::create<delegate_service<RANGE, OFFSET>, &delegate_service<RANGE, OFFSET>::unhandled>(*this);

      lookup.fill(default_delegate);
    }

    //*************************************************************************
    /// Registers a delegate for the specified id.
    /// Compile time assert if the id is out of range.
    /// \tparam ID The id of the delegate.
    /// \param delegate Reference to the delegate.
    //*************************************************************************
    template <const size_t ID>
    void register_delegate(etl::delegate<void(size_t)> callback)
    {
      ETL_STATIC_ASSERT(ID < (OFFSET + RANGE), "Callback Id out of range");
      ETL_STATIC_ASSERT(ID >= OFFSET,          "Callback Id out of range");

      lookup[ID - OFFSET] = callback;
    }

    //*************************************************************************
    /// Registers a delegate for the specified id.
    /// No action if the id is out of range.
    /// \param id       Id of the delegate.
    /// \param delegate Reference to the delegate.
    //*************************************************************************
    void register_delegate(const size_t id, etl::delegate<void(size_t)> callback)
    {
      if ((id >= OFFSET) && (id < (OFFSET + RANGE)))
      {
        lookup[id - OFFSET] = callback;
      }
    }

    //*************************************************************************
    /// Registers an alternative delegate for unhandled ids.
    /// \param delegate A reference to the user supplied 'unhandled' delegate.
    //*************************************************************************
    void register_unhandled_delegate(etl::delegate<void(size_t)> callback)
    {
      unhandled_delegate = callback;
    }

    //*************************************************************************
    /// Executes the delegate function for the index.
    /// Compile time assert if the id is out of range.
    /// \tparam ID The id of the delegate.
    //*************************************************************************
    template <const size_t ID>
    void call()
    {
      ETL_STATIC_ASSERT(ID < (OFFSET + RANGE), "Callback Id out of range");
      ETL_STATIC_ASSERT(ID >= OFFSET,          "Callback Id out of range");

      lookup[ID - OFFSET](ID);
    }

    //*************************************************************************
    /// Executes the delegate function for the index.
    /// \param id Id of the delegate.
    //*************************************************************************
    void call(const size_t id)
    {
      if ((id >= OFFSET) && (id < (OFFSET + RANGE)))
      {
        lookup[id - OFFSET](id);
      }
      else
      {
        if (unhandled_delegate.is_valid())
        {
          unhandled_delegate(id);
        }
      }
    }

  private:

    //*************************************************************************
    /// The default callback function.
    /// Calls the user defined 'unhandled' callback if it exists.
    //*************************************************************************
    void unhandled(size_t id)
    {
      if (unhandled_delegate.is_valid())
      {
        unhandled_delegate(id);
      }
    }

    /// The default delegate for unhandled ids.
    etl::delegate<void(size_t)> unhandled_delegate;

    /// Lookup table of delegates.
    etl::array<etl::delegate<void(size_t)>, RANGE> lookup;
  };
}

#endif
