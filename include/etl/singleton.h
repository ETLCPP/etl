///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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

#ifndef ETL_SINGLETON_INCLUDED
#define ETL_SINGLETON_INCLUDED

#include "platform.h"
#include "static_assert.h"
#include "type_traits.h"

namespace etl
{
  //***************************************************************************
  /// Implementation of the Singleton pattern.
  //***************************************************************************
  template <typename T>
  class singleton
  {
  public:
    
    //*********************************
    /// Get the single instance of T.
    //*********************************
    static T& get()
    {
      ETL_STATIC_ASSERT((etl::is_base_of<Singleton<T>, T>::value), "T is not derived from etl::singleton<T>");
      static T obj;
      return obj;
    }

  protected:

    singleton() {};
    virtual ~singleton() {};

    // Deleted functions.
    singleton(const singleton&) ETL_DELETE;
    singleton& operator=(const singleton&) ETL_DELETE;

#if ETL_CPP11_SUPPORTED
    singleton(singleton&&) ETL_DELETE;
    singleton& operator=(singleton&&) ETL_DELETE;
#endif
  };
}

#endif
