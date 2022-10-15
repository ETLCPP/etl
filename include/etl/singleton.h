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

#ifndef ETL_SINGLETON_INCLUDED
#define ETL_SINGLETON_INCLUDED

///\defgroup singleton singleton
/// Templated version of the singleton pattern.
///\ingroup etl

#include "platform.h"
#include "utility.h"
#include "error_handler.h"
#include "file_error_numbers.h"
#include "memory.h"

namespace etl
{
  //*************************************************************************
  /// Base singleton error exception.
  //*************************************************************************
  class singleton_exception : public etl::exception
  {
  public:

    singleton_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //*************************************************************************
  /// Singleton not created error exception.
  //*************************************************************************
  class singleton_not_created : public etl::singleton_exception
  {
  public:

    singleton_not_created(string_type file_name_, numeric_type line_number_)
      : singleton_exception(ETL_ERROR_TEXT("singleton:not created", ETL_SINGLETON_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

#if __cpp_rvalue_references && __cpp_variadic_templates && !defined(ETL_SINGLETON_FORCE_CPP03_IMPLEMENTATION)
  //*************************************************************************
  /// Singleton pattern base class.
  /// \ingroup singleton
  //*************************************************************************
  template<typename T>
  class singleton
  {
  public:

    using type = T;

    //*************************************************************************
    /// Constructs the instance of the singleton type.
    //*************************************************************************
    template <typename... TArgs>
    static void create(TArgs&&... args)
    {
      Data& data = get_data();

      if (!data.valid)
      {
        T* p = data.storage.begin();
        ::new (p) T(etl::forward<TArgs>(args)...);
        data.valid = true;
      }
    }

    //*************************************************************************
    /// Destructs the instance of the singleton type.
    //*************************************************************************
    static void destroy()
    {
      Data& data = get_data();

      if (data.valid)
      {
        T* p = data.storage.begin();
        p->~T();
        data.valid = false;
      }
    }

    //*************************************************************************
    /// Returns a reference to the instance.
    /// Asserts an etl::singleton_not_created if create has not been called.
    ///\return A reference to the instance.
    //*************************************************************************
    static T& instance()
    {
      Data& data = get_data();

      ETL_ASSERT(data.valid, ETL_ERROR(etl::singleton_not_created));

      return *data.storage.begin();
    }

    //*************************************************************************
    /// Checks the validity of the singleton.
    //*************************************************************************
    static bool is_valid()
    {
      Data& data = get_data();

      return data.valid;
    }

  private:

    //*************************************************************************
    /// The data for the singleton.
    //*************************************************************************
    struct Data
    {
      Data()
        : valid(false)
      {
      }

      etl::uninitialized_buffer_of<T, 1U> storage;
      bool valid;
    };

    //*************************************************************************
    /// Gets the static data for the singleton.
    //*************************************************************************
    static Data& get_data()
    {
      static Data data;

      return data;
    }

    // Disabled.
    singleton() = delete;
    singleton(const singleton&) = delete;
    singleton& operator =(const singleton&) = delete;   
  };
#else
  //*************************************************************************
  /// Singleton pattern base class.
  /// \ingroup singleton
  //*************************************************************************
  template<typename T>
  class singleton
  {
  public:

    typedef T type;

    //*************************************************************************
    /// Constructs the instance of the singleton type.
    //*************************************************************************
    static void create()
    {
      Data& data = get_data();

      if (!data.valid)
      {
        T* p = data.storage.begin();
        ::new (p) T();
        data.valid = true;
      }
    }

    //*************************************************************************
    /// Constructs the instance of the singleton type.
    /// 1 parameter.
    //*************************************************************************
    template <typename T1>
    static void create(const T1& p1)
    {
      Data& data = get_data();

      if (!data.valid)
      {
        T* p = data.storage.begin();
        ::new (p) T(p1);
        data.valid = true;
      }
    }

    //*************************************************************************
    /// Constructs the instance of the singleton type.
    /// 2 parameters.
    //*************************************************************************
    template <typename T1, typename T2>
    static void create(const T1& p1, const T2& p2)
    {
      Data& data = get_data();

      if (!data.valid)
      {
        T* p = data.storage.begin();
        ::new (p) T(p1, p2);
        data.valid = true;
      }
    }

    //*************************************************************************
    /// Constructs the instance of the singleton type.
    /// 3 parameters.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    static void create(const T1& p1, const T2& p2, const T3& p3)
    {
      Data& data = get_data();

      if (!data.valid)
      {
        T* p = data.storage.begin();
        ::new (p) T(p1, p2, p3);
        data.valid = true;
      }
    }

    //*************************************************************************
    /// Constructs the instance of the singleton type.
    /// 4 parameters.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    static void create(const T1& p1, const T2& p2, const T3& p3, const T4& p4)
    {
      Data& data = get_data();

      if (!data.valid)
      {
        T* p = data.storage.begin();
        ::new (p) T(p1, p2, p3, p4);
        data.valid = true;
      }
    }

    //*************************************************************************
    /// Destructs the instance of the singleton type.
    //*************************************************************************
    static void destroy()
    {
      Data& data = get_data();

      if (data.valid)
      {
        T* p = data.storage.begin();
        p->~T();
        data.valid = false;
      }
    }

    //*************************************************************************
    /// Returns a reference to the instance.
    /// Asserts an etl::singleton_not_created if create has not been called.
    ///\return A reference to the instance.
    //*************************************************************************
    static T& instance()
    {
      Data& data = get_data();

      ETL_ASSERT(data.valid, ETL_ERROR(etl::singleton_not_created));

      return *data.storage.begin();
    }

    //*************************************************************************
    /// Checks the validity of the singleton.
    //*************************************************************************
    static bool is_valid()
    {
      Data& data = get_data();

      return data.valid;
    }

  private:

    //*************************************************************************
    /// The data for the singleton.
    //*************************************************************************
    struct Data
    {
      Data()
        : valid(false)
      {
      }

      etl::uninitialized_buffer_of<T, 1U> storage;
      bool valid;
    };

    static Data& get_data()
    {
      static Data data;

      return data;
    }

    // Disabled.
    singleton();
    singleton(const singleton&);
    singleton& operator =(const singleton&);
  };
#endif
}

#endif

