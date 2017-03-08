///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2014 jwellbelove, rlindeman

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

#ifndef __ETL_ISET__
#define __ETL_ISET__
#define __ETL_IN_ISET_H__

#include <iterator>
#include <algorithm>
#include <functional>
#include <stddef.h>

#include "nullptr.h"
#include "private/set_base.h"
#include "type_traits.h"
#include "parameter_type.h"
#include "pool.h"
#include "platform.h"

#ifdef ETL_COMPILER_MICROSOFT
#undef min
#endif

namespace etl
{
  //***************************************************************************
  /// A templated base for all etl::set types.
  ///\ingroup set
  //***************************************************************************
  template <typename T, typename TCompare>
  class iset : public set_base
  {
  public:

    typedef const T     key_type;
    typedef const T     value_type;
    typedef TCompare    key_compare;
    typedef TCompare    value_compare;
    typedef value_type& const_reference;
    typedef value_type* const_pointer;
    typedef size_t      size_type;

    //*************************************************************************
    /// How to compare two key elements.
    //*************************************************************************
    struct key_comp
    {
      bool operator ()(key_type& key1, key_type& key2) const
      {
        return key_compare()(key1, key2);
      }
    };

    //*************************************************************************
    /// How to compare two value elements.
    //*************************************************************************
    struct value_comp
    {
      bool operator ()(value_type& value1, value_type& value2) const
      {
        return value_compare()(value1, value2);
      }
    };

  protected:

    //*************************************************************************
    /// The data node element in the set.
    //*************************************************************************
    struct Data_Node : public Node
    {
      explicit Data_Node(value_type value)
        : value(value)
      {
      }

      value_type value;
    };

    /// Defines the key value parameter type
    typedef typename parameter_type<T>::type key_value_parameter_t;

    //*************************************************************************
    /// How to compare node elements.
    //*************************************************************************
    bool node_comp(const Data_Node& node1, const Data_Node& node2) const
    {
      return key_compare()(node1.value, node2.value);
    }
    bool node_comp(const Data_Node& node, const key_value_parameter_t& key) const
    {
      return key_compare()(node.value, key);
    }
    bool node_comp(const key_value_parameter_t& key, const Data_Node& node) const
    {
      return key_compare()(key, node.value);
    }

  private:

    /// The pool of data nodes used in the set.
    etl::ipool* p_node_pool;

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
    class iterator : public std::iterator<std::bidirectional_iterator_tag, value_type>
    {
    public:

      friend class iset;

      iterator()
        : p_set(nullptr)
        , p_node(nullptr)
      {
      }

      iterator(iset& set)
        : p_set(&set)
        , p_node(nullptr)
      {
      }

      iterator(iset& set, Node* node)
        : p_set(&set)
        , p_node(node)
      {
      }

      iterator(const iterator& other)
        : p_set(other.p_set)
        , p_node(other.p_node)
      {
      }

      ~iterator()
      {
      }

      iterator& operator ++()
      {
        p_set->next_node(p_node);
        return *this;
      }

      iterator operator ++(int)
      {
        iterator temp(*this);
        p_set->next_node(p_node);
        return temp;
      }

      iterator& operator --()
      {
        p_set->prev_node(p_node);
        return *this;
      }

      iterator operator --(int)
      {
        iterator temp(*this);
        p_set->prev_node(p_node);
        return temp;
      }

      iterator operator =(const iterator& other)
      {
        p_set = other.p_set;
        p_node = other.p_node;
        return *this;
      }

      const_reference operator *() const
      {
        return iset::data_cast(p_node)->value;
      }

      const_pointer operator &() const
      {
        return &(iset::data_cast(p_node)->value);
      }

      const_pointer operator ->() const
      {
        return &(iset::data_cast(p_node)->value);
      }

      friend bool operator == (const iterator& lhs, const iterator& rhs)
      {
        return lhs.p_set == rhs.p_set && lhs.p_node == rhs.p_node;
      }

      friend bool operator != (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      // Pointer to set associated with this iterator
      iset* p_set;

      // Pointer to the current node for this iterator
      Node* p_node;
    };
    friend class iterator;

    //*************************************************************************
    /// const_iterator
    //*************************************************************************
    class const_iterator : public std::iterator<std::bidirectional_iterator_tag, const value_type>
    {
    public:

      friend class iset;

      const_iterator()
        : p_set(nullptr)
        , p_node(nullptr)
      {
      }

      const_iterator(const iset& set)
        : p_set(&set)
        , p_node(nullptr)
      {
      }

      const_iterator(const iset& set, const Node* node)
        : p_set(&set)
        , p_node(node)
      {
      }

      const_iterator(const typename iset::iterator& other)
        : p_set(other.p_set)
        , p_node(other.p_node)
      {
      }

      const_iterator(const const_iterator& other)
        : p_set(other.p_set)
        , p_node(other.p_node)
      {
      }

      ~const_iterator()
      {
      }

      const_iterator& operator ++()
      {
        p_set->next_node(p_node);
        return *this;
      }

      const_iterator operator ++(int)
      {
        const_iterator temp(*this);
        p_set->next_node(p_node);
        return temp;
      }

      const_iterator& operator --()
      {
        p_set->prev_node(p_node);
        return *this;
      }

      const_iterator operator --(int)
      {
        const_iterator temp(*this);
        p_set->prev_node(p_node);
        return temp;
      }

      const_iterator operator =(const const_iterator& other)
      {
        p_set = other.p_set;
        p_node = other.p_node;
        return *this;
      }

      const_reference operator *() const
      {
        return iset::data_cast(p_node)->value;
      }

      const_pointer operator &() const
      {
        return iset::data_cast(p_node)->value;
      }

      const_pointer operator ->() const
      {
        return &(iset::data_cast(p_node)->value);
      }

      friend bool operator == (const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.p_set == rhs.p_set && lhs.p_node == rhs.p_node;
      }

      friend bool operator != (const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:
      // Pointer to set associated with this iterator
      const iset* p_set;

      // Pointer to the current node for this iterator
      const Node* p_node;
    };
    friend class const_iterator;

    typedef typename std::iterator_traits<iterator>::difference_type difference_type;

    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;


    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    iset& operator = (const iset& rhs)
    {
      if (this != &rhs)
      {
        assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

    //*************************************************************************
    /// Gets the beginning of the set.
    //*************************************************************************
    iterator begin()
    {
      return iterator(*this, find_limit_node(root_node, kLeft));
    }

    //*************************************************************************
    /// Gets the beginning of the set.
    //*************************************************************************
    const_iterator begin() const
    {
      return const_iterator(*this, find_limit_node(root_node, kLeft));
    }

    //*************************************************************************
    /// Gets the end of the set.
    //*************************************************************************
    iterator end()
    {
      return iterator(*this);
    }

    //*************************************************************************
    /// Gets the end of the set.
    //*************************************************************************
    const_iterator end() const
    {
      return const_iterator(*this);
    }

    //*************************************************************************
    /// Gets the beginning of the set.
    //*************************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(*this, find_limit_node(root_node, kLeft));
    }

    //*************************************************************************
    /// Gets the end of the set.
    //*************************************************************************
    const_iterator cend() const
    {
      return const_iterator(*this);
    }

    //*************************************************************************
    /// Gets the reverse beginning of the list.
    //*************************************************************************
    reverse_iterator rbegin()
    {
      return reverse_iterator(iterator(*this));
    }

    //*************************************************************************
    /// Gets the reverse beginning of the list.
    //*************************************************************************
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(const_iterator(*this));
    }

    //*************************************************************************
    /// Gets the reverse end of the list.
    //*************************************************************************
    reverse_iterator rend()
    {
      return reverse_iterator(iterator(*this, find_limit_node(root_node, kLeft)));
    }

    //*************************************************************************
    /// Gets the reverse end of the list.
    //*************************************************************************
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(iterator(*this, find_limit_node(root_node, kLeft)));
    }

    //*************************************************************************
    /// Gets the reverse beginning of the list.
    //*************************************************************************
    const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(const_iterator(*this));
    }

    //*************************************************************************
    /// Gets the reverse end of the list.
    //*************************************************************************
    const_reverse_iterator crend() const
    {
      return const_reverse_iterator(const_iterator(*this, find_limit_node(root_node, kLeft)));
    }

    //*********************************************************************
    /// Assigns values to the set.
    /// If asserts or exceptions are enabled, emits set_full if the set does not have enough free space.
    /// If asserts or exceptions are enabled, emits set_iterator if the iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
      initialise();
      insert(first, last);
    }

    //*************************************************************************
    /// Clears the set.
    //*************************************************************************
    void clear()
    {
      initialise();
    }

    //*********************************************************************
    /// Counts the number of elements that contain the key specified.
    ///\param key The key to search for.
    ///\return 1 if element was found, 0 otherwise.
    //*********************************************************************
    size_type count(const key_value_parameter_t& key) const
    {
      return find_node(root_node, key) ? 1 : 0;
    }

    //*************************************************************************
    /// Returns two iterators with bounding (lower bound, upper bound) the
    /// value provided
    //*************************************************************************
    std::pair<iterator, iterator> equal_range(const value_type& value)
    {
      return std::make_pair<iterator, iterator>(
        iterator(*this, find_lower_node(root_node, value)),
        iterator(*this, find_upper_node(root_node, value)));
    }

    //*************************************************************************
    /// Returns two const iterators with bounding (lower bound, upper bound)
    /// the value provided.
    //*************************************************************************
    std::pair<const_iterator, const_iterator> equal_range(const value_type& value) const
    {
      return std::make_pair<const_iterator, const_iterator>(
        const_iterator(*this, find_lower_node(root_node, value)),
        const_iterator(*this, find_upper_node(root_node, value)));
    }

    //*************************************************************************
    /// Erases the value at the specified position.
    //*************************************************************************
    void erase(iterator position)
    {
      // Remove the node by its key
      erase((*position));
    }

    //*************************************************************************
    /// Erases the value at the specified position.
    //*************************************************************************
    iterator erase(const_iterator position)
    {
      // Find the parent node to be removed
      Node*& reference_node = find_node(root_node, position.p_node);
      iterator next(*this, reference_node);
      ++next;

      remove_node(root_node, (*position));

      return next;
    }

    //*************************************************************************
    // Erase the key specified.
    //*************************************************************************
    size_type erase(const key_value_parameter_t& key_value)
    {
      // Return 1 if key value was found and removed
      return remove_node(root_node, key_value) ? 1 : 0;
    }

    //*************************************************************************
    /// Erases a range of elements.
    //*************************************************************************
    iterator erase(iterator first, iterator last)
    {
      iterator next;
      while (first != last)
      {
        next = erase(const_iterator(first++));
      }

      return next;
    }

    //*************************************************************************
    /// Erases a range of elements.
    //*************************************************************************
    iterator erase(const_iterator first, const_iterator last)
    {
      iterator next;
      while (first != last)
      {
        next = erase(first++);
      }

      return next;
    }

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator pointing to the element or end() if not found.
    //*********************************************************************
    iterator find(const key_value_parameter_t& key_value)
    {
      return iterator(*this, find_node(root_node, key_value));
    }

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator pointing to the element or end() if not found.
    //*********************************************************************
    const_iterator find(const key_value_parameter_t& key_value) const
    {
      return const_iterator(*this, find_node(root_node, key_value));
    }

    //*********************************************************************
    /// Inserts a value to the set.
    /// If asserts or exceptions are enabled, emits set_full if the set is already full.
    ///\param value    The value to insert.
    //*********************************************************************
    std::pair<iterator, bool> insert(value_type& value)
    {
      // Default to no inserted node
      Node* inserted_node = nullptr;
      bool inserted = false;

      ETL_ASSERT(!full(), ETL_ERROR(set_full));

      // Get next available free node
      Data_Node& node = allocate_data_node(value);

      // Obtain the inserted node (might be nullptr if node was a duplicate)
      inserted_node = insert_node(root_node, node);
      inserted = inserted_node == &node;

      // Insert node into tree and return iterator to new node location in tree
      return std::make_pair(iterator(*this, inserted_node), inserted);
    }

    //*********************************************************************
    /// Inserts a value to the set starting at the position recommended.
    /// If asserts or exceptions are enabled, emits set_full if the set is already full.
    ///\param position The position that would precede the value to insert.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(iterator, value_type& value)
    {
      // Default to no inserted node
      Node* inserted_node = nullptr;

      ETL_ASSERT(!full(), ETL_ERROR(set_full));

      // Get next available free node
      Data_Node& node = allocate_data_node(value);

      // Obtain the inserted node (might be nullptr if node was a duplicate)
      inserted_node = insert_node(root_node, node);

      // Insert node into tree and return iterator to new node location in tree
      return iterator(*this, inserted_node);
    }

    //*********************************************************************
    /// Inserts a value to the set starting at the position recommended.
    /// If asserts or exceptions are enabled, emits set_full if the set is already full.
    ///\param position The position that would precede the value to insert.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator, value_type& value)
    {
      // Default to no inserted node
      Node* inserted_node = nullptr;

      ETL_ASSERT(!full(), ETL_ERROR(set_full));

      // Get next available free node
      Data_Node& node = allocate_data_node(value);

      // Obtain the inserted node (might be nullptr if node was a duplicate)
      inserted_node = insert_node(root_node, node);

      // Insert node into tree and return iterator to new node location in tree
      return iterator(*this, inserted_node);
    }

    //*********************************************************************
    /// Inserts a range of values to the set.
    /// If asserts or exceptions are enabled, emits set_full if the set does not have enough free space.
    ///\param position The position to insert at.
    ///\param first    The first element to add.
    ///\param last     The last + 1 element to add.
    //*********************************************************************
    template <class TIterator>
    void insert(TIterator first, TIterator last)
    {
      while (first != last)
      {
        insert(*first++);
      }
    }

    //*********************************************************************
    /// Returns an iterator pointing to the first element in the container
    /// whose key is not considered to go before the key provided or end()
    /// if all keys are considered to go before the key provided.
    ///\return An iterator pointing to the element not before key or end()
    //*********************************************************************
    iterator lower_bound(const key_value_parameter_t& key)
    {
      return iterator(*this, find_lower_node(root_node, key));
    }

    //*********************************************************************
    /// Returns a const_iterator pointing to the first element in the
    /// container whose key is not considered to go before the key provided
    /// or end() if all keys are considered to go before the key provided.
    ///\return An const_iterator pointing to the element not before key or end()
    //*********************************************************************
    const_iterator lower_bound(const key_value_parameter_t& key) const
    {
      return const_iterator(*this, find_lower_node(root_node, key));
    }

    //*********************************************************************
    /// Returns an iterator pointing to the first element in the container
    /// whose key is not considered to go after the key provided or end()
    /// if all keys are considered to go after the key provided.
    ///\return An iterator pointing to the element after key or end()
    //*********************************************************************
    iterator upper_bound(const key_value_parameter_t& key)
    {
      return iterator(*this, find_upper_node(root_node, key));
    }

    //*********************************************************************
    /// Returns a const_iterator pointing to the first element in the
    /// container whose key is not considered to go after the key provided
    /// or end() if all keys are considered to go after the key provided.
    ///\return An const_iterator pointing to the element after key or end()
    //*********************************************************************
    const_iterator upper_bound(const key_value_parameter_t& key) const
    {
      return const_iterator(*this, find_upper_node(root_node, key));
    }

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    iset(ipool& node_pool, size_t max_size_)
      : set_base(max_size_)
      , p_node_pool(&node_pool)
    {
    }

    //*************************************************************************
    /// Initialise the set.
    //*************************************************************************
    void initialise()
    {
      erase(begin(), end());
    }

  private:

    //*************************************************************************
    /// Allocate a Data_Node.
    //*************************************************************************
    Data_Node& allocate_data_node(value_type value)
    {
      Data_Node& node = *p_node_pool->allocate<Data_Node>();
      ::new ((void*)&node.value) value_type(value);
      ++construct_count;
      return node;
    }

    //*************************************************************************
    /// Destroy a Data_Node.
    //*************************************************************************
    void destroy_data_node(Data_Node& node)
    {
      node.value.~value_type();
      p_node_pool->release(&node);
      --construct_count;
    }

    //*************************************************************************
    /// Find the value matching the node provided
    //*************************************************************************
    Node* find_node(Node* position, const key_value_parameter_t& key)
    {
      Node* found = position;
      while (found)
      {
        // Downcast found to Data_Node class for comparison and other operations
        Data_Node& found_data_node = iset::data_cast(*found);

        // Compare the node value to the current position value
        if (node_comp(key, found_data_node))
        {
          // Keep searching for the node on the left
          found = found->children[kLeft];
        }
        else if (node_comp(found_data_node, key))
        {
          // Keep searching for the node on the right
          found = found->children[kRight];
        }
        else
        {
          // Node that matches the key provided was found, exit loop
          break;
        }
      }

      // Return the node found (might be nullptr)
      return found;
    }

    //*************************************************************************
    /// Find the value matching the node provided
    //*************************************************************************
    const Node* find_node(const Node* position, const key_value_parameter_t& key) const
    {
      const Node* found = position;
      while (found)
      {
        // Downcast found to Data_Node class for comparison and other operations
        const Data_Node& found_data_node = iset::data_cast(*found);

        // Compare the node value to the current position value
        if (node_comp(key, found_data_node))
        {
          // Keep searching for the node on the left
          found = found->children[kLeft];
        }
        else if (node_comp(found_data_node, key))
        {
          // Keep searching for the node on the right
          found = found->children[kRight];
        }
        else
        {
          // Node that matches the key provided was found, exit loop
          break;
        }
      }

      // Return the node found (might be nullptr)
      return found;
    }

    //*************************************************************************
    /// Find the reference node matching the node provided
    //*************************************************************************
    Node*& find_node(Node*& position, const Node* node)
    {
      Node* found = position;
      while (found)
      {
        if (found->children[kLeft] == node)
        {
          return found->children[kLeft];
        }
        else if (found->children[kRight] == node)
        {
          return found->children[kRight];
        }
        else
        {
          // Downcast found to Data_Node class for comparison and other operations
          Data_Node& found_data_node = iset::data_cast(*found);
          const Data_Node& data_node = iset::data_cast(*node);

          // Compare the node value to the current position value
          if (node_comp(data_node, found_data_node))
          {
            // Keep searching for the node on the left
            found = found->children[kLeft];
          }
          else if (node_comp(found_data_node, data_node))
          {
            // Keep searching for the node on the right
            found = found->children[kRight];
          }
          else
          {
            // Return position provided (it matches the node)
            return position;
          }
        }
      }

      // Return root node if nothing was found
      return root_node;
    }

    //*************************************************************************
    /// Find the parent node that contains the node provided in its left or
    /// right tree
    //*************************************************************************
    Node* find_parent_node(Node* position, const Node* node)
    {
      // Default to no parent node found
      Node* found = nullptr;

      // If the position provided is the same as the node then there is no parent
      if (position && node && position != node)
      {
        while (position)
        {
          // Is this position not the parent of the node we are looking for?
          if (position->children[kLeft] != node &&
              position->children[kRight] != node)
          {
            // Downcast node and position to Data_Node references for key comparisons
            const Data_Node& node_data_node = iset::data_cast(*node);
            Data_Node& position_data_node = iset::data_cast(*position);
            // Compare the node value to the current position value
            if (node_comp(node_data_node, position_data_node))
            {
              // Keep looking for parent on the left
              position = position->children[kLeft];
            }
            else if (node_comp(position_data_node, node_data_node))
            {
              // Keep looking for parent on the right
              position = position->children[kRight];
            }
          }
          else
          {
            // Return the current position as the parent node found
            found = position;

            // Parent node found, exit loop
            break;
          }
        }
      }

      // Return the parent node found (might be nullptr)
      return found;
    }

    //*************************************************************************
    /// Find the parent node that contains the node provided in its left or
    /// right tree
    //*************************************************************************
    const Node* find_parent_node(const Node* position, const Node* node) const
    {
      // Default to no parent node found
      const Node* found = nullptr;

      // If the position provided is the same as the node then there is no parent
      if (position && node && position != node)
      {
        while (position)
        {
          // Is this position not the parent of the node we are looking for?
          if (position->children[kLeft] != node &&
              position->children[kRight] != node)
          {
            // Downcast node and position to Data_Node references for key comparisons
            const Data_Node& node_data_node = iset::data_cast(*node);
            const Data_Node& position_data_node = iset::data_cast(*position);
            // Compare the node value to the current position value
            if (node_comp(node_data_node, position_data_node))
            {
              // Keep looking for parent on the left
              position = position->children[kLeft];
            }
            else if (node_comp(position_data_node, node_data_node))
            {
              // Keep looking for parent on the right
              position = position->children[kRight];
            }
          }
          else
          {
            // Return the current position as the parent node found
            found = position;

            // Parent node found, exit loop
            break;
          }
        }
      }

      // Return the parent node found (might be nullptr)
      return found;
    }

    //*************************************************************************
    /// Find the node whose key is not considered to go before the key provided
    //*************************************************************************
    Node* find_lower_node(Node* position, const key_value_parameter_t& key) const
    {
      // Something at this position? keep going
      Node* lower_node = position;
      while (lower_node)
      {
        // Downcast lower node to Data_Node reference for key comparisons
        Data_Node& data_node = iset::data_cast(*lower_node);
        // Compare the key value to the current lower node key value
        if (node_comp(key, data_node))
        {
          if (lower_node->children[kLeft])
          {
            lower_node = lower_node->children[kLeft];
          }
          else
          {
            // Found lowest node
            break;
          }
        }
        else if (node_comp(data_node, key))
        {
          lower_node = lower_node->children[kRight];
        }
        else
        {
          // Found equal node
          break;
        }
      }

      // Return the lower_node position found
      return lower_node;
    }

    //*************************************************************************
    /// Find the node whose key is considered to go after the key provided
    //*************************************************************************
    Node* find_upper_node(Node* position, const key_value_parameter_t& key) const
    {
      // Keep track of parent of last upper node
      Node* upper_node = nullptr;
      // Start with position provided
      Node* node = position;
      while (node)
      {
        // Downcast position to Data_Node reference for key comparisons
        Data_Node& data_node = iset::data_cast(*node);
        // Compare the key value to the current upper node key value
        if (node_comp(key, data_node))
        {
          upper_node = node;
          node = node->children[kLeft];
        }
        else if (node_comp(data_node, key))
        {
          node = node->children[kRight];
        }
        else if (node->children[kRight])
        {
          upper_node = find_limit_node(node->children[kRight], kLeft);
          break;
        }
        else
        {
          break;
        }
      }

      // Return the upper node position found (might be nullptr)
      return upper_node;
    }

    //*************************************************************************
    /// Insert a node.
    //*************************************************************************
    Node* insert_node(Node*& position, Data_Node& node)
    {
      // Find the location where the node belongs
      Node* found = position;

      // Was position provided not empty? then find where the node belongs
      if (position)
      {
        // Find the critical parent node (default to nullptr)
        Node* critical_parent_node = nullptr;
        Node* critical_node = root_node;

        while (found)
        {
          // Search for critical weight node (all nodes whose weight factor
          // is set to kNeither (balanced)
          if (kNeither != found->weight)
          {
            critical_node = found;
          }

          // Downcast found to Data_Node class for comparison and other operations
          Data_Node& found_data_node = iset::data_cast(*found);

          // Is the node provided to the left of the current position?
          if (node_comp(node, found_data_node))
          {
            // Update direction taken to insert new node in parent node
            found->dir = kLeft;
          }
          // Is the node provided to the right of the current position?
          else if (node_comp(found_data_node, node))
          {
            // Update direction taken to insert new node in parent node
            found->dir = kRight;
          }
          else
          {
            // Update direction taken to insert new node in parent node
            found->dir = kNeither;

            // Clear critical node value to skip weight step below
            critical_node = nullptr;

            // Destroy the node provided (its a duplicate)
            destroy_data_node(node);

            // Exit loop, duplicate node found
            break;
          }

          // Is there a child of this parent node?
          if (found->children[found->dir])
          {
            // Will this node be the parent of the next critical node whose
            // weight factor is set to kNeither (balanced)?
            if (kNeither != found->children[found->dir]->weight)
            {
              critical_parent_node = found;
            }

            // Keep looking for empty spot to insert new node
            found = found->children[found->dir];
          }
          else
          {
            // Attatch node to right
            attach_node(found->children[found->dir], node);

            // Return newly added node
            found = found->children[found->dir];

            // Exit loop
            break;
          }
        }

        // Was a critical node found that should be checked for balance?
        if (critical_node)
        {
          if (critical_parent_node == nullptr && critical_node == root_node)
          {
            balance_node(root_node);
          }
          else if (critical_parent_node == nullptr && critical_node == position)
          {
            balance_node(position);
          }
          else
          {
            balance_node(critical_parent_node->children[critical_parent_node->dir]);
          }
        }
      }
      else
      {
        // Attatch node to current position
        attach_node(position, node);

        // Return newly added node at current position
        found = position;
      }

      // Return the node found (might be nullptr)
      return found;
    }

    //*************************************************************************
    /// Find the next node in sequence from the node provided
    //*************************************************************************
    void next_node(Node*&position)
    {
      if (position)
      {
        // Is there a tree on the right? then find the minimum of that tree
        if (position->children[kRight])
        {
          // Return minimum node found
          position = find_limit_node(position->children[kRight], kLeft);
        }
        // Otherwise find the parent of this node
        else
        {
          // Start with current position as parent
          Node* parent = position;
          do {
            // Update current position as previous parent
            position = parent;
            // Find parent of current position
            parent = find_parent_node(root_node, position);
            // Repeat while previous position was on right side of parent tree
          } while (parent && parent->children[kRight] == position);

          // Set parent node as the next position
          position = parent;
        }
      }
    }

    //*************************************************************************
    /// Find the next node in sequence from the node provided
    //*************************************************************************
    void next_node(const Node*& position) const
    {
      if (position)
      {
        // Is there a tree on the right? then find the minimum of that tree
        if (position->children[kRight])
        {
          // Return minimum node found
          position = find_limit_node(position->children[kRight], kLeft);
        }
        // Otherwise find the parent of this node
        else
        {
          // Start with current position as parent
          const Node* parent = position;
          do {
            // Update current position as previous parent
            position = parent;
            // Find parent of current position
            parent = find_parent_node(root_node, position);
            // Repeat while previous position was on right side of parent tree
          } while (parent && parent->children[kRight] == position);

          // Set parent node as the next position
          position = parent;
        }
      }
    }

    //*************************************************************************
    /// Find the previous node in sequence from the node provided
    //*************************************************************************
    void prev_node(Node*&position)
    {
      // If starting at the terminal end, the previous node is the maximum node
      // from the root
      if (!position)
      {
        position = find_limit_node(root_node, kRight);
      }
      else
      {
        // Is there a tree on the left? then find the maximum of that tree
        if (position->children[kLeft])
        {
          // Return maximum node found
          position = find_limit_node(position->children[kLeft], kRight);
        }
        // Otherwise find the parent of this node
        else
        {
          // Start with current position as parent
          Node* parent = position;
          do {
            // Update current position as previous parent
            position = parent;
            // Find parent of current position
            parent = find_parent_node(root_node, position);
            // Repeat while previous position was on left side of parent tree
          } while (parent && parent->children[kLeft] == position);

          // Set parent node as the next position
          position = parent;
        }
      }
    }

    //*************************************************************************
    /// Find the previous node in sequence from the node provided
    //*************************************************************************
    void prev_node(const Node*& position) const
    {
      // If starting at the terminal end, the previous node is the maximum node
      // from the root
      if (!position)
      {
        position = find_limit_node(root_node, kRight);
      }
      else
      {
        // Is there a tree on the left? then find the maximum of that tree
        if (position->children[kLeft])
        {
          // Return maximum node found
          position = find_limit_node(position->children[kLeft], kRight);
        }
        // Otherwise find the parent of this node
        else
        {
          // Start with current position as parent
          const Node* parent = position;
          do {
            // Update current position as previous parent
            position = parent;
            // Find parent of current position
            parent = find_parent_node(root_node, position);
            // Repeat while previous position was on left side of parent tree
          } while (parent && parent->children[kLeft] == position);

          // Set parent node as the next position
          position = parent;
        }
      }
    }

    //*************************************************************************
    /// Remove the node specified from somewhere starting at the position
    /// provided
    //*************************************************************************
    Node* remove_node(Node*& position, const key_value_parameter_t& key)
    {
      // Step 1: Find the target node that matches the key provided, the
      // replacement node (might be the same as target node), and the critical
      // node to start rebalancing the tree from (up to the replacement node)
      Node* found_parent = nullptr;
      Node* found = nullptr;
      Node* replace_parent = nullptr;
      Node* replace = position;
      Node* balance_parent = nullptr;
      Node* balance = root_node;
      while (replace)
      {
        // Downcast found to Data_Node class for comparison and other operations
        Data_Node& replace_data_node = iset::data_cast(*replace);

        // Compare the key provided to the replace data node key
        if (node_comp(key, replace_data_node))
        {
          // Update the direction to the target/replace node
          replace->dir = kLeft;
        }
        else if (node_comp(replace_data_node, key))
        {
          // Update the direction to the target/replace node
          replace->dir = kRight;
        }
        else
        {
          // Update the direction to the replace node (target node found here)
          replace->dir = replace->children[kLeft] ? kLeft : kRight;

          // Note the target node was found (and its parent)
          found_parent = replace_parent;
          found = replace;
        }
        // Replacement node found if its missing a child in the replace->dir
        // value set above
        if (replace->children[replace->dir] == nullptr)
        {
          // Exit loop once replace node is found (target might not have been)
          break;
        }

        // If replacement node weight is kNeither or we are taking the shorter
        // path of replacement node and our sibling (on longer path) is
        // balanced then we need to update the balance node to match this
        // replacement node but all our ancestors will not require rebalancing
        if ((replace->weight == kNeither) ||
          (replace->weight == (1 - replace->dir) &&
          replace->children[1 - replace->dir]->weight == kNeither))
        {
          // Update balance node (and its parent) to replacement node
          balance_parent = replace_parent;
          balance = replace;
        }

        // Keep searching for the replacement node
        replace_parent = replace;
        replace = replace->children[replace->dir];
      }

      // If target node was found, proceed with rebalancing and replacement
      if (found)
      {
        // Step 2: Update weights from critical node to replacement parent node
        while (balance)
        {
          if (balance->children[balance->dir] == nullptr)
          {
            break;
          }

          if (balance->weight == kNeither)
          {
            balance->weight = 1 - balance->dir;
          }
          else if (balance->weight == balance->dir)
          {
            balance->weight = kNeither;
          }
          else
          {
            int weight = balance->children[1 - balance->dir]->weight;
            // Perform a 3 node rotation if weight is same as balance->dir
            if (weight == balance->dir)
            {
              // Is the root node being rebalanced (no parent)
              if (balance_parent == nullptr)
              {
                rotate_3node(root_node, 1 - balance->dir,
                  balance->children[1 - balance->dir]->children[balance->dir]->weight);
              }
              else
              {
                rotate_3node(balance_parent->children[balance_parent->dir], 1 - balance->dir,
                  balance->children[1 - balance->dir]->children[balance->dir]->weight);
              }
            }
            // Already balanced, rebalance and make it heavy in opposite
            // direction of the node being removed
            else if (weight == kNeither)
            {
              // Is the root node being rebalanced (no parent)
              if (balance_parent == nullptr)
              {
                rotate_2node(root_node, 1 - balance->dir);
                root_node->weight = balance->dir;
              }
              else
              {
                rotate_2node(balance_parent->children[balance_parent->dir], 1 - balance->dir);
                balance_parent->children[balance_parent->dir]->weight = balance->dir;
              }
              // Update balance node weight in opposite direction of node removed
              balance->weight = 1 - balance->dir;
            }
            // Rebalance and leave it balanced
            else
            {
              // Is the root node being rebalanced (no parent)
              if (balance_parent == nullptr)
              {
                rotate_2node(root_node, 1 - balance->dir);
              }
              else
              {
                rotate_2node(balance_parent->children[balance_parent->dir], 1 - balance->dir);
              }
            }

            // Is balance node the same as the target node found? then update
            // its parent after the rotation performed above
            if (balance == found)
            {
              if (balance_parent)
              {
                found_parent = balance_parent->children[balance_parent->dir];
                // Update dir since it is likely stale
                found_parent->dir = found_parent->children[kLeft] == found ? kLeft : kRight;
              }
              else
              {
                found_parent = root_node;
                root_node->dir = root_node->children[kLeft] == found ? kLeft : kRight;
              }
            }
          }

          // Next balance node to consider
          balance_parent = balance;
          balance = balance->children[balance->dir];
        } // while(balance)

        // Step 3: Swap found node with replacement node
        if (found_parent)
        {
          // Handle traditional case
          detach_node(found_parent->children[found_parent->dir],
            replace_parent->children[replace_parent->dir]);
        }
        // Handle root node removal
        else
        {
          // Valid replacement node for root node being removed?
          if (replace_parent)
          {
            detach_node(root_node, replace_parent->children[replace_parent->dir]);
          }
          else
          {
            // Target node and replacement node are both root node
            detach_node(root_node, root_node);
          }
        }

        // Downcast found into data node
        Data_Node& found_data_node = iset::data_cast(*found);

        // One less.
        --current_size;

        // Destroy the node removed
        destroy_data_node(found_data_node);
      } // if(found)

      // Return node found (might be nullptr)
      return found;
    }

    // Disable copy construction.
    iset(const iset&);
  };
}

//***************************************************************************
/// Equal operator.
///\param lhs Reference to the first lookup.
///\param rhs Reference to the second lookup.
///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
///\ingroup lookup
//***************************************************************************
template <typename T, typename TCompare>
bool operator ==(const etl::iset<T, TCompare>& lhs, const etl::iset<T, TCompare>& rhs)
{
  return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

//***************************************************************************
/// Not equal operator.
///\param lhs Reference to the first lookup.
///\param rhs Reference to the second lookup.
///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
///\ingroup lookup
//***************************************************************************
template <typename T, typename TCompare>
bool operator !=(const etl::iset<T, TCompare>& lhs, const etl::iset<T, TCompare>& rhs)
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
template <typename T, typename TCompare>
bool operator <(const etl::iset<T, TCompare>& lhs, const etl::iset<T, TCompare>& rhs)
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
///\return <b>true</b> if the first list is lexicographically greater than the
/// second, otherwise <b>false</b>.
//*************************************************************************
template <typename T, typename TCompare>
bool operator >(const etl::iset<T, TCompare>& lhs, const etl::iset<T, TCompare>& rhs)
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
template <typename T, typename TCompare>
bool operator <=(const etl::iset<T, TCompare>& lhs, const etl::iset<T, TCompare>& rhs)
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
template <typename T, typename TCompare>
bool operator >=(const etl::iset<T, TCompare>& lhs, const etl::iset<T, TCompare>& rhs)
{
  return !(lhs < rhs);
}

#ifdef ETL_COMPILER_MICROSOFT
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#undef __ETL_IN_ISET_H__

#endif
