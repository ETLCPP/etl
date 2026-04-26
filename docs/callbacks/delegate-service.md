---
title: "delegate_service"
---

{{< callout type="info">}}
  Header: `delegate_service.h`  
{{< /callout >}}

Delegate Service
This template class allows easier integration of 'C' style events (such as interrupt vectors) and C++ handlers.  
It can allow abstraction between low level events such as interrupts and their application dependent handlers.  
The handlers may be any combination of global, static, member functions, lambdas and functors. 
It utilises the templated function wrapper.  

The delegate callbacks are identified by an id. The values of the ids must range from zero or a specified offset, up to the maximum range of specified delegates. Calling an unused delegate id will either do nothing or, if the user has specified a handler, call this with the id of the delegate.  

There are functions that use both runtime and compile time checks of the delegate id.  
Compile time is preferable.  

---

```cpp
template <size_t Range, size_t Offset = 0U, const etl::delegate<void(size_t)>* Delegates = nullptr>
delegate_service
```
`Range`  
The id range of the delegates (last - first + 1).  

`Offset`  
The starting id for the range. Default `0`.  

`Delegates`  
An optional pointer to an array of delegate pointers.  

If the pointer to a delegate array is supplied as a template parameter then the delegate service may be declared `constexpr`, otherwise the service is runtime only.

## Member functions

```cpp
delegate_service()
```
Runtime only.  
Sets all of the delegates to route to the unhandled delegate.  
Sets the unhandled delegate to default (do nothing).

---

```cpp
template <const size_t ID>
void register_delegate(etl::delegate<void(size_t)>& callback)
```
Runtime only.  
Registers delegate with the id specified in the template parameter.  
A compile time error will occur if the id is out of range.

---

```cpp
void register_delegate(size_t id, etl::delegate<void(size_t)>& callback)
```
Runtime only.  
Registers delegate with the id specified in the template parameter.  
The registration will be ignored if the id is out of range.

---

```cpp
void register_unhandled_delegate(etl::delegate<void(size_t)>& callback)
```
Runtime only.  
Registers the delegate to be used for unhandled ids.

---

```cpp
template <const size_t ID>
void call()
```
Calls the delegate associated with the id.  
Calls the unhandled delegate if the id has not been registered.  
A compile time error will occur if the id is out of range.

---

```cpp
void call(const size_t id)
```
Calls the delegate associated with the id.  
Calls the unhandled delegate if the id has not been registered or if is out of range.

## Example

See the example project in `examples\FunctionInterruptSimulation-Delegates`.
