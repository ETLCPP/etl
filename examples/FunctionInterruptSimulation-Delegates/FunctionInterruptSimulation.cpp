#include <iostream>

#include "etl/delegate.h"
#include "etl/delegate_service.h"

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

typedef etl::delegate_service<VECTOR_ID_RANGE, VECTOR_ID_OFFSET> InterruptVectors;

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
    interruptVectors.call<TIM1_CC_IRQ_HANDLER>();
  }

  // Function called from the timer2 interrupt vector.
  void TIM2_IRQHandler()
  {
    interruptVectors.call<TIM2_IRQ_HANDLER>();
  }

  // Function called from the timer3 interrupt vector.
  void TIM3_IRQHandler()
  {
    interruptVectors.call<TIM3_IRQ_HANDLER>();
  }

  // Function called from the usart1 interrupt vector.
  void USART1_IRQHandler()
  {
    interruptVectors.call<USART1_IRQ_HANDLER>();
  }

  // Function called from the usart2 interrupt vector.
  void USART2_IRQHandler()
  {
    interruptVectors.call<USART2_IRQ_HANDLER>();
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
  Uart(int port_id, size_t interruptId)
    : port_id(port_id),
      callback(etl::delegate<void(size_t)>::create<Uart, &Uart::InterruptHandler>(*this))
  {
    GetInterruptVectorsInstance().register_delegate(interruptId, callback);
  }

  // Handler for interrupts from the UART.
  void InterruptHandler(const size_t id)
  {
    std::cout << "UART" << port_id << "                    : ID " << id << "\n";
  }

  etl::delegate<void(size_t)> callback;

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
etl::delegate<void(size_t)> timer_member_callback = etl::delegate<void(size_t)>::create<Timer, timer, &Timer::InterruptHandler>();

// Declare the callbacks for the free functions.
etl::delegate<void(size_t)> timer_free_callback = etl::delegate<void(size_t)>::create<FreeTimerInterruptHandler>();
etl::delegate<void(size_t)> unhandled_callback  = etl::delegate<void(size_t)>::create<UnhandledInterrupt>();

//********************************
// Test it out.
//********************************
int main()
{
  // Setup the callbacks.
  InterruptVectors& interruptVectors = GetInterruptVectorsInstance();

  interruptVectors.register_delegate<TIM1_CC_IRQ_HANDLER>(timer_member_callback);
  interruptVectors.register_delegate<TIM2_IRQ_HANDLER>(timer_free_callback);
  interruptVectors.register_unhandled_delegate(unhandled_callback);

  // Simulate the interrupts.
  TIM1_CC_IRQHandler();
  TIM2_IRQHandler();
  USART1_IRQHandler();
  USART2_IRQHandler();
  TIM3_IRQHandler(); // Unhandled!

  return 0;
}
