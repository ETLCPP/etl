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

#ifndef ETL_INDIRECT_VECTOR_INCLUDED
#define ETL_INDIRECT_VECTOR_INCLUDED

#include "platform.h"
#include "vector.h"
#include "pool.h"
#include "iterator.h"
#include "utility.h"
#include "functional.h"
#include "static_assert.h"
#include "initializer_list.h"

//*****************************************************************************
///\defgroup indirect_vector indirect_vector
/// A indirect_vector with the capacity defined at compile time. Objects are allocated from a pool and stored as pointers.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  ///\ingroup vector
  /// Vector full exception.
  //***************************************************************************
  class indirect_vector_buffer_missmatch : public vector_exception
  {
  public:

    indirect_vector_buffer_missmatch(string_type file_name_, numeric_type line_number_)
      : vector_exception(ETL_ERROR_TEXT("indirect_vector:buffer_missmatch", ETL_INDIRECT_VECTOR_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The base class for specifically sized vectors.
  /// Can be used as a reference type for all vectors containing a specific type.
  ///\ingroup indirect_vector
  //***************************************************************************
  template <typename T>
  class iindirect_vector
  {
  public:

    typedef T              value_type;
    typedef T&             reference;
    typedef const T&       const_reference;
#if ETL_USING_CPP11
    typedef T&&            rvalue_reference;
#endif
    typedef T*             pointer;
    typedef const T*       const_pointer;

    typedef typename etl::ivector<T*>::iterator        indirect_iterator;
    typedef typename etl::ivector<T*>::const_iterator  indirect_const_iterator;

    typedef typename etl::ivector<T*>::size_type       size_type;
    typedef typename etl::ivector<T*>::difference_type difference_type;

    //*************************************************************************
    /// Unary function adaptor.
    //*************************************************************************
    template <typename TUnaryFunction, typename TReturnType = void>
    class unary_function_adaptor
    {
    public:

      unary_function_adaptor(TUnaryFunction unary_function_)
        : unary_function(unary_function_)
      {
      }

      TReturnType operator()(const_pointer indirect_itr)
      {
        return unary_function(*indirect_itr);
      }

      TUnaryFunction unary_function;
    };

    //*************************************************************************
    template <typename TUnaryFunction>
    class unary_function_adaptor<TUnaryFunction, void>
    {
    public:

      unary_function_adaptor(TUnaryFunction unary_function_)
        : unary_function(unary_function_)
      {
      }

      void operator()(const_pointer indirect_itr)
      {
        unary_function(*indirect_itr);
      }

      TUnaryFunction unary_function;
    };

    //*************************************************************************
    /// Binary function adaptor.
    //*************************************************************************
    template <typename TBinaryFunction, typename TReturnType = void>
    class binary_function_adaptor
    {
    public:

      binary_function_adaptor(TBinaryFunction binary_function_)
        : binary_function(binary_function_)
      {
      }

      TReturnType operator()(const_pointer indirect_itr_lhs,
                             const_pointer indirect_itr_rhs)
      {
        return binary_function(*indirect_itr_lhs, *indirect_itr_rhs);
      }

      TBinaryFunction binary_function;
    };

    //*************************************************************************
    template <typename TBinaryFunction>
    class binary_function_adaptor<TBinaryFunction, void>
    {
    public:

      binary_function_adaptor(TBinaryFunction binary_function_)
        : binary_function(binary_function_)
      {
      }

      void operator()(const_pointer indirect_itr_lhs,
                      const_pointer indirect_itr_rhs)
      {
        binary_function(*indirect_itr_lhs, *indirect_itr_rhs);
      }

      TBinaryFunction binary_function;
    };

    //*************************************************************************
    /// iterator.
    //*************************************************************************
    class iterator : public etl::iterator<ETL_OR_STD::random_access_iterator_tag, T>
    {
    public:

      friend class iindirect_vector;
      friend class const_iterator;

      iterator()
        : lookup_itr()
      {
      }

      iterator(const iterator& other)
        : lookup_itr(other.lookup_itr)
      {
      }

      iterator& operator ++()
      {
        ++lookup_itr;
        return *this;
      }

      iterator operator ++(int)
      {
        iterator temp(*this);
        ++lookup_itr;
        return temp;
      }

      iterator& operator --()
      {
        --lookup_itr;
        return *this;
      }

      iterator operator --(int)
      {
        iterator temp(*this);
        --lookup_itr;
        return temp;
      }

      iterator& operator =(const iterator& other)
      {
        lookup_itr = other.lookup_itr;
        return *this;
      }

      iterator operator +=(size_type n)
      {
        lookup_itr += n;
        return *this;
      }

      iterator operator -=(size_type n)
      {
        lookup_itr -= n;
        return *this;
      }

      reference operator *() const
      {
        return **lookup_itr;
      }

      pointer operator &() const
      {
        return &(**lookup_itr);
      }

      pointer operator ->() const
      {
        return &(**lookup_itr);
      }

      friend iterator operator +(const iterator& lhs, difference_type offset)
      {
        iterator result(lhs);
        result += offset;
        return result;
      }

      friend iterator operator -(const iterator& lhs, difference_type offset)
      {
        iterator result(lhs);
        result -= offset;
        return result;
      }

      indirect_iterator indirection()
      {
        return lookup_itr;
      }

      indirect_const_iterator indirection() const
      {
        return lookup_itr;
      }

      friend difference_type operator -(const iterator& lhs, const iterator& rhs)
      {
        return lhs.lookup_itr - rhs.lookup_itr;
      }

      friend bool operator == (const iterator& lhs, const iterator& rhs)
      {
        return lhs.lookup_itr == rhs.lookup_itr;
      }

      friend bool operator != (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

      friend bool operator < (const iterator& lhs, const iterator& rhs)
      {
        return lhs.lookup_itr < rhs.lookup_itr;
      }

    private:

      iterator(indirect_iterator itr_)
        : lookup_itr(itr_)
      {
      }

      indirect_iterator lookup_itr;
    };

    //*************************************************************************
    /// const_iterator.
    //*************************************************************************
    class const_iterator : public etl::iterator<ETL_OR_STD::random_access_iterator_tag, const T>
    {
    public:

      friend class iindirect_vector;

      const_iterator()
        : lookup_itr()
      {
      }

      const_iterator(const const_iterator& other)
        : lookup_itr(other.lookup_itr)
      {
      }

      const_iterator(const typename iindirect_vector::iterator& other)
        : lookup_itr(other.lookup_itr)
      {
      }

      const_iterator& operator ++()
      {
        ++lookup_itr;
        return *this;
      }

      const_iterator operator ++(int)
      {
        const_iterator temp(*this);
        ++lookup_itr;
        return temp;
      }

      const_iterator& operator --()
      {
        --lookup_itr;
        return *this;
      }

      const_iterator operator --(int)
      {
        const_iterator temp(*this);
        --lookup_itr;
        return temp;
      }

      const_iterator operator +=(size_type n)
      {
        lookup_itr += n;
        return *this;
      }

      const_iterator operator -=(size_type n)
      {
        lookup_itr -= n;
        return *this;
      }

      const_iterator& operator =(const const_iterator& other)
      {
        lookup_itr = other.lookup_itr;
        return *this;
      }

      const_reference operator *() const
      {
        return **lookup_itr;
      }

      const_pointer operator &() const
      {
        return &(**lookup_itr);
      }

      const_pointer operator ->() const
      {
        return &(**lookup_itr);
      }

      indirect_const_iterator indirection() const
      {
        return lookup_itr;
      }

      friend const_iterator operator +(const const_iterator& lhs, difference_type offset)
      {
        const_iterator result(lhs);
        result += offset;
        return result;
      }

      friend const_iterator operator -(const const_iterator& lhs, difference_type offset)
      {
        const_iterator result(lhs);
        result -= offset;
        return result;
      }

      friend difference_type operator -(const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.lookup_itr - rhs.lookup_itr;
      }

      friend bool operator == (const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.lookup_itr == rhs.lookup_itr;
      }

      friend bool operator != (const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs == rhs);
      }

      friend bool operator < (const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.lookup_itr < rhs.lookup_itr;
      }

    private:

      typedef typename etl::ivector<T*>::const_iterator lookup_itr_t;

      const_iterator(indirect_const_iterator itr_)
        : lookup_itr(itr_)
      {
      }

      indirect_const_iterator lookup_itr;
    };

    typedef ETL_OR_STD::reverse_iterator<iterator>       reverse_iterator;
    typedef ETL_OR_STD::reverse_iterator<const_iterator> const_reverse_iterator;

  protected:

    typedef typename etl::parameter_type<T>::type parameter_t;

  public:

    //*********************************************************************
    /// Returns an iterator to the beginning of the indirect_vector.
    ///\return An iterator to the beginning of the indirect_vector.
    //*********************************************************************
    iterator begin()
    {
      return iterator(lookup.begin());
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the indirect_vector.
    ///\return A const iterator to the beginning of the indirect_vector.
    //*********************************************************************
    const_iterator begin() const
    {
      return const_iterator(lookup.begin());
    }

    //*********************************************************************
    /// Returns an iterator to the end of the indirect_vector.
    ///\return An iterator to the end of the indirect_vector.
    //*********************************************************************
    iterator end()
    {
      return iterator(lookup.end());
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the indirect_vector.
    ///\return A const iterator to the end of the indirect_vector.
    //*********************************************************************
    const_iterator end() const
    {
      return const_iterator(lookup.end());
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the indirect_vector.
    ///\return A const iterator to the beginning of the indirect_vector.
    //*********************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(lookup.begin());
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the indirect_vector.
    ///\return A const iterator to the end of the indirect_vector.
    //*********************************************************************
    const_iterator cend() const
    {
      return const_iterator(lookup.cend());
    }

    //*********************************************************************
    /// Returns an reverse iterator to the reverse beginning of the indirect_vector.
    ///\return Iterator to the reverse beginning of the indirect_vector.
    //*********************************************************************
    reverse_iterator rbegin()
    {
      return reverse_iterator(end());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the indirect_vector.
    ///\return Const iterator to the reverse beginning of the indirect_vector.
    //*********************************************************************
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(end());
    }

    //*********************************************************************
    /// Returns a reverse iterator to the end + 1 of the indirect_vector.
    ///\return Reverse iterator to the end + 1 of the indirect_vector.
    //*********************************************************************
    reverse_iterator rend()
    {
      return reverse_iterator(begin());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the indirect_vector.
    ///\return Const reverse iterator to the end + 1 of the indirect_vector.
    //*********************************************************************
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(begin());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the indirect_vector.
    ///\return Const reverse iterator to the reverse beginning of the indirect_vector.
    //*********************************************************************
    const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(cend());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the indirect_vector.
    ///\return Const reverse iterator to the end + 1 of the indirect_vector.
    //*********************************************************************
    const_reverse_iterator crend() const
    {
      return const_reverse_iterator(cbegin());
    }

    //*********************************************************************
    /// Resizes the indirect_vector.
    /// If asserts or exceptions are enabled and the new size is larger than the
    /// maximum then a vector_full is thrown.
    ///\param new_size The new size.
    //*********************************************************************
    void resize(size_t new_size)
    {
      resize(new_size, T());
    }

    //*********************************************************************
    /// Resizes the indirect_vector.
    /// If asserts or exceptions are enabled and the new size is larger than the
    /// maximum then a vector_full is thrown.
    ///\param new_size The new size.
    ///\param value   The value to fill new elements with. Default = default constructed value.
    //*********************************************************************
    void resize(size_t new_size, const_reference value)
    {
      ETL_ASSERT(new_size <= capacity(), ETL_ERROR(vector_full));

      if (new_size <= capacity())
      {
        if (new_size > size())
        {
          size_type n = new_size - size();

          while (n-- != 0U)
          {
            T* p = storage.create<T>(value);
            lookup.push_back(p);
          }
        }
        else
        {
          size_type n = size() - new_size;

          while (n-- != 0U)
          {
            pop_back();
          }
        }
      }
    }

    //*********************************************************************
    /// For compatibility with the STL vector API.
    /// Does not increase the capacity, as this is fixed.
    /// Asserts an etl::vector_out_of_bounds error if the request is for more than the capacity.
    //*********************************************************************
    void reserve(size_t n)
    {
      ETL_ASSERT(n <= capacity(), ETL_ERROR(vector_out_of_bounds));
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'i'
    ///\param i The index.
    ///\return A reference to the value at index 'i'
    //*********************************************************************
    reference operator [](size_t i)
    {
      return *lookup[i];
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'i'
    ///\param i The index.
    ///\return A const reference to the value at index 'i'
    //*********************************************************************
    const_reference operator [](size_t i) const
    {
      return *lookup[i];
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'i'
    /// If asserts or exceptions are enabled, emits an etl::vector_out_of_bounds if the index is out of range.
    ///\param i The index.
    ///\return A reference to the value at index 'i'
    //*********************************************************************
    reference at(size_t i)
    {
      return *lookup.at(i);
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'i'
    /// If asserts or exceptions are enabled, emits an etl::vector_out_of_bounds if the index is out of range.
    ///\param i The index.
    ///\return A const reference to the value at index 'i'
    //*********************************************************************
    const_reference at(size_t i) const
    {
      return *lookup.at(i);
    }

    //*********************************************************************
    /// Returns a reference to the first element.
    ///\return A reference to the first element.
    //*********************************************************************
    reference front()
    {
      return *(lookup.front());
    }

    //*********************************************************************
    /// Returns a const reference to the first element.
    ///\return A const reference to the first element.
    //*********************************************************************
    const_reference front() const
    {
      return *(lookup.front());
    }

    //*********************************************************************
    /// Returns a reference to the last element.
    ///\return A reference to the last element.
    //*********************************************************************
    reference back()
    {
      return *(lookup.back());
    }

    //*********************************************************************
    /// Returns a const reference to the last element.
    ///\return A const reference to the last element.
    //*********************************************************************
    const_reference back() const
    {
      return *(lookup.back());
    }

    //*********************************************************************
    /// Assigns values to the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector does not have enough free space.
    /// If asserts or exceptions are enabled, emits vector_iterator if the iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
      ETL_STATIC_ASSERT((etl::is_same<typename etl::remove_cv<T>::type, typename etl::remove_cv<typename etl::iterator_traits<TIterator>::value_type>::type>::value), "Iterator type does not match container type");

#if ETL_IS_DEBUG_BUILD
      difference_type d = etl::distance(first, last);
      ETL_ASSERT(static_cast<size_t>(d) <= capacity(), ETL_ERROR(vector_full));
#endif

      initialise();

      while (first != last)
      {
        T* p = storage.create<T>(*first);
        lookup.push_back(p);
        ++first;
      }
    }

    //*********************************************************************
    /// Assigns values to the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector does not have enough free space.
    ///\param n     The number of elements to add.
    ///\param value The value to insert for each element.
    //*********************************************************************
    void assign(size_t n, parameter_t value)
    {
      ETL_ASSERT(n <= capacity(), ETL_ERROR(vector_full));

      initialise();

      while (n-- != 0U)
      {
        T* p = storage.create<T>(value);
        lookup.push_back(p);
      }
    }

    //*************************************************************************
    /// Clears the indirect_vector.
    //*************************************************************************
    void clear()
    {
      initialise();
    }

    //*************************************************************************
    /// Fills the buffer.
    //*************************************************************************
    void fill(const T& value)
    {
      etl::fill(begin(), end(), value);
    }

    //*********************************************************************
    /// Inserts a value at the end of the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    void push_back(const_reference value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(size() != capacity(), ETL_ERROR(vector_full));
#endif
      T* p = storage.create<T>(value);
      lookup.push_back(p);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Inserts a value at the end of the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    void push_back(rvalue_reference value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(size() != capacity(), ETL_ERROR(vector_full));
#endif
      T* p = storage.create<T>(etl::move(value));
      lookup.push_back(p);
    }
#endif

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT && !defined(ETL_VECTOR_FORCE_CPP03_IMPLEMENTATION)
    //*********************************************************************
    /// Constructs a value at the end of the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    template <typename ... Args>
    reference emplace_back(Args && ... args)
    {
      T* p = storage.create<T>(etl::forward<Args>(args)...);
      lookup.push_back(p);
      return back();
    }
#else
    //*********************************************************************
    /// Constructs a value at the end of the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    reference emplace_back()
    {
      T* p = storage.create<T>(T());
      lookup.push_back(p);
      return back();
    }

    //*********************************************************************
    /// Constructs a value at the end of the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    template <typename T1>
    reference emplace_back(const T1& value1)
    {
      T* p = storage.create<T>(T(value1));
      lookup.push_back(p);
      return back();
    }

    //*********************************************************************
    /// Constructs a value at the end of the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    template <typename T1, typename T2>
    reference emplace_back(const T1& value1, const T2& value2)
    {
      T* p = storage.create<T>(T(value1, value2));
      lookup.push_back(p);
      return back();
    }

    //*********************************************************************
    /// Constructs a value at the end of the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    template <typename T1, typename T2, typename T3>
    reference emplace_back(const T1& value1, const T2& value2, const T3& value3)
    {
      T* p = storage.create<T>(T(value1, value2, value3));
      lookup.push_back(p);
      return back();
    }

    //*********************************************************************
    /// Constructs a value at the end of the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    reference emplace_back(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      T* p = storage.create<T>(T(value1, value2, value3, value4));
      lookup.push_back(p);
      return back();
    }
#endif

    //*************************************************************************
    /// Removes an element from the end of the indirect_vector.
    //*************************************************************************
    void pop_back()
    {
      ETL_ASSERT(!empty(), ETL_ERROR(vector_empty));

      reference object = back();
      storage.destroy<T>(etl::addressof(object));
      lookup.pop_back();
    }

    //*********************************************************************
    /// Inserts a value to the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param position The position to insert before.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator position, const_reference value)
    {
      ETL_ASSERT(size() != capacity(), ETL_ERROR(vector_full));

      T* p = storage.create<T>(T(value));
      position = iterator(lookup.insert(position.lookup_itr, p));

      return to_iterator(position);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Inserts a value to the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param position The position to insert before.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator position, rvalue_reference value)
    {
      ETL_ASSERT(size() != capacity(), ETL_ERROR(vector_full));

      T* p = storage.create<T>(T(etl::move(value)));
      position = iterator(lookup.insert(position.lookup_itr, p));

      return to_iterator(position);
    }
#endif

    //*************************************************************************
    /// Emplaces a value to the vector at the specified position.
    //*************************************************************************
#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT && !defined(ETL_VECTOR_FORCE_CPP03_IMPLEMENTATION)
    template <typename ... Args>
    iterator emplace(iterator position, Args && ... args)
    {
      ETL_ASSERT(!full(), ETL_ERROR(vector_full));

      T* p = storage.create<T>(T(etl::forward<Args>(args)...));
      position = iterator(lookup.insert(position.lookup_itr, p));

      return position;
    }
#else
    iterator emplace(iterator position)
    {
      ETL_ASSERT(!full(), ETL_ERROR(vector_full));

      T* p = storage.create<T>(T());
      position = iterator(lookup.insert(position.lookup_itr, p));

      return position;
    }

    template <typename T1>
    iterator emplace(iterator position, const T1& value1)
    {
      ETL_ASSERT(!full(), ETL_ERROR(vector_full));

      T* p = storage.create<T>(T(value1));
      position = iterator(lookup.insert(position.lookup_itr, p));

      return position;
    }

    template <typename T1, typename T2>
    iterator emplace(iterator position, const T1& value1, const T2& value2)
    {
      ETL_ASSERT(!full(), ETL_ERROR(vector_full));

      T* p = storage.create<T>(T(value1, value2));
      position = iterator(lookup.insert(position.lookup_itr, p));

      return position;
    }

    template <typename T1, typename T2, typename T3>
    iterator emplace(iterator position, const T1& value1, const T2& value2, const T3& value3)
    {
      ETL_ASSERT(!full(), ETL_ERROR(vector_full));

      T* p = storage.create<T>(T(value1, value2, value3));
      position = iterator(lookup.insert(position.lookup_itr, p));

      return position;
    }

    template <typename T1, typename T2, typename T3, typename T4>
    iterator emplace(iterator position, const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      ETL_ASSERT(!full(), ETL_ERROR(vector_full));

      T* p = storage.create<T>(T(value1, value2, value3, value4));
      position = iterator(lookup.insert(position.lookup_itr, p));

      return position;
    }
#endif

    //*********************************************************************
    /// Inserts 'n' values to the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector does not have enough free space.
    ///\param position The position to insert before.
    ///\param n        The number of elements to add.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator position, size_t n, parameter_t value)
    {
      ETL_ASSERT((size() + n) <= capacity(), ETL_ERROR(vector_full));

      iterator position_ = to_iterator(position);

      // Make space for the new lookup pointers.
      typename etl::ivector<T*>::iterator lookup_itr = position_.lookup_itr;
      lookup.insert(lookup_itr, n, ETL_NULLPTR);

      while (n-- != 0U)
      {
        T* p = storage.create<T>(value);
        *lookup_itr++ = p;
      }

      return position_;
    }

    //*********************************************************************
    /// Inserts a range of values to the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector does not have enough free space.
    ///\param position The position to insert before.
    ///\param first    The first element to add.
    ///\param last     The last + 1 element to add.
    //*********************************************************************
    template <class TIterator>
    iterator insert(const_iterator position, TIterator first, TIterator last)
    {
      size_t count = size_t(etl::distance(first, last));

      ETL_ASSERT((size() + count) <= capacity(), ETL_ERROR(vector_full));

      // Make space for the new lookup pointers.
      typename etl::ivector<T*>::iterator lookup_itr = to_iterator(position).lookup_itr;
      lookup.insert(lookup_itr, count, ETL_NULLPTR);

      while (first != last)
      {
        T* p = storage.create<T>(*first);
        *lookup_itr++ =  p;
        ++first;
      }

      return to_iterator(position);
    }

    //*********************************************************************
    /// Erases an element.
    ///\param i_element Iterator to the element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(iterator i_element)
    {
      storage.destroy<T>(etl::addressof(*i_element));

      return iterator(lookup.erase(i_element.lookup_itr));
    }

    //*********************************************************************
    /// Erases an element.
    ///\param i_element Iterator to the element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(const_iterator i_element)
    {
      storage.destroy<T>(etl::addressof(*i_element));

      return iterator(lookup.erase(i_element.lookup_itr));
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
      iterator element = to_iterator(first);

      while (element != last)
      {
        storage.destroy<T>(etl::addressof(*element));
        ++element;
      }

      lookup.erase(first.lookup_itr, last.lookup_itr);

      return to_iterator(last);
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    iindirect_vector& operator = (const iindirect_vector& rhs)
    {
      if (&rhs != this)
      {
        assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    iindirect_vector& operator = (iindirect_vector&& rhs)
    {
      if (&rhs != this)
      {
        clear();
        iterator itr = rhs.begin();
        while (itr != rhs.end())
        {
          push_back(etl::move(*itr));
          ++itr;
        }

        rhs.initialise();
      }

      return *this;
    }
#endif

    //*************************************************************************
    /// Gets the current size of the indirect_vector.
    ///\return The current size of the indirect_vector.
    //*************************************************************************
    size_type size() const
    {
      return lookup.size();
    }

    //*************************************************************************
    /// Gets the current capacity of the indirect_vector.
    ///\return The capacity of the indirect_vector.
    //*************************************************************************
    size_type capacity() const
    {
      return lookup.capacity();
    }

    //*************************************************************************
    /// Checks the 'empty' state of the indirect_vector.
    ///\return <b>true</b> if empty.
    //*************************************************************************
    bool empty() const
    {
      return lookup.empty();
    }

    //*************************************************************************
    /// Checks the 'full' state of the indirect_vector.
    ///\return <b>true</b> if full.
    //*************************************************************************
    bool full() const
    {
      return lookup.full();
    }

    //*************************************************************************
    /// Returns the maximum size.
    ///\return The maximum size.
    //*************************************************************************
    size_type max_size() const
    {
      return lookup.max_size();
    }

    //*************************************************************************
    /// Returns the remaining capacity.
    ///\return The remaining capacity.
    //*************************************************************************
    size_type available() const
    {
      return lookup.available();
    }

  protected:

    //*********************************************************************
    /// Constructor.
    //*********************************************************************
    iindirect_vector(etl::ivector<T*>& lookup_, etl::ipool& storage_)
      : lookup(lookup_)
      , storage(storage_)
    {
    }

    //*********************************************************************
    /// Initialise the indirect_vector.
    //*********************************************************************
    void initialise()
    {
      iterator itr = begin();

      while (itr != end())
      {
        storage.destroy<T>(etl::addressof(*itr));
        ++itr;
      }

      lookup.clear();
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Move from a container.
    //*********************************************************************
    void move_container(iindirect_vector&& other)
    {
      if (this != &other)
      {
        initialise();

        typename iindirect_vector<T>::iterator itr = other.begin();

        while (itr != other.end())
        {
          push_back(etl::move(*itr));
          ++itr;
        }

        other.initialise();
      }
    }
#endif

    etl::ivector<T*>& lookup;
    etl::ipool&       storage;

  private:

    // Disable copy construction.
    iindirect_vector(const iindirect_vector&) ETL_DELETE;

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_INDIRECT_VECTOR) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual
#else
  protected:
#endif
    ~iindirect_vector()
    {
    }

  protected:

    //*************************************************************************
    /// Convert from const_iterator to iterator.
    //*************************************************************************
    iterator to_iterator(const_iterator itr) const
    {
      return iterator(const_cast<indirect_iterator>(itr.lookup_itr));
    }
  };

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first indirect_vector.
  ///\param rhs Reference to the second indirect_vector.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup indirect_vector
  //***************************************************************************
  template <typename T>
  bool operator ==(const etl::iindirect_vector<T>& lhs, const etl::iindirect_vector<T>& rhs)
  {
    return (lhs.size() == rhs.size()) && etl::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first indirect_vector.
  ///\param rhs Reference to the second indirect_vector.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup indirect_vector
  //***************************************************************************
  template <typename T>
  bool operator !=(const etl::iindirect_vector<T>& lhs, const etl::iindirect_vector<T>& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Less than operator.
  ///\param lhs Reference to the first indirect_vector.
  ///\param rhs Reference to the second indirect_vector.
  ///\return <b>true</b> if the first indirect_vector is lexicographically less than the second, otherwise <b>false</b>
  ///\ingroup indirect_vector
  //***************************************************************************
  template <typename T>
  bool operator <(const etl::iindirect_vector<T>& lhs, const etl::iindirect_vector<T>& rhs)
  {
    return etl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }

  //***************************************************************************
  /// Greater than operator.
  ///\param lhs Reference to the first indirect_vector.
  ///\param rhs Reference to the second indirect_vector.
  ///\return <b>true</b> if the first indirect_vector is lexicographically greater than the second, otherwise <b>false</b>
  ///\ingroup indirect_vector
  //***************************************************************************
  template <typename T>
  bool operator >(const etl::iindirect_vector<T>& lhs, const etl::iindirect_vector<T>& rhs)
  {
    return (rhs < lhs);
  }

  //***************************************************************************
  /// Less than or equal operator.
  ///\param lhs Reference to the first indirect_vector.
  ///\param rhs Reference to the second indirect_vector.
  ///\return <b>true</b> if the first indirect_vector is lexicographically less than or equal to the second, otherwise <b>false</b>
  ///\ingroup indirect_vector
  //***************************************************************************
  template <typename T>
  bool operator <=(const etl::iindirect_vector<T>& lhs, const etl::iindirect_vector<T>& rhs)
  {
    return !(lhs > rhs);
  }

  //***************************************************************************
  /// Greater than or equal operator.
  ///\param lhs Reference to the first indirect_vector.
  ///\param rhs Reference to the second indirect_vector.
  ///\return <b>true</b> if the first indirect_vector is lexicographically greater than or equal to the second, otherwise <b>false</b>
  ///\ingroup indirect_vector
  //***************************************************************************
  template <typename T>
  bool operator >=(const etl::iindirect_vector<T>& lhs, const etl::iindirect_vector<T>& rhs)
  {
    return !(lhs < rhs);
  }

  //***************************************************************************
  /// A indirect_vector implementation that uses a fixed size buffer.
  ///\tparam T The element type.
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup indirect_vector
  //***************************************************************************
  template <typename T, const size_t MAX_SIZE_>
  class indirect_vector : public etl::iindirect_vector<T>
  {
  public:

    ETL_STATIC_ASSERT((MAX_SIZE_ > 0U), "Zero capacity etl::indirect_vector is not valid");

    static ETL_CONSTANT size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    indirect_vector()
      : etl::iindirect_vector<T>(lookup_vector, storage_pool)
    {
    }

    //*************************************************************************
    /// Constructor, with size.
    ///\param initial_size The initial size of the indirect_vector.
    //*************************************************************************
    explicit indirect_vector(size_t initial_size)
      : etl::iindirect_vector<T>(lookup_vector, storage_pool)
    {
      this->resize(initial_size);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initial_size  The initial size of the indirect_vector.
    ///\param value        The value to fill the indirect_vector with.
    //*************************************************************************
    indirect_vector(size_t initial_size, typename etl::iindirect_vector<T>::parameter_t value)
      : etl::iindirect_vector<T>(lookup_vector, storage_pool)
    {
      this->resize(initial_size, value);
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    indirect_vector(TIterator first, TIterator last)
      : etl::iindirect_vector<T>(lookup_vector, storage_pool)
    {
      this->assign(first, last);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    /// Constructor, from an initializer_list.
    //*************************************************************************
    indirect_vector(std::initializer_list<T> init)
      : etl::iindirect_vector<T>(lookup_vector, storage_pool)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    indirect_vector(const indirect_vector& other)
      : etl::iindirect_vector<T>(lookup_vector, storage_pool)
    {
      this->assign(other.begin(), other.end());
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    indirect_vector& operator = (const indirect_vector& rhs)
    {
      if (&rhs != this)
      {
        this->assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move constructor.
    //*************************************************************************
    indirect_vector(indirect_vector&& other)
      : etl::iindirect_vector<T>(lookup_vector, storage_pool)
    {
      this->move_container(etl::move(other));
    }

    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    indirect_vector& operator = (indirect_vector&& rhs)
    {
      this->move_container(etl::move(rhs));

      return *this;
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~indirect_vector()
    {
      this->clear();
    }

  private:

    etl::vector<T*, MAX_SIZE> lookup_vector;
    etl::pool<T, MAX_SIZE>    storage_pool;
  };

  template <typename T, const size_t MAX_SIZE_>
  ETL_CONSTANT size_t indirect_vector<T, MAX_SIZE_>::MAX_SIZE;

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST
  template <typename T, typename... Ts>
  indirect_vector(T, Ts...)
    ->indirect_vector<etl::enable_if_t<(etl::is_same_v<T, Ts> && ...), T>, 1U + sizeof...(Ts)>;
#endif 

  //*************************************************************************
  /// Make
  //*************************************************************************
#if ETL_USING_CPP11 && ETL_HAS_INITIALIZER_LIST
  template <typename... T>
  constexpr auto make_indirect_vector(T&&... t) -> etl::indirect_vector<typename etl::common_type_t<T...>, sizeof...(T)>
  {
    return { { etl::forward<T>(t)... } };
  }
#endif

  //***************************************************************************
  /// A indirect_vector implementation that uses a fixed size buffer.
  /// The buffer is supplied on construction.
  ///\tparam T The element type.
  ///\ingroup indirect_vector
  //***************************************************************************
  template <typename T>
  class indirect_vector_ext : public etl::iindirect_vector<T>
  {
  public:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    indirect_vector_ext(etl::ivector<T*>& lookup_, etl::ipool& pool_)
      : etl::iindirect_vector<T>(lookup_, pool_)
    {
      ETL_ASSERT(lookup_.capacity() <= pool_.capacity(), ETL_ERROR(indirect_vector_buffer_missmatch));
    }

    //*************************************************************************
    /// Constructor, with size.
    ///\param initial_size The initial size of the indirect_vector_ext.
    //*************************************************************************
    explicit indirect_vector_ext(size_t initial_size, etl::ivector<T*>& lookup_, etl::ipool& pool_)
      : etl::iindirect_vector<T>(lookup_, pool_)
    {
      ETL_ASSERT(lookup_.capacity() <= pool_.capacity(), ETL_ERROR(indirect_vector_buffer_missmatch));
      this->resize(initial_size);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initial_size  The initial size of the indirect_vector_ext.
    ///\param value        The value to fill the indirect_vector_ext with.
    //*************************************************************************
    indirect_vector_ext(size_t initial_size, typename etl::iindirect_vector<T>::parameter_t value, etl::ivector<T*>& lookup_, etl::ipool& pool_)
      : etl::iindirect_vector<T>(lookup_, pool_)
    {
      ETL_ASSERT(lookup_.capacity() <= pool_.capacity(), ETL_ERROR(indirect_vector_buffer_missmatch));
      this->resize(initial_size, value);
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    indirect_vector_ext(TIterator first, TIterator last, etl::ivector<T*>& lookup_, etl::ipool& pool_)
      : etl::iindirect_vector<T>(lookup_, pool_)
    {
      ETL_ASSERT(lookup_.capacity() <= pool_.capacity(), ETL_ERROR(indirect_vector_buffer_missmatch));
      this->assign(first, last);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    /// Constructor, from an initializer_list.
    //*************************************************************************
    indirect_vector_ext(std::initializer_list<T> init, etl::ivector<T*>& lookup_, etl::ipool& pool_)
      : etl::iindirect_vector<T>(lookup_, pool_)
    {
      ETL_ASSERT(lookup_.capacity() <= pool_.capacity(), ETL_ERROR(indirect_vector_buffer_missmatch));
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Construct a copy.
    //*************************************************************************
    indirect_vector_ext(const indirect_vector_ext& other, etl::ivector<T*>& lookup_, etl::ipool& pool_)
      : etl::iindirect_vector<T>(lookup_, pool_)
    {
      ETL_ASSERT(lookup_.capacity() <= pool_.capacity(), ETL_ERROR(indirect_vector_buffer_missmatch));
      this->assign(other.begin(), other.end());
    }

    //*************************************************************************
    /// Copy constructor (Deleted)
    //*************************************************************************
    indirect_vector_ext(const indirect_vector_ext& other) ETL_DELETE;

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    indirect_vector_ext& operator = (const indirect_vector_ext& rhs)
    {
      if (&rhs != this)
      {
        this->assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move construct.
    //*************************************************************************
    indirect_vector_ext(indirect_vector_ext&& other, etl::ivector<T*>& lookup_, etl::ipool& pool_)
      : etl::iindirect_vector<T>(lookup_, pool_)
    {
      ETL_ASSERT(lookup_.capacity() <= pool_.capacity(), ETL_ERROR(indirect_vector_buffer_missmatch));
      this->move_container(etl::move(other));
    }

    //*************************************************************************
    /// Move constructor.
    //*************************************************************************
    indirect_vector_ext(indirect_vector_ext&& other) ETL_DELETE;

    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    indirect_vector_ext& operator = (indirect_vector_ext&& rhs)
    {
      this->move_container(etl::move(rhs));

      return *this;
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~indirect_vector_ext()
    {
      this->clear();
    }
  };
}

#endif

