///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2015 jwellbelove, rlindeman

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

#if !defined(__ETL_IN_IMULTISET_H__)
#error This header is a private element of etl::multiset & etl::imultiset
#endif

#ifndef __ETL_MULTISET_BASE__
#define __ETL_MULTISET_BASE__

#include <stddef.h>
#include "../exception.h"
#include "../error_handler.h"
#include "../debug_count.h"

#undef ETL_FILE
#define ETL_FILE "10"

namespace etl
{
  //***************************************************************************
  /// Exception for the set.
  ///\ingroup set
  //***************************************************************************
  class multiset_exception : public exception
  {
  public:

    multiset_exception(string_type what, string_type file_name, numeric_type line_number)
      : exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Full exception for the set.
  ///\ingroup set
  //***************************************************************************
  class multiset_full : public multiset_exception
  {
  public:

    multiset_full(string_type file_name, numeric_type line_number)
      : multiset_exception(ETL_ERROR_TEXT("multiset:full", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Map out of bounds exception.
  ///\ingroup set
  //***************************************************************************
  class multiset_out_of_bounds : public multiset_exception
  {
  public:

    multiset_out_of_bounds(string_type file_name, numeric_type line_number)
      : multiset_exception(ETL_ERROR_TEXT("multiset:bounds", ETL_FILE"B"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Iterator exception for the set.
  ///\ingroup set
  //***************************************************************************
  class multiset_iterator : public multiset_exception
  {
  public:

    multiset_iterator(string_type file_name, numeric_type line_number)
      : multiset_exception(ETL_ERROR_TEXT("multiset:iterator", ETL_FILE"C"), file_name, line_number)
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
      return MAX_SIZE;
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
      return current_size == MAX_SIZE;
    }

    //*************************************************************************
    /// Returns the capacity of the vector.
    ///\return The capacity of the vector.
    //*************************************************************************
    size_type capacity() const
    {
      return MAX_SIZE;
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

    static const uint_least8_t kLeft    = 0;
    static const uint_least8_t kRight   = 1;
    static const uint_least8_t kNeither = 2;

    //*************************************************************************
    /// The node element in the multiset.
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
    multiset_base(size_type max_size)
      : current_size(0)
      , MAX_SIZE(max_size)
      , root_node(nullptr)
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
    const size_type MAX_SIZE; ///< The maximum size of the set.                    
    Node* root_node;          ///< The node that acts as the multiset root.
    etl::debug_count construct_count;
  };
}

#undef ETL_FILE

#endif
