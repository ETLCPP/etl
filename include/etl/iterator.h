///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

#ifndef ETL_ITERATOR_INCLUDED
#define ETL_ITERATOR_INCLUDED

#include "platform.h"
#include "type_traits.h"
#include "utility.h"
#include "private/addressof.h"

#if ETL_USING_STL || defined(ETL_IN_UNIT_TEST)
  #include <iterator>
#endif

///\defgroup iterator iterator
///\ingroup utilities

namespace etl
{
  //***************************************************************************
  // iterator tags
  struct input_iterator_tag {};
  struct output_iterator_tag {};
  struct forward_iterator_tag : public input_iterator_tag {};
  struct bidirectional_iterator_tag : public forward_iterator_tag {};
  struct random_access_iterator_tag : public bidirectional_iterator_tag {};
  //struct contiguous_iterator_tag : public random_access_iterator_tag {};

  //***************************************************************************
  // iterator_traits

  // For anything not a fundamental type.
  template <typename TIterator, typename = typename etl::enable_if<!etl::is_fundamental<TIterator>::value, void>::type>
  struct iterator_traits
  {
    typedef typename TIterator::iterator_category iterator_category;
    typedef typename TIterator::value_type        value_type;
    typedef typename TIterator::difference_type   difference_type;
    typedef typename TIterator::pointer           pointer;
    typedef typename TIterator::reference         reference;
  };
 
  // For pointers.
  template <typename T>
  struct iterator_traits<T*, void>
  {
    typedef ETL_OR_STD::random_access_iterator_tag iterator_category;
    typedef T                                      value_type;
    typedef ptrdiff_t                              difference_type;
    typedef typename etl::remove_cv<T>::type*      pointer;
    typedef T&                                     reference;
  };

  // For const pointers.
  template <typename T>
  struct iterator_traits<const T*, void>
  {
    typedef ETL_OR_STD::random_access_iterator_tag  iterator_category;
    typedef T                                       value_type;
    typedef ptrdiff_t                               difference_type;
    typedef const typename etl::remove_cv<T>::type* pointer;
    typedef const T&                                reference;
  };

  //***************************************************************************
  // advance
  template <typename TIterator, typename TDistance>
  ETL_CONSTEXPR14 void advance_helper(TIterator& itr, TDistance n, ETL_OR_STD::output_iterator_tag)
  {
    while (n--)
    {
      ++itr;
    }
  }

  template <typename TIterator, typename TDistance>
  ETL_CONSTEXPR14 void advance_helper(TIterator& itr, TDistance n, ETL_OR_STD::forward_iterator_tag)
  {
    while (n--)
    {
      ++itr;
    }
  }

  template <typename TIterator, typename TDistance>
  ETL_CONSTEXPR14 void advance_helper(TIterator& itr, TDistance n, ETL_OR_STD::bidirectional_iterator_tag)
  {
    if (n > 0)
    {
      while (n--)
      {
        ++itr;
      }
    }
    else
    {
      while (n++)
      {
        --itr;
      }
    }
  }

  template <typename TIterator, typename TDistance>
  ETL_CONSTEXPR14 void advance_helper(TIterator& itr, TDistance n, ETL_OR_STD::random_access_iterator_tag)
  {
    itr += n;
  }

  template <typename TIterator, typename TDistance>
  ETL_CONSTEXPR14 void advance(TIterator& itr, TDistance n)
  {
    typedef typename etl::iterator_traits<TIterator>::iterator_category tag;

    advance_helper(itr, n, tag());
  }

  //***************************************************************************
  // distance
  template<typename TIterator>
  ETL_CONSTEXPR14 typename etl::iterator_traits<TIterator>::difference_type distance_helper(TIterator first, TIterator last, ETL_OR_STD::input_iterator_tag)
  {
    typename etl::iterator_traits<TIterator>::difference_type d = 0;

    while (first != last)
    {
      ++d;
      ++first;
    }

    return d;
  }

  template<typename TIterator>
  ETL_CONSTEXPR14 typename etl::iterator_traits<TIterator>::difference_type distance_helper(TIterator first, TIterator last, ETL_OR_STD::forward_iterator_tag)
  {
    typename etl::iterator_traits<TIterator>::difference_type d = 0;

    while (first != last)
    {
      ++d;
      ++first;
    }

    return d;
  }

  template<typename TIterator>
  ETL_CONSTEXPR14 typename etl::iterator_traits<TIterator>::difference_type distance_helper(TIterator first, TIterator last, ETL_OR_STD::bidirectional_iterator_tag)
  {
    typename etl::iterator_traits<TIterator>::difference_type d = 0;

    while (first != last)
    {
      ++d;
      ++first;
    }

    return d;
  }

  template<typename TIterator>
  ETL_CONSTEXPR14 typename etl::iterator_traits<TIterator>::difference_type distance_helper(TIterator first, TIterator last, ETL_OR_STD::random_access_iterator_tag)
  {
    return last - first;
  }

  template<typename TIterator>
  ETL_CONSTEXPR14 typename etl::iterator_traits<TIterator>::difference_type distance(TIterator first, TIterator last)
  {
    typedef typename etl::iterator_traits<TIterator>::iterator_category tag;

    return distance_helper(first, last, tag());
  }

  //***************************************************************************
  // Previous
  template<typename TIterator>
  ETL_CONSTEXPR14 TIterator prev(TIterator itr, typename etl::iterator_traits<TIterator>::difference_type n = 1)
  {
    etl::advance(itr, -n);

    return itr;
  }

  //***************************************************************************
  // Next
  template<typename TIterator>
  ETL_CONSTEXPR14 TIterator next(TIterator itr, typename etl::iterator_traits<TIterator>::difference_type n = 1)
  {
    etl::advance(itr, n);

    return itr;
  }

  //***************************************************************************
  // reverse_iterator
  template <typename TIterator>
  class reverse_iterator
  {
  public:

    typedef typename iterator_traits<TIterator>::iterator_category iterator_category;
    typedef typename iterator_traits<TIterator>::value_type        value_type;
    typedef typename iterator_traits<TIterator>::difference_type   difference_type;
    typedef typename iterator_traits<TIterator>::pointer           pointer;
    typedef typename iterator_traits<TIterator>::reference         reference;

    typedef TIterator iterator_type;

    ETL_CONSTEXPR14 reverse_iterator()
      : current()
    {
    }

    ETL_CONSTEXPR14 explicit reverse_iterator(TIterator itr)
      : current(itr)
    {
    }

    template <typename TOther>
    ETL_CONSTEXPR14 reverse_iterator(const reverse_iterator<TOther>& other)
      : current(other.base())
    {
    }

    template<class TOther>
    ETL_CONSTEXPR14 reverse_iterator& operator =(const reverse_iterator<TOther>& other)
    {
      current = other.base();

      return (*this);
    }

    ETL_CONSTEXPR14 TIterator base() const
    {
      return current;
    }

    ETL_NODISCARD ETL_CONSTEXPR14 reference operator*() const
    {
      TIterator temp = current;

      return *(--temp);
    }

    ETL_NODISCARD ETL_CONSTEXPR14 pointer operator->() const
    {
      TIterator temp = current;

      return &(*--temp);
    }

    ETL_CONSTEXPR14 reverse_iterator& operator++()
    {
      --current;

      return *this;
    }

    ETL_CONSTEXPR14 reverse_iterator operator++(int)
    {
      reverse_iterator temp = *this;
      --current;

      return temp;
    }

    ETL_CONSTEXPR14 reverse_iterator& operator--()
    {
      ++current;

      return (*this);
    }

    ETL_CONSTEXPR14 reverse_iterator operator--(int)
    {
      reverse_iterator temp = *this;
      ++current;

      return temp;
    }

    ETL_CONSTEXPR14 reverse_iterator& operator+=(const difference_type offset)
    {
      current -= offset;

      return (*this);
    }

    ETL_CONSTEXPR14 reverse_iterator& operator-=(const difference_type offset)
    {
      current += offset;

      return (*this);
    }

    ETL_NODISCARD ETL_CONSTEXPR14 reverse_iterator operator+(const difference_type offset) const
    {
      return reverse_iterator(current - offset);
    }

    ETL_NODISCARD ETL_CONSTEXPR14 reverse_iterator operator-(const difference_type offset) const
    {
      return (reverse_iterator(current + offset));
    }

    ETL_NODISCARD ETL_CONSTEXPR14 reference operator[](const difference_type offset) const
    {
      return (*(*this + offset));
    }

  protected:

    TIterator current;
  };

  template <typename TIterator>
  ETL_CONSTEXPR14 bool operator ==(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return lhs.base() == rhs.base();
  }

  template <typename TIterator>
  ETL_CONSTEXPR14 bool operator !=(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return !(lhs == rhs);
  }

  template <typename TIterator>
  ETL_CONSTEXPR14 bool operator <(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return rhs.base() < lhs.base();
  }

  template <typename TIterator>
  ETL_CONSTEXPR14 bool operator >(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return rhs < lhs;
  }

  template <typename TIterator>
  ETL_CONSTEXPR14 bool operator <=(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return !(rhs < lhs);
  }

  template <typename TIterator>
  ETL_CONSTEXPR14 bool operator >=(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return !(lhs < rhs);
  }

  template <typename TIterator>
  ETL_CONSTEXPR14 typename reverse_iterator<TIterator>::difference_type operator -(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return rhs.base() - lhs.base();
  }

  template <typename TIterator, class TDifference>
  ETL_CONSTEXPR14 reverse_iterator<TIterator> operator +(TDifference n, const reverse_iterator<TIterator>& itr)
  {
    return itr.operator +(n);
  }

  //***************************************************************************
  /// iterator
  //***************************************************************************
  template <typename TCategory, typename T, typename TDistance = ptrdiff_t, typename TPointer = T* , typename TReference = T& >
  struct iterator
  {
    typedef T          value_type;
    typedef TDistance  difference_type;
    typedef TPointer   pointer;
    typedef TReference reference;
    typedef TCategory  iterator_category;
  };

#if ETL_USING_CPP11
  //***************************************************************************
  // move_iterator
  template <typename TIterator>
  class move_iterator
  {
  public:

    typedef typename iterator_traits<TIterator>::iterator_category iterator_category;
    typedef typename iterator_traits<TIterator>::value_type        value_type;
    typedef typename iterator_traits<TIterator>::difference_type   difference_type;
    typedef TIterator    iterator_type;
    typedef TIterator    pointer;
    typedef value_type&& reference;

    move_iterator()
    {
    }

    explicit move_iterator(TIterator itr)
      : current(itr)
    {
    }

    template <typename U>
    move_iterator(const move_iterator<U>& itr)
      : current(itr.base())
    {
    }

    template <typename U>
    move_iterator& operator =(const move_iterator<U>& itr)
    {
      current = itr.current;
      return *this;
    }

    iterator_type base() const
    {
      return current;
    }

    pointer operator ->() const
    {
      return current;
    }

    reference operator *() const
    {
      return etl::move(*current);
    }

    move_iterator& operator++()
    {
      ++current;
      return *this;
    }

    move_iterator& operator--()
    {
      --current;
      return *this;
    }

    move_iterator operator++(int)
    {
      move_iterator temp = *this;
      ++current;
      return temp;
    }

    move_iterator operator--(int)
    {
      move_iterator temp = *this;
      --current;
      return temp;
    }

    move_iterator operator +(difference_type n) const
    {
      return move_iterator(current + n);
    }

    move_iterator operator -(difference_type n) const
    {
      return move_iterator(current - n);
    }

    move_iterator operator +=(difference_type n)
    {
      current += n;
      return *this;
    }

    move_iterator operator -=(difference_type n)
    {
      current -= n;
      return *this;
    }

    reference operator [](difference_type n) const
    {
      return etl::move(current[n]);
    }

  private:

    TIterator current;
  };

  template <typename TIterator>
  bool operator ==(const etl::move_iterator<TIterator>& lhs,
                   const etl::move_iterator<TIterator>& rhs)
  {
    return lhs.base() == rhs.base();
  }

  template <typename TIterator>
  bool operator !=(const etl::move_iterator<TIterator>& lhs,
                   const etl::move_iterator<TIterator>& rhs)
  {
    return !(lhs == rhs);
  }

  template <typename TIterator>
  bool operator <(const etl::move_iterator<TIterator>& lhs,
                  const etl::move_iterator<TIterator>& rhs)
  {
    return lhs.base() < rhs.base();
  }

  template <typename TIterator>
  bool operator <=(const etl::move_iterator<TIterator>& lhs,
                   const etl::move_iterator<TIterator>& rhs)
  {
    return !(rhs < lhs);
  }

  template <typename TIterator>
  bool operator >(const etl::move_iterator<TIterator>& lhs,
                  const etl::move_iterator<TIterator>& rhs)
  {
    return (rhs < lhs);
  }

  template <typename TIterator>
  bool operator >=(const etl::move_iterator<TIterator>& lhs,
                   const etl::move_iterator<TIterator>& rhs)
  {
    return !(lhs < rhs);
  }

  template <typename TIterator>
  move_iterator<TIterator> operator +(typename move_iterator<TIterator>::difference_type n,
                                      const move_iterator<TIterator>& rhs)
  {
    return rhs + n;
  }

  template <typename  TIterator1, typename TIterator2 >
  auto operator -(const move_iterator<TIterator1>& lhs,
                  const move_iterator<TIterator2>& rhs) -> decltype(lhs.base() - rhs.base())
  {
    return lhs.base() - rhs.base();
  }

  template <typename TIterator>
  etl::move_iterator<TIterator> make_move_iterator(TIterator itr)
  {
    return etl::move_iterator<TIterator>(itr);
  }

#endif //ETL_USING_CPP11

  //***************************************************************************
  // back_insert_iterator
  //***************************************************************************

  //***************************************************************************
  /// Turns assignment into push_back
  //***************************************************************************
  template <typename TContainer>
  class back_insert_iterator : public etl::iterator<ETL_OR_STD::output_iterator_tag, void, void, void, void>
  {
  public:

    typedef TContainer container_type;

    //***************************************************************************
    /// Constructor
    //***************************************************************************
    explicit ETL_CONSTEXPR14 back_insert_iterator(TContainer& c)
      : container(etl::addressof(c))
    {
    }

    //***************************************************************************
    /// Assignment operator
    //***************************************************************************
    ETL_CONSTEXPR14 back_insert_iterator& operator =(const typename TContainer::value_type& value)
    {
      container->push_back(value);
      
      return (*this);
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Move assignment operator.
    //***************************************************************************
    ETL_CONSTEXPR14 back_insert_iterator& operator =(typename TContainer::value_type&& value)
    {
      container->push_back(etl::move(value));
      
      return (*this);
    }
#endif  // ETL_USING_CPP11

    //***************************************************************************
    /// Dereference operator.
    //***************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR14 back_insert_iterator& operator *()
    {
      return (*this);
    }

    //***************************************************************************
    /// Pre-increment operator.
    //***************************************************************************
    ETL_CONSTEXPR14 back_insert_iterator& operator ++()
    {
      return (*this);
    }

    //***************************************************************************
    /// Post-increment operator.
    //***************************************************************************
    ETL_CONSTEXPR14 back_insert_iterator operator ++(int)
    {
      return (*this);
    }

  protected:

    TContainer* container;
  };

  //***************************************************************************
  /// Creates a back_insert_iterator from a container.
  //***************************************************************************
  template <typename TContainer>
  ETL_NODISCARD 
  ETL_CONSTEXPR14 
  etl::back_insert_iterator<TContainer> back_inserter(TContainer& container)
  {
    return etl::back_insert_iterator<TContainer>(container);
  }

  //***************************************************************************
  // front_insert_iterator
  //***************************************************************************

  //***************************************************************************
  /// Turns assignment into a push_front.
  //***************************************************************************
  template <typename TContainer>
  class front_insert_iterator : public etl::iterator<ETL_OR_STD::output_iterator_tag, void, void, void, void>
  {
  public:

    typedef TContainer container_type;

    //***************************************************************************
    /// Constructor
    //***************************************************************************
    explicit ETL_CONSTEXPR14 front_insert_iterator(TContainer& c)
      : container(etl::addressof(c))
    {
    }

    //***************************************************************************
    /// Assignment operator
    //***************************************************************************
    ETL_CONSTEXPR14 front_insert_iterator& operator =(const typename TContainer::value_type& value)
    {
      container->push_front(value);
      return (*this);
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Move assignment operator.
    //***************************************************************************
    ETL_CONSTEXPR14 front_insert_iterator& operator =(typename TContainer::value_type&& value)
    {
      container->push_front(etl::move(value));
      return (*this);
    }
#endif  // ETL_USING_CPP11

    //***************************************************************************
    /// Dereference operator.
    //***************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR14 front_insert_iterator& operator *()
    {
      return (*this);
    }

    //***************************************************************************
    /// Pre-increment operator.
    //***************************************************************************
    ETL_CONSTEXPR14 front_insert_iterator& operator ++()
    {
      return (*this);
    }

    //***************************************************************************
    /// Post-increment operator.
    //***************************************************************************
    ETL_CONSTEXPR14 front_insert_iterator operator ++(int)
    {
      return (*this);
    }

  protected:

    TContainer* container;
  };

  //***************************************************************************
  /// Creates a front_insert_iterator from a container.
  //***************************************************************************
  template <typename TContainer>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  etl::front_insert_iterator<TContainer> front_inserter(TContainer& container)
  {
    return etl::front_insert_iterator<TContainer>(container);
  }

  //***************************************************************************
  // push_insert_iterator
  //***************************************************************************

  //***************************************************************************
  /// Turns assignment into a push.
  //***************************************************************************
  template <typename TContainer>
  class push_insert_iterator : public etl::iterator<ETL_OR_STD::output_iterator_tag, void, void, void, void>
  {
  public:

    typedef TContainer container_type;

    //***************************************************************************
    /// Constructor
    //***************************************************************************
    explicit ETL_CONSTEXPR14 push_insert_iterator(TContainer& c)
      : container(etl::addressof(c))
    {
    }

    //***************************************************************************
    /// Assignment operator
    //***************************************************************************
    ETL_CONSTEXPR14 push_insert_iterator& operator =(const typename TContainer::value_type& value)
    {
      container->push(value);

      return (*this);
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Move assignment operator.
    //***************************************************************************
    ETL_CONSTEXPR14 push_insert_iterator& operator =(typename TContainer::value_type&& value)
    {
      container->push(etl::move(value));

      return (*this);
    }
#endif  // ETL_USING_CPP11

    //***************************************************************************
    /// Dereference operator.
    //***************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR14 push_insert_iterator& operator *()
    {
      return (*this);
    }

    //***************************************************************************
    /// Pre-increment operator.
    //***************************************************************************
    ETL_CONSTEXPR14 push_insert_iterator& operator ++()
    {
      return (*this);
    }

    //***************************************************************************
    /// Post-increment operator.
    //***************************************************************************
    ETL_CONSTEXPR14 push_insert_iterator operator ++(int)
    {
      return (*this);
    }

  protected:

    TContainer* container;
  };

  //***************************************************************************
  /// Creates a push_insert_iterator from a container.
  //***************************************************************************
  template <typename TContainer>
  ETL_NODISCARD
    ETL_CONSTEXPR14
    etl::push_insert_iterator<TContainer> push_inserter(TContainer& container)
  {
    return etl::push_insert_iterator<TContainer>(container);
  }

  //***************************************************************************
  // Helper templates.
  //***************************************************************************
  template <typename T>
  struct is_input_iterator
  {
    static ETL_CONSTANT bool value = etl::is_same<typename etl::iterator_traits<T>::iterator_category, ETL_OR_STD::input_iterator_tag>::value;
  };

  template <typename T>
  ETL_CONSTANT bool is_input_iterator<T>::value;

  template <typename T>
  struct is_output_iterator
  {
    static ETL_CONSTANT bool value = etl::is_same<typename etl::iterator_traits<T>::iterator_category, ETL_OR_STD::output_iterator_tag>::value;
  };

  template <typename T>
  ETL_CONSTANT bool is_output_iterator<T>::value;

  template <typename T>
  struct is_forward_iterator
  {
    static ETL_CONSTANT bool value = etl::is_same<typename etl::iterator_traits<T>::iterator_category, ETL_OR_STD::forward_iterator_tag>::value;
  };

  template <typename T>
  ETL_CONSTANT bool is_forward_iterator<T>::value;

  template <typename T>
  struct is_bidirectional_iterator
  {
    static ETL_CONSTANT bool value = etl::is_same<typename etl::iterator_traits<T>::iterator_category, ETL_OR_STD::bidirectional_iterator_tag>::value;
  };

  template <typename T>
  ETL_CONSTANT bool is_bidirectional_iterator<T>::value;

  // Deprecated
  template <typename T>
  struct is_random_iterator
  {
    static ETL_CONSTANT bool value = etl::is_same<typename etl::iterator_traits<T>::iterator_category, ETL_OR_STD::random_access_iterator_tag>::value;
  };

  template <typename T>
  ETL_CONSTANT bool is_random_iterator<T>::value;

  template <typename T>
  struct is_random_access_iterator
  {
    static ETL_CONSTANT bool value = etl::is_same<typename etl::iterator_traits<T>::iterator_category, ETL_OR_STD::random_access_iterator_tag>::value;
  };

  template <typename T>
  ETL_CONSTANT bool is_random_access_iterator<T>::value;

  template <typename T>
  struct is_input_iterator_concept
  {
    static ETL_CONSTANT bool value = etl::is_input_iterator<T>::value ||
                                               etl::is_forward_iterator<T>::value ||
                                               etl::is_bidirectional_iterator<T>::value ||
                                               etl::is_random_iterator<T>::value;
  };

  template <typename T>
  ETL_CONSTANT bool is_input_iterator_concept<T>::value;

  template <typename T>
  struct is_output_iterator_concept
  {
    static ETL_CONSTANT bool value = etl::is_output_iterator<T>::value ||
                                               etl::is_forward_iterator<T>::value ||
                                               etl::is_bidirectional_iterator<T>::value ||
                                               etl::is_random_iterator<T>::value;
  };

  template <typename T>
  ETL_CONSTANT bool is_output_iterator_concept<T>::value;

  template <typename T>
  struct is_forward_iterator_concept
  {
    static ETL_CONSTANT bool value = etl::is_forward_iterator<T>::value ||
                                               etl::is_bidirectional_iterator<T>::value ||
                                               etl::is_random_iterator<T>::value;
  };

  template <typename T>
  ETL_CONSTANT bool is_forward_iterator_concept<T>::value;

  template <typename T>
  struct is_bidirectional_iterator_concept
  {
    static ETL_CONSTANT bool value = etl::is_bidirectional_iterator<T>::value ||
                                               etl::is_random_iterator<T>::value;
  };

  template <typename T>
  ETL_CONSTANT bool is_bidirectional_iterator_concept<T>::value;

  // Deprecated
  template <typename T>
  struct is_random_iterator_concept
  {
    static ETL_CONSTANT bool value = etl::is_random_iterator<T>::value;
  };

  // Deprecated
  template <typename T>
  ETL_CONSTANT bool is_random_iterator_concept<T>::value;

  // Deprecated
  template <typename T>
  struct is_random_access_iterator_concept
  {
    static ETL_CONSTANT bool value = etl::is_random_access_iterator<T>::value;
  };

  // Deprecated
  template <typename T>
  ETL_CONSTANT bool is_random_access_iterator_concept<T>::value;

#if ETL_NOT_USING_STL || ETL_CPP11_NOT_SUPPORTED
  //*****************************************************************************
  /// Get the 'begin' iterator.
  ///\ingroup container
  //*****************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::iterator begin(TContainer& container)
  {
    return container.begin();
  }

  //*****************************************************************************
  /// Get the 'begin' const_iterator for a container.
  ///\ingroup container
  //*****************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::const_iterator begin(const TContainer& container)
  {
    return container.begin();
  }

  //*****************************************************************************
  /// Get the 'begin' const_iterator for a container.
  ///\ingroup container
  //*****************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::const_iterator cbegin(const TContainer& container)
  {
    return container.cbegin();
  }

  //*****************************************************************************
  /// Get the 'end' iterator for a container.
  ///\ingroup container
  //*****************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::iterator end(TContainer& container)
  {
    return container.end();
  }

  //*****************************************************************************
  /// Get the 'end' const_iterator for a container.
  ///\ingroup container
  //*****************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::const_iterator end(const TContainer& container)
  {
    return container.end();
  }

  //*****************************************************************************
  /// Get the 'end' const_iterator for a container.
  ///\ingroup container
  //*****************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::const_iterator cend(const TContainer& container)
  {
    return container.cend();
  }

  //*****************************************************************************
  /// Get the 'begin' pointer for an array.
  ///\ingroup container
  //*****************************************************************************
  template<typename TValue, size_t Array_Size>
  ETL_CONSTEXPR TValue* begin(TValue(&data)[Array_Size])
  {
    return &data[0];
  }

  //*****************************************************************************
  /// Get the 'begin' const iterator for an array.
  ///\ingroup container
  //*****************************************************************************
  template<typename TValue, size_t Array_Size>
  ETL_CONSTEXPR const TValue* begin(const TValue(&data)[Array_Size])
  {
    return &data[0];
  }

  //*****************************************************************************
  /// Get the 'begin' const iterator for an array.
  ///\ingroup container
  //*****************************************************************************
  template<typename TValue, size_t Array_Size>
  ETL_CONSTEXPR const TValue* cbegin(const TValue(&data)[Array_Size])
  {
    return &data[0];
  }

  //*****************************************************************************
  /// Get the 'end' iterator for an array.
  ///\ingroup container
  //*****************************************************************************
  template<typename TValue, size_t Array_Size>
  ETL_CONSTEXPR TValue* end(TValue(&data)[Array_Size])
  {
    return &data[Array_Size];
  }

  //*****************************************************************************
  /// Get the 'end' const iterator for an array.
  ///\ingroup container
  //*****************************************************************************
  template<typename TValue, size_t Array_Size>
  ETL_CONSTEXPR const TValue* end(const TValue(&data)[Array_Size])
  {
    return &data[Array_Size];
  }

  //*****************************************************************************
  /// Get the 'end' const iterator for an array.
  ///\ingroup container
  //*****************************************************************************
  template<typename TValue, size_t Array_Size>
  ETL_CONSTEXPR const TValue* cend(const TValue(&data)[Array_Size])
  {
    return &data[Array_Size];
  }
#endif

#if ETL_NOT_USING_STL || ETL_CPP14_NOT_SUPPORTED
  //*****************************************************************************
  /// Get the 'begin' reverse_iterator for a container.
  ///\ingroup container
  //*****************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::reverse_iterator rbegin(TContainer& container)
  {
    return container.rbegin();
  }

  //*****************************************************************************
  /// Get the 'begin' reverse_iterator for a container.
  ///\ingroup container
  //*****************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::const_reverse_iterator rbegin(const TContainer& container)
  {
    return container.rbegin();
  }

  //*****************************************************************************
  /// Get the 'begin' reverse_iterator for a container.
  ///\ingroup container
  //*****************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::const_reverse_iterator crbegin(const TContainer& container)
  {
    return container.crbegin();
  }

  //*****************************************************************************
  /// Get the 'end' reverse_iterator for a container.
  ///\ingroup container
  //*****************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::reverse_iterator rend(TContainer& container)
  {
    return container.rend();
  }

  //*****************************************************************************
  /// Get the 'end' reverse_iterator for a container.
  ///\ingroup container
  //*****************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::const_reverse_iterator rend(const TContainer& container)
  {
    return container.rend();
  }

  //*****************************************************************************
  /// Get the 'end' reverse_iterator for a container.
  ///\ingroup container
  //*****************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::const_reverse_iterator crend(const TContainer& container)
  {
    return container.crend();
  }

  //*****************************************************************************
  /// Get the 'begin' reverse_iterator for an array.
  ///\ingroup container
  //*****************************************************************************
  template<typename TValue, size_t Array_Size>
  ETL_OR_STD::reverse_iterator<TValue*> rbegin(TValue(&data)[Array_Size])
  {
    return ETL_OR_STD::reverse_iterator<TValue*>(&data[Array_Size]);
  }

  //*****************************************************************************
  /// Get the 'begin' const reverse_iterator for an array.
  ///\ingroup container
  //*****************************************************************************
  template<typename TValue, size_t Array_Size>
  ETL_CONSTEXPR ETL_OR_STD::reverse_iterator<const TValue*> crbegin(const TValue(&data)[Array_Size])
  {
    return ETL_OR_STD::reverse_iterator<const TValue*>(&data[Array_Size]);
  }

  //*****************************************************************************
  /// Get the 'end' reverse_iterator for an array.
  ///\ingroup container
  //*****************************************************************************
  template<typename TValue, size_t Array_Size>
  ETL_CONSTEXPR ETL_OR_STD::reverse_iterator<TValue*> rend(TValue(&data)[Array_Size])
  {
    return ETL_OR_STD::reverse_iterator<TValue*>(&data[0]);
  }

  //*****************************************************************************
  /// Get the 'end' const reverse_iterator for an array.
  ///\ingroup container
  //*****************************************************************************
  template<typename TValue, size_t Array_Size>
  ETL_CONSTEXPR ETL_OR_STD::reverse_iterator<const TValue*> crend(const TValue(&data)[Array_Size])
  {
    return ETL_OR_STD::reverse_iterator<const TValue*>(&data[0]);
  }
#endif

#if ETL_NOT_USING_STL || ETL_CPP17_NOT_SUPPORTED
  //**************************************************************************
  /// Get the size of a container.
  /// Expects the container to have defined 'size_type'.
  ///\ingroup container
  //**************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::size_type size(const TContainer& container)
  {
    return container.size();
  }

  ///**************************************************************************
  /// Get the size of an array in elements at run time, or compile time if C++11 or above.
  ///\ingroup container
  ///**************************************************************************
  template<typename TValue, size_t Array_Size>
  ETL_CONSTEXPR size_t size(TValue(&)[Array_Size])
  {
    return Array_Size;
  }
#endif

  //**************************************************************************
  /// Get the size of an array in elements at compile time for C++03
  ///\code
  /// sizeof(array_size(array))
  ///\endcode
  ///\ingroup container
  //**************************************************************************
  template <typename T, size_t Array_Size>
  char(&array_size(T(&array)[Array_Size]))[Array_Size];

#define ETL_ARRAY_SIZE(a) sizeof(etl::array_size(a))
}

#endif

