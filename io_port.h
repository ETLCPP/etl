///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

Copyright(c) 2014 jwellbelove

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

#ifndef __ETL_IO_PORT__
#define __ETL_IO_PORT__

///\defgroup io_port io port
/// IO port access
///\ingroup utilities

#include <stdint.h>

#include "nullptr.h"
#include "parameter_type.h"

namespace etl
{
  //***************************************************************************
  /// Read write port.
  //***************************************************************************
  template <typename T, uintptr_t ADDRESS = 0>
  class io_port_rw
  {
  private:

    typedef volatile T* pointer_t;
    typedef typename etl::parameter_type<T>::type parameter_t;

  public:

    class iterator
    {
      typedef io_port_rw<T, ADDRESS> iop_t;

    public:

      iterator(iop_t& iop)
        : p_iop(&iop)
      {
      }

      iterator(const iterator& other)
        : p_iop(other)
      {
      }

      iterator& operator =(const iterator& other)
      {
        p_iop = other.p_iop;
      }

      iop_t& operator *()
      {
        return *p_iop;
      }

      const iop_t& operator *() const
      {
        return *p_iop;
      }

      iterator& operator ++()
      {
        return *this;
      }

      iterator operator ++(int)
      {
        return *this;
      }

      iterator& operator --()
      {
        return *this;
      }

      iterator operator --(int)
      {
        return *this;
      }

    private:

      iop_t* p_iop;
    };

    /// Read.
    operator T() volatile const
    {
      return *reinterpret_cast<pointer_t>(ADDRESS);
    }

    /// Read.
    T value() volatile const
    {
      return *reinterpret_cast<pointer_t>(ADDRESS);
    }

    /// Write.
    io_port_rw& operator =(parameter_t value)
    {
      *reinterpret_cast<pointer_t>(ADDRESS) = value;
      return *this;
    }

    /// Get the IO port address.
    pointer_t get_address()
    {
      return reinterpret_cast<pointer_t>(ADDRESS);
    }

    /// Get the iterator.
    iterator get_iterator()
    {
      return iterator(*this);
    }
  };

  //***************************************************************************
  /// Read only port.
  //***************************************************************************
  template <typename T, uintptr_t ADDRESS = 0>
  class io_port_ro
  {
  private:

    typedef volatile const T* pointer_t;
    typedef typename etl::parameter_type<T>::type parameter_t;

    /// Write disabled.
    void operator =(parameter_t value);

  public:

    /// Read.
    operator T() volatile const
    {
      return *reinterpret_cast<pointer_t>(ADDRESS);
    }

    /// Read.
    T value() volatile const
    {
      return *reinterpret_cast<pointer_t>(ADDRESS);
    }

    /// Get the IO port address.
    pointer_t get_address()
    {
      return reinterpret_cast<pointer_t>(ADDRESS);
    }
  };

  //***************************************************************************
  /// Write only port.
  //***************************************************************************
  template <typename T, uintptr_t ADDRESS = 0>
  class io_port_wo
  {
  private:

    typedef T* pointer_t;
    typedef typename etl::parameter_type<T>::type parameter_t;

    /// Read disabled.
    operator T() volatile const;

  public:

    /// Write.
    void operator =(parameter_t value)
    {
      *reinterpret_cast<pointer_t>(ADDRESS) = value;
    }

    /// Get the IO port address.
    pointer_t get_address()
    {
      return reinterpret_cast<pointer_t>(ADDRESS);
    }
  };

  //***************************************************************************
  /// Write only port with shadow register.
  //***************************************************************************
  template <typename T, uintptr_t ADDRESS = 0>
  class io_port_wos
  {
  private:

    typedef T* pointer_t;
    typedef typename etl::parameter_type<T>::type parameter_t;

  public:

    /// Read.
    operator T() const
    {
      return shadow_value;
    }

    /// Read.
    T value() const
    {
      return shadow_value;
    }

    /// Write.
    io_port_wos& operator =(parameter_t value)
    {
      shadow_value = value;
      *reinterpret_cast<pointer_t>(ADDRESS) = shadow_value;
      return *this;
    }

    /// Get the IO port address.
    pointer_t get_address()
    {
      return reinterpret_cast<pointer_t>(ADDRESS);
    }

  private:

    T shadow_value;
  };

  //***************************************************************************
  /// Read write port.
  /// Specialisation for dynamic addresses.
  //***************************************************************************
  template <typename T>
  class io_port_rw<T, 0>
  {
  private:

    typedef volatile T* pointer_t;
    typedef typename etl::parameter_type<T>::type parameter_t;

  public:

    // Default constructor.
    io_port_rw()
      : address(nullptr)
    {
    }

    // Constructor.
    io_port_rw(uint8_t* address_)
      : address(reinterpret_cast<pointer_t>(address_))
    {
    }

    /// Set the IO port address.
    void set_address(uintptr_t address_)
    {
      address = reinterpret_cast<pointer_t>(address_);
    }

    /// Get the IO port address.
    pointer_t get_address()
    {
      return address;
    }

    /// Read.
    operator T() volatile const
    {
      return *address;
    }

    /// Read.
    T value() volatile const
    {
      return *address;
    }

    /// Write.
    io_port_rw& operator =(parameter_t value)
    {
      *address = value;
      return *this;
    }

  private:

    pointer_t address;
  };

  //***************************************************************************
  /// Read only port.
  /// Specialisation for dynamic addresses.
  //***************************************************************************
  template <typename T>
  class io_port_ro<T, 0>
  {
  private:

    typedef volatile const T* pointer_t;

  public:

    // Default constructor.
    io_port_ro()
      : address(nullptr)
    {
    }

    // Constructor.
    io_port_ro(void* address_)
      : address(reinterpret_cast<pointer_t>(address_))
    {
    }

    void set_address(uintptr_t address_)
    {
      address = reinterpret_cast<pointer_t>(address_);
    }

    /// Get the IO port address.
    pointer_t get_address()
    {
      return address;
    }

    /// Read.
    operator T() volatile const
    {
      return *address;
    }

    /// Read.
    T value() volatile const
    {
      return *address;
    }

  private:

    typedef typename etl::parameter_type<T>::type parameter_t;

    /// Write disabled.
    void operator =(parameter_t value);

    pointer_t address;
  };

  //***************************************************************************
  /// Write only port.
  /// Specialisation for dynamic addresses.
  //***************************************************************************
  template <typename T>
  class io_port_wo<T, 0>
  {
  private:

    typedef T* pointer_t;

  public:

    typedef typename etl::parameter_type<T>::type parameter_t;

    // Default constructor.
    io_port_wo()
      : address(nullptr)
    {
    }

    // Constructor.
    io_port_wo(void* address_)
      : address(reinterpret_cast<pointer_t>(address_))
    {
    }

    /// Set the IO port address.
    void set_address(uintptr_t address_)
    {
      address = reinterpret_cast<pointer_t>(address_);
    }

    /// Get the IO port address.
    pointer_t get_address()
    {
      return address;
    }

    /// Write.
    void operator =(parameter_t value)
    {
      *address = value;
    }

  private:

    /// Read disabled.
    operator T() volatile const;

    pointer_t address;
  };

  //***************************************************************************
  /// Write only port with shadow register.
  /// Specialisation for dynamic addresses.
  //***************************************************************************
  template <typename T>
  class io_port_wos<T, 0>
  {
  private:

    typedef T* pointer_t;
    typedef typename etl::parameter_type<T>::type parameter_t;

  public:

    // Default constructor.
    io_port_wos()
      : address(nullptr)
    {
    }

    // Constructor.
    io_port_wos(void* address_)
      : address(reinterpret_cast<pointer_t>(address_))
    {
    }

    /// Set the IO port address.
    void set_address(uintptr_t address_)
    {
      address = reinterpret_cast<pointer_t>(address_);
    }

    /// Get the IO port address.
    pointer_t get_address()
    {
      return address;
    }

    /// Read.
    operator T() const
    {
      return shadow_value;
    }

    /// Read.
    T value() const
    {
      return shadow_value;
    }

    /// Write.
    io_port_wos& operator =(parameter_t value)
    {
      shadow_value = value;
      *address     = shadow_value;
      return *this;
    }

  private:

    T         shadow_value;
    pointer_t address;
  };
}

#endif
