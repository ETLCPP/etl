---
title: "multi_range"
---

A lightweight, chainable range system that simulates nested loops over multiple ranges, with customisable stepping and end conditions.  

`etl::multi_range<T>` is a composable range-iteration helper that can chain multiple ranges to produce nested-loop style iteration. It lets you build “outer/inner” ranges and iterate across all combinations, while customizing stepping and termination logic.  

Each range is defined as an `etl::multi_range` which may be linked to create nested multiple ranges.  

```cpp
template <typename T>
class multi_range
```

All multi_range<T> classes derive from `imulti_range`.    

By default, each call to next() will invoke either the forward_step or reverse_step types.  
If the type is an arithmetic or pointer type then the default stepper will be automatically selected, dependent on the first and last parameters. If not, the default will be `forward_step`.  

If a custom stepper for the type is required, then it may be defined in the constructor.  

By default, each call to `completed()` will invoke `not_equal_compare` against the terminating range value. If a custom incrementor for the type is required, then it may be defined in the constructor.  

## Types

`value_type`  
The type of the range.  

`const_reference`  
The const reference type of the range.  

---

`step_type`  
Pure abstract base.  
Inherit from this to define custom step functionality.  

`forward_step`  
A pre-defined step type that calls the type's `operator++()`.  
This is the default step type if the type is arithmetic or random access iterator and the range is ascending, or the type is not an arithmetic or pointer.  

`forward_step_by`  
A pre-defined step type that calls the type's `operator+=()`.  
The constructor requires the step value to add.  

`reverse_step`  
A pre-defined step type that calls the type's `operator--()`.  
This is the default step type if the type is arithmetic or random access iterator and the range is descending.  

`reverse_step_by`  
A pre-defined step type that calls the type's `operator-=()`.  
The constructor requires the step value to subtract.  

---

`compare_type`  
Pure abstract base.  
Inherit from this to define custom compare functionality.  

`not_equal_compare`  
A pre-defined step type that calls etl::not_equal_to.  
This is the default compare type.  

`less_than_compare`  
A pre-defined step type that calls `etl::less`.  

`greater_than_compare`  
A pre-defined step type that calls `etl::greater`.  

## Constructor

```cpp
multi_range(value_type start, 
            value_type last)
```            
**Description**  
Initialises the loop where start is the initial value and last is the terminate value.

---

```cpp
multi_range(value_type first,
            value_type last,
            step_type& stepper)
```
**Description**  
Initialises the loop where start is the initial value and last is the terminate value.  
stepper will be called to increment the current range value.  

---

```cpp
multi_range(value_type    first,
            value_type    last,
            compare_type& compare)
```
**Description**  
Initialises the loop where start is the initial value and last is the terminate value.  
compare will be called to compare with the  terminating value.  

---

```cpp
multi_range(value_type    first, 
            value_type    last, 
            step_type&    stepper,
            compare_type& compare)
```
**Description**  
Initialises the loop where start is the initial value and last is the terminate value.  
stepper will be called to increment the current range value.  
compare will be called to compare with the  terminate value.  

## Modifiers

The nested ranges may be constructed in a variety of ways.  

```cpp
imulti_range& append(imulti_range& inner_ranges)
```
**Description**  
Appends inner loop(s) to the current range(s).
If the current range, **A**,  is linked to ranges **B** & **C** in the order **A** -> **B** -> **C** and the new linked ranges are **D** -> **E**,
`A.append(D)` will result in **A** -> **B** -> **C** -> **D** -> **E**.

---

```cpp
imulti_range& insert(imulti_range& inner_ranges)
```
**Description**  
Inserts inner range(s) after the current range.
If the current range, **A**,  is linked to ranges **B** & **C** in the order **A** -> **B** -> **C** and the new linked ranges are **D** -> **E**,
`A.insert(D)` will result in **A** -> **D** -> **E** -> **B** -> **C**.

---

```cpp
void detach()
```
**Description**  
Unlinks this range from its inner ranges.

---

```cpp
void detach_all()
```
**Description**  
Unlinks this range and all its inner ranges from each other.

## Access

```cpp
const_reference value() const;
```
**Description**  
Gets a const reference to the current value

---

```cpp
const_reference begin() const;
```
**Description**  
Gets a const reference to the first value

---

```cpp
const_reference end() const;
```
**Description**  
Gets a const reference to the terminate value

---

```cpp
bool completed() const
```
**Description**  
Returns true if the range, and all of its inner ranges, have completed.

---

```cpp
size_t number_of_ranges() const
```
**Description**  
Returns the number of linked ranges, starting from this.  
For linked ranges **A** -> **B** -> **C** -> **D** -> **E**, `B.number_of_ranges()` returns `4`.

---

```cpp
size_t number_of_iterations()
```
**Description**  
Returns the total number of iterations for all linked ranges, starting from this.  
For linked ranges **A** -> **B** -> **C** -> **D** -> **E**, and a range size of `5` for each one,
`B.number_of_iterations()` returns `625` (`5 * 5 * 5 * 5`).  

This function is not const as it must simulate the running of all of the nested ranges.

## Operations

```cpp
void start()
```
**Description**  
Resets the range, and all its linked inner ranges to the starting conditions, with each range reset to its begin value.

---

```cpp
void next()
```
**Description**  
Moves to the next iteration of the nested range.

---

## Example

```cpp
using Iterator = std::forward_list<std::string>::const_iterator;

// Less Than compare type
using LessThanCompare = etl::multi_range<const short*>::less_than_compare;

// Decrementing step type
using DecrementPtr = etl::multi_range<const short*>::reverse_step;

using Outer  = etl::multi_range<int>;          // Incrementing int
using Middle = etl::multi_range<const short*>; // Decrementing const short*
using Inner  = etl::multi_range<Iterator>;     // Incrementing const_iterator

const short data[4] = { 0, 1, 2, 3 };

std::forward_list<std::string> strings =
{
  "zero", "one", "two", "three"
};

LessThanCompare lessThan;
DecrementPtr    decrementPtr;

// Setup the loops.
Outer  outer(0, 4, lessThan);
Middle middle(data + 3, data - 1, decrementPtr);
Inner  inner(strings.begin(), strings.end());

outer.append(middle).append(inner);

size_t n_outer_loops  = outer.number_of_loops();  // == 3
size_t n_middle_loops = middle.number_of_loops(); // == 2
size_t n_inner_loops  = inner.number_of_loops();  // == 1

size_t n_outer_iterations  = outer.number_of_iterations();  // == 64
size_t n_middle_iterations = middle.number_of_iterations(); // == 16
size_t n_inner_iterations  = inner.number_of_iterations();  // == 4

// Create const references to the loop values.
const int&      value_outer  = outer.value();
const short*&   value_middle = middle.value();
const Iterator& value_inner  = inner.value();

// Iterate through the nested loops.
int i = 0;

for (outer.start(); !outer.completed(); outer.next())
{
  std::cout << "Iteration " << i++ << " : "
            << "Outer  = " << value_outer << " : "
            << "Middle = " << *value_middle << " : "
            << "Inner  = " << *value_inner
            << "\n"; 
}

// Just iterate through the two nested loops.
i = 0;

for (middle.start(); !middle.completed(); middle.next())
{
  std::cout << "Iteration " << i++ << " : "
            << "Middle = " << *value_middle << " : "
            << "Inner  = " << *value_inner
            << "\n"; 
}
```
