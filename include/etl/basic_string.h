///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 John Wellbelove

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

#ifndef ETL_BASIC_STRING_INCLUDED
#define ETL_BASIC_STRING_INCLUDED

#include "platform.h"
#include "algorithm.h"
#include "iterator.h"
#include "functional.h"
#include "alignment.h"
#include "array.h"
#include "type_traits.h"
#include "error_handler.h"
#include "integral_limits.h"
#include "exception.h"
#include "memory.h"
#include "binary.h"
#include "flags.h"
#include "string_utilities.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#if ETL_USING_LIBC_WCHAR_H
  #include <wchar.h>
#endif

#if ETL_USING_STL && ETL_USING_CPP17
  #include <string_view>
#endif

#if ETL_USING_STL
  #include <ostream>
#endif

#include "private/minmax_push.h"

//*****************************************************************************
///\defgroup basic_string basic_string
/// A basic_string with the capacity defined at compile time.
///\ingroup containers
//*****************************************************************************

// Forward declaration of string_view
namespace etl 
{
  template <typename T, typename TTraits>
  class basic_string_view;
}

namespace etl
{
  //***************************************************************************
  ///\ingroup string
  /// Exception base for strings
  //***************************************************************************
  class string_exception : public etl::exception
  {
  public:

    string_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup string
  /// String empty exception.
  //***************************************************************************
  class string_empty : public etl::string_exception
  {
  public:

    string_empty(string_type file_name_, numeric_type line_number_)
      : string_exception(ETL_ERROR_TEXT("string:empty", ETL_BASIC_STRING_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup string
  /// String out of bounds exception.
  //***************************************************************************
  class string_out_of_bounds : public etl::string_exception
  {
  public:

    string_out_of_bounds(string_type file_name_, numeric_type line_number_)
      : string_exception(ETL_ERROR_TEXT("string:bounds", ETL_BASIC_STRING_FILE_ID"B"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup string
  /// String iterator exception.
  //***************************************************************************
  class string_iterator : public etl::string_exception
  {
  public:

    string_iterator(string_type file_name_, numeric_type line_number_)
      : string_exception(ETL_ERROR_TEXT("string:iterator", ETL_BASIC_STRING_FILE_ID"C"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup string
  /// String truncation exception.
  //***************************************************************************
  class string_truncation : public etl::string_exception
  {
  public:

    string_truncation(string_type file_name_, numeric_type line_number_)
      : string_exception(ETL_ERROR_TEXT("string:truncation", ETL_BASIC_STRING_FILE_ID"D"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup string
  /// The base class for all templated string types.
  //***************************************************************************
  namespace private_basic_string
  {
    //*************************************************************************
    template <typename T = void>
    class string_base_statics
    {
    public:

      typedef size_t size_type;

      static ETL_CONSTANT uint_least8_t IS_TRUNCATED    = etl::bit<0>::value;
      static ETL_CONSTANT uint_least8_t CLEAR_AFTER_USE = etl::bit<1>::value;
      
      static ETL_CONSTANT size_type npos = etl::integral_limits<size_type>::max;
    };

    template <typename T>
    ETL_CONSTANT uint_least8_t string_base_statics<T>::IS_TRUNCATED;

    template <typename T>
    ETL_CONSTANT uint_least8_t string_base_statics<T>::CLEAR_AFTER_USE;

    template <typename T>
    ETL_CONSTANT typename string_base_statics<T>::size_type string_base_statics<T>::npos;
  }

  //***************************************************************************
  class string_base : public private_basic_string::string_base_statics<>
  {
  public:

    typedef size_t size_type;

    //*************************************************************************
    /// Gets the current size of the string.
    ///\return The current size of the string.
    //*************************************************************************
    size_type size() const
    {
      return current_size;
    }

    //*************************************************************************
    /// Gets the current size of the string.
    ///\return The current size of the string.
    //*************************************************************************
    size_type length() const
    {
      return current_size;
    }

    //*************************************************************************
    /// Checks the 'empty' state of the string.
    ///\return <b>true</b> if empty.
    //*************************************************************************
    bool empty() const
    {
      return (current_size == 0);
    }

    //*************************************************************************
    /// Checks the 'full' state of the string.
    ///\return <b>true</b> if full.
    //*************************************************************************
    bool full() const
    {
      return current_size == CAPACITY;
    }

    //*************************************************************************
    /// Returns the capacity of the string.
    ///\return The capacity of the string.
    //*************************************************************************
    size_type capacity() const
    {
      return CAPACITY;
    }

    //*************************************************************************
    /// Returns the maximum possible size of the string.
    ///\return The maximum size of the string.
    //*************************************************************************
    size_type max_size() const
    {
      return CAPACITY;
    }

    //*************************************************************************
    /// Returns the remaining capacity.
    ///\return The remaining capacity.
    //*************************************************************************
    size_type available() const
    {
      return max_size() - size();
    }

    //*************************************************************************
    /// Returns whether the string was truncated by the last operation.
    ///\return Whether the string was truncated by the last operation.
    //*************************************************************************
    bool is_truncated() const
    {
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      return flags.test<IS_TRUNCATED>();
#else
      return false;
#endif
    }

    //*************************************************************************
    /// Returns whether the string was truncated by the last operation.
    /// Deprecated. Use is_truncated()
    ///\return Whether the string was truncated by the last operation.
    //*************************************************************************
    ETL_DEPRECATED
    bool truncated() const
    {
      return is_truncated();
    }

#if ETL_HAS_STRING_TRUNCATION_CHECKS
    //*************************************************************************
    /// Clears the 'truncated' flag.
    //*************************************************************************
    void clear_truncated()
    {
      flags.set<IS_TRUNCATED, false>();
    }
#endif

#if ETL_HAS_STRING_CLEAR_AFTER_USE
    //*************************************************************************
    /// Sets the 'secure' flag to the requested state.
    //*************************************************************************
    void set_secure()
    {
      flags.set<CLEAR_AFTER_USE>();
    }
#endif

    //*************************************************************************
    /// Gets the 'secure' state flag.
    //*************************************************************************
    bool is_secure() const
    {
#if ETL_HAS_STRING_CLEAR_AFTER_USE
      return flags.test<CLEAR_AFTER_USE>();
#else
      return false;
#endif
    }

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    string_base(size_type max_size_)
      : current_size(0)
      , CAPACITY(max_size_)
    {
    }

#if ETL_HAS_STRING_TRUNCATION_CHECKS
    //*************************************************************************
    /// Sets the 'truncated' flag.
    //*************************************************************************
    void set_truncated(bool status)
    {
      flags.set<IS_TRUNCATED>(status);
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~string_base()
    {
    }

    size_type       current_size;   ///< The current number of elements in the string.
    const size_type CAPACITY;       ///< The maximum number of elements in the string.

#if ETL_HAS_STRING_TRUNCATION_CHECKS || ETL_HAS_STRING_CLEAR_AFTER_USE
    etl::flags<uint_least8_t> flags;
#endif
  };

  //***************************************************************************
  /// The base class for specifically sized strings.
  /// Can be used as a reference type for all strings containing a specific type.
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  class ibasic_string : public etl::string_base
  {
  public:

    typedef ibasic_string<T> interface_type;

    typedef T                                     value_type;
    typedef T&                                    reference;
    typedef const T&                              const_reference;
    typedef T*                                    pointer;
    typedef const T*                              const_pointer;
    typedef T*                                    iterator;
    typedef const T*                              const_iterator;
    typedef ETL_OR_STD::reverse_iterator<iterator>       reverse_iterator;
    typedef ETL_OR_STD::reverse_iterator<const_iterator> const_reverse_iterator;

    typedef typename etl::iterator_traits<iterator>::difference_type difference_type;

    //*********************************************************************
    /// Returns an iterator to the beginning of the string.
    ///\return An iterator to the beginning of the string.
    //*********************************************************************
    iterator begin()
    {
      return &p_buffer[0];
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the string.
    ///\return A const iterator to the beginning of the string.
    //*********************************************************************
    const_iterator begin() const
    {
      return &p_buffer[0];
    }

    //*********************************************************************
    /// Returns an iterator to the end of the string.
    ///\return An iterator to the end of the string.
    //*********************************************************************
    iterator end()
    {
      return &p_buffer[current_size];
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the string.
    ///\return A const iterator to the end of the string.
    //*********************************************************************
    const_iterator end() const
    {
      return &p_buffer[current_size];
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the string.
    ///\return A const iterator to the beginning of the string.
    //*********************************************************************
    const_iterator cbegin() const
    {
      return &p_buffer[0];
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the string.
    ///\return A const iterator to the end of the string.
    //*********************************************************************
    const_iterator cend() const
    {
      return &p_buffer[current_size];
    }

    //*********************************************************************
    /// Returns an reverse iterator to the reverse beginning of the string.
    ///\return Iterator to the reverse beginning of the string.
    //*********************************************************************
    reverse_iterator rbegin()
    {
      return reverse_iterator(end());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the string.
    ///\return Const iterator to the reverse beginning of the string.
    //*********************************************************************
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(end());
    }

    //*********************************************************************
    /// Returns a reverse iterator to the end + 1 of the string.
    ///\return Reverse iterator to the end + 1 of the string.
    //*********************************************************************
    reverse_iterator rend()
    {
      return reverse_iterator(begin());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the string.
    ///\return Const reverse iterator to the end + 1 of the string.
    //*********************************************************************
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(begin());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the string.
    ///\return Const reverse iterator to the reverse beginning of the string.
    //*********************************************************************
    const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(cend());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the string.
    ///\return Const reverse iterator to the end + 1 of the string.
    //*********************************************************************
    const_reverse_iterator crend() const
    {
      return const_reverse_iterator(cbegin());
    }

    //*********************************************************************
    /// Resizes the string.
    /// If asserts or exceptions are enabled and the new size is larger than the
    ///\param new_size The new size.
    //*********************************************************************
    void resize(size_type new_size)
    {
      resize(new_size, 0);
    }

    //*********************************************************************
    /// Resizes the string.
    ///\param new_size The new size.
    ///\param value    The value to fill new elements with. Default = default constructed value.
    //*********************************************************************
    void resize(size_type new_size, T value)
    {
      if (new_size > CAPACITY)
      {
#if ETL_HAS_STRING_TRUNCATION_CHECKS
        set_truncated(true);

#if ETL_HAS_ERROR_ON_STRING_TRUNCATION
        ETL_ASSERT_FAIL(ETL_ERROR(string_truncation));
#endif
#endif
      }

      new_size = etl::min(new_size, CAPACITY);

      // Size up?
      if (new_size > current_size)
      {
        etl::fill(p_buffer + current_size, p_buffer + new_size, value);
      }

      current_size = new_size;
      p_buffer[new_size] = 0;
      cleanup();
    }

    //*********************************************************************
    /// Resizes the string and overwrites to data using the operation.
    //*********************************************************************
    template <typename TOperation>
    void resize_and_overwrite(size_type new_size, TOperation operation)
    {
      if (new_size > CAPACITY)
      {
        ETL_ASSERT_FAIL(ETL_ERROR(string_out_of_bounds));
      }

      current_size = operation(p_buffer, new_size);
      p_buffer[current_size] = '\0';
      cleanup();
    }

    //*********************************************************************
    /// Resizes the string, but doesn't initialise the free space
    /// except for a terminator null.
    ///\param new_size The new size.
    //*********************************************************************
    void uninitialized_resize(size_type new_size)
    {
      new_size = etl::min(new_size, CAPACITY);

      current_size = new_size;
      p_buffer[new_size] = 0;
    }

    //*********************************************************************
    /// Fills the string with the specified character.
    /// Does not change the string length.
    ///\param value The character used to fill the string.
    //*********************************************************************
    void fill(T value)
    {
      etl::fill(begin(), end(), value);
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'i'
    ///\param i The index.
    ///\return A reference to the value at index 'i'
    //*********************************************************************
    reference operator [](size_type i)
    {
      return p_buffer[i];
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'i'
    ///\param i The index.
    ///\return A const reference to the value at index 'i'
    //*********************************************************************
    const_reference operator [](size_type i) const
    {
      return p_buffer[i];
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'i'
    /// If asserts or exceptions are enabled, emits an etl::string_out_of_bounds if the index is out of range.
    ///\param i The index.
    ///\return A reference to the value at index 'i'
    //*********************************************************************
    reference at(size_type i)
    {
      ETL_ASSERT(i < size(), ETL_ERROR(string_out_of_bounds));
      return p_buffer[i];
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'i'
    /// If asserts or exceptions are enabled, emits an etl::string_out_of_bounds if the index is out of range.
    ///\param i The index.
    ///\return A const reference to the value at index 'i'
    //*********************************************************************
    const_reference at(size_type i) const
    {
      ETL_ASSERT(i < size(), ETL_ERROR(string_out_of_bounds));
      return p_buffer[i];
    }

    //*********************************************************************
    /// Returns a reference to the first element.
    ///\return A reference to the first element.
    //*********************************************************************
    reference front()
    {
      return p_buffer[0];
    }

    //*********************************************************************
    /// Returns a const reference to the first element.
    ///\return A const reference to the first element.
    //*********************************************************************
    const_reference front() const
    {
      return p_buffer[0];
    }

    //*********************************************************************
    /// Returns a reference to the last element.
    ///\return A reference to the last element.
    //*********************************************************************
    reference back()
    {
      return p_buffer[current_size - 1];
    }

    //*********************************************************************
    /// Returns a const reference to the last element.
    ///\return A const reference to the last element.
    //*********************************************************************
    const_reference back() const
    {
      return p_buffer[current_size - 1];
    }

    //*********************************************************************
    /// Returns a pointer to the beginning of the string data.
    ///\return A pointer to the beginning of the string data.
    //*********************************************************************
    pointer data()
    {
      return p_buffer;
    }

    //*********************************************************************
    /// Returns a const pointer to the beginning of the string data.
    ///\return A const pointer to the beginning of the string data.
    //*********************************************************************
    ETL_CONSTEXPR const_pointer data() const
    {
      return p_buffer;
    }

    //*********************************************************************
    /// Returns a pointer to the beginning of the string data.
    ///\return A pointer to the beginning of the string data.
    //*********************************************************************
    pointer data_end()
    {
      return p_buffer + current_size;
    }

    //*********************************************************************
    /// Returns a const pointer to the beginning of the string data.
    ///\return A const pointer to the beginning of the string data.
    //*********************************************************************
    const_pointer data_end() const
    {
      return p_buffer + current_size;
    }

    //*********************************************************************
    /// Assigns values to the string.
    /// Truncates if the string does not have enough free space.
    ///\param other The other string.
    //*********************************************************************
    void assign(const etl::ibasic_string<T>& other)
    {
      if (&other != this)
      {
        append_impl(begin(), other.begin(), other.end(), other.is_truncated(), other.is_secure());
      }
    }

    //*********************************************************************
    /// Assigns values to the string.
    /// Truncates if the string does not have enough free space.
    ///\param other The other string.
    ///\param subposition The position to start from.
    ///\param sublength   The length to copy.
    //*********************************************************************
    void assign(const etl::ibasic_string<T>& other, size_type subposition, size_type sublength)
    {
      if (&other != this)
      {
        if (sublength == npos)
        {
          sublength = other.size() - subposition;
        }

        ETL_ASSERT(subposition <= other.size(), ETL_ERROR(string_out_of_bounds));

        append_impl(begin(), other.begin() + subposition, other.begin() + subposition + sublength, other.is_truncated(), other.is_secure());
      }
    }

    //*********************************************************************
    /// Assigns values to the string.
    /// If asserts or exceptions are enabled, emits string_iterator if the iterators are reversed.
    /// Truncates if the string does not have enough free space.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
      append_impl(begin(), first, last, false, false);
    }

    //*********************************************************************
    /// Assigns values to the string.
    /// Truncates if the string does not have enough free space.
    ///\param other The other string.
    //*********************************************************************
    void assign(const_pointer str)
    {
      append_impl(begin(), str, false, false);
    }

    //*********************************************************************
    /// Assigns values to the string.
    /// Truncates if the string does not have enough free space.
    ///\param other The other string.
    ///\param length The length to copy.
    //*********************************************************************
    void assign(const_pointer str, size_type n)
    {
      append_impl(begin(), str, str + n, false, false);
    }

    //********************************************************************* 
    /// Assigns values to the string from a view.
    //*********************************************************************
    template <typename TOtherTraits>
    void assign(const etl::basic_string_view<T, TOtherTraits>& view)
    {
      append_impl(begin(), view.begin(), view.end(), false, false);
    }

    //*********************************************************************
    /// Assigns values to the string.
    /// Truncates if the string does not have enough free space.
    ///\param n     The number of elements to add.
    ///\param value The value to insert for each element.
    //*********************************************************************
    void assign(size_type n, T c)
    {
      clear();

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      set_truncated(n > CAPACITY);

#if ETL_HAS_ERROR_ON_STRING_TRUNCATION
      ETL_ASSERT(is_truncated == false, ETL_ERROR(string_truncation));
#endif
#endif

      n = etl::min(n, CAPACITY);

      etl::fill_n(begin(), n, c);
      current_size = n;
      p_buffer[current_size] = 0;
    }

    //*************************************************************************
    /// Clears the string.
    //*************************************************************************
    void clear()
    {
      initialise();
      cleanup();
    }

    //*********************************************************************
    /// Inserts a value at the end of the string.
    /// Sets 'truncated' if the string is already full.
    ///\param value The value to add.
    //*********************************************************************
    void push_back(T value)
    {
      if (current_size != CAPACITY)
      {
        p_buffer[current_size++] = value;
        p_buffer[current_size]   = 0;
      }
      else
      {
#if ETL_HAS_STRING_TRUNCATION_CHECKS
        set_truncated(true);

#if ETL_HAS_ERROR_ON_STRING_TRUNCATION
        ETL_ASSERT_FAIL(ETL_ERROR(string_truncation));
#endif
#endif
      }
    }

    //*************************************************************************
    /// Removes an element from the end of the string.
    /// Does nothing if the string is empty.
    //*************************************************************************
    void pop_back()
    {
      if (current_size != 0)
      {
        p_buffer[--current_size] = 0;
      }
    }

    //*********************************************************************
    /// Appends to the string.
    ///\param str The string to append.
    //*********************************************************************
    ibasic_string& append(const ibasic_string& str)
    {
      append_impl(end(), str.begin(), str.end(), str.is_truncated(), str.is_secure());

      return *this;
    }

    //*********************************************************************
    /// Appends to the string.
    ///\param str The string to append.
    ///\param subposition The position in str.
    ///\param sublength The number of characters.
    //*********************************************************************
    ibasic_string& append(const ibasic_string& str, size_type subposition, size_type sublength = npos)
    {
      if (sublength == npos)
      {
        sublength = str.size() - subposition;
      }

      ETL_ASSERT(subposition <= str.size(), ETL_ERROR(string_out_of_bounds));

      append_impl(end(), str.begin() + subposition, str.begin() + subposition + sublength, str.is_truncated(), str.is_secure());

      return *this;
    }

    //*********************************************************************
    /// Appends to the string.
    ///\param first The first of the characters to append.
    ///\param last  The last + 1 character to add.
    //*********************************************************************
    template <class TIterator>
    ibasic_string& append(TIterator first, TIterator last)
    {
      append_impl(end(), first, last, false, false);

      return *this;
    }

    //*********************************************************************
    /// Appends to the string.
    ///\param str The string to append.
    //*********************************************************************
    ibasic_string& append(const_pointer str)
    {
      append_impl(end(), str, false, false);

      return *this;
    }

    //*********************************************************************
    /// Appends to the string.
    ///\param str The string to append.
    ///\param n   The number of characters.
    //*********************************************************************
    ibasic_string& append(const_pointer str, size_type n)
    {
      append_impl(end(), str, str + n, false, false);

      return *this;
    }

    //*********************************************************************
    /// Appends to the string.
    ///\param view An etl::string_view.
    //*********************************************************************
    template <typename TOtherTraits>
    ibasic_string& append(const etl::basic_string_view<T, TOtherTraits>& view)
    {
      append_impl(end(), view.begin(), view.end(), false, false);

      return *this;
    }

    //*********************************************************************
    /// Appends to the string.
    ///\param n The number of characters.
    ///\param c The character.
    //*********************************************************************
    ibasic_string& append(size_type n, T c)
    {
      size_type free_space = CAPACITY - current_size;

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      set_truncated(n > free_space);

#if ETL_HAS_ERROR_ON_STRING_TRUNCATION
      ETL_ASSERT(is_truncated == false, ETL_ERROR(string_truncation));
#endif
#endif

      n = etl::min(n, size_t(free_space));

      etl::fill_n(end(), n, c);
      current_size += n;
      p_buffer[current_size] = 0;

      return *this;
    }

    //*********************************************************************
    /// Inserts a value to the string.
    ///\param position The position to insert before.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator position, T value)
    {
      // Quick hack, as iterators are pointers.
      iterator insert_position = to_iterator(position);

      if (current_size < CAPACITY)
      {
        // Not full yet.
        if (position != end())
        {
          // Insert in the middle.
          ++current_size;
          etl::mem_move(insert_position, end() - 1, insert_position + 1);
          *insert_position = value;
        }
        else
        {
          // Insert at the end.
          *insert_position = value;
          ++current_size;
        }
      }
      else
      {
        // Already full.
        if (position != end())
        {
          // Insert in the middle.
          etl::mem_move(insert_position, end() - 1, insert_position + 1);          
          *insert_position = value;
        }

#if ETL_HAS_STRING_TRUNCATION_CHECKS
        set_truncated(true);

#if ETL_HAS_ERROR_ON_STRING_TRUNCATION
        ETL_ASSERT_FAIL(ETL_ERROR(string_truncation));
#endif
#endif
      }

      p_buffer[current_size] = 0;

      return insert_position;
    }

    //*********************************************************************
    /// Inserts 'n' values to the string.
    ///\param position The position to insert before.
    ///\param n        The number of elements to add.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator position, size_type n, T value)
    {
      iterator position_ = to_iterator(position);

      if (n == 0)
      {
        return position_;
      }

      // Quick hack, as iterators are pointers.
      iterator insert_position = to_iterator(position);
      const size_type start = etl::distance(cbegin(), position);

      // No effect.
      if (start >= CAPACITY)
      {
#if ETL_HAS_STRING_TRUNCATION_CHECKS
        set_truncated(true);

#if ETL_HAS_ERROR_ON_STRING_TRUNCATION
        ETL_ASSERT_FAIL(ETL_ERROR(string_truncation));
#endif
#endif
        return to_iterator(position);;
      }

      // Fills the string to the end?
      if ((start + n) >= CAPACITY)
      {
        if ((current_size + n) > CAPACITY)
        {
#if ETL_HAS_STRING_TRUNCATION_CHECKS
          set_truncated(true);

#if ETL_HAS_ERROR_ON_STRING_TRUNCATION
          ETL_ASSERT_FAIL(ETL_ERROR(string_truncation));
#endif
#endif
        }

        current_size = CAPACITY;
        etl::fill(insert_position, end(), value);
      }
      else
      {
        // Lets do some shifting.
        const size_type shift_amount = n;
        const size_type to_position = start + shift_amount;
        const size_type remaining_characters = current_size - start;
        const size_type max_shift_characters = CAPACITY - start - shift_amount;
        const size_type characters_to_shift = etl::min(max_shift_characters, remaining_characters);

        // Will the string truncate?
        if ((start + shift_amount + remaining_characters) > CAPACITY)
        {
          current_size = CAPACITY;

#if ETL_HAS_STRING_TRUNCATION_CHECKS
          set_truncated(true);

#if ETL_HAS_ERROR_ON_STRING_TRUNCATION
          ETL_ASSERT_FAIL(ETL_ERROR(string_truncation));
#endif
#endif
        }
        else
        {
          current_size += shift_amount;
        }

        etl::mem_move(insert_position, insert_position + characters_to_shift, begin() + to_position);
        etl::fill(insert_position, insert_position + shift_amount, value);
      }

      p_buffer[current_size] = 0;

      return position_;
    }

    //*********************************************************************
    /// Inserts a range of values to the string.
    /// If asserts or exceptions are enabled, emits string_full if the string does not have enough free space.
    ///\param position The position to insert before.
    ///\param first    The first element to add.
    ///\param last     The last + 1 element to add.
    //*********************************************************************
    template <typename TIterator>
    iterator insert(const_iterator position, TIterator first, TIterator last)
    {
      iterator position_ = to_iterator(position);

      if (first == last)
      {
        return position_;
      }

      const size_type start = etl::distance(begin(), position_);
      const size_type n = etl::distance(first, last);

      // No effect.
      if (start >= CAPACITY)
      {
#if ETL_HAS_STRING_TRUNCATION_CHECKS
        set_truncated(true);

#if ETL_HAS_ERROR_ON_STRING_TRUNCATION
        ETL_ASSERT_FAIL(ETL_ERROR(string_truncation));
#endif
#endif
        return position_;
      }

      // Fills the string to the end?
      if ((start + n) >= CAPACITY)
      {
        if (((current_size + n) > CAPACITY))
        {
#if ETL_HAS_STRING_TRUNCATION_CHECKS
          set_truncated(true);

#if ETL_HAS_ERROR_ON_STRING_TRUNCATION
          ETL_ASSERT_FAIL(ETL_ERROR(string_truncation));
#endif
#endif
        }

        current_size = CAPACITY;

        position_ = copy_characters(first, etl::distance(position_, end()), position_);
      }
      else
      {
        // Lets do some shifting.
        const size_type shift_amount = n;
        const size_type to_position = start + shift_amount;
        const size_type remaining_characters = current_size - start;
        const size_type max_shift_characters = CAPACITY - start - shift_amount;
        const size_type characters_to_shift = etl::min(max_shift_characters, remaining_characters);

        // Will the string truncate?
        if ((start + shift_amount + remaining_characters) > CAPACITY)
        {
          current_size = CAPACITY;

#if ETL_HAS_STRING_TRUNCATION_CHECKS
          set_truncated(true);

#if ETL_HAS_ERROR_ON_STRING_TRUNCATION
          ETL_ASSERT_FAIL(ETL_ERROR(string_truncation));
#endif
#endif
        }
        else
        {
          current_size += shift_amount;
        }

        etl::mem_move(position_, position_ + characters_to_shift, begin() + to_position);
        //etl::copy_backward(position_, position_ + characters_to_shift, begin() + to_position + characters_to_shift);

        position_ = copy_characters(first, etl::distance(first, last), position_);
      }

      p_buffer[current_size] = 0;

      return position_;
    }

    //*********************************************************************
    /// Inserts a view to the string.
    /// If asserts or exceptions are enabled, emits string_full if the string does not have enough free space.
    ///\param position The position to insert before.
    ///\param view     The view element to add.
    //*********************************************************************
    template <typename TOtherTraits>
    iterator insert(const_iterator position, const etl::basic_string_view<T, TOtherTraits>& view)
    {
      return insert(position, view.begin(), view.end());
    }

    //*********************************************************************
    /// Inserts a string at the specified position.
    ///\param position The position to insert before.
    ///\param str      The string to insert.
    //*********************************************************************
    etl::ibasic_string<T>& insert(size_type position, const etl::ibasic_string<T>& str)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));

      insert(begin() + position, str.cbegin(), str.cend());

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      if (str.is_truncated())
      {
        set_truncated(true);

#if ETL_HAS_ERROR_ON_STRING_TRUNCATION
        ETL_ASSERT_FAIL(ETL_ERROR(string_truncation));
#endif
      }
#endif

      return *this;
    }

    //*********************************************************************
    /// Inserts a string at the specified position.
    ///\param position The position to insert before.
    ///\param view     The view to insert.
    //*********************************************************************
    template <typename TOtherTraits>
    etl::ibasic_string<T>& insert(size_type position, const etl::basic_string_view<T, TOtherTraits>& view)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));

      insert(begin() + position, view.cbegin(), view.cend());

      return *this;
    }

    //*********************************************************************
    /// Inserts a string at the specified position from subposition for sublength.
    ///\param position    The position to insert before.
    ///\param str         The string to insert.
    ///\param subposition The subposition to start from.
    ///\param sublength   The number of characters to insert.
    //*********************************************************************
    etl::ibasic_string<T>& insert(size_type position, const etl::ibasic_string<T>& str, size_type subposition, size_type sublength)
    {
      ETL_ASSERT(position    <= size(),     ETL_ERROR(string_out_of_bounds));
      ETL_ASSERT(subposition <= str.size(), ETL_ERROR(string_out_of_bounds));

      if ((sublength == npos) || (subposition + sublength > str.size()))
      {
        sublength = str.size() - subposition;
      }

      insert(begin() + position, str.cbegin() + subposition, str.cbegin() + subposition + sublength);

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      if (str.is_truncated())
      {
        set_truncated(true);

#if ETL_HAS_ERROR_ON_STRING_TRUNCATION
        ETL_ASSERT_FAIL(ETL_ERROR(string_truncation));
#endif
      }
#endif

      return *this;
    }

    //*********************************************************************
    /// Inserts a view at the specified position from subposition for sublength.
    ///\param position    The position to insert before.
    ///\param view        The view to insert.
    ///\param subposition The subposition to start from.
    ///\param sublength   The number of characters to insert.
    //*********************************************************************
    template <typename TOtherTraits>
    etl::ibasic_string<T>& insert(size_type position, const etl::basic_string_view<T, TOtherTraits>& view, size_type subposition, size_type sublength)
    {
      ETL_ASSERT(position    <= size(),      ETL_ERROR(string_out_of_bounds));
      ETL_ASSERT(subposition <= view.size(), ETL_ERROR(string_out_of_bounds));

      if ((sublength == npos) || (subposition + sublength > view.size()))
      {
        sublength = view.size() - subposition;
      }

      insert(begin() + position, view.cbegin() + subposition, view.cbegin() + subposition + sublength);

      return *this;
    }

    //*********************************************************************
    /// Inserts a string at the specified position from pointer.
    ///\param position The position to insert before.
    ///\param s        The string to insert.
    //*********************************************************************
    etl::ibasic_string<T>& insert(size_type position, const_pointer s)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));

      insert(begin() + position, s, s + etl::strlen(s));
      return *this;
    }

    //*********************************************************************
    /// Inserts a string at the specified position from pointer for n characters.
    ///\param position The position to insert before.
    ///\param s        The string to insert.
    ///\param n        The number of characters to insert.
    //*********************************************************************
    etl::ibasic_string<T>& insert(size_type position, const_pointer s, size_type n)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));

      insert(begin() + position, s, s + n);
      return *this;
    }

    //*********************************************************************
    /// Insert n characters of c at position.
    ///\param position The position to insert before.
    ///\param n        The number of characters to insert.
    ///\param c        The character to insert.
    //*********************************************************************
    etl::ibasic_string<T>& insert(size_type position, size_type n, value_type c)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));

      insert(begin() + position, n, c);
      return *this;
    }

    //*********************************************************************
    /// Erases a sequence.
    ///\param position Position to start from.
    ///\param length   Number of characters.
    ///\return A reference to this string.
    //*********************************************************************
    etl::ibasic_string<T>& erase(size_type position, size_type length_ = npos)
    {
      // Limit the length.
      length_ = etl::min(length_, size() - position);

      erase(begin() + position, begin() + position + length_);

      return *this;
    }

    //*********************************************************************
    /// Erases an element.
    ///\param i_element Iterator to the element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(iterator i_element)
    {
      etl::mem_move(i_element + 1, end(), i_element);
      p_buffer[--current_size] = 0;

      return i_element;
    }

    //*********************************************************************
    /// Erases an element.
    ///\param i_element Iterator to the element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(const_iterator i_element)
    {
      iterator i_element_(to_iterator(i_element));

      etl::mem_move(i_element + 1, end(), i_element_);
      p_buffer[--current_size] = 0;

      return i_element_;
    }

    //*********************************************************************
    /// Erases a range of elements.
    /// The range includes all the elements between first and last, including the
    /// element pointed by first, but not the one pointed by last.
    ///\param first Iterator to the first element.
    ///\param last  Iterator to the last element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(const_iterator first, const_iterator last)
    {
      iterator first_ = to_iterator(first);
      iterator last_  = to_iterator(last);

      if (first_ == last_)
      {
        return first_;
      }

      etl::mem_move(last_, end(), first_);
      size_type n_delete = etl::distance(first_, last_);

      current_size -= n_delete;
      p_buffer[current_size] = 0;
      cleanup();

      return first_;
    }

    //*********************************************************************
    /// Return a pointer to a C string.
    //*********************************************************************
    const_pointer c_str() const
    {
      return p_buffer;
    }

    //*********************************************************************
    /// Copies a portion of a string.
    ///\param dest  Pointer to the destination buffer.
    ///\param count The number of characters to copy.
    ///\param pos   The position to start copying from.
    //*********************************************************************
    size_type copy(pointer dest, size_type count, size_type pos = 0) const
    {
      if (pos < size())
      {
        if (count != npos)
        {
          count = etl::min(count, size() - pos);
        }
        else
        {
          count = size() - pos;
        }

        etl::mem_move(p_buffer + pos, count, dest);

        return count;
      }
      else
      {
        return 0U;
      }
    }

    //*********************************************************************
    /// Find content within the string
    ///\param str The content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_type find(const ibasic_string<T>& str, size_type pos = 0) const
    {
      return find_impl(str.begin(), str.end(), str.size(), pos);
    }

    //*********************************************************************
    /// Find content within the string
    ///\param view The content to find
    ///\param pos  The position to start searching from.
    //*********************************************************************
    template <typename TOtherTraits>
    size_type find(const etl::basic_string_view<T, TOtherTraits>& view, size_type pos = 0) const
    {
      return find_impl(view.begin(), view.end(), view.size(), pos);
    }

    //*********************************************************************
    /// Find content within the string
    ///\param s   Pointer to the content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_type find(const_pointer s, size_type pos = 0) const
    {
      size_t sz = etl::strlen(s);

      return find_impl(s, s + sz, sz, pos);
    }

    //*********************************************************************
    /// Find content within the string
    ///\param s   Pointer to the content to find
    ///\param pos The position to start searching from.
    ///\param n   The number of characters to search for.
    //*********************************************************************
    size_type find(const_pointer s, size_type pos, size_type n) const
    {
      size_t sz = etl::strlen(s);

      return find_impl(s, s + n, sz, pos);
    }

    //*********************************************************************
    /// Find character within the string
    ///\param c        The character to find.
    ///\param position The position to start searching from.
    //*********************************************************************
    size_type find(T c, size_type position = 0) const
    {
      const_iterator i = etl::find(begin() + position, end(), c);

      if (i != end())
      {
        return etl::distance(begin(), i);
      }
      else
      {
        return npos;
      }
    }

    //*********************************************************************
    /// Find content within the string
    ///\param str The content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_type rfind(const ibasic_string<T>& str, size_type position = npos) const
    {
      return rfind_impl(str.rbegin(), str.rend(), str.size(), position);
    }

    //*********************************************************************
    /// Find content within the string
    ///\param view The content to find
    ///\param pos  The position to start searching from.
    //*********************************************************************
    template <typename TOtherTraits>
    size_type rfind(const etl::basic_string_view<T, TOtherTraits>& view, size_type pos = 0) const
    {
      return rfind_impl(view.rbegin(), view.rend(), view.size(), pos);
    }

    //*********************************************************************
    /// Find content within the string
    ///\param str The content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_type rfind(const_pointer s, size_type position = npos) const
    {
      size_type len = etl::strlen(s);

      const_reverse_iterator srbegin(s + len);
      const_reverse_iterator srend(s);

      return rfind_impl(srbegin, srend, len, position);
    }

    //*********************************************************************
    /// Find content within the string
    ///\param str The content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_type rfind(const_pointer s, size_type position, size_type length_) const
    {
      const_reverse_iterator srbegin(s + length_);
      const_reverse_iterator srend(s);

      return rfind_impl(srbegin, srend, length_, position);
    }

    //*********************************************************************
    /// Find character within the string
    ///\param c   The character to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_type rfind(T c, size_type position = npos) const
    {
      if (position >= size())
      {
        position = size();
      }

      position = size() - position;

      const_reverse_iterator i = etl::find(rbegin() + position, rend(), c);

      if (i != rend())
      {
        return size() - etl::distance(rbegin(), i) - 1;
      }
      else
      {
        return npos;
      }
    }

    //*********************************************************************
    /// Checks that the string is within this string
    //*********************************************************************
    bool contains(const etl::ibasic_string<T>& str) const 
    {
      return find(str) != npos;
    }

    //*********************************************************************
    /// Checks that the view is within this string
    //*********************************************************************
    template <typename TOtherTraits>
    bool contains(const etl::basic_string_view<T, TOtherTraits>& view) const 
    {
      return find(view) != npos;
    }

    //*********************************************************************
    /// Checks that text is within this string
    //*********************************************************************
    bool contains(const_pointer s) const 
    {
      return find(s) != npos;
    }

    //*********************************************************************
    /// Checks that character is within this string
    //*********************************************************************
    bool contains(value_type c) const 
    {
      return find(c) != npos;
    }

    //*********************************************************************
    /// Checks that the string is the start of this string
    //*********************************************************************
    bool starts_with(const etl::ibasic_string<T>& str) const 
    {
      return compare(0, str.size(), str) == 0;
    }

    //*********************************************************************
    /// Checks that the view is the start of this string
    //*********************************************************************
    template <typename TOtherTraits>
    bool starts_with(const etl::basic_string_view<T, TOtherTraits>& view) const 
    {
      return compare(0, view.size(), view) == 0;
    }

    //*********************************************************************
    /// Checks that the string is the start of this string
    //*********************************************************************
    bool starts_with(const_pointer s) const 
    {
      size_t len = etl::strlen(s);

      return compare(0, len, s, len) == 0;
    }

    //*********************************************************************
    /// Checks that the character is the start of this string
    //*********************************************************************
    bool starts_with(value_type c) const 
    {
      return !empty() && (front() == c);
    }

    //*********************************************************************
    /// Checks that the string is the end of this string
    //*********************************************************************
    bool ends_with(const etl::ibasic_string<T>& str) const 
    {
      if (str.size() > size()) 
      {
        return false;
      }

      return compare(size() - str.size(), str.size(), str) == 0;
    }

    //*********************************************************************
    /// Checks that the view is the end of this string
    //*********************************************************************
    template <typename TOtherTraits>
    bool ends_with(const etl::basic_string_view<T, TOtherTraits>& view) const 
    {
      if (view.size() > size()) 
      {
        return false;
      }

      return compare(size() - view.size(), view.size(), view) == 0;
    }

    //*********************************************************************
    /// Checks that the string is the end of this string
    //*********************************************************************
    bool ends_with(const_pointer s) const 
    {
      size_t len = etl::strlen(s);

      if (len > size()) 
      {
        return false;
      }

      return compare(size() - len, len, s, len) == 0;
    }

    //*********************************************************************
    /// Checks that the character is the end of this string
    //*********************************************************************
    bool ends_with(value_type c) const 
    {
      return !empty() && (back() == c);
    }

    //*********************************************************************
    /// Replace 'length' characters from 'position' with 'str'.
    ///\param position The position to start from.
    ///\param length   The number of characters to replace.
    ///\param str      The string to replace it with.
    //*********************************************************************
    ibasic_string& replace(size_type position, size_type length_, const ibasic_string& str)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));

      // Limit the length.
      length_ = etl::min(length_, size() - position);

      // Erase the bit we want to replace.
      erase(position, length_);

      // Insert the new stuff.
      insert(position, str);

      return *this;
    }

    //*********************************************************************
    /// Replace 'length' characters from 'position' with 'view'.
    ///\param position The position to start from.
    ///\param length   The number of characters to replace.
    ///\param view     The string to replace it with.
    //*********************************************************************
    template <typename TOtherTraits>
    ibasic_string& replace(size_type position, size_type length_, const etl::basic_string_view<T, TOtherTraits>& view)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));

      // Limit the length.
      length_ = etl::min(length_, size() - position);

      // Erase the bit we want to replace.
      erase(position, length_);

      // Insert the new stuff.
      insert(position, view);

      return *this;
    }

    //*********************************************************************
    /// Replace characters from 'first' to one before 'last' with 'str'.
    ///\param first The position to start from.
    ///\param last  The one after the position to end at.
    ///\param str   The string to replace it with.
    //*********************************************************************
    ibasic_string& replace(const_iterator first, const_iterator last, const ibasic_string& str)
    {
      // Quick hack, as iterators are pointers.
      iterator first_ = to_iterator(first);
      iterator last_  = to_iterator(last);

      // Erase the bit we want to replace.
      erase(first_, last_);

      // Insert the new stuff.
      insert(first_, str.begin(), str.end());

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      if (str.is_truncated())
      {
        set_truncated(true);

#if ETL_HAS_ERROR_ON_STRING_TRUNCATION
        ETL_ASSERT_FAIL(ETL_ERROR(string_truncation));
#endif
      }
#endif

      return *this;
    }

    //*********************************************************************
    /// Replace characters from 'first' to one before 'last' with 'view'.
    ///\param first The position to start from.
    ///\param last  The one after the position to end at.
    ///\param view  The string view to replace it with.
    //*********************************************************************
    template <typename TOtherTraits>
    ibasic_string& replace(const_iterator first, const_iterator last, const etl::basic_string_view<T, TOtherTraits>& view)
    {
      // Quick hack, as iterators are pointers.
      iterator first_ = to_iterator(first);
      iterator last_  = to_iterator(last);

      // Erase the bit we want to replace.
      erase(first_, last_);

      // Insert the new stuff.
      insert(first_, view.begin(), view.end());

      return *this;
    }

    //*********************************************************************
    /// Replace characters from 'position' of 'length' with 'str' from 'subposition' of 'sublength'.
    //*********************************************************************
    ibasic_string& replace(size_type position, size_type length_, const ibasic_string& str, size_type subposition, size_type sublength)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));
      ETL_ASSERT(subposition <= str.size(), ETL_ERROR(string_out_of_bounds));

      // Limit the lengths.
      length_   = etl::min(length_, size() - position);
      sublength = etl::min(sublength, str.size() - subposition);

      // Erase the bit we want to replace.
      erase(position, length_);

      // Insert the new stuff.
      insert(position, str, subposition, sublength);

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      if (str.is_truncated())
      {
        set_truncated(true);

#if ETL_HAS_ERROR_ON_STRING_TRUNCATION
        ETL_ASSERT_FAIL(ETL_ERROR(string_truncation));
#endif
      }
#endif

      return *this;
    }

    //*********************************************************************
    /// Replace characters from 'position' of 'length' with 'view' from 'subposition' of 'sublength'.
    //*********************************************************************
    template <typename TOtherTraits>
    ibasic_string& replace(size_type position, size_type length_, const etl::basic_string_view<T, TOtherTraits>& view, size_type subposition, size_type sublength)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));
      ETL_ASSERT(subposition <= view.size(), ETL_ERROR(string_out_of_bounds));

      // Limit the lengths.
      length_   = etl::min(length_, size() - position);
      sublength = etl::min(sublength, view.size() - subposition);

      // Erase the bit we want to replace.
      erase(position, length_);

      // Insert the new stuff.
      insert(position, view, subposition, sublength);

      return *this;
    }

    //*********************************************************************
    /// Replace characters from 'position' of 'length' with pointed to string.
    //*********************************************************************
    ibasic_string& replace(size_type position, size_type length_, const_pointer s)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));

      // Limit the length.
      length_ = etl::min(length_, size() - position);

      // Erase the bit we want to replace.
      erase(position, length_);

      // Insert the new stuff.
      insert(position, s, etl::strlen(s));

      return *this;
    }

    //*********************************************************************
    /// Replace characters from 'first'  'last' with pointed to string.
    //*********************************************************************
    ibasic_string& replace(const_iterator first, const_iterator last, const_pointer s)
    {
      // Quick hack, as iterators are pointers.
      iterator first_ = to_iterator(first);
      iterator last_ = to_iterator(last);

      // Erase the bit we want to replace.
      erase(first_, last_);

      // Insert the new stuff.
      insert(first_, s, s + etl::strlen(s));

      return *this;
    }

    //*********************************************************************
    /// Replace characters from 'position' of 'length' with 'n' characters from pointed to string.
    //*********************************************************************
    ibasic_string& replace(size_type position, size_type length_, const_pointer s, size_type n)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));

      // Limit the length.
      length_ = etl::min(length_, size() - position);

      // Erase the bit we want to replace.
      erase(position, length_);

      // Insert the new stuff.
      insert(position, s, n);

      return *this;
    }

    //*********************************************************************
    /// Replace characters from 'first' to 'last' with 'n' characters from pointed to string.
    //*********************************************************************
    ibasic_string& replace(const_iterator first, const_iterator last, const_pointer s, size_type n)
    {
      // Quick hack, as iterators are pointers.
      iterator first_ = to_iterator(first);
      iterator last_ = to_iterator(last);

      // Erase the bit we want to replace.
      erase(first_, last_);

      // Insert the new stuff.
      insert(first_, s, s + n);

      return *this;
    }

    //*********************************************************************
    /// Replace characters from 'position' of 'length' with 'n' copies of 'c'.
    //*********************************************************************
    ibasic_string& replace(size_type position, size_type length_, size_type n, value_type c)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));

      // Limit the length.
      length_ = etl::min(length_, size() - position);

      // Erase the bit we want to replace.
      erase(position, length_);

      // Insert the new stuff.
      insert(position, n, c);

      return *this;
    }

    //*********************************************************************
    /// Replace characters from 'first' of 'last' with 'n' copies of 'c'.
    //*********************************************************************
    ibasic_string& replace(const_iterator first, const_iterator last, size_type n, value_type c)
    {
      // Quick hack, as iterators are pointers.
      iterator first_ = to_iterator(first);
      iterator last_ = to_iterator(last);

      // Erase the bit we want to replace.
      erase(first_, last_);

      // Insert the new stuff.
      insert(first_, n, c);

      return *this;
    }

    //*********************************************************************
    /// Replace characters from 'first' of 'last' with characters from 'first_replace' to 'last_replace'.
    //*********************************************************************
    template <typename TIterator>
    ibasic_string& replace(const_iterator first, const_iterator last, TIterator first_replace, TIterator last_replace)
    {
      // Quick hack, as iterators are pointers.
      iterator first_ = to_iterator(first);
      iterator last_ = to_iterator(last);

      // Erase the bit we want to replace.
      erase(first_, last_);

      // Insert the new stuff.
      insert(first_, first_replace, last_replace);

      return *this;
    }

    //*************************************************************************
    /// Compare with string.
    //*************************************************************************
    int compare(const ibasic_string& str) const
    {
      return compare(p_buffer,
                     p_buffer + size(),
                     str.p_buffer,
                     str.p_buffer + str.size());
    }

    //*************************************************************************
    /// Compare with etl::basic_string_view.
    //*************************************************************************
    template <typename TOtherTraits>
    int compare(const etl::basic_string_view<T, TOtherTraits>& view) const
    {
      return compare(p_buffer,
                     p_buffer + size(),
                     view.data(),
                     view.data() + view.size());
    }

    //*************************************************************************
    /// Compare position / length with string.
    //*************************************************************************
    int compare(size_type position, size_type length_, const ibasic_string& str) const
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));

      // Limit the length.
      length_ = etl::min(length_, size() - position);

      return compare(p_buffer + position,
                     p_buffer + position + length_,
                     str.p_buffer,
                     str.p_buffer + str.size());
    }

    //*************************************************************************
    /// Compare position / length with etl::basic_string_view.
    //*************************************************************************
    template <typename TOtherTraits>
    int compare(size_type position, size_type length_, const etl::basic_string_view<T, TOtherTraits>& view) const
    {
      return compare(p_buffer + position,
                     p_buffer + position + length_,
                     view.data(),
                     view.data() + view.size());
    }

    //*************************************************************************
    /// Compare position / length with string / subposition / sublength.
    //*************************************************************************
    int compare(size_type position, size_type length_, const ibasic_string& str, size_type subposition, size_type sublength) const
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));
      ETL_ASSERT(subposition <= str.size(), ETL_ERROR(string_out_of_bounds));

      // Limit the lengths.
      length_ = etl::min(length_, size() - position);
      sublength = etl::min(sublength, str.size() - subposition);

      return compare(p_buffer + position,
                     p_buffer + position + length_,
                     str.p_buffer + subposition,
                     str.p_buffer + subposition + sublength);
    }

    //*************************************************************************
    /// Compare position / length with etl::basic_string_view. / subposition / sublength.
    //*************************************************************************
    template <typename TOtherTraits>
    int compare(size_type position, size_type length_, const etl::basic_string_view<T, TOtherTraits>& view, size_type subposition, size_type sublength) const
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));
      ETL_ASSERT(subposition <= view.size(), ETL_ERROR(string_out_of_bounds));

      // Limit the lengths.
      length_   = etl::min(length_, size() - position);
      sublength = etl::min(sublength, view.size() - subposition);

      return compare(p_buffer + position,
                     p_buffer + position + length_,
                     view.data() + subposition,
                     view.data() + subposition + sublength);
    }

    //*************************************************************************
    ///  Compare with C string
    //*************************************************************************
    int compare(const value_type* s) const
    {
      return compare(p_buffer,
                     p_buffer + size(),
                     s,
                     s + etl::strlen(s));
    }

    //*************************************************************************
    /// Compare position / length with C string.
    //*************************************************************************
    int compare(size_type position, size_type length_, const_pointer s) const
    {
      return compare(p_buffer + position,
                     p_buffer + position + length_,
                     s,
                     s + etl::strlen(s));
    }

    //*************************************************************************
    /// Compare position / length with C string / n.
    //*************************************************************************
    int compare(size_type position, size_type length_, const_pointer s, size_type n) const
    {
      return compare(p_buffer + position,
                     p_buffer + position + length_,
                     s,
                     s + n);
    }

    //*********************************************************************
    /// Find first of any of content within the string
    ///\param str The content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_type find_first_of(const ibasic_string<T>& str, size_type position = 0) const
    {
      return find_first_of(str.c_str(), position, str.size());
    }

    //*********************************************************************
    /// Find first of any of content within the string
    ///\param s   Pointer to the content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_type find_first_of(const_pointer s, size_type position = 0) const
    {
      return find_first_of(s, position, etl::strlen(s));
    }

    //*********************************************************************
    /// Find first of any of content within the string
    ///\param view The content to find
    ///\param pos  The position to start searching from.
    //*********************************************************************
    template <typename TOtherTraits>
    size_type find_first_of(const etl::basic_string_view<T, TOtherTraits>& view, size_type position = 0) const
    {
      return find_first_of(view.data(), position, view.size());
    }

    //*********************************************************************
    /// Find first of any of content within the string
    ///\param s   Pointer to the content to find
    ///\param pos The position to start searching from.
    ///\param n   The number of characters to search for.
    //*********************************************************************
    size_type find_first_of(const_pointer s, size_type position, size_type n) const
    {
      if (position < size())
      {
        for (size_type i = position; i < size(); ++i)
        {
          for (size_type j = 0; j < n; ++j)
          {
            if (p_buffer[i] == s[j])
            {
              return i;
            }
          }
        }
      }

      return npos;
    }

    //*********************************************************************
    /// Find first of character within the string
    ///\param c   The character to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_type find_first_of(value_type c, size_type position = 0) const
    {
      if (position < size())
      {
        for (size_type i = position; i < size(); ++i)
        {
          if (p_buffer[i] == c)
          {
            return i;
          }
        }
      }

      return npos;
    }

    //*********************************************************************
    /// Find last of any of content within the string
    ///\param str The content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_type find_last_of(const ibasic_string<T>& str, size_type position = npos) const
    {
      return find_last_of(str.c_str(), position, str.size());
    }

    //*********************************************************************
    /// Find last of any of content within the string
    ///\param s   Pointer to the content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_type find_last_of(const_pointer s, size_type position = npos) const
    {
      return find_last_of(s, position, etl::strlen(s));
    }

    //*********************************************************************
    /// Find last of any of content within the string
    ///\param view The content to find
    ///\param pos  The position to start searching from.
    //*********************************************************************
    template <typename TOtherTraits>
    size_type find_last_of(const etl::basic_string_view<T, TOtherTraits>& view, size_type position = npos) const
    {
      return find_last_of(view.data(), position, view.size());
    }

    //*********************************************************************
    /// Find last of any of content within the string
    ///\param s   Pointer to the content to find
    ///\param pos The position to start searching from.
    ///\param n   The number of characters to search for.
    //*********************************************************************
    size_type find_last_of(const_pointer s, size_type position, size_type n) const
    {
      if (empty())
      {
        return npos;
      }

      position = etl::min(position, size() - 1);

      const_reverse_iterator it = rbegin() + size() - position - 1;

      while (it != rend())
      {
        for (size_type j = 0; j < n; ++j)
        {
          if (p_buffer[position] == s[j])
          {
            return position;
          }
        }

        ++it;
        --position;
      }

      return npos;
    }

    //*********************************************************************
    /// Find last of character within the string
    ///\param c   The character to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_type find_last_of(value_type c, size_type position = npos) const
    {
      if (empty())
      {
        return npos;
      }

      position = etl::min(position, size() - 1);

      const_reverse_iterator it = rbegin() + size() - position - 1;

      while (it != rend())
      {
        if (p_buffer[position] == c)
        {
          return position;
        }

        ++it;
        --position;
      }

      return npos;
    }

    //*********************************************************************
    /// Find first not of any of content within the string
    ///\param str The content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_type find_first_not_of(const ibasic_string<T>& str, size_type position = 0) const
    {
      return find_first_not_of(str.c_str(), position, str.size());
    }

    //*********************************************************************
    /// Find first not of any of content within the string
    ///\param s   Pointer to the content to not find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_type find_first_not_of(const_pointer s, size_type position = 0) const
    {
      return find_first_not_of(s, position, etl::strlen(s));
    }

    //*********************************************************************
    /// Find first not of any of content within the string
    ///\param view The content to find
    ///\param pos  The position to start searching from.
    //*********************************************************************
    template <typename TOtherTraits>
    size_type find_first_not_of(const etl::basic_string_view<T, TOtherTraits>& view, size_type position = 0) const
    {
      return find_first_not_of(view.data(), position, view.size());
    }

    //*********************************************************************
    /// Find first not of any of content within the string
    ///\param s   Pointer to the content to not find
    ///\param pos The position to start searching from.
    ///\param n   The number of characters to search for.
    //*********************************************************************
    size_type find_first_not_of(const_pointer s, size_type position, size_type n) const
    {
      if (position < size())
      {
        for (size_type i = position; i < size(); ++i)
        {
          bool found = false;

          for (size_type j = 0; j < n; ++j)
          {
            if (p_buffer[i] == s[j])
            {
              found = true;
            }
          }

          if (!found)
          {
            return i;
          }
        }
      }

      return npos;
    }

    //*********************************************************************
    /// Find first not of character within the string
    ///\param c   The character to not find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_type find_first_not_of(value_type c, size_type position = 0) const
    {
      if (position < size())
      {
        for (size_type i = position; i < size(); ++i)
        {
          if (*(p_buffer + i) != c)
          {
            return i;
          }
        }
      }

      return npos;
    }

    //*********************************************************************
    /// Find last not of any of content within the string
    ///\param str The content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_type find_last_not_of(const ibasic_string<T>& str, size_type position = npos) const
    {
      return find_last_not_of(str.c_str(), position, str.size());
    }

    //*********************************************************************
    /// Find last not of any of content within the string
    ///\param s   The pointer to the content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_type find_last_not_of(const_pointer s, size_type position = npos) const
    {
      return find_last_not_of(s, position, etl::strlen(s));
    }

    //*********************************************************************
    /// Find last not of any of content within the string
    ///\param view The content to find
    ///\param pos  The position to start searching from.
    //*********************************************************************
    template <typename TOtherTraits>
    size_type find_last_not_of(const etl::basic_string_view<T, TOtherTraits>& view, size_type position = npos) const
    {
      return find_last_not_of(view.data(), position, view.size());
    }

    //*********************************************************************
    /// Find last not of any of content within the string
    ///\param s   The pointer to the content to find
    ///\param pos The position to start searching from.
    ///\param n   The number of characters to use.
    //*********************************************************************
    size_type find_last_not_of(const_pointer s, size_type position, size_type n) const
    {
      if (empty())
      {
        return npos;
      }

      position = etl::min(position, size() - 1);

      const_reverse_iterator it = rbegin() + size() - position - 1;

      while (it != rend())
      {
        bool found = false;

        for (size_type j = 0; j < n; ++j)
        {
          if (p_buffer[position] == s[j])
          {
            found = true;
          }
        }

        if (!found)
        {
          return position;
        }

        ++it;
        --position;
      }

      return npos;
    }

    //*********************************************************************
    //
    //*********************************************************************
    size_type find_last_not_of(value_type c, size_type position = npos) const
    {
      if (empty())
      {
        return npos;
      }

      position = etl::min(position, size() - 1);

      const_reverse_iterator it = rbegin() + size() - position - 1;

      while (it != rend())
      {
        if (p_buffer[position] != c)
        {
          return position;
        }

        ++it;
        --position;
      }

      return npos;
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    ibasic_string& operator = (const ibasic_string& rhs)
    {
      if (&rhs != this)
      {
        assign(rhs);
      }

      return *this;
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    ibasic_string& operator = (const_pointer rhs)
    {
      assign(rhs);

      return *this;
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    template <typename TOtherTraits>
    ibasic_string& operator = (const etl::basic_string_view<T, TOtherTraits>& view)
    {
      assign(view);

      return *this;
    }

    //*************************************************************************
    /// += operator.
    //*************************************************************************
    ibasic_string& operator += (const ibasic_string& rhs)
    {
      append(rhs);

      return *this;
    }

    //*************************************************************************
    /// += operator.
    //*************************************************************************
    template <typename TOtherTraits>
    ibasic_string& operator += (const etl::basic_string_view<T, TOtherTraits>& rhs)
    {
      append(rhs);

      return *this;
    }

    //*************************************************************************
    /// += operator.
    //*************************************************************************
    ibasic_string& operator += (const_pointer rhs)
    {
      append(rhs);

      return *this;
    }

    //*************************************************************************
    /// += operator.
    //*************************************************************************
    ibasic_string& operator += (T rhs)
    {
      append(size_type(1), rhs);

      return *this;
    }

#if ETL_HAS_ISTRING_REPAIR
    //*************************************************************************
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
    virtual void repair() = 0;
#endif

    //*********************************************************************
    /// Clears the free space to string terminator value.
    //*********************************************************************
    void initialize_free_space()
    {
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      set_truncated(false);
#endif
      etl::mem_set(&p_buffer[current_size], &p_buffer[CAPACITY + 1U], char(0));
    }

    //*********************************************************************
    /// Trim the size to the distance to the first null terminator.
    /// If the last buffer position has a non-null value then the truncated
    /// flag will be set.
    //*********************************************************************
    void trim_to_terminator()
    {
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      set_truncated(p_buffer[CAPACITY] != T(0));
#endif

      p_buffer[CAPACITY] = T(0); // Ensure a terminating null.
      current_size = etl::strlen(p_buffer);
    }

  protected:

    //*********************************************************************
    /// Constructor.
    //*********************************************************************
    ibasic_string(T* p_buffer_, size_type MAX_SIZE_)
      : string_base(MAX_SIZE_),
        p_buffer(p_buffer_)
    {
    }

    //*********************************************************************
    /// Initialise the string.
    //*********************************************************************
    void initialise()
    {
      current_size = 0U;
      p_buffer[0] = 0;
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      set_truncated(false);
#endif
    }

    //*************************************************************************
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
    void repair_buffer(T* p_buffer_)
    {
      p_buffer = p_buffer_;
    }

  private:

    //*************************************************************************
    /// Compare helper function
    //*************************************************************************
    static int compare(const_pointer first1, const_pointer last1, 
                       const_pointer first2, const_pointer last2)
    {
      typedef typename etl::make_unsigned<value_type>::type type;

      difference_type length1        = etl::distance(first1, last1);
      difference_type length2        = etl::distance(first2, last2);
      difference_type compare_length = etl::min(length1, length2);

      // First compare the string characters.
      while (compare_length != 0)
      {
        if (static_cast<type>(*first1) < static_cast<type>(*first2))
        {
          // Compared character is lower.
          return -1;
        }
        else if (static_cast<type>(*first1) > static_cast<type>(*first2))
        {
          // Compared character is higher.
          return 1;
        }

        ++first1;
        ++first2;
        --compare_length;
      }

      // Then compare the string lengths.
      if (length1 < length2)
      {
        // First string is shorter.
        return -1;
      }
      
      if (length1 > length2)
      {
        // First string is longer.
        return 1;
      }

      // Strings are the same length.
      return 0;
    }

    //*************************************************************************
    /// Clear the unused trailing portion of the string.
    //*************************************************************************
    void cleanup()
    {
#if ETL_HAS_STRING_CLEAR_AFTER_USE
      if (is_secure())
      {
        etl::memory_clear_range(&p_buffer[current_size], &p_buffer[CAPACITY]);
      }
#endif
    }

    //*************************************************************************
    /// Disable copy construction.
    //*************************************************************************
    ibasic_string(const ibasic_string&);

    //*************************************************************************
    /// Pointer to the string's buffer.
    //*************************************************************************
    T* p_buffer;

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_STRINGS) || defined(ETL_POLYMORPHIC_CONTAINERS) || defined(ETL_ISTRING_REPAIR_ENABLE)
  public:
    virtual
#else
  protected:
#endif
    ~ibasic_string()
    {
#if ETL_HAS_STRING_CLEAR_AFTER_USE
      if (is_secure())
      {
        clear();
      }
#endif
    }

  protected:

    //*************************************************************************
    /// Convert from const_iterator to iterator
    //*************************************************************************
    iterator to_iterator(const_iterator itr) const
    {
      return const_cast<iterator>(itr);
    }

  private:

    //*********************************************************************
    /// Copy characters using pointers.
    /// Returns a pointer to the character after the last copied.
    //*********************************************************************
    template <typename TIterator1, typename TIterator2>
    static
    typename etl::enable_if<etl::is_pointer<TIterator1>::value && etl::is_pointer<TIterator2>::value, TIterator2>::type
      copy_characters(TIterator1 from, size_t n, TIterator2 to)
    {
      etl::mem_move(from, n, to);

      return to + n;
    }

    //*********************************************************************
    /// Copy characters using non-pointers.
    /// Returns an iterator to the character after the last copied.
    //*********************************************************************
    template <typename TIterator1, typename TIterator2>
    static
    typename etl::enable_if<!etl::is_pointer<TIterator1>::value || !etl::is_pointer<TIterator2>::value, TIterator2>::type
      copy_characters(TIterator1 from, size_t n, TIterator2 to)
    {
      size_t count = 0;

      while (count != n) 
      {
        *to++ = *from++;
        ++count;
      }

      return to;
    }

    //*********************************************************************
    /// Common implementation for 'assign' and 'append' for iterators.
    //*********************************************************************
    template <typename TIterator>
    void append_impl(iterator position, TIterator first, TIterator last, bool truncated, bool secure)
    {
      difference_type start      = etl::distance(p_buffer, position);
      difference_type count      = etl::distance(first, last);
      difference_type free_space = etl::distance(position, p_buffer + CAPACITY);

#if ETL_IS_DEBUG_BUILD     
      ETL_ASSERT(count >= 0, ETL_ERROR(string_iterator));
#endif
     
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      set_truncated((count > free_space) || this->is_truncated() || truncated);

#if ETL_HAS_ERROR_ON_STRING_TRUNCATION
      ETL_ASSERT(is_truncated == false, ETL_ERROR(string_truncation));
#endif
#endif

#if ETL_HAS_STRING_CLEAR_AFTER_USE
      if (secure)
      {
        set_secure();
      }
#endif

      // Limit the actual distance to the capacity.
      count = etl::min(count, free_space);
      copy_characters(first, size_t(count), position);
      current_size = size_t(start + count);
      p_buffer[current_size] = 0;

      cleanup();
    }

    //*********************************************************************
    /// Common implementation for 'assign' and 'append' for C string pointer.
    //*********************************************************************
    void append_impl(iterator position, const_pointer src, bool truncated, bool secure)
    {
      if (src == ETL_NULLPTR)
      {
        clear();
        return;
      }

      difference_type start      = etl::distance(p_buffer, position);
      difference_type free_space = etl::distance(position, p_buffer + CAPACITY);

      pointer dst    = position;     
      size_t  length = get_string_length(src);
      size_t  count  = (length < size_t(free_space)) ? length : size_t(free_space);
      etl::mem_move(src, count, dst);

      truncated |= (src[count] != 0);
      current_size = size_t(start) + count;
      p_buffer[current_size] = 0;

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      set_truncated(truncated);
#if ETL_HAS_ERROR_ON_STRING_TRUNCATION
      ETL_ASSERT(is_truncated == false, ETL_ERROR(string_truncation));
#endif
#endif

#if ETL_HAS_STRING_CLEAR_AFTER_USE
      if (secure)
      {
        set_secure();
      }
#endif

      cleanup();
    }

    //*************************************************************************
    /// Common implementation for 'find'.
    //*************************************************************************
    template <typename TIterator>
    size_type find_impl(TIterator first, TIterator last, size_type sz, size_type pos = 0) const
    {
      if ((pos + sz) > size())
      {
        return npos;
      }

      const_iterator iposition = etl::search(begin() + pos, end(), first, last);

      if (iposition == end())
      {
        return npos;
      }
      else
      {
        return etl::distance(begin(), iposition);
      }
    }

    //*************************************************************************
    /// Common implementation for 'rfind'.
    //*************************************************************************
    template <typename TIterator>
    size_type rfind_impl(TIterator rfirst, TIterator rlast, size_type sz, size_type pos = 0) const
    {
      if (sz > size())
      {
        return npos;
      }

      if (pos >= size())
      {
        pos = size();
      }

      pos = size() - pos;

      const_reverse_iterator iposition = etl::search(rbegin() + pos, rend(), rfirst, rlast);

      if (iposition == rend())
      {
        return npos;
      }
      else
      {
        return size() - sz - etl::distance(rbegin(), iposition);
      }
    }

    //*********************************************************************
    /// get_string_length, optimised for sizeof(U) == sizeof(char).
    //*********************************************************************
    template <typename U>
    static
    typename etl::enable_if<sizeof(U) == sizeof(char), size_t>::type
      get_string_length(const U* str)
    {
      return ::strlen(reinterpret_cast<const char*>(str));
    }

#if ETL_USING_LIBC_WCHAR_H
    //*********************************************************************
    /// get_string_length, optimised for sizeof(U) == sizeof(wchar_t).
    //*********************************************************************
    template <typename U>
    static
    typename etl::enable_if<sizeof(U) == sizeof(wchar_t), size_t>::type
      get_string_length(const U* str)
    {
      return ::wcslen(reinterpret_cast<const wchar_t*>(str));
    }
#endif

    //*********************************************************************
    /// get_string_length, optimised for anything else.
    //*********************************************************************
    template <typename U>
    static
#if ETL_USING_LIBC_WCHAR_H
    typename etl::enable_if<(sizeof(U) != sizeof(char)) && (sizeof(U) != sizeof(wchar_t)), size_t>::type
#else
    typename etl::enable_if<(sizeof(U) != sizeof(char)), size_t>::type
#endif
      get_string_length(const U* str)
    {
      if (str == ETL_NULLPTR)
      {
        return 0;
      }

      const U* end = str;

      while (*end++ != 0)
      {
        // Do nothing.
      }

      return size_t(end - str) - 1;
    }
  };

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first string.
  ///\param rhs Reference to the second string.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  bool operator ==(const etl::ibasic_string<T>& lhs, const etl::ibasic_string<T>& rhs)
  {
    return (lhs.size() == rhs.size()) && etl::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first string.
  ///\param rhs Reference to the second string.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  bool operator ==(const etl::ibasic_string<T>& lhs, const T* rhs)
  {
    return (lhs.size() == etl::strlen(rhs)) && etl::equal(lhs.begin(), lhs.end(), rhs);
  }

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first string.
  ///\param rhs Reference to the second string.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  bool operator ==(const T* lhs, const etl::ibasic_string<T>& rhs)
  {
    return (rhs.size() == etl::strlen(lhs)) && etl::equal(rhs.begin(), rhs.end(), lhs);
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first string.
  ///\param rhs Reference to the second string.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  bool operator !=(const etl::ibasic_string<T>& lhs, const etl::ibasic_string<T>& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first string.
  ///\param rhs Reference to the second string.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  bool operator !=(const etl::ibasic_string<T>& lhs, const T* rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first string.
  ///\param rhs Reference to the second string.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  bool operator !=(const T* lhs, const etl::ibasic_string<T>& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Less than operator.
  ///\param lhs Reference to the first string.
  ///\param rhs Reference to the second string.
  ///\return <b>true</b> if the first string is lexicographically less than the second, otherwise <b>false</b>
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  bool operator <(const etl::ibasic_string<T>& lhs, const etl::ibasic_string<T>& rhs)
  {
    return etl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }

  //***************************************************************************
  /// Less than operator.
  ///\param lhs Reference to the first string.
  ///\param rhs Reference to the second string.
  ///\return <b>true</b> if the first string is lexicographically less than the second, otherwise <b>false</b>
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  bool operator <(const etl::ibasic_string<T>& lhs, const T* rhs)
  {
    return etl::lexicographical_compare(lhs.begin(), lhs.end(), rhs, rhs + etl::strlen(rhs));
  }

  //***************************************************************************
  /// Less than operator.
  ///\param lhs Reference to the first string.
  ///\param rhs Reference to the second string.
  ///\return <b>true</b> if the first string is lexicographically less than the second, otherwise <b>false</b>
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  bool operator <(const T* lhs, const etl::ibasic_string<T>& rhs)
  {
    return etl::lexicographical_compare(lhs, lhs + etl::strlen(lhs), rhs.begin(), rhs.end());
  }


  //***************************************************************************
  /// Greater than operator.
  ///\param lhs Reference to the first string.
  ///\param rhs Reference to the second string.
  ///\return <b>true</b> if the first string is lexicographically greater than the second, otherwise <b>false</b>
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  bool operator >(const etl::ibasic_string<T>& lhs, const etl::ibasic_string<T>& rhs)
  {
    return (rhs < lhs);
  }

  //***************************************************************************
  /// Greater than operator.
  ///\param lhs Reference to the first string.
  ///\param rhs Reference to the second string.
  ///\return <b>true</b> if the first string is lexicographically greater than the second, otherwise <b>false</b>
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  bool operator >(const etl::ibasic_string<T>& lhs, const T* rhs)
  {
    return (rhs < lhs);
  }

  //***************************************************************************
  /// Greater than operator.
  ///\param lhs Reference to the first string.
  ///\param rhs Reference to the second string.
  ///\return <b>true</b> if the first string is lexicographically greater than the second, otherwise <b>false</b>
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  bool operator >(const T* lhs, const etl::ibasic_string<T>& rhs)
  {
    return (rhs < lhs);
  }


  //***************************************************************************
  /// Less than or equal operator.
  ///\param lhs Reference to the first string.
  ///\param rhs Reference to the second string.
  ///\return <b>true</b> if the first string is lexicographically less than or equal to the second, otherwise <b>false</b>
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  bool operator <=(const etl::ibasic_string<T>& lhs, const etl::ibasic_string<T>& rhs)
  {
    return !(lhs > rhs);
  }

  //***************************************************************************
  /// Less than or equal operator.
  ///\param lhs Reference to the first string.
  ///\param rhs Reference to the second string.
  ///\return <b>true</b> if the first string is lexicographically less than or equal to the second, otherwise <b>false</b>
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  bool operator <=(const etl::ibasic_string<T>& lhs, const T* rhs)
  {
    return !(lhs > rhs);
  }

  //***************************************************************************
  /// Less than or equal operator.
  ///\param lhs Reference to the first string.
  ///\param rhs Reference to the second string.
  ///\return <b>true</b> if the first string is lexicographically less than or equal to the second, otherwise <b>false</b>
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  bool operator <=(const T* lhs, const etl::ibasic_string<T>& rhs)
  {
    return !(lhs > rhs);
  }


  //***************************************************************************
  /// Greater than or equal operator.
  ///\param lhs Reference to the first string.
  ///\param rhs Reference to the second string.
  ///\return <b>true</b> if the first string is lexicographically greater than or equal to the second, otherwise <b>false</b>
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  bool operator >=(const etl::ibasic_string<T>& lhs, const etl::ibasic_string<T>& rhs)
  {
    return !(lhs < rhs);
  }

  //***************************************************************************
  /// Greater than or equal operator.
  ///\param lhs Reference to the first string.
  ///\param rhs Reference to the second string.
  ///\return <b>true</b> if the first string is lexicographically greater than or equal to the second, otherwise <b>false</b>
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  bool operator >=(const etl::ibasic_string<T>& lhs, const T* rhs)
  {
    return !(lhs < rhs);
  }

  //***************************************************************************
  /// Greater than or equal operator.
  ///\param lhs Reference to the first string.
  ///\param rhs Reference to the second string.
  ///\return <b>true</b> if the first string is lexicographically greater than or equal to the second, otherwise <b>false</b>
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  bool operator >=(const T* lhs, const etl::ibasic_string<T>& rhs)
  {
    return !(lhs < rhs);
  }

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
                                                          const etl::ibasic_string<T>& str)
  {
    os.write(str.data(), str.size());
    return os;
  }
#endif
}

#undef ETL_USING_WCHAR_T_H

#include "private/minmax_pop.h"

#endif
