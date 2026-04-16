---
title: "bresenham_line"
---

A 'pseudo' container that generates coordinates on a line between two points using the [Bresenham line algorithm](https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm).  
The class has an STL-like API and is a forward iterator type container.  

**Note:** The iterator only supports pre-increment.

```cpp
etl::bresenham_line<typename T>
```
Where `T` is the coordinate element type.

## Member types

```cpp
value_type              etl::coordinate_2d<T>
size_type               std::size_t
difference_type         std::ptrdiff_t
const_reference         const value_type&
const_pointer           const value_type*
const_iterator          Constant forward iterator
```

## Constructor
```cpp
etl::bresenham_line<T>();
```
**Description**  
Default constructor.  
Creates an empty line.

---

```cpp
etl::bresenham_line<T>(T first_x, T first_y, T last_x, T last_y);
```
**Description**  
Creates a line from pairs of coordinates.

---

```cpp
etl::bresenham_line<T>(const etl::coordinate_2d<T>& first, const etl::coordinate_2d<T>& last);
```
**Description**  
Creates a line from pairs of coordinates.

## Initialisation

```cpp
void reset(T first_x, T first_y, T last_x, T last_y);
```
**Description**  
Creates a line from pairs of coordinates.  
Overwrites any current coordinates.


---

```cpp
void reset(const etl::coordinate_2d<T>& first, const etl::coordinate_2d<T>& last);
```
**Description**  
Creates a line from pairs of coordinates.  
Overwrites any current coordinates.

## Element access

```cpp
const_reference front() const
```
**Return**  
A const reference to the first coordinate in the line.

---

```cpp
const_reference back() const
```
**Return**  
A const reference to the last coordinate in the line.

## Iterators

```cpp
const_iterator begin()
```
**Return**  
An iterator to the beginning of the coordinate series.  
This will reset the Bresenham line algorithm to the first coordinate.

```cpp
const_iterator end() const
```
**Return**  An iterator to the end of the coordinate series.

## Capacity

```cpp
size_t size() const
```
**Return**  
The number of coordinates in the series.

## Non-member functions
```cpp
== true if the two lines are equal, otherwise false.
!= true if the two lines are not equal, otherwise false.
```

## Examples

**Plot pixels on a line**  
```cpp
std::ostream& operator << (std::ostream& os, const etl::coordinate_2d<int>& coordinate)
{
  os << "(" << coordinate.x << "," << coordinate.y << ")";
  return os;
}

etl::coordinate_2d<int> first = { -3,  5 };
etl::coordinate_2d<int> last  = {  3, -5 };

etl::bresenham_line<int> line(first, last);

std::cout << "There are " 
          << line.size() 
          << " coordinates between " 
          << line.front() 
          << " and " 
          << line.back();

// Plot the pixels between first and last.
std::for_each(line.begin(), line.end(), PlotPixel);
```

**Create a vector of pixels on a line**  
```cpp
etl::coordinate_2d<int> first = { -3,  5 };
etl::coordinate_2d<int> last  = {  3, -5 };

etl::bresenham_line<int> line(first, last);

std::vector<etl::coordinate_2d<int>> coordinates;

// Create the vector of points on the line between first and last.
std::copy(line.begin(), line.end(), std::back_inserter(coordinates));
```
