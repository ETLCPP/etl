///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2016 jwellbelove

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

#ifndef __ETL_ISTRING__
#define __ETL_ISTRING__
#define __ETL_IN_ISTRING_H__

#include <iterator>
#include <algorithm>
#include <functional>
#include <stddef.h>
#include <string.h>

#include "private/string_base.h"
#include "platform.h"
#include "algorithm.h"
#include "type_traits.h"
#include "error_handler.h"
#include "algorithm.h"
#include "char_traits.h"

#ifdef ETL_COMPILER_GCC
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#ifdef ETL_COMPILER_MICROSOFT
  #undef min
#endif

namespace etl
{
  //***************************************************************************
  /// Alternative strlen for all character types.
  //***************************************************************************
  template <typename T>
  size_t strlen(const T* t)
  {
    return etl::char_traits<T>::length(t);
  }

  //***************************************************************************
  /// The base class for specifically sized strings.
  /// Can be used as a reference type for all strings containing a specific type.
  ///\ingroup string
  //***************************************************************************
  template <typename T>
  class ibasic_string : public string_base
  {
  public:

    typedef T                                     value_type;
    typedef T&                                    reference;
    typedef const T&                              const_reference;
    typedef T*                                    pointer;
    typedef const T*                              const_pointer;
    typedef T*                                    iterator;
    typedef const T*                              const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef size_t                                size_type;

    typedef typename std::iterator_traits<iterator>::difference_type difference_type;

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
    void resize(size_t new_size)
    {
      resize(new_size, 0);
    }

    //*********************************************************************
    /// Resizes the string.
    ///\param new_size The new size.
    ///\param value    The value to fill new elements with. Default = default constructed value.
    //*********************************************************************
    void resize(size_t new_size, T value)
    {
      new_size = std::min(new_size, MAX_SIZE);

      // Size up?
      if (new_size > current_size)
      {
        std::fill(p_buffer + current_size, p_buffer + new_size, value);
      }

      current_size = new_size;
      p_buffer[new_size] = 0;
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'i'
    ///\param i The index.
    ///\return A reference to the value at index 'i'
    //*********************************************************************
    reference operator [](size_t i)
    {
      return p_buffer[i];
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'i'
    ///\param i The index.
    ///\return A const reference to the value at index 'i'
    //*********************************************************************
    const_reference operator [](size_t i) const
    {
      return p_buffer[i];
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'i'
    /// If asserts or exceptions are enabled, emits an etl::string_out_of_bounds if the index is out of range.
    ///\param i The index.
    ///\return A reference to the value at index 'i'
    //*********************************************************************
    reference at(size_t i)
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
    const_reference at(size_t i) const
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
    const_pointer data() const
    {
      return p_buffer;
    }

    //*********************************************************************
    /// Assigns values to the string.
    /// Truncates if the string does not have enough free space.
    ///\param other The other string.
    //*********************************************************************
    void assign(const etl::ibasic_string<T>& other)
    {
      size_t length = std::min(MAX_SIZE, other.size());
      assign(other.begin(), other.begin() + length);
    }

    //*********************************************************************
    /// Assigns values to the string.
    /// Truncates if the string does not have enough free space.
    ///\param other The other string.
    ///\param subposition The position to start from.
    ///\param sublength   The length to copy.
    //*********************************************************************
    void assign(const etl::ibasic_string<T>& other, size_t subposition, size_t sublength)
    {
      if (sublength == npos)
      {
        sublength = other.size() - subposition;
      }

      ETL_ASSERT(subposition <= other.size(), ETL_ERROR(string_out_of_bounds));

      assign(other.begin() + subposition, sublength);
    }

    //*********************************************************************
    /// Assigns values to the string.
    /// Truncates if the string does not have enough free space.
    ///\param other The other string.
    //*********************************************************************
    void assign(const_pointer other)
    {
      initialise();

      while ((*other != 0) && (current_size < MAX_SIZE))
      {
        p_buffer[current_size++] = *other++;
      }

      is_truncated = (*other != 0);

      p_buffer[current_size] = 0;
    }

    //*********************************************************************
    /// Assigns values to the string.
    /// Truncates if the string does not have enough free space.
    ///\param other The other string.
    ///\param length The length to copy.
    //*********************************************************************
    void assign(const_pointer other, size_t length)
    {
      length = std::min(length, MAX_SIZE);

      initialise();

      etl::copy_n(other, length, begin());

      current_size = length;
      p_buffer[current_size] = 0;
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
#if defined(ETL_DEBUG)
      difference_type count = std::distance(first, last);
      ETL_ASSERT(count >= 0, ETL_ERROR(string_iterator));
#endif

      initialise();

      while ((first != last) && (current_size != MAX_SIZE))
      {
        p_buffer[current_size++] = *first++;
      }

      p_buffer[current_size] = 0;
    }

    //*********************************************************************
    /// Assigns values to the string.
    /// Truncates if the string does not have enough free space.
    ///\param n     The number of elements to add.
    ///\param value The value to insert for each element.
    //*********************************************************************
    void assign(size_t n, T value)
    {
      initialise();

      n = std::min(n, MAX_SIZE);

      std::fill_n(begin(), n, value);
      current_size = n;
      p_buffer[current_size] = 0;
    }

    //*************************************************************************
    /// Clears the string.
    //*************************************************************************
    void clear()
    {
      initialise();
    }

    //*********************************************************************
    /// Inserts a value at the end of the string.
    /// Sets 'truncated' if the string is already full.
    ///\param value The value to add.
    //*********************************************************************
    void push_back(T value)
    {
      if (current_size != MAX_SIZE)
      {
        p_buffer[current_size++] = value;
        is_truncated = false;
      }
      else
      {
        is_truncated = true;
      }
    }

    //*************************************************************************
    /// Removes an element from the end of the string.
    /// Does nothing if the string is empty.
    //*************************************************************************
    void pop_back()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!empty(), ETL_ERROR(string_empty));
#endif

        p_buffer[--current_size] = 0;
    }

    //*********************************************************************
    /// Appends to the string.
    ///\param str The string to append.
    //*********************************************************************
    ibasic_string& append(const ibasic_string& str)
    {
      insert(end(), str.begin(), str.end());
      return *this;
    }

    //*********************************************************************
    /// Appends to the string.
    ///\param str The string to append.
    ///\param subposition The position in str.
    ///\param sublength The number of characters.
    //*********************************************************************
    ibasic_string& append(const ibasic_string& str, size_t subposition, size_t sublength = npos)
    {
      ETL_ASSERT(subposition <= str.size(), ETL_ERROR(string_out_of_bounds));

      insert(size(), str, subposition, sublength);
      return *this;
    }

    //*********************************************************************
    /// Appends to the string.
    ///\param str The string to append.
    //*********************************************************************
    ibasic_string& append(const T* str)
    {
      insert(size(), str);
      return *this;
    }

    //*********************************************************************
    /// Appends to the string.
    ///\param str The string to append.
    ///\param n   The number of characters.
    //*********************************************************************
    ibasic_string& append(const T* str, size_t n)
    {
      insert(size(), str, n);
      return *this;
    }

    //*********************************************************************
    /// Appends to the string.
    ///\param n The number of characters.
    ///\param c The character.
    //*********************************************************************
    ibasic_string& append(size_t n, T c)
    {
      insert(size(), n, c);
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
      insert(end(), first, last);
      return *this;
    }

    //*********************************************************************
    /// Inserts a value to the string.
    ///\param position The position to insert before.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator position, T value)
    {
      is_truncated = false;

      // Quick hack, as iterators are pointers.
      iterator insert_position = const_cast<iterator>(position);

      if (current_size < MAX_SIZE)
      {
        // Not full yet.
        if (position != end())
        {
          // Insert in the middle.
          ++current_size;
          std::copy_backward(insert_position, end() - 1, end());
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
          std::copy_backward(insert_position, end() - 1, end());
          *insert_position = value;
        }

        is_truncated = true;
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
    void insert(const_iterator position, size_t n, T value)
    {
      is_truncated = false;

      if (n == 0)
      {
        return;
      }

      // Quick hack, as iterators are pointers.
      iterator insert_position = const_cast<iterator>(position);
      const size_t start = std::distance(cbegin(), position);
      
      // No effect.
      if (start == MAX_SIZE)
      {
        return;
      }

      // Fills the string to the end?
      if ((start + n) >= MAX_SIZE)
      {
        is_truncated = ((current_size + n) > MAX_SIZE);
        current_size = MAX_SIZE;
        std::fill(insert_position, end(), value);
      }
      else
      {
        // Lets do some shifting.
        const size_t shift_amount         = n;
        const size_t to_position          = start + shift_amount;
        const size_t remaining_characters = current_size - start;
        const size_t max_shift_characters = MAX_SIZE - start - shift_amount;
        const size_t characters_to_shift  = std::min(max_shift_characters, remaining_characters);
        
        // Will the string truncate?
        if ((start + shift_amount + remaining_characters) > MAX_SIZE)
        {
          current_size = MAX_SIZE;
          is_truncated = true;
        }
        else
        {
          current_size += shift_amount;
        }

        std::copy_backward(insert_position, insert_position + characters_to_shift, begin() + to_position + characters_to_shift);
        std::fill(insert_position, insert_position + shift_amount, value);
      }

      p_buffer[current_size] = 0;
    }

    //*********************************************************************
    /// Inserts a range of values to the string.
    /// If asserts or exceptions are enabled, emits string_full if the string does not have enough free space.
    ///\param position The position to insert before.
    ///\param first    The first element to add.
    ///\param last     The last + 1 element to add.
    //*********************************************************************
    template <class TIterator>
    void insert(iterator position, TIterator first, TIterator last)
    {
      is_truncated = false;

      if (first == last)
      {
        return;
      }

      const size_t start = std::distance(begin(), position);
      const size_t n     = std::distance(first, last);

      // No effect.
      if (start == MAX_SIZE)
      {
        return;
      }

      // Fills the string to the end?
      if ((start + n) >= MAX_SIZE)
      {
        is_truncated = ((current_size + n) > MAX_SIZE);
        current_size = MAX_SIZE;

        while (position != end())
        {
          *position++ = *first++;
        }
      }
      else
      {
        // Lets do some shifting.
        const size_t shift_amount         = n;
        const size_t to_position          = start + shift_amount;
        const size_t remaining_characters = current_size - start;
        const size_t max_shift_characters = MAX_SIZE - start - shift_amount;
        const size_t characters_to_shift  = std::min(max_shift_characters, remaining_characters);

        // Will the string truncate?
        if ((start + shift_amount + remaining_characters) > MAX_SIZE)
        {
          current_size = MAX_SIZE;
          is_truncated = true;
        }
        else
        {
          current_size += shift_amount;
        }

        std::copy_backward(position, position + characters_to_shift, begin() + to_position + characters_to_shift);

        while (first != last)
        {
          *position++ = *first++;
        }
      }

      p_buffer[current_size] = 0;
    }

    //*********************************************************************
    /// Inserts a string at the specified position.
    ///\param position The position to insert before.
    ///\param str      The string to insert.
    //*********************************************************************
    etl::ibasic_string<T>& insert(size_t position, const etl::ibasic_string<T>& str)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));
      
      insert(begin() + position, str.cbegin(), str.cend());
      return *this;
    }

    //*********************************************************************
    /// Inserts a string at the specified position from subposition for sublength.
    ///\param position    The position to insert before.
    ///\param str         The string to insert.
    ///\param subposition The subposition to start from.
    ///\param sublength   The number of characters to insert.
    //*********************************************************************
    etl::ibasic_string<T>& insert(size_t position, const etl::ibasic_string<T>& str, size_t subposition, size_t sublength)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));
      ETL_ASSERT(subposition <= str.size(), ETL_ERROR(string_out_of_bounds));

      if ((sublength == npos) || (subposition + sublength > str.size()))
      {
        sublength = str.size() - subposition;
      }

      insert(begin() + position, str.cbegin() + subposition, str.cbegin() + subposition + sublength);
      return *this;
    }

    //*********************************************************************
    /// Inserts a string at the specified position from pointer.
    ///\param position The position to insert before.
    ///\param s        The string to insert.
    //*********************************************************************
    etl::ibasic_string<T>& insert(size_t position, const_pointer s)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));

      insert(begin() + position, s, s + strlen(s));
      return *this;
    }

    //*********************************************************************
    /// Inserts a string at the specified position from pointer for n characters.
    ///\param position The position to insert before.
    ///\param s        The string to insert.
    ///\param n        The number of characters to insert.
    //*********************************************************************
    etl::ibasic_string<T>& insert(size_t position, const_pointer s, size_t n)
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
    etl::ibasic_string<T>& insert(size_t position, size_t n, value_type c)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));

      insert(begin() + position, n, c);
      return *this;
    }

    //*********************************************************************
    /// Erases a sequence.
    ///\param position Position to start from.
    ///\param length   Number of characters.
    ///\return A refernce to this string.
    //*********************************************************************
    etl::ibasic_string<T>& erase(size_t position, size_t length = npos)
    {
      // Limit the length.
      length = std::min(length, size() - position);

      erase(begin() + position, begin() + position + length);

      return *this;
    }

    //*********************************************************************
    /// Erases an element.
    ///\param i_element Iterator to the element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(iterator i_element)
    {
      std::copy(i_element + 1, end(), i_element);
      p_buffer[--current_size] = 0;
      
      return i_element;
    }

    //*********************************************************************
    /// Erases a range of elements.
    /// The range includes all the elements between first and last, including the
    /// element pointed by first, but not the one pointed by last.
    ///\param first Iterator to the first element.
    ///\param last  Iterator to the last element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(iterator first, iterator last)
    {
      std::copy(last, end(), first);
      size_t n_delete = std::distance(first, last);

      current_size -= n_delete;
      p_buffer[current_size] = 0;

      return first;
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
    ///\param s   Pointer to the string to copy.
    ///\param len The number of characters to copy.
    ///\param pos The position to start copying from.
    //*********************************************************************
    size_t copy(pointer s, size_t len, size_t pos = 0)
    {
      size_t endpos = std::min(pos + len, size());

      for (size_t i = pos; i < endpos; ++i)
      {
        *s++ = p_buffer[i];
      }

      return endpos - pos;
    }

    //*********************************************************************
    /// Find content within the string
    ///\param str The content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_t find(const ibasic_string<T>& str, size_t pos = 0) const
    {
      if ((pos + str.size()) > size())
      {
        return npos;
      }

      const_iterator iposition = std::search(begin() + pos, end(), str.begin(), str.end());

      if (iposition == end())
      {
        return npos;
      }
      else
      {
        return std::distance(begin(), iposition);
      }
    }

    //*********************************************************************
    /// Find content within the string
    ///\param s   Pointer to the content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_t find(const_pointer s, size_t pos = 0) const
    {
#if defined(ETL_DEBUG)
      if ((pos + etl::strlen(s)) > size())
      {
        return npos;
      }
#endif

      const_iterator iposition = std::search(begin() + pos, end(), s, s + etl::strlen(s));

      if (iposition == end())
      {
        return npos;
      }
      else
      {
        return std::distance(begin(), iposition);
      }
    }

    //*********************************************************************
    /// Find content within the string
    ///\param s   Pointer to the content to find
    ///\param pos The position to start searching from.
    ///\param n   The number of characters to search for.
    //*********************************************************************
    size_t find(const_pointer s, size_t pos, size_t n) const
    {
#if defined(ETL_DEBUG)
      if ((pos + etl::strlen(s) - n) > size())
      {
        return npos;
      }
#endif

      const_iterator iposition = std::search(begin() + pos, end(), s, s + n);

      if (iposition == end())
      {
        return npos;
      }
      else
      {
        return std::distance(begin(), iposition);
      }
    }

    //*********************************************************************
    /// Find character within the string
    ///\param c        The character to find.
    ///\param position The position to start searching from.
    //*********************************************************************
    size_t find(T c, size_t position = 0) const
    {
      const_iterator i = std::find(begin() + position, end(), c);

      if (i != end())
      {
        return std::distance(begin(), i);
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
    size_t rfind(const ibasic_string<T>& str, size_t position = npos) const
    {
      if ((str.size()) > size())
      {
        return npos;
      }

      if (position >= size())
      {
        position = size();
      }

      position = size() - position;

      const_reverse_iterator iposition = std::search(rbegin() + position, rend(), str.rbegin(), str.rend());

      if (iposition == rend())
      {
        return npos;
      }
      else
      {
        return size() - str.size() - std::distance(rbegin(), iposition);
      }
    }

    //*********************************************************************
    /// Find content within the string
    ///\param str The content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_t rfind(const_pointer s, size_t position = npos) const
    {
      size_t length = etl::strlen(s);

      if (length > size())
      {
        return npos;
      }

      if (position >= size())
      {
        position = size();
      }

      position = size() - position;

      const_reverse_iterator srbegin(s + length);
      const_reverse_iterator srend(s);

      const_reverse_iterator iposition = std::search(rbegin() + position, rend(), srbegin, srend);

      if (iposition == rend())
      {
        return npos;
      }
      else
      {
        return size() - length - std::distance(rbegin(), iposition);
      }
    }

    //*********************************************************************
    /// Find content within the string
    ///\param str The content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_t rfind(const_pointer s, size_t position, size_t length) const
    {
      if (length > size())
      {
        return npos;
      }

      if (position >= size())
      {
        position = size();
      }

      position = size() - position;

      const_reverse_iterator srbegin(s + length);
      const_reverse_iterator srend(s);

      const_reverse_iterator iposition = std::search(rbegin() + position, rend(), srbegin, srend);

      if (iposition == rend())
      {
        return npos;
      }
      else
      {
        return size() - length - std::distance(rbegin(), iposition);
      }
    }

    //*********************************************************************
    /// Find character within the string
    ///\param c   The character to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_t rfind(T c, size_t position = npos) const
    {
      if (position >= size())
      {
        position = size();
      }

      position = size() - position;

      const_reverse_iterator i = std::find(rbegin() + position, rend(), c);

      if (i != rend())
      {
        return size() - std::distance(rbegin(), i) - 1;
      }
      else
      {
        return npos;
      }
    }

    //*********************************************************************
    /// Replace 'length' characters from 'position' with 'str'.
    ///\param position The position to start from.
    ///\param length   The number of characters to replace.
    ///\param str      The string to replace it with.
    //*********************************************************************
    ibasic_string& replace(size_t position, size_t length, const ibasic_string& str)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));

      // Limit the length.
      length = std::min(length, size() - position);

      // Erase the bit we want to replace.
      erase(position, length);

      // Insert the new stuff.
      insert(position, str);

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
      iterator first_ = const_cast<iterator>(first);
      iterator last_ = const_cast<iterator>(last);

      // Erase the bit we want to replace.
      erase(first_, last_);

      // Insert the new stuff.
      insert(first_, str.begin(), str.end());

      return *this;
    }

    //*********************************************************************
    /// Replace characters from 'position' of 'length' with 'str' from 'subpsotion' of 'sublength'.
    //*********************************************************************
    ibasic_string& replace(size_t position, size_t length, const ibasic_string& str, size_t subposition, size_t sublength)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));
      ETL_ASSERT(subposition <= str.size(), ETL_ERROR(string_out_of_bounds));

      // Limit the lengths.
      length    = std::min(length,    size() - position);
      sublength = std::min(sublength, str.size() - subposition);

      // Erase the bit we want to replace.
      erase(position, length);

      // Insert the new stuff.
      insert(position, str, subposition, sublength);
      
      return *this;
    }

    //*********************************************************************
    /// Replace characters from 'position' of 'length' with pointed to string.
    //*********************************************************************
    ibasic_string& replace(size_t position, size_t length, const_pointer s)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));

      // Limit the length.
      length = std::min(length, size() - position);

      // Erase the bit we want to replace.
      erase(position, length);

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
      iterator first_ = const_cast<iterator>(first);
      iterator last_  = const_cast<iterator>(last);
      
      // Erase the bit we want to replace.
      erase(first_, last_);
      
      // Insert the new stuff.
      insert(first_, s, s + etl::strlen(s));

      return *this;
    }

    //*********************************************************************
    /// Replace characters from 'position' of 'length' with 'n' characters from pointed to string.
    //*********************************************************************
    ibasic_string& replace(size_t position, size_t length, const_pointer s, size_t n)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));

      // Limit the length.
      length = std::min(length, size() - position);

      // Erase the bit we want to replace.
      erase(position, length);

      // Insert the new stuff.
      insert(position, s, n);

      return *this;
    }

    //*********************************************************************
    /// Replace characters from 'first' to 'last' with 'n' characters from pointed to string.
    //*********************************************************************
    ibasic_string& replace(const_iterator first, const_iterator last, const_pointer s, size_t n)
    {
      // Quick hack, as iterators are pointers.
      iterator first_ = const_cast<iterator>(first);
      iterator last_  = const_cast<iterator>(last);

      // Erase the bit we want to replace.
      erase(first_, last_);

      // Insert the new stuff.
      insert(first_, s, s + n);

      return *this;
    }

    //*********************************************************************
    /// Replace characters from 'position' of 'length' with 'n' copies of 'c'.
    //*********************************************************************
    ibasic_string& replace(size_t position, size_t length, size_t n, value_type c)
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));

      // Limit the length.
      length = std::min(length, size() - position);

      // Erase the bit we want to replace.
      erase(position, length);

      // Insert the new stuff.
      insert(position, n, c);

      return *this;
    }
   
    //*********************************************************************
    /// Replace characters from 'first' of 'last' with 'n' copies of 'c'.
    //*********************************************************************
    ibasic_string& replace(const_iterator first, const_iterator last, size_t n, value_type c)
    {
      // Quick hack, as iterators are pointers.
      iterator first_ = const_cast<iterator>(first);
      iterator last_  = const_cast<iterator>(last);

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
      iterator first_ = const_cast<iterator>(first);
      iterator last_ = const_cast<iterator>(last);

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
    /// Compare position / length with string.
    //*************************************************************************
    int compare(size_t position, size_t length, const ibasic_string& str) const
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));

      // Limit the length.
      length = std::min(length, size() - position);

      return compare(p_buffer + position, 
                     p_buffer + position + length, 
                     str.p_buffer, 
                     str.p_buffer + str.size());
    }

    //*************************************************************************
    /// Compare position / length with string / subposition / sublength.
    //*************************************************************************
    int compare(size_t position, size_t length, const ibasic_string& str, size_t subposition, size_t sublength) const
    {
      ETL_ASSERT(position <= size(), ETL_ERROR(string_out_of_bounds));
      ETL_ASSERT(subposition <= str.size(), ETL_ERROR(string_out_of_bounds));

      // Limit the lengths.
      length    = std::min(length,    size() - position);
      sublength = std::min(sublength, str.size() - subposition);

      return compare(p_buffer + position, 
                     p_buffer + position + length, 
                     str.p_buffer + subposition, 
                     str.p_buffer + subposition + sublength);
    }

    //*************************************************************************
    ///  Compare with C string
    //*************************************************************************
    int compare(const value_type* s) const
    {
      return compare(p_buffer,
                     p_buffer+ size(),
                     s,
                     s + etl::strlen(s));
    }

    //*************************************************************************
    /// Compare position / length with C string.
    //*************************************************************************
    int compare(size_t position, size_t length, const_pointer s) const
    {
      return compare(p_buffer + position,
                     p_buffer + position + length,
                     s,
                     s + etl::strlen(s));
    }

    //*************************************************************************
    /// Compare position / length with C string / n.
    //*************************************************************************
    int compare(size_t position, size_t length, const_pointer s, size_t n) const
    {
      return compare(p_buffer + position,
                     p_buffer + position + length,
                     s,
                     s + n);;
    }

    //*********************************************************************
    /// Find first of any of content within the string
    ///\param str The content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_t find_first_of(const ibasic_string<T>& str, size_t position = 0) const
    {
      return find_first_of(str.c_str(), position, str.size());
    }

    //*********************************************************************
    /// Find first of any of content within the string
    ///\param s   Pointer to the content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_t find_first_of(const_pointer s, size_t position = 0) const
    {
      return find_first_of(s, position, etl::strlen(s));
    }

    //*********************************************************************
    /// Find first of any of content within the string
    ///\param s   Pointer to the content to find
    ///\param pos The position to start searching from.
    ///\param n   The number of characters to search for.
    //*********************************************************************
    size_t find_first_of(const_pointer s, size_t position, size_t n) const
    {
      if (position < size())
      {
        for (size_t i = position; i < size(); ++i)
        {
          for (size_t j = 0; j < n; ++j)
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
    size_t find_first_of(value_type c, size_t position = 0) const
    {
      if (position < size())
      {
        for (size_t i = position; i < size(); ++i)
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
    size_t find_last_of(const ibasic_string<T>& str, size_t position = npos) const
    {
      return find_last_of(str.c_str(), position, str.size());
    }

    //*********************************************************************
    /// Find last of any of content within the string
    ///\param s   Pointer to the content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_t find_last_of(const_pointer s, size_t position = npos) const
    {
      return find_last_of(s, position, etl::strlen(s));
    }

    //*********************************************************************
    /// Find last of any of content within the string
    ///\param s   Pointer to the content to find
    ///\param pos The position to start searching from.
    ///\param n   The number of characters to search for.
    //*********************************************************************
    size_t find_last_of(const_pointer s, size_t position, size_t n) const
    {
      if (empty())
      {
        return npos;
      }

      position = std::min(position, size() - 1);

      const_reverse_iterator it = rbegin() + size() - position - 1;

      while (it != rend())
      {
        for (size_t j = 0; j < n; ++j)
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
    size_t find_last_of(value_type c, size_t position = npos) const
    {
      if (empty())
      {
        return npos;
      }

      position = std::min(position, size() - 1);

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
    size_t find_first_not_of(const ibasic_string<T>& str, size_t position = 0) const
    {
      return find_first_not_of(str.c_str(), position, str.size());
    }

    //*********************************************************************
    /// Find first not of any of content within the string
    ///\param s   Pointer to the content to not find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_t find_first_not_of(const_pointer s, size_t position = 0) const
    {
      return find_first_not_of(s, position, etl::strlen(s));
    }

    //*********************************************************************
    /// Find first not of any of content within the string
    ///\param s   Pointer to the content to not find
    ///\param pos The position to start searching from.
    ///\param n   The number of characters to search for.
    //*********************************************************************
    size_t find_first_not_of(const_pointer s, size_t position, size_t n) const
    {
      if (position < size())
      {
        for (size_t i = position; i < size(); ++i)
        {
          bool found = false;

          for (size_t j = 0; j < n; ++j)
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
    size_t find_first_not_of(value_type c, size_t position = 0) const
    {
      if (position < size())
      {
        for (size_t i = position; i < size(); ++i)
        {
          if (p_buffer[i] != c)
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
    size_t find_last_not_of(const ibasic_string<T>& str, size_t position = npos) const
    {
      return find_last_not_of(str.c_str(), position, str.size());
    }

    //*********************************************************************
    /// Find last not of any of content within the string
    ///\param s   The pointer to the content to find
    ///\param pos The position to start searching from.
    //*********************************************************************
    size_t find_last_not_of(const_pointer s, size_t position = npos) const
    {
      return find_last_not_of(s, position, etl::strlen(s));
    }

    //*********************************************************************
    /// Find last not of any of content within the string
    ///\param s   The pointer to the content to find
    ///\param pos The position to start searching from.
    ///\param n   The number of characters to use.
    //*********************************************************************
    size_t find_last_not_of(const_pointer s, size_t position, size_t n) const
    {
      if (empty())
      {
        return npos;
      }

      position = std::min(position, size() - 1);

      const_reverse_iterator it = rbegin() + size() - position - 1;

      while (it != rend())
      {
        bool found = false;

        for (size_t j = 0; j < n; ++j)
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
    size_t find_last_not_of(value_type c, size_t position = npos) const
    {
      if (empty())
      {
        return npos;
      }

      position = std::min(position, size() - 1);

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
        assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

    //*************************************************************************
    /// += operator.
    //*************************************************************************
    ibasic_string& operator += (const ibasic_string& rhs)
    {
      if (&rhs != this)
      {
        append(rhs);
      }

      return *this;
    }

    //*************************************************************************
    /// += operator.
    //*************************************************************************
    ibasic_string& operator += (const T* rhs)
    {
      append(rhs);

      return *this;
    }

    //*************************************************************************
    /// += operator.
    //*************************************************************************
    ibasic_string& operator += (T rhs)
    {
      append(size_t(1), rhs);

      return *this;
    }

  protected:

    //*********************************************************************
    /// Constructor.
    //*********************************************************************
    ibasic_string(T* p_buffer, size_t MAX_SIZE)
      : string_base(MAX_SIZE),
        p_buffer(p_buffer)
    {
    }

    //*********************************************************************
    /// Initialise the string.
    //*********************************************************************
    void initialise()
    {
      current_size = 0;
      p_buffer[0] = 0;
    }

  private:

    //*************************************************************************
    /// Compare helper function
    //*************************************************************************
    int compare(const_pointer first1, const_pointer last1, const_pointer first2, const_pointer last2) const
    {
      while ((first1 != last1) && (first2 != last2))
      {
        if (*first1 < *first2)
        {
          // Compared character is lower.
          return -1;
        }
        else if (*first1 > *first2)
        {
          // Compared character is higher.
          return 1;
        }

        ++first1;
        ++first2;
      }

      // We reached the end of one or both of the strings.
      if ((first1 == last1) && (first2 == last2))
      {
        // Same length.
        return 0;
      }
      else if (first1 == last1)
      {
        // Compared string is shorter.
        return -1;
      }
      else
      {
        // Compared string is longer.
        return 1;
      }
    }

    // Disable copy construction.
    ibasic_string(const ibasic_string&);

    T* p_buffer;
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
    return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
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
    return (lhs.size() == etl::strlen(rhs)) && std::equal(lhs.begin(), lhs.end(), rhs);
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
    return (rhs.size() == etl::strlen(lhs)) && std::equal(rhs.begin(), rhs.end(), lhs);
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
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
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
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs, rhs + etl::strlen(rhs));
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
    return std::lexicographical_compare(lhs, lhs + etl::strlen(lhs), rhs.begin(), rhs.end());
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
}

#ifdef ETL_COMPILER_MICROSOFT
  #define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#undef __ETL_IN_ISTRING_H__
#endif
