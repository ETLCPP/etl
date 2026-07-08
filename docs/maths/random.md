---
title: "random"
---

Utilities for producing random numbers.

## random
The base class for all 32 bit random number generators.

If `ETL_POLYMORPHIC_RANDOM` is defined, then the base class will have the following members, otherwise the base is empty.

```cpp
virtual ~random();
virtual void initialise(uint32_t seed) = 0;
virtual uint32_t operator()() = 0;
virtual uint32_t range(uint32_t low, uint32_t high) = 0;
```

## random_xorshift
Uses a 128bit XOR shift algorithm for producing a pseudo-random sequence of integers.  
The result is a 32 bit integer between 0 and 4,294,967,295 (2^32 - 1).

```cpp
random_xor_shift()
```
**Description**  
Constructor.  
Uses the address of the object as the seed for the sequence.

---

```cpp
random_xor_shift(uint32_t seed)
```
**Description**  
Constructor.  
Uses seed as the `seed` for the sequence.

---

```cpp
void initialise(uint32_t seed)
```
**Description**  
Sets the seed as the new seed for the sequence.

---

```cpp
uint32_t operator()()
```
**Return**  
The next number in the pseudo-random sequence.

---

```cpp
uint32_t range(uint32_t low, uint32_t high)
```
** Return**  
A number between the specified ranges, inclusive.

## random_lcg
Generates a 32 bit pseudo-random number using a linear congruent generator.  
The result is a 32 bit integer between 0 and 2,147,483,647 (2^31 - 1).

```cpp
random_lcg()
```
**Description**  
Constructor.  
Uses the address of the object as the seed for the sequence

---
```cpp
random_lcg(uint32_t seed)
```
**Description**  
Constructor.  
Uses seed as the seed for the sequence.

---

```cpp
void initialise(uint32_t seed)
```
**Description**  
Sets the seed as the new seed for the sequence.

---

```cpp
uint32_t operator()()
```
**Return**  
The next number in the pseudo-random sequence.

---

```cpp
uint32_t range(uint32_t low, uint32_t high)
```
**Return**  
A number between the specified ranges, inclusive.

## random_clcg
Generates a 32 bit pseudo-random number using a combined linear congruent generator.  
The result is a 32 bit integer between 0 and 2,147,483,647 (2^31 - 1).

```cpp
random_clcg()
```
**Description**  
Constructor.  
Uses the address of the object as the seed for the sequence.

---

```cpp
random_clcg(uint32_t seed)
```
**Description**  
Constructor.  
Uses seed as the seed for the sequence.

---

```cpp
void initialise(uint32_t seed)
```
**Description**  
Sets the seed as the new seed for the sequence.

---

```cpp
uint32_t operator()()
```
**Return**  
The next number in the pseudo-random sequence.

---

```cpp
uint32_t range(uint32_t low, uint32_t high)
```
**Return**  
A number between the specified ranges, inclusive.

## random_lsfr
Generates a 32 bit pseudo-random number using a linear shift feedback register.  
The result is a 32 bit integer between 1 and 4,294,967,295 (2^32 - 1).  
The seed must not be zero. The output does not include zero.

```cpp
random_lsfr()
```
**Description**  
Constructor.  
Uses the address of the object as the seed for the sequence.

---

```cpp
random_lsfr(uint32_t seed)
```
**Description**  
Constructor.  
Uses seed as the seed for the sequence.

---

```cpp
void initialise(uint32_t seed)
```
**Description**  
Sets the seed as the new seed for the sequence.

```cpp
uint32_t operator()()
```
**Return**  
The next number in the pseudo-random sequence.

---

```cpp
uint32_t range(uint32_t low, uint32_t high)
```
**Return**  
A number between the specified ranges, inclusive.

## random_mwc
Generates a 32 bit pseudo-random number using a multiply-with-carry algorithm.
The result is a 32 bit integer between 1 and 4,294,967,295 (2^32 - 1). 

```cpp
random_mwc()
```
**Description**  
Constructor.  
Uses the address of the object as the seed for the sequence.

---

```cpp
random_mwc(uint32_t seed)
```
**Description**  
Constructor.  
Uses seed as the seed for the sequence.

---

```cpp
void initialise(uint32_t seed)
```
**Description**  
Sets the seed as the new seed for the sequence.

---

```cpp
uint32_t operator()()
```
**Return**  
The next number in the pseudo-random sequence.

---

```cpp
uint32_t range(uint32_t low, uint32_t high)
```
**Return**  
A number between the specified ranges, inclusive.

## random_pcg
Generates a 32 bit pseudo-random number using a  permuted congruential generator algorithm.  
The result is a 32 bit integer between 1 and 4,294,967,295 (2^32 - 1). 

```cpp
random_pcg()
```
**Description**  
Constructor.  
Uses the address of the object as the seed for the sequence.

---

```cpp
random_pcg(uint32_t seed)
```
**Description**  
Constructor.  
Uses seed as the seed for the sequence.

---

```cpp
void initialise(uint32_t seed)
```
**Description**  
Sets the seed as the new seed for the sequence.

```cpp
uint32_t operator()()
```
**Return**  
The next number in the pseudo-random sequence.

---

```cpp
uint32_t range(uint32_t low, uint32_t high)
```
**Return**  
A number between the specified ranges, inclusive.

## random_hash

```cpp
template <typename THash>
```
Generates a 32 bit pseudo-random number by applying a user supplied 32bit hash to a counter.
The hash must implement void `add(uint8_t)` and `uint8_t value()` member functions.

```cpp
random_hash()
```
**Description**  
Constructor.  
Uses the address of the object as the seed for the sequence.

---

```cpp
random_hash(uint32_t seed)
```
**Description**  
Constructor.  
Uses seed as the seed for the sequence.

---

```cpp
void initialise(uint32_t seed)
```
**Description**  
Sets the seed as the new seed for the sequence.

---

```cpp
uint32_t operator()()
```
**Return**  
The next number in the pseudo-random sequence.

---

```cpp
uint32_t range(uint32_t low, uint32_t high)
```
**Return**  
A number between the specified ranges, inclusive.
