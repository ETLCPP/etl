///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove, rlindeman

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

#ifndef ETL_MULTISET_INCLUDED
#define ETL_MULTISET_INCLUDED

#include "platform.h"
#include "algorithm.h"
#include "iterator.h"
#include "functional.h"
#include "parameter_type.h"
#include "pool.h"
#include "exception.h"
#include "error_handler.h"
#include "debug_count.h"
#include "nullptr.h"
#include "type_traits.h"
#include "nth_type.h"
#include "utility.h"
#include "placement_new.h"
#include "initializer_list.h"

#include <stddef.h>

#include "private/minmax_push.h"
#include "private/comparator_is_transparent.h"

//*****************************************************************************
/// A multiset with the capacity defined at compile time.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// Exception for the set.
  ///\ingroup set
  //***************************************************************************
  class multiset_exception : public etl::exception
  {
  public:

    multiset_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : etl::exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Full exception for the set.
  ///\ingroup set
  //***************************************************************************
  class multiset_full : public etl::multiset_exception
  {
  public:

    multiset_full(string_type file_name_, numeric_type line_number_)
      : etl::multiset_exception(ETL_ERROR_TEXT("multiset:full", ETL_MULTISET_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Map out of bounds exception.
  ///\ingroup set
  //***************************************************************************
  class multiset_out_of_bounds : public etl::multiset_exception
  {
  public:

    multiset_out_of_bounds(string_type file_name_, numeric_type line_number_)
      : etl::multiset_exception(ETL_ERROR_TEXT("multiset:bounds", ETL_MULTISET_FILE_ID"B"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Iterator exception for the set.
  ///\ingroup set
  //***************************************************************************
  class multiset_iterator : public etl::multiset_exception
  {
  public:

    multiset_iterator(string_type file_name_, numeric_type line_number_)
      : etl::multiset_exception(ETL_ERROR_TEXT("multiset:iterator", ETL_MULTISET_FILE_ID"C"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The base class for all sets.
  ///\ingroup set
  //***************************************************************************
  class multiset_base
  {
  public:

    typedef size_t size_type; ///< The type used for determining the size of set.

    //*************************************************************************
    /// Gets the size of the set.
    //*************************************************************************
    size_type size() const
    {
      return current_size;
    }

    //*************************************************************************
    /// Gets the maximum possible size of the set.
    //*************************************************************************
    size_type max_size() const
    {
      return CAPACITY;
    }

    //*************************************************************************
    /// Checks to see if the set is empty.
    //*************************************************************************
    bool empty() const
    {
      return current_size == 0;
    }

    //*************************************************************************
    /// Checks to see if the set is full.
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
    /// The node element in the multiset.
    //*************************************************************************
    struct Node
    {
      //***********************************************************************
      /// Constructor
      //***********************************************************************
      Node() :
        parent(ETL_NULLPTR),
        weight(kNeither),
        dir(kNeither)
      {
        children[0] = ETL_NULLPTR;
        children[1] = ETL_NULLPTR;
      }

      //***********************************************************************
      /// Marks the node as a leaf.
      //***********************************************************************
      void mark_as_leaf()
      {
        weight = kNeither;
        dir = kNeither;
        parent = ETL_NULLPTR;
        children[0] = ETL_NULLPTR;
        children[1] = ETL_NULLPTR;
      }

      Node* parent;
      Node* children[2];
      uint_least8_t weight;
      uint_least8_t dir;
    };

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    multiset_base(size_type max_size_)
      : current_size(0)
      , CAPACITY(max_size_)
      , root_node(ETL_NULLPTR)
    {
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~multiset_base()
    {
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

      if (replacement != ETL_NULLPTR)
      {
        replacement->parent = swap->parent;
      }

      // Point swap node to detached node's parent, children and weight
      swap->parent = detached->parent;
      swap->children[kLeft] = detached->children[kLeft];
      swap->children[kRight] = detached->children[kRight];
      if (swap->children[kLeft])
      {
        swap->children[kLeft]->parent = swap;
      }
      if (swap->children[kRight])
      {
        swap->children[kRight]->parent = swap;
      }
      swap->weight = detached->weight;
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
    /// Find the next node in sequence from the node provided
    //*************************************************************************
    void next_node(Node*& position) const
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
            parent = position->parent; // find_parent_node(root_node, position);
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
            parent = position->parent;
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
            parent = position->parent;
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
            parent = position->parent;
            // Repeat while previous position was on left side of parent tree
          } while (parent && parent->children[kLeft] == position);

          // Set parent node as the next position
          position = parent;
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
      position->weight = kNeither;
      // Position's parent becomes new_root
      position->parent = new_root;
      position = new_root;
      // Clear weight factor from new root
      position->weight = kNeither;
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
      position->children[dir]->weight = third != kNeither && third != dir ? dir : uint_least8_t(kNeither);

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
      position->weight = third != kNeither && third == dir ? 1 - dir : kNeither;

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
      position->weight = kNeither;
    }

    size_type current_size;   ///< The number of the used nodes.
    const size_type CAPACITY; ///< The maximum size of the set.
    Node* root_node;          ///< The node that acts as the multiset root.
    ETL_DECLARE_DEBUG_COUNT;
  };

  //***************************************************************************
  /// A templated base for all etl::multiset types.
  ///\ingroup set
  //***************************************************************************
  template <typename TKey, typename TCompare = ETL_OR_STD::less<TKey> >
  class imultiset : public etl::multiset_base
  {
  public:

    typedef TKey                key_type;
    typedef TKey                value_type;
    typedef TCompare            key_compare;
    typedef TCompare            value_compare;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
#if ETL_USING_CPP11
    typedef value_type&&        rvalue_reference;
#endif
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef size_t              size_type;

  protected:

    //*************************************************************************
    /// The data node element in the multiset.
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
    typedef const TKey& key_parameter_t;

    //*************************************************************************
    /// How to compare node elements.
    //*************************************************************************
    bool node_comp(const Data_Node& node1, const Data_Node& node2) const
    {
      return compare(node1.value, node2.value);
    }

    bool node_comp(const Data_Node& node, key_parameter_t key) const
    {
      return compare(node.value, key);
    }

    bool node_comp(key_parameter_t key, const Data_Node& node) const
    {
      return compare(key, node.value);
    }

#if ETL_USING_CPP11
    template <typename K, typename KC = TCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    bool node_comp(const Data_Node& node, const K& key) const
    {
      return compare(node.value, key);
    }

    template <typename K, typename KC = TCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    bool node_comp(const K& key, const Data_Node& node) const
    {
      return compare(key, node.value);
    }
#endif

  private:

    /// The pool of data nodes used in the multiset.
    ipool* p_node_pool;

    key_compare compare;

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
    class iterator : public etl::iterator<ETL_OR_STD::bidirectional_iterator_tag, value_type>
    {
    public:

      friend class imultiset;
      friend class const_iterator;

      iterator()
        : p_multiset(ETL_NULLPTR)
        , p_node(ETL_NULLPTR)
      {
      }

      iterator(imultiset& multiset)
        : p_multiset(&multiset)
        , p_node(ETL_NULLPTR)
      {
      }

      iterator(imultiset& multiset, Node* node)
        : p_multiset(&multiset)
        , p_node(node)
      {
      }

      iterator(const iterator& other)
        : p_multiset(other.p_multiset)
        , p_node(other.p_node)
      {
      }

      ~iterator()
      {
      }

      iterator& operator ++()
      {
        p_multiset->next_node(p_node);
        return *this;
      }

      iterator operator ++(int)
      {
        iterator temp(*this);
        p_multiset->next_node(p_node);
        return temp;
      }

      iterator& operator --()
      {
        p_multiset->prev_node(p_node);
        return *this;
      }

      iterator operator --(int)
      {
        iterator temp(*this);
        p_multiset->prev_node(p_node);
        return temp;
      }

      iterator& operator =(const iterator& other)
      {
        p_multiset = other.p_multiset;
        p_node = other.p_node;
        return *this;
      }

      reference operator *() const
      {
        return imultiset::data_cast(p_node)->value;
      }

      pointer operator &() const
      {
        return &(imultiset::data_cast(p_node)->value);
      }

      pointer operator ->() const
      {
        return &(imultiset::data_cast(p_node)->value);
      }

      friend bool operator == (const iterator& lhs, const iterator& rhs)
      {
        return lhs.p_multiset == rhs.p_multiset && lhs.p_node == rhs.p_node;
      }

      friend bool operator != (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      // Pointer to multiset associated with this iterator
      imultiset* p_multiset;

      // Pointer to the current node for this iterator
      Node* p_node;
    };

    friend class iterator;

    //*************************************************************************
    /// const_iterator
    //*************************************************************************
    class const_iterator : public etl::iterator<ETL_OR_STD::bidirectional_iterator_tag, const value_type>
    {
    public:

      friend class imultiset;

      const_iterator()
        : p_multiset(ETL_NULLPTR)
        , p_node(ETL_NULLPTR)
      {
      }

      const_iterator(const imultiset& multiset)
        : p_multiset(&multiset)
        , p_node(ETL_NULLPTR)
      {
      }

      const_iterator(const imultiset& multiset, const Node* node)
        : p_multiset(&multiset)
        , p_node(node)
      {
      }

      const_iterator(const typename imultiset::iterator& other)
        : p_multiset(other.p_multiset)
        , p_node(other.p_node)
      {
      }

      const_iterator(const const_iterator& other)
        : p_multiset(other.p_multiset)
        , p_node(other.p_node)
      {
      }

      ~const_iterator()
      {
      }

      const_iterator& operator ++()
      {
        p_multiset->next_node(p_node);
        return *this;
      }

      const_iterator operator ++(int)
      {
        const_iterator temp(*this);
        p_multiset->next_node(p_node);
        return temp;
      }

      const_iterator& operator --()
      {
        p_multiset->prev_node(p_node);
        return *this;
      }

      const_iterator operator --(int)
      {
        const_iterator temp(*this);
        p_multiset->prev_node(p_node);
        return temp;
      }

      const_iterator& operator =(const const_iterator& other)
      {
        p_multiset = other.p_multiset;
        p_node = other.p_node;
        return *this;
      }

      const_reference operator *() const
      {
        return imultiset::data_cast(p_node)->value;
      }

      const_pointer operator &() const
      {
        return imultiset::data_cast(p_node)->value;
      }

      const_pointer operator ->() const
      {
        return &(imultiset::data_cast(p_node)->value);
      }

      friend bool operator == (const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.p_multiset == rhs.p_multiset && lhs.p_node == rhs.p_node;
      }

      friend bool operator != (const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      // Convert to an iterator.
      imultiset::iterator to_iterator() const
      {
        return imultiset::iterator(const_cast<imultiset&>(*p_multiset), const_cast<Node*>(p_node));
      }

      // Pointer to multiset associated with this iterator
      const imultiset* p_multiset;

      // Pointer to the current node for this iterator
      const Node* p_node;
    };

    friend class const_iterator;

    typedef typename etl::iterator_traits<iterator>::difference_type difference_type;

    typedef ETL_OR_STD::reverse_iterator<iterator>       reverse_iterator;
    typedef ETL_OR_STD::reverse_iterator<const_iterator> const_reverse_iterator;

    //*************************************************************************
    /// Gets the beginning of the multiset.
    //*************************************************************************
    iterator begin()
    {
      return iterator(*this, find_limit_node(root_node, kLeft));
    }

    //*************************************************************************
    /// Gets the beginning of the multiset.
    //*************************************************************************
    const_iterator begin() const
    {
      return const_iterator(*this, find_limit_node(root_node, kLeft));
    }

    //*************************************************************************
    /// Gets the end of the multiset.
    //*************************************************************************
    iterator end()
    {
      return iterator(*this);
    }

    //*************************************************************************
    /// Gets the end of the multiset.
    //*************************************************************************
    const_iterator end() const
    {
      return const_iterator(*this);
    }

    //*************************************************************************
    /// Gets the beginning of the multiset.
    //*************************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(*this, find_limit_node(root_node, kLeft));
    }

    //*************************************************************************
    /// Gets the end of the multiset.
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
    /// Assigns values to the multiset.
    /// If asserts or exceptions are enabled, emits set_full if the multiset does not have enough free space.
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
    /// Clears the multiset.
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
    
#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    size_type count(const K& key) const
    {
      return count_nodes(key);
    }
#endif

    //*************************************************************************
    /// Returns two iterators with bounding (lower bound, upper bound) the key
    /// provided
    //*************************************************************************
    ETL_OR_STD::pair<iterator, iterator> equal_range(key_parameter_t key)
    {
      return ETL_OR_STD::make_pair<iterator, iterator>(iterator(*this, find_lower_node(root_node, key)),
                                                       iterator(*this, find_upper_node(root_node, key)));
    }

#if ETL_USING_CPP11
    //*************************************************************************
    template <typename K, typename KC = TCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_OR_STD::pair<iterator, iterator> equal_range(const K& key)
    {
      return ETL_OR_STD::make_pair<iterator, iterator>(iterator(*this, find_lower_node(root_node, key)),
                                                       iterator(*this, find_upper_node(root_node, key)));
    }
#endif

    //*************************************************************************
    /// Returns two const iterators with bounding (lower bound, upper bound)
    /// the key provided.
    //*************************************************************************
    ETL_OR_STD::pair<const_iterator, const_iterator> equal_range(key_parameter_t key) const
    {
      return ETL_OR_STD::make_pair<const_iterator, const_iterator>(const_iterator(*this, find_lower_node(root_node, key)),
                                                                   const_iterator(*this, find_upper_node(root_node, key)));
    }

#if ETL_USING_CPP11
    //*************************************************************************
    template <typename K, typename KC = TCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_OR_STD::pair<const_iterator, const_iterator> equal_range(key_parameter_t key) const
    {
      return ETL_OR_STD::make_pair<const_iterator, const_iterator>(const_iterator(*this, find_lower_node(root_node, key)),
                                                                   const_iterator(*this, find_upper_node(root_node, key)));
    }
#endif

    //*************************************************************************
    /// Erases the value at the specified position.
    //*************************************************************************
    iterator erase(iterator position)
    {
      // Remove the node by its node specified in iterator position
      return erase(const_iterator(position));
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
    size_type erase(key_parameter_t key_value)
    {
      // Number of nodes removed
      size_type d = 0;
      const_iterator lower(*this, find_lower_node(root_node, key_value));
      const_iterator upper(*this, find_upper_node(root_node, key_value));
      while (lower != upper)
      {
        // Increment count for each node removed
        ++d;
        // Remove node using the other erase method
        lower = erase(lower);
      }

      // Return the total count erased
      return d;
    }

    //*************************************************************************
#if ETL_USING_CPP11
    template <typename K, typename KC = TCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    size_type erase(K&& key_value)
    {
      // Number of nodes removed
      size_type d = 0;
      const_iterator lower(*this, find_lower_node(root_node, etl::forward<K>(key_value)));
      const_iterator upper(*this, find_upper_node(root_node, etl::forward<K>(key_value)));
      while (lower != upper)
      {
        // Increment count for each node removed
        ++d;
        // Remove node using the other erase method
        lower = erase(lower);
      }

      // Return the total count erased
      return d;
    }
#endif

    //*************************************************************************
    /// Erases a range of elements.
    //*************************************************************************
    iterator erase(const_iterator first, const_iterator last)
    {
      iterator next;
      while (first != last)
      {
        first = erase(first);
      }

      return last.to_iterator();
    }

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator pointing to the element or end() if not found.
    //*********************************************************************
    iterator find(key_parameter_t key_value)
    {
      return iterator(*this, find_node(root_node, key_value));
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    iterator find(const K& k)
    {
      return iterator(*this, find_node(root_node, k));
    }
#endif

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator pointing to the element or end() if not found.
    //*********************************************************************
    const_iterator find(key_parameter_t key_value) const
    {
      return const_iterator(*this, find_node(root_node, key_value));
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    const_iterator find(const K& k) const
    {
      return const_iterator(*this, find_node(root_node, k));
    }
#endif

    //*********************************************************************
    /// Inserts a value to the multiset.
    /// If asserts or exceptions are enabled, emits set_full if the multiset is already full.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_reference value)
    {
      // Default to no inserted node
      Node* inserted_node = ETL_NULLPTR;

      ETL_ASSERT(!full(), ETL_ERROR(multiset_full));

      // Get next available free node
      Data_Node& node = allocate_data_node(value);

      // Obtain the inserted node (might be ETL_NULLPTR if node was a duplicate)
      inserted_node = insert_node(root_node, node);

      // Insert node into tree and return iterator to new node location in tree
      return iterator(*this, inserted_node);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Inserts a value to the multiset.
    /// If asserts or exceptions are enabled, emits set_full if the multiset is already full.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(rvalue_reference value)
    {
      // Default to no inserted node
      Node* inserted_node = ETL_NULLPTR;

      ETL_ASSERT(!full(), ETL_ERROR(multiset_full));

      // Get next available free node
      Data_Node& node = allocate_data_node(etl::move(value));

      // Obtain the inserted node (might be ETL_NULLPTR if node was a duplicate)
      inserted_node = insert_node(root_node, node);

      // Insert node into tree and return iterator to new node location in tree
      return iterator(*this, inserted_node);
    }
#endif

    //*********************************************************************
    /// Inserts a value to the multiset starting at the position recommended.
    /// If asserts or exceptions are enabled, emits set_full if the multiset is already full.
    ///\param position The position that would precede the value to insert.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator /*position*/, const_reference value)
    {
      // Ignore position provided and just do a normal insert
      return insert(value);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Inserts a value to the multiset starting at the position recommended.
    /// If asserts or exceptions are enabled, emits set_full if the multiset is already full.
    ///\param position The position that would precede the value to insert.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator /*position*/, rvalue_reference value)
    {
      // Ignore position provided and just do a normal insert
      return insert(etl::move(value));
    }
#endif

    //*********************************************************************
    /// Inserts a range of values to the multiset.
    /// If asserts or exceptions are enabled, emits set_full if the multiset does not have enough free space.
    ///\param position The position to insert at.
    ///\param first    The first element to add.
    ///\param last     The last + 1 element to add.
    //*********************************************************************
    template <class TIterator>
    void insert(TIterator first, TIterator last)
    {
      while (first != last)
      {
        insert(*first);
        ++first;
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

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    iterator lower_bound(const K& key)
    {
      return iterator(*this, find_lower_node(root_node, key));
    }
#endif

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

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    const_iterator lower_bound(const K& key) const
    {
      return const_iterator(*this, find_lower_node(root_node, key));
    }
#endif

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

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    iterator upper_bound(const K& key)
    {
      return iterator(*this, find_upper_node(root_node, key));
    }
#endif

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

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    const_iterator upper_bound(const K& key) const
    {
      return const_iterator(*this, find_upper_node(root_node, key));
    }
#endif

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    imultiset& operator = (const imultiset& rhs)
    {
      // Skip if doing self assignment
      if (this != &rhs)
      {
        assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    imultiset& operator = (imultiset&& rhs)
    {
      // Skip if doing self assignment
      if (this != &rhs)
      {
        clear();

        typename etl::imultiset<TKey, TCompare>::iterator from = rhs.begin();

        while (from != rhs.end())
        {
          typename etl::imultiset<TKey, TCompare>::iterator temp = from;
          ++temp;

          this->insert(etl::move(*from));
          from = temp;
        }
      }

      return *this;
    }
#endif

    //*************************************************************************
    /// How to compare two key elements.
    //*************************************************************************
    key_compare key_comp() const
    {
      return compare;
    };

    //*************************************************************************
    /// How to compare two value elements.
    //*************************************************************************
    value_compare value_comp() const
    {
      return compare;
    };

    //*************************************************************************
    /// Check if the set contains the key.
    //*************************************************************************
    bool contains(key_parameter_t key) const
    {
      return find(key) != end();
    }

#if ETL_USING_CPP11
    //*************************************************************************
    template <typename K, typename KC = TCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    bool contains(const K& k) const
    {
      return find(k) != end();
    }
#endif

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    imultiset(etl::ipool& node_pool, size_t max_size_)
      : etl::multiset_base(max_size_)
      , p_node_pool(&node_pool)
    {
    }

    //*************************************************************************
    /// Initialise the multiset.
    //*************************************************************************
    void initialise()
    {
      const_iterator item = begin();

      while (item != end())
      {
        item = erase(item);
      }
    }

  private:

    //*************************************************************************
    /// Allocate a Data_Node.
    //*************************************************************************
    Data_Node& allocate_data_node(const_reference value)
    {
      Data_Node* node = allocate_data_node();
      ::new ((void*)&node->value) value_type(value);
      ETL_INCREMENT_DEBUG_COUNT;
      return *node;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Allocate a Data_Node.
    //*************************************************************************
    Data_Node& allocate_data_node(rvalue_reference value)
    {
      Data_Node* node = allocate_data_node();
      ::new ((void*)&node->value) value_type(etl::move(value));
      ETL_INCREMENT_DEBUG_COUNT;
      return *node;
    }
#endif

    //*************************************************************************
    /// Create a Data_Node.
    //*************************************************************************
    Data_Node* allocate_data_node()
    {
      Data_Node* (etl::ipool::*func)() = &etl::ipool::allocate<Data_Node>;
      return (p_node_pool->*func)();
    }

    //*************************************************************************
    /// Destroy a Data_Node.
    //*************************************************************************
    void destroy_data_node(Data_Node& node)
    {
      node.value.~value_type();
      p_node_pool->release(&node);
      ETL_DECREMENT_DEBUG_COUNT;
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
        const Data_Node& data_node = imultiset::data_cast(*lower);

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

#if ETL_USING_CPP11
    //*************************************************************************
    template <typename K, typename KC = TCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    size_type count_nodes(const K& key) const
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
        const Data_Node& data_node = imultiset::data_cast(*lower);

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
#endif

    //*************************************************************************
    /// Find the value matching the node provided
    //*************************************************************************
    Node* find_node(Node* position, key_parameter_t key)
    {
      Node* found = ETL_NULLPTR;
      while (position)
      {
        // Downcast found to Data_Node class for comparison and other operations
        Data_Node& data_node = imultiset::data_cast(*position);
        // Compare the node value to the current position value
        if (node_comp(key, data_node))
        {
          // Keep searching for the node on the left
          position = position->children[kLeft];
        }
        else if (node_comp(data_node, key))
        {
          // Keep searching for the node on the right
          position = position->children[kRight];
        }
        else
        {
          // We found one, keep looking for more on the left
          found = position;
          position = position->children[kLeft];
        }
      }

      // Return the node found (might be ETL_NULLPTR)
      return found;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    template <typename K, typename KC = TCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    Node* find_node(Node* position, const K& key)
    {
      Node* found = ETL_NULLPTR;
      while (position)
      {
        // Downcast found to Data_Node class for comparison and other operations
        Data_Node& data_node = imultiset::data_cast(*position);
        // Compare the node value to the current position value
        if (node_comp(key, data_node))
        {
          // Keep searching for the node on the left
          position = position->children[kLeft];
        }
        else if (node_comp(data_node, key))
        {
          // Keep searching for the node on the right
          position = position->children[kRight];
        }
        else
        {
          // We found one, keep looking for more on the left
          found = position;
          position = position->children[kLeft];
        }
      }

      // Return the node found (might be ETL_NULLPTR)
      return found;
    }
#endif

    //*************************************************************************
    /// Find the value matching the node provided
    //*************************************************************************
    const Node* find_node(const Node* position, key_parameter_t key) const
    {
      const Node* found = ETL_NULLPTR;
      while (position)
      {
        // Downcast found to Data_Node class for comparison and other operations
        const Data_Node& data_node = imultiset::data_cast(*position);
        // Compare the node value to the current position value
        if (node_comp(key, data_node))
        {
          // Keep searching for the node on the left
          position = position->children[kLeft];
        }
        else if (node_comp(data_node, key))
        {
          // Keep searching for the node on the right
          position = position->children[kRight];
        }
        else
        {
          // We found one, keep looking for more on the left
          found = position;
          position = position->children[kLeft];
        }
      }

      // Return the node found (might be ETL_NULLPTR)
      return found;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    template <typename K, typename KC = TCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    const Node* find_node(const Node* position, const K& key) const
    {
      const Node* found = ETL_NULLPTR;
      while (position)
      {
        // Downcast found to Data_Node class for comparison and other operations
        const Data_Node& data_node = imultiset::data_cast(*position);
        // Compare the node value to the current position value
        if (node_comp(key, data_node))
        {
          // Keep searching for the node on the left
          position = position->children[kLeft];
        }
        else if (node_comp(data_node, key))
        {
          // Keep searching for the node on the right
          position = position->children[kRight];
        }
        else
        {
          // We found one, keep looking for more on the left
          found = position;
          position = position->children[kLeft];
        }
      }

      // Return the node found (might be ETL_NULLPTR)
      return found;
    }
#endif

    //*************************************************************************
    /// Find the node whose key is not considered to go before the key provided
    //*************************************************************************
    Node* find_lower_node(Node* position, key_parameter_t key) const
    {
      // Something at this position? keep going
      Node* lower_node = ETL_NULLPTR;
      while (position)
      {
        // Downcast lower node to Data_Node reference for key comparisons
        Data_Node& data_node = imultiset::data_cast(*position);
        // Compare the key value to the current lower node key value
        if (node_comp(key, data_node))
        {
          lower_node = position;
          if (position->children[kLeft])
          {
            position = position->children[kLeft];
          }
          else
          {
            // Found lowest node
            break;
          }
        }
        else if (node_comp(data_node, key))
        {
          position = position->children[kRight];
        }
        else
        {
          // Make note of current position, but keep looking to left for more
          lower_node = position;
          position = position->children[kLeft];
        }
      }

      // Return the lower_node position found
      return lower_node;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    template <typename K, typename KC = TCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    Node* find_lower_node(Node* position, const K& key) const
    {
      // Something at this position? keep going
      Node* lower_node = ETL_NULLPTR;
      while (position)
      {
        // Downcast lower node to Data_Node reference for key comparisons
        Data_Node& data_node = imultiset::data_cast(*position);
        // Compare the key value to the current lower node key value
        if (node_comp(key, data_node))
        {
          lower_node = position;
          if (position->children[kLeft])
          {
            position = position->children[kLeft];
          }
          else
          {
            // Found lowest node
            break;
          }
        }
        else if (node_comp(data_node, key))
        {
          position = position->children[kRight];
        }
        else
        {
          // Make note of current position, but keep looking to left for more
          lower_node = position;
          position = position->children[kLeft];
        }
      }

      // Return the lower_node position found
      return lower_node;
    }
#endif

    //*************************************************************************
    /// Find the node whose key is considered to go after the key provided
    //*************************************************************************
    Node* find_upper_node(Node* position, key_parameter_t key) const
    {
      // Keep track of parent of last upper node
      Node* upper_node = ETL_NULLPTR;
      // Has an equal node been found? start with no
      bool found = false;
      while (position)
      {
        // Downcast position to Data_Node reference for key comparisons
        Data_Node& data_node = imultiset::data_cast(*position);
        // Compare the key value to the current upper node key value
        if (node_comp(data_node, key))
        {
          position = position->children[kRight];
        }
        else if (node_comp(key, data_node))
        {
          upper_node = position;
          // If a node equal to key hasn't been found go left
          if (!found && position->children[kLeft])
          {
            position = position->children[kLeft];
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

      // Return the upper node position found (might be ETL_NULLPTR)
      return upper_node;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    template <typename K, typename KC = TCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    Node* find_upper_node(Node* position, const K& key) const
    {
      // Keep track of parent of last upper node
      Node* upper_node = ETL_NULLPTR;
      // Has an equal node been found? start with no
      bool found = false;
      while (position)
      {
        // Downcast position to Data_Node reference for key comparisons
        Data_Node& data_node = imultiset::data_cast(*position);
        // Compare the key value to the current upper node key value
        if (node_comp(data_node, key))
        {
          position = position->children[kRight];
        }
        else if (node_comp(key, data_node))
        {
          upper_node = position;
          // If a node equal to key hasn't been found go left
          if (!found && position->children[kLeft])
          {
            position = position->children[kLeft];
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

      // Return the upper node position found (might be ETL_NULLPTR)
      return upper_node;
    }
#endif

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
        // Find the critical parent node (default to ETL_NULLPTR)
        Node* critical_parent_node = ETL_NULLPTR;
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
          Data_Node& found_data_node = imultiset::data_cast(*found);

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
            // Update direction taken to insert new node in parent (and
            // duplicate) node to the right.
            found->dir = kRight;
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
          if (critical_parent_node == ETL_NULLPTR && critical_node == root_node)
          {
            balance_node(root_node);
          }
          else if (critical_parent_node == ETL_NULLPTR && critical_node == position)
          {
            balance_node(position);
          }
          else
          {
            if (critical_parent_node != ETL_NULLPTR)
            {
              balance_node(critical_parent_node->children[critical_parent_node->dir]);
            }
          }
        }
      }
      else
      {
        // Attach node to current position (which is assumed to be root)
        attach_node(ETL_NULLPTR, position, node);

        // Return newly added node at current position
        found = position;
      }

      // Return the node found (might be ETL_NULLPTR)
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
        Data_Node& data_node = imultiset::data_cast(*node);

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
              node->parent->children[kLeft] == node ? kLeft : kRight;

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
            node->dir = node->children[kLeft] ? kLeft : kRight;

            // Exit loop and proceed with step 3
            break;
          }
          else
          {
            // If this nodes weight is kNeither or we are taking the shorter path
            // to the next node and our sibling (on longer path) is balanced then
            // we need to update the balance node to this node but all our
            // ancestors will not require rebalancing
            if ((node->weight == kNeither) ||
              (node->weight == (1 - node->dir) &&
                node->children[1 - node->dir]->weight == kNeither))
            {
              // Update balance node to this node
              balance = node;
            }

            // Keep searching for found in the direction provided in step 1
            node = node->children[node->dir];
          }
        }
        // The value for node should not be ETL_NULLPTR at this point otherwise
        // step 1 failed to provide the correct path to found. Step 5 will fail
        // (probably subtly) if node should be ETL_NULLPTR at this point

        // Step 3: Find the node (node should be equal to found at this point)
        // to replace found with (might end up equal to found) while also
        // continuing to update balance the same as in step 2 above.
        while (node)
        {
          // Replacement node found if its missing a child in the replace->dir
          // value set at the end of step 2 above
          if (node->children[node->dir] == ETL_NULLPTR)
          {
            // Exit loop once node to replace found is determined
            break;
          }

          // If this nodes weight is kNeither or we are taking the shorter path
          // to the next node and our sibling (on longer path) is balanced then
          // we need to update the balance node to this node but all our
          // ancestors will not require rebalancing
          if ((node->weight == kNeither) ||
            (node->weight == (1 - node->dir) &&
              node->children[1 - node->dir]->weight == kNeither))
          {
            // Update balance node to this node
            balance = node;
          }

          // Keep searching for replacement node in the direction specified above
          node = node->children[node->dir];

          // Downcast node to Data_Node class for comparison operations
          Data_Node& replace_data_node = imultiset::data_cast(*node);

          // Compare the key provided to the replace data node key
          if (node_comp(data_node, replace_data_node))
          {
            // Update the direction to the replace node
            node->dir = kLeft;
          }
          else if (node_comp(replace_data_node, data_node))
          {
            // Update the direction to the replace node
            node->dir = kRight;
          }
          else
          {
            // Update the direction to the replace node
            node->dir = node->children[kLeft] ? kLeft : kRight;
          }
        } // while(node)

          // Step 4: Update weights from balance to parent of node determined
          // in step 3 above rotating (2 or 3 node rotations) as needed.
        while (balance)
        {
          // Break when balance node reaches the parent of replacement node
          if (balance->children[balance->dir] == ETL_NULLPTR)
          {
            break;
          }

          // If balance node is balanced already (kNeither) then just imbalance
          // the node in the opposite direction of the node being removed
          if (balance->weight == kNeither)
          {
            balance->weight = 1 - balance->dir;
          }
          // If balance node is imbalanced in the opposite direction of the
          // node being removed then the node now becomes balanced
          else if (balance->weight == balance->dir)
          {
            balance->weight = kNeither;
          }
          // Otherwise a rotation is required at this node
          else
          {
            int weight = balance->children[1 - balance->dir]->weight;
            // Perform a 3 node rotation if weight is same as balance->dir
            if (weight == balance->dir)
            {
              // Is the root node being rebalanced (no parent)
              if (balance->parent == ETL_NULLPTR)
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
            else if (weight == kNeither)
            {
              // Is the root node being rebalanced (no parent)
              if (balance->parent == ETL_NULLPTR)
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
              if (balance->parent == ETL_NULLPTR)
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
    imultiset(const imultiset&);

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_MULTISET) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~imultiset()
    {
    }
#else
  protected:
    ~imultiset()
    {
    }
#endif
  };

  //*************************************************************************
  /// A templated multiset implementation that uses a fixed size buffer.
  //*************************************************************************
  template <typename TKey, const size_t MAX_SIZE_, typename TCompare = ETL_OR_STD::less<TKey> >
  class multiset : public etl::imultiset<TKey, TCompare>
  {
  public:

    static ETL_CONSTANT size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    multiset()
      : etl::imultiset<TKey, TCompare>(node_pool, MAX_SIZE)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    multiset(const multiset& other)
      : etl::imultiset<TKey, TCompare>(node_pool, MAX_SIZE)
    {
      this->assign(other.cbegin(), other.cend());
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move constructor.
    //*************************************************************************
    multiset(multiset&& other)
      : etl::imultiset<TKey, TCompare>(node_pool, MAX_SIZE)
    {
      if (this != &other)
      {
        typename etl::imultiset<TKey, TCompare>::iterator from = other.begin();

        while (from != other.end())
        {
          typename etl::imultiset<TKey, TCompare>::iterator temp = from;
          ++temp;

          this->insert(etl::move(*from));
          from = temp;
        }
      }
    }
#endif

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    multiset(TIterator first, TIterator last)
      : etl::imultiset<TKey, TCompare>(node_pool, MAX_SIZE)
    {
      this->assign(first, last);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    /// Constructor, from an initializer_list.
    //*************************************************************************
    multiset(std::initializer_list<typename etl::imultiset<TKey, TCompare>::value_type> init)
      : etl::imultiset<TKey, TCompare>(node_pool, MAX_SIZE)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~multiset()
    {
      this->initialise();
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    multiset& operator = (const multiset& rhs)
    {
      // Skip if doing self assignment
      if (this != &rhs)
      {
        this->assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    multiset& operator = (multiset&& rhs)
    {
      if (this != &rhs)
      {
        this->clear();

        typename etl::imultiset<TKey, TCompare>::iterator from = rhs.begin();

        while (from != rhs.end())
        {
          this->insert(etl::move(*from));
          ++from;
        }
      }

      return *this;
    }
#endif

  private:

    /// The pool of data nodes used for the multiset.
    etl::pool<typename etl::imultiset<TKey, TCompare>::Data_Node, MAX_SIZE> node_pool;
  };

  template <typename TKey, const size_t MAX_SIZE_, typename TCompare>
  ETL_CONSTANT size_t multiset<TKey, MAX_SIZE_, TCompare>::MAX_SIZE;

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST
  template <typename... T>
  multiset(T...) -> multiset<etl::nth_type_t<0, T...>, sizeof...(T)>;
#endif

  //*************************************************************************
  /// Make
  //*************************************************************************
#if ETL_USING_CPP11 && ETL_HAS_INITIALIZER_LIST
  template <typename TKey, typename TKeyCompare = etl::less<TKey>, typename... T>
  constexpr auto make_multiset(T&&... keys) -> etl::multiset<TKey, sizeof...(T), TKeyCompare>
  {
    return { {etl::forward<T>(keys)...} };
  }
#endif

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first lookup.
  ///\param rhs Reference to the second lookup.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup lookup
  //***************************************************************************
  template <typename TKey, typename TCompare>
  bool operator ==(const etl::imultiset<TKey, TCompare>& lhs, const etl::imultiset<TKey, TCompare>& rhs)
  {
    return (lhs.size() == rhs.size()) && ETL_OR_STD::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first lookup.
  ///\param rhs Reference to the second lookup.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup lookup
  //***************************************************************************
  template <typename TKey, typename TCompare>
  bool operator !=(const etl::imultiset<TKey, TCompare>& lhs, const etl::imultiset<TKey, TCompare>& rhs)
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
  template <typename TKey, typename TCompare>
  bool operator <(const etl::imultiset<TKey, TCompare>& lhs, const etl::imultiset<TKey, TCompare>& rhs)
  {
    return ETL_OR_STD::lexicographical_compare(lhs.begin(),
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
  template <typename TKey, typename TCompare>
  bool operator >(const etl::imultiset<TKey, TCompare>& lhs, const etl::imultiset<TKey, TCompare>& rhs)
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
  template <typename TKey, typename TCompare>
  bool operator <=(const etl::imultiset<TKey, TCompare>& lhs, const etl::imultiset<TKey, TCompare>& rhs)
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
  template <typename TKey, typename TCompare>
  bool operator >=(const etl::imultiset<TKey, TCompare>& lhs, const etl::imultiset<TKey, TCompare>& rhs)
  {
    return !(lhs < rhs);
  }
}

#include "private/minmax_pop.h"

#endif
