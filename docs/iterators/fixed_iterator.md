---
title: fixed_iterator
---

{{< callout type="info">}}
  Header: `fixed_iterator.h`  
  Since: `TBC`  
{{< /callout >}}

An iterator where increments and decrements are null operations.  
Can be used to copy to or from a fixed address such as a register.

## Constructor
```cpp
template <typename TIterator>
etl::fixed_iterator();
```

```cpp
template <typename TIterator>
etl::fixed_iterator(TIterator it);
```

```cpp
template <typename TIterator>
etl::fixed_iterator(const etl::fixed_iterator&);
```

## Access
```cpp
Titerator get() const;
```
**Decscription**  
Get the internal iterator.

```cpp
void get(TIterator it);
```
**Decscription**  
Set the iterator.

## Operators
```cpp
typename etl::iterator_traits<TIterator>::value_type operator *()
const typename etl::iterator_traits<TIterator>::value_type operator *() const
```
**Decscription**  
Dereference operators

---

```cpp
TIterator operator ->()
const TIterator operator ->() const
```
**Decscription**  
Member dereference operators

---

```cpp
operator TIterator() const
```
**Decscription**  
Conversion operator

## Example
```cpp
etl::vector<char, 32> buffer;

const char* UART_READ  = (const char*) 0x1000;
const char* UART_WRITE = (const char*) 0x1001;

etl::fixed_iterator<char*> uart_read(UART_READ);
etl::fixed_iterator<char*> uart_write(UART_WRITE);

// Read 20 characters from the port.
std::copy_n(uart_read, 20, std::back_inserter<char>(buffer));

// Write the buffer of characters to the port.
std::copy(buffer.begin(), buffer.end(), uart_write);
```
