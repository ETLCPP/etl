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

#ifndef ETL_PERSISTENCE_INCLUDED
#define ETL_PERSISTENCE_INCLUDED

#include "platform.h"
#include "memory.h"
#include "type_traits.h"
#include "error_handler.h"
#include "exception.h"

namespace etl 
{
  namespace experimental
  {
    //***************************************************************************
    /// Exception base for persistence
    //***************************************************************************
    class persistence_exception : public etl::exception
    {
    public:

      persistence_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
        : exception(reason_, file_name_, line_number_)
      {
      }
    };

    //***************************************************************************
    /// Size mismatch error
    //***************************************************************************
    class persistence_size_mismatch : public etl::experimental::persistence_exception
    {
    public:

      persistence_size_mismatch(string_type file_name_, numeric_type line_number_)
        : persistence_exception(ETL_ERROR_TEXT("persistence:size mismatch", ETL_PERSISTENCE_FILE_ID"A"), file_name_, line_number_)
      {
      }
    };

    //***************************************************************************
    /// Persistence interface.
    //***************************************************************************
    class ipersistence
    {
    public:

      virtual ~ipersistence()
      {
      }

      virtual void start() = 0;
      virtual void step(size_t n) = 0;

      virtual void save(const char* data, size_t length) = 0;
      virtual void load(char* data, size_t length) = 0;

      virtual void flush() = 0;

      struct stepper
      {
        stepper(size_t n_)
          : n(n_)
        {
        }

        size_t n;
      };
    };

    //***************************************************************************
    /// Persistence size calculator.
    //***************************************************************************
    class persistence_profiler : public ipersistence
    {
    public:

      persistence_profiler()
        : index(0U)
      {
      }

      void start() ETL_OVERRIDE
      {
        index = 0U;
      }

      void step(size_t n) ETL_OVERRIDE
      {
        index += n;
      }

      void save(const char* data, size_t length) ETL_OVERRIDE
      {
        index += length;
      }

      void load(char* data, size_t length) ETL_OVERRIDE
      {
      }

      void flush() ETL_OVERRIDE
      {
      }

      size_t size() const
      {
        return index;
      }

    private:

      size_t index;
    };

    //***************************************************************************
    /// Generic Save Persistent.
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_integral<T>::value || etl::is_floating_point<T>::value || etl::is_pointer<T>::value || etl::is_trivially_copyable<T>::value, void>::type
      persistence_save(etl::experimental::ipersistence& persistence, T value)
    {
      T temp(value);

      const char* pvalue = reinterpret_cast<const char*>(&temp);
      size_t      length = sizeof(temp);
      persistence.save(pvalue, length);
    }

    //***************************************************************************
    /// Generic Load Persistent.
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_integral<T>::value || etl::is_floating_point<T>::value || etl::is_pointer<T>::value || etl::is_trivially_copyable<T>::value, void>::type
      persistence_load(etl::experimental::ipersistence& persistence, T& value)
    {
      size_t length = sizeof(T);
      persistence.load(reinterpret_cast<char*>(&value), length);
    }

    //***************************************************************************
    /// Generic streaming operators.
    //***************************************************************************
    template <typename T>
    etl::experimental::ipersistence& operator <<(etl::experimental::ipersistence& ip, const T& value)
    {
      persistence_save(ip, value);

      return ip;
    }

    //*********************************
#if ETL_CPP11_SUPPORTED
    template <typename T>
    etl::experimental::ipersistence& operator <<(etl::experimental::ipersistence& ip, T&& value)
    {
      persistence_save(ip, etl::move(value));

      return ip;
    }
#endif

    //*********************************
    template <typename T>
    etl::experimental::ipersistence& operator >>(etl::experimental::ipersistence& ip, T& value)
    {
      persistence_load(ip, value);

      return ip;
    }

    //***************************************************************************
    /// Find the require persistence size for a value.
    //***************************************************************************
    template <typename T>
    size_t persistence_size(const T& value)
    {
      using etl::experimental::persistence_save;

      persistence_profiler profiler;

      persistence_save(profiler, value);

      return profiler.size();
    }

#if ETL_CPP11_SUPPORTED
    template <typename T>
    size_t persistence_size(T&& value)
    {
      using etl::experimental::persistence_save;

      persistence_profiler profiler;

      persistence_save(profiler, etl::move(value));

      return profiler.size();
    }
#endif

    //***************************************************************************
    /// Generic Step Persistent.
    //***************************************************************************
    template <typename T>
    void persistence_step(etl::experimental::ipersistence& persistence, const T& value)
    {
      persistence.step(persistence_size(value));
    }

#if ETL_CPP11_SUPPORTED
    template <typename T>
    void persistence_step(etl::experimental::ipersistence& persistence, T&& value)
    {
      persistence.step(persistence_size(etl::move(value)));
    }
#endif

    //*********************************
    inline etl::experimental::ipersistence& operator <<(etl::experimental::ipersistence& ip, etl::experimental::ipersistence::stepper step)
    {
      persistence_step(ip, step.n);

      return ip;
    }

    //*********************************
    inline etl::experimental::ipersistence& operator >>(etl::experimental::ipersistence& ip, etl::experimental::ipersistence::stepper step)
    {
      persistence_step(ip, step.n);

      return ip;
    }
  }
}

#endif
