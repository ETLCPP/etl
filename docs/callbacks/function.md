---
title: "function"
---

{{< callout type="warning">}}
  Deprecated: Please use the more versatile `etl::delegate` or `etl::inplace_function` classes instead.
{{< /callout >}}

{{< callout type="info">}}
  Header: `function.h`  
  Supported: `TBC`  
{{< /callout >}}

A set of wrapper templates to allow a member or static function to be called without the caller having to know the specific details of the callee apart from the parameter type. The templates allow the called function to be abstracted.  

This may be used to implement a platform abstraction layer that allows an embedded application to interface with multiple hardware platforms.  

| Template          | Description                                                               |
| ----------------- | ------------------------------------------------------------------------- |
| etl::function     | Callbacks to free or member functions taking zero or one parameter.<br/>Function pointer at runtime. |
| etl::function_fv  | Callback to a free function taking no parameters. (Functiion Void).<br/>Function pointer at compile time. |
| etl::function_fp  | Callback to a free function taking one parameter. (Function Parameter).<br/> Function pointer at compile time.
| etl::function_mv  | Callback to a member function taking no parameters. (Member Void).<br/>Function pointer at compile time. |
| etl::function_imv | Callback to a member function taking no parameters.  (Instance Member Void ).<br/>Instance reference and function pointer at compile time. |
| etl::function_mp  | Callback to a member function taking one parameter. (Member Parameter).<br/>Function pointer at compile time.|
| etl::function_imp | Callback to a member function taking one parameter. (Instance Member Parameter).<br/>Instance reference and function pointer at compile time.|

*Instances of any of these types may be passed as pointers or references to etl::ifunction.*  

## Interface classes.

```cpp
template <typename TParameter>
class ifunction
```
**Description**  
Interface class for a function taking one parameter.

---

```cpp
template <>
class ifunction<void>
```
**Description**  
Interface class for a function taking no parameters.

**All of the following classes are derived from the above interface classes.**  

## Function address at run time.
The functions are called indirectly through an internal pointer.

```cpp
template <typename TObject, 
          typename TParameter>
class function : public ifunction<TParameter>
```
**Description**  
Class for a member function taking one parameter.

---

```cpp
template <typename TObject>
class function<TObject, void> : public ifunction<void>
```
**Description**  
Class for a member function taking no parameters.

```cpp
template <typename TParameter>
class function<void, TParameter> : public ifunction<TParameter>
```
**Description**  
Class for a free function taking one parameter.

---

```cpp
template <>
class function<void, void> : public ifunction<void>
```
**Description**  
Class for a free function taking no parameters.

## Function address at compile time.
These will be more efficient than the previous definitions, as the one level of indirection is eliminated.

```cpp
template <typename TObject, 
          void (TObject::*Function)()>
class function_mv : public ifunction<void>
```
**Description**  
Class for a member function taking no parameters.

---

```cpp
template <typename TObject, 
          TObject& Instance, 
          void (TObject::*Function)()>
class function_imv : public ifunction<void>
```
**Description**  
Class for a member function taking no parameters.

---

```cpp
template <typename TObject, 
          typename TParameter, 
          void (TObject::*Function)(TParameter)>
class function_mp : public ifunction<TParameter>
```
**Description**  
Class for a member function taking one parameter.

---

```cpp
template <typename TObject, 
          typename TParameter, 
          TObject& Instance, 
          void (TObject::*Function)(TParameter)>
class function_imp : public ifunction<TParameter>
```
**Description**  
Class for a member function taking one parameter.

---

```cpp
template <void (*Function)()>
class function_fv : public ifunction<void>
```
**Description**  
Class for a free function taking no parameters.

---

```cpp
template <typename TParameter, 
          void (*Function)()>
class function_fp : public ifunction<TParameter>
```
**Description**  
Class for a free function taking one parameter.

## Example

```cpp
etl::function
```

These templates are designed to enable easy creation of callbacks to global, static and class member functions without the caller having to know which type actually it is.  

One use that is applicable to embedded platforms is to use them connect interrupt vectors to class member handling functions. Particularly useful when the code is applicable to multiple targets devices.  

The template is designed in such a way that the caller does not need to be aware of the type of the callee.  

The caller declares an instance of `etl::function` or one of its variants. The caller defines a pointer or reference to an `etl::ifunction`. This pointer or reference will be initialised with the instance defined in the caller.  

**Example**  

The interrupt vector table contains entries for three timer timeouts and two uart character rx handlers. These are vectored to normal static functions.  

```cpp
void Timer1TimeoutInterrupt();
void Timer2TimeoutInterrupt();
void Timer3TimeoutInterrupt();
void Uart1RxInterrupt();
void Uart2RxInterrupt();
```

The timer timeout interrupts are handled by an instance of the class `Timer` and the free function `FreeTimerInterruptHandler`. The UART Rx interrupts are handled by instances of the class `Uart`.

`Timer1` interrupts call the member function of an instance of `Timer`.
`Timer2` interrupts call the static member function of `Timer`.
`Timer3` interrupts call the free function `FreeTimerInterruptHandler`.
`UART1` interrupts call the member function of instance 1 of `Uart`.
`UART1` interrupts call the member function of instance 2 of `Uart`.

```cpp
#include <iostream>
#include <iomanip>

#include "function.h"

//********************************
// Fake UART Rx register.
//********************************
char get_char()
{
  static char c = 'A';
  return c++;
}

//********************************
// Interrupt vectors & callbacks. 
//********************************
// Callback interfaces.
// Note that they do not require any knowledge about the callee apart from the parameter type.
etl::ifunction<void>* timer1_callback;   // A pointer to a callback taking no parameters.
etl::ifunction<void>* timer2_callback;   // A pointer to a callback taking no parameters.
etl::ifunction<void>* timer3_callback;   // A pointer to a callback taking no parameters.
etl::ifunction<char>* uart1_rx_callback; // A pointer to a callback taking a char parameter.
etl::ifunction<char>* uart2_rx_callback; // A pointer to a callback taking a char parameter.

extern "C"
{
// Function called from the timer1 interrupt vector.
void Timer1Interrupt()
{
  (*timer1_callback)();
}

// Function called from the timer2 interrupt vector.
void Timer2Interrupt()
{
  (*timer2_callback)();
}

// Function called from the timer3 interrupt vector.
void Timer3Interrupt()
{
  (*timer3_callback)();
}

// Function called from the UART1 rx interrupt vector.
void Uart1RxInterrupt()
{
  (*uart1_rx_callback)(get_char());
}

// Function called from the UART2 rx interrupt vector.
void Uart2RxInterrupt()
{
  (*uart2_rx_callback)(get_char());
}
}

//********************************
// Timer driver.
//********************************
class Timer
{
public:

  // Constructor.
  Timer()
  {
  }

  // Handler for interrupts from the timer.
  void MemberTimerInterruptHandler()
  {
    std::cout << "Timer interrupt (member)\n";
  }

  // Static handler for interrupts from the timer.
  static void StaticTimerInterruptHandler()
  {
    std::cout << "Timer interrupt (static)\n";
  }
};

//********************************
// Free function timer driver.
//********************************
void FreeTimerInterruptHandler()
{
  std::cout << "Timer interrupt (free)\n";
}

etl::function_fv<FreeTimerInterruptHandler> free_callback;

//********************************
// UART driver.
//********************************
class Uart
{
public:

  // Constructor.
  Uart(int port_id)
    : port_id(port_id)
  {
  }

  // Handler for rx interrupts from the UART.
  void RxInterruptHandler(char c)
  {
    std::cout << "UART" << port_id << " Rx char interrupt : Received '" << c << "'\n";
  }

  int port_id;
};

// Declare the driver instances.
Timer timer;
Uart  uart1(0);
Uart  uart2(1);

etl::function_imv<Timer, timer, &Timer::MemberTimerInterruptHandler> timer_member_callback;
etl::function_fv<&Timer::StaticTimerInterruptHandler>                timer_static_callback;
etl::function_imp<Uart, char, uart1, &Uart::RxInterruptHandler>      uart1_callback;
etl::function_imp<Uart, char, uart2, &Uart::RxInterruptHandler>      uart2_callback;

//********************************
// Test it out.
//********************************
int main()
{

  // Setup the callbacks.
  timer1_callback   = &timer_member_callback;
  timer2_callback   = &timer_static_callback;
  timer3_callback   = &free_callback;
  uart1_rx_callback = &uart1_callback;
  uart2_rx_callback = &uart2_callback;

  // Simulate the interrupts.
  Timer1Interrupt();
  Timer2Interrupt();
  Timer3Interrupt();
  Uart1RxInterrupt();
  Uart2RxInterrupt();

  return 0;
}
```
