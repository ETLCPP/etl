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
  // This may be part of the cross platform interface.
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


