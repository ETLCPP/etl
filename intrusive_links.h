///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2016 jwellbelove

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

#ifndef __ETL_INTRUSIVE_LINKS__
#define __ETL_INTRUSIVE_LINKS__

#include "nullptr.h"
#include "type_traits.h"
#include "exception.h"
#include "error_handler.h"

#undef ETL_FILE
#define ETL_FILE "23"

namespace etl
{
  namespace link_option
  {
    enum
    {
      DEFAULT = 0,
      AUTO    = 1,
      SAFE    = 2
    };
  };

  //***************************************************************************
  /// Deque full exception.
  ///\ingroup deque
  //***************************************************************************
  class link_exception : public etl::exception
  {
  public:

    link_exception(string_type file_name, numeric_type line_number)
      : exception(ETL_ERROR_TEXT("link:still linked", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// A forward link.
  //***************************************************************************
  template <const size_t ID_ = 0>
  struct forward_link
  {
    enum
    {
      ID = ID_
    };

    void clear()
    {
      etl_next = nullptr;
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
  link_insert(TLink& lhs, TLink& rhs)
  {
    rhs.etl_next = lhs.etl_next;
    lhs.etl_next = &rhs;
  }

  // Pointer, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID> >::value, void>::type
  link(TLink* lhs, TLink* rhs)
  {
    if (lhs != nullptr)
    {
      lhs->etl_next = rhs;
    }
  }

  // Pointer, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID> >::value, void>::type
  link_insert(TLink* lhs, TLink* rhs)
  {
    if (lhs != nullptr)
    {
      if (rhs != nullptr)
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
  link_insert(TLink& lhs, TLink* rhs)
  {
    if (rhs != nullptr)
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
    if (lhs != nullptr)
    {
      lhs->etl_next = &rhs;
    }
  }

  // Pointer, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID> >::value, void>::type
  link_insert(TLink* lhs, TLink& rhs)
  {
    if (lhs != nullptr)
    {
      rhs.etl_next  = lhs->etl_next;
      lhs->etl_next = &rhs;
    }
  }

  // Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID> >::value, void>::type
  unlink_after(TLink& node)
  {
    if (node.etl_next != nullptr)
    {
      node.etl_next = node.etl_next->etl_next;
    }
  }

  // Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID> >::value, void>::type
  unlink_after(TLink* node)
  {
    if (node != nullptr)
    {
      if (node->etl_next != nullptr)
      {
        node->etl_next = node.etl_next->etl_next;
      }
    }
  }

  //***************************************************************************
  /// A bidirectional link.
  //***************************************************************************
  template <const size_t ID_ = 0, const int OPTION_ = etl::link_option::DEFAULT>
  struct bidirectional_link
  {
    enum
    {
      ID     = ID_,
      OPTION = OPTION_
    };

    void clear()
    {
      etl_previous = nullptr;
      etl_next     = nullptr;
    }

    bidirectional_link* etl_previous;
    bidirectional_link* etl_next;
  };

  //******************************************************************
  // Specialisation for auto unlinked option.
  // When this link is destroyed it will automatically unlink itself.
  //******************************************************************
  template <const size_t ID_>
  struct bidirectional_link<ID_, etl::link_option::AUTO>
  {
    enum
    {
      ID     = ID_,
      OPTION = etl::link_option::AUTO_UNLINK
    };

    ~bidirectional_link()
    {
      // Connect the previous link with the next.
      if (etl_previous != nullptr)
      {
        etl_previous->etl_next = etl_next;
      }

      // Connect the next link with the previous.
      if (etl_next != nullptr)
      {
        etl_next->etl_previous = etl_previous;
      }
    }

    void clear()
    {
      etl_previous = nullptr;
      etl_next     = nullptr;
    }

    bidirectional_link* etl_previous;
    bidirectional_link* etl_next;
  };

  //******************************************************************
  // Specialisation for safe unlinked option.
  // An error will be generated if the links are valid when the object
  // is destroyed.
  //******************************************************************
  template <const size_t ID_>
  struct bidirectional_link<ID_, etl::link_option::SAFE>
  {
    enum
    {
      ID     = ID_,
      OPTION = etl::link_option::AUTO_UNLINK
    };

    ~bidirectional_link()
    {
      ETL_ASSERT(etl_previous == nullptr, ETL_ERROR(link_exception));
      ETL_ASSERT(etl_next     == nullptr, ETL_ERROR(link_exception));
    }

    void clear()
    {
      etl_previous = nullptr;
      etl_next     = nullptr;
    }

    bidirectional_link* etl_previous;
    bidirectional_link* etl_next;
  };

  // Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link(TLink& lhs, TLink& rhs)
  {
    lhs.etl_next     = &rhs;
    rhs.etl_previous = &lhs;
  }

  // Pointer, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link(TLink* lhs, TLink* rhs)
  {
    if (lhs != nullptr)
    {
      lhs->etl_next = rhs;
    }

    if (rhs != nullptr)
    {
      rhs->etl_previous = lhs;
    }
  }

  // Reference, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link(TLink& lhs, TLink* rhs)
  {
    lhs.etl_next = rhs;

    if (rhs != nullptr)
    {
      rhs->etl_previous = &lhs;
    }
  }

  // Pointer, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link(TLink* lhs, TLink& rhs)
  {
    if (lhs != nullptr)
    {
      lhs->etl_next = &rhs;
    }

    rhs.etl_previous = lhs;
  }

  // Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID, TLink::OPTION> >::value, void>::type
  unlink(TLink& node)
  {
    if (node.etl_next != nullptr)
    {
      node.etl_next->etl_previous = node.etl_previous;
    }

    if (node.etl_previous != nullptr)
    {
      node.etl_previous->etl_next = node.etl_next;
    }
  }

  // Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID, TLink::OPTION> >::value, void>::type
  unlink(TLink* node)
  {
    if (node != nullptr)
    {
      if (node->etl_next != nullptr)
      {
        node->etl_next->etl_previous = node->etl_previous;
      }

      if (node->etl_previous != nullptr)
      {
        node->etl_previous->etl_next = node->etl_next;
      }
    }
  }

  //***************************************************************************
  /// A tree link.
  //***************************************************************************
  template <const size_t ID_ = 0>
  struct tree_link
  {
    enum
    {
      ID = ID_
    };

    void clear()
    {
      etl_parent = nullptr;
      etl_left   = nullptr;
      etl_right  = nullptr;
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
    if (parent != nullptr)
    {
      parent->etl_left = leaf;
    }

    if (leaf != nullptr)
    {
      leaf->etl_parent = parent;
    }
  }

  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_right(TLink* parent, TLink* leaf)
  {
    if (parent != nullptr)
    {
      parent->etl_right = leaf;
    }

    if (leaf != nullptr)
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

    if (leaf != nullptr)
    {
      leaf->etl_parent = &parent;
    }
  }

  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_right(TLink& parent, TLink* leaf)
  {
    parent.etl_right = leaf;

    if (leaf != nullptr)
    {
      leaf->etl_parent = &parent;
    }
  }

  // Pointer, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_left(TLink* parent, TLink& leaf)
  {
    if (parent != nullptr)
    {
      parent->etl_left = &leaf;
    }

    leaf.etl_parent = parent;
  }

  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID> >::value, void>::type
  link_right(TLink* parent, TLink& leaf)
  {
    if (parent != nullptr)
    {
      parent->etl_right = &leaf;
    }

    leaf.etl_parent = parent;
  }
}

#undef ETL_FILE
#endif
