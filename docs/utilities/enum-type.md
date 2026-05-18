---
title: "enum_type"
---

{{< callout type="info">}}
  Header: `enum_type.h`  
{{< /callout >}}

Smart enumerations.  

A method of declaring enumerations that allow grouping within a structure.  
Avoids the problem of clashing names that can occur with standard enumerations. 

One way to think of the code is as a type with built-in constants and an optional conversion to a string.  

## Example
```cpp
struct CompassDirection
{
  enum enum_type
  {
    North = 0,
    South = 180,
    East  = 90,
    West  = 270
  };

  ETL_DECLARE_ENUM_TYPE(CompassDirection, int)
  ETL_ENUM_TYPE(North, "North")
  ETL_ENUM_TYPE(South, "South")
  ETL_ENUM_TYPE(East,  "East")
  ETL_ENUM_TYPE(West,  "West")
  ETL_END_ENUM_TYPE
};

CompassDirection direction;          // Default construction.

direction = CompassDirection::North; // Assignment from an enumeration constant;

int value = direction;               // Implicit conversion to 'int'.
 
direction = CompassDirection(value); // Explicit conversion from 'int'.
 
direction = CompassDirection(3);     // Explicit conversion from an invalid value.
                                     // This unfortunately cannot be avoided. 
                                     // Caveat emptor!
 
direction = value;                   // **** Compilation error ****
 
std::cout << "Direction = " << direction.c_str(); // Prints "Direction = North"
```

If a conversion to a string is not required then the `ETL_ENUM_TYPE` declarations may be omitted. In that case the `c_str()` function will return a `"?"`. This will also be the case for any enumeration value that does not have an `ETL_ENUM_TYPE` entry.
