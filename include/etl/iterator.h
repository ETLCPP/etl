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
#include "invoke.h"
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
  ETL_CONSTEXPR14 void advance_helper(TIterator& itr, TDistance n, ETL_OR_STD::input_iterator_tag)
  {
    while (n-- > 0)
    {
      ++itr;
    }
  }

  template <typename TIterator, typename TDistance>
  ETL_CONSTEXPR14 void advance_helper(TIterator& itr, TDistance n, ETL_OR_STD::output_iterator_tag)
  {
    while (n-- > 0)
    {
      ++itr;
    }
  }

  template <typename TIterator, typename TDistance>
  ETL_CONSTEXPR14 void advance_helper(TIterator& itr, TDistance n, ETL_OR_STD::forward_iterator_tag)
  {
    while (n-- > 0)
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

#if ETL_USING_CPP17
  template<class T>
  using iter_value_t = typename etl::iterator_traits<etl::remove_cvref_t<T>>::value_type;

  template<class T>
  using iter_reference_t = decltype(*etl::declval<T&>());

#if ETL_USING_CPP20
  template<class T>
  using iter_const_reference_t = typename etl::common_reference_t<const etl::iter_value_t<T>&&, etl::iter_reference_t<T>>;
#endif

  template<class T>
  using iter_difference_t = typename etl::iterator_traits<etl::remove_cvref_t<T>>::difference_type;

  template<class I, class Proj>
  using projected_value_t = etl::remove_cvref_t<etl::invoke_result_t<Proj&, etl::iter_reference_t<I>>>;

  namespace ranges
  {
    namespace private_ranges
    {
      struct begin
      {
        template<class T>
        constexpr auto operator()(T& t) const
        {
          return ETL_OR_STD::begin(t);
        }
      };

      struct end
      {
        template<class T>
        constexpr auto operator()(T& t) const
        {
          return ETL_OR_STD::end(t);
        }
      };

      struct cbegin
      {
        template<class T>
        constexpr auto operator()(T& t) const
        {
          return ETL_OR_STD::cbegin(t);
        }
      };

      struct cend
      {
        template<class T>
        constexpr auto operator()(T& t) const
        {
          return ETL_OR_STD::cend(t);
        }
      };

      struct rbegin
      {
        template<class T>
        constexpr auto operator()(T& t) const
        {
          return ETL_OR_STD::rbegin(t);
        }
      };

      struct rend
      {
        template<class T>
        constexpr auto operator()(T& t) const
        {
          return ETL_OR_STD::rend(t);
        }
      };

      struct crbegin
      {
        template<class T>
        constexpr auto operator()(T& t) const
        {
          return ETL_OR_STD::crbegin(t);
        }
      };

      struct crend
      {
        template<class T>
        constexpr auto operator()(T& t) const
        {
          return ETL_OR_STD::crend(t);
        }
      };

      template<class T, class = void>
      struct has_size_member : etl::false_type {};

      template<class T>
      struct has_size_member<T, etl::void_t<decltype(etl::declval<const T&>().size())>> : etl::true_type {};

      template<class T, class = void>
      struct has_empty_member : etl::false_type {};

      template<class T>
      struct has_empty_member<T, etl::void_t<decltype(etl::declval<const T&>().empty())>> : etl::true_type {};

      struct distance
      {
        // Overload for common ranges (iterator == sentinel type)
        template<typename I, typename = etl::enable_if_t<etl::is_input_iterator_concept<I>::value || etl::is_output_iterator_concept<I>::value>>
        constexpr etl::iter_difference_t<I> operator()(I first, I last) const
        {
          if constexpr (etl::is_random_access_iterator_concept<I>::value)
          {
            return last - first;
          }
          else
          {
            etl::iter_difference_t<I> n = 0;
            while (!(first == last))
            {
              ++first;
              ++n;
            }
            return n;
          }
        }

        // Overload for non-common ranges (iterator != sentinel type)
        template<typename I, typename S, typename = etl::enable_if_t<
          (etl::is_input_iterator_concept<I>::value || etl::is_output_iterator_concept<I>::value) &&
          !etl::is_same<I, S>::value>>
        constexpr etl::iter_difference_t<I> operator()(I first, S last) const
        {
          etl::iter_difference_t<I> n = 0;
          while (!(first == last))
          {
            ++first;
            ++n;
          }
          return n;
        }
      };

      struct size
      {
        template<class T>
        constexpr size_t operator()(T&& t) const
        {
          using U = etl::remove_cvref_t<T>;

          if constexpr (has_size_member<U>::value)
          {
            return static_cast<size_t>(t.size());
          }
          else
          {
            using iter_type = decltype(ETL_OR_STD::begin(t));
            static_assert(etl::is_forward_iterator_concept<iter_type>::value,
              "ranges::size requires a sized range or at least a forward range; "
              "single-pass input ranges are not supported");
            return static_cast<size_t>(distance{}(ETL_OR_STD::begin(t), ETL_OR_STD::end(t)));
          }
        }
      };

      struct ssize
      {
        template<class T>
        constexpr auto operator()(T&& t) const
        {
          using U = etl::remove_cvref_t<T>;

          if constexpr (has_size_member<U>::value)
          {
            return static_cast<ptrdiff_t>(t.size());
          }
          else
          {
            using iter_type = decltype(ETL_OR_STD::begin(t));
            static_assert(etl::is_forward_iterator_concept<iter_type>::value,
              "ranges::ssize requires a sized range or at least a forward range; "
              "single-pass input ranges are not supported");
            return static_cast<ptrdiff_t>(distance{}(ETL_OR_STD::begin(t), ETL_OR_STD::end(t)));
          }
        }
      };

      struct empty
      {
        template<class T>
        constexpr auto operator()(T&& t) const
        {
          using U = etl::remove_cvref_t<T>;

          if constexpr (has_empty_member<U>::value)
          {
            return t.empty();
          }
          else
          {
            return ETL_OR_STD::cbegin(t) == ETL_OR_STD::cend(t);
          }
        }
      };

      struct data
      {
        template<class T>
        constexpr auto operator()(T& t) const
        {
          return ETL_OR_STD::data(t);
        }
      };

      struct cdata
      {
        template<class T>
        constexpr etl::add_pointer_t<etl::add_const_t<etl::remove_pointer_t<decltype(ETL_OR_STD::data(etl::declval<T&>()))>>> operator()(T& t) const
        {
          return ETL_OR_STD::data(t);
        }
      };
    }

    inline constexpr private_ranges::begin begin;
    inline constexpr private_ranges::end end;
    inline constexpr private_ranges::cbegin cbegin;
    inline constexpr private_ranges::cend cend;
    inline constexpr private_ranges::rbegin rbegin;
    inline constexpr private_ranges::rend rend;
    inline constexpr private_ranges::crbegin crbegin;
    inline constexpr private_ranges::crend crend;
    inline constexpr private_ranges::size size;
    inline constexpr private_ranges::ssize ssize;
    inline constexpr private_ranges::empty empty;
    inline constexpr private_ranges::data data;
    inline constexpr private_ranges::cdata cdata;
    inline constexpr private_ranges::distance distance;

    //*************************************************************************
    /// Range primitives.
    //*************************************************************************

    template<class T>
    using iterator_t = decltype(etl::ranges::begin(etl::declval<T&>()));

    template<class T>
    using const_iterator_t = decltype(etl::ranges::cbegin(etl::declval<T&>()));

    template<class T>
    using sentinel_t = decltype(etl::ranges::end(etl::declval<T&>()));

    template<class T>
    using const_sentinel_t = decltype(etl::ranges::cend(etl::declval<T&>()));

    template<class T>
    using range_size_t = decltype(etl::ranges::size(etl::declval<T&>()));

    template<class T>
    using range_difference_t = etl::iter_difference_t<etl::ranges::iterator_t<T>>;

    template<class T>
    using range_value_t = etl::iter_value_t<etl::ranges::iterator_t<T>>;

    template<class T>
    using range_reference_t = etl::iter_reference_t<ranges::iterator_t<T>>;

    struct advance_fn
    {
      template<typename I, typename = etl::enable_if_t<(etl::is_input_iterator_concept<I>::value || etl::is_output_iterator_concept<I>::value) && etl::is_integral<etl::iter_difference_t<I>>::value>>
      constexpr void operator()(I& i, etl::iter_difference_t<I> n) const
      {
        if constexpr (etl::is_random_access_iterator_concept<I>::value)
        {
          i += n;
        }
        else
        {
          while (n > 0)
          {
            --n;
            ++i;
          }

          if constexpr (etl::is_bidirectional_iterator_concept<I>::value)
          {
            while (n < 0)
            {
              ++n;
              --i;
            }
          }
        }
      }

      template<typename I, typename S, typename = etl::enable_if_t<(etl::is_input_iterator_concept<I>::value || etl::is_output_iterator_concept<I>::value) && !etl::is_integral<S>::value>>
      constexpr void operator()(I& i, S bound) const
      {
        if constexpr (etl::is_assignable_v<I&, S>)
        {
          i = etl::move(bound);
        }
        else if constexpr (etl::is_same_v<S, I> && etl::is_random_access_iterator_concept<I>::value)
        {
          (*this)(i, bound - i);
        }
        else
        {
          while (!(i == bound))
          {
            ++i;
          }
        }
      }

      template<typename I, typename S, typename = etl::enable_if_t<etl::is_input_iterator_concept<I>::value || etl::is_output_iterator_concept<I>::value>>
      constexpr etl::iter_difference_t<I>
      operator()(I& i, etl::iter_difference_t<I> n, S bound) const
      {
        if constexpr (etl::is_same_v<S, I> && etl::is_random_access_iterator_concept<I>::value)
        {
          const auto dist = bound - i;

          if ((n >= 0 && dist >= 0 && n >= dist) ||
              (n <= 0 && dist <= 0 && n <= dist))
          {
            (*this)(i, bound);
            return n - dist;
          }

          (*this)(i, n);
          return 0;
        }
        else
        {
          while (n > 0 && !(i == bound))
          {
            --n;
            ++i;
          }

          if constexpr (etl::is_bidirectional_iterator_concept<I>::value)
          {
            while (n < 0 && !(i == bound))
            {
              ++n;
              --i;
            }
          }

          return n;
        }
      }
    };

    inline constexpr auto advance = advance_fn();

    struct prev_fn
    {
      template<typename I, typename = etl::enable_if_t<etl::is_bidirectional_iterator_concept<I>::value>>
      constexpr I operator()(I i) const
      {
        --i;
        return i;
      }

      template<typename I, typename = etl::enable_if_t<etl::is_bidirectional_iterator_concept<I>::value>>
      constexpr I operator()(I i, etl::iter_difference_t<I> n) const
      {
        ranges::advance(i, -n);
        return i;
      }

      template<typename I, typename = etl::enable_if_t<etl::is_bidirectional_iterator_concept<I>::value>>
      constexpr I operator()(I i, etl::iter_difference_t<I> n, I bound) const
      {
        ranges::advance(i, -n, bound);
        return i;
      }
    };

    inline constexpr auto prev = prev_fn();

    struct next_fn
    {
      template<typename I, typename = etl::enable_if_t<etl::is_input_iterator_concept<I>::value || etl::is_output_iterator_concept<I>::value>>
      constexpr I operator()(I i) const
      {
        ++i;
        return i;
      }

      template<typename I, typename = etl::enable_if_t<(etl::is_input_iterator_concept<I>::value || etl::is_output_iterator_concept<I>::value) && etl::is_integral<etl::iter_difference_t<I>>::value>>
      constexpr I operator()(I i, etl::iter_difference_t<I> n) const
      {
        ranges::advance(i, n);
        return i;
      }

      template<typename I, typename S, typename = etl::enable_if_t<(etl::is_input_iterator_concept<I>::value || etl::is_output_iterator_concept<I>::value) && !etl::is_integral<S>::value>>
      constexpr I operator()(I i, S bound) const
      {
        ranges::advance(i, bound);
        return i;
      }

      template<typename I, typename S, typename = etl::enable_if_t<(etl::is_input_iterator_concept<I>::value || etl::is_output_iterator_concept<I>::value) && !etl::is_integral<S>::value>>
      constexpr I operator()(I i, etl::iter_difference_t<I> n, S bound) const
      {
        ranges::advance(i, n, bound);
        return i;
      }
    };

    inline constexpr auto next = next_fn();
  }

  struct unreachable_sentinel_t
  {
  };

  inline constexpr unreachable_sentinel_t unreachable_sentinel{};

  template<typename I>
  constexpr bool operator==(unreachable_sentinel_t, const I&) noexcept
  {
    return false;
  }

  template<typename I>
  constexpr bool operator==(const I&, unreachable_sentinel_t) noexcept
  {
    return false;
  }

  template<typename I>
  constexpr bool operator!=(unreachable_sentinel_t, const I& i) noexcept
  {
    return !(unreachable_sentinel_t{} == i);
  }

  template<typename I>
  constexpr bool operator!=(const I& i, unreachable_sentinel_t) noexcept
  {
    return !(i == unreachable_sentinel_t{});
  }

  struct default_sentinel_t
  {
  };

  inline constexpr default_sentinel_t default_sentinel{};

  namespace private_iterator
  {
    template<typename T, typename = void>
    struct has_arrow_operator : etl::false_type {};

    template<typename T>
    struct has_arrow_operator<T, etl::void_t<decltype(etl::declval<const T&>().operator->())>> : etl::true_type {};

    //***********************************
    /// Proxy that owns a copy of the dereferenced value so that operator->
    /// can safely return a pointer to it. Used when the wrapped iterator
    /// has no member operator-> and is not a raw pointer (i.e. *it may
    /// yield a prvalue / proxy whose address would otherwise dangle).
    //***********************************
    template<typename TValue>
    struct arrow_proxy
    {
      TValue stored;

      constexpr arrow_proxy(TValue value) : stored(etl::move(value)) {}
      constexpr const TValue* operator->() const noexcept { return etl::addressof(stored); }
    };
  }

  template<class I>
  class counted_iterator
  {
      template<class> friend class counted_iterator;

    public:
      using iterator_type = I;
      using value_type = etl::iter_value_t<I>;
      using difference_type = etl::iter_difference_t<I>;
      using iterator_category = typename etl::iterator_traits<iterator_type>::iterator_category;
      using pointer = typename etl::iterator_traits<iterator_type>::pointer;
      using reference = typename etl::iterator_traits<iterator_type>::reference;

      constexpr counted_iterator() = default;

      constexpr counted_iterator(I x, etl::iter_difference_t<I> n): current(etl::move(x)), length(n)
      {
      }

      template<class I2>
      constexpr counted_iterator(const counted_iterator<I2>& other): current(other.current), length(other.length)
      {
      }

      template<class I2>
      constexpr counted_iterator& operator=(const counted_iterator<I2>& other)
      {
        current = other.current;
        length = other.length;
        return *this;
      }

      constexpr const I& base() const& noexcept
      {
        return current;
      }

      constexpr I base() &&
      {
        return etl::move(current);
      }

      constexpr etl::iter_difference_t<I> count() const noexcept
      {
        return length;
      }

      constexpr decltype(auto) operator*() const
      {
        return *current;
      }

      // operator-> for iterator types that provide a member operator->
      template<typename J = I, etl::enable_if_t<
        (etl::is_input_iterator_concept<J>::value || etl::is_output_iterator_concept<J>::value) &&
        private_iterator::has_arrow_operator<J>::value, int> = 0>
      constexpr auto operator->() const noexcept
      {
        return current.operator->();
      }

      // operator-> fallback for raw-pointer iterators (addressof is always safe)
      template<typename J = I, etl::enable_if_t<
        (etl::is_input_iterator_concept<J>::value || etl::is_output_iterator_concept<J>::value) &&
        !private_iterator::has_arrow_operator<J>::value &&
        etl::is_pointer<J>::value, int> = 0>
      constexpr auto operator->() const noexcept
      {
        return current;
      }

      // operator-> fallback for class-type iterators without member operator->
      // When *current yields an lvalue reference, just take its address.
      template<typename J = I, etl::enable_if_t<
        (etl::is_input_iterator_concept<J>::value || etl::is_output_iterator_concept<J>::value) &&
        !private_iterator::has_arrow_operator<J>::value &&
        !etl::is_pointer<J>::value &&
        etl::is_lvalue_reference<decltype(*etl::declval<const J&>())>::value, int> = 0>
      constexpr auto operator->() const noexcept
      {
        return etl::addressof(*current);
      }

      // operator-> fallback for class-type iterators without member operator->
      // When *current yields a prvalue / proxy, use an owning proxy so the
      // address remains valid.
      template<typename J = I, etl::enable_if_t<
        (etl::is_input_iterator_concept<J>::value || etl::is_output_iterator_concept<J>::value) &&
        !private_iterator::has_arrow_operator<J>::value &&
        !etl::is_pointer<J>::value &&
        !etl::is_lvalue_reference<decltype(*etl::declval<const J&>())>::value, int> = 0>
      constexpr auto operator->() const
      {
        return private_iterator::arrow_proxy<value_type>{*current};
      }

      template<typename J = I, etl::enable_if_t<etl::is_random_access_iterator<J>::value, int> = 0>
      constexpr decltype(auto) operator[](etl::iter_difference_t<I> n) const
      {
        return current[n];
      }

      constexpr counted_iterator& operator++()
      {
        ++current;
        --length;
        return *this;
      }

      constexpr counted_iterator operator++(int)
      {
        counted_iterator tmp = *this;
        current++;
        length--;
        return tmp;
      }

      template<typename J = I, etl::enable_if_t<etl::is_random_access_iterator<J>::value, int> = 0>
      constexpr counted_iterator& operator+=(etl::iter_difference_t<I> n)
      {
        current += n;
        length -= n;
        return *this;
      }

      template<typename J = I, etl::enable_if_t<etl::is_random_access_iterator<J>::value, int> = 0>
      constexpr counted_iterator operator+(etl::iter_difference_t<I> n) const
      {
        counted_iterator result{*this};
        result += n;
        return result;
      }

      constexpr counted_iterator& operator--()
      {
        --current;
        ++length;
        return *this;
      }

      constexpr counted_iterator operator--(int)
      {
        counted_iterator tmp = *this;
        current--;
        length++;
        return tmp;
      }

      template<typename J = I, etl::enable_if_t<etl::is_random_access_iterator<J>::value, int> = 0>
      constexpr counted_iterator& operator-=(etl::iter_difference_t<I> n)
      {
        current -= n;
        length += n;
        return *this;
      }

      template<typename J = I, etl::enable_if_t<etl::is_random_access_iterator<J>::value, int> = 0>
      constexpr counted_iterator operator-(etl::iter_difference_t<I> n) const
      {
        counted_iterator result{*this};
        result -= n;
        return result;
      }

      friend constexpr bool operator==(const counted_iterator& x, const counted_iterator& y)
      {
        return x.length == y.length;
      }

      friend constexpr bool operator==(const counted_iterator& x, etl::default_sentinel_t)
      {
        return x.count() == 0;
      }

      friend constexpr bool operator==(etl::default_sentinel_t, const counted_iterator& x)
      {
        return x.count() == 0;
      }

      friend constexpr bool operator!=(const counted_iterator& x, etl::default_sentinel_t)
      {
        return x.count() != 0;
      }

      friend constexpr bool operator!=(etl::default_sentinel_t, const counted_iterator& y)
      {
        return y.count() != 0;
      }

      template<typename J = I, etl::enable_if_t<etl::is_random_access_iterator<J>::value, int> = 0>
      friend constexpr counted_iterator operator+(etl::iter_difference_t<I> n, const counted_iterator& x)
      {
        return counted_iterator(x.current + n, x.length - n);
      }

      friend constexpr etl::iter_difference_t<I> operator-(const counted_iterator& x, const counted_iterator& y)
      {
        return y.length - x.length;
      }

      friend constexpr etl::iter_difference_t<I> operator-(const counted_iterator& x, etl::default_sentinel_t)
      {
        return -x.length;
      }

      friend constexpr etl::iter_difference_t<I> operator-(etl::default_sentinel_t, const counted_iterator& y)
      {
        return y.length;
      }

    private:
      I current{};
      difference_type length{};
  };

  template<typename TIterator, typename = etl::enable_if_t<etl::is_base_of<etl::counted_iterator<typename TIterator::iterator_type>, TIterator>::value>>
  constexpr typename etl::iterator_traits<TIterator>::difference_type distance(TIterator first, etl::default_sentinel_t)
  {
    return first.count();
  }
#endif

#if ETL_USING_CPP14
  template <class T, typename = void>
  struct is_range: etl::false_type
  {
  };

  template <class T>
  struct is_range<T,
    etl::void_t<decltype(ETL_OR_STD::begin(etl::declval<T&>())),
                decltype(ETL_OR_STD::end(etl::declval<T&>()))>>: etl::true_type
  {
  };

#if ETL_USING_CPP17
  template <typename T>
  inline constexpr bool is_range_v = is_range<T>::value;
#endif
#endif

#if ETL_NOT_USING_STL || ETL_CPP17_NOT_SUPPORTED
  //**************************************************************************
  /// Returns a pointer to the block of memory containing the elements of the range.
  ///\ingroup container
  //**************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::pointer data(TContainer& container)
  {
    return container.data();
  }

  //**************************************************************************
  /// Returns a const_pointer to the block of memory containing the elements of the range.
  ///\ingroup container
  //**************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::const_pointer data(const TContainer& container)
  {
    return container.data();
  }

  ///**************************************************************************
  /// Returns a pointer to the block of memory containing the elements of the range.
  ///\ingroup container
  ///**************************************************************************
  template<typename TValue, size_t Array_Size>
  ETL_CONSTEXPR TValue* data(TValue(&a)[Array_Size])
  {
    return a;
  }

  ///**************************************************************************
  /// Returns a const pointer to the block of memory containing the elements of the range.
  ///\ingroup container
  ///**************************************************************************
  template<typename TValue, size_t Array_Size>
  ETL_CONSTEXPR const TValue* data(const TValue(&a)[Array_Size])
  {
    return a;
  }
#endif
}

#endif
