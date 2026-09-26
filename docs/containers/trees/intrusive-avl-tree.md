---
title: "intrusive_avl_tree"
weight: 1
---

{{< callout type="info">}}
  Header: `intrusive_avl_tree.h`  
  Similar to: `std::set` / `std::map` (ordering only, no ownership)
{{< /callout >}}

An intrusive, self-balancing AVL tree.

```cpp
template <typename TValue, size_t ID_ = 0>
etl::intrusive_avl_tree
```

`TValue` is the type that contains the actual values. It must derive from
`etl::intrusive_avl_tree_base<ID_>::link_type`, exposed as the tree's own
`link_type`. Unlike `etl::intrusive_list`/`etl::intrusive_forward_list`,
which are templated directly on a link type and expect the node to derive
from it publicly, `intrusive_avl_tree` is templated only on `TValue` and an
`ID_`. Its `link_type` is a deliberately opaque wrapper around
`etl::tree_link<ID_>` that exposes almost nothing as public (or even
protected) API, by design, so that inheriting from it imposes as little
"intrusive"-ness as possible, avoids name conflicts, and hides implementation
details.

As with the other intrusive containers, an object may inherit multiple
`link_type`s with different `ID_` values to be a member of several trees (or
lists) at once:

```cpp
typedef etl::intrusive_avl_tree_base<0>::link_type FirstLink;
typedef etl::intrusive_avl_tree_base<1>::link_type SecondLink;

struct Item : public FirstLink, public SecondLink
{
  int value;
};

etl::intrusive_avl_tree<Item, 0> tree0;
etl::intrusive_avl_tree<Item, 1> tree1;
```

See also:  
[Intrusive Links]({{% relref "../../utilities/intrusive-links.md" %}})  
[intrusive_list]({{% relref "../lists/intrusive-list.md" %}})  
[intrusive_forward_list]({{% relref "../lists/intrusive-forward-list.md" %}})

There is no class-level comparator template parameter. Every ordering
operation instead takes its own comparator functor at the call site.
Lookup functions (`find`, `lower_bound`, `upper_bound`, `find_or_insert`)
take a unary comparator accepting a single `const value_type&` and returning
an `int`: `>0` if the search target is "greater" than the argument, `0` if
found, `<0` if "smaller". The range constructor takes a binary comparator
`int(const value_type& lhs, const value_type& rhs)`; `etl::compare<value_type>::cmp`
can be used directly if `value_type` defines `<`.

## Member types

```cpp
link_type           (nested, inherited from etl::intrusive_avl_tree_base<ID_>)
value_type          TValue             
pointer             value_type*        
const_pointer const value_type*  
reference           value_type&        
const_reference     const value_type&  
size_type           size_t             
rvalue_reference    value_type&&        // C++11
```

## The link type

`TValue` must derive from `link_type`. It has its own set of special member
functions that a node author relies on.

```cpp
link_type()
```
**Description**  
Default constructor. Leaves the item unlinked.

---

```cpp
link_type(link_type&& other)
```
**Description**  
C++11. Constructs a new item by moving `other` into `this`. After
construction, `this` replaces `other` in the same tree position that
`other` occupied, so no tree balancing is needed. `other` becomes unlinked.

**Complexity**  
{{< complexity "constant" "1" >}}

---

```cpp
link_type& operator=(link_type&& other)
```
**Description**  
C++11. Assigns `other` by moving it into `this`. Does nothing on
self-assignment. After assignment, `this` replaces `other` in the same tree
position that `other` occupied; `other` becomes unlinked. `this` might end
up in a different tree than it started in.

**Complexity**  
{{< complexity "logarithmic" "log N" >}} if `this` is already linked to a tree (it has to be erased first, with rebalancing), or  
{{< complexity "constant" "1" >}} if `this` is not linked, where `N` is number of items in the tree.

---

```cpp
link_type(const link_type&)            = delete;
link_type& operator=(const link_type&) = delete;
```
**Description**  
C++11. Copy construction and assignment are disabled.

---

```cpp
~link_type()
```
**Description**  
Might rotate the tree as necessary to keep it balanced. The tree is not the
real owner of its nodes' memory - the node's actual owner may destroy it at
any time, with or without a prior explicit `erase`. If the link is still
linked to a tree when it's destroyed, the destructor unlinks it
automatically; otherwise the node's former parent and children would keep
dangling pointers to it, breaking the tree and leading to undefined
behaviour. This is what makes it safe to embed a tree node in an object
whose lifetime is managed elsewhere.

**Complexity**  
{{< complexity "logarithmic" "log N" >}}, where `N` is number of items in the tree.

## Constructors

```cpp
etl::intrusive_avl_tree<TValue, ID_>()
```
**Description**  
Default constructor. Constructs an empty tree.

---

```cpp
template <typename TIterator, typename TBinaryCompare>
etl::intrusive_avl_tree<TValue, ID_>(TIterator first, TIterator last, TBinaryCompare binary_comp)
```
**Description**  
Constructs the tree from the range [`first`, `last`) of items, using
`binary_comp` to order them. All items in the range must be unlinked
initially; if any item is already linked to some other tree, throws
`etl::intrusive_avl_tree_value_is_already_linked`, and any items already
inserted are unlinked again - it's all or nothing.  
If duplicates should be kept, `binary_comp` should return a non-zero result
even for "equal" items - `<0` prepends a duplicate, `>0` appends one.
Returning `0` for equal items keeps only the first of each run of duplicates
in the range; the rest are left unlinked. `etl::compare<value_type>::cmp`
can be used directly as `binary_comp`.

**Complexity**  
{{< complexity "linearithmic" "N log N" >}}, where `N` is number of items in the range.

---

```cpp
intrusive_avl_tree(intrusive_avl_tree&&) = default;
```
**Description**  
C++11. Move constructor.

**Complexity**  
{{< complexity "constant" "1" >}}

---

```cpp
intrusive_avl_tree(const intrusive_avl_tree&) = delete;
```
**Description**  
C++11. Copy construction is disabled.

---

```cpp
~intrusive_avl_tree()
```
**Description**  
Every remaining node is unlinked; none of the value objects are destroyed,
since the tree never owns their memory.

**Complexity**  
{{< complexity "linear" "N" >}}, where `N` is number of items in the tree.

## Assignment

```cpp
intrusive_avl_tree& operator=(intrusive_avl_tree&&) = default;
```
**Description**  
C++11. Move assignment. Does nothing on self-assignment. All of `this`
tree's former items have to be unlinked.

**Complexity**  
{{< complexity "linear" "N" >}}, where `N` is the size of `this` tree before assignment.

---

```cpp
intrusive_avl_tree& operator=(const intrusive_avl_tree&) = delete;
```
**Description**  
C++11. Copy assignment is disabled.

## Capacity

```cpp
bool empty() const
```
**Description**  
Returns `true` if the tree has no linked items.

**Complexity**  
{{< complexity "constant" "1" >}}

---

```cpp
size_t size() const
```
**Description**  
Returns the number of linked items.

**Complexity**  
{{< complexity "constant" "1" >}}

## Iterators

Only bidirectional `iterator`/`const_iterator` are provided; there is no
`reverse_iterator` (use `visit_in_order` with `is_reverse = true` instead).
An iterator may be in one of three logical states: it references a real
node (`has_value() == true`), it is the terminal sentinel (`it == end()`),
or it is "valueless" (`!has_value() && it != end()` - e.g. a
default-constructed iterator, or the result of `get_parent()`/`get_child()`
when there is no such node).

```cpp
iterator begin()
const_iterator begin() const
const_iterator cbegin() const
```
**Description**  
Returns an iterator to the smallest item. Returns `end()` if the tree is
empty.

**Complexity**  
{{< complexity "logarithmic" "log N" >}}, where `N` is number of items in the tree.

---

```cpp
iterator end()
const_iterator end() const
const_iterator cend() const
```
**Description**  
Returns an iterator to the terminal sentinel.

**Complexity**  
{{< complexity "constant" "1" >}}

---

```cpp
iterator min()
const_iterator min() const
```
**Description**  
Equivalent to `begin()`.

**Complexity**  
{{< complexity "logarithmic" "log N" >}}, where `N` is number of items in the tree.

---

```cpp
iterator max()
const_iterator max() const
```
**Description**  
Equivalent to `--end()`.

**Complexity**  
{{< complexity "logarithmic" "log N" >}}, where `N` is number of items in the tree.

---

```cpp
iterator& operator++()
iterator operator++(int)
```
**Description**  
Advances the iterator to the next ("greater") item.

**Complexity**  
{{< complexity "constant" "1" >}} amortized,  
{{< complexity "logarithmic" "log N" >}} worst-case, where `N` is number of items in the tree.

---

```cpp
iterator& operator--()
iterator operator--(int)
```
**Description**  
Moves the iterator to the previous ("smaller") item.

**Complexity**  
{{< complexity "constant" "1" >}} amortized,  
{{< complexity "logarithmic" "log N" >}} worst-case, where `N` is number of items in the tree.

---

```cpp
reference operator*() const
pointer operator->() const
pointer get() const
```
**Description**  
Dereferences an iterator that has a value. `operator*`/`operator->` throw
`etl::intrusive_avl_tree_iterator_exception` if `has_value()` is `false`.
`get()` returns `nullptr` instead of throwing.

---

```cpp
bool has_value() const
explicit operator bool() const
```
**Description**  
Returns `true` if the iterator references a real node.

## Advanced traversal

These are normally only needed for advanced traversal.

```cpp
iterator get_root()
const_iterator get_root() const
```
**Description**  
Returns an iterator to the root node. Valueless (`has_value() == false`) if
the tree is empty.

**Complexity**  
{{< complexity "constant" "1" >}}

---

```cpp
int_fast8_t get_balance_factor() const
```
**Description**  
Returns the node's AVL balance factor: `-1`, `0` or `+1`.

**Complexity**  
{{< complexity "constant" "1" >}}

---

```cpp
iterator get_parent() const
```
**Description**  
Returns an iterator to the node's parent. Valueless if there is no parent.

**Complexity**  
{{< complexity "constant" "1" >}}

---

```cpp
iterator get_child(bool is_right) const
```
**Description**  
Returns an iterator to the node's left (`is_right == false`) or right
(`is_right == true`) child. Valueless if there is no such child.

**Complexity**  
{{< complexity "constant" "1" >}}

## Lookup

```cpp
template <typename TCompare>
iterator find(TCompare comp)
template <typename TCompare>
const_iterator find(TCompare comp) const
```
**Description**  
Finds an item using the unary comparator `comp`. Returns `end()` if there
is no matching item.

**Complexity**  
{{< complexity "logarithmic" "log N" >}}, where `N` is number of items in the tree,
and assuming `comp` is {{< complexity "constant" "1" >}}.

---

```cpp
template <typename TCompare>
iterator lower_bound(TCompare comp)
template <typename TCompare>
const_iterator lower_bound(TCompare comp) const
```
**Description**  
Returns an iterator to the first item that compares as "not less" than the
target of `comp`. Returns `end()` if no such item exists.

**Complexity**  
{{< complexity "logarithmic" "log N" >}}, where `N` is number of items in the tree,
and assuming `comp` is {{< complexity "constant" "1" >}}.

---

```cpp
template <typename TCompare>
iterator upper_bound(TCompare comp)
template <typename TCompare>
const_iterator upper_bound(TCompare comp) const
```
**Description**  
Returns an iterator to the first item that compares as "greater" than the
target of `comp`. Returns `end()` if no such item exists.

**Complexity**  
{{< complexity "logarithmic" "log N" >}}, where `N` is number of items in the tree,
and assuming `comp` is {{< complexity "constant" "1" >}}.

## Modifiers

```cpp
template <typename TCompare, typename TFactory>
etl::pair<iterator, bool> find_or_insert(TCompare comp, TFactory factory)
```
**Description**  
Finds an existing item using the unary comparator `comp`; if not found,
calls `factory()` (no arguments) and inserts the item it returns at the
position where the search stopped. This operation never invalidates
existing iterators, though depending on where the new item was linked,
existing iterators may skip over it.  
`factory` must return the address of the new value (castable to
`link_type*`). If it returns `nullptr`, the tree is left unmodified and the
result iterator is valueless. If the returned value is already linked to
any tree, throws `etl::intrusive_avl_tree_value_is_already_linked`.  
Returns a pair of: an iterator to the found or inserted item (never `end()`,
but may be valueless), and a `bool` that is `true` if the tree was modified.  
If duplicates are wanted, `comp` should return a non-zero result even for
"equal" items - `+1` appends after existing duplicates, `-1` prepends
before them.

**Complexity**  
{{< complexity "logarithmic" "log N" >}}, where `N` is number of items in the tree,
and assuming `comp` and `factory` are {{< complexity "constant" "1" >}}.

---

```cpp
iterator erase(iterator position)
iterator erase(const_iterator position)
```
**Description**  
Erases the item at `position`, including tree rebalancing. `position` must
reference a real item (asserts/throws
`etl::intrusive_avl_tree_iterator_exception` otherwise) and must originate
from this tree instance. Invalidates any existing iterator to the erased
item, but no others. Returns an iterator to the next node.  
Use `clear()` instead if every item needs to be erased - no rebalancing is
involved.

**Complexity**  
{{< complexity "logarithmic" "log N" >}}, where `N` is number of items in the tree.

---

```cpp
void clear()
```
**Description**  
Unlinks every item, leaving the tree empty. Invalidates all existing
iterators. This is cheaper than calling `erase` on every item, which would
cost {{< complexity "linearithmic" "N log N" >}} once intermediate
rebalancing is taken into account.

**Complexity**  
{{< complexity "linear" "N" >}}, where `N` is number of items in the tree.

---

```cpp
void swap(intrusive_avl_tree& other)
friend void swap(intrusive_avl_tree& lhs, intrusive_avl_tree& rhs)
```
**Description**  
Swaps the contents of two trees. Does nothing on self-swap.

**Complexity**  
{{< complexity "constant" "1" >}}

## Traversal

Each of these visits every item exactly once, without recursion, and has a
`const` overload. The visitor must not modify the tree during visitation.

```cpp
template <typename Visitor>
void visit_in_order(bool is_reverse, Visitor visitor)
```
**Description**  
Visits every item in sorted order. `is_reverse` selects ascending (`false`)
or descending (`true`) order.

**Complexity**  
{{< complexity "linear" "N" >}}, where `N` is number of items in the tree.

---

```cpp
template <typename Visitor>
void visit_pre_order(bool is_reverse, Visitor visitor)
```
**Description**  
Visits every item in pre-order (parent before children).

**Complexity**  
{{< complexity "linear" "N" >}}, where `N` is number of items in the tree.

---

```cpp
template <typename Visitor>
void visit_post_order(bool is_reverse, Visitor visitor)
```
**Description**  
Visits every item in post-order (children before parent).

**Complexity**  
{{< complexity "linear" "N" >}}, where `N` is number of items in the tree.

## Exceptions

```cpp
intrusive_avl_tree_exception                    // base exception
intrusive_avl_tree_iterator_exception            // invalid iterator use
intrusive_avl_tree_value_is_already_linked       // item already linked to a tree
```
