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

#ifndef ETL_CALLBACK_SERVICE_INCLUDED
#define ETL_CALLBACK_SERVICE_INCLUDED

#include "nullptr.h"
#include "static_assert.h"
#include "function.h"

namespace etl
{
  //***************************************************************************
  /// An indexed callback service.
  /// \tparam NUMBER_OF_CALLBACKS The number of callbacks to handle.
  /// The callback ids must range between 0 and NUMBER_OF_CALLBACKS - 1.
  //***************************************************************************
  template <const size_t NUMBER_OF_CALLBACKS>
  class callback_service
  {
  public:

    //*************************************************************************
    /// Reset the callback service.
    /// Sets all callbacks to the internal default.
    //*************************************************************************
    static void initialize()
    {
      for (size_t i = 0; i < NUMBER_OF_CALLBACKS; ++i)
      {
        lookup[i] = &unhandled_callback;
      }

      p_unhandled = nullptr;
    }

    //*************************************************************************
    /// Registers a callback for the specified id.
    /// Compile time assert if the id is out of range.
    /// \tparam ID The id of the callback.
    /// \param callback reference to the callback.
    //*************************************************************************
    template <const size_t ID>
    static void register_callback(etl::ifunction<size_t>& callback)
    {
      ETL_STATIC_ASSERT(ID < NUMBER_OF_CALLBACKS, "Callback Id out of range");

      lookup[ID] = &callback;
    }

    //*************************************************************************
    /// Registers a callback for the specified id.
    /// No action if the id is out of range.
    /// \param id       Id of the callback.
    /// \param callback Reference to the callback.
    //*************************************************************************
    static void register_callback(const size_t id, etl::ifunction<size_t>& callback)
    {
      if (id < NUMBER_OF_CALLBACKS)
      {
        lookup[id] = &callback;
      }
    }

    //*************************************************************************
    /// Executes the callback function for the index.
    /// Compile time assert if the id is out of range.
    /// \tparam ID The id of the callback.
    //*************************************************************************
    template <const size_t ID>
    static void callback()
    {
      ETL_STATIC_ASSERT(ID < NUMBER_OF_CALLBACKS, "Callback Id out of range");

      (*lookup[ID])(ID);
    }

    //*************************************************************************
    /// Executes the callback function for the index.
    /// \param id Id of the callback.
    //*************************************************************************
    static void callback(const size_t id)
    {
      if (id < NUMBER_OF_CALLBACKS)
      {
        (*lookup[id])(id);
      }
      else
      {
        unhandled(id);
      }
    }

    //*************************************************************************
    /// Registers an alternative callback for unhandled ids.
    /// \param callback A reference to the user supplied 'unhandled' callback.
    //*************************************************************************
    static void register_unhandled_callback(etl::ifunction<size_t>& callback)
    {
      p_unhandled = &callback;
    }

  private:

    //*************************************************************************
    /// The default callback function.
    /// Does nothing.
    //*************************************************************************
    static void unhandled(size_t id)
    {
      if (p_unhandled != nullptr)
      {
        (*p_unhandled)(id);
      }
    }

    /// The default callback for unhandled ids.
    static etl::function_fp<size_t, etl::callback_service<NUMBER_OF_CALLBACKS>::unhandled> unhandled_callback;

    /// Pointer to the user defined 'unhandled' callback.
    static etl::ifunction<size_t>* p_unhandled;

    /// Lookup table of callbacks.
    static etl::ifunction<size_t>* lookup[NUMBER_OF_CALLBACKS];
  };

  template <const size_t NUMBER_OF_CALLBACKS>
  etl::function_fp<size_t, etl::callback_service<NUMBER_OF_CALLBACKS>::unhandled> etl::callback_service<NUMBER_OF_CALLBACKS>::unhandled_callback;

  template <const size_t NUMBER_OF_CALLBACKS>
  etl::ifunction<size_t>* etl::callback_service<NUMBER_OF_CALLBACKS>::lookup[NUMBER_OF_CALLBACKS];

  template <const size_t NUMBER_OF_CALLBACKS>
  etl::ifunction<size_t>* etl::callback_service<NUMBER_OF_CALLBACKS>::p_unhandled = nullptr;
}

#endif
