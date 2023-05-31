///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 John Wellbelove

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

#ifndef ETL_INTRUSIVE_LINKS_INCLUDED
#define ETL_INTRUSIVE_LINKS_INCLUDED

#include "platform.h"
#include "nullptr.h"
#include "type_traits.h"
#include "exception.h"
#include "error_handler.h"
#include "utility.h"
#include "algorithm.h"

#include <assert.h>

//*****************************************************************************
// Note:
// The link functions work slightly differently to the STL 'insert' convention
// in that the second link parameter will be inserted after the first.
// i.e.
// If the list contains '1', '2', '3', '4' and "link_splice '2','5'" is invoked the
// resulting list will contain '1', '2', '5', '3', '4'
// This is to maintain consistency between forward and bidirectional links
// and also is intuitive.
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// Link exception.
  //***************************************************************************
  class link_exception : public etl::exception
  {
  public:

    link_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// not unlinked exception.
  //***************************************************************************
  class not_unlinked_exception : public etl::link_exception
  {
  public:

    not_unlinked_exception(string_type file_name_, numeric_type line_number_)
      : link_exception(ETL_ERROR_TEXT("link:still linked", ETL_INTRUSIVE_LINKS_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// A forward link.
  //***************************************************************************
  template <size_t ID_>
  struct forward_link
  {
    enum
    {
      ID = ID_,
    };

    void clear()
    {
      etl_next = ETL_NULLPTR;
    }

    bool is_linked() const
    {
      return etl_next != ETL_NULLPTR;
    }

    forward_link* etl_next;
  };

  // Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID> >::value, void>::type
  link(TLink& lhs, TLink& rhs)
  {
    lhs.etl_next = &rhs;
  }

  // Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID> >::value, void>::type
  link_splice(TLink& lhs, TLink& rhs)
  {
    rhs.etl_next = lhs.etl_next;
    lhs.etl_next = &rhs;
  }

  // Pointer, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID> >::value, void>::type
  link(TLink* lhs, TLink* rhs)
  {
    if (lhs != ETL_NULLPTR)
    {
      lhs->etl_next = rhs;
    }
  }

  // Pointer, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID> >::value, void>::type
  link_splice(TLink* lhs, TLink* rhs)
  {
    if (lhs != ETL_NULLPTR)
    {
      if (rhs != ETL_NULLPTR)
      {
          rhs->etl_next = lhs->etl_next;
      }

      lhs->etl_next = rhs;
    }
  }

  // Reference, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID> >::value, void>::type
  link(TLink& lhs, TLink* rhs)
  {
    lhs.etl_next = rhs;
  }

  // Reference, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID> >::value, void>::type
  link_splice(TLink& lhs, TLink* rhs)
  {
    if (rhs != ETL_NULLPTR)
    {
      rhs->etl_next = lhs.etl_next;
    }

    lhs.etl_next = rhs;
  }

  // Pointer, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID> >::value, void>::type
  link(TLink* lhs, TLink& rhs)
  {
    if (lhs != ETL_NULLPTR)
    {
      lhs->etl_next = &rhs;
    }
  }

  // Pointer, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID> >::value, void>::type
  link_splice(TLink* lhs, TLink& rhs)
  {
    if (lhs != ETL_NULLPTR)
    {
      rhs.etl_next  = lhs->etl_next;
      lhs->etl_next = &rhs;
    }
  }

  // Reference, Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID> >::value, void>::type
  link_splice(TLink& lhs, TLink& first, TLink& last)
  {
    last.etl_next = lhs.etl_next;
    lhs.etl_next  = &first;
  }

  // Pointer, Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID> >::value, void>::type
  link_splice(TLink* lhs, TLink& first, TLink& last)
  {
    if (lhs != ETL_NULLPTR)
    {
      last.etl_next = lhs->etl_next;
      lhs->etl_next  = &first;
    }
    else
    {
      last.etl_next = ETL_NULLPTR;
    }
  }

  // Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID> >::value, void>::type
  unlink_after(TLink& node)
  {
    if (node.etl_next != ETL_NULLPTR)
    {
      TLink* unlinked_node = node.etl_next;
      node.etl_next = unlinked_node->etl_next;
    }
  }

  // Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID> >::value, void>::type
  unlink_after(TLink& before, TLink& last)
  {
    before.etl_next = last.etl_next;
  }

  //***************************************************************************
  /// A bidirectional link.
  //***************************************************************************
  template <size_t ID_>
  struct bidirectional_link
  {
    enum
    {
      ID = ID_,
    };

    void clear()
    {
      etl_previous = ETL_NULLPTR;
      etl_next     = ETL_NULLPTR;
    }

    bool is_linked() const
    {
      return (etl_previous != ETL_NULLPTR) || (etl_next != ETL_NULLPTR);
    }

    void reverse()
    {
      using ETL_OR_STD::swap; // Allow ADL

      swap(etl_previous, etl_next);
    }

    bidirectional_link* etl_previous;
    bidirectional_link* etl_next;

    void unlink()
    {
        // Connect the previous link with the next.
        if (etl_previous != ETL_NULLPTR)
        {
          etl_previous->etl_next = etl_next;
        }

        // Connect the next link with the previous.
        if (etl_next != ETL_NULLPTR)
        {
          etl_next->etl_previous = etl_previous;
        }

        clear();
    }
  };

  // Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID> >::value, void>::type
  link(TLink& lhs, TLink& rhs)
  {
    lhs.etl_next     = &rhs;
    rhs.etl_previous = &lhs;
  }

  // Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID> >::value, void>::type
  link_splice(TLink& lhs, TLink& rhs)
  {
    rhs.etl_next     = lhs.etl_next;
    rhs.etl_previous = &lhs;

    if (lhs.etl_next != ETL_NULLPTR)
    {
      lhs.etl_next->etl_previous = &rhs;
    }

    lhs.etl_next = &rhs;
  }

  // Pointer, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID> >::value, void>::type
  link(TLink* lhs, TLink* rhs)
  {
    if (lhs != ETL_NULLPTR)
    {
      lhs->etl_next = rhs;
    }

    if (rhs != ETL_NULLPTR)
    {
      rhs->etl_previous = lhs;
    }
  }

  // Pointer, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID> >::value, void>::type
  link_splice(TLink* lhs, TLink* rhs)
  {
    if (rhs != ETL_NULLPTR)
    {
      if (lhs != ETL_NULLPTR)
      {
        rhs->etl_next = lhs->etl_next;
      }

      rhs->etl_previous = lhs;
    }

    if (lhs != ETL_NULLPTR)
    {
      if (lhs->etl_next != ETL_NULLPTR)
      {
        lhs->etl_next->etl_previous = rhs;
      }

      lhs->etl_next = rhs;
    }
  }

  // Reference, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID> >::value, void>::type
  link(TLink& lhs, TLink* rhs)
  {
    lhs.etl_next = rhs;

    if (rhs != ETL_NULLPTR)
    {
      rhs->etl_previous = &lhs;
    }
  }

  // Reference, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID> >::value, void>::type
  link_splice(TLink& lhs, TLink* rhs)
  {
    if (rhs != ETL_NULLPTR)
    {
      rhs->etl_next     = lhs.etl_next;
      rhs->etl_previous = &lhs;
    }

    if (lhs.etl_next != ETL_NULLPTR)
    {
      lhs.etl_next->etl_previous = rhs;
    }

    lhs.etl_next = rhs;
  }

  // Pointer, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID> >::value, void>::type
  link(TLink* lhs, TLink& rhs)
  {
    if (lhs != ETL_NULLPTR)
    {
      lhs->etl_next = &rhs;
    }

    rhs.etl_previous = lhs;
  }

  // Pointer, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID> >::value, void>::type
  link_splice(TLink* lhs, TLink& rhs)
  {
    if (lhs != ETL_NULLPTR)
    {
      rhs.etl_next = lhs->etl_next;
    }

    rhs.etl_previous = lhs;

    if (lhs != ETL_NULLPTR)
    {
      if (lhs->etl_next != ETL_NULLPTR)
      {
        lhs->etl_next->etl_previous = &rhs;
      }

      lhs->etl_next = &rhs;
    }
  }

  // Reference, Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID> >::value, void>::type
  link_splice(TLink& lhs, TLink& first, TLink& last)
  {
    last.etl_next = lhs.etl_next;
    first.etl_previous = &lhs;

    if (last.etl_next != ETL_NULLPTR)
    {
      last.etl_next->etl_previous = &last;
    }

    lhs.etl_next = &first;
  }

  // Pointer, Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID> >::value, void>::type
  link_splice(TLink* lhs, TLink& first, TLink& last)
  {
    if (lhs != ETL_NULLPTR)
    {
      last.etl_next = lhs->etl_next;
    }
    else
    {
      last.etl_next = ETL_NULLPTR;
    }

    first.etl_previous = lhs;

    if (last.etl_next != ETL_NULLPTR)
    {
      last.etl_next->etl_previous = &last;
    }

    if (lhs != ETL_NULLPTR)
    {
      lhs->etl_next = &first;
    }
  }

  // Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID> >::value, void>::type
  unlink(TLink& node)
  {
    node.unlink();
  }

  // Reference Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID> >::value, void>::type
  unlink(TLink& first, TLink& last)
  {
    if (&first == &last)
    {
      first.unlink();
    }
    else
    {
      if (last.etl_next != ETL_NULLPTR)
      {
        last.etl_next->etl_previous = first.etl_previous;
      }

      if (first.etl_previous != ETL_NULLPTR)
      {
        first.etl_previous->etl_next = last.etl_next;
      }
    }
  }

  //***************************************************************************
  /// A binary tree link.
  //***************************************************************************
  template <size_t ID_>
  struct tree_link
  {
      enum
      {
        ID = ID_,
      };

      void clear()
      {
        etl_parent = ETL_NULLPTR;
        etl_left   = ETL_NULLPTR;
        etl_right  = ETL_NULLPTR;
      }

      bool is_linked() const
      {
        return (etl_parent != ETL_NULLPTR) || (etl_left != ETL_NULLPTR) || (etl_right != ETL_NULLPTR);
      }

      tree_link* etl_parent;
      tree_link* etl_left;
      tree_link* etl_right;
  };

  // Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_left(TLink& parent, TLink& leaf)
  {
    parent.etl_left = &leaf;
    leaf.etl_parent = &parent;
  }

  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_right(TLink& parent, TLink& leaf)
  {
    parent.etl_right = &leaf;
    leaf.etl_parent  = &parent;
  }

  // Pointer, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_left(TLink* parent, TLink* leaf)
  {
    if (parent != ETL_NULLPTR)
    {
      parent->etl_left = leaf;
    }

    if (leaf != ETL_NULLPTR)
    {
      leaf->etl_parent = parent;
    }
  }

  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_right(TLink* parent, TLink* leaf)
  {
    if (parent != ETL_NULLPTR)
    {
      parent->etl_right = leaf;
    }

    if (leaf != ETL_NULLPTR)
    {
      leaf->etl_parent = parent;
    }
  }

  // Reference, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_left(TLink& parent, TLink* leaf)
  {
    parent.etl_left = leaf;

    if (leaf != ETL_NULLPTR)
    {
      leaf->etl_parent = &parent;
    }
  }

  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_right(TLink& parent, TLink* leaf)
  {
    parent.etl_right = leaf;

    if (leaf != ETL_NULLPTR)
    {
      leaf->etl_parent = &parent;
    }
  }

  // Pointer, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_left(TLink* parent, TLink& leaf)
  {
    if (parent != ETL_NULLPTR)
    {
      parent->etl_left = &leaf;
    }

    leaf.etl_parent = parent;
  }

  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_right(TLink* parent, TLink& leaf)
  {
    if (parent != ETL_NULLPTR)
    {
      parent->etl_right = &leaf;
    }

    leaf.etl_parent = parent;
  }

  // Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_rotate_left(TLink& parent, TLink& leaf)
  {
     parent.etl_right = leaf.etl_left;

     if (parent.etl_right != ETL_NULLPTR)
     {
       parent.etl_right->etl_parent = &parent;
     }

     leaf.etl_parent   = parent.etl_parent;
     parent.etl_parent = &leaf;
     leaf.etl_left     = &parent;
  }

  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_rotate_right(TLink& parent, TLink& leaf)
  {
     parent.etl_left = leaf.etl_right;

     if (parent.etl_left != ETL_NULLPTR)
     {
       parent.etl_left->etl_parent = &parent;
     }

     leaf.etl_parent   = parent.etl_parent;
     parent.etl_parent = &leaf;
     leaf.etl_right    = &parent;
  }

  // Pointer, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_rotate_left(TLink* parent, TLink* leaf)
  {
    if ((parent != ETL_NULLPTR) && (leaf != ETL_NULLPTR))
    {
      link_rotate_left(*parent, *leaf);
    }
  }

  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_rotate_right(TLink* parent, TLink* leaf)
  {
    if ((parent != ETL_NULLPTR) && (leaf != ETL_NULLPTR))
    {
      link_rotate_right(*parent, *leaf);
    }
  }

  // Reference, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_rotate_left(TLink& parent, TLink* leaf)
  {
    if (leaf != ETL_NULLPTR)
    {
      link_rotate_left(parent, *leaf);
    }
  }

  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_rotate_right(TLink& parent, TLink* leaf)
  {
    if (leaf != ETL_NULLPTR)
    {
      link_rotate_right(parent, *leaf);
    }
  }

  // Pointer, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_rotate_left(TLink* parent, TLink& leaf)
  {
    if (parent != ETL_NULLPTR)
    {
      link_rotate_left(*parent, leaf);
    }
  }

  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_rotate_right(TLink* parent, TLink& leaf)
  {
    if (parent != ETL_NULLPTR)
    {
      link_rotate_right(*parent, leaf);
    }
  }

  // Reference, Reference
  /// Automatically detects whether a left or right rotate is expected.
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_rotate(TLink& parent, TLink& leaf)
  {
    if (parent.etl_left == &leaf)
    {
      etl::link_rotate_right(parent, leaf);
    }
    else
    {
      etl::link_rotate_left(parent, leaf);
    }
  }

  // Pointer, Pointer
  /// Automatically detects whether a left or right rotate is expected.
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_rotate(TLink* parent, TLink* leaf)
  {
    if ((parent != ETL_NULLPTR) && (leaf != ETL_NULLPTR))
    {
      if (parent->etl_left == leaf)
      {
        etl::link_rotate_right(*parent, *leaf);
      }
      else
      {
        etl::link_rotate_left(*parent, *leaf);
      }
    }
  }

  // Reference, Pointer
  /// Automatically detects whether a left or right rotate is expected.
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_rotate(TLink& parent, TLink* leaf)
  {
    if (leaf != ETL_NULLPTR)
    {
      if (parent.etl_left == leaf)
      {
        etl::link_rotate_right(parent, *leaf);
      }
      else
      {
        etl::link_rotate_left(parent, *leaf);
      }
    }
  }

  // Pointer, Reference
  /// Automatically detects whether a left or right rotate is expected.
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_rotate(TLink* parent, TLink& leaf)
  {
    if (parent != ETL_NULLPTR)
    {
      if (parent->etl_left == &leaf)
      {
        etl::link_rotate_right(*parent, leaf);
      }
      else
      {
        etl::link_rotate_left(*parent, leaf);
      }
    }
  }
}

#endif
