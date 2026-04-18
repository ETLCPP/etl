---
title: "Endian"
---

{{< callout type="info">}}
  Header: `endian.h`  
  Since: `TBC`  
{{< /callout >}}

Constants & utilities for endianess.  

For endian specific integral types, see `unaligned_type`.  

The values for endianness are dependant on the platform setup.

If `ETL_ENDIAN_NATIVE` is defined by the user, then  
&emsp;`etl::endian::little = 0`  
&emsp;`etl::endian::big    = 1`  

If `ETL_ENDIAN_NATIVE` is not defined by the user, then the ETL selects an appropriate definition.  
&emsp;If `ETL_CPP20_SUPPORTED == 1` and `ETL_USING_STL == 1` then  
&emsp;&emsp;`etl::endian::little = std::endian::little`  
&emsp;&emsp;`etl::endian::big    = std::endian::big`  
&emsp;&emsp;`etl::endian::native = std::endian::native`  

else, if `__BYTE_ORDER__` is defined then
&emsp;If `__ORDER_LITTLE_ENDIAN__` is defined then
&emsp;&emsp;`etl::endian::little =  __ORDER_LITTLE_ENDIAN__`  
&emsp;&emsp;`etl::endian::big    =  __ORDER_BIG_ENDIAN__`  
&emsp;&emsp;`etl::endian::native =  __BYTE_ORDER__`  

&emsp;else if `__LITTLE_ENDIAN__` is defined  
&emsp;&emsp;`etl::endian::little =  __LITTLE_ENDIAN__`  
&emsp;&emsp;`etl::endian::big    =  __BIG_ENDIAN__`  
&emsp;&emsp;`etl::endian::native =  __BYTE_ORDER__`  

else  
The user needs to define `ETL_ENDIAN_NATIVE` either as `0` for little endian or `1` for big endian.

## endian
A smart enumeration defining little and big members.  
```cpp
etl::endian::little;
etl::endian::big;
```

## endianness
Interrogates the endianness of the platform.  
```cpp
etl::endian operator ()() const
```
`constexpr` if `ETL_CPP11_SUPPORTED == 1` and `ETL_ENDIAN_NATIVE` is defined.

---

```cpp
operator etl::endian() const
```
`constexpr` if `ETL_CPP11_SUPPORTED == 1` and `ETL_ENDIAN_NATIVE` is defined.

---

```cpp
static etl::endian value()
```
`constexpr` if `ETL_CPP11_SUPPORTED == 1` and `ETL_ENDIAN_NATIVE` is defined.


## Host to network
```cpp
template <typename T>
T hton(T value)
```
**Description**  
Converts `value` from host to network ordering.

## Network to host
```cpp
template <typename T>
T ntoh(T value)
```
**Description**  
Converts `value` from network to host ordering.