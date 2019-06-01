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

#ifndef ETL_FORWARD_LIST_INCLUDED
#define ETL_FORWARD_LIST_INCLUDED

#include <stddef.h>

#include <new>

#include "platform.h"

#include "stl/algorithm.h"
#include "stl/iterator.h"
#include "stl/functional.h"
#include "stl/utility.h"

#include "pool.h"
#include "container.h"
#include "exception.h"
#include "error_handler.h"
#include "debug_count.h"
#include "nullptr.h"
#include "type_traits.h"
#include "memory.h"

#if ETL_CPP11_SUPPORTED && !defined(ETL_STLPORT) && !defined(ETL_NO_STL)
  #include <initializer_list>
#endif

#include "private/minmax_push.h"

#undef ETL_FILE
#define ETL_FILE "6"

//*****************************************************************************
///\defgroup forward_list forward_list
/// A linked forward_list with the capacity defined at compile time.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// Exception for the forward_list.
  ///\ingroup forward_list
  //***************************************************************************
  class forward_list_exception : public etl::exception
  {
  public:

    forward_list_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Full exception for the forward_list.
  ///\ingroup forward_list
  //***************************************************************************
  class forward_list_full : public etl::forward_list_exception
  {
  public:

    forward_list_full(string_type file_name_, numeric_type line_number_)
      : etl::forward_list_exception(ETL_ERROR_TEXT("forward_list:full", ETL_FILE"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Empty exception for the forward_list.
  ///\ingroup forward_list
  //***************************************************************************
  class forward_list_empty : public etl::forward_list_exception
  {
  public:

    forward_list_empty(string_type file_name_, numeric_type line_number_)
      : etl::forward_list_exception(ETL_ERROR_TEXT("forward_list:empty", ETL_FILE"B"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Iterator exception for the forward_list.
  ///\ingroup forward_list
  //***************************************************************************
  class forward_list_iterator : public etl::forward_list_exception
  {
  public:

    forward_list_iterator(string_type file_name_, numeric_type line_number_)
      : etl::forward_list_exception(ETL_ERROR_TEXT("forward_list:iterator", ETL_FILE"C"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Unsorted exception for the list.
  ///\ingroup list
  //***************************************************************************
  class forward_list_no_pool : public forward_list_exception
  {
  public:

    forward_list_no_pool(string_type file_name_, numeric_type line_number_)
      : forward_list_exception(ETL_ERROR_TEXT("list:no pool", ETL_FILE"D"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The base class for all forward_lists.
  ///\ingroup forward_list
  //***************************************************************************
  class forward_list_base
  {
  protected:

    //*************************************************************************
    /// The node element in the forward_list.
    //*************************************************************************
    struct node_t
    {
      node_t()
        : next(nullptr)
      {
      }

      node_t* next;
    };

  public:

    typedef size_t size_type; ///< The type used for determining the size of forward_list.

    //*************************************************************************
    /// <b>true</b> if the list has a shared pool.
    //*************************************************************************
    bool has_shared_pool() const
    {
      return pool_is_shared;
    }

    //*************************************************************************
    /// Gets the size of the forward_list.
    //*************************************************************************
    size_type size() const
    {
      if (has_shared_pool())
      {
        // We have to count what we actually own.
        size_type count = 0;

        node_t* p_node = start_node.next;

        while (p_node != nullptr)
        {
          ++count;
          p_node = p_node->next;
        }

        return count;
      }
      else
      {
        ETL_ASSERT(p_node_pool != nullptr, ETL_ERROR(forward_list_no_pool));
        return p_node_pool->size();
      }
    }

    //*************************************************************************
    /// Gets the maximum possible size of the forward_list.
    //*************************************************************************
    size_type max_size() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Checks to see if the forward_list is empty.
    //*************************************************************************
    bool empty() const
    {
      if (has_shared_pool())
      {
        return (size() == 0);
      }
      else
      {
        ETL_ASSERT(p_node_pool != nullptr, ETL_ERROR(forward_list_no_pool));
        return p_node_pool->empty();
      }
    }

    //*************************************************************************
    /// Checks to see if the forward_list is full.
    //*************************************************************************
    bool full() const
    {
      ETL_ASSERT(p_node_pool != nullptr, ETL_ERROR(forward_list_no_pool));
      return p_node_pool->full();
    }

    //*************************************************************************
    /// Returns the remaining capacity.
    ///\return The remaining capacity.
    //*************************************************************************
    size_t available() const
    {
      ETL_ASSERT(p_node_pool != nullptr, ETL_ERROR(forward_list_no_pool));
      return p_node_pool->available();
    }

    //*************************************************************************
    /// Reverses the forward_list.
    //*************************************************************************
    void reverse()
    {
      if (is_trivial_list())
      {
        return;
      }

      node_t* p_last = &start_node;
      node_t* p_current = p_last->next;
      node_t* p_next = p_current->next;

      p_current->next = nullptr;

      while (p_next != nullptr)
      {
        p_last = p_current;
        p_current = p_next;
        p_next = p_current->next;

        p_current->next = p_last;
      }

      join(&start_node, p_current);
    }

  protected:

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    forward_list_base(bool pool_is_shared_)
      : p_node_pool(nullptr),
        MAX_SIZE(0),
        pool_is_shared(pool_is_shared_)
    {
    }

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    forward_list_base(etl::ipool& node_pool_, size_type max_size_, bool pool_is_shared_)
      : p_node_pool(&node_pool_),
        MAX_SIZE(max_size_),
        pool_is_shared(pool_is_shared_)
    {
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~forward_list_base()
    {
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    node_t* get_head()
    {
      return start_node.next;
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    const node_t* get_head() const
    {
      return start_node.next;
    }

    //*************************************************************************
    /// Insert a node.
    //*************************************************************************
    inline void insert_node_after(node_t& position, node_t& node)
    {
      // Connect to the forward_list.
      join(&node, position.next);
      join(&position, &node);
    }

    //*************************************************************************
    /// Is the forward_list a trivial length?
    //*************************************************************************
    bool is_trivial_list() const
    {
      return (size() < 2);
    }

    //*************************************************************************
    /// Join two nodes.
    //*************************************************************************
    void join(node_t* left, node_t* right)
    {
      left->next = right;
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

    node_t      start_node;     ///< The node that acts as the forward_list start.
    etl::ipool* p_node_pool;    ///< The pool of data nodes used in the list.
    size_type   MAX_SIZE;       ///< The maximum size of the forward_list.
    bool        pool_is_shared; ///< If <b>true</b> then the pool is shared between lists.
    ETL_DECLARE_DEBUG_COUNT     ///< Internal debugging.
  };

  //***************************************************************************
  /// A templated base for all etl::forward_list types.
  ///\ingroup forward_list
  //***************************************************************************
  template <typename T>
  class iforward_list : public etl::forward_list_base
  {
  public:

    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef size_t   size_type;

#if ETL_CPP11_SUPPORTED
    typedef T&&      rvalue_reference;
#endif

  protected:

    //*************************************************************************
    /// The data node element in the forward_list.
    //*************************************************************************
    struct data_node_t : public node_t
    {
      explicit data_node_t(const T& value_)
        : value(value_)
      {}

      T value;
    };

  public:

    //*************************************************************************
    /// iterator.
    //*************************************************************************
    class iterator : public std::iterator<std::forward_iterator_tag, T>
    {
    public:

      friend class iforward_list;

      iterator()
        : p_node(nullptr)
      {
      }

      iterator(node_t* node)
        : p_node(node)
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

      iterator operator =(const iterator& other)
      {
        p_node = other.p_node;
        return *this;
      }

      reference operator *()
      {
        return iforward_list::data_cast(p_node)->value;
      }

      const_reference operator *() const
      {
        return iforward_list::data_cast(p_node)->value;
      }

      pointer operator &()
      {
        return &(iforward_list::data_cast(p_node)->value);
      }

      const_pointer operator &() const
      {
        return &(iforward_list::data_cast(p_node)->value);
      }

      pointer operator ->()
      {
        return &(iforward_list::data_cast(p_node)->value);
      }

      const_pointer operator ->() const
      {
        return &(iforward_list::data_cast(p_node)->value);
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
    class const_iterator : public std::iterator<std::forward_iterator_tag, const T>
    {
    public:

      friend class iforward_list;

      const_iterator()
        : p_node(nullptr)
      {
      }

      const_iterator(node_t* node)
        : p_node(node)
      {
      }

      const_iterator(const node_t* node)
        : p_node(node)
      {
      }

      const_iterator(const typename iforward_list::iterator& other)
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

      const_iterator operator =(const const_iterator& other)
      {
        p_node = other.p_node;
        return *this;
      }

      const_reference operator *() const
      {
        return iforward_list::data_cast(p_node)->value;
      }

      const_pointer operator &() const
      {
        return &(iforward_list::data_cast(p_node)->value);
      }

      const_pointer operator ->() const
      {
        return &(iforward_list::data_cast(p_node)->value);
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

    typedef typename std::iterator_traits<iterator>::difference_type difference_type;

    //*************************************************************************
    /// Gets the beginning of the forward_list.
    //*************************************************************************
    iterator begin()
    {
      return iterator(get_head());
    }

    //*************************************************************************
    /// Gets the beginning of the forward_list.
    //*************************************************************************
    const_iterator begin() const
    {
      return const_iterator(get_head());
    }

    //*************************************************************************
    /// Gets before the beginning of the forward_list.
    //*************************************************************************
    iterator before_begin()
    {
      return iterator(&start_node);
    }

    //*************************************************************************
    /// Gets before the beginning of the forward_list.
    //*************************************************************************
    const_iterator before_begin() const
    {
      return const_iterator(&start_node);
    }

    //*************************************************************************
    /// Gets the beginning of the forward_list.
    //*************************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(get_head());
    }

    //*************************************************************************
    /// Gets the end of the forward_list.
    //*************************************************************************
    iterator end()
    {
      return iterator();
    }

    //*************************************************************************
    /// Gets the end of the forward_list.
    //*************************************************************************
    const_iterator end() const
    {
      return const_iterator();
    }

    //*************************************************************************
    /// Gets the end of the forward_list.
    //*************************************************************************
    const_iterator cend() const
    {
      return const_iterator();
    }

    //*************************************************************************
    /// Clears the forward_list.
    //*************************************************************************
    void clear()
    {
      initialise();
    }

    //*************************************************************************
    /// Gets a reference to the first element.
    //*************************************************************************
    reference front()
    {
      return data_cast(*get_head()).value;
    }

    //*************************************************************************
    /// Gets a const reference to the first element.
    //*************************************************************************
    const_reference front() const
    {
      return data_cast(*get_head()).value;
    }

    //*************************************************************************
    /// Assigns a range of values to the forward_list.
    /// If asserts or exceptions are enabled throws etl::forward_list_full if the forward_list does not have enough free space.
    /// If ETL_THROW_EXCEPTIONS & ETL_DEBUG are defined throws forward_list_iterator if the iterators are reversed.
    //*************************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
#if defined(ETL_DEBUG)
      difference_type d = std::distance(first, last);
      ETL_ASSERT(d >= 0, ETL_ERROR(forward_list_iterator));
#endif

      initialise();

      node_t* p_last_node = &start_node;

      // Add all of the elements.
      while (first != last)
      {
        ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));

        data_node_t& data_node = allocate_data_node(*first++);
        join(p_last_node, &data_node);
        data_node.next = nullptr;
        p_last_node = &data_node;
      }
    }

    //*************************************************************************
    /// Assigns 'n' copies of a value to the forward_list.
    //*************************************************************************
    void assign(size_t n, const T& value)
    {
      ETL_ASSERT(n <= MAX_SIZE, ETL_ERROR(forward_list_full));

      initialise();

      node_t* p_last_node = &start_node;

      // Add all of the elements.
      while (size() < n)
      {
        data_node_t& data_node = allocate_data_node(value);
        join(p_last_node, &data_node);
        data_node.next = nullptr;
        p_last_node = &data_node;
      }
    }

    //*************************************************************************
    /// Pushes a value to the front of the forward_list.
    //*************************************************************************
    void push_front(const T& value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));
#endif

      data_node_t& data_node = allocate_data_node(value);
      insert_node_after(start_node, data_node);
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Pushes a value to the front of the forward_list.
    //*************************************************************************
    void push_front(rvalue_reference value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));
#endif

      data_node_t& data_node = allocate_data_node(std::move(value));
      insert_node_after(start_node, data_node);
    }
#endif

#if ETL_CPP11_SUPPORTED && !defined(ETL_STLPORT)
    //*************************************************************************
    /// Emplaces a value to the front of the list..
    //*************************************************************************
    template <typename ... Args>
    void emplace_front(Args && ... args)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));
#endif
      data_node_t* p_data_node = p_node_pool->allocate<data_node_t>();
      ::new (&(p_data_node->value)) T(std::forward<Args>(args)...);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node_after(start_node, *p_data_node);
    }
#else
    //*************************************************************************
    /// Emplaces a value to the front of the list..
    //*************************************************************************
    template <typename T1>
    void emplace_front(const T1& value1)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));
#endif
      data_node_t* p_data_node = p_node_pool->allocate<data_node_t>();
      ::new (&(p_data_node->value)) T(value1);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node_after(start_node, *p_data_node);
    }

    //*************************************************************************
    /// Emplaces a value to the front of the list..
    //*************************************************************************
    template <typename T1, typename T2>
    void emplace_front(const T1& value1, const T2& value2)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));
#endif
      data_node_t* p_data_node = p_node_pool->allocate<data_node_t>();
      ::new (&(p_data_node->value)) T(value1, value2);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node_after(start_node, *p_data_node);
    }

    //*************************************************************************
    /// Emplaces a value to the front of the list..
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    void emplace_front(const T1& value1, const T2& value2, const T3& value3)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));
#endif
      data_node_t* p_data_node = p_node_pool->allocate<data_node_t>();
      ::new (&(p_data_node->value)) T(value1, value2, value3);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node_after(start_node, *p_data_node);
    }

    //*************************************************************************
    /// Emplaces a value to the front of the list..
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    void emplace_front(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));
#endif
      data_node_t* p_data_node = p_node_pool->allocate<data_node_t>();
      ::new (&(p_data_node->value)) T(value1, value2, value3, value4);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node_after(start_node, *p_data_node);
    }
#endif // ETL_CPP11_SUPPORTED && !defined(ETL_STLPORT)

    //*************************************************************************
    /// Removes a value from the front of the forward_list.
    //*************************************************************************
    void pop_front()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!empty(), ETL_ERROR(forward_list_empty));
#endif
      remove_node_after(start_node);
    }

    //*************************************************************************
    /// Resizes the forward_list.
    //*************************************************************************
    void resize(size_t n)
    {
      resize(n, T());
    }

    //*************************************************************************
    /// Resizes the forward_list.
    /// If asserts or exceptions are enabled, will throw an etl::forward_list_full
    /// if <b>n</b> is larger than the maximum size.
    //*************************************************************************
    void resize(size_t n, T value)
    {
      ETL_ASSERT(n <= MAX_SIZE, ETL_ERROR(forward_list_full));

      size_t i = 0;
      iterator i_node = begin();
      iterator i_last_node;

      if (empty())
      {
        assign(n, value);
      }
      else
      {
        // Find where we're currently at.
        while ((i < n) && (i_node != end()))
        {
          ++i;
          i_last_node = i_node;
          ++i_node;
        }

        if (i_node != end())
        {
          // Reduce.
          erase_after(i_last_node, end());
        }
        else if (i_node == end())
        {
          // Increase.
          while (i < n)
          {
            i_last_node = insert_after(i_last_node, value);
            ++i;
          }
        }
      }
    }

    //*************************************************************************
    /// Inserts a value to the forward_list after the specified position.
    //*************************************************************************
    iterator insert_after(iterator position, const T& value)
    {
      ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));

      data_node_t& data_node = allocate_data_node(value);
      insert_node_after(*position.p_node, data_node);

      return iterator(&data_node);
    }

#if ETL_CPP11_SUPPORTED && !defined(ETL_STLPORT)
    //*************************************************************************
    /// Emplaces a value to the forward_list after the specified position.
    //*************************************************************************
    template <typename ... Args>
    iterator emplace_after(iterator position, Args && ... args)
    {
      ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));

      data_node_t* p_data_node = p_node_pool->allocate<data_node_t>();
      ::new (&(p_data_node->value)) T(std::forward<Args>(args)...);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node_after(*position.p_node, *p_data_node);

      return iterator(p_data_node);
    }
#else
    //*************************************************************************
    /// Emplaces a value to the forward_list after the specified position.
    //*************************************************************************
    template <typename T1>
    iterator emplace_after(iterator position, const T1& value1)
    {
      ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));

      data_node_t* p_data_node = p_node_pool->allocate<data_node_t>();
      ::new (&(p_data_node->value)) T(value1);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node_after(*position.p_node, *p_data_node);

      return iterator(p_data_node);
    }

    //*************************************************************************
    /// Emplaces a value to the forward_list after the specified position.
    //*************************************************************************
    template <typename T1, typename T2>
    iterator emplace_after(iterator position, const T1& value1, const T2& value2)
    {
      ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));

      data_node_t* p_data_node = p_node_pool->allocate<data_node_t>();
      ::new (&(p_data_node->value)) T(value1, value2);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node_after(*position.p_node, *p_data_node);

      return iterator(p_data_node);
    }

    //*************************************************************************
    /// Emplaces a value to the forward_list after the specified position.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    iterator emplace_after(iterator position, const T1& value1, const T2& value2, const T3& value3)
    {
      ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));

      data_node_t* p_data_node = p_node_pool->allocate<data_node_t>();
      ::new (&(p_data_node->value)) T(value1, value2, value3);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node_after(*position.p_node, *p_data_node);

      return iterator(p_data_node);
    }

    //*************************************************************************
    /// Emplaces a value to the forward_list after the specified position.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    iterator emplace_after(iterator position, const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));

      data_node_t* p_data_node = p_node_pool->allocate<data_node_t>();
      ::new (&(p_data_node->value)) T(value1, value2, value3, value4);
      ETL_INCREMENT_DEBUG_COUNT
      insert_node_after(*position.p_node, *p_data_node);

      return iterator(p_data_node);
    }
#endif // ETL_CPP11_SUPPORTED && !defined(ETL_STLPORT)

    //*************************************************************************
    /// Inserts 'n' copies of a value to the forward_list after the specified position.
    //*************************************************************************
    void insert_after(iterator position, size_t n, const T& value)
    {
      ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));

      for (size_t i = 0; !full() && (i < n); ++i)
      {
        // Set up the next free node.
        data_node_t& data_node = allocate_data_node(value);
        insert_node_after(*position.p_node, data_node);
      }
    }

    //*************************************************************************
    /// Inserts a range of values to the forward_list after the specified position.
    //*************************************************************************
    template <typename TIterator>
    void insert_after(iterator position, TIterator first, TIterator last)
    {
#if defined(ETL_DEBUG)
      difference_type d = std::distance(first, last);
      ETL_ASSERT((d + size()) <= MAX_SIZE, ETL_ERROR(forward_list_full));
#endif

      while (first != last)
      {
        // Set up the next free node.
        data_node_t& data_node = allocate_data_node(*first++);
        insert_node_after(*position.p_node, data_node);
        ++position;
      }
    }

    //*************************************************************************
    /// Erases the value at the specified position.
    //*************************************************************************
    iterator erase_after(iterator position)
    {
      iterator next(position);
      if (next != end())
      {
        ++next;
        if (next != end())
        {
          ++next;
          remove_node_after(*position.p_node);
        }
      }

      return next;
    }

    //*************************************************************************
    /// Erases a range of elements.
    //*************************************************************************
    iterator erase_after(iterator first, iterator last)
    {
      if (first != end() && (first != last))
      {
        node_t* p_first = first.p_node;
        node_t* p_last = last.p_node;
        node_t* p_next = p_first->next;

        // Join the ends.
        join(p_first, p_last);

        p_first = p_next;

        // Erase the ones in between.
        while (p_first != p_last)
        {
          p_next = p_first->next;                               // Remember the next node.
          destroy_data_node(static_cast<data_node_t&>(*p_first)); // Destroy the pool object.
          p_first = p_next;                                     // Move to the next node.
        }

        if (p_next == nullptr)
        {
          return end();
        }
        else
        {
          return iterator(p_last);
        }
      }
      else
      {
        return end();
      }
    }

    //*************************************************************************
    /// Erases the value at the specified position.
    //*************************************************************************
    void move_after(const_iterator from_before, const_iterator to_before)
    {
      if (from_before == to_before) // Can't move to after yourself!
      {
        return;
      }

      node_t* p_from_before = const_cast<node_t*>(from_before.p_node); // We're not changing the value, just it's position.
      node_t* p_to_before = const_cast<node_t*>(to_before.p_node);   // We're not changing the value, just it's position.

      node_t* p_from = p_from_before->next;

      // Disconnect from the list.
      join(p_from_before, p_from->next);

      // Attach it to the new position.
      join(p_from, p_to_before->next);
      join(p_to_before, p_from);
    }

    //*************************************************************************
    /// Moves a range from one position to another within the list.
    /// Moves a range at position 'first_before'/'last' to the position before 'to_before'.
    //*************************************************************************
    void move_after(const_iterator first_before, const_iterator last, const_iterator to_before)
    {
      if ((first_before == to_before) || (last == to_before))
      {
        return; // Can't more to before yourself!
      }

#if defined(ETL_DEBUG)
      // Check that we are not doing an illegal move!
      for (const_iterator item = first_before; item != last; ++item)
      {
        ETL_ASSERT(item != to_before, ETL_ERROR(forward_list_iterator));
      }
#endif

      node_t* p_first_before = const_cast<node_t*>(first_before.p_node); // We're not changing the value, just it's position.
      node_t* p_last = const_cast<node_t*>(last.p_node);         // We're not changing the value, just it's position.
      node_t* p_to_before = const_cast<node_t*>(to_before.p_node);    // We're not changing the value, just it's position.
      node_t* p_first = p_first_before->next;
      node_t* p_final = p_first_before;

      // Find the last node that will be moved.
      while (p_final->next != p_last)
      {
        p_final = p_final->next;
      }

      // Disconnect from the list.
      join(p_first_before, p_final->next);

      // Attach it to the new position.
      join(p_final, p_to_before->next);
      join(p_to_before, p_first);
    }

    //*************************************************************************
    /// Removes all but the first element from every consecutive group of equal
    /// elements in the container.
    //*************************************************************************
    void unique()
    {
      unique(std::equal_to<T>());
    }

    //*************************************************************************
    /// Removes all but the one element from every consecutive group of equal
    /// elements in the container.
    //*************************************************************************
    template <typename TIsEqual>
    void unique(TIsEqual isEqual)
    {
      if (empty())
      {
        return;
      }

      node_t* last = get_head();
      node_t* current = last->next;

      while (current != nullptr)
      {
        // Is this value the same as the last?
        if (isEqual(data_cast(current)->value, data_cast(last)->value))
        {
          remove_node_after(*last);
        }
        else
        {
          // Move on one.
          last = current;
        }

        current = last->next;
      }
    }

    //*************************************************************************
    /// Sort using in-place merge sort algorithm.
    /// Uses 'less-than operator as the predicate.
    //*************************************************************************
    void sort()
    {
      sort(std::less<T>());
    }

    //*************************************************************************
    /// Sort using in-place merge sort algorithm.
    /// Uses a supplied predicate function or functor.
    /// This is not my algorithm. I got it off the web somewhere.
    //*************************************************************************
    template <typename TCompare>
    void sort(TCompare compare)
    {
      iterator p_left;
      iterator p_right;
      iterator p_node;
      iterator p_head;
      iterator p_tail;
      int      list_size = 1;
      int      number_of_merges;
      int      left_size;
      int      right_size;

      if (is_trivial_list())
      {
        return;
      }

      while (true)
      {
        p_left = begin();
        p_head = before_begin();
        p_tail = before_begin();

        number_of_merges = 0;  // Count the number of merges we do in this pass.

        while (p_left != end())
        {
          ++number_of_merges;  // There exists a merge to be done.
          p_right = p_left;
          left_size = 0;

          // Step 'list_size' places along from left
          for (int i = 0; i < list_size; ++i)
          {
            ++left_size;

            ++p_right;

            if (p_right == end())
            {
              break;
            }
          }

          // If right hasn't fallen off end, we have two lists to merge.
          right_size = list_size;

          // Now we have two lists. Merge them.
          while (left_size > 0 || (right_size > 0 && p_right != end()))
          {
            // Decide whether the next node of merge comes from left or right.
            if (left_size == 0)
            {
              // Left is empty. The node must come from right.
              p_node = p_right;
              ++p_right;
              --right_size;
            }
            else if (right_size == 0 || p_right == end())
            {
              // Right is empty. The node must come from left.
              p_node = p_left;
              ++p_left;
              --left_size;
            }
            else if (!compare(*p_right, *p_left))
            {
              // First node of left is lower or same. The node must come from left.
              p_node = p_left;
              ++p_left;
              --left_size;
            }
            else
            {
              // First node of right is lower. The node must come from right.
              p_node = p_right;
              ++p_right;
              --right_size;
            }

            // Add the next node to the merged head.
            if (p_head == before_begin())
            {
              join(p_head.p_node, p_node.p_node);
              p_head = p_node;
              p_tail = p_node;
            }
            else
            {
              join(p_tail.p_node, p_node.p_node);
              p_tail = p_node;
            }

            p_tail.p_node->next = nullptr;
          }

          // Now left has stepped `list_size' places along, and right has too.
          p_left = p_right;
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
    // Removes the values specified.
    //*************************************************************************
    void remove(const T& value)
    {
      iterator i_item = begin();
      iterator i_last_item = before_begin();

      while (i_item != end())
      {
        if (*i_item == value)
        {
          i_item = erase_after(i_last_item);
        }
        else
        {
          ++i_item;
          ++i_last_item;
        }
      }
    }

    //*************************************************************************
    /// Removes according to a predicate.
    //*************************************************************************
    template <typename TPredicate>
    void remove_if(TPredicate predicate)
    {
      iterator i_item = begin();
      iterator i_last_item = before_begin();

      while (i_item != end())
      {
        if (predicate(*i_item))
        {
          i_item = erase_after(i_last_item);
        }
        else
        {
          ++i_item;
          ++i_last_item;
        }
      }
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    iforward_list& operator = (const iforward_list& rhs)
    {
      if (&rhs != this)
      {
        assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    iforward_list& operator = (iforward_list&& rhs)
    {
      move_container(std::move(rhs));

      return *this;
    }
#endif

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    iforward_list(bool pool_is_shared_)
      : forward_list_base(pool_is_shared_)
    {
    }

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    iforward_list(etl::ipool& node_pool, size_t max_size_, bool pool_is_shared_)
      : forward_list_base(node_pool, max_size_, pool_is_shared_)
    {
    }

    //*************************************************************************
    /// Initialise the forward_list.
    //*************************************************************************
    void initialise()
    {
      if (!empty())
      {
        if (etl::is_trivially_destructible<T>::value && !has_shared_pool())
        {
          ETL_ASSERT(p_node_pool != nullptr, ETL_ERROR(forward_list_no_pool));
          p_node_pool->release_all();
          ETL_RESET_DEBUG_COUNT
        }
        else
        {
          node_t* p_first = start_node.next;
          node_t* p_next;

          // Erase the ones in between.
          while (p_first != nullptr)
          {
            p_next = p_first->next;                                 // Remember the next node.
            destroy_data_node(static_cast<data_node_t&>(*p_first)); // Destroy the pool object.
            p_first = p_next;                                       // Move to the next node.
          }
        }
      }

      start_node.next = nullptr;
    }

    //*************************************************************************
    /// Allocate a data_node_t.
    //*************************************************************************
    data_node_t& allocate_data_node(const_reference value)
    {
      data_node_t* p_node = p_node_pool->allocate<data_node_t>();
      ::new (&(p_node->value)) T(value);
      ETL_INCREMENT_DEBUG_COUNT

      return *p_node;
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Allocate a data_node_t.
    //*************************************************************************
    data_node_t& allocate_data_node(rvalue_reference value)
    {
      data_node_t* p_node = p_node_pool->allocate<data_node_t>();
      ::new (&(p_node->value)) T(std::move(value));
      ETL_INCREMENT_DEBUG_COUNT

      return *p_node;
    }
#endif

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Move a forward list
    //*************************************************************************
    void move_container(iforward_list&& rhs)
    {
      if (&rhs != this)
      {
        this->initialise();

        node_t* p_last_node = &start_node;

        etl::iforward_list<T>::iterator first = rhs.begin();
        etl::iforward_list<T>::iterator last = rhs.end();

        // Add all of the elements.
        while (first != last)
        {
          ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));

          data_node_t& data_node = this->allocate_data_node(std::move(*first++));
          join(p_last_node, &data_node);
          data_node.next = nullptr;
          p_last_node = &data_node;
        }

        rhs.initialise();
      }
    }
#endif

  private:

    //*************************************************************************
    /// Downcast a node_t* to a data_node_t*
    //*************************************************************************
    static data_node_t* data_cast(node_t* p_node)
    {
      return static_cast<data_node_t*>(p_node);
    }

    //*************************************************************************
    /// Downcast a node_t& to a data_node_t&
    //*************************************************************************
    static data_node_t& data_cast(node_t& node)
    {
      return static_cast<data_node_t&>(node);
    }

    //*************************************************************************
    /// Downcast a const node_t* to a const data_node_t*
    //*************************************************************************
    static const data_node_t* data_cast(const node_t* p_node)
    {
      return static_cast<const data_node_t*>(p_node);
    }

    //*************************************************************************
    /// Downcast a const node_t& to a const data_node_t&
    //*************************************************************************
    static const data_node_t& data_cast(const node_t& node)
    {
      return static_cast<const data_node_t&>(node);
    }

    //*************************************************************************
    /// Remove a node.
    //*************************************************************************
    void remove_node_after(node_t& node)
    {
      // The node to erase.
      node_t* p_node = node.next;

      if (p_node != nullptr)
      {
        // Disconnect the node from the forward_list.
        join(&node, p_node->next);

        // Destroy the pool object.
        destroy_data_node(static_cast<data_node_t&>(*p_node));
      }
    }

    //*************************************************************************
    /// Destroy a data_node_t.
    //*************************************************************************
    void destroy_data_node(data_node_t& node)
    {
      node.value.~T();
      p_node_pool->release(&node);
      ETL_DECREMENT_DEBUG_COUNT
    }

    // Disable copy construction.
    iforward_list(const iforward_list&);

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_FORWARD_LIST) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~iforward_list()
    {
    }
#else
  protected:
    ~iforward_list()
    {
    }
#endif
  };

  //*************************************************************************
  /// A templated forward_list implementation that uses a fixed size pool.
  ///\note 'merge' and 'splice_after' and are not supported.
  //*************************************************************************
  template <typename T, const size_t MAX_SIZE_>
  class forward_list : public etl::iforward_list<T>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

  public:

    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef size_t   size_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    forward_list()
      : etl::iforward_list<T>(node_pool, MAX_SIZE, false)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Construct from size and value.
    //*************************************************************************
    explicit forward_list(size_t initial_size, const T& value = T())
      : etl::iforward_list<T>(node_pool, MAX_SIZE, false)
    {
      this->assign(initial_size, value);
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    forward_list(const forward_list& other)
      : etl::iforward_list<T>(node_pool, MAX_SIZE, false)
    {
      this->assign(other.cbegin(), other.cend());
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Move constructor.
    //*************************************************************************
    forward_list(forward_list&& other)
      : etl::iforward_list<T>(node_pool, MAX_SIZE, false)
    {
      this->move_container(std::move(other));
    }
#endif

    //*************************************************************************
    /// Construct from range.
    //*************************************************************************
    template <typename TIterator>
    forward_list(TIterator first, TIterator last)
      : etl::iforward_list<T>(node_pool, MAX_SIZE, false)
    {
      this->assign(first, last);
    }

#if ETL_CPP11_SUPPORTED && !defined(ETL_STLPORT) && !defined(ETL_NO_STL)
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    forward_list(std::initializer_list<T> init)
      : etl::iforward_list<T>(node_pool, MAX_SIZE, false)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~forward_list()
    {
      this->initialise();
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    forward_list& operator = (const forward_list& rhs)
    {
      if (&rhs != this)
      {
        this->assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    forward_list& operator = (forward_list&& rhs)
    {

      this->move_container(std::move(rhs));

      return *this;
    }
#endif

  private:

    /// The pool of nodes used in the list.
    etl::pool<typename etl::iforward_list<T>::data_node_t, MAX_SIZE> node_pool;
  };

  //*************************************************************************
  /// A templated forward_list implementation that uses a fixed size pool.
  ///\note 'merge' and 'splice_after' and are not supported.
  //*************************************************************************
  template <typename T>
  class forward_list<T, 0> : public etl::iforward_list<T>
  {
  public:

    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef size_t   size_type;

    typedef typename etl::iforward_list<T>::data_node_t pool_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    forward_list()
      : etl::iforward_list<T>(true)
    {
    }

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    explicit forward_list(etl::ipool& node_pool)
      : etl::iforward_list<T>(node_pool, node_pool.max_size(), true)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Construct from size.
    //*************************************************************************
    explicit forward_list(size_t initial_size, etl::ipool& node_pool)
      : etl::iforward_list<T>(node_pool, node_pool.max_size(), true)
    {
      this->assign(initial_size, T());
    }

    //*************************************************************************
    /// Construct from size and value.
    //*************************************************************************
    explicit forward_list(size_t initial_size, const T& value, etl::ipool& node_pool)
      : etl::iforward_list<T>(node_pool, node_pool.max_size(), true)
    {
      this->assign(initial_size, value);
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    forward_list(const forward_list& other)
      : etl::iforward_list<T>(*other.p_node_pool, other.p_node_pool->max_size(), true)
    {
      this->assign(other.cbegin(), other.cend());
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Move constructor.
    //*************************************************************************
    forward_list(forward_list&& other)
      : etl::iforward_list<T>(*other.p_node_pool, other.p_node_pool->max_size(), true)
    {
      if (this != &other)
      {
        this->initialise();

        typename etl::iforward_list<T>::iterator itr = other.begin();
        while (itr != other.end())
        {
          this->push_back(std::move(*itr));
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
    forward_list(TIterator first, TIterator last, etl::ipool& node_pool)
      : etl::iforward_list<T>(node_pool, node_pool.max_size(), true)
    {
      this->assign(first, last);
    }

#if ETL_CPP11_SUPPORTED && !defined(ETL_STLPORT) && !defined(ETL_NO_STL)
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    forward_list(std::initializer_list<T> init, etl::ipool& node_pool)
      : etl::iforward_list<T>(node_pool, node_pool.max_size(), true)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~forward_list()
    {
      this->initialise();
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    forward_list& operator = (const forward_list& rhs)
    {
      if (&rhs != this)
      {
        this->assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    forward_list& operator = (forward_list&& rhs)
    {
      this->move_container(std::move(rhs));

      return *this;
    }
#endif

    //*************************************************************************
    /// Set the pool instance.
    //*************************************************************************
    void set_pool(etl::ipool& pool)
    {
      // Clear the list of any current elements.
      if (this->get_node_pool() != nullptr)
      {
        this->clear();
      }

      this->set_node_pool(pool);
    }
  };
}

//*************************************************************************
/// Equal operator.
///\param lhs Reference to the first forward_list.
///\param rhs Reference to the second forward_list.
///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator ==(const etl::iforward_list<T>& lhs, const etl::iforward_list<T>& rhs)
{
  return (lhs.size() == rhs.size()) &&
    std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

//*************************************************************************
/// Not equal operator.
///\param lhs Reference to the first forward_list.
///\param rhs Reference to the second forward_list.
///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator !=(const etl::iforward_list<T>& lhs, const etl::iforward_list<T>& rhs)
{
  return !(lhs == rhs);
}

//*************************************************************************
/// Less than operator.
///\param lhs Reference to the first forward_list.
///\param rhs Reference to the second forward_list.
///\return <b>true</b> if the first forward_list is lexicographically less than the
/// second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator <(const etl::iforward_list<T>& lhs, const etl::iforward_list<T>& rhs)
{
  return std::lexicographical_compare(lhs.begin(),
    lhs.end(),
    rhs.begin(),
    rhs.end());
}

//*************************************************************************
/// Greater than operator.
///\param lhs Reference to the first forward_list.
///\param rhs Reference to the second forward_list.
///\return <b>true</b> if the first forward_list is lexicographically greater than the
/// second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator >(const etl::iforward_list<T>& lhs, const etl::iforward_list<T>& rhs)
{
  return (rhs < lhs);
}

//*************************************************************************
/// Less than or equal operator.
///\param lhs Reference to the first forward_list.
///\param rhs Reference to the second forward_list.
///\return <b>true</b> if the first forward_list is lexicographically less than or equal
/// to the second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator <=(const etl::iforward_list<T>& lhs, const etl::iforward_list<T>& rhs)
{
  return !(lhs > rhs);
}

//*************************************************************************
/// Greater than or equal operator.
///\param lhs Reference to the first forward_list.
///\param rhs Reference to the second forward_list.
///\return <b>true</b> if the first forward_list is lexicographically greater than or
/// equal to the second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator >=(const etl::iforward_list<T>& lhs, const etl::iforward_list<T>& rhs)
{
  return !(lhs < rhs);
}

#include "private/minmax_pop.h"

#undef ETL_FILE

#endif
