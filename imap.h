///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

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

    typedef TKey                           key_type;
    typedef std::pair<const TKey, TMapped> value_type;
    typedef TMapped                        mapped_type;
    typedef TKeyCompare                    key_compare;
    typedef value_type&                    reference;
    typedef const value_type&              const_reference;
    typedef value_type*                    pointer;
    typedef const value_type*              const_pointer;
    typedef size_t                         size_type;

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
    static const uint8_t kLeft = 0;
    static const uint8_t kRight = 1;
    static const uint8_t kNeither = 2;

    //*************************************************************************
    /// The node element in the map.
    //*************************************************************************
    struct Node
    {
      //***********************************************************************
      /// Constructor
      //***********************************************************************
      Node() :
        weight(kNeither),
        dir(kNeither)
      {
      }

      //***********************************************************************
      /// Marks the node as a leaf.
      //***********************************************************************
      void mark_as_leaf()
      {
        weight = kNeither;
        dir = kNeither;
        children[0] = nullptr;
        children[1] = nullptr;
      }

      Node* children[2];
      uint8_t weight;
      uint8_t dir;
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
    typedef typename parameter_type<TKey>::type key_value_parameter_t;

    //*************************************************************************
    /// How to compare node elements.
    //*************************************************************************
    bool node_comp(const Data_Node& node1, const Data_Node& node2) const
    {
      return key_compare()(node1.value.first, node2.value.first);
    }
    bool node_comp(const Data_Node& node, const key_value_parameter_t& key) const
    {
      return key_compare()(node.value.first, key);
    }
    bool node_comp(const key_value_parameter_t& key, const Data_Node& node) const
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
    /// Gets the beginning of the map.
    //*************************************************************************
    iterator begin()
    {
      return iterator(*this, find_limit_node(root_node, kLeft));
    }

    //*************************************************************************
    /// Gets the beginning of the map.
    //*************************************************************************
    const_iterator begin() const
    {
      return const_iterator(*this, find_limit_node(root_node, kLeft));
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
      return const_iterator(*this, find_limit_node(root_node, kLeft));
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
    /// Returns a reference to the value at index 'key'
    ///\param i The index.
    ///\return A reference to the value at index 'key'
    //*********************************************************************
    mapped_type& operator [](const key_value_parameter_t& key)
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
    mapped_type& at(const key_value_parameter_t& key)
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
    const mapped_type& at(const key_value_parameter_t& key) const
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
    size_type count(const key_value_parameter_t& key) const
    {
      return find_node(root_node, key) ? 1 : 0;
    }

    //*************************************************************************
    /// Returns two iterators with bounding (lower bound, upper bound) the key
    /// provided
    //*************************************************************************
    std::pair<iterator, iterator> equal_range(const key_value_parameter_t& key)
    {
      return std::make_pair<iterator, iterator>(
        iterator(*this, find_lower_node(root_node, key)),
        iterator(*this, find_upper_node(root_node, key)));
    }

    //*************************************************************************
    /// Returns two const iterators with bounding (lower bound, upper bound)
    /// the key provided.
    //*************************************************************************
    std::pair<const_iterator, const_iterator> equal_range(const key_value_parameter_t& key) const
    {
      return std::make_pair<const_iterator, const_iterator>(
        const_iterator(*this, find_lower_node(root_node, key)),
        const_iterator(*this, find_upper_node(root_node, key)));
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
      Node*& reference_node = find_node(root_node, position.p_node);
      iterator next(*this, reference_node);
      ++next;

      remove_node(root_node, (*position).first);

      return next;
    }

    //*************************************************************************
    // Erase the key specified.
    //*************************************************************************
    size_type erase(const key_value_parameter_t& key)
    {
      // Return 1 if key value was found and removed
      return remove_node(root_node, key) ? 1 : 0;
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
    iterator find(const key_value_parameter_t& key)
    {
      return iterator(*this, find_node(root_node, key));
    }

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator pointing to the element or end() if not found.
    //*********************************************************************
    const_iterator find(const key_value_parameter_t& key) const
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
    ///\param position The position that would precede the value to insert.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(iterator, const value_type& value)
    {
      // Default to no inserted node
      Node* inserted_node = nullptr;

      if (!full())
      {
        // Get next available free node
        Data_Node& node = allocate_data_node(value);

        // Obtain the inserted node (might be nullptr if node was a duplicate)
        inserted_node = insert_node(root_node, node);
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
    ///\param position The position that would precede the value to insert.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator, const value_type& value)
    {
      // Default to no inserted node
      Node* inserted_node = nullptr;

      if (!full())
      {
        // Get next available free node
        Data_Node& node = allocate_data_node(value);

        // Obtain the inserted node (might be nullptr if node was a duplicate)
        inserted_node = insert_node(root_node, node);
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
    /// Balance the critical node at the position provided as needed
    //*************************************************************************
    void balance_node(Node*& critical_node)
    {
      // Step 1: Update weights for all children of the critical node up to the
      // newly inserted node. This step is costly (in terms of traversing nodes
      // multiple times during insertion) but doesn't require as much recursion
      Node* weight_node = critical_node->children[critical_node->dir];
      while (weight_node)
      {
        // Keep going until we reach a terminal node (dir == kNeither)
        if (kNeither != weight_node->dir)
        {
          // Does this insert balance the previous weight factor value?
          if (weight_node->weight == 1 - weight_node->dir)
          {
            weight_node->weight = kNeither;
          }
          else
          {
            weight_node->weight = weight_node->dir;
          }

          // Update weight factor node to point to next node
          weight_node = weight_node->children[weight_node->dir];
        }
        else
        {
          // Stop loop, terminal node found
          break;
        }
      } // while(weight_node)

      // Step 2: Update weight for critical_node or rotate tree to balance node
      if (kNeither == critical_node->weight)
      {
        critical_node->weight = critical_node->dir;
      }
      // If direction is different than weight, then it will now be balanced
      else if (critical_node->dir != critical_node->weight)
      {
        critical_node->weight = kNeither;
      }
      // Rotate is required to balance the tree at the critical node
      else
      {
        // If critical node matches child node direction then perform a two
        // node rotate in the direction of the critical node
        if (critical_node->weight == critical_node->children[critical_node->dir]->dir)
        {
          rotate_2node(critical_node, critical_node->dir);
        }
        // Otherwise perform a three node rotation in the direction of the
        // critical node
        else
        {
          rotate_3node(critical_node, critical_node->dir,
            critical_node->children[critical_node->dir]->children[1 - critical_node->dir]->dir);
        }
      }
    }

    //*************************************************************************
    /// Detach the node at the position provided
    //*************************************************************************
    void detach_node(Node*& position, Node*& replacement)
    {
      // Make temporary copy of actual nodes involved because we might lose
      // their references in the process (e.g. position is the same as
      // replacement or replacement is a child of position)
      Node* detached = position;
      Node* swap = replacement;

      // Update current position to point to swap (replacement) node first
      position = swap;

      // Update replacement node to point to child in opposite direction
      // otherwise we might lose the other child of the swap node
      replacement = swap->children[1 - swap->dir];

      // Point swap node to detached node's children and weight
      swap->children[kLeft] = detached->children[kLeft];
      swap->children[kRight] = detached->children[kRight];
      swap->weight = detached->weight;
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
        Data_Node& found_data_node = imap::data_cast(*found);

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
        const Data_Node& found_data_node = imap::data_cast(*found);

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
          Data_Node& found_data_node = imap::data_cast(*found);
          const Data_Node& data_node = imap::data_cast(*node);

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
    /// Find the node whose key would go before all the other keys from the
    /// position provided
    //*************************************************************************
    Node* find_limit_node(Node* position, const int8_t dir) const
    {
      // Something at this position and in the direction specified? keep going
      Node* limit_node = position;
      while (limit_node && limit_node->children[dir])
      {
        limit_node = limit_node->children[dir];
      }

      // Return the limit node position found
      return limit_node;
    }

    //*************************************************************************
    /// Find the node whose key would go before all the other keys from the
    /// position provided
    //*************************************************************************
    const Node* find_limit_node(const Node* position, const int8_t dir) const
    {
      // Something at this position and in the direction specified? keep going
      Node* limit_node = position;
      while (limit_node && limit_node->children[dir])
      {
        limit_node = limit_node->children[dir];
      }

      // Return the limit node position found
      return limit_node;
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
            const Data_Node& node_data_node = imap::data_cast(*node);
            Data_Node& position_data_node = imap::data_cast(*position);
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
            const Data_Node& node_data_node = imap::data_cast(*node);
            const Data_Node& position_data_node = imap::data_cast(*position);
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
        Data_Node& data_node = imap::data_cast(*lower_node);
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
        Data_Node& data_node = imap::data_cast(*node);
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
          Data_Node& found_data_node = imap::data_cast(*found);

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
        Data_Node& replace_data_node = imap::data_cast(*replace);

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
        Data_Node& found_data_node = imap::data_cast(*found);

        // One less.
        --current_size;

        // Destroy the node removed
        destroy_data_node(found_data_node);
      } // if(found)

      // Return node found (might be nullptr)
      return found;
    }

    //*************************************************************************
    /// Rotate two nodes at the position provided the to balance the tree
    //*************************************************************************
    void rotate_2node(Node*& position, uint8_t dir)
    {
      //     A            C             A          B
      //   B   C   ->   A   E   OR    B   C  ->  D   A
      //      D E      B D           D E            E C
      // C (new position) becomes the root
      // A (position) takes ownership of D as its children[kRight] child
      // C (new position) takes ownership of A as its left child
      //                 OR
      // B (new position) becomes the root
      // A (position) takes ownership of E as its left child
      // B (new position) takes ownership of A as its right child

      // Capture new root
      Node* new_root = position->children[dir];
      // Replace position's previous child with new root's other child
      position->children[dir] = new_root->children[1 - dir];
      // New root now becomes parent of current position
      new_root->children[1 - dir] = position;
      // Clear weight factor from current position
      position->weight = kNeither;
      // Newly detached right now becomes current position
      position = new_root;
      // Clear weight factor from new root
      position->weight = kNeither;
    }

    //*************************************************************************
    /// Rotate three nodes at the position provided the to balance the tree
    //*************************************************************************
    void rotate_3node(Node*& position, uint8_t dir, uint8_t third)
    {
      //        __A__             __E__            __A__             __D__
      //      _B_    C    ->     B     A    OR    B    _C_   ->     A     C
      //     D   E              D F   G C             D   E        B F   G E
      //        F G                                  F G
      // E (new position) becomes the root
      // B (position) takes ownership of F as its left child
      // A takes ownership of G as its right child
      //                  OR
      // D (new position) becomes the root
      // A (position) takes ownership of F as its right child
      // C takes ownership of G as its left child

      // Capture new root (either E or D depending on dir)
      Node* new_root = position->children[dir]->children[1 - dir];
      // Set weight factor for B or C based on F or G existing and being a different than dir
      position->children[dir]->weight = third != kNeither && third != dir ? dir : kNeither;

      // Detach new root from its tree (replace with new roots child)
      position->children[dir]->children[1 - dir] =
        new_root->children[dir];
      // Attach current left tree to new root
      new_root->children[dir] = position->children[dir];
      // Set weight factor for A based on F or G
      position->weight = third != kNeither && third == dir ? 1 - dir : kNeither;

      // Move new root's right tree to current roots left tree
      position->children[dir] = new_root->children[1 - dir];
      // Attach current root to new roots right tree
      new_root->children[1 - dir] = position;
      // Replace current position with new root
      position = new_root;
      // Clear weight factor for new current position
      position->weight = kNeither;
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
