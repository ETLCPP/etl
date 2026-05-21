---
title: "Intrusive Links"
---

{{< callout type="info">}}
  Header: `intrusive_links.h`  
{{< /callout >}}

A set of link structures designed to be used within containers such as `etl::intrusive_list`.  
They are parameterised by an id that allows them to be multiply inherited from when creating objects that must exist in more than one intrusive container.  

There are link and unlink functions supplied to manage connections between links. The functions will accept any permutation of pointer and reference parameters, though reference parameters offer the best performance.  
For bidirectional links, unlinking a node will automatically adjust the links of the surrounding nodes at the same level.  

## Forward link

```cpp
template <size_t ID_>
struct forward_link
```

### Template parameters
`const size_t ID_` A unique id for this link level.

### Public constants
```cpp
enum ID
``` 
**Description**  
The unique id for this link level.

### Public variables
```cpp
forward_link<ID>* etl_next
```
**Description**  
A pointer to the next forward link at this level.

### Public functions
```cpp
clear()
```
**Description**  
Clears the pointers to nullptr.

### Link functions

```cpp
template <typename TLink>
struct is_forward_link

template <typename TLink>
inline constexpr bool is_forward_link_v;
```
Checks if TLink is an `etl::forward_link`.
From: C++17

---

```cpp
template <typename TLink>
void link(TLink& lhs, TLink& rhs)

template <typename TLink>
void link(TLink* lhs, TLink* rhs)

template <typename TLink>
void link(TLink& lhs, TLink* rhs)

template <typename TLink>
void link(TLink* lhs, TLink& rhs)
```
**Description**  
Make `rhs` the next element after `lhs`.

---

```cpp
template <typename TLink>
void link_splice(TLink& lhs, TLink& rhs)

template <typename TLink>
void link_splice(TLink* lhs, TLink* rhs)

template <typename TLink>
void link_splice(TLink& lhs, TLink* rhs)

template <typename TLink>
void link_splice(TLink* lhs, TLink& rhs)
```
**Description**  
Insert `rhs` as the element after `lhs`.  
The original element after `lhs` will be linked after `rhs`.

---

```cpp
template <typename TLink>
void link_splice(TLink& lhs, TLink& first, TLink& last)

template <typename TLink>
void link_splice(TLink* lhs, TLink& first, TLink& last)
```
**Description**  
The elements in the range [`first`, `last`] will be inserted after `lhs`.

---

```cpp
template <typename TLink>
TLink* unlink_after(TLink& node)
```
**Description**  
Unlinks the element after `node`.

**Return**  
A pointer to the removed node or `nullptr`.

---

```cpp
template <typename TLink>
TLink* unlink_after(TLink& before, TLink& last)
```
**Description**  
Unlinks the the range (`before`, `last`].

**Return**  
A pointer to the first element in the removed range.

---

```cpp
template <typename TLink>
bool is_linked(TLink& node)

template <typename TLink>
bool is_linked(TLink* node)
```
**Return**  
`true` if `node` is linked to another.

---

```cpp
template <typename TLink>
void link_clear(TLink& node)

template <typename TLink>
void link_clear(TLink* node)
```
**Description**  
Clears the links for `node`.

---

```cpp
template <typename TLink>
void link_clear_range(TLink& start)

template <typename TLink>
void link_clear_range(TLink* start)
```
**Description**  
Clears the links for all nodes linked to `start`.

---

```cpp
template <typename TLink, typename... TLinks>
TLink* create_linked_list(TLink& first, TLinks&... links)
```
**Description**  
Create a linked list from a number of `forward_link` nodes.

---

```cpp
template <typename TLink>
void detach_linked_list(TLink& first)

template <typename TLink>
void detach_linked_list(TLink* first)
```
**Description**  
A synonym for `link_clear_range`.

## Bidirectional link
```cpp
template <const size_t ID_>
struct bidirectional_link
```

### Template parameters
```cpp
const size_t ID_
```
**Description**  
The unique id for this link level.

### Public constants
```cpp
enum ID
```
**Description**  
The unique id for this link level.

### Public variables
```cpp
bidirectional_link<ID>* etl_previous
```
**Description**  
A pointer to the previous bidirectional link at this level.

```cpp
bidirectional_link<ID>* etl_next
```
**Description**  
A pointer to the next bidirectional link at this level.

### Public functions
```cpp
clear()
```
**Description**  
Clears the link pointers to `nullptr`.

---

```cpp
reverse()
```
**Description**  
Reverses the order of the elements.

### Link functions

```cpp
void etl::link<type>(lhs, rhs)
```
**Description**  
Link lhs to rhs.

---

```cpp
void etl::link_splice<type>(lhs, rhs) 
```
Insert `rhs` as the element after `lhs`.  
The original element after `lhs` will be linked after `rhs`.

```cpp
void etl::link_splice<type>(lhs, first, last)
```
**Description**  
The elements in the range [`first`, `last`] will be inserted after `lhs`.

---

**unlink**  

```cpp
void etl::unlink(node)
```
**Description**  
Unlink the specified node. Elements either side are linked to each other.

```cpp
void etl::unlink(first, last)
```
**Description**  
Unlinks the range [`first`, `last`].  
The range [`first`, `last`] remain linked to each other.

---

```cpp
template <typename T>
etl::is_bidirectional_link
```
**Description**  
Tests if type `T` is an `etl::bidirectional_link`.

---

```cpp
template <typename T>
etl::is_bidirectional_link_v
```
**Description**  
Tests if type `T` is an `etl::bidirectional_link`.
From C++17

## Tree link
```cpp
template <const size_t ID_>
struct tree_link
```

### Template parameters
```cpp
const size_t ID_
```
**Description**  
A unique id for this link level.

### Public constants
```cpp
enum ID
```
**Description**  
The unique id for this link level.

### Public variables
```cpp
tree_link<ID>* etl_parent
```
**Description**  
A pointer to the parent tree link at this level.

```cpp
tree_link<ID>* etl_left
```
**Description**  
A pointer to the left tree link at this level.

```cpp
tree_link<ID>* etl_right
```
**Description**  
A pointer to the right tree link at this level.

### Public functions
```cpp
clear()
```
**Description**  
Clears the pointers to `nullptr`.

### Link Functions
```cpp
void etl::link_left<type>(parent, leaf)
```
**Description**  
Links leaf to the left of parent.

---

```cpp
void etl::link_right<type>(parent, leaf)
```
**Description**  
Links leaf to the right of parent.

---

```cpp
void etl::link_rotate_left<type>(parent, leaf)  
```
**Description**  
Rotates the link left making leaf the new parent.

---

```cpp
void etl::link_rotate_right<type>(parent, leaf) 
```
**Description**  
Rotates the link right making leaf the new parent.

---

```cpp
void etl::link_rotate<type>(parent, leaf) 
```
**Description**  
Rotates the link left or right making leaf the new parent.  
Chooses left or right rotate depending on the leaf connection.

---

```cpp
template <typename T>
etl::is_tree_link
```
Tests if type `T` is an `etl::tree_link`.  
From: `20.37.0`

---

```cpp
template <typename T>
etl::is_tree_link_v
```
Tests if type `T` is an `etl::tree_link`.  
From: `20.37.0`
From: C++17

**Example 1**  
A simple two level intrusive list.

```cpp
// The link levels
typedef etl::bidirectional_link<0> level0_t;
typedef etl::bidirectional_link<1> level1_t;

constexpr bool is_bdl = etl::is_bidirectional_link_v<level0_t>;

// The item stored in the lists
struct item : public level0_t, public level1_t
{
  item(int value)
    :: value(value)
  {
  }

  int value;
};

item data0(0);
item data1(1);
item data2(2);
item data3(3);

etl::intrusive_list<item, level0_t> level0_list;
etl::intrusive_list<item, level1_t> level1_list;

// Add items to level0 list
level0_list.push_back(data0);
level0_list.push_back(data1);
level0_list.push_back(data2);

// Add items to level1 list
level1_list.push_back(data3);
level1_list.push_back(data2);
level1_list.push_back(data1);
```

**Example 2**  
Manual list manipulation.

```cpp
typedef etl::bidirectional_link<0> level0_t;
typedef etl::bidirectional_link<1> level1_t;

// The item stored in the lists
struct item : public level0_t, public level1_t
{
  item(int value)
    :: value(value)
  {
  }

  int value;
};

item data0(0);
item data1(1);
item data2(2);
item data3(3);

// Set the first and last nodes links to nullptr.
data0.level0_t::clear();
data3.level0_t::clear();

// Make the links.
etl::link<level0_t>(data0, data1);
etl::link<level0_t>(data1, data2);
etl::link<level0_t>(data2, data3); // Level 0 = data0, data1, data2, data3

// Set the first and last nodes links to nullptr.
data2.level1_t::clear();
data1.level1_t::clear();

// Make the links.
etl::link<level1_t>(data2, data3);
etl::link<level1_t>(data3, data0);
etl::link<level1_t>(data0, data1); // Level 1 = data2, data3, data0, data1

// Disconnect a node.
etl::unlink<level1_t>(data3); // Level 1 = data2, data0, data1
TLink*TLink*
```