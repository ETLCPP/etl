///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 Sergei Shirokov

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

#ifndef ETL_INTRUSIVE_AVL_TREE_INCLUDED
#define ETL_INTRUSIVE_AVL_TREE_INCLUDED

#include "platform.h"
#include "error_handler.h"
#include "intrusive_links.h"
#include "iterator.h"
#include "memory.h"
#include "type_traits.h"
#include "utility.h"

#include <stddef.h>

namespace etl
{
  //***************************************************************************
  /// Exception for the intrusive_avl_tree.
  /// \ingroup intrusive_avl_tree
  //***************************************************************************
  class intrusive_avl_tree_exception : public exception
  {
  public:

    intrusive_avl_tree_exception(const string_type reason_, const string_type file_name_, const numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Iterator exception for the intrusive_avl_tree.
  /// \ingroup intrusive_avl_tree
  //***************************************************************************
  class intrusive_avl_tree_iterator_exception : public intrusive_avl_tree_exception
  {
  public:

    intrusive_avl_tree_iterator_exception(const string_type file_name_, const numeric_type line_number_)
      : intrusive_avl_tree_exception(ETL_ERROR_TEXT("intrusive_avl_tree:iterator", ETL_INTRUSIVE_AVL_TREE_FILE_ID "A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Already-linked exception for the intrusive_avl_tree.
  ///\ingroup intrusive_avl_tree
  //***************************************************************************
  class intrusive_avl_tree_value_is_already_linked : public intrusive_avl_tree_exception
  {
  public:

    intrusive_avl_tree_value_is_already_linked(const string_type file_name_, const numeric_type line_number_)
      : intrusive_avl_tree_exception(ETL_ERROR_TEXT("intrusive_avl_tree:value is already linked", ETL_INTRUSIVE_AVL_TREE_FILE_ID"B"), file_name_,
                                                    line_number_)
    {
    }
  };

  //***************************************************************************
  /// \ingroup intrusive_avl_tree
  /// Base for intrusive AVL tree. Stores elements derived from 'intrusive_avl_tree_base<ID>::link_type'.
  /// \tparam ID_ The link ID that the value is derived from.
  //***************************************************************************
  template <size_t ID_>
  class intrusive_avl_tree_base
  {
  public:

    enum
    {
      ID = ID_,
    };

#if ETL_USING_CPP11
    intrusive_avl_tree_base(const intrusive_avl_tree_base&)            = delete;
    intrusive_avl_tree_base& operator=(const intrusive_avl_tree_base&) = delete;
#endif

    /// Base for elements of this AVL tree.
    /// It's expected that a tree node type is inherited from this base.
    ///
    /// Almost nothing is exposed from this type as public (or even protected) API -
    /// this is done deliberately and by design, so that:
    /// - impose as little "intrusive"-ness as possible when you inherit your nodes from this base.
    /// - don't worry about possible name conflicts
    /// - hide implementation details
    struct link_type : private etl::tree_link<ID_>
    {
    private:

      typedef etl::tree_link<ID_> base;

    public:

      link_type() ETL_NOEXCEPT
        : base()
        , etl_size(0)
      {
      }

#if ETL_USING_CPP11
      //***************************************************************************
      /// Constructs a new item by moving `other` item into `this` one.
      /// Complexity: O(1)
      /// After construction `this` item will replace `other` item
      /// in the same tree position as the `other` was, so no tree balancing is needed.
      /// The `other` item becomes unlinked.
      ///
      /// Note also that move construction/assignment of the `intrusive_avl_tree` class itself
      /// is heavily based on this move constructor for its `origin` sentinel item.
      //***************************************************************************
      link_type(link_type&& other) ETL_NOEXCEPT
        : base()
        , etl_size(0)
      {
        move_impl(other);
      }

      //***************************************************************************
      /// Assigns `other` item by moving it into `this` one.
      /// Does nothing in case of self-assignment (when `this == &other`).
      /// Complexity:
      /// - O(log(N)) if `this` item is already linked to a tree, and so
      ///   the item has to be erased from the tree (with rebalancing).
      /// - O(1) if `this` item is not linked to any tree.
      /// Might rotate the original tree of `this` item as necessary
      /// to keep the tree balanced after erasing the item.
      /// After assignment `this` item will replace `other` item
      /// in the same tree position as the `other` was, so no tree balancing is needed.
      /// The `other` item becomes unlinked.
      /// Note that after assignment `this` item might change its tree.
      //***************************************************************************
      link_type& operator=(link_type&& other) ETL_NOEXCEPT
      {
        if (this != &other)
        {
          // Before move assigning `other` we have to
          // make sure this item is not linked to any tree.
          erase_impl(this);
          move_impl(other);
        }
        return *this;
      }
#endif

#if ETL_USING_CPP11
      // Disable copy construction and assignment.
      link_type(const link_type&)            = delete;
      link_type& operator=(const link_type&) = delete;
#endif

      //***************************************************************************
      /// Destructor of a tree link.
      /// Complexity: O(log(N)).
      /// Might rotate the tree as necessary to keep it balanced.
      /// NB! The tree itself is not the real owner (of memory) of its nodes (and their inherited links).
      /// The real owner (aka the user) of a node could destroy it as he/she wishes,
      /// with or without prior explicit erasure from its tree. So, if the node link happens
      /// to be still linked to a tree during its destruction, then we have to unlink it -
      /// otherwise the node's parent and its former children will keep dangling pointers to the node,
      /// which will essentially break the tree consistency and lead to UB.
      //***************************************************************************
      ~link_type()
      {
        // We can't just remove item (by simple rewiring of links at parent and children)
        // b/c its former "owner" tree has to be rebalanced after the removal -
        // hence the full-blown erasing which might rotate the tree as necessary.
        erase_impl(this);
      }

    private:

      friend class intrusive_avl_tree_base;

      union
      {
        int_fast8_t etl_bf;   ///< Stores -1, 0, or +1 balancing factor in the real nodes.
        size_t      etl_size; ///< Stores total number of items in the tree (origin node only).
      };

#if ETL_USING_CPP11
#else
      // Disable copy construction and assignment.
      link_type(const link_type&);
      link_type& operator=(const link_type&);
#endif

      ETL_NODISCARD
      bool is_linked() const
      {
        return base::is_linked();
      }

      ETL_NODISCARD
      bool is_origin() const
      {
        return ETL_NULLPTR == base::etl_parent;
      }

      ETL_NODISCARD
      link_type* get_parent()
      {
        return static_cast<link_type*>(base::etl_parent);
      }

      ETL_NODISCARD
      const link_type* get_parent() const
      {
        return static_cast<const link_type*>(base::etl_parent);
      }

      ETL_NODISCARD
      bool is_child(const bool is_right) const
      {
        const link_type* parent = get_parent();
        return (ETL_NULLPTR != parent) && (this == parent->get_child(is_right));
      }

      ETL_NODISCARD
      bool is_left_child() const
      {
        return is_child(false);
      }

      ETL_NODISCARD
      bool is_right_child() const
      {
        return is_child(true);
      }

      ETL_NODISCARD
      link_type* get_child(const bool is_right)
      {
        return static_cast<link_type*>(is_right ? base::etl_right : base::etl_left);
      }

      ETL_NODISCARD
      const link_type* get_child(const bool is_right) const
      {
        return static_cast<const link_type*>(is_right ? base::etl_right : base::etl_left);
      }

      void set_child(link_type* const child, const bool is_right)
      {
        base*& child_ref = is_right ? base::etl_right : base::etl_left;
        child_ref        = child;
      }

      ETL_NODISCARD
      link_type* get_left()
      {
        return static_cast<link_type*>(base::etl_left);
      }

      ETL_NODISCARD
      const link_type* get_left() const
      {
        return static_cast<const link_type*>(base::etl_left);
      }

      ETL_NODISCARD
      link_type* get_right()
      {
        return static_cast<link_type*>(base::etl_right);
      }

      ETL_NODISCARD
      const link_type* get_right() const
      {
        return static_cast<const link_type*>(base::etl_right);
      }

      void move_impl(link_type& other)
      {
        const bool is_right = other.is_right_child();
        base::etl_parent    = other.etl_parent;
        base::etl_left      = other.etl_left;
        base::etl_right     = other.etl_right;
        etl_size            = other.etl_size;

        other.clear();
        other.etl_size = 0;

        if (ETL_NULLPTR != base::etl_parent)
        {
          get_parent()->link_child(this, is_right);
        }
        if (ETL_NULLPTR != base::etl_left)
        {
          get_left()->set_parent(this);
        }
        if (ETL_NULLPTR != base::etl_right)
        {
          get_right()->set_parent(this);
        }
      }

      void rotate(const bool is_right)
      {
        const bool       was_right = is_right_child();
        link_type* const child     = get_child(!is_right);
        etl::link_rotate<base>(this, child);
        if (link_type* const parent = child->get_parent())
        {
          parent->set_child(child, was_right);
        }
      }

      ETL_NODISCARD
      link_type* adjust_balance(const bool increase)
      {
        const int_fast8_t new_bf = etl_bf + (increase ? +1 : -1);
        if ((-1 <= new_bf) && (new_bf <= +1))
        {
          etl_bf = new_bf;
          return this;
        }

        const bool        is_right_rotation = new_bf < 0;
        const int_fast8_t sign              = is_right_rotation ? +1 : -1;
        link_type* const  z_leaf            = get_child(!is_right_rotation);
        if (z_leaf->etl_bf * sign <= 0)
        {
          rotate(is_right_rotation);
          if (z_leaf->etl_bf == 0)
          {
            etl_bf         = -sign;
            z_leaf->etl_bf = +sign;
          }
          else
          {
            etl_bf         = 0;
            z_leaf->etl_bf = 0;
          }
          return z_leaf;
        }

        link_type* const y_leaf = z_leaf->get_child(is_right_rotation);
        z_leaf->rotate(!is_right_rotation);
        rotate(is_right_rotation);
        if (y_leaf->etl_bf == 0)
        {
          etl_bf         = 0;
          z_leaf->etl_bf = 0;
        }
        else if (y_leaf->etl_bf == sign)
        {
          etl_bf         = 0;
          y_leaf->etl_bf = 0;
          z_leaf->etl_bf = -sign;
        }
        else
        {
          etl_bf         = +sign;
          y_leaf->etl_bf = 0;
          z_leaf->etl_bf = 0;
        }
        return y_leaf;
      }

      void link_child(link_type* child, const bool is_right)
      {
        if (is_right)
        {
          etl::link_right<base>(this, child);
        }
        else
        {
          etl::link_left<base>(this, child);
        }
      }

    }; // link_type

    //*************************************************************************
    /// Checks if the tree is in the empty state.
    /// Complexity: O(1).
    //*************************************************************************
    ETL_NODISCARD
    bool empty() const ETL_NOEXCEPT
    {
      return ETL_NULLPTR == get_root();
    }

    //*************************************************************************
    /// Returns the number of elements.
    /// Complexity: O(1).
    //*************************************************************************
    ETL_NODISCARD
    size_t size() const ETL_NOEXCEPT
    {
      return get_origin().etl_size;
    }

    //*************************************************************************
    /// Unlinks all current items, leaving this tree in the empty state.
    /// Complexity: O(N).
    /// Operation invalidates all existing iterators.
    ///
    /// Note that the same "clear all" effect could be achieved by using the `erase`
    /// method for each item, but b/c of intermediate tree rebalancing
    /// complexity will be higher - O(N*log(N)).
    //*************************************************************************
    void clear() ETL_NOEXCEPT
    {
#if ETL_USING_CPP11
      auto unlinker = [](link_type& link)
      {
        link.clear();
        link.etl_size = 0;
      };
#else
      struct
      {
        void operator()(link_type& link) const
        {
          link.clear();
          link.etl_size = 0;
        }
      } unlinker;
#endif

      // No need to balance b/c everything will be unlinked.
      // Note that "post-order" visitation is important -
      // it ensures that once a link is passed to the "visitor" functor,
      // traversal won't use a pointer to this link anymore,
      // so we could efficiently clear the link.
      visit_post_order_impl(&origin, false, unlinker);
      origin.set_left(ETL_NULLPTR);
      origin.etl_size = 0;
    }

    //*******************************************
    /// Swap with another tree.
    /// Complexity: O(1).
    /// Does nothing in case of self-swap (when `this == &other`).
    //*******************************************
    void swap(intrusive_avl_tree_base& other) ETL_NOEXCEPT
    {
      if (this != &other)
      {
        ETL_OR_STD::swap(origin, other.origin);
      }
    }

    //*************************************************************************
    /// Swaps the trees.
    /// Complexity: O(1).
    /// Does nothing in case of self-swap (when `&lhs == &rhs`).
    //*************************************************************************
    friend void swap(intrusive_avl_tree_base& lhs, intrusive_avl_tree_base& rhs) ETL_NOEXCEPT
    {
      if (&lhs != &rhs)
      {
        lhs.swap(rhs);
      }
    }

  protected:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    intrusive_avl_tree_base() ETL_NOEXCEPT {}

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move constructor.
    /// Complexity: O(1).
    /// NB! Proper `= default` move behavior is actually based on the move
    /// construction of the `origin` link (see `link_type(link_type&& other)`).
    //*************************************************************************
    intrusive_avl_tree_base(intrusive_avl_tree_base&&) ETL_NOEXCEPT = default;

    //*************************************************************************
    /// Move assignment.
    /// Does nothing in case of self-assignment (when `this == &other`).
    /// Complexity: O(N), where N is the size of `this` tree before assignment -
    /// all former items have to be unlinked.
    //*************************************************************************
    intrusive_avl_tree_base& operator=(intrusive_avl_tree_base&& other) ETL_NOEXCEPT
    {
      if (this != &other)
      {
        intrusive_avl_tree_base tmp(etl::move(other));
        swap(tmp);
      }
      return *this;
    }
#endif

    //*************************************************************************
    /// Destructor of a tree.
    /// Complexity: O(N).
    /// All existing nodes will stay alive
    /// but will be completely unlinked from the tree (and from each other).
    /// If you want more control on what happens with still linked tree nodes, then
    /// enumerate and unlink (aka "erase") them first (potentially moving to somewhere else).
    /// If needed, the whole content of the tree could be O(1) effectively moved
    /// to another tree, leaving this tree as empty - use C++11 move constructor of the tree.
    //***************************************************************************
    ~intrusive_avl_tree_base()
    {
      // It's important to clear, so that none of the former (but still alive) items
      // stay linked to this tree (neither directly at the root item
      // nor transitively via "parent" links from leaf items up to the origin).
      clear();
    }

    ETL_NODISCARD
    link_type* get_root() ETL_NOEXCEPT
    {
      return static_cast<link_type*>(origin.etl_left);
    }

    ETL_NODISCARD
    const link_type* get_root() const ETL_NOEXCEPT
    {
      return static_cast<const link_type*>(origin.etl_left);
    }

    ETL_NODISCARD
    link_type& get_origin() ETL_NOEXCEPT
    {
      return origin;
    }

    const link_type& get_origin() const ETL_NOEXCEPT
    {
      return origin;
    }

    template <typename TLink>
    ETL_NODISCARD
    static TLink* get_origin(TLink* link) ETL_NOEXCEPT
    {
      while ((ETL_NULLPTR != link) && !link->is_origin())
      {
        link = link->get_parent();
      }
      return link;
    }

    ETL_NODISCARD
    static bool is_real_link(const link_type* link) ETL_NOEXCEPT
    {
      return (ETL_NULLPTR != link) && !link->is_origin();
    }

    template <typename TLink>
    ETL_NODISCARD
    static TLink* begin_impl(TLink& origin) ETL_NOEXCEPT
    {
      TLink* curr = &origin;
      TLink* next = curr->get_child(false);
      while (ETL_NULLPTR != next)
      {
        curr = next;
        next = next->get_child(false);
      }
      return curr;
    }

    template <typename TLink>
    ETL_NODISCARD
    static TLink* end_impl(TLink& origin) ETL_NOEXCEPT
    {
      return &origin;
    }

    template <typename TLink>
    ETL_NODISCARD
    static TLink* find_extremum_impl(TLink* curr, const bool is_max) ETL_NOEXCEPT
    {
      if (ETL_NULLPTR != curr)
      {
        TLink* next = curr->get_child(is_max);
        while (ETL_NULLPTR != next)
        {
          curr = next;
          next = curr->get_child(is_max);
        }
      }
      return curr;
    }

    template <typename TLink>
    ETL_NODISCARD
    static TLink* next_in_order_impl(TLink* curr) ETL_NOEXCEPT
    {
      if ((ETL_NULLPTR == curr) || curr->is_origin())
      {
        return curr;
      }

      if (TLink* const next = curr->get_child(true))
      {
        return find_extremum_impl(next, false);
      }

      while (curr->is_right_child())
      {
        curr = curr->get_parent();
      }
      return curr->get_parent();
    }

    template <typename TLink>
    ETL_NODISCARD
    static TLink* prev_in_order_impl(TLink* curr) ETL_NOEXCEPT
    {
      if (ETL_NULLPTR == curr)
      {
        return ETL_NULLPTR;
      }

      if (TLink* const next = curr->get_child(false))
      {
        return find_extremum_impl(next, true);
      }

      while (curr->is_left_child())
      {
        curr = curr->get_parent();
      }
      return curr->is_origin() ? curr : curr->get_parent();
    }

    template <typename TLink, typename Visitor>
    static void visit_in_order_impl(TLink* curr, const bool is_reverse, Visitor visitor)
    {
      TLink* prev = ETL_NULLPTR;
      while (ETL_NULLPTR != curr)
      {
        TLink* next = curr->get_parent();
        if (prev == next)
        {
          if (TLink* const child1 = curr->get_child(is_reverse))
          {
            next = child1;
          }
          else
          {
            if (!curr->is_origin())
            {
              visitor(*curr);
            }

            if (TLink* const child2 = curr->get_child(!is_reverse))
            {
              next = child2;
            }
          }
        }
        else if (prev == curr->get_child(is_reverse))
        {
          if (!curr->is_origin())
          {
            visitor(*curr);
          }

          if (TLink* const child2 = curr->get_child(!is_reverse))
          {
            next = child2;
          }
        }

        prev = etl::exchange(curr, next);
      }
    }

    template <typename TLink, typename Visitor>
    static void visit_post_order_impl(TLink* curr, const bool is_reverse, Visitor visitor)
    {
      TLink* prev = ETL_NULLPTR;
      while (ETL_NULLPTR != curr)
      {
        TLink* next = curr->get_parent();
        if (prev == next)
        {
          if (TLink* const child1 = curr->get_child(is_reverse))
          {
            next = child1;
          }
          else if (TLink* const child2 = curr->get_child(!is_reverse))
          {
            next = child2;
          }
          else if (!curr->is_origin())
          {
            visitor(*curr);
          }
        }
        else if (prev == curr->get_child(is_reverse))
        {
          if (TLink* const child2 = curr->get_child(!is_reverse))
          {
            next = child2;
          }
          else if (!curr->is_origin())
          {
            visitor(*curr);
          }
        }
        else if (!curr->is_origin())
        {
          visitor(*curr);
        }

        prev = etl::exchange(curr, next);
      }
    }

    template <typename TLink, typename Visitor>
    static void visit_pre_order_impl(TLink* curr, const bool is_reverse, Visitor visitor)
    {
      TLink* prev = ETL_NULLPTR;
      while (ETL_NULLPTR != curr)
      {
        TLink* next = curr->get_parent();
        if (prev == next)
        {
          if (!curr->is_origin())
          {
            visitor(*curr);
          }

          if (TLink* const child1 = curr->get_child(is_reverse))
          {
            next = child1;
          }
          else if (TLink* const child2 = curr->get_child(!is_reverse))
          {
            next = child2;
          }
        }
        else if (prev == curr->get_child(is_reverse))
        {
          if (TLink* const child2 = curr->get_child(!is_reverse))
          {
            next = child2;
          }
        }

        prev = etl::exchange(curr, next);
      }
    }

    ETL_NODISCARD
    static int_fast8_t get_balance_factor_impl(const link_type* const curr) ETL_NOEXCEPT
    {
      return (ETL_NULLPTR != curr) ? curr->etl_bf : 0;
    }

    template <typename TLink>
    ETL_NODISCARD
    static TLink* get_parent_impl(TLink* const curr) ETL_NOEXCEPT
    {
      if (ETL_NULLPTR == curr)
      {
        return ETL_NULLPTR;
      }
      TLink* const parent = curr->get_parent();
      if ((ETL_NULLPTR == parent) || parent->is_origin())
      {
        return ETL_NULLPTR;
      }
      return parent;
    }

    template <typename TLink>
    ETL_NODISCARD
    static TLink* get_child_impl(TLink* const curr, const bool is_right) ETL_NOEXCEPT
    {
      if (ETL_NULLPTR == curr)
      {
        return ETL_NULLPTR;
      }
      return curr->get_child(is_right);
    }

    template <typename TValue, typename TIterator, typename TBinaryCompare>
    void assign_impl(TIterator first, TIterator last, TBinaryCompare binary_comp)
    {
#if ETL_IS_DEBUG_BUILD
      const intmax_t diff = etl::distance(first, last);
      ETL_ASSERT(diff >= 0, ETL_ERROR(intrusive_avl_tree_iterator_exception));
#endif

      // Add all the elements.
      while (first != last)
      {
        link_type& link  = *first++;
        TValue&    value = static_cast<TValue&>(link);
#if ETL_USING_CPP11
        find_or_insert_impl<TValue>(                                                         //
          [&value, &binary_comp](const TValue& other) { return binary_comp(value, other); }, //
          [&value] { return &value; });
#else
        const CompareFactory<TValue, TBinaryCompare> compareFactory(value, binary_comp);
        find_or_insert_impl<TValue>(compareFactory, compareFactory);
#endif
      }
    }

    template <typename TValue, typename TLink, typename TCompare>
    static TValue* find_impl(TLink* const root, TCompare comp)
    {
      // Try to find existing node.
      TLink* curr = root;
      while (ETL_NULLPTR != curr)
      {
        TValue* const result = static_cast<TValue*>(curr);
        const int     cmp    = comp(*result);
        if (0 == cmp)
        {
          // Found!
          return result;
        }

        curr = curr->get_child(cmp > 0);
      }

      // Not found.
      return ETL_NULLPTR;
    }

    template <typename TValue, typename TCompare, typename TFactory>
    etl::pair<TValue*, bool> find_or_insert_impl(TCompare comp, TFactory factory)
    {
      // Try to find existing node.
      bool       is_right = false;
      link_type* curr     = get_root();
      link_type* parent   = &origin;
      while (ETL_NULLPTR != curr)
      {
        TValue* const result = static_cast<TValue*>(curr);
        const int     cmp    = comp(*result);
        if (0 == cmp)
        {
          // Found! Tree was not modified.
          return etl::make_pair(result, false);
        }

        parent   = curr;
        is_right = cmp > 0;
        curr     = curr->get_child(is_right);
      }

      // Try to instantiate new node.
      TValue* const result = factory();
      if (ETL_NULLPTR == result)
      {
        // Failed (or rejected)! The tree was not modified.
        return etl::make_pair(ETL_NULLPTR, false);
      }

      link_type& result_link = static_cast<link_type&>(*result);
      ETL_ASSERT(!(result_link.is_linked()), ETL_ERROR(intrusive_avl_tree_value_is_already_linked));

      // Link the new node.
      parent->link_child(&result_link, is_right);
      get_origin(parent)->etl_size += 1;

      retrace_on_insert(&result_link);

      // Successfully linked, so the tree was modified.
      return etl::make_pair(result, true);
    }

    template <bool IsUpper, typename TValue, typename TLink, typename TCompare>
    static TValue* find_bound_impl(TLink* const root, TCompare comp)
    {
      TValue* result = ETL_NULLPTR;
      TLink*  next   = root;
      while (ETL_NULLPTR != next)
      {
        TValue* const next_value = static_cast<TValue*>(next);
        const int     cmp        = comp(*next_value);
        if ((cmp > 0) || (IsUpper && (cmp == 0)))
        {
          next = next->get_right();
        }
        else
        {
          result = next_value;
          next   = next->get_left();
        }
      }
      return result;
    }

    static void erase_impl(link_type* const z_link) ETL_NOEXCEPT
    {
      // Remove only real and still-linked items.
      // Tree itself uses this `link_type` for its `origin` sentinel item,
      // but you can't (and there is no need to) erase it.
      if ((ETL_NULLPTR == z_link) || z_link->is_origin() || !z_link->is_linked())
      {
        return;
      }

      link_type* parent   = z_link->get_parent();
      bool       is_right = z_link->is_right_child();

      if (!z_link->has_left())
      {
        parent->link_child(z_link->get_right(), is_right);
      }
      else if (!z_link->has_right())
      {
        parent->link_child(z_link->get_left(), is_right);
      }
      else
      {
        link_type* const y_link        = next_in_order_impl(z_link);
        link_type* const y_link_parent = y_link->get_parent();
        y_link->etl_bf                 = z_link->etl_bf;
        if (z_link != y_link_parent)
        {
          y_link_parent->link_child(y_link->get_right(), y_link->is_right_child());
          link_type* const z_right = z_link->get_right();
          y_link->set_right(z_right);
          z_right->set_parent(y_link);
          parent->link_child(y_link, is_right);

          is_right = false;
          parent   = y_link_parent;
        }
        else
        {
          parent->link_child(y_link, is_right);

          is_right = true;
          parent   = y_link;
        }
        link_type* const z_left = z_link->get_left();
        y_link->set_left(z_left);
        z_left->set_parent(y_link);
      }

      z_link->clear();
      z_link->etl_size = 0;
      get_origin(parent)->etl_size -= 1;

      retrace_on_erase(parent, is_right);
    }

  private:

    /// This is the origin link whose left child points to the root link (if any) of the tree.
    /// This link instance is an internal implementation detail, and it's almost never exposed to the public
    /// API of the tree - the only exception is its natural usage for the "end" terminal iterator.
    /// So, the end iterator contains the address of this sentinel node (rather than a non-null pointer).
    link_type origin;

#if ETL_USING_CPP11
#else
    // Disable copy construction and assignment.
    intrusive_avl_tree_base(const intrusive_avl_tree_base&);
    intrusive_avl_tree_base& operator=(const intrusive_avl_tree_base&);

    template <typename TValue, typename TBinaryCompare>
    struct CompareFactory
    {
      TValue&        value_ref;
      TBinaryCompare binary_comp;

      CompareFactory(TValue& value, TBinaryCompare comp)
        : value_ref(value)
        , binary_comp(comp)
      {
      }

      /// Adapts binary comparator to a "unary" one.
      ETL_NODISCARD
      int operator()(const TValue& other) const
      {
        return binary_comp(value_ref, other);
      }

      /// Fake "factory" that returns address of existing value.
      ETL_NODISCARD
      TValue* operator()() const
      {
        return &value_ref;
      }

    }; // CompareFactory
#endif

    static void retrace_on_insert(link_type* curr)
    {
      link_type* parent = curr->get_parent();
      while (!parent->is_origin())
      {
        const bool is_right = curr->is_right_child();
        curr                = parent->adjust_balance(is_right);
        parent              = curr->get_parent();
        if (curr->etl_bf == 0)
        {
          break;
        }
      }

      if (parent->is_origin())
      {
        parent->set_left(curr);
      }
    }

    static void retrace_on_erase(link_type* parent, bool is_right)
    {
      while (!parent->is_origin())
      {
        link_type* const curr = parent->adjust_balance(!is_right);
        parent                = curr->get_parent();
        if ((curr->etl_bf != 0) || parent->is_origin())
        {
          if (parent->is_origin())
          {
            parent->set_left(curr);
          }
          break;
        }
        is_right = curr->is_right_child();
      }
    }

  }; // intrusive_avl_tree_base

  //***************************************************************************
  /// \ingroup intrusive_avl_tree
  /// An intrusive AVL tree. Stores elements derived from 'intrusive_avl_tree<ID>::link_type'.
  ///
  /// NB! The tree itself is not the real owner (of memory) of its nodes.
  /// The node `TValue` type contains all the necessary means (via required inheritance from `link_type`)
  /// to be able to participate in the tree as a "linked" node - hence the "intrusive" term.
  ///
  /// \warning This tree cannot be used for concurrent access from multiple threads.
  /// \tparam TValue The type of value that the tree holds.
  /// \tparam ID_ The link ID that the value is derived from.
  //***************************************************************************
  template <typename TValue, size_t ID_ = 0>
  class intrusive_avl_tree : public etl::intrusive_avl_tree_base<ID_>
  {
    typedef etl::intrusive_avl_tree_base<ID_> base;

  public:

    // Node typedef.
    typedef typename base::link_type link_type;

    // STL style typedefs.
    typedef TValue            value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef size_t            size_type;
#if ETL_USING_CPP11
    typedef value_type&& rvalue_reference;
#endif

    //*************************************************************************
    /// Defines iterator of mutable items of the tree.
    ///
    /// Normally, an iterator might...
    /// - either reference an existing item;
    /// - or be equal to the `end`.
    /// Iterator could also be in "valueless" state:
    /// - see default constructor;
    /// - special cases of `find_or_insert`;
    /// - advanced traversal methods, like `get_parent` and `get_child`.
    /// Both "end" and "valueless" conditions could be easily isolated using `has_value` or `bool operator`:
    /// - `has_value() == true` - iterator references a real node
    /// - `it == end()` - terminal sentinel
    /// - `!has_value() && it != end()` - valueless iterator
    /// The real item (referenced by the iterator) could be modified by the user,
    /// but so that it doesn't affect current ordering of the tree.
    //*************************************************************************
    class iterator : public etl::iterator<ETL_OR_STD::bidirectional_iterator_tag, value_type>
    {
    public:

      friend class intrusive_avl_tree;
      friend class const_iterator;

      iterator() ETL_NOEXCEPT
        : p_value(ETL_NULLPTR)
      {
      }

      //*************************************************************************
      /// Increments iterator to point to the next ("greater") item.
      /// Complexity: amortized O(1), worst-case O(log(N)).
      //*************************************************************************
      iterator& operator++() ETL_NOEXCEPT
      {
        p_value = base::next_in_order_impl(p_value);
        return *this;
      }

      //*************************************************************************
      /// Increments iterator to point to the next ("greater") item.
      /// Complexity: amortized O(1), worst-case O(log(N)).
      /// Returns original iterator (before the increment).
      //*************************************************************************
      iterator operator++(int) ETL_NOEXCEPT
      {
        iterator temp(*this);
        p_value = base::next_in_order_impl(p_value);
        return temp;
      }

      //*************************************************************************
      /// Decrements iterator to point to the previous ("smaller") item.
      /// Complexity: amortized O(1), worst-case O(log(N)).
      //*************************************************************************
      iterator& operator--() ETL_NOEXCEPT
      {
        p_value = base::prev_in_order_impl(p_value);
        return *this;
      }

      //*************************************************************************
      /// Decrements iterator to point to the previous ("smaller") item.
      /// Complexity: amortized O(1), worst-case O(log(N)).
      /// Returns original iterator (before the decrement).
      //*************************************************************************
      iterator operator--(int) ETL_NOEXCEPT
      {
        iterator temp(*this);
        p_value = base::prev_in_order_impl(p_value);
        return temp;
      }

      //*************************************************************************
      /// Dereferences an existing mutable item.
      /// Could be used only for iterator which has value (`has_value() == true`).
      /// If asserts or exceptions are enabled, throws an `etl::intrusive_avl_tree_iterator_exception`
      ///   if iterator doesn't reference a real item.
      //*************************************************************************
      reference operator*() const
      {
        ETL_ASSERT(has_value(), ETL_ERROR(intrusive_avl_tree_iterator_exception));

#include "private/diagnostic_null_dereference_push.h"

        return *static_cast<pointer>(p_value);
#include "private/diagnostic_pop.h"
      }

      //*************************************************************************
      /// Gets address of an existing mutable item (if any).
      /// Returns `nullptr` if iterator doesn't reference a real item.
      //*************************************************************************
      pointer get() const ETL_NOEXCEPT
      {
        return static_cast<pointer>(has_value() ? p_value : ETL_NULLPTR);
      }

      //*************************************************************************
      /// Dereferences an existing mutable item.
      /// Could be used only for iterator which has value (`has_value() == true`).
      /// If asserts or exceptions are enabled, throws an `etl::intrusive_avl_tree_iterator_exception`
      ///   if iterator doesn't reference a real item.
      //*************************************************************************
      pointer operator->() const
      {
        ETL_ASSERT_OR_RETURN_VALUE(has_value(), ETL_ERROR(intrusive_avl_tree_iterator_exception), ETL_NULLPTR);

        return static_cast<pointer>(p_value);
      }

      friend bool operator==(const iterator& lhs, const iterator& rhs) ETL_NOEXCEPT
      {
        return lhs.p_value == rhs.p_value;
      }

      friend bool operator!=(const iterator& lhs, const iterator& rhs) ETL_NOEXCEPT
      {
        return !(lhs == rhs);
      }

      ETL_EXPLICIT operator bool() const ETL_NOEXCEPT
      {
        return has_value();
      }

      ETL_NODISCARD
      bool has_value() const ETL_NOEXCEPT
      {
        return base::is_real_link(p_value);
      }

      //*************************************************************************
      /// Gets balance factor of tree node.
      /// Complexity: O(1).
      /// Normally is not needed unless advanced traversal is required.
      /// Result: -1, 0 or +1 depending on children tree height difference.
      //*************************************************************************
      ETL_NODISCARD
      int_fast8_t get_balance_factor() const ETL_NOEXCEPT
      {
        return base::get_balance_factor_impl(p_value);
      }

      //*************************************************************************
      /// Gets parent node.
      /// Complexity: O(1).
      /// Normally is not needed unless advanced traversal is required.
      /// Result iterator will be valueless (`has_value() == false`) if there is no parent.
      //*************************************************************************
      ETL_NODISCARD
      iterator get_parent() const ETL_NOEXCEPT
      {
        return iterator(base::get_parent_impl(p_value));
      }

      //*************************************************************************
      /// Gets a child node.
      /// Complexity: O(1).
      /// Normally is not needed unless advanced traversal is required.
      /// Result iterator will be valueless (`has_value() == false`) if there is no such child.
      //*************************************************************************
      ETL_NODISCARD
      iterator get_child(const bool is_right) const ETL_NOEXCEPT
      {
        return iterator(base::get_child_impl(p_value, is_right));
      }

    private:

      ETL_EXPLICIT iterator(link_type* value)
        : p_value(value)
      {
      }

      link_type* p_value;

    }; // iterator

    //*************************************************************************
    /// Defines constant iterator of immutable items of the tree.
    ///
    /// Normally, an iterator might...
    /// - either reference an existing item;
    /// - or be equal to the `end`.
    /// Iterator could also be in "valueless" state:
    /// - see default constructor;
    /// - advanced traversal methods, like `get_parent` and `get_child`.
    /// Both "end" and "valueless" conditions could be easily isolated using `has_value` or `bool operator`:
    /// - `has_value() == true` - iterator references a real node
    /// - `it == end()` - terminal sentinel
    /// - `!has_value() && it != end()` - valueless iterator
    //*************************************************************************
    class const_iterator : public etl::iterator<ETL_OR_STD::bidirectional_iterator_tag, const value_type>
    {
    public:

      friend class intrusive_avl_tree;

      const_iterator() ETL_NOEXCEPT
        : p_value(ETL_NULLPTR)
      {
      }

      // Implicit by design - it's always safe to make const iterator from ordinary one.
      const_iterator(const typename intrusive_avl_tree::iterator& other) ETL_NOEXCEPT // NOLINT
        : p_value(other.p_value)
      {
      }

      //*************************************************************************
      /// Increments const iterator to point to the next ("greater") item.
      /// Complexity: amortized O(1), worst-case O(log(N)).
      //*************************************************************************
      const_iterator& operator++() ETL_NOEXCEPT
      {
        p_value = base::next_in_order_impl(p_value);
        return *this;
      }

      //*************************************************************************
      /// Increments const iterator to point to the next ("greater") item.
      /// Complexity: amortized O(1), worst-case O(log(N)).
      /// Returns original iterator (before the increment).
      const_iterator operator++(int) ETL_NOEXCEPT
      {
        const_iterator temp(*this);
        p_value = base::next_in_order_impl(p_value);
        return temp;
      }

      //*************************************************************************
      /// Decrements const iterator to point to the previous ("smaller") item.
      /// Complexity: amortized O(1), worst-case O(log(N)).
      //*************************************************************************
      const_iterator& operator--() ETL_NOEXCEPT
      {
        p_value = base::prev_in_order_impl(p_value);
        return *this;
      }

      //*************************************************************************
      /// Decrements const iterator to point to the previous ("smaller") item.
      /// Complexity: amortized O(1), worst-case O(log(N)).
      /// Returns original iterator (before the decrement).
      //*************************************************************************
      const_iterator operator--(int) ETL_NOEXCEPT
      {
        const_iterator temp(*this);
        p_value = base::prev_in_order_impl(p_value);
        return temp;
      }

      //*************************************************************************
      /// Dereferences an existing immutable item.
      /// Could be used only for iterator which has value (`has_value() == true`).
      /// If asserts or exceptions are enabled, throws an `etl::intrusive_avl_tree_iterator_exception`
      ///   if iterator doesn't reference a real item.
      //*************************************************************************
      const_reference operator*() const
      {
        ETL_ASSERT(has_value(), ETL_ERROR(intrusive_avl_tree_iterator_exception));

#include "private/diagnostic_null_dereference_push.h"

        return *static_cast<const_pointer>(p_value);
#include "private/diagnostic_pop.h"
      }

      //*************************************************************************
      /// Gets address of an existing immutable item (if any).
      /// Returns `nullptr` if iterator doesn't reference a real item.
      //*************************************************************************
      const_pointer get() const ETL_NOEXCEPT
      {
        return static_cast<const_pointer>(has_value() ? p_value : ETL_NULLPTR);
      }

      //*************************************************************************
      /// Dereferences an existing immutable item.
      /// Could be used only for iterator which has value (`has_value() == true`).
      /// If asserts or exceptions are enabled, throws an `etl::intrusive_avl_tree_iterator_exception`
      ///   if iterator doesn't reference a real item.
      //*************************************************************************
      const_pointer operator->() const
      {
        ETL_ASSERT_OR_RETURN_VALUE(has_value(), ETL_ERROR(intrusive_avl_tree_iterator_exception), ETL_NULLPTR);

        return static_cast<const_pointer>(p_value);
      }

      friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) ETL_NOEXCEPT
      {
        return lhs.p_value == rhs.p_value;
      }

      friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) ETL_NOEXCEPT
      {
        return !(lhs == rhs);
      }

      ETL_EXPLICIT operator bool() const ETL_NOEXCEPT
      {
        return has_value();
      }

      ETL_NODISCARD
      bool has_value() const ETL_NOEXCEPT
      {
        return base::is_real_link(p_value);
      }

      //*************************************************************************
      /// Gets balance factor of tree node.
      /// Complexity: O(1).
      /// Normally is not needed unless advanced traversal is required.
      /// Result: -1, 0 or +1 depending on children tree height difference.
      //*************************************************************************
      ETL_NODISCARD
      int_fast8_t get_balance_factor() const ETL_NOEXCEPT
      {
        return base::get_balance_factor_impl(p_value);
      }

      //*************************************************************************
      /// Gets parent node.
      /// Complexity: O(1).
      /// Normally is not needed unless advanced traversal is required.
      /// Result iterator will be valueless (`has_value() == false`) if there is no parent.
      //*************************************************************************
      ETL_NODISCARD
      const_iterator get_parent() const ETL_NOEXCEPT
      {
        return const_iterator(base::get_parent_impl(p_value));
      }

      //*************************************************************************
      /// Gets a child node.
      /// Complexity: O(1).
      /// Normally is not needed unless advanced traversal is required.
      /// Result iterator will be valueless (`has_value() == false`) if there is no such child.
      //*************************************************************************
      ETL_NODISCARD
      const_iterator get_child(const bool is_right) const ETL_NOEXCEPT
      {
        return const_iterator(base::get_child_impl(p_value, is_right));
      }

    private:

      ETL_EXPLICIT const_iterator(const link_type* value)
        : p_value(value)
      {
      }

      const link_type* p_value;

    }; // const_iterator

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    intrusive_avl_tree() ETL_NOEXCEPT
      : intrusive_avl_tree_base<ID_>()
    {
    }

    //*************************************************************************
    /// Constructor from range of items.
    /// Complexity: O(N*log(N)).
    /// NB! All items in the range must be in the "unlinked" state initially; otherwise the "already linked" exception is thrown.
    /// On success, all inserted items (except possible duplicates; see the comparator description below) will be linked.
    ///
    /// The binary comparator should accept two `const value_type&` arguments (aka `lhs` and `rhs`),
    /// and return integer result (aka `lhs - rhs` "subtraction" result):
    /// - `>0` if the `lhs` is "greater" than the `rhs`
    /// - `0` if the `lhs` is "equal" to the `rhs`
    /// - `<0` if the `lhs` is "smaller" than the `rhs`
    /// - if throws then exception is propagated
    /// Hints:
    /// - if you want duplicates then always return either `>0` or `<0`, even for equal arguments:
    ///   - `<0` (e.g. `-1`) will prepend a duplicate item to the tree
    ///   - `>0` (e.g. `+1`) will append a duplicate item to the tree
    /// - if you want to avoid duplicates in the result tree then use `0` for "equal" arguments -
    ///   in such case only the first item of duplicates in the range
    ///   will be linked to the tree; the rest will be skipped (left unlinked)
    /// - `etl::compare<value_type>::cmp` could be directly used as comparator (if value's `less` is defined) -
    ///   its `Less`, `Equal` and `Greater` are convertible/compatible with expected result integers.
    ///
    /// If asserts or exceptions are enabled, throws:
    /// - an etl::intrusive_avl_tree_iterator_exception if the `first` > `last` (DEBUG build only).
    /// - an etl::intrusive_avl_tree_value_is_already_linked if any item (in `[first, last)` range) is already linked to some other tree.
    /// - whatever the `binary_comp` might throw
    /// Any exception during tree building will unlink already inserted nodes - so it's all or nothing.
    //*************************************************************************
    template <typename TIterator, typename TBinaryCompare>
    intrusive_avl_tree(TIterator first, TIterator last, TBinaryCompare binary_comp,
                       typename etl::enable_if<!etl::is_integral<TIterator>::value, int>::type = 0)
    {
      base::template assign_impl<value_type>(first, last, binary_comp);
    }

    //*************************************************************************
    /// Destructor.
    /// Complexity: O(N).
    //*************************************************************************
    ~intrusive_avl_tree() {}

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move constructor.
    /// Complexity: O(1).
    //*************************************************************************
    intrusive_avl_tree(intrusive_avl_tree&&) ETL_NOEXCEPT = default;

    //*************************************************************************
    /// Move assignment.
    /// Does nothing in case of self-assignment (when `this == &other`).
    /// Complexity: O(N), where N is the size of `this` tree before assignment -
    /// all former items have to be unlinked.
    //*************************************************************************
    intrusive_avl_tree& operator=(intrusive_avl_tree&&) ETL_NOEXCEPT = default;

    intrusive_avl_tree(const intrusive_avl_tree&)            = delete;
    intrusive_avl_tree& operator=(const intrusive_avl_tree&) = delete;
#endif

    //*************************************************************************
    /// Gets the beginning of the intrusive_avl_tree.
    /// Complexity: O(log(N)).
    /// Returns `end` if tree is empty.
    //*************************************************************************
    ETL_NODISCARD
    iterator begin() ETL_NOEXCEPT
    {
      return iterator(base::begin_impl(base::get_origin()));
    }

    //*************************************************************************
    /// Gets the beginning of the intrusive_avl_tree.
    /// Complexity: O(log(N)).
    /// Returns `end` if tree is empty.
    //*************************************************************************
    ETL_NODISCARD
    const_iterator begin() const ETL_NOEXCEPT
    {
      return const_iterator(base::begin_impl(base::get_origin()));
    }

    //*************************************************************************
    /// Gets the beginning of the intrusive_avl_tree.
    /// Complexity: O(log(N)).
    /// Returns `end` if tree is empty.
    //*************************************************************************
    ETL_NODISCARD
    const_iterator cbegin() const ETL_NOEXCEPT
    {
      return begin();
    }

    //*************************************************************************
    /// Gets the end of the intrusive_avl_tree.
    /// Complexity: O(1).
    //*************************************************************************
    ETL_NODISCARD
    iterator end() ETL_NOEXCEPT
    {
      return iterator(base::end_impl(base::get_origin()));
    }

    //*************************************************************************
    /// Gets the end of the intrusive_avl_tree.
    /// Complexity: O(1).
    //*************************************************************************
    ETL_NODISCARD
    const_iterator end() const ETL_NOEXCEPT
    {
      return const_iterator(base::end_impl(base::get_origin()));
    }

    //*************************************************************************
    /// Gets the end of the intrusive_avl_tree.
    /// Complexity: O(1).
    //*************************************************************************
    ETL_NODISCARD
    const_iterator cend() const ETL_NOEXCEPT
    {
      return end();
    }

    //*************************************************************************
    /// Gets the maximum of the intrusive_avl_tree.
    /// Complexity: O(log(N)).
    /// Returns `end` if tree is empty.
    //*************************************************************************
    ETL_NODISCARD
    iterator max() ETL_NOEXCEPT
    {
      return --end();
    }

    //*************************************************************************
    /// Gets the maximum of the intrusive_avl_tree.
    /// Complexity: O(log(N)).
    /// Returns `end` if tree is empty.
    //*************************************************************************
    ETL_NODISCARD
    const_iterator max() const ETL_NOEXCEPT
    {
      return --end();
    }

    //*************************************************************************
    /// Gets the minimum of the intrusive_avl_tree.
    /// Complexity: O(log(N)).
    /// Returns `end` if tree is empty.
    //*************************************************************************
    ETL_NODISCARD
    iterator min() ETL_NOEXCEPT
    {
      return begin();
    }

    //*************************************************************************
    /// Gets the minimum of the intrusive_avl_tree.
    /// Complexity: O(log(N)).
    /// Returns `end` if tree is empty.
    //*************************************************************************
    ETL_NODISCARD
    const_iterator min() const ETL_NOEXCEPT
    {
      return begin();
    }

    //*************************************************************************
    /// Returns an iterator pointing to the first item that compares as "not less".
    /// Complexity: O(log(N)) assuming O(1) for the comparator.
    /// The unary comparator should accept single `const value_type& value` argument,
    /// and return integer result:
    /// - `>0` (e.g. `+1`) if the find target is "greater" than the argument
    /// - `0` if the target is found
    /// - `<0` (e.g. `-1`) if the target is "smaller" than the argument
    /// - if throws then exception is propagated
    ///
    /// Result iterator will be `end()` if no such item exists.
    //*************************************************************************
    template <typename TCompare>
    iterator lower_bound(TCompare comp)
    {
      pointer ptr = base::template find_bound_impl<false, value_type>(base::get_root(), comp);
      return make_iterator(ptr, end());
    }
    template <typename TCompare>
    const_iterator lower_bound(TCompare comp) const
    {
      const_pointer ptr = base::template find_bound_impl<false, const value_type>(base::get_root(), comp);
      return make_iterator(ptr, end());
    }

    //*************************************************************************
    /// Returns an iterator pointing to the first item that compares as "greater".
    /// Complexity: O(log(N)) assuming O(1) for the comparator.
    /// The unary comparator should accept single `const value_type& value` argument,
    /// and return integer result:
    /// - `>0` (e.g. `+1`) if the find target is "greater" than the argument
    /// - `0` if the target is found
    /// - `<0` (e.g. `-1`) if the target is "smaller" than the argument
    /// - if throws then exception is propagated
    ///
    /// Result iterator will be `end()` if no such item exists.
    //*************************************************************************
    template <typename TCompare>
    iterator upper_bound(TCompare comp)
    {
      pointer ptr = base::template find_bound_impl<true, value_type>(base::get_root(), comp);
      return make_iterator(ptr, end());
    }
    template <typename TCompare>
    const_iterator upper_bound(TCompare comp) const
    {
      const_pointer ptr = base::template find_bound_impl<true, const value_type>(base::get_root(), comp);
      return make_iterator(ptr, end());
    }

    //*************************************************************************
    /// Gets root node (if any).
    /// Complexity: O(1).
    /// Normally is not needed unless advanced traversal is required.
    /// Result iterator will be valueless (`has_value() == false`) if tree is empty.
    //*************************************************************************
    ETL_NODISCARD
    iterator get_root() ETL_NOEXCEPT
    {
      return iterator(base::get_root());
    }

    //*************************************************************************
    /// Gets root node (if any).
    /// Complexity: O(1).
    /// Normally is not needed unless advanced traversal is required.
    /// Result iterator will be valueless (`has_value() == false`) if tree is empty.
    //*************************************************************************
    ETL_NODISCARD
    const_iterator get_root() const ETL_NOEXCEPT
    {
      return const_iterator(base::get_root());
    }

    //*************************************************************************
    /// Finds an item using given unary comparator.
    /// Complexity: O(log(N)) assuming O(1) for the comparator.
    ///
    /// The unary comparator should accept single `const value_type& value` argument,
    /// and return integer result:
    /// - `>0` (e.g. `+1`) if the find target is "greater" than the argument
    /// - `0` if the target is found
    /// - `<0` (e.g. `-1`) if the target is "smaller" than the argument
    /// - if throws then exception is propagated
    ///
    /// Result iterator will be `end()` if there is no matching item.
    //*************************************************************************
    template <typename TCompare>
    iterator find(TCompare comp)
    {
      pointer ptr = base::template find_impl<value_type>(base::get_root(), comp);
      return make_iterator(ptr, end());
    }

    //*************************************************************************
    /// Finds a constant item using given unary comparator.
    /// Complexity: O(log(N)) assuming O(1) for the comparator.
    ///
    /// The unary comparator should accept single `const value_type& value` argument,
    /// and return integer result:
    /// - `>0` (e.g. `+1`) if the find target is "greater" than the argument
    /// - `0` if the target is found
    /// - `<0` (e.g. `-1`) if the target is "smaller"
    /// - if throws then exception is propagated
    ///
    /// Result iterator will be `end()` if there is no matching item.
    //*************************************************************************
    template <typename TCompare>
    const_iterator find(TCompare comp) const
    {
      const_pointer ptr = base::template find_impl<const value_type>(base::get_root(), comp);
      return make_iterator(ptr, end());
    }

    //*************************************************************************
    /// Finds an existing item using given unary comparator, and if not found
    /// then invokes `factory` functor to insert a new item. The new item
    /// is inserted at the tree position where the search has stopped.
    /// Complexity: O(log(N)) assuming O(1) for the comparator and factory.
    /// Operation does NOT invalidate any already existing iterators,
    /// but depending on where the new item was linked to the tree
    /// the existing iterators may skip the recently linked item.
    ///
    /// The unary comparator should accept single `const value_type& value` argument,
    /// and return integer result:
    /// - `>0` (e.g. `+1`) if the find target is "greater" than the argument
    /// - `0` if the target is found
    /// - `<0` (e.g. `-1`) if the target is "smaller" than the argument
    /// - if throws then exception is propagated
    /// Hint: If result tree should contain "duplicates" then return non-zero
    /// result even for "equal" items, like e.g.:
    /// - `+1` to append a new item after existing duplicates
    /// - `-1` to prepend a new item before existing duplicates
    ///
    /// The factory functor should return address of a "new" value (castable to the `link_type*`).
    /// The returned value should not be already linked to any tree, otherwise throws
    /// `intrusive_avl_tree_value_is_already_linked` (if asserts or exceptions are enabled).
    /// If return address is `nullptr` then tree won't be modified,
    /// and final result iterator will be valueless.
    /// If factory throws then exception is propagated (without modifying the tree).
    ///
    /// Result contains a pair of:
    /// - iterator to found (or inserted) item (could be valueless, but never `end()`).
    /// - boolean indicating whether tree was modified (due to successful insertion).
    //*************************************************************************
    template <typename TCompare, typename TFactory>
    etl::pair<iterator, bool> find_or_insert(TCompare comp, TFactory factory)
    {
      const etl::pair<value_type*, bool> ptr_mod = base::template find_or_insert_impl<value_type>(comp, factory);
      return etl::make_pair(make_iterator(ptr_mod.first, iterator()), ptr_mod.second);
    }

    //*************************************************************************
    /// Erases the value at the specified position.
    /// Complexity: O(log(N)) - includes tree rebalancing.
    /// Operation invalidates any existing iterator to the same item,
    /// but it does NOT affect any other iterators.
    /// \param position iterator must originate from the same tree instance.
    /// Returns iterator to the next tree node (after the erased one).
    /// If asserts or exceptions are enabled, throws etl::intrusive_avl_tree_iterator_exception
    ///   if iterator doesn't reference a real item.
    /// Hint: use `clear` method if you need to erase all items - no tree rebalancing will be involved.
    //*************************************************************************
    iterator erase(iterator position)
    {
      ETL_ASSERT(position.has_value(), ETL_ERROR(intrusive_avl_tree_iterator_exception));
#if ETL_IS_DEBUG_BUILD
      // Iterator must originate from the same tree instance.
      // The check is still of the same O(log(N)) complexity.
      ETL_ASSERT(base::get_origin(position.p_value) == &base::get_origin(), ETL_ERROR(intrusive_avl_tree_iterator_exception));
#endif

      iterator next(position);
      ++next;

      base::erase_impl(position.p_value);

      return next;
    }

    //*************************************************************************
    /// Erases the value at the specified position.
    /// Complexity: O(log(N)) - includes tree rebalancing.
    /// Operation invalidates any existing iterator to the same item,
    /// but it does NOT affect any other iterators.
    /// \param position iterator must originate from the same tree instance.
    /// Returns iterator to the next tree node (after the erased one).
    /// If asserts or exceptions are enabled, throws etl::intrusive_avl_tree_iterator_exception
    ///   if iterator doesn't reference a real item.
    /// Hint: use `clear` method if you need to erase all items - no tree rebalancing will be involved.
    //*************************************************************************
    iterator erase(const_iterator position)
    {
      // It's safe to `const_cast` b/c we just need iterator to locate corresponding link.
      // Tree itself is not `const` anyway - so it's a legitimate operation, and it also matches
      // with similar `std::list::erase` or `etl::intrusive_list::erase` methods.
      return erase(iterator(const_cast<link_type*>(position.p_value)));
    }

    //*************************************************************************
    /// Visits all items of the tree in (ascending or descending) order.
    /// See https://en.wikipedia.org/wiki/Tree_traversal
    /// Complexity: O(N); no recursion.
    /// `is_reverse` determines in which order: `false` -> ascending, `true` -> descending.
    /// The `visitor` functor will be called with reference to the next in-order item.
    /// The item could be modified but so that it doesn't affect current ordering of the tree.
    /// NB! The visitor must not modify the tree during visitation.
    /// If visitor throws then exception is propagated.
    //*************************************************************************
    template <typename Visitor>
    void visit_in_order(const bool is_reverse, Visitor visitor)
    {
#if ETL_USING_CPP11
      base::visit_in_order_impl(         //
        &base::get_origin(), is_reverse, //
        [&visitor](link_type& link) { visitor(static_cast<reference>(link)); });
#else
      const CastingVisitor<reference, Visitor> casting_visitor(visitor);
      base::visit_in_order_impl(&base::get_origin(), is_reverse, casting_visitor);
#endif
    }

    //*************************************************************************
    /// Visits all items of the tree in (ascending or descending) order.
    /// See https://en.wikipedia.org/wiki/Tree_traversal
    /// Complexity: O(N); no recursion.
    /// `is_reverse` determines in which order: `false` -> ascending, `true` -> descending.
    /// The `visitor` functor will be called with const reference to the next in-order item.
    /// NB! The visitor must not modify the tree during visitation.
    /// If visitor throws then exception is propagated.
    //*************************************************************************
    template <typename Visitor>
    void visit_in_order(const bool is_reverse, Visitor visitor) const
    {
#if ETL_USING_CPP11
      base::visit_in_order_impl(         //
        &base::get_origin(), is_reverse, //
        [&visitor](const link_type& link) { visitor(static_cast<const_reference>(link)); });
#else
      const CastingVisitor<const_reference, Visitor> casting_visitor(visitor);
      base::visit_in_order_impl(&base::get_origin(), is_reverse, casting_visitor);
#endif
    }

    //*************************************************************************
    /// Visits all items of the tree using "post" ordering -
    /// child items first, and then "current" item (finishing with the root one).
    /// See https://en.wikipedia.org/wiki/Tree_traversal
    /// Complexity: O(N); no recursion.
    /// `is_reverse` determines in which order to visit children:
    /// - `false` -> "smaller" child first (if any), and then "bigger" one
    /// - `true` -> "bigger" child first (if any), and then "smaller" one
    /// The `visitor` functor will be called with reference to the next post-order item.
    /// The item could be modified but so that it doesn't affect current ordering of the tree.
    /// NB! The visitor must not modify the tree during visitation.
    /// If visitor throws then exception is propagated.
    //*************************************************************************
    template <typename Visitor>
    void visit_post_order(const bool is_reverse, Visitor visitor)
    {
#if ETL_USING_CPP11
      base::visit_post_order_impl(       //
        &base::get_origin(), is_reverse, //
        [&visitor](link_type& link) { visitor(static_cast<reference>(link)); });
#else
      const CastingVisitor<reference, Visitor> casting_visitor(visitor);
      base::visit_post_order_impl(&base::get_origin(), is_reverse, casting_visitor);
#endif
    }

    //*************************************************************************
    /// Visits all items of the tree using "post" ordering -
    /// child items first, and then "current" item (finishing with the root one).
    /// See https://en.wikipedia.org/wiki/Tree_traversal
    /// Complexity: O(N); no recursion.
    /// `is_reverse` determines in which order to visit children:
    /// - `false` -> "smaller" child first (if any), and then "bigger" one
    /// - `true` -> "bigger" child first (if any), and then "smaller" one
    /// The `visitor` functor will be called with const reference to the next post-order item.
    /// NB! The visitor must not modify the tree during visitation.
    /// If visitor throws then exception is propagated.
    //*************************************************************************
    template <typename Visitor>
    void visit_post_order(const bool is_reverse, Visitor visitor) const
    {
#if ETL_USING_CPP11
      base::visit_post_order_impl(       //
        &base::get_origin(), is_reverse, //
        [&visitor](const link_type& link) { visitor(static_cast<const_reference>(link)); });
#else
      const CastingVisitor<const_reference, Visitor> casting_visitor(visitor);
      base::visit_post_order_impl(&base::get_origin(), is_reverse, casting_visitor);
#endif
    }

    //*************************************************************************
    /// Visits all items of the tree using "pre" ordering -
    /// "current" item first (starting from the root), and then children.
    /// See https://en.wikipedia.org/wiki/Tree_traversal
    /// Complexity: O(N); no recursion.
    /// `is_reverse` determines in which order to visit children:
    /// - `false` -> "smaller" child first (if any), and then "bigger" one
    /// - `true` -> "bigger" child first (if any), and then "smaller" one
    /// The `visitor` functor will be called with reference to the next pre-order item.
    /// The item could be modified but so that it doesn't affect current ordering of the tree.
    /// NB! The visitor must not modify the tree during visitation.
    /// If visitor throws then exception is propagated.
    //*************************************************************************
    template <typename Visitor>
    void visit_pre_order(const bool is_reverse, Visitor visitor)
    {
#if ETL_USING_CPP11
      base::visit_pre_order_impl(        //
        &base::get_origin(), is_reverse, //
        [&visitor](link_type& link) { visitor(static_cast<reference>(link)); });
#else
      const CastingVisitor<reference, Visitor> casting_visitor(visitor);
      base::visit_pre_order_impl(&base::get_origin(), is_reverse, casting_visitor);
#endif
    }

    //*************************************************************************
    /// Visits all items of the tree using "pre" ordering -
    /// "current" item first (starting from the root), and then children.
    /// See https://en.wikipedia.org/wiki/Tree_traversal
    /// Complexity: O(N); no recursion.
    /// `is_reverse` determines in which order to visit children:
    /// - `false` -> "smaller" child first (if any), and then "bigger" one
    /// - `true` -> "bigger" child first (if any), and then "smaller" one
    /// The `visitor` functor will be called with const reference to the next pre-order item.
    /// NB! The visitor must not modify the tree during visitation.
    /// If visitor throws then exception is propagated.
    //*************************************************************************
    template <typename Visitor>
    void visit_pre_order(const bool is_reverse, Visitor visitor) const
    {
#if ETL_USING_CPP11
      base::visit_pre_order_impl(        //
        &base::get_origin(), is_reverse, //
        [&visitor](const link_type& link) { visitor(static_cast<const_reference>(link)); });
#else
      const CastingVisitor<const_reference, Visitor> casting_visitor(visitor);
      base::visit_pre_order_impl(&base::get_origin(), is_reverse, casting_visitor);
#endif
    }

  private:

#if ETL_USING_CPP11
#else
    // Disable copy construction and assignment.
    intrusive_avl_tree(const intrusive_avl_tree&);
    intrusive_avl_tree& operator=(const intrusive_avl_tree& rhs);

    template <typename T, typename Visitor>
    struct CastingVisitor
    {
      Visitor& visitor;
      ETL_EXPLICIT CastingVisitor(Visitor& visitor)
        : visitor(visitor)
      {
      }

      template <typename TLink>
      void operator()(TLink& link) const
      {
        visitor(static_cast<T>(link));
      }
    };
#endif

    template <typename TIterator, typename Pointer>
    ETL_NODISCARD
    static TIterator make_iterator(Pointer const ptr, const TIterator end)
    {
      return (ETL_NULLPTR != ptr) ? TIterator(ptr) : end;
    }

  }; // intrusive_avl_tree

} // namespace etl

#endif
