///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove

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

#ifndef ETL_CONST_BASIC_STRING_INCLUDED
#define ETL_CONST_BASIC_STRING_INCLUDED

#include "platform.h"
#include "iterator.h"
#include "char_traits.h"

#if ETL_USING_CPP14

namespace etl
{
  //***************************************************************************
  /// A string implementation that uses a fixed size external const buffer.
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  class const_basic_string
  {
  public:

    using value_type      = T;
    using size_type       = size_t;
    using const_pointer   = const value_type*;
    using const_reference = const value_type&;
    using const_iterator  = const value_type*;

    using const_reverse_iterator = etl::reverse_iterator<const_iterator>;

    //*************************************************************************
    /// Gets the size of the string.
    //*************************************************************************
    ETL_NODISCARD
    constexpr size_t size() const noexcept
    {
      return buffer_size;
    }

    //*************************************************************************
    /// Gets the size of the string.
    //*************************************************************************
    ETL_NODISCARD
    constexpr size_t length() const noexcept
    {
      return buffer_size;
    }

    //*************************************************************************
    /// Gets the capacity of the string.
    //*************************************************************************
    ETL_NODISCARD
    constexpr size_t capacity() const noexcept
    {
      return buffer_size;
    }

    //*************************************************************************
    /// Gets the maximum size of the string.
    //*************************************************************************
    ETL_NODISCARD
    constexpr size_t max_size() const noexcept
    {
      return buffer_size;
    }

    //*************************************************************************
    /// Gets the amount of free space in the string.
    //*************************************************************************
    ETL_NODISCARD
    constexpr size_t available() const noexcept
    {
      return 0U;
    }

    //*************************************************************************
    /// Gets the start of the string.
    //*************************************************************************
    ETL_NODISCARD
    constexpr const_iterator begin() const noexcept
    {
      return p_buffer;
    }

    //*************************************************************************
    /// Gets the start of the string.
    //*************************************************************************
    ETL_NODISCARD
    constexpr const_iterator cbegin() const noexcept
    {
      return p_buffer;
    }

    //*************************************************************************
    /// Gets the reverse start of the string.
    //*************************************************************************
    ETL_NODISCARD
    constexpr const_reverse_iterator rbegin() const noexcept
    {
      return const_reverse_iterator(end());
    }

    //*************************************************************************
    /// Gets the reverse start of the string.
    //*************************************************************************
    ETL_NODISCARD
    constexpr const_reverse_iterator crbegin() const noexcept
    {
      return const_reverse_iterator(end());
    }

    //*************************************************************************
    /// Gets the end of the string.
    //*************************************************************************
    ETL_NODISCARD
    constexpr const_iterator end() const noexcept
    {
      return p_buffer + buffer_size;
    }

    //*************************************************************************
    /// Gets the end of the string.
    //*************************************************************************
    ETL_NODISCARD
    constexpr const_iterator cend() const noexcept
    {
      return p_buffer + buffer_size;
    }

    //*************************************************************************
    /// Gets the reverse end of the string.
    //*************************************************************************
    ETL_NODISCARD
    constexpr const_reverse_iterator rend() const noexcept
    {
      return const_reverse_iterator(begin());
    }

    //*************************************************************************
    /// Gets the reverse end of the string.
    //*************************************************************************
    ETL_NODISCARD
    constexpr const_reverse_iterator crend() const noexcept
    {
      return const_reverse_iterator(begin());
    }

    //*************************************************************************
    /// Gets the start of the string.
    //*************************************************************************
    ETL_NODISCARD
    constexpr const_pointer data() const noexcept
    {
      return p_buffer;
    }

    //*************************************************************************
    /// Gets the end of the string.
    //*************************************************************************
    ETL_NODISCARD
    constexpr const_pointer data_end() const noexcept
    {
      return p_buffer + buffer_size;
    }

    //*********************************************************************
    /// Return a pointer to a C string.
    //*********************************************************************
    ETL_NODISCARD
    constexpr const_pointer c_str() const
    {
      return p_buffer;
    }

    //*************************************************************************
    /// Is the string empty?
    //*************************************************************************
    ETL_NODISCARD
    constexpr bool empty() const noexcept
    {
      return buffer_size == 0U;
    }

    //*************************************************************************
    /// Is the string full?
    //*************************************************************************
    ETL_NODISCARD
    constexpr bool full() const noexcept
    {
      return true;
    }

    //*************************************************************************
    /// Is the string truncated?
    //*************************************************************************
    ETL_NODISCARD
    constexpr bool is_truncated() const noexcept
    {
      return truncated_state;
    }

    //*************************************************************************
    /// Is the string secure?
    //*************************************************************************
    ETL_NODISCARD
    constexpr bool is_secure() const noexcept
    {
      return false;
    }

    //*************************************************************************
    /// Returns a const reference to the value at the index.
    //*************************************************************************
    ETL_NODISCARD
    constexpr const_reference operator [](int i) const noexcept
    {
      return p_buffer[i];
    }

    //*************************************************************************
    /// Returns a const reference to the value at the index.
    //*************************************************************************
    ETL_NODISCARD
    constexpr const_reference at(int i) const noexcept
    {
      return p_buffer[i];
    }

    //*************************************************************************
    /// Returns a const reference to the value at the front of the string.
    //*************************************************************************
    ETL_NODISCARD
    constexpr const_reference front() const noexcept
    {
      return p_buffer[0];
    }

    //*************************************************************************
    /// Returns a const reference to the value at the front of the string.
    //*************************************************************************
    ETL_NODISCARD
    constexpr const_reference back() const noexcept
    {
      return p_buffer[buffer_size - 1U];
    }

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    constexpr const_basic_string(const value_type* buffer, size_t buff_size, bool is_truncated)
      : p_buffer(buffer)
      , buffer_size(buff_size)
      , truncated_state(is_truncated)
    {
    }

  private:

    const value_type* p_buffer;
    const size_t      buffer_size;
    const bool        truncated_state;
  };

  //***************************************************************************
  /// Operator overload to write to std basic_ostream
  ///\param os Reference to the output stream.
  ///\param str Reference to the string to write.
  ///\return Reference to the output stream, for chaining write operations.
  ///\ingroup string
  //***************************************************************************
#if ETL_USING_STL
  template <typename T>
  std::basic_ostream<T, std::char_traits<T> > &operator<<(std::basic_ostream<T, std::char_traits<T> > &os, 
                                                          const etl::const_basic_string<T>& str)
  {
    os.write(str.data(), str.size());
    return os;
  }
#endif
}

#endif
#endif
