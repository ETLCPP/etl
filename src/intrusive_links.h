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

#include <assert.h>
#include <utility>

#include "nullptr.h"
#include "type_traits.h"
#include "exception.h"
#include "error_handler.h"

#undef ETL_FILE
#define ETL_FILE "22"

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
  namespace link_option
  {
    enum
    {
      DEFAULT,
      AUTO,
      CHECKED
    };
  }

  namespace count_option
  {
    enum
    {
      SLOW_COUNT,
      FAST_COUNT
    };
  }

  //***************************************************************************
  /// Link exception.
  //***************************************************************************
  class link_exception : public etl::exception
  {
  public:

    link_exception(string_type what, string_type file_name, numeric_type line_number)
      : exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// not unlinked exception.
  //***************************************************************************
  class not_unlinked_exception : public etl::link_exception
  {
  public:

    not_unlinked_exception(string_type file_name, numeric_type line_number)
      : link_exception(ETL_ERROR_TEXT("link:still linked", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  namespace __private_intrusive_links__
  {
    //***************************************************************************
    /// A forward link base.
    //***************************************************************************
    template <typename TLink, const size_t ID_, const size_t OPTION_>
    struct forward_link_base
    {
      enum
      {
        ID = ID_,
        OPTION = OPTION_
      };

      void clear()
      {
        etl_next = nullptr;
      }

      bool is_linked() const
      {
        return etl_next != nullptr;
      }

      TLink* etl_next;
    };
  }

  //***************************************************************************
  /// A forward link.
  //***************************************************************************
  template <const size_t ID_, const size_t OPTION_ = etl::link_option::DEFAULT>
  struct forward_link
    : public __private_intrusive_links__::forward_link_base<forward_link<ID_, OPTION_>, ID_, OPTION_>
  {
  };

  //******************************************************************
  // There is no valid specialisation for auto link
  //******************************************************************
  template <const size_t ID_>
  struct forward_link<ID_, etl::link_option::AUTO>
    : public __private_intrusive_links__::forward_link_base<forward_link<ID_, etl::link_option::AUTO>, ID_, etl::link_option::AUTO>
  {
    forward_link()
    {
      this->clear();
    }
  };

  //******************************************************************
  // Specialisation for checked unlink option.
  // An error will be generated if the links are valid when the object
  // is destroyed.
  //******************************************************************
  template <const size_t ID_>
  struct forward_link<ID_, etl::link_option::CHECKED>
    : public __private_intrusive_links__::forward_link_base<forward_link<ID_, etl::link_option::CHECKED>, ID_, etl::link_option::CHECKED>
  {
    forward_link()
    {
      this->clear();
    }

    ~forward_link()
    {
      assert(this->etl_next != nullptr);
    }
  };

  // Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link(TLink& lhs, TLink& rhs)
  {
    lhs.etl_next = &rhs;
  }

  // Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link_splice(TLink& lhs, TLink& rhs)
  {
    rhs.etl_next = lhs.etl_next;
    lhs.etl_next = &rhs;
  }

  // Pointer, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link(TLink* lhs, TLink* rhs)
  {
    if (lhs != nullptr)
    {
      lhs->etl_next = rhs;
    }
  }

  // Pointer, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link_splice(TLink* lhs, TLink* rhs)
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
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link(TLink& lhs, TLink* rhs)
  {
    lhs.etl_next = rhs;
  }

  // Reference, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link_splice(TLink& lhs, TLink* rhs)
  {
    if (rhs != nullptr)
    {
      rhs->etl_next = lhs.etl_next;
    }

    lhs.etl_next = rhs;
  }

  // Pointer, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link(TLink* lhs, TLink& rhs)
  {
    if (lhs != nullptr)
    {
      lhs->etl_next = &rhs;
    }
  }

  // Pointer, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link_splice(TLink* lhs, TLink& rhs)
  {
    if (lhs != nullptr)
    {
      rhs.etl_next  = lhs->etl_next;
      lhs->etl_next = &rhs;
    }
  }

  // Reference, Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link_splice(TLink& lhs, TLink& first, TLink& last)
  {
    last.etl_next = lhs.etl_next;
    lhs.etl_next  = &first;
  }

  // Pointer, Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link_splice(TLink* lhs, TLink& first, TLink& last)
  {
    if (lhs != nullptr)
    {
      last.etl_next = lhs->etl_next;
      lhs->etl_next  = &first;
    }
    else
    {
      last.etl_next = nullptr;
    }
  }

  // Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID, TLink::OPTION> >::value, void>::type
  unlink_after(TLink& node)
  {
    if (node.etl_next != nullptr)
    {
      TLink* unlinked_node = node.etl_next;
      node.etl_next = unlinked_node->etl_next;

      if ((int(TLink::OPTION) == etl::link_option::AUTO) ||
          (int(TLink::OPTION) == etl::link_option::CHECKED))
      {
        unlinked_node->clear();
      }
    }
  }

  // Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::forward_link<TLink::ID, TLink::OPTION> >::value, void>::type
  unlink_after(TLink& before, TLink& last)
  {
    before.etl_next = last.etl_next;

    if ((int(TLink::OPTION) == etl::link_option::AUTO) ||
        (int(TLink::OPTION) == etl::link_option::CHECKED))
    {
      last.clear();
    }
  }

  namespace __private_intrusive_links__
  {
    //***************************************************************************
    /// A bidirectional link base.
    //***************************************************************************
    template <typename TLink, const size_t ID_, const size_t OPTION_>
    struct bidirectional_link_base
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

      bool is_linked() const
      {
        return (etl_previous != nullptr) || (etl_next != nullptr);
      }

      void reverse()
      {
        std::swap(etl_previous, etl_next);
      }

      TLink* etl_previous;
      TLink* etl_next;

    protected:

      void base_unlink()
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
    };
  }

  //***************************************************************************
  /// A bidirectional link.
  //***************************************************************************
  template <const size_t ID_, const size_t OPTION_ = etl::link_option::DEFAULT>
  struct bidirectional_link
    : public __private_intrusive_links__::bidirectional_link_base<bidirectional_link<ID_, OPTION_>, ID_, OPTION_>
  {
    void unlink()
    {
      this->base_unlink();
    }
  };

  //******************************************************************
  // Specialisation for auto unlinked option.
  // When this link is destroyed it will automatically unlink itself.
  //******************************************************************
  template <const size_t ID_>
  struct bidirectional_link<ID_, etl::link_option::AUTO>
    : public __private_intrusive_links__::bidirectional_link_base<bidirectional_link<ID_, etl::link_option::AUTO>, ID_, etl::link_option::AUTO>
  {
    bidirectional_link()
    {
      this->clear();
    }

    ~bidirectional_link()
    {
      this->base_unlink();
    }

    void unlink()
    {
      this->base_unlink();
      this->clear();
    }
  };

  //******************************************************************
  // Specialisation for checked unlink option.
  // An error will be generated if the links are valid when the object
  // is destroyed.
  //******************************************************************
  template <const size_t ID_>
  struct bidirectional_link<ID_, etl::link_option::CHECKED>
    : public __private_intrusive_links__::bidirectional_link_base<bidirectional_link<ID_, etl::link_option::CHECKED>, ID_, etl::link_option::CHECKED>
  {
    bidirectional_link()
    {
      this->clear();
    }

    ~bidirectional_link()
    {
      assert(this->etl_previous == nullptr);
      assert(this->etl_next     == nullptr);
    }

    void unlink()
    {
      this->base_unlink();
      this->clear();
    }
  };

  // Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link(TLink& lhs, TLink& rhs)
  {
    lhs.etl_next     = &rhs;
    rhs.etl_previous = &lhs;
  }

  // Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link_splice(TLink& lhs, TLink& rhs)
  {
    rhs.etl_next     = lhs.etl_next;
    rhs.etl_previous = &lhs;

    if (lhs.etl_next != nullptr)
    {
      lhs.etl_next->etl_previous = &rhs;
    }

    lhs.etl_next = &rhs;
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

  // Pointer, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link_splice(TLink* lhs, TLink* rhs)
  {
    if (rhs != nullptr)
    {
      if (lhs != nullptr)
      {
        rhs->etl_next = lhs->etl_next;
      }

      rhs->etl_previous = lhs;
    }

    if (lhs != nullptr)
    {
      if (lhs->etl_next != nullptr)
      {
        lhs->etl_next->etl_previous = rhs;
      }

      lhs->etl_next = rhs;
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

  // Reference, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link_splice(TLink& lhs, TLink* rhs)
  {
    if (rhs != nullptr)
    {
      rhs->etl_next     = lhs.etl_next;
      rhs->etl_previous = &lhs;
    }

    if (lhs.etl_next != nullptr)
    {
      lhs.etl_next->etl_previous = rhs;
    }

    lhs.etl_next = rhs;
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

  // Pointer, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link_splice(TLink* lhs, TLink& rhs)
  {
    if (lhs != nullptr)
    {
      rhs.etl_next = lhs->etl_next;
    }

    rhs.etl_previous = lhs;

    if (lhs != nullptr)
    {
      if (lhs->etl_next != nullptr)
      {
        lhs->etl_next->etl_previous = &rhs;
      }

      lhs->etl_next = &rhs;
    }
  }

  // Reference, Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link_splice(TLink& lhs, TLink& first, TLink& last)
  {
    last.etl_next = lhs.etl_next;
    first.etl_previous = &lhs;

    if (last.etl_next != nullptr)
    {
      last.etl_next->etl_previous = &last;
    }

    lhs.etl_next = &first;
  }

  // Pointer, Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link_splice(TLink* lhs, TLink& first, TLink& last)
  {
    if (lhs != nullptr)
    {
      last.etl_next = lhs->etl_next;
    }
    else
    {
      last.etl_next = nullptr;
    }

    first.etl_previous = lhs;

    if (last.etl_next != nullptr)
    {
      last.etl_next->etl_previous = &last;
    }

    if (lhs != nullptr)
    {
      lhs->etl_next = &first;
    }
  }

  // Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID, TLink::OPTION> >::value, void>::type
  unlink(TLink& node)
  {
    node.unlink();
  }

  // Reference Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::bidirectional_link<TLink::ID, TLink::OPTION> >::value, void>::type
  unlink(TLink& first, TLink& last)
  {
    if (&first == &last)
    {
      first.unlink();
    }
    else
    {
      if (last.etl_next != nullptr)
      {
        last.etl_next->etl_previous = first.etl_previous;
      }

      if (first.etl_previous != nullptr)
      {
        first.etl_previous->etl_next = last.etl_next;
      }

      if ((TLink::OPTION == etl::link_option::AUTO) ||
          (int(TLink::OPTION) == etl::link_option::CHECKED))
      {
        first.etl_previous = nullptr;
        last.etl_next = nullptr;
      }
    }
  }

  namespace __private_intrusive_links__
  {
    //***************************************************************************
    /// A tree link base.
    //***************************************************************************
    template <typename TLink, const size_t ID_, const size_t OPTION_>
    struct tree_link_base
    {
      enum
      {
        ID     = ID_,
        OPTION = OPTION_
      };

      void clear()
      {
        etl_parent = nullptr;
        etl_left   = nullptr;
        etl_right  = nullptr;
      }

      bool is_linked() const
      {
        return (etl_parent != nullptr) || (etl_left != nullptr) || (etl_right != nullptr);
      }

      TLink* etl_parent;
      TLink* etl_left;
      TLink* etl_right;
    };
  }

  //***************************************************************************
  /// A tree link.
  //***************************************************************************
  template <const size_t ID_, const size_t OPTION_ = etl::link_option::DEFAULT>
  struct tree_link
    : public __private_intrusive_links__::tree_link_base<tree_link<ID_, OPTION_>, ID_, OPTION_>
  {
  };

  //******************************************************************
  // There is no valid specialisation for auto link
  //******************************************************************
  template <const size_t ID_>
  struct tree_link<ID_, etl::link_option::AUTO>
    : public __private_intrusive_links__::tree_link_base<tree_link<ID_, etl::link_option::AUTO>, ID_, etl::link_option::AUTO>
  {
    tree_link()
    {
      this->clear();
    }
  };

  //******************************************************************
  // Specialisation for checked unlink option.
  // An error will be generated if the links are valid when the object
  // is destroyed.
  //******************************************************************
  template <const size_t ID_>
  struct tree_link<ID_, etl::link_option::CHECKED>
    : public __private_intrusive_links__::tree_link_base<tree_link<ID_, etl::link_option::CHECKED>, ID_, etl::link_option::CHECKED>
  {
    tree_link()
    {
      this->clear();
    }

    ~tree_link()
    {
      assert(this->etl_parent != nullptr);
      assert(this->etl_left   != nullptr);
      assert(this->etl_right  != nullptr);
    }
  };

  // Reference, Reference
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link_left(TLink& parent, TLink& leaf)
  {
    parent.etl_left = &leaf;
    leaf.etl_parent = &parent;
  }

  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link_right(TLink& parent, TLink& leaf)
  {
    parent.etl_right = &leaf;
    leaf.etl_parent  = &parent;
  }

  // Pointer, Pointer
  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID, TLink::OPTION> >::value, void>::type
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
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID, TLink::OPTION> >::value, void>::type
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
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link_left(TLink& parent, TLink* leaf)
  {
    parent.etl_left = leaf;

    if (leaf != nullptr)
    {
      leaf->etl_parent = &parent;
    }
  }

  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID, TLink::OPTION> >::value, void>::type
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
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID, TLink::OPTION> >::value, void>::type
  link_left(TLink* parent, TLink& leaf)
  {
    if (parent != nullptr)
    {
      parent->etl_left = &leaf;
    }

    leaf.etl_parent = parent;
  }

  template <typename TLink>
  typename etl::enable_if<etl::is_same<TLink, etl::tree_link<TLink::ID, TLink::OPTION> >::value, void>::type
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
