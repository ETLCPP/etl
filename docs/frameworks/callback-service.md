---
title: "callback_service"
---

{{< callout type="info">}}
  Header: `callback_service.h`  
{{< /callout >}}

This template class allows easier integration of 'C' style events (such as interrupt vectors) and C++ handlers.  
It can allow abstraction between low level events such as interrupts and their application dependent handlers.  
The handlers may be any combination of global, static or member functions.  
It utilises the templated function wrapper.  

The callbacks are identified by an id. The values of the ids must range from zero or a specified offset, up to the maximum number of specified callbacks. Calling an unused callback id will either do nothing or, if the user has specified a handler, call this with the id of the callback.  

There are functions that use both runtime and compile time checks of the callback id.  
Compile time is preferable.  

## Member functions

```cpp
template <const size_t RANGE, const size_t OFFSET = 0U>
etl::callback_service
```
`RANGE`  The id range of the callbacks.
`OFFSET` The starting id for the range.

It can allow abstraction between low level events such as interrupts and their application dependent handlers.

```cpp
callback_service()
```
Sets all of the callbacks to route to the unhandled callback.  
Sets the unhandled callback to default (do nothing).

---

```cpp
template <const size_t ID>
void register_callback(etl::ifunction<size_t>& callback)
```
Registers callback with the id specified in the template parameter.  
A compile time error will occur if the id is out of range.

---

```cpp
void register_callback(size_t id, etl::ifunction<size_t>& callback)
```
Registers callback with the id specified in the template parameter.  
The registration will be ignored if the id is out of range.

---

```cpp
void register_unhandled_callback(etl::ifunction<size_t>& callback)
```
Registers the callback to be used for unhandled ids.

---

```cpp
template <const size_t ID>
void callback()
```
Calls the callback associated with the id.  
Calls the unhandled callback if the id has not been registered.  
A compile time error will occur if the id is out of range.

---

```cpp
void callback(const size_t id)
```
Calls the callback associated with the id.  
Calls the unhandled callback if the id has not been registered or if is out of range.

## Example

See the example project in `examples\FunctionInterruptSimulation`.
