---
title: "io_port"
---

{{< callout type="info">}}
  Header: `io_port.h`  
  Supported: `20.39.0`  
{{< /callout >}}

A set of templates for building interface classes to memory mapped hardware ports.
They avoid the need to directly map carefully packed (and possibly non-portable) structures onto memory addresses.

>A read from a write-only, or write to a read-only port will result in a compile time error.

**Defines classes for the following IO types**  
Read / Write.  
Read only.  
Write only.  
Write only with shadow register.  

With a shadow register the value written is stored locally and may be read back.  

The port may either have an address fixed at compile time or set at runtime. The compile time versions require no extra overhead compared to a plain memory mapped structure.  

Ports may be sent as parameters to algorithms that expect iterators, except the runtime address version of `io_port_wos`, which must use the built-in iterator if the shadow value is to be correctly updated for writes.  

All classes define the following typedefs.

| Type name         | Maps to             |
| ----------------- | ------------------- |
| `value_type`      | `T`                 |
| `pointer`         | `volatile T*`       |
| `const_pointer`   | `volatile const T*` |
| `reference`       | `volatile T&`       |
| `const_reference` | `volatile const T&` |

## Read/write port
```cpp
io_port_rw<typename T, uintptr_t Address>
```
**Description**  
Compile time port address.

---

**Types**  
iterator  
const_iterator  

---

```cpp
operator T() const
```
**Description**  
Read the value.  
Conversion operator to T.

---

```cpp
iterator iter()
```
**Description**  
Get an iterator to this port.

---

```cpp
const_iterator iter() const
const_iterator citer() const
```
**Description**  
Get a const_iterator to this port.

---

```cpp
T read() const
```
**Description**  
Read the value.

---

```cpp
void write(T value)
```
**Description**  
Write the value.

---

```cpp
io_port_rw& operator =(T value)
```
**Description**  
Write the value.

---

```cpp
pointer get_address()
const_pointer get_address() const
**Description**  
Gets the address of the port.

---

```cpp
io_port_rw& operator |=(value_type value)
```
**Description**  
Or-Equals operator.

---

```cpp
io_port_rw& operator &=(value_type value)
```
**Description**  
And-Equals operator.

---

```cpp
io_port_rw& operator ^=(value_type value)
```
**Description**
Exclusive-Or-Equals operator.

---

```cpp
io_port_rw& operator <<=(int shift)
```
**Description**  
Left-Shift-Equals operator.

---

```cpp
io_port_rw& operator >>=(int shift)
```
**Description**  
Right-Shift-Equals operator.

---

```cpp
value_type operator ~() const
```
**Description**  
Not operator.

## Read only port
```cpp
io_port_ro<typename T, uintptr_t Address>
```

---

**Types**  
```cpp
const_iterator
```

---

```cpp
operator T() const
```
**Description**  
Read the value. Conversion operator to T.

---

```cpp
const_iterator iter() const
const_iterator citer() const
```
**Description**
Get a const_iterator to this port.

---

```cpp
T read() const
```
**Description**  
Read the value.

---

```cpp
const_pointer get_address() const
```
**Description**  
Gets the address of the port.

## Write only port
```cpp
io_port_wo<typename T, uintptr_t Address>
```

---

**Types**  
iterator

---

```cpp
iterator iter()
```
**Description**  
Get an iterator to this port.

---

```cpp
void write(T value)
```
**Description**  
Write the value.

---

```cpp
io_port_wo& operator =(T value)
```
**Description**  
Write the value.

---

```cpp
pointer get_address()
```
**Description**  
Gets the address of the port.

## Write only port, with shadow register
```cpp
io_port_wos<typename T, uintptr_t Address>
```

---

**Types**  
```cpp
iterator
const_iterator
```

---

```cpp
operator T() const
```
**Description**  
Read the value. Conversion operator to `T`.

---

```cpp
iterator iter()
```
**Description**
Get an iterator to this port.

---

```cpp
const_iterator iter() const
const_iterator citer() const
```
**Decsription**  
Get a `const_iterator` to this port.

---

```cpp
T read() const
```
**Description**  
Read the value.

---

```cpp
void write(T value)
```
**Description**  
Write the value.

---

```cpp
io_port_rw& operator =(T value)
```
**Description**
Write the value.

---

```cpp
pointer get_address()
const_pointer get_address() const
```
**Description**  
Gets the address of the port.

---

```cpp
io_port_rw& operator |=(value_type value)
```
**Description**  
Or-Equals operator.

---
```cpp
io_port_rw& operator &=(value_type value)
```
**Description**  
And-Equals operator.

---

```cpp
io_port_rw& operator ^=(value_type value)
```
**Description**  
Exclusive-Or-Equals operator.

---

```cpp
io_port_rw& operator <<=(int shift)
```
**Description**  
Left-Shift-Equals operator.

---

```cpp
io_port_rw& operator >>=(int shift)
```
**Description**  
Right-Shift-Equals operator.

---

```cpp
value_type operator ~() const
```
**description**  
Not operator.

## Example serial port

The example uses a port at a compile time address.

| Variable  | Description                                                |
| --------- | ---------------------------------------------------------- |
| `rxdata`  | An 8 bit read only port                                    |
| `txdata`  | An 8 bit write only port                                   |
| `control` | A 16 bit write only port, with shadow register             |
| `status`  | A 16 bit read only port. It shares an address with control |
| `option`  | An 8 bit read/write port                                   |

```cpp
template <uintptr_t Address>
struct serial_port
{
  etl::io_port_ro<char,      Address>     rxdata;  // Read only rx data register.
  etl::io_port_wo<char,      Address + 1> txdata;  // Write only tx register.
  etl::io_port_wos<uint16_t, Address + 2> control; // Write only, with shadow, control register.
  etl::io_port_ro<uint16_t,  Address + 2> status;  // Read only status register.
  etl::io_port_ro<char,      Address>     option;  // Read/Write register.
};

serial_port<0x800> port; // A serial port at address 0x800

// Read Rx data
char data   = port.rxdata;

// Write Tx data
port.txdata = 'A';

// Compile error! txdata is write only.
data = port.txdata;

// Write to the control register and read back what we wrote.
port.control = 0x1234;
uint16_t control = port.control;

// Flip bit 3 of the control register.
port.control ^= 0x0080;

// Read from the status register.
uint16_t status = port.status;

// Copy data from a buffer to the Tx data port.
std::copy(txBuffer.begin(), txBuffer.end(), serial_port.txdata.iter());

// Copy data from the Rx data port to a buffer.
std::copy_n(serial_port.rxdata, serial_port.status, std::back_inserter(rxBuffer)); 
```
