# ETL C++17 Ranges Implementation

## Overview

The Embedded Template Library provides a C++17-compatible implementation of ranges, inspired by the C++20 ranges library. This implementation enables range-based algorithms and views for embedded and resource-constrained environments where full C++20 support may not be available.

## Features

- **Ranges**: Provides range types and iterator wrappers for composing operations over sequences.
- **Views**: Includes lightweight, composable views such as `filter_view`, `transform_view`, and `subrange`.
- **Algorithms**: Supports range-based algorithms compatible with ETL containers and standard containers.
- **Compatibility**: Designed for C++17, with minimal dependencies and no reliance on C++20 features.

## Getting Started

Include the main header in your project:

```cpp
#include <etl/ranges.h>
```

### Example Usage

#### Using Ranges

```cpp
#include <etl/print.h>
#include <etl/ranges.h>
#include <etl/vector.h>

...

  etl::vector<int, 10> data = {6, 1, 3, 3, 2};
  etl::ranges::sort(data);
  etl::ranges::for_each(data, [](const int& i){etl::print(" {}", i);});
```

Output:
```text
 1 2 3 3 6
```

#### Using Views

```cpp
#include <etl/print.h>
#include <etl/ranges.h>
#include <etl/vector.h>

...

  etl::vector<int, 10> data = {1, 2, 3, 4, 5};
  auto even = [](int v) { return v % 2 == 0; };
  auto filtered = etl::ranges::filter_view(data, even);
  etl::ranges::for_each(filtered, [](const int& i){etl::print(" {}", i);});
```

Output:
```text
 2 4
```

#### Transforming Elements

```cpp
#include <etl/print.h>
#include <etl/ranges.h>
#include <etl/vector.h>

...

  etl::vector<int, 10> data = {1, 2, 3, 4, 5};
  auto squared = etl::ranges::transform_view(data, [](int v) { return v * v; });
  etl::ranges::for_each(squared, [](const int& i){etl::print(" {}", i);});
```

Output:
```text
 1 4 9 16 25
```

#### Composition

Views can be composed using the pipe (`|`) operator, allowing you to chain multiple transformations in a readable, left-to-right style:

```cpp
#include <etl/print.h>
#include <etl/ranges.h>
#include <etl/vector.h>

namespace views = etl::ranges::views;

...

  etl::vector<int, 10> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  auto result = data
    | views::filter([](const int& v) { return v % 2 == 0; })
    | views::transform([](const int& v) { return v * v; });

  etl::ranges::for_each(result, [](const int& i){ etl::print(" {}", i); });
```

Output:
```text
 4 16 36 64 100
```

This first filters the even numbers and then squares them. Each `|` passes the result of the previous stage as input to the next view adaptor.

## Supported Views

All views are in the `etl::ranges` namespace. Corresponding range adaptor objects are available in `etl::ranges::views`.

### Range Factories

| View | `views::` adaptor | Description |
|---|---|---|
| `empty_view<T>` | `views::empty<T>` | A view with no elements. |
| `single_view` | `views::single` | A view containing exactly one element. |
| `iota_view` | `views::iota` | A view of sequentially increasing values. |
| `repeat_view` | `views::repeat` | A view that repeats a value a given number of times. |

### Range Adaptors

| View | `views::` adaptor | Description |
|---|---|---|
| `ref_view` | `views::ref` | A non-owning view that wraps a reference to a range. |
| `owning_view` | `views::owning` | A view that takes ownership of a range via move. |
| — | `views::all` | Returns the range itself (if already a view), a `ref_view`, or an `owning_view`. |
| `filter_view` | `views::filter` | Filters elements based on a predicate. |
| `transform_view` | `views::transform` | Applies a transformation to each element. |
| `as_rvalue_view` | `views::as_rvalue` | Casts each element to an rvalue reference. |
| `as_const_view` | `views::as_const` | Provides a const view of the elements. |
| `cache_latest_view` | `views::cache_latest` | Caches the most recently accessed element (avoids recomputation). |
| `reverse_view` | `views::reverse` | Reverses the order of elements. |
| `drop_view` | `views::drop` | Skips the first *n* elements. |
| `drop_while_view` | `views::drop_while` | Skips leading elements while a predicate is true. |
| `take_view` | `views::take` | Takes the first *n* elements. |
| `take_while_view` | `views::take_while` | Takes leading elements while a predicate is true. |
| `join_view` | `views::join` | Flattens a range of ranges into a single range. |
| `join_with_view` | `views::join_with` | Flattens a range of ranges, inserting a delimiter between each. |
| `split_view` | `views::split` | Splits a range into subranges around a delimiter pattern. |
| `lazy_split_view` | `views::lazy_split` | Lazily splits a range by a pattern (inner ranges discovered on iteration). |
| — | `views::counted` | Creates a view of *n* elements starting from an iterator. |
| `concat_view` | `views::concat` | Concatenates multiple ranges into a single view. |
| `zip_view` | `views::zip` | Zips multiple ranges into a view of tuples (length of shortest range). |
| `zip_transform_view` | `views::zip_transform` | Zips multiple ranges and applies a function to each tuple of elements. |
| `common_view` | `views::common` | Adapts a view so that its iterator and sentinel types are the same. |
| `enumerate_view` | `views::enumerate` | Pairs each element with its index, producing tuples of (index, value). |
| `elements_view` | `views::elements` | Extracts the *N*-th element from each tuple-like value. |
| `keys_view` | `views::keys` | Alias for `elements_view` with *N*=0 (extracts first element of pairs/tuples). |
| `values_view` | `views::values` | Alias for `elements_view` with *N*=1 (extracts second element of pairs/tuples). |
| `adjacent_view` | `views::adjacent` | Produces a view of tuples of *N* adjacent elements (sliding window of tuples). |
| — | `views::pairwise` | Alias for `views::adjacent<2>`. |
| `adjacent_transform_view` | `views::adjacent_transform` | Applies a function to each group of *N* adjacent elements. |
| — | `views::pairwise_transform` | Alias for `views::adjacent_transform<2>`. |
| `chunk_view` | `views::chunk` | Splits a range into non-overlapping chunks of a given size. |
| `slide_view` | `views::slide` | Produces overlapping subranges (sliding windows) of a given size. |
| `chunk_by_view` | `views::chunk_by` | Splits a range into subranges between adjacent elements where a predicate is false. |
| `stride_view` | `views::stride` | Yields every *N*-th element from the underlying range. |
| `cartesian_product_view` | `views::cartesian_product` | Produces the Cartesian product of multiple ranges as a view of tuples. |
| `to_input_view` | `views::to_input` | Downgrades iterator category to input iterator while preserving elements and order. |
| `subrange` | — | Represents a sub-range defined by an iterator–sentinel pair. |

All views support range-based for-loop iteration and can be composed with the pipe (`|`) operator.

## Supported Algorithms

All algorithms are callable objects in the `etl::ranges` namespace. Each supports both an iterator-pair overload and a range overload (where applicable), and most accept optional projection and comparator arguments.

### Non-modifying Sequence Operations

| Algorithm | Description |
|---|---|
| `for_each` | Applies a function to each element in a range. |
| `for_each_n` | Applies a function to the first *n* elements. |
| `find` | Finds the first element equal to a value. |
| `find_if` | Finds the first element satisfying a predicate. |
| `find_if_not` | Finds the first element not satisfying a predicate. |
| `find_end` | Finds the last occurrence of a subsequence. |
| `find_first_of` | Finds the first element matching any in a second range. |
| `adjacent_find` | Finds the first pair of adjacent equal elements. |
| `count` | Counts elements equal to a value. |
| `count_if` | Counts elements satisfying a predicate. |
| `all_of` | Checks if all elements satisfy a predicate. |
| `any_of` | Checks if any element satisfies a predicate. |
| `none_of` | Checks if no elements satisfy a predicate. |
| `mismatch` | Finds the first position where two ranges differ. |
| `equal` | Checks if two ranges are equal. |
| `is_permutation` | Checks if one range is a permutation of another. |
| `search` | Searches for the first occurrence of a subsequence. |
| `search_n` | Searches for *n* consecutive copies of a value. |
| `starts_with` | Checks if a range starts with another range. |
| `ends_with` | Checks if a range ends with another range. |
| `lexicographical_compare` | Compares two ranges lexicographically. |

### Fold Operations

| Algorithm | Description |
|---|---|
| `fold_left` | Left-folds elements with a binary operation. |
| `fold_left_with_iter` | Left-folds, returning both the result and an iterator. |
| `fold_left_first` | Left-folds using the first element as the initial value. |
| `fold_left_first_with_iter` | Like `fold_left_first`, also returning an iterator. |
| `fold_right` | Right-folds elements with a binary operation. |
| `fold_right_last` | Right-folds using the last element as the initial value. |

### Modifying Sequence Operations

| Algorithm | Description |
|---|---|
| `copy` | Copies elements to a destination range. |
| `copy_n` | Copies *n* elements to a destination range. |
| `copy_if` | Copies elements satisfying a predicate. |
| `copy_backward` | Copies elements backwards to a destination range. |
| `move` | Moves elements to a destination range. |
| `move_backward` | Moves elements backwards to a destination range. |
| `swap_ranges` | Swaps elements between two ranges. |
| `replace` | Replaces elements equal to a value. |
| `replace_if` | Replaces elements satisfying a predicate. |
| `replace_copy` | Copies, replacing elements equal to a value. |
| `replace_copy_if` | Copies, replacing elements satisfying a predicate. |
| `remove` | Removes elements equal to a value. |
| `remove_if` | Removes elements satisfying a predicate. |
| `remove_copy` | Copies, omitting elements equal to a value. |
| `fill` | Fills a range with a value. |
| `fill_n` | Fills *n* elements with a value. |
| `generate` | Assigns each element the result of a generator function. |
| `generate_n` | Assigns *n* elements the result of a generator function. |
| `iota` | Fills a range with sequentially increasing values. |
| `unique` | Removes consecutive duplicate elements. |
| `unique_copy` | Copies, removing consecutive duplicates. |
| `transform` | Applies a transformation to each element. |
| `reverse` | Reverses the order of elements. |
| `reverse_copy` | Copies elements in reverse order. |
| `rotate` | Rotates elements in a range. |
| `rotate_copy` | Copies elements with rotation. |
| `shift_left` | Shifts elements to the left. |
| `shift_right` | Shifts elements to the right. |
| `shuffle` | Randomly reorders elements. |
| `sample` | Selects *n* random elements from a range. |

### Sorting Operations

| Algorithm | Description |
|---|---|
| `sort` | Sorts elements in a range. |
| `stable_sort` | Sorts elements preserving relative order of equivalent elements. |
| `partial_sort` | Partially sorts a range so that the first *n* elements are sorted. |
| `partial_sort_copy` | Copies and partially sorts elements. |
| `nth_element` | Partially sorts so that the *n*-th element is in its sorted position. |
| `is_sorted` | Checks if a range is sorted. |
| `is_sorted_until` | Finds the first unsorted element. |

### Partitioning Operations

| Algorithm | Description |
|---|---|
| `partition` | Partitions elements by a predicate. |
| `stable_partition` | Partitions elements, preserving relative order. |
| `is_partitioned` | Checks if a range is partitioned. |
| `partition_copy` | Copies elements into two ranges based on a predicate. |
| `partition_point` | Finds the partition point. |

### Binary Search (on sorted ranges)

| Algorithm | Description |
|---|---|
| `lower_bound` | Finds the first element not less than a value. |
| `upper_bound` | Finds the first element greater than a value. |
| `equal_range` | Returns the range of elements equal to a value. |
| `binary_search` | Checks if a sorted range contains a value. |

### Set Operations (on sorted ranges)

| Algorithm | Description |
|---|---|
| `includes` | Checks if one sorted range includes another. |
| `merge` | Merges two sorted ranges. |
| `inplace_merge` | Merges two consecutive sorted sub-ranges in place. |
| `set_union` | Computes the union of two sorted ranges. |
| `set_intersection` | Computes the intersection of two sorted ranges. |
| `set_difference` | Computes the difference of two sorted ranges. |
| `set_symmetric_difference` | Computes the symmetric difference of two sorted ranges. |

### Heap Operations

| Algorithm | Description |
|---|---|
| `make_heap` | Creates a heap from a range. |
| `push_heap` | Pushes an element onto a heap. |
| `pop_heap` | Pops the top element from a heap. |
| `sort_heap` | Sorts a heap into a sorted range. |
| `is_heap` | Checks if a range is a heap. |
| `is_heap_until` | Finds the first element that breaks the heap property. |

### Min/Max Operations

| Algorithm | Description |
|---|---|
| `min` | Returns the smaller of two values or the smallest in an initializer list. |
| `min_element` | Finds the smallest element in a range. |
| `max` | Returns the larger of two values or the largest in an initializer list. |
| `max_element` | Finds the largest element in a range. |
| `minmax` | Returns the smaller and larger of two values. |
| `minmax_element` | Finds both the smallest and largest elements in a range. |
| `clamp` | Clamps a value between a minimum and maximum. |

### Permutation Operations

| Algorithm | Description |
|---|---|
| `next_permutation` | Generates the next lexicographic permutation. |
| `prev_permutation` | Generates the previous lexicographic permutation. |


## Reference

For reference to the STD implementation, see also:

- Algorithms: https://en.cppreference.com/w/cpp/algorithm.html
- Ranges/Views: https://en.cppreference.com/w/cpp/ranges.html

## Limitations

- Not all C++20 range features are available due to limitation to C++17. Especially C++20 concepts are not used.
- Designed for ETL containers but can work with standard containers if compatible with ETL's iterator requirements.
