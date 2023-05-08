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

#ifndef ETL_LIST_INCLUDED
#define ETL_LIST_INCLUDED

#include "platform.h"
#include "algorithm.h"
#include "iterator.h"
#include "functional.h"
#include "pool.h"
#include "exception.h"
#include "error_handler.h"
#include "debug_count.h"
#include "nullptr.h"
#include "type_traits.h"
#include "algorithm.h"
#include "memory.h"
#include "iterator.h"
#include "static_assert.h"
#include "parameter_type.h"
#include "placement_new.h"
#include "initializer_list.h"

#include <stddef.h>

#include "private/minmax_push.h"

//*****************************************************************************
///\defgroup list list
/// A linked list with the capacity defined at compile time.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// Exception for the list.
  ///\ingroup list
  //***************************************************************************
  class list_exception : public exception
  {
  public:

    list_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Full exception for the list.
  ///\ingroup list
  //***************************************************************************
  class list_full : public list_exception
  {
  public:

    list_full(string_type file_name_, numeric_type line_number_)
      : list_exception(ETL_ERROR_TEXT("list:full", ETL_LIST_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Empty exception for the list.
  ///\ingroup list
  //***************************************************************************
  class list_empty : public list_exception
  {
  public:

    list_empty(string_type file_name_, numeric_type line_number_)
      : list_exception(ETL_ERROR_TEXT("list:empty", ETL_LIST_FILE_ID"B"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Iterator exception for the list.
  ///\ingroup list
  //***************************************************************************
  class list_iterator : public list_exception
  {
  public:

    list_iterator(string_type file_name_, numeric_type line_number_)
      : list_exception(ETL_ERROR_TEXT("list:iterator", ETL_LIST_FILE_ID"C"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Unsorted exception for the list.
  ///\ingroup list
  //***************************************************************************
  class list_unsorted : public list_exception
  {
  public:

    list_unsorted(string_type file_name_, numeric_type line_number_)
      : list_exception(ETL_ERROR_TEXT("list:unsorted", ETL_LIST_FILE_ID"D"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Unsorted exception for the list.
  ///\ingroup list
  //***************************************************************************
  class list_no_pool : public list_exception
  {
  public:

    list_no_pool(string_type file_name_, numeric_type line_number_)
      : list_exception(ETL_ERROR_TEXT("list:no pool", ETL_LIST_FILE_ID"E"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The base class for all lists.
  ///\ingroup list
  //***************************************************************************
  class list_base
  {
  public:

    typedef size_t size_type; ///< The type used for determining the size of list.

    //*************************************************************************
    /// The node element in the list.
    //*************************************************************************
    struct node_t
    {
      //***********************************************************************
      /// Constructor
      //***********************************************************************
      node_t()
        : previous(ETL_NULLPTR),
          next(ETL_NULLPTR)
      {
      }

      //***********************************************************************
      /// Reverses the previous & next pointers.
      //***********************************************************************
      void reverse()
      {
        using ETL_OR_STD::swap; // Allow ADL

        swap(previous, next);
      }

      node_t* previous;
      node_t* next;
    };

    //*************************************************************************
    /// <b>true</b> if the list has a shared pool.
    //*************************************************************************
    bool has_shared_pool() const
    {
      return pool_is_shared;
    }

    //*************************************************************************
    /// Reverses the list.
    //*************************************************************************
    void reverse()
    {
      if (is_trivial_list())
      {
        return;
      }

      node_t* p_node = terminal_node.next;

      while (p_node != &terminal_node)
      {
        node_t* p_temp = p_node->previous;
        p_node->previous = p_node->next;
        p_node->next = p_temp;
        p_node = p_node->previous;
      }

      // Terminal node.
      node_t* p_temp = p_node->previous;
      p_node->previous = p_node->next;
      p_node->next = p_temp;
    }

    //*************************************************************************
    /// Gets the maximum possible size of the list.
    //*************************************************************************
    size_type max_size() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Gets the maximum possible size of the list.
    //*************************************************************************
    size_type capacity() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Gets the size of the list.
    //*************************************************************************
    size_type size() const
    {
      if (has_shared_pool())
      {
        // We have to count what we actually own.
        size_type count = 0U;

        node_t* p_node = terminal_node.next;

        while (p_node != &terminal_node)
        {
          ++count;
          p_node = p_node->next;
        }

        return count;
      }
      else
      {
        return p_node_pool->size();
      }
    }

    //*************************************************************************
    /// Checks to see if the list is empty.
    //*************************************************************************
    bool empty() const
    {
      return (terminal_node.next == &terminal_node);
    }

    //*************************************************************************
    /// Checks to see if the list is full.
    //*************************************************************************
    bool full() const
    {
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));
      return p_node_pool->full();
    }

    //*************************************************************************
    /// Returns the remaining capacity.
    ///\return The remaining capacity.
    //*************************************************************************
    size_type available() const
    {
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));
      return p_node_pool->available();
    }

  protected:

    //*************************************************************************
    /// Is the list a trivial length?
    //*************************************************************************
    bool is_trivial_list() const
    {
      return (size() < 2);
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    node_t& get_head()
    {
      return *terminal_node.next;
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    const node_t& get_head() const
    {
      return *terminal_node.next;
    }

    //*************************************************************************
    /// Get the tail node.
    //*************************************************************************
    node_t& get_tail()
    {
      return *terminal_node.previous;
    }

    //*************************************************************************
    /// Get the tail node.
    //*************************************************************************
    const node_t& get_tail() const
    {
      return *terminal_node.previous;
    }

    //*************************************************************************
    /// Insert a node before 'position'.
    //*************************************************************************
    void insert_node(node_t& position, node_t& node)
    {
      // Connect to the list.
      join(*position.previous, node);
      join(node, position);
    }

    //*************************************************************************
    /// Join two nodes.
    //*************************************************************************
    void join(node_t& left, node_t& right)
    {
      left.next      = &right;
      right.previous = &left;
    }

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    explicit list_base(bool pool_is_shared_)
      : p_node_pool(ETL_NULLPTR),
        MAX_SIZE(0),
        pool_is_shared(pool_is_shared_)
    {
      join(terminal_node, terminal_node);
    }

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    list_base(etl::ipool& node_pool_, size_type   max_size_, bool pool_is_shared_)
      : p_node_pool(&node_pool_),
        MAX_SIZE(max_size_),
        pool_is_shared(pool_is_shared_)
    {
      join(terminal_node, terminal_node);
    }

    //*************************************************************************
    /// Set the node pool instance.
    //*************************************************************************
    void set_node_pool(etl::ipool& node_pool_)
    {
      p_node_pool = &node_pool_;
      MAX_SIZE    = p_node_pool->max_size();
    }

    //*************************************************************************
    /// Get the node pool instance.
    //*************************************************************************
    etl::ipool* get_node_pool()
    {
      return p_node_pool;
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~list_base()
    {
    }

    etl::ipool* p_node_pool;     ///< The pool of data nodes used in the list.
    node_t      terminal_node;   ///< The node that acts as the list start and end.
    size_type   MAX_SIZE;        ///< The maximum size of the list.
    bool        pool_is_shared;  ///< If <b>true</b> then the pool is shared between lists.
    ETL_DECLARE_DEBUG_COUNT      ///< Internal debugging.
  };

  //***************************************************************************
  /// A templated base for all etl::list types.
  ///\ingroup list
  //***************************************************************************
  template <typename T>
  class ilist : public etl::list_base
  {
  public:

    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
#if ETL_USING_CPP11
    typedef T&&      rvalue_reference;
#endif
    typedef size_t   size_type;

  protected:

    typedef typename etl::parameter_type<T>::type parameter_t;

    //*************************************************************************
    /// The data node element in the list.
    //*************************************************************************
    struct data_node_t : public node_t
    {
      explicit data_node_t(const T& value_)
        : value(value_)
      {
      }

      T value;
    };

  private:

    //*************************************************************************
    /// Downcast a node_t* to a data_node_t*
    //*************************************************************************
    static data_node_t* data_cast(node_t* p_node)
    {
      return reinterpret_cast<data_node_t*>(p_node);
    }

    //*************************************************************************
    /// Downcast a node_t& to a data_node_t&
    //*************************************************************************
    static data_node_t& data_cast(node_t& node)
    {
      return reinterpret_cast<data_node_t&>(node);
    }

    //*************************************************************************
    /// Downcast a const node_t* to a const data_node_t*
    //*************************************************************************
    static const data_node_t* data_cast(const node_t* p_node)
    {
      return reinterpret_cast<const data_node_t*>(p_node);
    }

    //*************************************************************************
    /// Downcast a const node_t& to a const data_node_t&
    //*************************************************************************
    static const data_node_t& data_cast(const node_t& node)
    {
      return reinterpret_cast<const data_node_t&>(node);
    }

  public:

    //*************************************************************************
    /// iterator.
    //*************************************************************************
    class iterator : public etl::iterator<ETL_OR_STD::bidirectional_iterator_tag, T>
    {
    public:

      friend class ilist;
      friend class const_iterator;

      iterator()
        : p_node(ETL_NULLPTR)
      {
      }

      iterator(node_t& node)
        : p_node(&node)
      {
      }

      iterator(const iterator& other)
        : p_node(other.p_node)
      {
      }

      iterator& operator ++()
      {
        p_node = p_node->next;
        return *this;
      }

      iterator operator ++(int)
      {
        iterator temp(*this);
        p_node = p_node->next;
        return temp;
      }

      iterator& operator --()
      {
        p_node = p_node->previous;
        return *this;
      }

      iterator operator --(int)
      {
        iterator temp(*this);
        p_node = p_node->previous;
        return temp;
      }

      iterator& operator =(const iterator& other)
      {
        p_node = other.p_node;
        return *this;
      }

      reference operator *() const
      {
        return ilist::data_cast(p_node)->value;
      }

      pointer operator &() const
      {
        return &(ilist::data_cast(p_node)->value);
      }

      pointer operator ->() const
      {
        return &(ilist::data_cast(p_node)->value);
      }

      friend bool operator == (const iterator& lhs, const iterator& rhs)
      {
        return lhs.p_node == rhs.p_node;
      }

      friend bool operator != (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      node_t* p_node;
    };

    //*************************************************************************
    /// const_iterator
    //*************************************************************************
    class const_iterator : public etl::iterator<ETL_OR_STD::bidirectional_iterator_tag, const T>
    {
    public:

      friend class ilist;

      const_iterator()
        : p_node(ETL_NULLPTR)
      {
      }

      const_iterator(node_t& node)
        : p_node(&node)
      {
      }

      const_iterator(const node_t& node)
        : p_node(&node)
      {
      }

      const_iterator(const typename ilist::iterator& other)
        : p_node(other.p_node)
      {
      }

      const_iterator(const const_iterator& other)
        : p_node(other.p_node)
      {
      }

      const_iterator& operator ++()
      {
        p_node = p_node->next;
        return *this;
      }

      const_iterator operator ++(int)
      {
        const_iterator temp(*this);
        p_node = p_node->next;
        return temp;
      }

      const_iterator& operator --()
      {
        p_node = p_node->previous;
        return *this;
      }

      const_iterator operator --(int)
      {
        const_iterator temp(*this);
        p_node = p_node->previous;
        return temp;
      }

      const_iterator& operator =(const const_iterator& other)
      {
        p_node = other.p_node;
        return *this;
      }

      const_reference operator *() const
      {
        return ilist::data_cast(p_node)->value;
      }

      const_pointer operator &() const
      {
        return &(ilist::data_cast(p_node)->value);
      }

      const_pointer operator ->() const
      {
        return &(ilist::data_cast(p_node)->value);
      }

      friend bool operator == (const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.p_node == rhs.p_node;
      }

      friend bool operator != (const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      const node_t* p_node;
    };

    typedef typename etl::iterator_traits<iterator>::difference_type difference_type;

    typedef ETL_OR_STD::reverse_iterator<iterator>       reverse_iterator;
    typedef ETL_OR_STD::reverse_iterator<const_iterator> const_reverse_iterator;

    //*************************************************************************
    /// Gets the beginning of the list.
    //*************************************************************************
    iterator begin()
    {
      return iterator(get_head());
    }

    //*************************************************************************
    /// Gets the beginning of the list.
    //*************************************************************************
    const_iterator begin() const
    {
      return const_iterator(get_head());
    }

    //*************************************************************************
    /// Gets the end of the list.
    //*************************************************************************
    iterator end()
    {
      return iterator(terminal_node);
    }

    //*************************************************************************
    /// Gets the end of the list.
    //*************************************************************************
    const_iterator end() const
    {
      return const_iterator(terminal_node);
    }

    //*************************************************************************
    /// Gets the beginning of the list.
    //*************************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(get_head());
    }

    //*************************************************************************
    /// Gets the end of the list.
    //*************************************************************************
    const_iterator cend() const
    {
      return const_iterator(terminal_node);
    }

    //*************************************************************************
    /// Gets the reverse beginning of the list.
    //*************************************************************************
    reverse_iterator rbegin()
    {
      return reverse_iterator(terminal_node);
    }

    //*************************************************************************
    /// Gets the reverse beginning of the list.
    //*************************************************************************
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(terminal_node);
    }

    //*************************************************************************
    /// Gets the reverse end of the list.
    //*************************************************************************
    reverse_iterator rend()
    {
      return reverse_iterator(get_head());
    }

    //*************************************************************************
    /// Gets the reverse end of the list.
    //*************************************************************************
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(get_head());
    }

    //*************************************************************************
    /// Gets the reverse beginning of the list.
    //*************************************************************************
    const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(terminal_node);
    }

    //*************************************************************************
    /// Gets the reverse end of the list.
    //*************************************************************************
    const_reverse_iterator crend() const
    {
      return const_reverse_iterator(get_head());
    }

    //*************************************************************************
    /// Gets a reference to the first element.
    //*************************************************************************
    reference front()
    {
      return data_cast(get_head()).value;
    }

    //*************************************************************************
    /// Gets a const reference to the first element.
    //*************************************************************************
    const_reference front() const
    {
      return data_cast(get_head()).value;
    }

    //*************************************************************************
    /// Gets a reference to the last element.
    //*************************************************************************
    reference back()
    {
      return data_cast(get_tail()).value;
    }

    //*************************************************************************
    /// Gets a reference to the last element.
    //*************************************************************************
    const_reference back() const
    {
      return data_cast(get_tail()).value;
    }

    //*************************************************************************
    /// Assigns a range of values to the list.
    /// If asserts or exceptions are enabled throws etl::list_full if the list does not have enough free space.
    /// If ETL_THROW_EXCEPTIONS & ETL_DEBUG are defined throws list_iterator if the iterators are reversed.
    //*************************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last, typename etl::enable_if<!etl::is_integral<TIterator>::value, int>::type = 0)
    {
#if ETL_IS_DEBUG_BUILD
      difference_type d = etl::distance(first, last);
      ETL_ASSERT(d >= 0, ETL_ERROR(list_iterator));
      ETL_ASSERT(size_t(d) <= MAX_SIZE, ETL_ERROR(list_full));
#endif
      initialise();

      // Add all of the elements.
      while (first != last)
      {
        data_node_t& node = allocate_data_node(*first);
        join(get_tail(), node);
        join(node, terminal_node);
        ++first;
      }
    }

    //*************************************************************************
    /// Assigns 'n' copies of a value to the list.
    //*************************************************************************
    void assign(size_t n, const T& value)
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(n <= MAX_SIZE, ETL_ERROR(list_full));
#endif

      initialise();

      // Add all of the elements.
      while (n-- > 0)
      {
        data_node_t& node = allocate_data_node(value);
        join(*terminal_node.previous, node);
        join(node, terminal_node);
      }
    }

    //*************************************************************************
    /// Pushes a value to the front of the list.
    //*************************************************************************
    void push_front(const T& value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
#endif
      insert_node(get_head(), allocate_data_node(value));
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Pushes a value to the front of the list.
    //*************************************************************************
    void push_front(rvalue_reference value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
#endif
      insert_node(get_head(), allocate_data_node(etl::move(value)));
    }
#endif

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT
    //*************************************************************************
    /// Emplaces a value to the front of the list.
    //*************************************************************************
    template <typename ... Args>
    reference emplace_front(Args && ... args)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
#endif
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));

      data_node_t* p_data_node = create_data_node();
      ::new (&(p_data_node->value)) T(etl::forward<Args>(args)...);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node(get_head(), *p_data_node);
      return front();
    }
#else
    //*************************************************************************
    /// Emplaces a value to the front of the list.
    //*************************************************************************
    template <typename T1>
    reference emplace_front(const T1& value1)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
#endif
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));

      data_node_t* p_data_node = create_data_node();
      ::new (&(p_data_node->value)) T(value1);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node(get_head(), *p_data_node);
      return front();
    }

    //*************************************************************************
    /// Emplaces a value to the front of the list.
    //*************************************************************************
    template <typename T1, typename T2>
    reference emplace_front(const T1& value1, const T2& value2)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
#endif
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));

      data_node_t* p_data_node = create_data_node();
      ::new (&(p_data_node->value)) T(value1, value2);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node(get_head(), *p_data_node);
      return front();
    }

    //*************************************************************************
    /// Emplaces a value to the front of the list.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    reference emplace_front(const T1& value1, const T2& value2, const T3& value3)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
#endif
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));

      data_node_t* p_data_node = create_data_node();
      ::new (&(p_data_node->value)) T(value1, value2, value3);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node(get_head(), *p_data_node);
      return front();
    }

    //*************************************************************************
    /// Emplaces a value to the front of the list.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    reference emplace_front(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
#endif
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));

      data_node_t* p_data_node = create_data_node();
      ::new (&(p_data_node->value)) T(value1, value2, value3, value4);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node(get_head(), *p_data_node);
      return front();
    }
#endif

    //*************************************************************************
    /// Removes a value from the front of the list.
    //*************************************************************************
    void pop_front()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!empty(), ETL_ERROR(list_empty));
#endif
      node_t& node = get_head();
      remove_node(node);
    }

    //*************************************************************************
    /// Pushes a value to the back of the list.
    //*************************************************************************
    void push_back(const T& value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
#endif
      insert_node(terminal_node, allocate_data_node(value));
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Pushes a value to the back of the list.
    //*************************************************************************
    void push_back(rvalue_reference value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
#endif
      insert_node(terminal_node, allocate_data_node(etl::move(value)));
    }
#endif

    //*************************************************************************
    /// Emplaces a value to the back of the list.
    //*************************************************************************
#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT
    template <typename ... Args>
    reference emplace_back(Args && ... args)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
#endif
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));

      data_node_t* p_data_node = create_data_node();
      ::new (&(p_data_node->value)) T(etl::forward<Args>(args)...);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node(terminal_node, *p_data_node);
      return back();
    }
#else
    template <typename T1>
    reference emplace_back(const T1& value1)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
#endif
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));

      data_node_t* p_data_node = create_data_node();
      ::new (&(p_data_node->value)) T(value1);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node(terminal_node, *p_data_node);
      return back();
    }

    template <typename T1, typename T2>
    reference emplace_back(const T1& value1, const T2& value2)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
#endif
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));

      data_node_t* p_data_node = create_data_node();
      ::new (&(p_data_node->value)) T(value1, value2);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node(terminal_node, *p_data_node);
      return back();
    }

    template <typename T1, typename T2, typename T3>
    reference emplace_back(const T1& value1, const T2& value2, const T3& value3)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
#endif
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));

      data_node_t* p_data_node = create_data_node();
      ::new (&(p_data_node->value)) T(value1, value2, value3);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node(terminal_node, *p_data_node);
      return back();
    }

    template <typename T1, typename T2, typename T3, typename T4>
    reference emplace_back(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
#endif
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));

      data_node_t* p_data_node = create_data_node();
      ::new (&(p_data_node->value)) T(value1, value2, value3, value4);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node(terminal_node, *p_data_node);
      return back();
    }
#endif

    //*************************************************************************
    /// Removes a value from the back of the list.
    //*************************************************************************
    void pop_back()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!empty(), ETL_ERROR(list_empty));
#endif
      node_t& node = get_tail();
      remove_node(node);
    }

    //*************************************************************************
    /// Inserts a value to the list at the specified position.
    //*************************************************************************
    iterator insert(const_iterator position, const_reference value)
    {
      ETL_ASSERT(!full(), ETL_ERROR(list_full));

      data_node_t& data_node = allocate_data_node(value);
      insert_node(*to_iterator(position).p_node, data_node);

      return iterator(data_node);
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Inserts a value to the list at the specified position.
    //*************************************************************************
    iterator insert(const_iterator position, rvalue_reference value)
    {
      ETL_ASSERT(!full(), ETL_ERROR(list_full));

      data_node_t& data_node = allocate_data_node(etl::move(value));
      insert_node(*to_iterator(position).p_node, data_node);

      return iterator(data_node);
    }
#endif

    //*************************************************************************
    /// Emplaces a value to the list at the specified position.
    //*************************************************************************
#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT
    template <typename ... Args>
    iterator emplace(const_iterator position, Args && ... args)
    {
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));

      data_node_t* p_data_node = create_data_node();
      ::new (&(p_data_node->value)) T(etl::forward<Args>(args)...);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node(*to_iterator(position).p_node, *p_data_node);

      return iterator(*p_data_node);
    }
#else
    template <typename T1>
    iterator emplace(const_iterator position, const T1& value1)
    {
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));

      data_node_t* p_data_node = create_data_node();
      ::new (&(p_data_node->value)) T(value1);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node(*position.p_node, *p_data_node);

      return iterator(*p_data_node);
    }

    template <typename T1, typename T2>
    iterator emplace(const_iterator position, const T1& value1, const T2& value2)
    {
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));

      data_node_t* p_data_node = create_data_node();
      ::new (&(p_data_node->value)) T(value1, value2);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node(*position.p_node, *p_data_node);

      return iterator(*p_data_node);
    }

    template <typename T1, typename T2, typename T3>
    iterator emplace(const_iterator position, const T1& value1, const T2& value2, const T3& value3)
    {
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));

      data_node_t* p_data_node = create_data_node();
      ::new (&(p_data_node->value)) T(value1, value2, value3);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node(*position.p_node, *p_data_node);

      return iterator(*p_data_node);
    }

    template <typename T1, typename T2, typename T3, typename T4>
    iterator emplace(const_iterator position, const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      ETL_ASSERT(!full(), ETL_ERROR(list_full));
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));

      data_node_t* p_data_node = create_data_node();
      ::new (&(p_data_node->value)) T(value1, value2, value3, value4);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node(*position.p_node, *p_data_node);

      return iterator(*p_data_node);
    }
#endif

    //*************************************************************************
    /// Inserts 'n' copies of a value to the list at the specified position.
    //*************************************************************************
    void insert(const_iterator position, size_t n, const_reference value)
    {
      for (size_t i = 0UL; i < n; ++i)
      {
        ETL_ASSERT(!full(), ETL_ERROR(list_full));

        // Set up the next free node and insert.
        insert_node(*to_iterator(position).p_node, allocate_data_node(value));
      }
    }

    //*************************************************************************
    /// Inserts a range of values to the list at the specified position.
    //*************************************************************************
    template <typename TIterator>
    void insert(const_iterator position, TIterator first, TIterator last, typename etl::enable_if<!etl::is_integral<TIterator>::value, int>::type = 0)
    {
      while (first != last)
      {
        ETL_ASSERT(!full(), ETL_ERROR(list_full));

        // Set up the next free node and insert.
        insert_node(*to_iterator(position).p_node, allocate_data_node(*first));
        ++first;
      }
    }

    //*************************************************************************
    /// Erases the value at the specified position.
    //*************************************************************************
    iterator erase(const_iterator position)
    {
      iterator position_ = to_iterator(position);

      ++position_;
      remove_node(*position_.p_node->previous);
      return position_;
    }

    //*************************************************************************
    /// Erases a range of elements.
    //*************************************************************************
    iterator erase(const_iterator first, const_iterator last)
    {
      iterator first_ = to_iterator(first);
      iterator last_  = to_iterator(last);

      node_t* p_first = first_.p_node;
      node_t* p_last  = last_.p_node;
      node_t* p_next;

      // Join the ends.
      join(*(p_first->previous), *p_last);

      // Erase the ones in between.
      while (p_first != p_last)
      {
        p_next = p_first->next;                                 // Remember the next node.
        destroy_data_node(static_cast<data_node_t&>(*p_first)); // Destroy the current node.
        p_first = p_next;                                       // Move to the next node.
      }

      return last_;
    }

    //*************************************************************************
    /// Resizes the list.
    //*************************************************************************
    void resize(size_t n)
    {
      resize(n, T());
    }

    //*************************************************************************
    /// Resizes the list.
    //*************************************************************************
    void resize(size_t n, const_reference value)
    {
      ETL_ASSERT(n <= MAX_SIZE, ETL_ERROR(list_full));

      // Zero?
      if (n == 0U)
      {
        clear();
      }
      // Smaller?
      else if (n < size())
      {
        iterator i_start = end();
        etl::advance(i_start, -difference_type(size() - n));
        erase(i_start, end());
      }
      // Larger?
      else if (n > size())
      {
        insert(end(), n - size(), value);
      }
    }

    //*************************************************************************
    /// Clears the list.
    //*************************************************************************
    void clear()
    {
      initialise();
    }

    //*************************************************************************
    // Removes the values specified.
    //*************************************************************************
    void remove(const_reference value)
    {
      iterator iValue = begin();

      while (iValue != end())
      {
        if (value == *iValue)
        {
          iValue = erase(iValue);
        }
        else
        {
          ++iValue;
        }
      }
    }

    //*************************************************************************
    /// Removes according to a predicate.
    //*************************************************************************
    template <typename TPredicate>
    void remove_if(TPredicate predicate)
    {
      iterator iValue = begin();

      while (iValue != end())
      {
        if (predicate(*iValue))
        {
          iValue = erase(iValue);
        }
        else
        {
          ++iValue;
        }
      }
    }

    //*************************************************************************
    /// Removes all but the first element from every consecutive group of equal
    /// elements in the container.
    //*************************************************************************
    void unique()
    {
      unique(etl::equal_to<T>());
    }

    //*************************************************************************
    /// Removes all but the first element from every consecutive group of equal
    /// elements in the container.
    //*************************************************************************
    template <typename TIsEqual>
    void unique(TIsEqual isEqual)
    {
      if (empty())
      {
        return;
      }

      iterator i_item = begin();
      ++i_item;
      iterator i_previous = begin();

      while (i_item != end())
      {
        if (isEqual(*i_previous, *i_item))
        {
          i_item = erase(i_item);
        }
        else
        {
          i_previous = i_item;
          ++i_item;
        }
      }
    }

    //*************************************************************************
    /// Splices from another list to this.
    //*************************************************************************
    void splice(iterator to, ilist& other)
    {
      if (&other != this)
      {
        insert(to, other.begin(), other.end());
        other.erase(other.begin(), other.end());
      }
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Splices from another list to this.
    //*************************************************************************
    void splice(iterator to, ilist&& other)
    {
      if (&other != this)
      {
        typename ilist<T>::iterator itr = other.begin();
        while (itr != other.end())
        {
          to = insert(to, etl::move(*itr));
          ++itr;
        }

        other.erase(other.begin(), other.end());
      }
    }
#endif

    //*************************************************************************
    /// Splices an element from another list to this.
    //*************************************************************************
    void splice(iterator to, ilist& other, iterator from)
    {
      if (&other == this)
      {
        // Internal move.
        move(to, from);
      }
      else
      {
        // From another list.
        insert(to, *from);
        other.erase(from);
      }
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Splices an element from another list to this.
    //*************************************************************************
    void splice(iterator to, ilist&& other, iterator from)
    {
      if (&other == this)
      {
        // Internal move.
        move(to, from);
      }
      else
      {
        // From another list.
        insert(to, etl::move(*from));
        other.erase(from);
      }
    }
#endif

    //*************************************************************************
    /// Splices a range of elements from another list to this.
    //*************************************************************************
    void splice(iterator to, ilist& other, iterator first, iterator last)
    {
      if (&other == this)
      {
        // Internal move.
        move(to, first, last);
      }
      else
      {
        // From another list.
        insert(to, first, last);
        other.erase(first, last);
      }
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Splices a range of elements from another list to this.
    //*************************************************************************
    void splice(iterator to, ilist&& other, iterator first, iterator last)
    {
      if (&other == this)
      {
        // Internal move.
        move(to, first, last);
      }
      else
      {
        // From another list.
        ilist::iterator itr = first;
        while (itr != last)
        {
          to = insert(to, etl::move(*itr));
          ++itr;
          ++to;
        }

        other.erase(first, last);
      }
    }
#endif

    //*************************************************************************
    /// Merge another list into this one. Both lists should be sorted.
    //*************************************************************************
    void merge(ilist& other)
    {
      merge(other, etl::less<value_type>());
    }

    //*************************************************************************
    /// Merge another list into this one. Both lists should be sorted.
    //*************************************************************************
    template <typename TCompare>
    void merge(ilist& other, TCompare compare)
    {
      if ((this != &other) && !other.empty())
      {
#if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(etl::is_sorted(other.begin(), other.end(), compare), ETL_ERROR(list_unsorted));
        ETL_ASSERT(etl::is_sorted(begin(), end(), compare), ETL_ERROR(list_unsorted));
#endif

        ilist::iterator other_begin = other.begin();
        ilist::iterator other_end = other.end();

        ilist::iterator this_begin = begin();
        ilist::iterator this_end = end();

        while ((this_begin != this_end) && (other_begin != other_end))
        {
          // Find the place to insert.
          while ((this_begin != this_end) && !(compare(*other_begin, *this_begin)))
          {
            ++this_begin;
          }

          // Insert.
          if (this_begin != this_end)
          {
            while ((other_begin != other_end) && (compare(*other_begin, *this_begin)))
            {
              insert(this_begin, *other_begin);
              ++other_begin;
            }
          }
        }

        // Any left over?
        if ((this_begin == this_end) && (other_begin != other_end))
        {
          insert(this_end, other_begin, other_end);
        }

        other.clear();
      }
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Merge another list into this one. Both lists should be sorted.
    //*************************************************************************
    void merge(ilist&& other)
    {
      merge(etl::move(other), etl::less<value_type>());
    }

    //*************************************************************************
    /// Merge another list into this one. Both lists should be sorted.
    //*************************************************************************
    template <typename TCompare>
    void merge(ilist&& other, TCompare compare)
    {
      if (!other.empty())
      {
#if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(etl::is_sorted(other.begin(), other.end(), compare), ETL_ERROR(list_unsorted));
        ETL_ASSERT(etl::is_sorted(begin(), end(), compare), ETL_ERROR(list_unsorted));
#endif

        ilist::iterator other_begin = other.begin();
        ilist::iterator other_end = other.end();

        ilist::iterator this_begin = begin();
        ilist::iterator this_end = end();

        while ((this_begin != this_end) && (other_begin != other_end))
        {
          // Find the place to insert.
          while ((this_begin != this_end) && !(compare(*other_begin, *this_begin)))
          {
            ++this_begin;
          }

          // Insert.
          if (this_begin != this_end)
          {
            while ((other_begin != other_end) && (compare(*other_begin, *this_begin)))
            {
              insert(this_begin, etl::move(*other_begin));
              ++other_begin;
            }
          }
        }

        // Any left over?
        if ((this_begin == this_end) && (other_begin != other_end))
        {
          while (other_begin != other_end)
          {
            insert(this_end, etl::move(*other_begin));
            ++other_begin;
          }
        }

        other.clear();
      }
    }
#endif

    //*************************************************************************
    /// Sort using in-place merge sort algorithm.
    /// Uses 'less-than operator as the predicate.
    //*************************************************************************
    void sort()
    {
      sort(etl::less<T>());
    }

    //*************************************************************************
    /// Stable sort using in-place merge sort algorithm.
    /// Copyright 2001 Simon Tatham.
    ///
    /// Permission is hereby granted, free of charge, to any person
    /// obtaining a copy of this software and associated documentation
    /// files (the "Software"), to deal in the Software without
    /// restriction, including without limitation the rights to use,
    /// copy, modify, merge, publish, distribute, sublicense, and/or
    /// sell copies of the Software, and to permit persons to whom the
    /// Software is furnished to do so, subject to the following
    /// conditions:
    ///
    /// The above copyright notice and this permission notice shall be
    /// included in all copies or substantial portions of the Software.
    ///
    /// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    /// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    /// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    /// NONINFRINGEMENT.  IN NO EVENT SHALL SIMON TATHAM BE LIABLE FOR
    /// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
    /// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    /// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    /// SOFTWARE.
    //*************************************************************************
    template <typename TCompare>
    void sort(TCompare compare)
    {
      iterator i_left;
      iterator i_right;
      iterator i_node;
      iterator i_head;
      iterator i_tail;
      int   list_size = 1;
      int   number_of_merges;
      int   left_size;
      int   right_size;

      if (is_trivial_list())
      {
        return;
      }

      while (true)
      {
        i_left = begin();
        i_head = end();
        i_tail = end();

        number_of_merges = 0;  // Count the number of merges we do in this pass.

        while (i_left != end())
        {
          ++number_of_merges;  // There exists a merge to be done.
          i_right = i_left;
          left_size = 0;

          // Step 'list_size' places along from left
          for (int i = 0; i < list_size; ++i)
          {
            ++left_size;
            ++i_right;

            if (i_right == end())
            {
              break;
            }
          }

          // If right hasn't fallen off end, we have two lists to merge.
          right_size = list_size;

          // Now we have two lists. Merge them.
          while (left_size > 0 || (right_size > 0 && i_right != end()))
          {
            // Decide whether the next node of merge comes from left or right.
            if (left_size == 0)
            {
              // Left is empty. The node must come from right.
              i_node = i_right++;
              --right_size;
            }
            else if (right_size == 0 || i_right == end())
            {
              // Right is empty. The node must come from left.
              i_node = i_left++;
              --left_size;
            }
            else if (!compare(*i_right, *i_left))
            {
              // First node of left is lower or same. The node must come from left.
              i_node = i_left++;
              --left_size;
            }
            else
            {
              // First node of right is lower. The node must come from right.
              i_node = i_right;
              ++i_right;
              --right_size;
            }

            // Add the next node to the merged head.
            if (i_head == end())
            {
              join(*i_head.p_node, *i_node.p_node);
              i_head = i_node;
              i_tail = i_node;
            }
            else
            {
              join(*i_tail.p_node, *i_node.p_node);
              i_tail = i_node;
            }

            join(*i_tail.p_node, terminal_node);
          }

          // Now left has stepped `list_size' places along, and right has too.
          i_left = i_right;
        }

        // If we have done only one merge, we're finished.
        if (number_of_merges <= 1)   // Allow for number_of_merges == 0, the empty head case
        {
          return;
        }

        // Otherwise repeat, merging lists twice the size
        list_size *= 2;
      }
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    ilist& operator = (const ilist& rhs)
    {
      if (&rhs != this)
      {
        assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    ilist& operator = (ilist&& rhs)
    {
      if (&rhs != this)
      {
        this->initialise();

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

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    ilist(bool pool_is_shared_)
      : list_base(pool_is_shared_)
    {
    }

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    ilist(etl::ipool& node_pool, size_t max_size_, bool pool_is_shared_)
      : list_base(node_pool, max_size_, pool_is_shared_)
    {
    }

    //*************************************************************************
    /// Initialise the list.
    //*************************************************************************
    void initialise()
    {
      if (this->p_node_pool != ETL_NULLPTR)
      {
        if (!empty())
        {
          if (etl::is_trivially_destructible<T>::value && !has_shared_pool())
          {
            ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));
            p_node_pool->release_all();
            ETL_RESET_DEBUG_COUNT;
          }
          else
          {
            node_t* p_first = terminal_node.next;
            node_t* p_last = &terminal_node;

            while (p_first != p_last)
            {
              destroy_data_node(static_cast<data_node_t&>(*p_first)); // Destroy the current node.
              p_first = p_first->next;                                // Move to the next node.
            }
          }
        }
      }

      join(terminal_node, terminal_node);
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move a list
    //*************************************************************************
    void move_container(ilist&& rhs)
    {
      if (&rhs != this)
      {
        this->initialise();

        if (!rhs.empty())
        {
          // Are we using the same pool?
          if (this->get_node_pool() == rhs.get_node_pool())
          {
            // Just link the nodes to this list.
            join(terminal_node,  rhs.get_head());
            join(rhs.get_tail(), terminal_node);

            ETL_SET_DEBUG_COUNT(ETL_OBJECT_GET_DEBUG_COUNT(rhs));

            // Clear the rhs.
            ETL_OBJECT_RESET_DEBUG_COUNT(rhs);
            rhs.join(rhs.terminal_node, rhs.terminal_node);
          }
          else
          {
            // Add all of the elements.
            etl::ilist<T>::iterator first = rhs.begin();
            etl::ilist<T>::iterator last = rhs.end();

            while (first != last)
            {
              ETL_ASSERT(!full(), ETL_ERROR(list_full));

              insert_node(terminal_node, this->allocate_data_node(etl::move(*first)));
              ++first;
            }

            rhs.initialise();
          }
        }
      }
    }
#endif

  private:

    //*************************************************************************
    /// Moves an element from one position to another within the list.
    /// Moves the element at position 'from' to the position before 'to'.
    //*************************************************************************
    void move(iterator to, iterator from)
    {
      if (from == to)
      {
        return; // Can't more to before yourself!
      }

      node_t& from_node = *from.p_node;
      node_t& to_node = *to.p_node;

      // Disconnect the node from the list.
      join(*from_node.previous, *from_node.next);

      // Attach it to the new position.
      join(*to_node.previous, from_node);
      join(from_node, to_node);
    }

    //*************************************************************************
    /// Moves a range from one position to another within the list.
    /// Moves a range at position 'first'/'last' to the position before 'to'.
    //*************************************************************************
    void move(iterator to, iterator first, iterator last)
    {
      if ((first == to) || (last == to))
      {
        return; // Can't more to before yourself!
      }

#if ETL_IS_DEBUG_BUILD
      // Check that we are not doing an illegal move!
      for (const_iterator item = first; item != last; ++item)
      {
        ETL_ASSERT(item != to, ETL_ERROR(list_iterator));
      }
#endif

      node_t& first_node = *first.p_node;
      node_t& last_node = *last.p_node;
      node_t& to_node = *to.p_node;
      node_t& final_node = *last_node.previous;

      // Disconnect the range from the list.
      join(*first_node.previous, last_node);

      // Attach it to the new position.
      join(*to_node.previous, first_node);
      join(final_node, to_node);
    }

    //*************************************************************************
    /// Remove a node.
    //*************************************************************************
    void remove_node(node_t& node)
    {
      // Disconnect the node from the list.
      join(*node.previous, *node.next);

      // Destroy the pool object.
      destroy_data_node(static_cast<data_node_t&>(node));
    }

    //*************************************************************************
    /// Allocate a data_node_t.
    //*************************************************************************
    data_node_t& allocate_data_node(const_reference value)
    {
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));

      data_node_t* p_data_node = create_data_node();
      ::new (&(p_data_node->value)) T(value);
      ETL_INCREMENT_DEBUG_COUNT

      return *p_data_node;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Allocate a data_node_t.
    //*************************************************************************
    data_node_t& allocate_data_node(rvalue_reference value)
    {
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));

      data_node_t* p_data_node = create_data_node();
      ::new (&(p_data_node->value)) T(etl::move(value));
      ETL_INCREMENT_DEBUG_COUNT

        return *p_data_node;
    }
#endif

    //*************************************************************************
    /// Create a data_node_t.
    //*************************************************************************
    data_node_t* create_data_node()
    {
      data_node_t* (etl::ipool::*func)() = &etl::ipool::allocate<data_node_t>;
      return (p_node_pool->*func)();
    }

    //*************************************************************************
    /// Destroy a data_node_t.
    //*************************************************************************
    void destroy_data_node(data_node_t& node)
    {
      ETL_ASSERT(p_node_pool != ETL_NULLPTR, ETL_ERROR(list_no_pool));
      node.value.~T();
      p_node_pool->release(&node);
      ETL_DECREMENT_DEBUG_COUNT
    }

    // Disable copy construction.
    ilist(const ilist&);

#if defined(ETL_POLYMORPHIC_LIST) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~ilist()
    {
    }
#else
  protected:
    ~ilist()
    {
    }
#endif

  private:

    //*************************************************************************
    /// Convert from const_iterator to iterator.
    //*************************************************************************
    iterator to_iterator(const_iterator itr) const
    {
      return iterator(*(const_cast<node_t*>(itr.p_node)));
    }
  };

  //*************************************************************************
  /// A templated list implementation that uses a fixed size buffer.
  //*************************************************************************
  template <typename T, const size_t MAX_SIZE_>
  class list : public etl::ilist<T>
  {
  public:

    ETL_STATIC_ASSERT((MAX_SIZE_ > 0U), "Zero capacity etl::list is not valid");

    static ETL_CONSTANT size_t MAX_SIZE = MAX_SIZE_;

  public:

    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
#if ETL_USING_CPP11
    typedef T&&      rvalue_reference;
#endif
    typedef size_t   size_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    list()
      : etl::ilist<T>(node_pool, MAX_SIZE, false)
    {
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~list()
    {
      this->initialise();
    }

    //*************************************************************************
    /// Construct from size.
    //*************************************************************************
    explicit list(size_t initial_size)
      : etl::ilist<T>(node_pool, MAX_SIZE, false)
    {
      this->assign(initial_size, T());
    }

    //*************************************************************************
    /// Construct from size and value.
    //*************************************************************************
    list(size_t initial_size, const T& value)
      : etl::ilist<T>(node_pool, MAX_SIZE, false)
    {
      this->assign(initial_size, value);
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    list(const list& other)
      : etl::ilist<T>(node_pool, MAX_SIZE, false)
    {
      if (this != &other)
      {
        this->assign(other.cbegin(), other.cend());
      }
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move constructor.
    //*************************************************************************
    list(list&& other)
      : etl::ilist<T>(node_pool, MAX_SIZE, false)
    {
      if (this != &other)
      {
        this->initialise();

        typename etl::ilist<T>::iterator itr = other.begin();
        while (itr != other.end())
        {
          this->push_back(etl::move(*itr));
          ++itr;
        }

        other.initialise();
      }
    }
#endif

    //*************************************************************************
    /// Construct from range.
    //*************************************************************************
    template <typename TIterator>
    list(TIterator first, TIterator last, typename etl::enable_if<!etl::is_integral<TIterator>::value, int>::type = 0)
      : ilist<T>(node_pool, MAX_SIZE, false)
    {
      this->assign(first, last);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    list(std::initializer_list<T> init)
      : ilist<T>(node_pool, MAX_SIZE, false)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    list& operator = (const list& rhs)
    {
      if (&rhs != this)
      {
        this->assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    list& operator = (list&& rhs)
    {
      this->move_container(etl::move(rhs));

      return *this;
    }
#endif

  private:

    /// The pool of nodes used in the list.
    etl::pool<typename etl::ilist<T>::data_node_t, MAX_SIZE> node_pool;
  };

  template <typename T, const size_t MAX_SIZE_>
  ETL_CONSTANT size_t list<T, MAX_SIZE_>::MAX_SIZE;

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST
  template <typename... T>
  list(T...) -> list<typename etl::common_type_t<T...>,
                     sizeof...(T)>;
#endif

  //*************************************************************************
  /// Make
  //*************************************************************************
#if ETL_USING_CPP11 && ETL_HAS_INITIALIZER_LIST
  template <typename... T>
  constexpr auto make_list(T... t) -> etl::list<typename etl::common_type_t<T...>, sizeof...(T)>
  {
    return { { etl::forward<T>(t)... } };
  }
#endif

  //*************************************************************************
  /// A templated list implementation that uses a fixed size buffer.
  //*************************************************************************
  template <typename T>
  class list_ext : public etl::ilist<T>
  {
  public:

    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef size_t   size_type;

    typedef typename etl::ilist<T>::data_node_t pool_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    list_ext()
      : etl::ilist<T>(true)
    {
    }

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    explicit list_ext(etl::ipool& node_pool)
      : etl::ilist<T>(node_pool, node_pool.max_size(), true)
    {
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~list_ext()
    {
      this->initialise();
    }

    //*************************************************************************
    /// Construct from size.
    //*************************************************************************
    explicit list_ext(size_t initial_size, etl::ipool& node_pool)
      : etl::ilist<T>(node_pool, node_pool.max_size(), true)
    {
      this->assign(initial_size, T());
    }

    //*************************************************************************
    /// Construct from size and value.
    //*************************************************************************
    list_ext(size_t initial_size, const T& value, etl::ipool& node_pool)
      : etl::ilist<T>(node_pool, node_pool.max_size(), true)
    {
      this->assign(initial_size, value);
    }

    //*************************************************************************
    /// Copy constructor. Implicit pool.
    //*************************************************************************
    list_ext(const list_ext& other)
      : etl::ilist<T>(*other.p_node_pool, other.p_node_pool->max_size(), true)
    {
      if (this != &other)
      {
        this->assign(other.cbegin(), other.cend());
      }
    }

    //*************************************************************************
    /// Copy constructor. Explicit pool.
    //*************************************************************************
    list_ext(const list_ext& other, etl::ipool& node_pool)
      : etl::ilist<T>(node_pool, node_pool.max_size(), true)
    {
      if (this != &other)
      {
        this->assign(other.cbegin(), other.cend());
      }
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move constructor. Implicit pool.
    //*************************************************************************
    list_ext(list_ext&& other)
      : etl::ilist<T>(*other.p_node_pool, other.p_node_pool->max_size(), true)
    {
      this->move_container(etl::move(other));
    }

    //*************************************************************************
    /// Move constructor. Explicit pool.
    //*************************************************************************
    list_ext(list_ext&& other, etl::ipool& node_pool)
      : etl::ilist<T>(node_pool, node_pool.max_size(), true)
    {
      this->move_container(etl::move(other));
    }
#endif

    //*************************************************************************
    /// Construct from range.
    //*************************************************************************
    template <typename TIterator>
    list_ext(TIterator first, TIterator last, etl::ipool& node_pool, typename etl::enable_if<!etl::is_integral<TIterator>::value, int>::type = 0)
      : ilist<T>(node_pool, node_pool.max_size(), true)
    {
      this->assign(first, last);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    list_ext(std::initializer_list<T> init, etl::ipool& node_pool)
      : ilist<T>(node_pool, node_pool.max_size(), true)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    list_ext& operator = (const list_ext& rhs)
    {
      if (&rhs != this)
      {
        this->assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    list_ext& operator = (list_ext&& rhs)
    {
      this->move_container(etl::move(rhs));

      return *this;
    }
#endif

    //*************************************************************************
    /// Set the pool instance.
    //*************************************************************************
    void set_pool(etl::ipool& pool)
    {
      // Clear the list of any current elements.
      if (this->get_node_pool() != ETL_NULLPTR)
      {
        this->clear();
      }

      this->set_node_pool(pool);
    }

    //*************************************************************************
    /// Get the pool instance.
    //*************************************************************************
    etl::ipool& get_pool() const
    {
      return *this->p_node_pool;
    }
  };

  //*************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first list.
  ///\param rhs Reference to the second list.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>.
  //*************************************************************************
  template <typename T>
  bool operator ==(const etl::ilist<T>& lhs, const etl::ilist<T>& rhs)
  {
    return (lhs.size() == rhs.size()) && etl::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  //*************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first list.
  ///\param rhs Reference to the second list.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>.
  //*************************************************************************
  template <typename T>
  bool operator !=(const etl::ilist<T>& lhs, const etl::ilist<T>& rhs)
  {
    return !(lhs == rhs);
  }

  //*************************************************************************
  /// Less than operator.
  ///\param lhs Reference to the first list.
  ///\param rhs Reference to the second list.
  ///\return <b>true</b> if the first list is lexicographically less than the
  /// second, otherwise <b>false</b>.
  //*************************************************************************
  template <typename T>
  bool operator <(const etl::ilist<T>& lhs, const etl::ilist<T>& rhs)
  {
    return etl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }

  //*************************************************************************
  /// Greater than operator.
  ///\param lhs Reference to the first list.
  ///\param rhs Reference to the second list.
  ///\return <b>true</b> if the first list is lexicographically greater than the
  /// second, otherwise <b>false</b>.
  //*************************************************************************
  template <typename T>
  bool operator >(const etl::ilist<T>& lhs, const etl::ilist<T>& rhs)
  {
    return (rhs < lhs);
  }

  //*************************************************************************
  /// Less than or equal operator.
  ///\param lhs Reference to the first list.
  ///\param rhs Reference to the second list.
  ///\return <b>true</b> if the first list is lexicographically less than or equal
  /// to the second, otherwise <b>false</b>.
  //*************************************************************************
  template <typename T>
  bool operator <=(const etl::ilist<T>& lhs, const etl::ilist<T>& rhs)
  {
    return !(lhs > rhs);
  }

  //*************************************************************************
  /// Greater than or equal operator.
  ///\param lhs Reference to the first list.
  ///\param rhs Reference to the second list.
  ///\return <b>true</b> if the first list is lexicographically greater than or
  /// equal to the second, otherwise <b>false</b>.
  //*************************************************************************
  template <typename T>
  bool operator >=(const etl::ilist<T>& lhs, const etl::ilist<T>& rhs)
  {
    return !(lhs < rhs);
  }
}

#include "private/minmax_pop.h"

#endif
