///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#ifndef ETL_IO_PORT_INCLUDED
#define ETL_IO_PORT_INCLUDED

///\defgroup io_port io port
/// IO port access
///\ingroup utilities

#include <stdint.h>

#include "platform.h"
#include "nullptr.h"

#include "stl/iterator.h"

namespace etl
{
  //***************************************************************************
  /// Read write port.
  //***************************************************************************
  template <typename T, uintptr_t ADDRESS = 0>
  class io_port_rw : public std::iterator<std::forward_iterator_tag, T>
  {
  public:

    typedef volatile T*       pointer;
    typedef volatile const T* const_pointer;
    typedef volatile T&       reference;
    typedef volatile const T& const_reference;
  
    /// Read.
    operator T() const
    {
      return *reinterpret_cast<const_pointer>(ADDRESS);
    }

    /// Read.
    T read() const
    {
      return *reinterpret_cast<const_pointer>(ADDRESS);
    }

    /// Write.
    void write(T value_)
    {
      *reinterpret_cast<pointer>(ADDRESS) = value_;
    }

    /// Write.
    io_port_rw& operator =(T value_)
    {
      *reinterpret_cast<pointer>(ADDRESS) = value_;
      return *this;
    }

    /// Read / Write
    reference operator *()
    {
      return *reinterpret_cast<pointer>(ADDRESS);
    }

    /// Read
    const_reference operator *() const
    {
      return *reinterpret_cast<const_pointer>(ADDRESS);
    }

    /// Increment
    io_port_rw& operator ++()
    {
      return *this;
    }

    /// Increment
    io_port_rw operator ++(int)
    {
      return *this;
    }
    
    /// Get the IO port address.
    pointer get_address()
    {
      return reinterpret_cast<pointer>(ADDRESS);
    }

    /// Get the IO port address.
    const_pointer get_address() const
    {
      return reinterpret_cast<const_pointer>(ADDRESS);
    }

  private:

    /// Disabled.
    io_port_rw& operator =(const io_port_rw&);
  };

  //***************************************************************************
  /// Read only port.
  //***************************************************************************
  template <typename T, uintptr_t ADDRESS = 0>
  class io_port_ro : public std::iterator<std::input_iterator_tag, T>
  {
  public:

    typedef volatile T*       pointer;
    typedef volatile const T* const_pointer;
    typedef volatile T&       reference;
    typedef volatile const T& const_reference;

    /// Read.
    operator T() const
    {
      return *reinterpret_cast<const_pointer>(ADDRESS);
    }

    /// Read.
    T read() const
    {
      return *reinterpret_cast<const_pointer>(ADDRESS);
    }

    /// Read
    const_reference operator *() const
    {
      return *reinterpret_cast<const_pointer>(ADDRESS);
    }

    /// Increment
    io_port_ro& operator ++()
    {
      return *this;
    }

    /// Increment
    io_port_ro operator ++(int)
    {
      return *this;
    }

    /// Get the IO port address.
    pointer get_address()
    {
      return reinterpret_cast<pointer>(ADDRESS);
    }

    /// Get the IO port address.
    const_pointer get_address() const
    {
      return reinterpret_cast<const_pointer>(ADDRESS);
    }

  private:

    /// Write disabled.
    void operator =(T value);

    /// Disabled.
    io_port_ro& operator =(const io_port_ro&);
  };

  //***************************************************************************
  /// Write only port.
  //***************************************************************************
  template <typename T, uintptr_t ADDRESS = 0>
  class io_port_wo : public std::iterator<std::output_iterator_tag, T>
  {
  public:

    typedef volatile T*       pointer;
    typedef volatile const T* const_pointer;
    typedef volatile T&       reference;
    typedef volatile const T& const_reference;

    /// Write.
    void operator =(T value)
    {
      *reinterpret_cast<pointer>(ADDRESS) = value;
    }

    /// Write.
    void write(T value_)
    {
      *reinterpret_cast<pointer>(ADDRESS) = value_;
    }

    /// Write
    io_port_wo& operator *()
    {
      return *this;
    }

    /// Increment
    io_port_wo& operator ++()
    {
      return *this;
    }

    /// Increment
    io_port_wo operator ++(int)
    {
      return *this;
    }

    /// Get the IO port address.
    pointer get_address()
    {
      return reinterpret_cast<pointer>(ADDRESS);
    }

    /// Get the IO port address.
    const_pointer get_address() const
    {
      return reinterpret_cast<const_pointer>(ADDRESS);
    }

  private:

    /// Read disabled.
    operator T() const;

    /// Disabled.
    io_port_wo& operator =(const io_port_wo&);
  };

  //***************************************************************************
  /// Write only port with shadow register.
  //***************************************************************************
  template <typename T, uintptr_t ADDRESS = 0>
  class io_port_wos : public std::iterator<std::forward_iterator_tag, T>
  {
  public:

    typedef volatile T*       pointer;
    typedef volatile const T* const_pointer;
    typedef volatile T&       reference;
    typedef volatile const T& const_reference;

    /// Read.
    operator T() const
    {
      return shadow_value;
    }

    /// Read.
    T read() const
    {
      return shadow_value;
    }

    /// Write.
    void write(T value_)
    {
      shadow_value = value_;
      *reinterpret_cast<pointer>(ADDRESS) = shadow_value;
    }

    /// Write.
    io_port_wos& operator =(T value_)
    {
      shadow_value = value_;
      *reinterpret_cast<pointer>(ADDRESS) = shadow_value;
      return *this;
    }

    /// Read / Write
    io_port_wos& operator *()
    {
      return *this;
    }

    /// Read
    const_reference operator *() const
    {
      return shadow_value;
    }

    /// Increment
    io_port_wos& operator ++()
    {
      return *this;
    }

    /// Increment
    io_port_wos operator ++(int)
    {
      return *this;
    }

    /// Get the IO port address.
    pointer get_address()
    {
      return reinterpret_cast<pointer>(ADDRESS);
    }

  private:

    /// Disabled.
    io_port_wos& operator =(const io_port_wos&);

    T shadow_value;
  };

  //***************************************************************************
  /// Read write port.
  /// Specialisation for dynamic addresses.
  //***************************************************************************
  template <typename T>
  class io_port_rw<T, 0> : public std::iterator<std::forward_iterator_tag, T>
  {
  public:

    typedef volatile T*       pointer;
    typedef volatile const T* const_pointer;
    typedef volatile T&       reference;
    typedef volatile const T& const_reference;

    /// Default constructor.
    io_port_rw()
      : address(nullptr)
    {
    }

    /// Constructor.
    io_port_rw(void* address_)
      : address(reinterpret_cast<pointer>(address_))
    {
    }

    /// Copy Constructor.
    io_port_rw(const io_port_rw& other_)
      : address(reinterpret_cast<pointer>(other_.address))
    {
    }

    /// Assignment.
    io_port_rw& operator =(const io_port_rw& other_)
    {
      address = other_.address;
      return *this;
    }

    /// Set the IO port address.
    void set_address(void* address_)
    {
      address = reinterpret_cast<pointer>(address_);
    }

    /// Get the IO port address.
    pointer get_address()
    {
      return address;
    }

    /// Get the IO port address.
    const_pointer get_address() const
    {
      return address;
    }

    /// Read.
    operator T() const
    {
      return *address;
    }

    /// Read.
    T read() const
    {
      return *address;
    }

    /// Write.
    void write(T value_)
    {
      *address = value_;
    }

    /// Write.
    io_port_rw& operator =(T value_)
    {
      *address = value_;
      return *this;
    }

    /// Read / Write
    reference operator *()
    {
      return *address;
    }

    /// Read
    const_reference operator *() const
    {
      return *address;
    }

    /// Increment
    io_port_rw& operator ++()
    {
      return *this;
    }

    /// Increment
    io_port_rw operator ++(int)
    {
      return *this;
    }

  private:

    pointer address;
  };

  //***************************************************************************
  /// Read only port.
  /// Specialisation for dynamic addresses.
  //***************************************************************************
  template <typename T>
  class io_port_ro<T, 0> : public std::iterator<std::input_iterator_tag, T>
  {
  public:

    typedef volatile T*       pointer;
    typedef volatile const T* const_pointer;
    typedef volatile T&       reference;
    typedef volatile const T& const_reference;

    /// Default constructor.
    io_port_ro()
      : address(nullptr)
    {
    }

    /// Constructor.
    io_port_ro(void* address_)
      : address(reinterpret_cast<pointer>(address_))
    {
    }

    /// Copy Constructor.
    io_port_ro(const io_port_ro& other_)
      : address(reinterpret_cast<pointer>(other_.address))
    {
    }

    /// Assignment.
    io_port_ro& operator =(const io_port_ro& other_)
    {
      address = other_.address;
      return *this;
    }

    /// Set the IO port address.
    void set_address(void* address_)
    {
      address = reinterpret_cast<pointer>(address_);
    }

    /// Get the IO port address.
    const_pointer get_address() const
    {
      return address;
    }

    /// Read.
    operator T() const
    {
      return *address;
    }

    /// Read.
    T read() const
    {
      return *address;
    }

    /// Read
    const_reference operator *() const
    {
      return *address;
    }

    /// Increment
    io_port_ro& operator ++()
    {
      return *this;
    }

    /// Increment
    io_port_ro operator ++(int)
    {
      return *this;
    }

  private:

    /// Write disabled.
    void operator =(T value);

    pointer address;
  };

  //***************************************************************************
  /// Write only port.
  /// Specialisation for dynamic addresses.
  //***************************************************************************
  template <typename T>
  class io_port_wo<T, 0> : public std::iterator<std::output_iterator_tag, T>
  {
  public:

    typedef volatile T*       pointer;
    typedef volatile const T* const_pointer;
    typedef volatile T&       reference;
    typedef volatile const T& const_reference;

    /// Default constructor.
    io_port_wo()
      : address(nullptr)
    {
    }

    /// Constructor.
    io_port_wo(void* address_)
      : address(reinterpret_cast<pointer>(address_))
    {
    }

    /// Copy Constructor.
    io_port_wo(const io_port_wo& other_)
      : address(reinterpret_cast<pointer>(other_.address))
    {
    }

    /// Assignment.
    io_port_wo& operator =(const io_port_wo& other_)
    {
      address = other_.address;
      return *this;
    }

    /// Set the IO port address.
    void set_address(void* address_)
    {
      address = reinterpret_cast<pointer>(address_);
    }

    /// Get the IO port address.
    pointer get_address()
    {
      return address;
    }

    /// Get the IO port address.
    const_pointer get_address() const
    {
      return address;
    }

    /// Write.
    void write(T value_)
    {
      *address = value_;
    }

    /// Write.
    void operator =(T value)
    {
      *address = value;
    }

    /// Write
    io_port_wo& operator *()
    {
      return *this;
    }

    /// Increment
    io_port_wo& operator ++()
    {
      return *this;
    }

    /// Write
    io_port_wo operator ++(int)
    {
      return *this;
    }
    
  private:

    /// Read disabled.
    operator T() const;

    pointer address;
  };

  //***************************************************************************
  /// Write only port with shadow register.
  /// Specialisation for dynamic addresses.
  //***************************************************************************
  template <typename T>
  class io_port_wos<T, 0> : public std::iterator<std::forward_iterator_tag, T>
  {
  public:

    typedef volatile T*       pointer;
    typedef volatile const T* const_pointer;
    typedef volatile T&       reference;
    typedef volatile const T& const_reference;

    class iterator : public std::iterator<std::bidirectional_iterator_tag, T>
    {
      typedef io_port_wos<T, 0> iop_t;

    public:

      iterator(iop_t& iop)
        : p_iop(&iop)
      {
      }

      iterator(const iterator& other)
        : p_iop(other.p_iop)
      {
      }

      iterator& operator =(const iterator& other)
      {
        p_iop = other.p_iop;
        return *this;
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
    
    /// Default constructor.
    io_port_wos()
      : address(nullptr)
    {
    }

    /// Constructor.
    io_port_wos(void* address_)
      : address(reinterpret_cast<pointer>(address_))
    {
    }

    /// Copy Constructor.
    io_port_wos(const io_port_wos& other_)
      : shadow_value(other_.shadow_value),
        address(reinterpret_cast<pointer>(other_.address))
    {
    }

    /// Assignment.
    io_port_wos& operator =(const io_port_wos& other_)
    {
      shadow_value = other_.shadow_value;
      address      = other_.address;
      return *this;
    }

    /// Set the IO port address.
    void set_address(void* address_)
    {
      address = reinterpret_cast<pointer>(address_);
    }

    /// Get the IO port address.
    pointer get_address()
    {
      return address;
    }

    /// Get the IO port address.
    const_pointer get_address() const
    {
      return address;
    }

    /// Get the iterator.
    iterator get_iterator()
    {
      return iterator(*this);
    }

    /// Read.
    operator T() const
    {
      return shadow_value;
    }

    /// Read.
    T read() const
    {
      return shadow_value;
    }

    /// Write.
    void write(T value_)
    {
      shadow_value = value_;
      *address = shadow_value;
    }

    /// Write.
    io_port_wos& operator =(T value_)
    {
      shadow_value = value_;
      *address     = shadow_value;
      return *this;
    }

    /// Read / Write
    io_port_wos&  operator *()
    {
      return *this;
    }

    /// Read
    const_reference operator *() const
    {
      return shadow_value;
    }

    /// Increment
    io_port_wos& operator ++()
    {
      return *this;
    }

    /// Increment
    io_port_wos operator ++(int)
    {
      return *this;
    }
    
  private:

    T       shadow_value;
    pointer address;
  };
}

#endif
