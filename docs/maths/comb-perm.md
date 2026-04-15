---
title: "Combinations / Permutations"
---

Templates to provide combinations and permutations constants.

## Combinations

See [Combinations](https://en.wikipedia.org/wiki/Combination)

```cpp
template <size_t N, size_t K>
struct combinations
```

**Member const**  
Number of combinations of K items from a total of N.  
```cpp
static ETL_CONSTEXPR size_t value
```

If C++17 is supported.  
```cpp
template <size_t N, size_t K>
inline constexpr size_t combinations_v = combinations<N, K>::value;
```

## Permutations

See [Permutations](https://en.wikipedia.org/wiki/Permutation)

```cpp
template <size_t N, size_t K>
struct permutations
```

**Member const**  
Number of permutations of K items from a total of N.  
```cpp
static ETL_CONSTEXPR size_t value
```

If C++17 is supported.  
```cpp
template <size_t N, size_t K>
inline constexpr size_t permutations_v = permutations<N, K>::value;
```
