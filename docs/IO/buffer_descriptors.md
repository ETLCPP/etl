---
title: "buffer_descriptors"
---

{{< callout >}}
  Header: `buffer_descriptors.h`  
  Since: All versions  
{{< /callout >}}

A set of descriptors to a collection of buffers.

```cpp
template <typename TBuffer,      // The type to store in the buffer.
          TSize    BUFFER_SIZE,  // The size of each buffer.
          size_t   N_BUFFERS,    // The total number of buffers.
          typename TFlag = bool> // The 'in use' flag type.
class buffer_descriptors
```

The type used for the 'in use' flag depends on how the buffer descriptors class is used.
For interrupts and multi-threaded code, either the flag  type must force a fence (by using an atomic type) or the calls to allocate and release must ensure that they are not re-ordered by the compiler or processor.

## Member types
| Type          |                                                                      |
| ------------- | -------------------------------------------------------------------- |
| value_type    | The type that is stored in the buffers                               |
| size_type     | An unsigned integral type                                            |
| flag_type     |                                                                      |
| pointer       |                                                                      |
| descriptor    | A nested class that encapsulates the details of an individual buffer |
| notification  | A nested class that is sent to the user defined callback function    |
| callback_type | `etl::delegate<void(notification)>`                                  |

## Static Constants  
| Value       |                                                           |
| ----------- | --------------------------------------------------------- |
| N_BUFFERS   | The number of buffers that the buffer descriptor controls |
| BUFFER_SIZE | The number of elements in each buffer                     |

## Constructors  
```cpp
buffer_descriptors(pointer pbuffers)
```
**Description**  
Construct with a pointer to the start of the buffers to control.
This storage should be contiguous and large enough to hold `N_BUFFERS`.

---

```cpp
buffer_descriptors(pointer pbuffers, const callback_type& callback)
```
**Description**  
Construct with a pointer to the start of the buffers to control and the callback.

## Member functions  

```cpp
void set_callback(const callback_type& callback)
```
**Description**  
Set the callback for notification.

---

```cpp
bool is_valid() const
```
**Description**  
Returns true if class contains valid buffers.

---

```cpp
void notify(notification n)
```
**Description**  
Calls the user defined callback with the descriptor and buffer size.
Used when the buffer has been filled and is ready for processing via the callback.

---

```cpp
descriptor allocate()
```
**Description**  
Returns a new descriptor. 
If all descriptors are in use then the descriptor will be invalid.

---

```cpp
descriptor allocate(value_type fill)
```
**Description**  
Returns a new descriptor and fills the buffer with fill. 
If all descriptors are in use then the descriptor will be invalid.

---

```cpp
void clear()
```
**Description**  
Clears by releasing all allocated descriptors.

```cpp
descriptor
```
**Description**  
A nested class that encapsulates the details of an individual buffer.

---

```cpp
const size_type MAX_SIZE
```
**Description**  
The maximum size of the buffer.

---

```cpp
descriptor()
```
**Description**  
Default constructor.

---

```cpp
ETL_CONSTEXPR pointer data() const
```
**Description**  
Returns a pointer to the start of the buffer.

---

```cpp
ETL_CONSTEXPR size_type max_size() const
```
**Description**  
Returns the maximum size of the buffer.

---

```cpp
bool is_valid() const
```
**Description**  
Returns true if the descriptor points to a valid buffer.

---

```cpp
bool is_allocated() const
```
**Description**  
Returns true if the descriptor has been allocated.

---

```cpp
bool is_released() const
```
**Description**  
Returns true if the descriptor has been released.

---

```cpp
void release()
```
**Description**  
Releases the descriptor.

---

```cpp
notification
```
**Description**  
A nested class that is sent to the user defined callback function.

---

```cpp
notification()
```
**Description**  
Default contructor.
Initialises to a default constructed descriptor and a count of zero.

---

```cpp
notification(descriptor desc, size_t count)
```
**Description**  
Construct with the supplied parameters.

---

```cpp
descriptor get_descriptor() const
```
**Description**  
Gets the descriptor.

---

```cpp
size_t get_count() const
```
**Description**  
Gets the count.

## Example
*Very simplified.*  
Assumes that there is a DMA driver class called DMA.  
In the real world the descriptor would be queued in the callback and handled in a foreground thread.  
The handler in the thread would release the descriptor.

```cpp
constexpr size_t BUFFER_SIZE = 256U;
constexpr size_t N_BUFFERS   = 8U;

// Define the buffer descriptors type.
using BD = etl::buffer_descriptors<char, BUFFER_SIZE, N_BUFFERS, std::atomic_char>;

// The buffers to use with it.
char buffers[N_BUFFERS][BUFFER_SIZE];

// The function to call when a buffer is ready.
void Callback(BD::notification notification)
{
  // Process the buffer in the descriptor here.
  ProcessData(notification.get_descriptor().data(), notification.get_count());

  // Finished with the descriptor now, so release it back.
  notification.get_descriptor().release();
}

// Create the buffer_descriptors.
BD bd(&buffers[0][0], BD::callback_type::create<Callback>());

// The current dma descriptor.
BD::descriptor dma_descriptor;

// An object that controls the DMA.
DMA dma;

// Call to start the DMA.
void DMAStart()
{
  // Get a new descriptor.
  dma_descriptor = bd.allocate();

  if (dma_descriptor.is_valid())
  {
    // Link the buffer to the DMA channel.
    dma.Start(dma_descriptor.data());
  }
  else
  {
    // No valid descriptors available.
    LogError("No Descriptor Available");
  }
}

// Called when the DMA has completed (usually an interrupt).
void DMAComplete()
{
  // DMA is complete. Notify the callback.
  bd.notify(BD::notification(dma_descriptor, dma.GetTransferredSize()));
}
```
