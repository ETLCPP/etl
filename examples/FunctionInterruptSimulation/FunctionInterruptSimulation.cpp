#include <iostream>

#include "etl/function.h"
#include "etl/callback_service.h"

enum VectorId
{
  TIM1_CC_IRQ_HANDLER = 42,
  TIM2_IRQ_HANDLER    = 43,
  TIM3_IRQ_HANDLER    = 44,
  USART1_IRQ_HANDLER  = 52,
  USART2_IRQ_HANDLER  = 53,
  VECTOR_ID_END,
  VECTOR_ID_OFFSET    = TIM1_CC_IRQ_HANDLER,
  VECTOR_ID_RANGE     = VECTOR_ID_END - VECTOR_ID_OFFSET
};

typedef etl::callback_service<VECTOR_ID_RANGE, VECTOR_ID_OFFSET> InterruptVectors;

// Ensure that the callback service is initialised before use.
InterruptVectors& GetInterruptVectorsInstance()
{
  static InterruptVectors interruptVectors;

  return interruptVectors;
}

extern "C"
{
  InterruptVectors& interruptVectors = GetInterruptVectorsInstance();

  // Function called from the timer1 interrupt vector.
  void TIM1_CC_IRQHandler()
  {
    interruptVectors.callback<TIM1_CC_IRQ_HANDLER>();
  }

  // Function called from the timer2 interrupt vector.
  void TIM2_IRQHandler()
  {
    interruptVectors.callback<TIM2_IRQ_HANDLER>();
  }

  // Function called from the timer3 interrupt vector.
  void TIM3_IRQHandler()
  {
    interruptVectors.callback<TIM3_IRQ_HANDLER>();
  }

  // Function called from the usart1 interrupt vector.
  void USART1_IRQHandler()
  {
    interruptVectors.callback<USART1_IRQ_HANDLER>();
  }

  // Function called from the usart2 interrupt vector.
  void USART2_IRQHandler()
  {
    interruptVectors.callback<USART2_IRQ_HANDLER>();
  }
}

//********************************
// Timer driver.
//********************************
class Timer
{
public:

  // Handler for interrupts from the timer.
  void InterruptHandler(const size_t id)
  {
    std::cout << "Timer interrupt (member) : ID " << id << "\n";
  }
};

//********************************
// Free function timer driver.
//********************************
void FreeTimerInterruptHandler(const size_t id)
{
  std::cout << "Timer interrupt (free)   : ID " << id << "\n";
}

//********************************
// UART driver.
//********************************
class Uart
{
public:

  // Constructor.
  Uart(int port_id, int interruptId)
    : port_id(port_id),
      callback(*this)
  {
    GetInterruptVectorsInstance().register_callback(interruptId, callback);
  }

  // Handler for interrupts from the UART.
  void InterruptHandler(const size_t id)
  {
    std::cout << "UART" << port_id << "                    : ID " << id << "\n";
  }

  etl::function_mp<Uart, size_t, &Uart::InterruptHandler> callback;

  int port_id;
};

void UnhandledInterrupt(const size_t id)
{
  std::cout << "Unhandled Interrupt      : ID " << id << "\n";
}

// Declare the driver instances.
Timer timer;
Uart  uart1(0, USART1_IRQ_HANDLER);
Uart  uart2(1, USART2_IRQ_HANDLER);

// Declare a global callback for the timer.
// Uses the most efficient callback type for a class, as everything is known at compile time.
etl::function_imp<Timer, size_t, timer, &Timer::InterruptHandler> timer_member_callback;

// Declare the callbacks for the free functions.
etl::function_fp<size_t, FreeTimerInterruptHandler> timer_free_callback;
etl::function_fp<size_t, UnhandledInterrupt>        unhandled_callback;

//********************************
// Test it out.
//********************************
int main()
{
  // Setup the callbacks.
  InterruptVectors& interruptVectors = GetInterruptVectorsInstance();

  interruptVectors.register_callback<TIM1_CC_IRQ_HANDLER>(timer_member_callback);
  interruptVectors.register_callback<TIM2_IRQ_HANDLER>(timer_free_callback);
  interruptVectors.register_unhandled_callback(unhandled_callback);

  // Simulate the interrupts.
  TIM1_CC_IRQHandler();
  TIM2_IRQHandler();
  USART1_IRQHandler();
  USART2_IRQHandler();
  TIM3_IRQHandler(); // Unhandled!

  return 0;
}
