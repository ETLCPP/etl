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

#ifndef ETL_TRIVIAL_VECTOR_INCLUDED
#define ETL_TRIVIAL_VECTOR_INCLUDED

#include "platform.h"
#include "algorithm.h"
#include "iterator.h"
#include "utility.h"
#include "static_assert.h"

//*****************************************************************************
///\defgroup vector trivial_vector
///\ingroup containers
//*****************************************************************************

namespace etl
{
  template <typename T>
  class itrivial_vector
  {
  public:

    ETL_STATIC_ASSERT(etl::is_default_constructible<T>::value, "etl::trivial_vector value type must be default constructible");

    typedef T        value_type;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef T&&      rvalue_reference;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T*       iterator;
    typedef const T* const_iterator;
    typedef size_t   size_type;

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR size_t size() const ETL_NOEXCEPT
    {
      return size_t(pbuffer_end - pbuffer);
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 size_t max_size() const ETL_NOEXCEPT
    {
      return buffer_size;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 size_t capacity() const ETL_NOEXCEPT
    {
      return buffer_size;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR bool full() const ETL_NOEXCEPT
    {
      return this->size() == max_size();
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 bool empty() const ETL_NOEXCEPT
    {
      return pbuffer_end == pbuffer;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 iterator begin() ETL_NOEXCEPT
    {
      return pbuffer;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator begin() const ETL_NOEXCEPT
    {
      return pbuffer;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator cbegin() const ETL_NOEXCEPT
    {
      return pbuffer;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 iterator end() ETL_NOEXCEPT
    {
      return pbuffer_end;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator end() const ETL_NOEXCEPT
    {
      return pbuffer_end;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator cend() const ETL_NOEXCEPT
    {
      return pbuffer_end;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 pointer data() ETL_NOEXCEPT
    {
      return pbuffer;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 const_pointer data() const ETL_NOEXCEPT
    {
      return pbuffer;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 pointer data_end() ETL_NOEXCEPT
    {
      return pbuffer_end;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 const_pointer data_end() const ETL_NOEXCEPT
    {
      return pbuffer_end;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 reference operator [](int i) ETL_NOEXCEPT
    {
      return pbuffer[i];
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 const_reference operator [](int i) const ETL_NOEXCEPT
    {
      return pbuffer[i];
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 reference at(int i) ETL_NOEXCEPT
    {
      return pbuffer[i];
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 const_reference at(int i) const ETL_NOEXCEPT
    {
      return pbuffer[i];
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 reference front() ETL_NOEXCEPT
    {
      return *pbuffer;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 const_reference front() const ETL_NOEXCEPT
    {
      return *pbuffer;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 reference back() ETL_NOEXCEPT
    {
      return *(pbuffer_end - 1);
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 const_reference back() const ETL_NOEXCEPT
    {
      return *(pbuffer_end - 1);
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 void push_back(const_reference value) ETL_NOEXCEPT
    {
      if (!full())
      {
        *pbuffer_end++ = value;
      }
    }

    //*************************************************************************
    ///
    //*************************************************************************
    template <typename... TArgs>
    ETL_CONSTEXPR14 void push_back(rvalue_reference value) ETL_NOEXCEPT
    {
      if (!full())
      {
        *pbuffer_end++ = etl::move(value);
      }
    }

    //*************************************************************************
    ///
    //*************************************************************************
    template <typename... TArgs>
    ETL_CONSTEXPR14 reference emplace_back(TArgs&&... args) ETL_NOEXCEPT
    {
      if (!full())
      {
        *pbuffer_end++ = T(etl::forward<TArgs>(args)...);
      }
    }

    //*************************************************************************
    ///
    //*************************************************************************
    template <typename... TArgs>
    ETL_CONSTEXPR14 reference emplace(const_iterator pos, TArgs&&... args) ETL_NOEXCEPT
    {
      insert(pos, T(etl::forward<TArgs>(args)...));
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 void pop_back() ETL_NOEXCEPT
    {
      if (!empty())
      {
        --pbuffer_end;
      }
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 itrivial_vector<T>& operator =(const itrivial_vector<T>& other) ETL_NOEXCEPT
    {
      assign(other.begin(), other.end());

      return *this;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 itrivial_vector<T>& operator =(itrivial_vector<T>&& other) ETL_NOEXCEPT
    {
      for (size_type i = 0; i < size(); ++i)
      {
        pbuffer[i] = etl::move(other.pbuffer[i]);
      }
    
      return *this;
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 itrivial_vector& operator =(std::initializer_list<value_type> il) ETL_NOEXCEPT
    {
      assign(il.begin(), il.end());
    }
#endif

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 void clear() ETL_NOEXCEPT
    {
      pbuffer_end = pbuffer;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 void resize(size_type count)
    {
      resize(count, T());
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 void resize(size_type count, const value_type& value) ETL_NOEXCEPT
    {
      const size_type current_size = size();

      // Limit the resize.
      count = (count > size()) ? size() : count;

      if (count < current_size)
      {
        // Overwrite the difference with default.      
        for (size_type i = count; i < current_size; ++i)
        {
          pbuffer[i] = value;
        }

        pbuffer_end = pbuffer + count;
      }

      pbuffer_end = pbuffer + count;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 void assign(size_type count, const_reference value) ETL_NOEXCEPT
    {
      clear();

      iterator dst = pbuffer;

      while ((dst != pbuffer_end) && (count != 0))
      {
        *dst++ = value;
        --count;
      }

      pbuffer_end = dst;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    template <typename TIterator>
    ETL_CONSTEXPR14 void assign(TIterator first, TIterator last) ETL_NOEXCEPT
    {
      clear();

      iterator dst = pbuffer;

      while ((dst != pbuffer_end) && (first != last))
      {
        *dst++ = *first++;
      }

      pbuffer_end = dst;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 iterator insert(const_iterator pos, const_reference value) ETL_NOEXCEPT
    {
      return insert(pos, 1, value);
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 iterator insert(const_iterator pos, rvalue_reference value) ETL_NOEXCEPT
    {
      // Out of range?
      if ((pos < pbuffer) || (pos > pbuffer_end))
      {
        return nullptr;
      }

      // Convert to an iterator.
      iterator itr = pbuffer + (pos - pbuffer);

      if (!full())
      {
        // Shifts all elements after 'itr' one position to the right.
        const_iterator sitr = pbuffer_end;
        iterator       ditr = sitr + 1;

        while (sitr != itr)
        {
          *ditr-- = *sitr--;
        }

        *itr = etl::move(value);
        ++pbuffer_end;

        return itr;
      }
      else
      {
        return end();
      }
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 iterator insert(const_iterator pos, size_type count, const_reference value) ETL_NOEXCEPT
    {
      // Out of range?
      if ((pos < pbuffer) || (pos > pbuffer_end))
      {
        return nullptr;
      }

      // Would there be an overflow?
      if (count > (max_size() - size()))
      {
        return nullptr;
      }

      // Convert to an iterator.
      iterator itr = pbuffer + (pos - pbuffer);

      // Shifts all elements after 'itr' count positions to the right.
      const_iterator sitr = pbuffer_end;
      iterator       ditr = sitr + count;

      while (sitr != itr)
      {
        *ditr-- = *sitr--;
      }

      pbuffer_end += count;

      // Insert the values.
      while (count-- != 0)
      {
        *sitr++ = value;
      }

      return itr;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    template <typename TIterator>
    ETL_CONSTEXPR14 iterator insert(const_iterator pos, TIterator first, TIterator last) ETL_NOEXCEPT
    {
      // Out of range?
      if ((pos < pbuffer) || (pos > pbuffer_end))
      {
        return nullptr;
      }

      // Would there be an overflow?    
      size_type count = etl::distance(first, last);

      if (count > (max_size() - size()))
      {
        return nullptr;
      }

      // Convert to an iterator.
      iterator itr = pbuffer + (pos - pbuffer);

      // Shifts all elements after 'itr' count positions to the right.
      const_iterator sitr = pbuffer_end;
      iterator       ditr = sitr + count;

      while (sitr != itr)
      {
        *ditr-- = *sitr--;
      }

      pbuffer_end += count;

      // Insert the values.
      while (count-- != 0)
      {
        *sitr++ = *first++;
      }

      return itr;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    template <typename TRange>
    ETL_CONSTEXPR14 void insert_range(const_iterator pos, TRange range) ETL_NOEXCEPT
    {
      insert(pos, range.begin(), range.end());
    }

    //*************************************************************************
    ///
    //*************************************************************************
    template <typename TRange>
    ETL_CONSTEXPR14 void append_range(TRange range) ETL_NOEXCEPT
    {
      insert(end(), range.begin(), range.end());
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 iterator erase(const_iterator citr) ETL_NOEXCEPT
    {
      // Out of range?
      if ((citr < pbuffer) || (citr > pbuffer_end))
      {
        return nullptr;
      }

      // Convert to an iterator.
      iterator itr = pbuffer + (citr - pbuffer);

      // Null operation.
      if (empty() || (itr == pbuffer_end))
      {
        return nullptr;
      }

      // Shifts all elements after 'itr' one position to the left.
      const_iterator sitr = itr + 1;
      iterator       ditr = itr;

      while (sitr != pbuffer_end)
      {
        *ditr++ = *sitr++;
      }

      --pbuffer_end;

      return itr;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    ETL_CONSTEXPR14 iterator erase(const_iterator citr_begin, const_iterator citr_end) ETL_NOEXCEPT
    {
      // Out of range?
      if ((citr_begin < pbuffer) || (citr_begin > pbuffer_end) || (citr_end < pbuffer) || (citr_end > pbuffer_end))
      {
        return nullptr;
      }

      // Convert to iterators.
      iterator itr_begin = pbuffer + (citr_begin - pbuffer);
      iterator itr_end   = pbuffer + (citr_end - pbuffer);

      // Null operation.
      if (empty() || (citr_begin == pbuffer_end))
      {
        return nullptr;
      }

      // Shifts all elements after 'itr' n positions to the left.
      size_t n = size_t(citr_end - citr_begin);
      const_iterator sitr = itr_begin + n;
      iterator       ditr = itr_begin;

      while (sitr != pbuffer_end)
      {
        *ditr++ = *sitr++;
      }

      --pbuffer_end;

      return itr_begin;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    void swap(itrivial_vector& other) ETL_NOEXCEPT
    {
      pointer lhs = pbuffer;
      pointer rhs = other.pbuffer;

      swap_ranges(pbuffer, pbuffer_end, other.pbuffer);
    }

  protected:

    //*************************************************************************
    /// Constructs the trivial vector.
    //*************************************************************************
    ETL_CONSTEXPR14 itrivial_vector(T* pbuffer_, T* pbuffer_end_, size_type buffer_size_) ETL_NOEXCEPT
      : pbuffer(pbuffer_)
      , pbuffer_end(pbuffer_end_)
      , buffer_size(buffer_size_)
    {
    };

  private:

    T* pbuffer;
    T* pbuffer_end;
    size_type buffer_size;
  };

  //*************************************************************************
  /// Trivial vector.
  //*************************************************************************
  template <typename T, size_t Size>
  class trivial_vector : public itrivial_vector<T>
  {
  public:

    typedef T        value_type;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef T&&      rvalue_reference;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T*       iterator;
    typedef const T* const_iterator;
    typedef size_t   size_type;

    //*************************************************************************
    /// Constructs the default trivial vector.
    //*************************************************************************
    template <typename... TArgs>
    ETL_CONSTEXPR14 explicit trivial_vector() ETL_NOEXCEPT
      : itrivial_vector<T>(buffer, buffer, Size)
      , buffer()
      , buffer_end(buffer)
    {
    }

    //*************************************************************************
    /// Constructs the trivial vector.
    //*************************************************************************
    template <typename... TArgs>
    ETL_CONSTEXPR14 explicit trivial_vector(TArgs&&... args) ETL_NOEXCEPT
      : itrivial_vector<T>(buffer, buffer + sizeof... (args), Size)
      , buffer{etl::forward<TArgs>(args)...}
      , buffer_end{buffer + sizeof... (args)}
    {
    };

  private:

    T  buffer[Size];
    T* buffer_end;
  };

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_USING_CPP17
  template <typename... T>
  trivial_vector(T...) -> trivial_vector<typename etl::common_type_t<T...>, sizeof...(T)>;
#endif

  //*************************************************************************
  ///
  //*************************************************************************
  template <typename T, size_t Size>
  bool operator ==(const etl::trivial_vector<T, Size> lhs, const etl::trivial_vector<T, Size> rhs) ETL_NOEXCEPT
  {
    const etl::itrivial_vector<T>& ilhs = lhs;
    const etl::itrivial_vector<T>& irhs = rhs;

    return etl::equal(ilhs.begin(), ilhs.end(), irhs.begin());
  }

  //*************************************************************************
  ///
  //*************************************************************************
  template <typename T, size_t Size>
  bool operator !=(const etl::trivial_vector<T, Size> lhs, const etl::trivial_vector<T, Size> rhs) ETL_NOEXCEPT
  {
    return !(lhs == rhs);
  }

  //*************************************************************************
  ///
  //*************************************************************************
  template <typename T, size_t Size>
  bool operator <(const etl::trivial_vector<T, Size> lhs, const etl::trivial_vector<T, Size> rhs) ETL_NOEXCEPT
  {
    const etl::itrivial_vector<T>& ilhs = lhs;
    const etl::itrivial_vector<T>& irhs = rhs;

    return etl::lexicographical_compare(ilhs.begin(), ilhs.end(), irhs.begin());
  }

  //*************************************************************************
  ///
  //*************************************************************************
  template <typename T, size_t Size>
  bool operator <=(const etl::trivial_vector<T, Size> lhs, const etl::trivial_vector<T, Size> rhs) ETL_NOEXCEPT
  {
    return !(rhs < lhs);
  }

  //*************************************************************************
  ///
  //*************************************************************************
  template <typename T, size_t Size>
  bool operator >(const etl::trivial_vector<T, Size> lhs, const etl::trivial_vector<T, Size> rhs) ETL_NOEXCEPT
  {
    return (rhs < lhs);
  }

  //*************************************************************************
  ///
  //*************************************************************************
  template <typename T, size_t Size>
  bool operator >=(const etl::trivial_vector<T, Size> lhs, const etl::trivial_vector<T, Size> rhs) ETL_NOEXCEPT
  {
    return !(lhs < rhs);
  }

  //*************************************************************************
  /// Overloaded swap for etl::trivial_vector<T, SIZE>
  ///\param lhs The first trivial_vector.
  ///\param rhs The second trivial_vector.
  //*************************************************************************
  template <typename T, const size_t SIZE>
  void swap(etl::trivial_vector<T, SIZE> &lhs, etl::trivial_vector<T, SIZE> &rhs) ETL_NOEXCEPT
  {
    lhs.swap(rhs);
  }
}

#endif
