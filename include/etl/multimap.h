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

#ifndef ETL_MULTIMAP_INCLUDED
#define ETL_MULTIMAP_INCLUDED

#include <stddef.h>

#include <new>

#include "platform.h"

#include "stl/algorithm.h"
#include "stl/iterator.h"
#include "stl/functional.h"

#include "container.h"
#include "pool.h"
#include "exception.h"
#include "error_handler.h"
#include "debug_count.h"
#include "nullptr.h"
#include "type_traits.h"
#include "parameter_type.h"

#if ETL_CPP11_SUPPORTED && !defined(ETL_STLPORT) && !defined(ETL_NO_STL)
  #include <initializer_list>
#endif

#include "private/minmax_push.h"

#undef ETL_FILE
#define ETL_FILE "9"

//*****************************************************************************
/// A multimap with the capacity defined at compile time.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// Exception for the map.
  ///\ingroup map
  //***************************************************************************
  class multimap_exception : public etl::exception
  {
  public:

    multimap_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Full exception for the map.
  ///\ingroup map
  //***************************************************************************
  class multimap_full : public etl::multimap_exception
  {
  public:

    multimap_full(string_type file_name_, numeric_type line_number_)
      : etl::multimap_exception("multimap:full", file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Map out of bounds exception.
  ///\ingroup map
  //***************************************************************************
  class multimap_out_of_bounds : public etl::multimap_exception
  {
  public:

    multimap_out_of_bounds(string_type file_name_, numeric_type line_number_)
      : etl::multimap_exception("multimap:bounds", file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Iterator exception for the map.
  ///\ingroup map
  //***************************************************************************
  class multimap_iterator : public etl::multimap_exception
  {
  public:

    multimap_iterator(string_type file_name_, numeric_type line_number_)
      : etl::multimap_exception("multimap:iterator", file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The base class for all maps.
  ///\ingroup map
  //***************************************************************************
  class multimap_base
  {
  public:

    typedef size_t size_type; ///< The type used for determining the size of map.

    //*************************************************************************
    /// Gets the size of the map.
    //*************************************************************************
    size_type size() const
    {
      return current_size;
    }

    //*************************************************************************
    /// Gets the maximum possible size of the map.
    //*************************************************************************
    size_type max_size() const
    {
      return CAPACITY;
    }

    //*************************************************************************
    /// Checks to see if the map is empty.
    //*************************************************************************
    bool empty() const
    {
      return current_size == 0;
    }

    //*************************************************************************
    /// Checks to see if the map is full.
    //*************************************************************************
    bool full() const
    {
      return current_size == CAPACITY;
    }

    //*************************************************************************
    /// Returns the capacity of the vector.
    ///\return The capacity of the vector.
    //*************************************************************************
    size_type capacity() const
    {
      return CAPACITY;
    }

    //*************************************************************************
    /// Returns the remaining capacity.
    ///\return The remaining capacity.
    //*************************************************************************
    size_t available() const
    {
      return max_size() - size();
    }

  protected:

    enum
    {
      kLeft,
      kRight,
      kNeither
    };

    //*************************************************************************
    /// The node element in the multimap.
    //*************************************************************************
    struct Node
    {
      //***********************************************************************
      /// Constructor
      //***********************************************************************
      Node() :
        weight((uint_least8_t) kNeither),
        dir((uint_least8_t) kNeither)
      {
      }

      //***********************************************************************
      /// Marks the node as a leaf.
      //***********************************************************************
      void mark_as_leaf()
      {
        weight = (uint_least8_t) kNeither;
        dir = (uint_least8_t) kNeither;
        parent = nullptr;
        children[0] = nullptr;
        children[1] = nullptr;
      }

      Node* parent;
      Node* children[2];
      uint_least8_t weight;
      uint_least8_t dir;
    };

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    multimap_base(size_type max_size_)
      : current_size(0)
      , CAPACITY(max_size_)
      , root_node(nullptr)
    {
    }

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    ~multimap_base()
    {
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
        // Keep going until we reach a terminal node (dir == (uint_least8_t) kNeither)
        if ((uint_least8_t) kNeither != weight_node->dir)
        {
          // Does this insert balance the previous weight factor value?
          if (weight_node->weight == 1 - weight_node->dir)
          {
            weight_node->weight = (uint_least8_t) kNeither;
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
      if ((uint_least8_t) kNeither == critical_node->weight)
      {
        critical_node->weight = critical_node->dir;
      }
      // If direction is different than weight, then it will now be balanced
      else if (critical_node->dir != critical_node->weight)
      {
        critical_node->weight = (uint_least8_t) kNeither;
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
    /// Rotate two nodes at the position provided the to balance the tree
    //*************************************************************************
    void rotate_2node(Node*& position, uint_least8_t dir)
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

      // Capture new root (either B or C depending on dir) and its parent
      Node* new_root = position->children[dir];

      // Replace position's previous child with new root's other child
      position->children[dir] = new_root->children[1 - dir];
      // Update new root's other child parent pointer
      if (position->children[dir])
      {
        position->children[dir]->parent = position;
      }

      // New root's parent becomes current position's parent
      new_root->parent = position->parent;
      new_root->children[1 - dir] = position;
      new_root->dir = 1 - dir;

      // Clear weight factor from current position
      position->weight = (uint_least8_t) kNeither;
      // Position's parent becomes new_root
      position->parent = new_root;
      position = new_root;
      // Clear weight factor from new root
      position->weight = (uint_least8_t) kNeither;
    }

    //*************************************************************************
    /// Rotate three nodes at the position provided the to balance the tree
    //*************************************************************************
    void rotate_3node(Node*& position, uint_least8_t dir, uint_least8_t third)
    {
      //        --A--             --E--            --A--             --D--
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
      position->children[dir]->weight = third != (uint_least8_t) kNeither && third != dir ? dir : (uint_least8_t) kNeither;

      // Detach new root from its tree (replace with new roots child)
      position->children[dir]->children[1 - dir] = new_root->children[dir];
      // Update new roots child parent pointer
      if (new_root->children[dir])
      {
        new_root->children[dir]->parent = position->children[dir];
      }

      // Attach current left tree to new root and update its parent
      new_root->children[dir] = position->children[dir];
      position->children[dir]->parent = new_root;

      // Set weight factor for A based on F or G
      position->weight = third != (uint_least8_t) kNeither && third == dir ? 1 - dir : (uint_least8_t) kNeither;

      // Move new root's right tree to current roots left tree
      position->children[dir] = new_root->children[1 - dir];
      if (new_root->children[1 - dir])
      {
        new_root->children[1 - dir]->parent = position;
      }

      // Attach current root to new roots right tree and assume its parent
      new_root->parent = position->parent;
      new_root->children[1 - dir] = position;
      new_root->dir = 1 - dir;

      // Update current position's parent and replace with new root
      position->parent = new_root;
      position = new_root;
      // Clear weight factor for new current position
      position->weight = (uint_least8_t) kNeither;
    }

    //*************************************************************************
    /// Find the next node in sequence from the node provided
    //*************************************************************************
    void next_node(Node*& position) const
    {
      if (position)
      {
        // Is there a tree on the right? then find the minimum of that tree
        if (position->children[(uint_least8_t) kRight])
        {
          // Return minimum node found
          position = find_limit_node(position->children[(uint_least8_t) kRight], kLeft);
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
            parent = position->parent; // find_parent_node(root_node, position);
                                       // Repeat while previous position was on right side of parent tree
          } while (parent && parent->children[(uint_least8_t) kRight] == position);

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
        if (position->children[(uint_least8_t) kRight])
        {
          // Return minimum node found
          position = find_limit_node(position->children[(uint_least8_t) kRight], kLeft);
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
            parent = position->parent;
            // Repeat while previous position was on right side of parent tree
          } while (parent && parent->children[(uint_least8_t) kRight] == position);

          // Set parent node as the next position
          position = parent;
        }
      }
    }

    //*************************************************************************
    /// Find the previous node in sequence from the node provided
    //*************************************************************************
    void prev_node(Node*& position) const
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
        if (position->children[(uint_least8_t) kLeft])
        {
          // Return maximum node found
          position = find_limit_node(position->children[(uint_least8_t) kLeft], kRight);
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
            parent = position->parent;
            // Repeat while previous position was on left side of parent tree
          } while (parent && parent->children[(uint_least8_t) kLeft] == position);

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
        if (position->children[(uint_least8_t) kLeft])
        {
          // Return maximum node found
          position = find_limit_node(position->children[(uint_least8_t) kLeft], kRight);
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
            parent = position->parent;
            // Repeat while previous position was on left side of parent tree
          } while (parent && parent->children[(uint_least8_t) kLeft] == position);

          // Set parent node as the next position
          position = parent;
        }
      }
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
    /// Attach the provided node to the position provided
    //*************************************************************************
    void attach_node(Node* parent, Node*& position, Node& node)
    {
      // Mark new node as leaf on attach to tree at position provided
      node.mark_as_leaf();

      // Keep track of this node's parent
      node.parent = parent;

      // Add the node here
      position = &node;

      // One more.
      ++current_size;
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

      // Point swap node to detached node's parent, children and weight
      swap->parent = detached->parent;
      swap->children[(uint_least8_t) kLeft] = detached->children[(uint_least8_t) kLeft];
      swap->children[(uint_least8_t) kRight] = detached->children[(uint_least8_t) kRight];
      if (swap->children[(uint_least8_t) kLeft])
      {
        swap->children[(uint_least8_t) kLeft]->parent = swap;
      }
      if (swap->children[(uint_least8_t) kRight])
      {
        swap->children[(uint_least8_t) kRight]->parent = swap;
      }
      swap->weight = detached->weight;
    }

    size_type current_size;   ///< The number of the used nodes.
    const size_type CAPACITY; ///< The maximum size of the map.
    Node* root_node;          ///< The node that acts as the multimap root.
    ETL_DECLARE_DEBUG_COUNT
  };

  //***************************************************************************
  /// A templated base for all etl::multimap types.
  ///\ingroup map
  //***************************************************************************
  template <typename TKey, typename TMapped, typename TKeyCompare = std::less<TKey> >
  class imultimap : public etl::multimap_base
  {
  public:

    typedef std::pair<const TKey, TMapped> value_type;
    typedef const TKey                     key_type;
    typedef TMapped                        mapped_type;
    typedef TKeyCompare                    key_compare;
    typedef value_type&                    reference;
    typedef const value_type&              const_reference;
    typedef value_type*                    pointer;
    typedef const value_type*              const_pointer;
    typedef size_t                         size_type;

    class value_compare
    {
    public:

      bool operator()(const value_type& lhs, const value_type& rhs) const
      {
        return (kcompare(lhs.first, rhs.first));
      }

    private:

      key_compare kcompare;
    };

  protected:

    //*************************************************************************
    /// The data node element in the multimap.
    //*************************************************************************
    struct Data_Node : public Node
    {
      explicit Data_Node(value_type value_)
        : value(value_)
      {
      }

      value_type value;
    };

    /// Defines the key value parameter type
    typedef typename etl::parameter_type<TKey>::type key_parameter_t;

    //*************************************************************************
    /// How to compare node elements.
    //*************************************************************************
    bool node_comp(const Data_Node& node1, const Data_Node& node2) const
    {
      return kcompare(node1.value.first, node2.value.first);
    }

    bool node_comp(const Data_Node& node, key_parameter_t key) const
    {
      return kcompare(node.value.first, key);
    }

    bool node_comp(key_parameter_t key, const Data_Node& node) const
    {
      return kcompare(key, node.value.first);
    }

  private:

    /// The pool of data nodes used in the multimap.
    ipool* p_node_pool;

    key_compare   kcompare;
    value_compare vcompare;

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

      friend class imultimap;

      iterator()
        : p_multimap(nullptr)
        , p_node(nullptr)
      {
      }

      iterator(imultimap& multimap)
        : p_multimap(&multimap)
        , p_node(nullptr)
      {
      }

      iterator(imultimap& multimap, Node* node)
        : p_multimap(&multimap)
        , p_node(node)
      {
      }

      iterator(const iterator& other)
        : p_multimap(other.p_multimap)
        , p_node(other.p_node)
      {
      }

      ~iterator()
      {
      }

      iterator& operator ++()
      {
        p_multimap->next_node(p_node);
        return *this;
      }

      iterator operator ++(int)
      {
        iterator temp(*this);
        p_multimap->next_node(p_node);
        return temp;
      }

      iterator& operator --()
      {
        p_multimap->prev_node(p_node);
        return *this;
      }

      iterator operator --(int)
      {
        iterator temp(*this);
        p_multimap->prev_node(p_node);
        return temp;
      }

      iterator operator =(const iterator& other)
      {
        p_multimap = other.p_multimap;
        p_node = other.p_node;
        return *this;
      }

      reference operator *()
      {
        return imultimap::data_cast(p_node)->value;
      }

      const_reference operator *() const
      {
        return imultimap::data_cast(p_node)->value;
      }

      pointer operator &()
      {
        return &(imultimap::data_cast(p_node)->value);
      }

      const_pointer operator &() const
      {
        return &(imultimap::data_cast(p_node)->value);
      }

      pointer operator ->()
      {
        return &(imultimap::data_cast(p_node)->value);
      }

      const_pointer operator ->() const
      {
        return &(imultimap::data_cast(p_node)->value);
      }

      friend bool operator == (const iterator& lhs, const iterator& rhs)
      {
        return lhs.p_multimap == rhs.p_multimap && lhs.p_node == rhs.p_node;
      }

      friend bool operator != (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      // Pointer to multimap associated with this iterator
      imultimap* p_multimap;

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

      friend class imultimap;

      const_iterator()
        : p_multimap(nullptr)
        , p_node(nullptr)
      {
      }

      const_iterator(const imultimap& multimap)
        : p_multimap(&multimap)
        , p_node(nullptr)
      {
      }

      const_iterator(const imultimap& multimap, const Node* node)
        : p_multimap(&multimap)
        , p_node(node)
      {
      }

      const_iterator(const typename imultimap::iterator& other)
        : p_multimap(other.p_multimap)
        , p_node(other.p_node)
      {
      }

      const_iterator(const const_iterator& other)
        : p_multimap(other.p_multimap)
        , p_node(other.p_node)
      {
      }

      ~const_iterator()
      {
      }

      const_iterator& operator ++()
      {
        p_multimap->next_node(p_node);
        return *this;
      }

      const_iterator operator ++(int)
      {
        const_iterator temp(*this);
        p_multimap->next_node(p_node);
        return temp;
      }

      const_iterator& operator --()
      {
        p_multimap->prev_node(p_node);
        return *this;
      }

      const_iterator operator --(int)
      {
        const_iterator temp(*this);
        p_multimap->prev_node(p_node);
        return temp;
      }

      const_iterator operator =(const const_iterator& other)
      {
        p_multimap = other.p_multimap;
        p_node = other.p_node;
        return *this;
      }

      const_reference operator *() const
      {
        return imultimap::data_cast(p_node)->value;
      }

      const_pointer operator &() const
      {
        return imultimap::data_cast(p_node)->value;
      }

      const_pointer operator ->() const
      {
        return &(imultimap::data_cast(p_node)->value);
      }

      friend bool operator == (const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.p_multimap == rhs.p_multimap && lhs.p_node == rhs.p_node;
      }

      friend bool operator != (const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:
      // Pointer to multimap associated with this iterator
      const imultimap* p_multimap;

      // Pointer to the current node for this iterator
      const Node* p_node;
    };
    friend class const_iterator;

    typedef typename std::iterator_traits<iterator>::difference_type difference_type;

    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    //*************************************************************************
    /// Gets the beginning of the multimap.
    //*************************************************************************
    iterator begin()
    {
      return iterator(*this, find_limit_node(root_node, kLeft));
    }

    //*************************************************************************
    /// Gets the beginning of the multimap.
    //*************************************************************************
    const_iterator begin() const
    {
      return const_iterator(*this, find_limit_node(root_node, kLeft));
    }

    //*************************************************************************
    /// Gets the end of the multimap.
    //*************************************************************************
    iterator end()
    {
      return iterator(*this);
    }

    //*************************************************************************
    /// Gets the end of the multimap.
    //*************************************************************************
    const_iterator end() const
    {
      return const_iterator(*this);
    }

    //*************************************************************************
    /// Gets the beginning of the multimap.
    //*************************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(*this, find_limit_node(root_node, kLeft));
    }

    //*************************************************************************
    /// Gets the end of the multimap.
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
    /// Assigns values to the multimap.
    /// If asserts or exceptions are enabled, emits map_full if the multimap does not have enough free space.
    /// If asserts or exceptions are enabled, emits map_iterator if the iterators are reversed.
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
    /// Clears the multimap.
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
    size_type count(key_parameter_t key) const
    {
      return count_nodes(key);
    }

    //*************************************************************************
    /// Returns two iterators with bounding (lower bound, upper bound) the key
    /// provided
    //*************************************************************************
    std::pair<iterator, iterator> equal_range(key_parameter_t key)
    {
      return std::make_pair<iterator, iterator>(
        iterator(*this, find_lower_node(root_node, key)),
        iterator(*this, find_upper_node(root_node, key)));
    }

    //*************************************************************************
    /// Returns two const iterators with bounding (lower bound, upper bound)
    /// the key provided.
    //*************************************************************************
    std::pair<const_iterator, const_iterator> equal_range(key_parameter_t key) const
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
      // Remove the node by its node specified in iterator position
      (void)erase(const_iterator(position));
    }

    //*************************************************************************
    /// Erases the value at the specified position.
    //*************************************************************************
    iterator erase(const_iterator position)
    {
      // Cast const away from node to be removed. This is necessary because the
      // STL definition of this method requires we provide the next node in the
      // sequence as an iterator.
      Node* node = const_cast<Node*>(position.p_node);
      iterator next(*this, node);
      ++next;

      // Remove the non-const node provided
      remove_node(node);

      return next;
    }

    //*************************************************************************
    // Erase the key specified.
    //*************************************************************************
    size_type erase(key_parameter_t key)
    {
      // Number of nodes removed
      size_type d = 0;
      const_iterator lower(*this, find_lower_node(root_node, key));
      const_iterator upper(*this, find_upper_node(root_node, key));
      while (lower != upper)
      {
        // Increment count for each node removed
        ++d;
        // Remove node using the other erase method
        (void)erase(lower++);
      }

      // Return the total count erased
      return d;
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
    iterator find(key_parameter_t key)
    {
      return iterator(*this, find_node(root_node, key));
    }

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator pointing to the element or end() if not found.
    //*********************************************************************
    const_iterator find(key_parameter_t key) const
    {
      return const_iterator(*this, find_node(root_node, key));
    }

    //*********************************************************************
    /// Inserts a value to the multimap.
    /// If asserts or exceptions are enabled, emits map_full if the multimap is already full.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const value_type& value)
    {
      // Default to no inserted node
      Node* inserted_node = nullptr;

      ETL_ASSERT(!full(), ETL_ERROR(multimap_full));

      // Get next available free node
      Data_Node& node = allocate_data_node(value);

      // Obtain the inserted node (might be nullptr if node was a duplicate)
      inserted_node = insert_node(root_node, node);

      // Insert node into tree and return iterator to new node location in tree
      return iterator(*this, inserted_node);
    }

    //*********************************************************************
    /// Inserts a value to the multimap starting at the position recommended.
    /// If asserts or exceptions are enabled, emits map_full if the multimap is already full.
    ///\param position The position that would precede the value to insert.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(iterator /*position*/, const value_type& value)
    {
      // Ignore position provided and just do a normal insert
      return insert(value);
    }

    //*********************************************************************
    /// Inserts a value to the multimap starting at the position recommended.
    /// If asserts or exceptions are enabled, emits map_full if the multimap is already full.
    ///\param position The position that would precede the value to insert.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator /*position*/, const value_type& value)
    {
      // Ignore position provided and just do a normal insert
      return insert(value);
    }

    //*********************************************************************
    /// Inserts a range of values to the multimap.
    /// If asserts or exceptions are enabled, emits map_full if the multimap does not have enough free space.
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
    iterator lower_bound(key_parameter_t key)
    {
      return iterator(*this, find_lower_node(root_node, key));
    }

    //*********************************************************************
    /// Returns a const_iterator pointing to the first element in the
    /// container whose key is not considered to go before the key provided
    /// or end() if all keys are considered to go before the key provided.
    ///\return An const_iterator pointing to the element not before key or end()
    //*********************************************************************
    const_iterator lower_bound(key_parameter_t key) const
    {
      return const_iterator(*this, find_lower_node(root_node, key));
    }

    //*********************************************************************
    /// Returns an iterator pointing to the first element in the container
    /// whose key is not considered to go after the key provided or end()
    /// if all keys are considered to go after the key provided.
    ///\return An iterator pointing to the element after key or end()
    //*********************************************************************
    iterator upper_bound(key_parameter_t key)
    {
      return iterator(*this, find_upper_node(root_node, key));
    }

    //*********************************************************************
    /// Returns a const_iterator pointing to the first element in the
    /// container whose key is not considered to go after the key provided
    /// or end() if all keys are considered to go after the key provided.
    ///\return An const_iterator pointing to the element after key or end()
    //*********************************************************************
    const_iterator upper_bound(key_parameter_t key) const
    {
      return const_iterator(*this, find_upper_node(root_node, key));
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    imultimap& operator = (const imultimap& rhs)
    {
      // Skip if doing self assignment
      if (this != &rhs)
      {
        assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

    //*************************************************************************
    /// How to compare two key elements.
    //*************************************************************************
    key_compare key_comp() const
    {
      return kcompare;
    };

    //*************************************************************************
    /// How to compare two value elements.
    //*************************************************************************
    value_compare value_comp() const
    {
      return vcompare;
    };

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    imultimap(etl::ipool& node_pool, size_t max_size_)
      : multimap_base(max_size_)
      , p_node_pool(&node_pool)
    {
    }

    //*************************************************************************
    /// Initialise the multimap.
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
      ::new (&node.value) const value_type(value);
      ETL_INCREMENT_DEBUG_COUNT
      return node;
    }

    //*************************************************************************
    /// Destroy a Data_Node.
    //*************************************************************************
    void destroy_data_node(Data_Node& node)
    {
      node.value.~value_type();
      p_node_pool->release(&node);
      ETL_DECREMENT_DEBUG_COUNT
    }

    //*************************************************************************
    /// Count the nodes that match the key provided
    //*************************************************************************
    size_type count_nodes(key_parameter_t key) const
    {
      // Number of nodes that match the key provided result
      size_type result = 0;

      // Find lower and upper nodes for the key provided
      const Node* lower = find_lower_node(root_node, key);
      const Node* upper = find_upper_node(root_node, key);

      // Loop from lower node to upper node and find nodes that match
      while (lower != upper)
      {
        // Downcast found to Data_Node class for comparison and other operations
        const Data_Node& data_node = imultimap::data_cast(*lower);

        if (!node_comp(key, data_node) && !node_comp(data_node, key))
        {
          // This node matches the key provided
          ++result;
        }

        // Move on to the next node
        next_node(lower);
      }

      // Return the number of nodes that match
      return result;
    }

    //*************************************************************************
    /// Find the value matching the node provided
    //*************************************************************************
    Node* find_node(Node* position, key_parameter_t key) const
    {
      Node* found = nullptr;
      while (position)
      {
        // Downcast found to Data_Node class for comparison and other operations
        Data_Node& data_node = imultimap::data_cast(*position);
        // Compare the node value to the current position value
        if (node_comp(key, data_node))
        {
          // Keep searching for the node on the left
          position = position->children[(uint_least8_t) kLeft];
        }
        else if (node_comp(data_node, key))
        {
          // Keep searching for the node on the right
          position = position->children[(uint_least8_t) kRight];
        }
        else
        {
          // We found one, keep looking for more on the left
          found = position;
          position = position->children[(uint_least8_t) kLeft];
        }
      }

      // Return the node found (might be nullptr)
      return found;
    }

    //*************************************************************************
    /// Find the value matching the node provided
    //*************************************************************************
    const Node* find_node(const Node* position, key_parameter_t key) const
    {
      const Node* found = nullptr;
      while (position)
      {
        // Downcast found to Data_Node class for comparison and other operations
        const Data_Node& data_node = imultimap::data_cast(*position);
        // Compare the node value to the current position value
        if (node_comp(key, data_node))
        {
          // Keep searching for the node on the left
          position = position->children[(uint_least8_t) kLeft];
        }
        else if (node_comp(data_node, key))
        {
          // Keep searching for the node on the right
          position = position->children[(uint_least8_t) kRight];
        }
        else
        {
          // We found one, keep looking for more on the left
          found = position;
          position = position->children[(uint_least8_t) kLeft];
        }
      }

      // Return the node found (might be nullptr)
      return found;
    }

    //*************************************************************************
    /// Find the node whose key is not considered to go before the key provided
    //*************************************************************************
    Node* find_lower_node(Node* position, key_parameter_t key) const
    {
      // Something at this position? keep going
      Node* lower_node = nullptr;
      while (position)
      {
        // Downcast lower node to Data_Node reference for key comparisons
        Data_Node& data_node = imultimap::data_cast(*position);
        // Compare the key value to the current lower node key value
        if (node_comp(key, data_node))
        {
          lower_node = position;
          if (position->children[(uint_least8_t) kLeft])
          {
            position = position->children[(uint_least8_t) kLeft];
          }
          else
          {
            // Found lowest node
            break;
          }
        }
        else if (node_comp(data_node, key))
        {
          position = position->children[(uint_least8_t) kRight];
        }
        else
        {
          // Make note of current position, but keep looking to left for more
          lower_node = position;
          position = position->children[(uint_least8_t) kLeft];
        }
      }

      // Return the lower_node position found
      return lower_node;
    }

    //*************************************************************************
    /// Find the node whose key is considered to go after the key provided
    //*************************************************************************
    Node* find_upper_node(Node* position, key_parameter_t key) const
    {
      // Keep track of parent of last upper node
      Node* upper_node = nullptr;
      // Has an equal node been found? start with no
      bool found = false;
      while (position)
      {
        // Downcast position to Data_Node reference for key comparisons
        Data_Node& data_node = imultimap::data_cast(*position);
        // Compare the key value to the current upper node key value
        if (node_comp(data_node, key))
        {
          position = position->children[(uint_least8_t) kRight];
        }
        else if (node_comp(key, data_node))
        {
          upper_node = position;
          // If a node equal to key hasn't been found go left
          if (!found && position->children[(uint_least8_t) kLeft])
          {
            position = position->children[(uint_least8_t) kLeft];
          }
          else
          {
            break;
          }
        }
        else
        {
          // We found an equal item, break on next bigger item
          found = true;
          next_node(position);
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
          // is set to (uint_least8_t) kNeither (balanced)
          if ((uint_least8_t) kNeither != found->weight)
          {
            critical_node = found;
          }

          // Downcast found to Data_Node class for comparison and other operations
          Data_Node& found_data_node = imultimap::data_cast(*found);

          // Is the node provided to the left of the current position?
          if (node_comp(node, found_data_node))
          {
            // Update direction taken to insert new node in parent node
            found->dir = (uint_least8_t) kLeft;
          }
          // Is the node provided to the right of the current position?
          else if (node_comp(found_data_node, node))
          {
            // Update direction taken to insert new node in parent node
            found->dir = (uint_least8_t) kRight;
          }
          else
          {
            // Update direction taken to insert new node in parent (and
            // duplicate) node to the right.
            found->dir = (uint_least8_t) kRight;
          }

          // Is there a child of this parent node?
          if (found->children[found->dir])
          {
            // Will this node be the parent of the next critical node whose
            // weight factor is set to (uint_least8_t) kNeither (balanced)?
            if ((uint_least8_t) kNeither != found->children[found->dir]->weight)
            {
              critical_parent_node = found;
            }

            // Keep looking for empty spot to insert new node
            found = found->children[found->dir];
          }
          else
          {
            // Attach node as a child of the parent node found
            attach_node(found, found->children[found->dir], node);

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
        // Attach node to current position (which is assumed to be root)
        attach_node(nullptr, position, node);

        // Return newly added node at current position
        found = position;
      }

      // Return the node found (might be nullptr)
      return found;
    }

    //*************************************************************************
    /// Remove the node specified from somewhere starting at the position
    /// provided
    //*************************************************************************
    void remove_node(Node* node)
    {
      // If valid found node was provided then proceed with steps 1 through 5
      if (node)
      {
        // Downcast found node provided to Data_Node class
        Data_Node& data_node = imultimap::data_cast(*node);

        // Keep track of node as found node
        Node* found = node;

        // Step 1: Mark path from node provided back to the root node using the
        // internal temporary dir member value and using the parent pointer. This
        // will allow us to avoid recursion in finding the node in a tree that
        //might contain duplicate keys to be found.
        while (node)
        {
          if (node->parent)
          {
            // Which direction does parent use to get to this node?
            node->parent->dir =
              node->parent->children[(uint_least8_t) kLeft] == node ? (uint_least8_t) kLeft : (uint_least8_t) kRight;

            // Make this nodes parent the next node
            node = node->parent;
          }
          else
          {
            // Root node found - break loop
            break;
          }
        }

        // Step 2: Follow the path provided above until we reach the node
        // provided and look for the balance node to start rebalancing the tree
        // from (up to the replacement node that will be found in step 3)
        Node* balance = root_node;
        while (node)
        {
          // Did we reach the node provided originally (found) then go to step 3
          if (node == found)
          {
            // Update the direction towards a replacement node at the found node
            node->dir = node->children[(uint_least8_t) kLeft] ? (uint_least8_t) kLeft : (uint_least8_t) kRight;

            // Exit loop and proceed with step 3
            break;
          }
          else
          {
            // If this nodes weight is (uint_least8_t) kNeither or we are taking the shorter path
            // to the next node and our sibling (on longer path) is balanced then
            // we need to update the balance node to this node but all our
            // ancestors will not require rebalancing
            if ((node->weight == (uint_least8_t) kNeither) ||
              (node->weight == (1 - node->dir) &&
                node->children[1 - node->dir]->weight == (uint_least8_t) kNeither))
            {
              // Update balance node to this node
              balance = node;
            }

            // Keep searching for found in the direction provided in step 1
            node = node->children[node->dir];
          }
        }
        // The value for node should not be nullptr at this point otherwise
        // step 1 failed to provide the correct path to found. Step 5 will fail
        // (probably subtly) if node should be nullptr at this point

        // Step 3: Find the node (node should be equal to found at this point)
        // to replace found with (might end up equal to found) while also
        // continuing to update balance the same as in step 2 above.
        while (node)
        {
          // Replacement node found if its missing a child in the replace->dir
          // value set at the end of step 2 above
          if (node->children[node->dir] == nullptr)
          {
            // Exit loop once node to replace found is determined
            break;
          }

          // If this nodes weight is (uint_least8_t) kNeither or we are taking the shorter path
          // to the next node and our sibling (on longer path) is balanced then
          // we need to update the balance node to this node but all our
          // ancestors will not require rebalancing
          if ((node->weight == (uint_least8_t) kNeither) ||
            (node->weight == (1 - node->dir) &&
              node->children[1 - node->dir]->weight == (uint_least8_t) kNeither))
          {
            // Update balance node to this node
            balance = node;
          }

          // Keep searching for replacement node in the direction specified above
          node = node->children[node->dir];

          // Downcast node to Data_Node class for comparison operations
          Data_Node& replace_data_node = imultimap::data_cast(*node);

          // Compare the key provided to the replace data node key
          if (node_comp(data_node, replace_data_node))
          {
            // Update the direction to the replace node
            node->dir = (uint_least8_t) kLeft;
          }
          else if (node_comp(replace_data_node, data_node))
          {
            // Update the direction to the replace node
            node->dir = (uint_least8_t) kRight;
          }
          else
          {
            // Update the direction to the replace node
            node->dir = node->children[(uint_least8_t) kLeft] ? (uint_least8_t) kLeft : (uint_least8_t) kRight;
          }
        } // while(node)

          // Step 4: Update weights from balance to parent of node determined
          // in step 3 above rotating (2 or 3 node rotations) as needed.
        while (balance)
        {
          // Break when balance node reaches the parent of replacement node
          if (balance->children[balance->dir] == nullptr)
          {
            break;
          }

          // If balance node is balanced already ((uint_least8_t) kNeither) then just imbalance
          // the node in the opposite direction of the node being removed
          if (balance->weight == (uint_least8_t) kNeither)
          {
            balance->weight = 1 - balance->dir;
          }
          // If balance node is imbalanced in the opposite direction of the
          // node being removed then the node now becomes balanced
          else if (balance->weight == balance->dir)
          {
            balance->weight = (uint_least8_t) kNeither;
          }
          // Otherwise a rotation is required at this node
          else
          {
            int weight = balance->children[1 - balance->dir]->weight;
            // Perform a 3 node rotation if weight is same as balance->dir
            if (weight == balance->dir)
            {
              // Is the root node being rebalanced (no parent)
              if (balance->parent == nullptr)
              {
                rotate_3node(root_node, 1 - balance->dir,
                  balance->children[1 - balance->dir]->children[balance->dir]->weight);
              }
              else
              {
                rotate_3node(balance->parent->children[balance->parent->dir], 1 - balance->dir,
                  balance->children[1 - balance->dir]->children[balance->dir]->weight);
              }
            }
            // Already balanced, rebalance and make it heavy in opposite
            // direction of the node being removed
            else if (weight == (uint_least8_t) kNeither)
            {
              // Is the root node being rebalanced (no parent)
              if (balance->parent == nullptr)
              {
                rotate_2node(root_node, 1 - balance->dir);
                root_node->weight = balance->dir;
              }
              else
              {
                // Balance parent might change during rotate, keep local copy
                // to old parent so its weight can be updated after the 2 node
                // rotate is completed
                Node* old_parent = balance->parent;
                rotate_2node(balance->parent->children[balance->parent->dir], 1 - balance->dir);
                old_parent->children[old_parent->dir]->weight = balance->dir;
              }
              // Update balance node weight in opposite direction of node removed
              balance->weight = 1 - balance->dir;
            }
            // Rebalance and leave it balanced
            else
            {
              // Is the root node being rebalanced (no parent)
              if (balance->parent == nullptr)
              {
                rotate_2node(root_node, 1 - balance->dir);
              }
              else
              {
                rotate_2node(balance->parent->children[balance->parent->dir], 1 - balance->dir);
              }
            }
          }

          // Next balance node to consider
          balance = balance->children[balance->dir];
        } // while(balance)

          // Step 5: Swap found with node (replacement)
        if (found->parent)
        {
          // Handle traditional case
          detach_node(found->parent->children[found->parent->dir],
            node->parent->children[node->parent->dir]);
        }
        // Handle root node removal
        else
        {
          // Valid replacement node for root node being removed?
          if (node->parent)
          {
            detach_node(root_node, node->parent->children[node->parent->dir]);
          }
          else
          {
            // Found node and replacement node are both root node
            detach_node(root_node, root_node);
          }
        }

        // One less.
        --current_size;

        // Destroy the node detached above
        destroy_data_node(data_node);
      } // if(found)
    }

    // Disable copy construction.
    imultimap(const imultimap&);

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_MULTIMAP) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~imultimap()
    {
    }
#else
  protected:
    ~imultimap()
    {
    }
#endif

  };

  //*************************************************************************
  /// A templated multimap implementation that uses a fixed size buffer.
  //*************************************************************************
  template <typename TKey, typename TValue, const size_t MAX_SIZE_, typename TCompare = std::less<TKey> >
  class multimap : public etl::imultimap<TKey, TValue, TCompare>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    multimap()
      : etl::imultimap<TKey, TValue, TCompare>(node_pool, MAX_SIZE)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    multimap(const multimap& other)
      : etl::imultimap<TKey, TValue, TCompare>(node_pool, MAX_SIZE)
    {
      this->assign(other.cbegin(), other.cend());
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    multimap(TIterator first, TIterator last)
      : etl::imultimap<TKey, TValue, TCompare>(node_pool, MAX_SIZE)
    {
      this->assign(first, last);
    }

#if ETL_CPP11_SUPPORTED && !defined(ETL_STLPORT) && !defined(ETL_NO_STL)
    //*************************************************************************
    /// Constructor, from an initializer_list.
    //*************************************************************************
    multimap(std::initializer_list<typename etl::imultimap<TKey, TValue, TCompare>::value_type> init)
      : etl::imultimap<TKey, TValue, TCompare>(node_pool, MAX_SIZE)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~multimap()
    {
      this->initialise();
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    multimap& operator = (const multimap& rhs)
    {
      // Skip if doing self assignment
      if (this != &rhs)
      {
        this->assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

  private:

    /// The pool of data nodes used for the multimap.
    etl::pool<typename etl::imultimap<TKey, TValue, TCompare>::Data_Node, MAX_SIZE> node_pool;
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
bool operator ==(const etl::imultimap<TKey, TMapped, TKeyCompare>& lhs, const etl::imultimap<TKey, TMapped, TKeyCompare>& rhs)
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
bool operator !=(const etl::imultimap<TKey, TMapped, TKeyCompare>& lhs, const etl::imultimap<TKey, TMapped, TKeyCompare>& rhs)
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
bool operator <(const etl::imultimap<TKey, TMapped, TKeyCompare>& lhs, const etl::imultimap<TKey, TMapped, TKeyCompare>& rhs)
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
bool operator >(const etl::imultimap<TKey, TMapped, TKeyCompare>& lhs, const etl::imultimap<TKey, TMapped, TKeyCompare>& rhs)
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
template <typename TKey, typename TMapped, typename TKeyCompare>
bool operator <=(const etl::imultimap<TKey, TMapped, TKeyCompare>& lhs, const etl::imultimap<TKey, TMapped, TKeyCompare>& rhs)
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
template <typename TKey, typename TMapped, typename TKeyCompare>
bool operator >=(const etl::imultimap<TKey, TMapped, TKeyCompare>& lhs, const etl::imultimap<TKey, TMapped, TKeyCompare>& rhs)
{
  return !(lhs < rhs);
}

#include "private/minmax_pop.h"

#undef ETL_FILE

#endif
