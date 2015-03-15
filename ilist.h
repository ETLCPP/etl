///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

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

#ifndef __ETL_ILIST__
#define __ETL_ILIST__
#define __ETL_IN_ILIST_H__

#include <iterator>
#include <algorithm>
#include <functional>
#include <stddef.h>

#include "nullptr.h"
#include "list_base.h"
#include "type_traits.h"
#include "parameter_type.h"
#include "pool.h"

#if WIN32
#undef min
#endif

namespace etl
{
  //***************************************************************************
  /// A templated base for all etl::list types.
  ///\ingroup list
  //***************************************************************************
  template <typename T>
  class ilist : public list_base
  {
  public:

    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef size_t   size_type;

  protected:
    
    typedef typename parameter_type<T, is_fundamental<T>::value || is_pointer<T>::value>::type parameter_t;


    //*************************************************************************
    /// The node element in the list.
    //*************************************************************************
    struct Node
    {
      //***********************************************************************
      /// Constructor
      //***********************************************************************
      Node()
        : previous(nullptr),
          next(nullptr)
      {
      }

      //***********************************************************************
      /// Reverses the previous & next pointers.
      //***********************************************************************
      void reverse()
      {
        std::swap(previous, next);
      }

      Node* previous;
      Node* next;
    };

    //*************************************************************************
    /// The data node element in the list.
    //*************************************************************************
    struct Data_Node : public Node
    {
      explicit Data_Node(parameter_t value)
        : value(value)
      {
      }

      T value;
    };

    /// The node that acts as the list start and end.
    Node terminal_node;  

  private:

    /// The pool of data nodes used in the list.
    etl::ipool<Data_Node>* p_node_pool;

    //*************************************************************************
    /// Downcast a Node* to a Data_Node*
    //*************************************************************************
    static Data_Node* data_cast(Node* p_node)
    {
      return static_cast<Data_Node*>(p_node);
    }

    //*************************************************************************
    /// Downcast a Node& to a Data_Node&
    //*************************************************************************
    static Data_Node& data_cast(Node& node)
    {
      return static_cast<Data_Node&>(node);
    }

    //*************************************************************************
    /// Downcast a const Node* to a const Data_Node*
    //*************************************************************************
    static const Data_Node* data_cast(const Node* p_node)
    {
      return static_cast<const Data_Node*>(p_node);
    }

    //*************************************************************************
    /// Downcast a const Node& to a const Data_Node&
    //*************************************************************************
    static const Data_Node& data_cast(const Node& node)
    {
      return static_cast<const Data_Node&>(node);
    }

  public:

    //*************************************************************************
    /// iterator.
    //*************************************************************************
    class iterator : public std::iterator<std::bidirectional_iterator_tag, T>
    {
    public:

      friend class ilist;

      iterator()
        : p_node(nullptr)
      {
      }

      iterator(Node& node)
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

      iterator operator =(const iterator& other)
      {
        p_node = other.p_node;
        return *this;
      }

      reference operator *()
      {
        return ilist::data_cast(p_node)->value;
      }

      const_reference operator *() const
      {
        return p_node->value;
      }

      pointer operator &()
      {
        return &(ilist::data_cast(p_node)->value);
      }

      const_pointer operator &() const
      {
        return &(ilist::data_cast(p_node)->value);
      }

      pointer operator ->()
      {
        return &(ilist::data_cast(p_node)->value);
      }

      const_pointer operator ->() const
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

      Node* p_node;
    };

    //*************************************************************************
    /// const_iterator
    //*************************************************************************
    class const_iterator : public std::iterator<std::bidirectional_iterator_tag, const T>
    {
    public:

      friend class ilist;

      const_iterator()
        : p_node(nullptr)
      {
      }

      const_iterator(Node& node)
        : p_node(&node)
      {
      }

      const_iterator(const Node& node)
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

      const_iterator operator =(const const_iterator& other)
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
        return ilist::data_cast(p_node)->value;
      }

      const Data_Node* operator ->() const
      {
        return p_node;
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

      const Node* p_node;
    };

		typedef typename std::iterator_traits<iterator>::difference_type difference_type;

    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

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
      return const_iterator(static_cast<const Data_Node&>(terminal_node));
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
      return const_iterator(static_cast<const Data_Node&>(terminal_node));
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
      return const_reverse_iterator(static_cast<const Data_Node&>(terminal_node));
    }

    //*************************************************************************
    /// Gets the reverse end of the list.
    //*************************************************************************
    reverse_iterator rend()
    {
      return reverse_iterator(get_head());
    }

    //*************************************************************************
    /// Gets the reverse beginning of the list.
    //*************************************************************************
    const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(static_cast<const Data_Node&>(terminal_node));
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
		/// If ETL_THROW_EXCEPTIONS is defined throws etl::list_full if the list does not have enough free space.
    /// If ETL_THROW_EXCEPTIONS & _DEBUG are defined throws list_iterator if the iterators are reversed.
    //*************************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {      
#ifdef _DEBUG
      difference_type count = std::distance(first, last);

      if (count < 0)
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw list_iterator();
#else
        error_handler::error(list_iterator());
#endif
      }
#endif

      initialise();

      // Add all of the elements.
      while (first != last)
      {
        if (!full())
        {
          Data_Node& data_node = allocate_data_node(*first);
          join(get_tail(), data_node);
          join(data_node, terminal_node);
          ++first;
          ++current_size;
        }
        else
        {
#ifdef ETL_THROW_EXCEPTIONS
          throw list_full();
#else
          error_handler::error(list_full());
#endif
        }
      }
    }

    //*************************************************************************
    /// Assigns 'n' copies of a value to the list.
    //*************************************************************************
    void assign(size_t n, parameter_t value)
    {
      initialise();

      // Add all of the elements.
      while (current_size < n)
      {
        if (!full())
        {
          Data_Node& data_node = allocate_data_node(value);
          join(*terminal_node.previous, data_node);
          join(data_node, terminal_node);
          ++current_size;
        }
        else
        {
#ifdef ETL_THROW_EXCEPTIONS        
          throw list_full();
#else
          error_handler::error(list_full());
#endif
        }
      }
    }

    //*************************************************************************
    /// Adds a node to the front of the list so a new value can be assigned to front().
    //*************************************************************************
    void push_front()
    {
      if (!full())
      {
        Data_Node& data_node = allocate_data_node(T());
        insert_node(get_head(), data_node);
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS        
        throw list_full();
#else
        error_handler::error(list_full());
#endif
      }
    }

    //*************************************************************************
    /// Pushes a value to the front of the list.
    //*************************************************************************
    void push_front(parameter_t value)
    {
      if (!full())
      {
        Node& data_node = allocate_data_node(value);
        insert_node(get_head(), data_node);
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS        
        throw list_full();
#else
        error_handler::error(list_full());
#endif
      }
    }

    //*************************************************************************
    /// Removes a value from the front of the list.
    //*************************************************************************
    void pop_front()
    {
      if (!empty())
      {
    	  Node& node = get_head();
        remove_node(node);
      }
    }

    //*************************************************************************
    /// Adds a node to the back of the list so a new value can be assigned to back().
    //*************************************************************************
    void push_back()
    {
      if (!full())
      {
        Data_Node& data_node = allocate_data_node(T());
        insert_node(terminal_node, data_node);
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS        
        throw list_full();
#else
        error_handler::error(list_full());
#endif
      }
    }

    //*************************************************************************
    /// Pushes a value to the back of the list..
    //*************************************************************************
    void push_back(parameter_t value)
    {
      if (!full())
      {
        Data_Node& data_node = allocate_data_node(value);
        insert_node(terminal_node, data_node);
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS        
        throw list_full();
#else
        error_handler::error(list_full());
#endif
      }
    }

    //*************************************************************************
    /// Removes a value from the back of the list.
    //*************************************************************************
    void pop_back()
    {
      if (!empty())
      {
      	Node& node = get_tail();
        remove_node(node);
      }
    }

    //*************************************************************************
    /// Inserts a value to the list at the specified position.
    //*************************************************************************
    iterator insert(iterator position, const value_type& value)
    {
      if (!full())
      {
        Data_Node& data_node = allocate_data_node(value);
        insert_node(*position.p_node, data_node);

        return iterator(data_node);
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS        
        throw list_full();
#else
        error_handler::error(list_full());
        return end();
#endif
      }
    }

    //*************************************************************************
    /// Inserts 'n' copies of a value to the list at the specified position.
    //*************************************************************************
    void insert(iterator position, size_t n, const value_type& value)
    {
      for (size_t i = 0; i < n; ++i)
      {
        if (!full())
        {
          // Set up the next free node and insert.
          Data_Node& data_node = allocate_data_node(value);
          insert_node(*position.p_node, data_node);
        }
        else
        {
#ifdef ETL_THROW_EXCEPTIONS        
          throw list_full();
#else
          error_handler::error(list_full());
#endif
        }
      }
    }

    //*************************************************************************
    /// Inserts a range of values to the list at the specified position.
    //*************************************************************************
    template <typename TIterator>
    void insert(iterator position, TIterator first, TIterator last)
    {
      while (first != last)
      {
        if (!full())
        {
          // Set up the next free node and insert.
          Data_Node& data_node = allocate_data_node(*first++);
          insert_node(*position.p_node, data_node);
        }
        else
        {
#ifdef ETL_THROW_EXCEPTIONS        
          throw list_full();
#else
          error_handler::error(list_full());
#endif
        }
      }
    }

    //*************************************************************************
    /// Erases the value at the specified position.
    //*************************************************************************
    iterator erase(iterator position)
    {
      iterator next(position);
      ++next;

      remove_node(*position.p_node);
      
      return next;
    }

    //*************************************************************************
    /// Erases a range of elements.
    //*************************************************************************
    iterator erase(iterator first, iterator last)
    {
      Node* p_first = first.p_node;
      Node* p_last  = last.p_node;
      Node* p_next;

      // Join the ends.
      join(*(p_first->previous), *p_last);

      // Erase the ones in between.
      while (p_first != p_last)
      {
        // One less.
        --current_size;

        p_next  = p_first->next;                              // Remember the next node.
        destroy_data_node(static_cast<Data_Node&>(*p_first)); // Destroy the current node.
        p_first = p_next;                                     // Move to the next node.
      }

      return last;
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
    void resize(size_t n, parameter_t value)
    {
      if (n > MAX_SIZE)
      {
#ifdef ETL_THROW_EXCEPTIONS        
        throw list_full();
#else
        error_handler::error(list_full());
        n = MAX_SIZE;
#endif
      }

      // Smaller?
      if (n < size())
      {
        iterator i_start = end();
        std::advance(i_start, -difference_type(size() - n));
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
    void remove(const value_type& value)
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
        unique(std::equal_to<T>());
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
            else if (compare(*i_left, *i_right))
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
    /// Reverses the list.
    //*************************************************************************
    void reverse()
    {
      if (is_trivial_list())
      {
        return;
      }

      iterator i_item = begin();

      while (i_item != end())
      {
        i_item.p_node->reverse();
        --i_item; // Now we've reversed it, we must decrement it.
      }

      // Terminal node.
      i_item.p_node->reverse();
    }

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    ilist(etl::ipool<Data_Node>& node_pool, size_t max_size_)
      : list_base(max_size_),
        p_node_pool(&node_pool)
    {
      initialise();
    }

  private:

    //*************************************************************************
    /// Join two nodes.
    //*************************************************************************
    void join(Node& left, Node& right)
    {
      left.next      = &right;
      right.previous = &left;
    }

    //*************************************************************************
    /// Join two nodes.
    //*************************************************************************
    void join(Data_Node& left, Data_Node& right)
    {
      left.next      = &right;
      right.previous = &left;
    }

    //*************************************************************************
    /// Is the list a trivial length?
    //*************************************************************************
    bool is_trivial_list() const
    {
      return (size() < 2);
    }

    //*************************************************************************
    /// Insert a node before 'position'.
    //*************************************************************************
    void insert_node(Node& position, Node& node)
    {
      // Connect to the list.
      join(*position.previous, node);
      join(node,               position);

      // One more.
      ++current_size;
    }

    //*************************************************************************
    /// Remove a node.
    //*************************************************************************
    void remove_node(Node& node)
    {
      // Disconnect the node from the list.
      join(*node.previous, *node.next);

      // Destroy the pool object.
      destroy_data_node(static_cast<Data_Node&>(node));

      // One less.
      --current_size;
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    Node& get_head()
    {
      return *terminal_node.next;
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    const Node& get_head() const
    {
      return *terminal_node.next;
    }

    //*************************************************************************
    /// Get the tail node.
    //*************************************************************************
    Node& get_tail()
    {
      return *terminal_node.previous;
    }

    //*************************************************************************
    /// Get the tail node.
    //*************************************************************************
    const Node& get_tail() const
    {
      return *terminal_node.previous;
    }

    //*************************************************************************
    /// Allocate a Data_Node.
    //*************************************************************************
    Data_Node& allocate_data_node(parameter_t value) const
    {
      return *(p_node_pool->allocate(Data_Node(value)));
    }

    //*************************************************************************
    /// Destroy a Data_Node.
    //*************************************************************************
    void destroy_data_node(Data_Node& node) const
    {
      p_node_pool->release(&node);
    }

    //*************************************************************************
    /// Initialise the list.
    //*************************************************************************
    void initialise()
    {
      if (!empty())
      {
        p_node_pool->release_all();
      }

      current_size = 0;
      join(terminal_node, terminal_node);
    }
  };
}

//*************************************************************************
/// Equal operator.
///\param lhs Reference to the first list.
///\param rhs Reference to the second list.
///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator ==(const etl::ilist<T>& lhs, const etl::ilist<T>& rhs)
{
  return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
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
///\return <b>true</b> if the first list is lexigraphically less than the
/// second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator <(const etl::ilist<T>& lhs, const etl::ilist<T>& rhs)
{
  return std::lexicographical_compare(lhs.begin(),
                                      lhs.end(),
                                      rhs.begin(),
                                      rhs.end());
}

//*************************************************************************
/// Greater than operator.
///\param lhs Reference to the first list.
///\param rhs Reference to the second list.
///\return <b>true</b> if the first list is lexigraphically greater than the
/// second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator >(const etl::ilist<T>& lhs, const etl::ilist<T>& rhs)
{
  return std::lexicographical_compare(lhs.begin(),
                                      lhs.end(),
                                      rhs.begin(),
                                      rhs.end(),
                                      std::greater<T>());
}

//*************************************************************************
/// Less than or equal operator.
///\param lhs Reference to the first list.
///\param rhs Reference to the second list.
///\return <b>true</b> if the first list is lexigraphically less than or equal
/// to the second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator <=(const etl::ilist<T>& lhs, const etl::ilist<T>& rhs)
{
  return !operator >(lhs, rhs);
}

//*************************************************************************
/// Greater than or equal operator.
///\param lhs Reference to the first list.
///\param rhs Reference to the second list.
///\return <b>true</b> if the first list is lexigraphically greater than or
/// equal to the second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator >=(const etl::ilist<T>& lhs, const etl::ilist<T>& rhs)
{
  return !operator <(lhs, rhs);
}

#if WIN32
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#undef __ETL_IN_ILIST_H__

#endif
