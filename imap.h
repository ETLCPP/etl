///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

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

#ifndef __ETL_IMAP__
#define __ETL_IMAP__
#define __ETL_IN_IMAP_H__

#include <iterator>
#include <algorithm>
#include <functional>
#include <stddef.h>

#include "nullptr.h"
#include "map_base.h"
#include "type_traits.h"
#include "parameter_type.h"
#include "pool.h"

#include <iostream> // FIXME: Remove with std::cout

#if WIN32
#undef min
#endif

namespace etl
{
  //***************************************************************************
  /// A templated base for all etl::map types.
  ///\ingroup map
  //***************************************************************************
  template <typename TKey, typename TMapped, typename TKeyCompare>
  class imap : public map_base
  {
  public:

    typedef std::pair<TKey, TMapped>                  value_type;
    typedef TKey                                      key_type;
    typedef TMapped                                   mapped_type;
    typedef TKeyCompare                               key_compare;
    typedef value_type&                               reference;
    typedef const value_type&                         const_reference;
    typedef value_type*                               pointer;
    typedef const value_type*                         const_pointer;
    typedef size_t                                    size_type;

    //*************************************************************************
    /// How to compare two key elements.
    //*************************************************************************
    struct key_comp
    {
      bool operator ()(const key_type& key1, const key_type& key2) const
      {
        return key_compare()(key1, key2);
      }
    };

    //*************************************************************************
    /// How to compare two value elements.
    //*************************************************************************
    struct value_comp
    {
      bool operator ()(const value_type& value1, const value_type& value2) const
      {
        return key_compare()(value1.first, value2.first);
      }
    };

  protected:
    //*************************************************************************
    /// The node element in the map.
    //*************************************************************************
    struct Node
    {
      //***********************************************************************
      /// Constructor
      //***********************************************************************
      Node()
        : left(nullptr)
        , right(nullptr)
      {
      }

      //***********************************************************************
      /// Determines if the node is a leaf.
      //***********************************************************************
      bool is_leaf() const
      {
        return left == nullptr && right == nullptr;
      }

      //***********************************************************************
      /// Marks the node as a leaf.
      //***********************************************************************
      void mark_as_leaf()
      {
        left = nullptr;
        right = nullptr;
      }


      Node* left;
      Node* right;
    };

    //*************************************************************************
    /// The data node element in the map.
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
    typedef typename parameter_type<TKey>::type  key_value_parameter_t;

    //*************************************************************************
    /// How to compare node elements.
    //*************************************************************************
    bool node_comp(const Data_Node& node1, const Data_Node& node2) const
    {
      return key_compare()(node1.value.first, node2.value.first);
    }
    bool node_comp(const Data_Node& node, const key_value_parameter_t key) const
    {
      return key_compare()(node.value.first, key);
    }
    bool node_comp(const key_value_parameter_t key, const Data_Node& node) const
    {
      return key_compare()(key, node.value.first);
    }

  private:

    /// The pool of data nodes used in the map.
    ipool<Data_Node>* p_node_pool;

    /// The node that acts as the map root.
    Node* root_node;

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

      friend class imap;

      iterator()
        : p_map(nullptr)
        , p_node(nullptr)
      {
      }

      iterator(imap& map)
        : p_map(&map)
        , p_node(nullptr)
      {
      }

      iterator(imap& map, Node* node)
        : p_map(&map)
        , p_node(node)
      {
      }

      iterator(const iterator& other)
        : p_map(other.p_map)
        , p_node(other.p_node)
      {
      }

      ~iterator()
      {
      }

      iterator& operator ++()
      {
        p_map->next_node(p_node);
        return *this;
      }

      iterator operator ++(int)
      {
        iterator temp(*this);
        p_map->next_node(p_node);
        return temp;
      }

      iterator& operator --()
      {
        p_map->prev_node(p_node);
        return *this;
      }

      iterator operator --(int)
      {
        iterator temp(*this);
        p_map->prev_node(p_node);
        return temp;
      }

      iterator operator =(const iterator& other)
      {
        p_map = other.p_map;
        p_node = other.p_node;
        return *this;
      }

      reference operator *()
      {
        return imap::data_cast(p_node)->value;
      }

      const_reference operator *() const
      {
        return imap::data_cast(p_node)->value;
      }

      pointer operator &()
      {
        return &(imap::data_cast(p_node)->value);
      }

      const_pointer operator &() const
      {
        return &(imap::data_cast(p_node)->value);
      }

      pointer operator ->()
      {
        return &(imap::data_cast(p_node)->value);
      }

      const_pointer operator ->() const
      {
        return &(imap::data_cast(p_node)->value);
      }

      friend bool operator == (const iterator& lhs, const iterator& rhs)
      {
        return lhs.p_map == rhs.p_map && lhs.p_node == rhs.p_node;
      }

      friend bool operator != (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      // Pointer to map associated with this iterator
      imap* p_map;

      // Pointer to the current node for this iterator
      Node* p_node;
    };
    friend iterator;

    //*************************************************************************
    /// const_iterator
    //*************************************************************************
    class const_iterator : public std::iterator<std::bidirectional_iterator_tag, const value_type>
    {
    public:

      friend class imap;

      const_iterator()
        : p_map(nullptr)
        , p_node(nullptr)
      {
      }

      const_iterator(const imap& map)
        : p_map(&map)
        , p_node(nullptr)
      {
      }

      const_iterator(const imap& map, const Node* node)
        : p_map(&map)
        , p_node(node)
      {
      }

      const_iterator(const typename imap::iterator& other)
        : p_map(other.p_map)
        , p_node(other.p_node)
      {
      }

      const_iterator(const const_iterator& other)
        : p_map(other.p_map)
        , p_node(other.p_node)
      {
      }

      ~const_iterator()
      {
      }

      const_iterator& operator ++()
      {
        p_map->next_node(p_node);
        return *this;
      }

      const_iterator operator ++(int)
      {
        const_iterator temp(*this);
        p_map->next_node(p_node);
        return temp;
      }

      const_iterator& operator --()
      {
        p_map->prev_node(p_node);
        return *this;
      }

      const_iterator operator --(int)
      {
        const_iterator temp(*this);
        p_map->prev_node(p_node);
        return temp;
      }

      const_iterator operator =(const const_iterator& other)
      {
        p_map = other.p_map;
        p_node = other.p_node;
        return *this;
      }

      const_reference operator *() const
      {
        return imap::data_cast(p_node)->value;
      }

      const_pointer operator &() const
      {
        return imap::data_cast(p_node)->value;
      }

      const_pointer operator ->() const
      {
        return &(imap::data_cast(p_node)->value);
      }

      friend bool operator == (const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.p_map == rhs.p_map && lhs.p_node == rhs.p_node;
      }

      friend bool operator != (const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:
      // Pointer to map associated with this iterator
      const imap* p_map;

      // Pointer to the current node for this iterator
      const Node* p_node;
    };
    friend const_iterator;

    typedef typename std::iterator_traits<iterator>::difference_type difference_type;

    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;


    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    imap& operator = (const imap& rhs)
    {
      assign(rhs.cbegin(), rhs.cend());

      return *this;
    }

    //*************************************************************************
    /// Gets the beginning of the map.
    //*************************************************************************
    iterator begin()
    {
      return iterator(*this, lower_node(root_node));
    }

    //*************************************************************************
    /// Gets the beginning of the map.
    //*************************************************************************
    const_iterator begin() const
    {
      return const_iterator(*this, lower_node(root_node));
    }

    //*************************************************************************
    /// Gets the end of the map.
    //*************************************************************************
    iterator end()
    {
      return iterator(*this);
    }

    //*************************************************************************
    /// Gets the end of the map.
    //*************************************************************************
    const_iterator end() const
    {
      return const_iterator(*this);
    }

    //*************************************************************************
    /// Gets the beginning of the map.
    //*************************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(*this, lower_node(root_node));
    }

    //*************************************************************************
    /// Gets the end of the map.
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
      return reverse_iterator(iterator(*this, lower_node(root_node)));
    }

    //*************************************************************************
    /// Gets the reverse end of the list.
    //*************************************************************************
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(iterator(*this, lower_node(root_node)));
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
      return const_reverse_iterator(const_iterator(*this, lower_node(root_node)));
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'key'
    ///\param i The index.
    ///\return A reference to the value at index 'key'
    //*********************************************************************
    mapped_type& operator [](const key_value_parameter_t key)
    {
      iterator i_element = find(key);

      if (!i_element.p_node)
      {
        // Doesn't exist, so create a new one.
        i_element = insert(std::make_pair(key, mapped_type())).first;
      }

      return i_element->second;
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'key'
    /// If ETL_THROW_EXCEPTIONS is defined, emits an etl::lookup_out_of_bounds if the key is not in the range.
    ///\param i The index.
    ///\return A reference to the value at index 'key'
    //*********************************************************************
    mapped_type& at(const key_value_parameter_t key)
    {
      iterator i_element = find(key);

      if (!i_element.p_node)
      {
        // Doesn't exist.
#if ETL_THROW_EXCEPTIONS
        throw map_out_of_bounds();
#else
        error_handler::error(map_out_of_bounds());

#endif
      }

      return i_element->second;
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'key'
    /// If ETL_THROW_EXCEPTIONS is defined, emits an etl::lookup_out_of_bounds if the key is not in the range.
    ///\param i The index.
    ///\return A const reference to the value at index 'key'
    //*********************************************************************
    const mapped_type& at(const key_value_parameter_t key) const
    {
      const_iterator i_element = find(key);

      if (!i_element.p_node)
      {
        // Doesn't exist.
#if ETL_THROW_EXCEPTIONS
        throw map_out_of_bounds();
#else
        error_handler::error(map_out_of_bounds());

#endif
      }

      return i_element->second;
    }

    //*********************************************************************
    /// Assigns values to the map.
    /// If ETL_THROW_EXCEPTIONS is defined, emits map_full if the map does not have enough free space.
    /// If ETL_THROW_EXCEPTIONS is defined, emits map_iterator if the iterators are reversed.
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
    /// Clears the map.
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
    size_type count(const key_value_parameter_t key) const
    {
      return find_node(root_node, key) ? 1 : 0;
    }

    //*************************************************************************
    /// Returns two iterators with bounding (lower bound, upper bound) the key
    /// provided
    //*************************************************************************
    std::pair<iterator, iterator> equal_range(const key_type& key)
    {
      return std::make_pair<iterator, iterator>(
        iterator(*this, lower_node(root_node, key)),
        iterator(*this, upper_node(root_node, key)));
    }

    //*************************************************************************
    /// Returns two const iterators with bounding (lower bound, upper bound)
    /// the key provided.
    //*************************************************************************
    std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const
    {
      return std::make_pair<const_iterator, const_iterator>(
        const_iterator(*this, lower_node(root_node, key)),
        const_iterator(*this, upper_node(root_node, key)));
    }

    //*************************************************************************
    /// Erases the value at the specified position.
    //*************************************************************************
    void erase(iterator position)
    {
      // Remove the node by its key
      erase((*position).first);
    }

    //*************************************************************************
    /// Erases the value at the specified position.
    //*************************************************************************
    iterator erase(const_iterator position)
    {
      // Find the parent node to be removed
      Node* found_parent = find_parent_node(root_node, position.p_node);
      iterator next(*this, found_parent->left == position.p_node ?
        found_parent->left : found_parent->right);
      ++next;

      remove_node(found_parent->left == position.p_node ?
        found_parent->left : found_parent->right, position->first);

      return next;
    }

    //*************************************************************************
    // Erase the key specified.
    //*************************************************************************
    size_type erase(const key_value_parameter_t key)
    {
      // Return 1 if key value was found and removed
      return remove_node(root_node, key) ? 1 : 0;
    }

    //*************************************************************************
    /// Erases a range of elements.
    //*************************************************************************
    iterator erase(iterator first, iterator last)
    {
      while (first != last)
      {
        erase(first++);
      }

      return last;
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
    iterator find(const key_value_parameter_t key)
    {
      return iterator(*this, find_node(root_node, key));
    }

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator pointing to the element or end() if not found.
    //*********************************************************************
    const_iterator find(const key_value_parameter_t key) const
    {
      return const_iterator(*this, find_node(root_node, key));
    }

    //*********************************************************************
    /// Inserts a value to the map.
    /// If ETL_THROW_EXCEPTIONS is defined, emits map_full if the map is already full.
    ///\param value    The value to insert.
    //*********************************************************************
    std::pair<iterator, bool> insert(const value_type& value)
    {
      // Default to no inserted node
      Node* inserted_node = nullptr;
      bool inserted = false;

      if (!full())
      {
        // Get next available free node
        Data_Node& node = allocate_data_node(value);

        // Obtain the inserted node (might be nullptr if node was a duplicate)
        inserted_node = insert_node(root_node, node);
        inserted = inserted_node == &node;
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw map_full();
#else
        error_handler::error(map_full());
#endif
      }

      // Insert node into tree and return iterator to new node location in tree
      return std::make_pair(iterator(*this, inserted_node), inserted);
    }

    //*********************************************************************
    /// Inserts a value to the map starting at the position recommended.
    /// If ETL_THROW_EXCEPTIONS is defined, emits map_full if the map is already full.
    ///\param position The position that would preceed the value to insert.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(iterator position, const value_type& value)
    {
      // Default to no inserted node
      Node* inserted_node = nullptr;

      if (!full())
      {
        // Get next available free node
        Data_Node& node = allocate_data_node(value);

        // Obtain the inserted node (might be nullptr if node was a duplicate)
        inserted_node = insert_node(find_node(root_node, position.p_node), node);
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw map_full();
#else
        error_handler::error(map_full());
#endif
      }

      // Insert node into tree and return iterator to new node location in tree
      return iterator(*this, inserted_node);
    }

    //*********************************************************************
    /// Inserts a value to the map starting at the position recommended.
    /// If ETL_THROW_EXCEPTIONS is defined, emits map_full if the map is already full.
    ///\param position The position that would preceed the value to insert.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator position, const value_type& value)
    {
      // Default to no inserted node
      Node* inserted_node = nullptr;

      if (!full())
      {
        // Get next available free node
        Data_Node& node = allocate_data_node(value);

        // Obtain the inserted node (might be nullptr if node was a duplicate)
        inserted_node = insert_node(find_node(root_node, position.p_node), node);
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw map_full();
#else
        error_handler::error(map_full());
#endif
      }

      // Insert node into tree and return iterator to new node location in tree
      return iterator(*this, inserted_node);
    }

    //*********************************************************************
    /// Inserts a range of values to the map.
    /// If ETL_THROW_EXCEPTIONS is defined, emits map_full if the map does not have enough free space.
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
    iterator lower_bound(const key_value_parameter_t key)
    {
      return iterator(*this, lower_node(root_node, key));
    }

    //*********************************************************************
    /// Returns a const_iterator pointing to the first element in the
    /// container whose key is not considered to go before the key provided
    /// or end() if all keys are considered to go before the key provided.
    ///\return An const_iterator pointing to the element not before key or end()
    //*********************************************************************
    const_iterator lower_bound(const key_value_parameter_t key) const
    {
      return const_iterator(*this, lower_node(root_node, key));
    }

    //*********************************************************************
    /// Returns an iterator pointing to the first element in the container
    /// whose key is not considered to go after the key provided or end()
    /// if all keys are considered to go after the key provided.
    ///\return An iterator pointing to the element after key or end()
    //*********************************************************************
    iterator upper_bound(const key_value_parameter_t key)
    {
      return iterator(*this, upper_node(root_node, key));
    }

    //*********************************************************************
    /// Returns a const_iterator pointing to the first element in the
    /// container whose key is not considered to go after the key provided
    /// or end() if all keys are considered to go after the key provided.
    ///\return An const_iterator pointing to the element after key or end()
    //*********************************************************************
    const_iterator upper_bound(const key_value_parameter_t key) const
    {
      return const_iterator(*this, upper_node(root_node, key));
    }

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    imap(ipool<Data_Node>& node_pool, size_t max_size_)
      : map_base(max_size_)
      , p_node_pool(&node_pool)
      , root_node(nullptr)
    {
      initialise();
    }

  private:

    //*************************************************************************
    /// Allocate a Data_Node.
    //*************************************************************************
    Data_Node& allocate_data_node(value_type value) const
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
    /// Initialise the map.
    //*************************************************************************
    void initialise()
    {
      if (!empty())
      {
        p_node_pool->release_all();
      }

      current_size = 0;
      root_node = nullptr;
    }

    //*************************************************************************
    /// Attach the provided node to the position provided
    //*************************************************************************
    void attach_node(Node*& position, Data_Node& node)
    {
      // Mark new node as leaf on attach to tree at position provided
      node.mark_as_leaf();

      // Add the node here
      position = &node;

      // One more.
      ++current_size;
    }

    //*************************************************************************
    /// Detach the node at the position provided
    //*************************************************************************
    Node* detach_node(Node*& position)
    {
      // The node being detached
      Node* detached = position;

      // The node to be swapped with current position (might be nullptr)
      Node* swap_node = nullptr;

      // Found the node to be removed, does it have other nodes on the left?
      if (position->left)
      {
        // Find the upper node and its parent from the left of the current position
        Node* upper_parent_node = position;
        Node* upper_node = position->left;
        while (upper_node && upper_node->right)
        {
          upper_parent_node = upper_node;
          upper_node = upper_node->right;
        }

        // Recursively call detach_node for the upper node found above
        swap_node = detach_node(upper_parent_node->left == upper_node ?
          upper_parent_node->left : upper_parent_node->right);
      }
      // Found the node to be removed, does it have other nodes on the right?
      else if (position->right)
      {
        // Find the lower node and its parent from the right of the current position
        Node* lower_parent_node = position;
        Node* lower_node = position->right;
        while (lower_node && lower_node->left)
        {
          lower_parent_node = lower_node;
          lower_node = lower_node->left;
        }
        // Cast lower node into a data node to retrieve the key value
        Data_Node* lower_data_node = imap::data_cast(lower_node);

        // Recursively call detach_node for the lower node found
        swap_node = detach_node(lower_parent_node->left == lower_node ?
          lower_parent_node->left : lower_parent_node->right);
      }

      // If a swap node was provided above, update its child trees
      if (swap_node)
      {
        // Move children of position as new children of swap node
        swap_node->left = position->left;
        swap_node->right = position->right;
      }

      // Update current position to point to swap node
      position = swap_node;

      // Return the detached node
      return detached;
    }

    //*************************************************************************
    /// Find the value matching the node provided
    //*************************************************************************
    Node* find_node(Node* position, const key_value_parameter_t key)
    {
      Node* found = position;
      while (found)
      {
        // Downcast found to Data_Node class for comparison and other operations
        Data_Node& found_data_node = imap::data_cast(*found);

        // Compare the node value to the current position value
        if (node_comp(key, found_data_node))
        {
          // Keep searching for the node on the left
          found = found->left;
        }
        else if (node_comp(found_data_node, key))
        {
          // Keep searching for the node on the right
          found = found->right;
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
    const Node* find_node(const Node* position, const key_value_parameter_t key) const
    {
      const Node* found = position;
      while (found)
      {
        // Downcast found to Data_Node class for comparison and other operations
        const Data_Node& found_data_node = imap::data_cast(*found);

        // Compare the node value to the current position value
        if (node_comp(key, found_data_node))
        {
          // Keep searching for the node on the left
          found = found->left;
        }
        else if (node_comp(found_data_node, key))
        {
          // Keep searching for the node on the right
          found = found->right;
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
        if (found->left == node)
        {
          return found->left;
        }
        else if (found->right == node)
        {
          return found->right;
        }
        else
        {
          // Downcast found to Data_Node class for comparison and other operations
          Data_Node& found_data_node = imap::data_cast(*found);
          const Data_Node& data_node = imap::data_cast(*node);

          // Compare the node value to the current position value
          if (node_comp(data_node, found_data_node))
          {
            // Keep searching for the node on the left
            found = found->left;
          }
          else if (node_comp(found_data_node, data_node))
          {
            // Keep searching for the node on the right
            found = found->right;
          }
          else
          {
            // Exit loop if duplicate was found
            break;
          }
        }
      }

      // Return the initial position provided if node not found
      return position;
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
          if (position->left != node && position->right != node)
          {
            // Downcast node and position to Data_Node references for key comparisons
            const Data_Node& node_data_node = imap::data_cast(*node);
            Data_Node& position_data_node = imap::data_cast(*position);
            // Compare the node value to the current position value
            if (node_comp(node_data_node, position_data_node))
            {
              // Keep looking for parent on the left
              position = position->left;
            }
            else if (node_comp(position_data_node, node_data_node))
            {
              // Keep looking for parent on the right
              position = position->right;
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
          if (position->left != node && position->right != node)
          {
            // Downcast node and position to Data_Node references for key comparisons
            const Data_Node& node_data_node = imap::data_cast(*node);
            const Data_Node& position_data_node = imap::data_cast(*position);
            // Compare the node value to the current position value
            if (node_comp(node_data_node, position_data_node))
            {
              // Keep looking for parent on the left
              position = position->left;
            }
            else if (node_comp(position_data_node, node_data_node))
            {
              // Keep looking for parent on the right
              position = position->right;
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
    /// Insert a node.
    //*************************************************************************
    Node* insert_node(Node*& position, Data_Node& node)
    {
      // Find the location where the node belongs
      Node* found = position;

      // Was position provided not empty? then find where the node belongs
      if (position)
      {
        while (found)
        {
          // Downcast found to Data_Node class for comparison and other operations
          Data_Node& found_data_node = imap::data_cast(*found);

          // Is the node provided to the left of the current position?
          if (node_comp(node, found_data_node))
          {
            if (found->left)
            {
              // Node should be put on the left
              found = found->left;
            }
            else
            {
              // Attatch node to left
              attach_node(found->left, node);

              // Return newly added node
              found = found->left;

              // Exit loop
              break;
            }
          }
          // Is the node provided to the right of the current position?
          else if (node_comp(found_data_node, node))
          {
            if (found->right)
            {
              // Node should be put on the right
              found = found->right;
            }
            else
            {
              // Attatch node to right
              attach_node(found->right, node);

              // Return newly added node
              found = found->right;

              // Exit loop
              break;
            }
          }
          else
          {
            // Destroy the node provided (its a duplicate)
            destroy_data_node(node);

            // Exit loop, duplicate node found
            break;
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
    /// Find the node whose key would go before all the other keys from the
    /// position provided
    //*************************************************************************
    Node* lower_node(Node* position) const
    {
      // Something at this position and on the left? keep going
      Node* lower_node = position;
      while (lower_node && lower_node->left)
      {
        lower_node = lower_node->left;
      }

      // Return the lower node position found
      return lower_node;
    }

    //*************************************************************************
    /// Find the node whose key is not considered to go before the key provided
    //*************************************************************************
    Node* lower_node(Node* position, const key_value_parameter_t key) const
    {
      // Something at this position? keep going
      Node* lower_node = position;
      while (lower_node)
      {
        // Downcast lower node to Data_Node reference for key comparisons
        Data_Node& lower_data_node = imap::data_cast(*lower_node);
        // Compare the key value to the current lower node key value
        if (!node_comp(lower_data_node, key))
        {
          break;
        }
        else
        {
          lower_node = lower_node->right;
        }
      }

      // Return the lower node position found
      return lower_node;
    }

    //*************************************************************************
    /// Find the node whose key would go before all the other keys from the
    /// position provided
    //*************************************************************************
    const Node* lower_node(const Node* position) const
    {
      // Something at this position and on the left? keep going
      const Node* lower_node = position;
      while (lower_node && lower_node->left)
      {
        lower_node = lower_node->left;
      }

      // Return the lower node position found
      return lower_node;
    }

    //*************************************************************************
    /// Find the node whose key is not considered to go before the key provided
    //*************************************************************************
    const Node& lower_node(const Node* position, const key_value_parameter_t key) const
    {
      // Something at this position? keep going
      const Node* lower_node = position;
      while (lower_node)
      {
        // Downcast lower node to Data_Node reference for key comparisons
        const Data_Node& lower_data_node = imap::data_cast(*lower_node);
        // Compare the key value to the current lower node key value
        if (!node_comp(lower_data_node, key))
        {
          break;
        }
        else
        {
          lower_node = lower_node->right;
        }
      }

      // Return the lower node position found
      return lower_node;
    }

    //*************************************************************************
    /// Find the next node in sequence from the node provided
    //*************************************************************************
    void next_node(Node*&position)
    {
      if (position)
      {
        // Is there a tree on the right? then find the minimum of that tree
        if (position->right)
        {
          // Return minimum node found
          position = lower_node(position->right);
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
          } while (parent && parent->right == position);

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
        if (position->right)
        {
          // Return minimum node found
          position = lower_node(position->right);
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
          } while (parent && parent->right == position);

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
        position = upper_node(root_node);
      }
      else
      {
        // Is there a tree on the left? then find the maximum of that tree
        if (position->left)
        {
          // Return maximum node found
          position = upper_node(position->left);
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
          } while (parent && parent->left == position);

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
        position = upper_node(root_node);
      }
      else
      {
        // Is there a tree on the left? then find the maximum of that tree
        if (position->left)
        {
          // Return maximum node found
          position = upper_node(position->left);
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
          } while (parent && parent->left == position);

          // Set parent node as the next position
          position = parent;
        }
      }
    }

    //*************************************************************************
    /// Remove the node specified from somewhere starting at the position provided
    //*************************************************************************
    Node* remove_node(Node*& position, const key_value_parameter_t key)
    {
      // Step 1: Find the Node to be removed that matches the key provided
      Node* found_parent = nullptr;
      Node* found = position;
      while (found)
      {
        // Downcast found to Data_Node class for comparison and other operations
        Data_Node& found_data_node = imap::data_cast(*found);

        // Compare the node value to the current position value
        if (node_comp(key, found_data_node))
        {
          // Keep searching for the node to remove on the left
          found_parent = found;
          found = found->left;
        }
        else if (node_comp(found_data_node, key))
        {
          // Keep searching for the node to remove on the right
          found_parent = found;
          found = found->right;
        }
        else
        {
          // Node that matches the key provided was found, exit loop
          break;
        }
      }

      // Step 2: If the node was found, begin the remove process
      if (found)
      {
        // Does found and position point to the same memory address? then proceed with remove
        if (found == position)
        {
          // Detach the node at the current position
          detach_node(position);
        }
        else
        {
          // Detach the node using a reference provided by the parent of the node
          // found
          detach_node(found_parent->left == found ?
            found_parent->left : found_parent->right);
        }

        // Downcast found into data node
        Data_Node& found_data_node = imap::data_cast(*found);

        // One less.
        --current_size;

        // Destroy the node removed
        destroy_data_node(found_data_node);
      }

      // If this is reached, the key value was not found and removed
      return nullptr;
    }

    //*************************************************************************
    /// Find the node whose key would go before all the other keys from the
    /// position provided
    //*************************************************************************
    Node* upper_node(Node* position) const
    {
      // Something at this position and on the right? keep going
      Node* upper_node = position;
      while (upper_node && upper_node->right)
      {
        upper_node = upper_node->right;
      }

      // Return the upper node position found
      return upper_node;
    }

    //*************************************************************************
    /// Find the node whose key is considered to go before the key provided
    //*************************************************************************
    Node* upper_node(Node* position, const key_value_parameter_t key) const
    {
      // Something at this position and on the right? keep going
      Node* upper_node = position;
      while (upper_node)
      {
        // Downcast position to Data_Node reference for key comparisons
        Data_Node& upper_data_node = imap::data_cast(*upper_node);
        // Compare the key value to the current upper node key value
        if (!node_comp(key, upper_data_node))
        {
          upper_node = upper_node->right;
        }
        else
        {
          break;
        }
      }

      // Return the upper node position found
      return upper_node;
    }

    //*************************************************************************
    /// Find the node whose key would go before all the other keys from the
    /// position provided
    //*************************************************************************
    const Node* upper_node(const Node* position) const
    {
      // Something at this position and on the right? keep going
      Node* upper_node = position;
      while (upper_node && upper_node->right)
      {
        upper_node = upper_node->right;
      }

      // Return the upper node position found
      return upper_node;
    }

    //*************************************************************************
    /// Find the node whose key is considered to go before the key provided
    //*************************************************************************
    const Node* upper_node(const Node* position, const key_value_parameter_t key) const
    {
      // Something at this position and on the right? keep going
      const Node* upper_node = position;
      while (upper_node)
      {
        // Downcast position to Data_Node reference for key comparisons
        const Data_Node& upper_data_node = imap::data_cast(*upper_node);
        // Compare the key value to the current upper node key value
        if (!node_comp(key, upper_data_node))
        {
          upper_node = upper_node->right;
        }
        else
        {
          break;
        }
      }

      // Return the upper node position found
      return upper_node;
    }

  };
}

//***************************************************************************
/// Equal operator.
///\param lhs Reference to the first lookup.
///\param rhs Reference to the second lookup.
///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
///\ingroup lookup
//***************************************************************************
template <typename TKey, typename TMapped, typename TKeyCompare>
bool operator ==(const etl::imap<TKey, TMapped, TKeyCompare>& lhs, const etl::imap<TKey, TMapped, TKeyCompare>& rhs)
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
template <typename TKey, typename TMapped, typename TKeyCompare>
bool operator !=(const etl::imap<TKey, TMapped, TKeyCompare>& lhs, const etl::imap<TKey, TMapped, TKeyCompare>& rhs)
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
template <typename TKey, typename TMapped, typename TKeyCompare>
bool operator <(const etl::imap<TKey, TMapped, TKeyCompare>& lhs, const etl::imap<TKey, TMapped, TKeyCompare>& rhs)
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
template <typename TKey, typename TMapped, typename TKeyCompare>
bool operator >(const etl::imap<TKey, TMapped, TKeyCompare>& lhs, const etl::imap<TKey, TMapped, TKeyCompare>& rhs)
{
  return std::lexicographical_compare(lhs.begin(),
                                      lhs.end(),
                                      rhs.begin(),
                                      rhs.end(),
                                      std::greater<TMapped>());
}

//*************************************************************************
/// Less than or equal operator.
///\param lhs Reference to the first list.
///\param rhs Reference to the second list.
///\return <b>true</b> if the first list is lexicographically less than or equal
/// to the second, otherwise <b>false</b>.
//*************************************************************************
template <typename TKey, typename TMapped, typename TKeyCompare>
bool operator <=(const etl::imap<TKey, TMapped, TKeyCompare>& lhs, const etl::imap<TKey, TMapped, TKeyCompare>& rhs)
{
  return !operator >(lhs, rhs);
}

//*************************************************************************
/// Greater than or equal operator.
///\param lhs Reference to the first list.
///\param rhs Reference to the second list.
///\return <b>true</b> if the first list is lexicographically greater than or
/// equal to the second, otherwise <b>false</b>.
//*************************************************************************
template <typename TKey, typename TMapped, typename TKeyCompare>
bool operator >=(const etl::imap<TKey, TMapped, TKeyCompare>& lhs, const etl::imap<TKey, TMapped, TKeyCompare>& rhs)
{
  return !operator <(lhs, rhs);
}

#if WIN32
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#undef __ETL_IN_IMAP_H__

#endif
