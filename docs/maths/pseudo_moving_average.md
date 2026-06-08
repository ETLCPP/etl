---
title: "pseudo_moving_average"
---

A moving average algorithm that continuously calculates an average value from a stream of samples.  
The sample size does not affect the size of the instantiated object. There is no overhead based on the number of samples as it simulates a window of N values from the current average.  

There are four variants of the algorithm; two for integral values and two for floating point. Each sub-variant allows the selection of compile time or run time sample size.  
The integral variant allows a compile time scaling factor to emulate fixed point arithmetic.  

## Integral

```cpp
template <typename T, const size_t SAMPLE_SIZE, const size_t SCALING>
pseudo_moving_average;
```

```cpp
static const size_t SAMPLE_SIZE
```
The number of samples averaged over.  
If this value is zero, then the run time sample size specialisation is used.

```cpp
static const size_t SCALING
```
The sample scaling factor.

```cpp
pseudo_moving_average(const T initial_value)
```
**Description**  
Constructs the object with the initial value for the average.

---

```cpp
pseudo_moving_average(const T initial_value, const size_t sample_size)
```
**Description**  
*For runtime sample size specialisation only.*  
Constructs the object with the initial value for the average and the sample size.

---

```cpp
void clear(const T initial_value)
```
**Description**  
Clears the object to the initial value for the average.

---

```cpp
void add(T new_value)
```
**Description**  
Adds a new sample value.

---

```cpp
T value() const
```
**Description**  
Returns the scaled value of the average.  
To unscale the returned value, use one of the rounding found in scaled_rounding.

---

```cpp
iterator input()
```
**Description**  
Returns an iterator that allows the input of new values.

## Example
```cpp
std::array data{ 9, 1, 8, 2, 7, 3, 6, 4, 5 };
etl::pseudo_moving_average<int, SAMPLE_SIZE, SCALING> cma(0);
std::copy(data.begin(), data.end(), cma.input());
int average = cma.value();
```

---

```cpp
void set_sample_size(const size_t sample_size)
```
**Description**  
For runtime sample size specialisation only.  
Sets the sample size.

## Floating point

```cpp
template <typename T, const size_t SAMPLE_SIZE>
pseudo_moving_average;
```
```cpp
static const size_t SAMPLE_SIZE
```
The number of samples averaged over.

---

```cpp
pseudo_moving_average(const T initial_value)
```
**Description**  
Constructs the object with the initial value for the average.

---

```cpp
pseudo_moving_average(const T initial_value, const size_t sample_size)
```
**Description**  
For runtime sample size specialisation only.  
Constructs the object with the initial value for the average and the sample size.

---

```cpp
void clear(const T initial_value)
```
**Description**  
Clears the object to the initial value for the average.

---

```cpp
void add(T new_value)
```
**Description**  
Adds a new sample value.

---

```cpp
T value() const
```
**Description**  
Returns the average value.

---

```cpp
void set_sample_size(const size_t sample_size)
```
**Description**  
For runtime sample size specialisation only.  
Sets the sample size.

## How It Works

If the current moving average is 5, then an equivalent sequence of samples (for a sample size of 9), that gives the same average, would be 5, 5, 5, 5, 5, 5, 5, 5, 5  

This means, to find the average when adding a new sample to a moving average that has a current value of 5, all we need to do is multiply the current average by the sample size (9), add the new sample, and divide by the sample size + 1 (10).  
