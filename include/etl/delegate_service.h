///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 John Wellbelove

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
  /// \tparam Range  The number of delegates to handle.
  /// \tparam Offset The lowest delegate id value.
  /// \tparam Delegates Pointer to an array of delegate pointers.
  /// The delegate ids must range between Offset and Offset + Range - 1.
  //***************************************************************************
#if ETL_USING_CPP11 && !defined(ETL_DELEGATE_FORCE_CPP03_IMPLEMENTATION)
  template <size_t Range, 
            size_t Offset = 0U,
            const etl::delegate<void(size_t)>* Delegates = nullptr>
  class delegate_service
  {
  public:

    typedef etl::delegate<void(size_t)> delegate_type;

    //*************************************************************************
    /// Executes the delegate function for the index.
    /// Compile time assert if the id is out of range.
    /// \tparam Id The id of the delegate.
    //*************************************************************************
    template <size_t Id>
    void call() const
    {
      ETL_STATIC_ASSERT(Id < (Offset + Range), "Callback Id out of range");
      ETL_STATIC_ASSERT(Id >= Offset,          "Callback Id out of range");

      Delegates[Id - Offset](Id);
    }

    //*************************************************************************
    /// Executes the delegate function for the index.
    /// \param id Id of the delegate.
    //*************************************************************************
    void call(size_t id) const
    {
      if ((id >= Offset) && (id < (Offset + Range)))
      {
        // Call the delegate with the specified Id.
        Delegates[id - Offset](id);
      }
      else
      {
        // Call the 'unhandled' delegate.
        Delegates[Range](id);
      }
    }
  };
#endif

  //***************************************************************************
  /// An indexed delegate service.
  /// \tparam Range  The number of delegates to handle.
  /// \tparam Offset The lowest delegate id value.
  /// The delegate ids must range between Offset and Offset + Range - 1.
  //***************************************************************************
  template <size_t Range, 
            size_t Offset>
#if ETL_USING_CPP11 && !defined(ETL_DELEGATE_FORCE_CPP03_IMPLEMENTATION)
  class delegate_service<Range, Offset, nullptr>
#else
  class delegate_service
#endif
  {
  public:

    typedef etl::delegate<void(size_t)> delegate_type;

    //*************************************************************************
    /// Default constructor.
    /// Sets all delegates to the internal default.
    //*************************************************************************
    delegate_service()
    {
      delegate_type default_delegate = delegate_type::create<delegate_service<Range, Offset>, &delegate_service<Range, Offset>::unhandled>(*this);

      lookup.fill(default_delegate);
    }

    //*************************************************************************
    /// Registers a delegate for the specified id.
    /// Compile time assert if the id is out of range.
    /// \tparam Id The id of the delegate.
    /// \param delegate Reference to the delegate.
    //*************************************************************************
    template <size_t Id>
    void register_delegate(delegate_type callback)
    {
      ETL_STATIC_ASSERT(Id < (Offset + Range), "Callback Id out of range");
      ETL_STATIC_ASSERT(Id >= Offset, "Callback Id out of range");

      lookup[Id - Offset] = callback;
    }

    //*************************************************************************
    /// Registers a delegate for the specified id.
    /// No action if the id is out of range.
    /// \param id       Id of the delegate.
    /// \param delegate Reference to the delegate.
    //*************************************************************************
    void register_delegate(size_t id, delegate_type callback)
    {
      if ((id >= Offset) && (id < (Offset + Range)))
      {
        lookup[id - Offset] = callback;
      }
    }

    //*************************************************************************
    /// Registers an alternative delegate for unhandled ids.
    /// \param delegate A reference to the user supplied 'unhandled' delegate.
    //*************************************************************************
    void register_unhandled_delegate(delegate_type callback)
    {
      unhandled_delegate = callback;
    }

    //*************************************************************************
    /// Executes the delegate function for the index.
    /// Compile time assert if the id is out of range.
    /// \tparam Id The id of the delegate.
    //*************************************************************************
    template <size_t Id>
    void call() const
    {
      ETL_STATIC_ASSERT(Id < (Offset + Range), "Callback Id out of range");
      ETL_STATIC_ASSERT(Id >= Offset, "Callback Id out of range");

      lookup[Id - Offset](Id);
    }

    //*************************************************************************
    /// Executes the delegate function for the index.
    /// \param id Id of the delegate.
    //*************************************************************************
    void call(const size_t id) const
    {
      if ((id >= Offset) && (id < (Offset + Range)))
      {
        // Call the delegate with the specified Id.
        lookup[id - Offset](id);
      }
      else
      {
        // Call the 'unhandled' delegate.
        unhandled(id);
      }
    }

  private:

    //*************************************************************************
    /// The default callback function.
    /// Calls the user defined 'unhandled' callback if it exists.
    //*************************************************************************
    void unhandled(size_t id) const
    {
      if (unhandled_delegate.is_valid())
      {
        unhandled_delegate(id);
      }
    }

    /// The default delegate for unhandled ids.
    delegate_type unhandled_delegate;

    /// Lookup table of delegates.
    etl::array<delegate_type, Range> lookup;
  };
}

#endif

