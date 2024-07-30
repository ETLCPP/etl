///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

///\defgroup io_port io_port
/// IO port access
///\ingroup utilities

#include "platform.h"
#include "nullptr.h"
#include "iterator.h"
#include "utility.h"
#include "static_assert.h"

#include <stdint.h>

namespace etl
{
  template <typename, uintptr_t>
  class io_port_rw;

  namespace private_io_port
  {
    //***************************************************************************
    /// Common io_port iterator implementation
    //***************************************************************************
    template <typename TIO_Port>
    class iterator : public etl::iterator<ETL_OR_STD::forward_iterator_tag, typename TIO_Port::value_type>
    {
    public:

      //********************************
      /// Allow TIO_Port access
      //********************************
      friend TIO_Port;

      //********************************
      /// Allow const_iterator access
      //********************************
      template <typename U>
      friend class const_iterator;

      //********************************
      /// Types
      //********************************
      typedef TIO_Port io_port_type;
      typedef typename TIO_Port::value_type value_type;

      //********************************
      /// 
      //********************************
      iterator()
        : p_iop(ETL_NULLPTR)
      {
      }

      //********************************
      /// 
      //********************************
      iterator(const iterator& other)
        : p_iop(other.p_iop)
      {
      }

      //********************************
      /// 
      //********************************
      iterator& operator =(const iterator& other)
      {
        p_iop = other.p_iop;
        return *this;
      }

      //********************************
      /// 
      //********************************
      io_port_type& operator *()
      {
        return *p_iop;
      }

      //********************************
      /// 
      //********************************
      const io_port_type& operator *() const
      {
        return *p_iop;
      }

      //********************************
      /// Pre-increment operator.
      //********************************
      iterator& operator ++()
      {
        return *this;
      }

      //********************************
      /// Post-increment operator.
      //********************************
      iterator operator ++(int)
      {
        return *this;
      }

    private:

      //********************************
      /// 
      //********************************
      iterator(io_port_type& iop)
        : p_iop(&iop)
      {
      }

      io_port_type* p_iop;
    };

    //***************************************************************************
    /// Common io_port const_iterator implementation
    //***************************************************************************
    template <typename TIO_Port>
    class const_iterator : public etl::iterator<ETL_OR_STD::forward_iterator_tag, const typename TIO_Port::value_type>
    {
    private:

      typedef etl::private_io_port::iterator<TIO_Port> iterator_type;

    public:

      //********************************
      /// Allow TIO_Port access
      //********************************
      friend TIO_Port;

      //********************************
      /// Types
      //********************************
      typedef TIO_Port io_port_type;
      typedef const typename TIO_Port::value_type value_type;

      //********************************
      /// 
      //********************************
      const_iterator()
        : p_iop(ETL_NULLPTR)
      {
      }

      //********************************
      /// 
      //********************************
      const_iterator(const iterator_type& other)
        : p_iop(other.p_iop)
      {
      }

      //********************************
      /// 
      //********************************
      const_iterator(const const_iterator& other)
        : p_iop(other.p_iop)
      {
      }

      //********************************
      /// 
      //********************************
      const_iterator& operator =(const iterator_type& other)
      {
        p_iop = other.p_iop;
        return *this;
      }

      //********************************
      /// 
      //********************************
      const_iterator& operator =(const const_iterator& other)
      {
        p_iop = other.p_iop;
        return *this;
      }

      //********************************
      /// 
      //********************************
      const io_port_type& operator *() const
      {
        return *p_iop;
      }

      //********************************
      /// Pre-increment operator.
      //********************************
      const_iterator& operator ++()
      {
        return *this;
      }

      //********************************
      /// Post-increment operator.
      //********************************
      const_iterator operator ++(int)
      {
        return *this;
      }

    private:

      //********************************
      /// 
      //********************************
      const_iterator(const io_port_type& iop)
        : p_iop(&iop)
      {
      }

      const io_port_type* p_iop;
    };
  }

  //***************************************************************************
  /// Read write port.
  //***************************************************************************
  template <typename T, uintptr_t Address = 0>
  class io_port_rw
  {
  public:

    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    typedef T                 value_type;
    typedef volatile T*       pointer;
    typedef volatile const T* const_pointer;
    typedef volatile T&       reference;
    typedef volatile const T& const_reference;

    typedef etl::private_io_port::iterator<io_port_rw<T, Address> >       iterator;
    typedef etl::private_io_port::const_iterator<io_port_rw<T, Address> > const_iterator;

    //**********************************
    /// Get an iterator to this port.
    //**********************************
    iterator iter()
    {
      return iterator(*this);
    }

    //**********************************
    /// Get a const_iterator to this port.
    //**********************************
    const_iterator iter() const
    {
      return const_iterator(*this);
    }

    //**********************************
    /// Get a const_iterator to this port.
    //**********************************
    const_iterator citer() const
    {
      return const_iterator(*this);
    }

    //**********************************
    /// Read.
    //**********************************
    operator value_type() const
    {
      return *reinterpret_cast<const_pointer>(Address);
    }

    //**********************************
    /// Read.
    //**********************************
    value_type read() const
    {
      return *reinterpret_cast<const_pointer>(Address);
    }

    //**********************************
    /// Write.
    //**********************************
    void write(value_type value_)
    {
      *reinterpret_cast<pointer>(Address) = value_;
    }

    //**********************************
    /// Write.
    //**********************************
    io_port_rw& operator =(value_type value_)
    {
      *reinterpret_cast<pointer>(Address) = value_;
      return *this;
    }

    //**********************************
    /// Or-Equals operator.
    //**********************************
    io_port_rw& operator |=(value_type value)
    {
      pointer    address = reinterpret_cast<pointer>(Address);
      value_type temp = *address;
      temp |= value;
      *address = temp;

      return *this;
    }

    //**********************************
    /// And-Equals operator.
    //**********************************
    io_port_rw& operator &=(value_type value)
    {
      pointer    address = reinterpret_cast<pointer>(Address);
      value_type temp = *address;
      temp &= value;
      *address = temp;

      return *this;
    }

    //**********************************
    /// Exclusive-Or-Equals operator.
    //**********************************
    io_port_rw& operator ^=(value_type value)
    {
      pointer    address = reinterpret_cast<pointer>(Address);
      value_type temp = *address;
      temp ^= value;
      *address = temp;

      return *this;
    }

    //**********************************
    /// Left-Shift-Equals operator.
    //**********************************
    io_port_rw& operator <<=(int shift)
    {
      pointer    address = reinterpret_cast<pointer>(Address);
      value_type temp = *address;
      temp <<= shift;
      *address = temp;

      return *this;
    }

    //**********************************
    /// Right-Shift-Equals operator.
    //**********************************
    io_port_rw& operator >>=(int shift)
    {
      pointer    address = reinterpret_cast<pointer>(Address);
      value_type temp = *address;
      temp >>= shift;
      *address = temp;

      return *this;
    }

    //**********************************
    /// Not operator.
    //**********************************
    value_type operator ~() const
    {
      return ~(*reinterpret_cast<pointer>(Address));
    }

    //**********************************
    /// Get the IO port address.
    //**********************************
    pointer get_address()
    {
      return reinterpret_cast<pointer>(Address);
    }

    //**********************************
    /// Get the IO port address.
    //**********************************
    const_pointer get_address() const
    {
      return reinterpret_cast<const_pointer>(Address);
    }

  private:

    /// Disabled.
    io_port_rw& operator =(const io_port_rw&);
  };

  //***************************************************************************
  /// Read only port.
  //***************************************************************************
  template <typename T, uintptr_t Address = 0>
  class io_port_ro
  {
  public:

    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    typedef T                 value_type;
    typedef volatile T*       pointer;
    typedef volatile const T* const_pointer;
    typedef volatile T&       reference;
    typedef volatile const T& const_reference;

    typedef etl::private_io_port::const_iterator<io_port_ro<T, Address> > const_iterator;

    //**********************************
    /// Get a const_iterator to this port.
    //**********************************
    const_iterator iter() const
    {
      return const_iterator(*this);
    }

    //**********************************
    /// Get a const_iterator to this port.
    //**********************************
    const_iterator citer() const
    {
      return const_iterator(*this);
    }

    //**********************************
    /// Read.
    //**********************************
    operator value_type() const
    {
      return *reinterpret_cast<const_pointer>(Address);
    }

    //**********************************
    /// Read.
    //**********************************
    value_type read() const
    {
      return *reinterpret_cast<const_pointer>(Address);
    }

    //**********************************
    /// Get the IO port address.
    //**********************************
    pointer get_address()
    {
      return reinterpret_cast<pointer>(Address);
    }

    //**********************************
    /// Get the IO port address.
    //**********************************
    const_pointer get_address() const
    {
      return reinterpret_cast<const_pointer>(Address);
    }

  private:

    /// Write disabled.
    void operator =(value_type value) ETL_DELETE;

    /// Disabled.
    io_port_ro& operator =(const io_port_ro&) ETL_DELETE;
  };

  //***************************************************************************
  /// Write only port.
  //***************************************************************************
  template <typename T, uintptr_t Address = 0>
  class io_port_wo
  {
  public:

    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    typedef T                 value_type;
    typedef volatile T*       pointer;
    typedef volatile const T* const_pointer;
    typedef volatile T&       reference;
    typedef volatile const T& const_reference;

    typedef etl::private_io_port::iterator<io_port_wo<T, Address> > iterator;

    //**********************************
    /// Get an iterator to this port.
    //**********************************
    iterator iter()
    {
      return iterator(*this);
    }

    //**********************************
    /// Write.
    //**********************************
    void operator =(value_type value)
    {
      *reinterpret_cast<pointer>(Address) = value;
    }

    //**********************************
    /// Write.
    //**********************************
    void write(value_type value_)
    {
      *reinterpret_cast<pointer>(Address) = value_;
    }

    //**********************************
    /// Get the IO port address.
    //**********************************
    pointer get_address()
    {
      return reinterpret_cast<pointer>(Address);
    }

    //**********************************
    /// Get the IO port address.
    //**********************************
    const_pointer get_address() const
    {
      return reinterpret_cast<const_pointer>(Address);
    }

  private:

    /// Read disabled.
    operator value_type() const ETL_DELETE;

    /// Disabled.
    io_port_wo& operator =(const io_port_wo&) ETL_DELETE;
  };

  //***************************************************************************
  /// Write only port with shadow register.
  //***************************************************************************
  template <typename T, uintptr_t Address = 0>
  class io_port_wos
  {
  public:

    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    typedef T                 value_type;
    typedef volatile T*       pointer;
    typedef volatile const T* const_pointer;
    typedef volatile T&       reference;
    typedef volatile const T& const_reference;

    typedef etl::private_io_port::iterator<io_port_wos<T, Address> >       iterator;
    typedef etl::private_io_port::const_iterator<io_port_wos<T, Address> > const_iterator;

    //**********************************
    /// Default constructor.
    //**********************************
    io_port_wos()
      : shadow_value(value_type())
    {
    }

    //**********************************
    /// Get an iterator to this port.
    //**********************************
    iterator iter()
    {
      return iterator(*this);
    }

    //**********************************
    /// Get a const_iterator to this port.
    //**********************************
    const_iterator iter() const
    {
      return const_iterator(*this);
    }

    //**********************************
    /// Get a const_iterator to this port.
    //**********************************
    const_iterator citer() const
    {
      return const_iterator(*this);
    }

    //**********************************
    /// Read.
    //**********************************
    operator value_type() const
    {
      return shadow_value;
    }

    //**********************************
    /// Read.
    //**********************************
    value_type read() const
    {
      return shadow_value;
    }

    //**********************************
    /// Write.
    //**********************************
    void write(value_type value_)
    {
      shadow_value = value_;
      *reinterpret_cast<pointer>(Address) = shadow_value;
    }

    //**********************************
    /// Write.
    //**********************************
    io_port_wos& operator =(value_type value_)
    {
      shadow_value = value_;
      *reinterpret_cast<pointer>(Address) = shadow_value;
      return *this;
    }

    //**********************************
    /// Or-Equals operator.
    //**********************************
    io_port_wos& operator |=(value_type value)
    {
      shadow_value |= value;
      *reinterpret_cast<pointer>(Address) = shadow_value;

      return *this;
    }

    //**********************************
    /// And-Equals operator.
    //**********************************
    io_port_wos& operator &=(value_type value)
    {
      shadow_value &= value;
      *reinterpret_cast<pointer>(Address) = shadow_value;

      return *this;
    }

    //**********************************
    /// Exclusive-Or-Equals operator.
    //**********************************
    io_port_wos& operator ^=(value_type value)
    {
      shadow_value ^= value;
      *reinterpret_cast<pointer>(Address) = shadow_value;

      return *this;
    }

    //**********************************
    /// Left-Shift-Equals operator.
    //**********************************
    io_port_wos& operator <<=(int shift)
    {
      shadow_value <<= shift;
      *reinterpret_cast<pointer>(Address) = shadow_value;

      return *this;
    }

    //**********************************
    /// Right-Shift-Equals operator.
    //**********************************
    io_port_wos& operator >>=(int shift)
    {
      shadow_value >>= shift;
      *reinterpret_cast<pointer>(Address) = shadow_value;

      return *this;
    }

    //**********************************
    /// Not operator.
    //**********************************
    value_type operator ~() const
    {
      return ~shadow_value;
    }

    //**********************************
    /// Get the IO port address.
    //**********************************
    pointer get_address()
    {
      return reinterpret_cast<pointer>(Address);
    }

  private:

    /// Disabled.
    io_port_wos& operator =(const io_port_wos&);

    value_type shadow_value;
  };

  //***************************************************************************
  /// Read write port.
  /// Specialisation for dynamic addresses.
  //***************************************************************************
  template <typename T>
  class io_port_rw<T, 0>
  {
  public:

    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    typedef T                 value_type;
    typedef volatile T*       pointer;
    typedef volatile const T* const_pointer;
    typedef volatile T&       reference;
    typedef volatile const T& const_reference;

    typedef etl::private_io_port::iterator<io_port_rw<T, 0> >       iterator;
    typedef etl::private_io_port::const_iterator<io_port_rw<T, 0> > const_iterator;

    //**********************************
    /// Default constructor.
    //**********************************
    io_port_rw()
      : address(ETL_NULLPTR)
    {
    }

    //**********************************
    /// Constructor.
    //**********************************
    io_port_rw(void* address_)
      : address(reinterpret_cast<pointer>(address_))
    {
    }

    //**********************************
    /// Copy Constructor.
    //**********************************
    io_port_rw(const io_port_rw& other_)
      : address(reinterpret_cast<pointer>(other_.address))
    {
    }

    //**********************************
    /// Assignment.
    //**********************************
    io_port_rw& operator =(const io_port_rw& other_)
    {
      address = other_.address;
      return *this;
    }

    //**********************************
    /// Get an iterator to this port.
    //**********************************
    iterator iter()
    {
      return iterator(*this);
    }

    //**********************************
    /// Get a const_iterator to this port.
    //**********************************
    const_iterator iter() const
    {
      return const_iterator(*this);
    }

    //**********************************
    /// Get a const_iterator to this port.
    //**********************************
    const_iterator citer() const
    {
      return const_iterator(*this);
    }

    //**********************************
    /// Or-Equals operator.
    //**********************************
    io_port_rw& operator |=(value_type value)
    {
      value_type temp = *address;
      temp |= value;
      *address = temp;

      return *this;
    }

    //**********************************
    /// And-Equals operator.
    //**********************************
    io_port_rw& operator &=(value_type value)
    {
      value_type temp = *address;
      temp &= value;
      *address = temp;

      return *this;
    }

    //**********************************
    /// Exclusive-Or-Equals operator.
    //**********************************
    io_port_rw& operator ^=(value_type value)
    {
      value_type temp = *address;
      temp ^= value;
      *address = temp;

      return *this;
    }

    //**********************************
    /// Left-Shift-Equals operator.
    //**********************************
    io_port_rw& operator <<=(int shift)
    {
      value_type temp = *address;
      temp <<= shift;
      *address = temp;

      return *this;
    }

    //**********************************
    /// Right-Shift-Equals operator.
    //**********************************
    io_port_rw& operator >>=(int shift)
    {
      value_type temp = *address;
      temp >>= shift;
      *address = temp;

      return *this;
    }

    //**********************************
    /// Not operator.
    //**********************************
    value_type operator ~() const
    {
      return ~(*address);
    }

    //**********************************
    /// Set the IO port address.
    //**********************************
    void set_address(void* address_)
    {
      address = reinterpret_cast<pointer>(address_);
    }

    //**********************************
    /// Get the IO port address.
    //**********************************
    pointer get_address()
    {
      return address;
    }

    //**********************************
    /// Get the IO port address.
    //**********************************
    const_pointer get_address() const
    {
      return address;
    }

    //**********************************
    /// Read.
    //**********************************
    operator value_type() const
    {
      return *address;
    }

    //**********************************
    /// Read.
    //**********************************
    value_type read() const
    {
      return *address;
    }

    //**********************************
    /// Write.
    //**********************************
    void write(value_type value_)
    {
      *address = value_;
    }

    //**********************************
    /// Write.
    //**********************************
    io_port_rw& operator =(value_type value_)
    {
      *address = value_;
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
  class io_port_ro<T, 0>
  {
  public:

    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    typedef T                 value_type;
    typedef volatile T*       pointer;
    typedef volatile const T* const_pointer;
    typedef volatile T&       reference;
    typedef volatile const T& const_reference;

    typedef etl::private_io_port::const_iterator<io_port_ro<T, 0> > const_iterator;

    //**********************************
    /// Default constructor.
    //**********************************
    io_port_ro()
      : address(ETL_NULLPTR)
    {
    }

    //**********************************
    /// Constructor.
    //**********************************
    io_port_ro(void* address_)
      : address(reinterpret_cast<pointer>(address_))
    {
    }

    //**********************************
    /// Copy Constructor.
    //**********************************
    io_port_ro(const io_port_ro& other_)
      : address(reinterpret_cast<pointer>(other_.address))
    {
    }

    //**********************************
    /// Assignment.
    //**********************************
    io_port_ro& operator =(const io_port_ro& other_)
    {
      address = other_.address;
      return *this;
    }

    //**********************************
    /// Get a const_iterator to this port.
    //**********************************
    const_iterator iter() const
    {
      return const_iterator(*this);
    }

    //**********************************
    /// Get a const_iterator to this port.
    //**********************************
    const_iterator citer() const
    {
      return const_iterator(*this);
    }

    //**********************************
    /// Set the IO port address.
    //**********************************
    void set_address(void* address_)
    {
      address = reinterpret_cast<pointer>(address_);
    }

    //**********************************
    /// Get the IO port address.
    //**********************************
    const_pointer get_address() const
    {
      return address;
    }

    //**********************************
    /// Read.
    //**********************************
    operator value_type() const
    {
      return *address;
    }

    //**********************************
    /// Read.
    //**********************************
    value_type read() const
    {
      return *address;
    }

  private:

    /// Write disabled.
    void operator =(value_type value) ETL_DELETE;

    pointer address;
  };

  //***************************************************************************
  /// Write only port.
  /// Specialisation for dynamic addresses.
  //***************************************************************************
  template <typename T>
  class io_port_wo<T, 0>
  {
  public:

    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    typedef T                 value_type;
    typedef volatile T*       pointer;
    typedef volatile const T* const_pointer;
    typedef volatile T&       reference;
    typedef volatile const T& const_reference;

    typedef etl::private_io_port::iterator<io_port_wo<T, 0> > iterator;
  
    //**********************************
    /// Default constructor.
    //**********************************
    io_port_wo()
      : address(ETL_NULLPTR)
    {
    }

    //**********************************
    /// Constructor.
    //**********************************
    io_port_wo(void* address_)
      : address(reinterpret_cast<pointer>(address_))
    {
    }

    //**********************************
    /// Copy Constructor.
    //**********************************
    io_port_wo(const io_port_wo& other_)
      : address(reinterpret_cast<pointer>(other_.address))
    {
    }

    //**********************************
    /// Assignment.
    //**********************************
    io_port_wo& operator =(const io_port_wo& other_)
    {
      address = other_.address;
      return *this;
    }

    //**********************************
    /// Get an iterator to this port.
    //**********************************
    iterator iter()
    {
      return iterator(*this);
    }

    //**********************************
    /// Set the IO port address.
    //**********************************
    void set_address(void* address_)
    {
      address = reinterpret_cast<pointer>(address_);
    }

    //**********************************
    /// Get the IO port address.
    //**********************************
    pointer get_address()
    {
      return address;
    }

    //**********************************
    /// Get the IO port address.
    //**********************************
    const_pointer get_address() const
    {
      return address;
    }

    //**********************************
    /// Write.
    //**********************************
    void write(value_type value_)
    {
      *address = value_;
    }

    //**********************************
    /// Write.
    //**********************************
    void operator =(value_type value)
    {
      *address = value;
    }

  private:

    /// Read disabled.
    operator value_type() const ETL_DELETE;

    pointer address;
  };

  //***************************************************************************
  /// Write only port with shadow register.
  /// Specialisation for dynamic addresses.
  //***************************************************************************
  template <typename T>
  class io_port_wos<T, 0>
  {
  public:

    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    typedef T                 value_type;
    typedef volatile T*       pointer;
    typedef volatile const T* const_pointer;
    typedef volatile T&       reference;
    typedef volatile const T& const_reference;

    typedef etl::private_io_port::iterator<io_port_wos<T, 0> >       iterator;
    typedef etl::private_io_port::const_iterator<io_port_wos<T, 0> > const_iterator;

    //**********************************
    /// Default constructor.
    //**********************************
    io_port_wos()
      : shadow_value(T())
      , address(ETL_NULLPTR)
    {
    }

    //**********************************
    /// Constructor.
    //**********************************
    io_port_wos(void* address_)
      : shadow_value(T())
      , address(reinterpret_cast<pointer>(address_))
    {
    }

    //**********************************
    /// Copy Constructor.
    //**********************************
    io_port_wos(const io_port_wos& other_)
      : shadow_value(other_.shadow_value)
      , address(reinterpret_cast<pointer>(other_.address))
    {
    }

    //**********************************
    /// Assignment.
    //**********************************
    io_port_wos& operator =(const io_port_wos& other_)
    {
      shadow_value = other_.shadow_value;
      address      = other_.address;
      return *this;
    }

    //**********************************
    /// Get an iterator to this port.
    //**********************************
    iterator iter()
    {
      return iterator(*this);
    }

    //**********************************
    /// Get a const_iterator to this port.
    //**********************************
    const_iterator iter() const
    {
      return const_iterator(*this);
    }

    //**********************************
    /// Get a const_iterator to this port.
    //**********************************
    const_iterator citer() const
    {
      return const_iterator(*this);
    }

    //**********************************
    /// Or-Equals operator.
    //**********************************
    io_port_wos& operator |=(value_type value)
    {
      shadow_value |= value;
      *address = shadow_value;

      return *this;
    }

    //**********************************
    /// And-Equals operator.
    //**********************************
    io_port_wos& operator &=(value_type value)
    {
      shadow_value &= value;
      *address = shadow_value;

      return *this;
    }

    //**********************************
    /// Exclusive-Or-Equals operator.
    //**********************************
    io_port_wos& operator ^=(value_type value)
    {
      shadow_value ^= value;
      *address = shadow_value;

      return *this;
    }

    //**********************************
    /// Left-Shift-Equals operator.
    //**********************************
    io_port_wos& operator <<=(int shift)
    {
      shadow_value <<= shift;
      *address = shadow_value;

      return *this;
    }

    //**********************************
    /// Right-Shift-Equals operator.
    //**********************************
    io_port_wos& operator >>=(int shift)
    {
      shadow_value >>= shift;
      *address = shadow_value;

      return *this;
    }

    //**********************************
    /// Not operator.
    //**********************************
    value_type operator ~() const
    {
      return ~shadow_value;
    }

    //**********************************
    /// Set the IO port address.
    //**********************************
    void set_address(void* address_)
    {
      address = reinterpret_cast<pointer>(address_);
    }

    //**********************************
    /// Get the IO port address.
    //**********************************
    pointer get_address()
    {
      return address;
    }

    //**********************************
    /// Get the IO port address.
    //**********************************
    const_pointer get_address() const
    {
      return address;
    }

    //**********************************
    /// Read.
    //**********************************
    operator value_type() const
    {
      return shadow_value;
    }

    //**********************************
    /// Read.
    //**********************************
    value_type read() const
    {
      return shadow_value;
    }

    //**********************************
    /// Write.
    //**********************************
    void write(value_type value_)
    {
      shadow_value = value_;
      *address     = shadow_value;
    }

    //**********************************
    /// Write.
    //**********************************
    io_port_wos& operator =(value_type value_)
    {
      shadow_value = value_;
      *address     = shadow_value;
      return *this;
    }

    //**********************************
    /// Read / Write
    //**********************************
    io_port_wos& operator *()
    {
      return *this;
    }

    //**********************************
    /// Read
    //**********************************
    const_reference operator *() const
    {
      return shadow_value;
    }

  private:

    value_type shadow_value;
    pointer    address;
  };
}

#endif
