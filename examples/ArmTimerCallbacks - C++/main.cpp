
#include <stdio.h>

//#if (__cplusplus < 201103L)
extern "C"
{
//#endif
#include "Board_LED.h"     // ::Board Support:LED
#include "Board_Buttons.h" // ::Board Support:Buttons
//#if (__cplusplus < 201103L)
}
//#endif

#include "stm32f4xx.h"     // Device header

#include "etl/function.h"
#include "etl/callback_timer.h"
#include "etl/vector.h"
#include "etl/iterator.h"
#include "etl/binary.h"

struct FP
{
  void (*function)();
};

static etl::vector<FP, 10> power_callbacks;

void register_poweroff_callback(void (*function)())
{
  FP fp = { function };
  power_callbacks.push_back(fp);
}

const int N_TIMERS = 4;

etl::callback_timer<N_TIMERS> callback_timer;

etl::timer::id::type short_toggle;
etl::timer::id::type long_toggle;
etl::timer::id::type start_timers;
etl::timer::id::type swap_timers;

/*----------------------------------------------------------------------------
 * SystemCoreClockConfigure: configure SystemCoreClock using HSI
                             (HSE is not populated on Nucleo board)
 *----------------------------------------------------------------------------*/
void SystemCoreClockConfigure(void) {

  RCC->CR |= ((uint32_t)RCC_CR_HSION);                     // Enable HSI
  while ((RCC->CR & RCC_CR_HSIRDY) == 0);                  // Wait for HSI Ready

  RCC->CFGR = RCC_CFGR_SW_HSI;                             // HSI is system clock
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);  // Wait for HSI used as system clock

  FLASH->ACR  = FLASH_ACR_PRFTEN;                          // Enable Prefetch Buffer
  FLASH->ACR |= FLASH_ACR_ICEN;                            // Instruction cache enable
  FLASH->ACR |= FLASH_ACR_DCEN;                            // Data cache enable
  FLASH->ACR |= FLASH_ACR_LATENCY_5WS;                     // Flash 5 wait state

  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;                         // HCLK = SYSCLK
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;                        // APB1 = HCLK/4
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;                        // APB2 = HCLK/2

  RCC->CR &= ~RCC_CR_PLLON;                                // Disable PLL

  // PLL configuration:  VCO = HSI/M * N,  Sysclk = VCO/P
  RCC->PLLCFGR = ( 16ul                   |                // PLL_M =  16
                 (384ul <<  6U)           |                // PLL_N = 384
                 (  3ul << 16U)           |                // PLL_P =   8
                 (RCC_PLLCFGR_PLLSRC_HSI) |                // PLL_SRC = HSI
                 (  8ul << 24U)            );              // PLL_Q =   8

  RCC->CR |= RCC_CR_PLLON;                                 // Enable PLL
  while((RCC->CR & RCC_CR_PLLRDY) == 0) __NOP();           // Wait till PLL is ready

  RCC->CFGR &= ~RCC_CFGR_SW;                               // Select PLL as system clock source
  RCC->CFGR |=  RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);  // Wait till PLL is system clock src
}

void StartTimers()
{
  callback_timer.start(short_toggle);
  callback_timer.start(swap_timers);
}

void SwapTimers()
{
  static bool state = false;

  if (!state)
  {
    callback_timer.stop(short_toggle);
    callback_timer.start(long_toggle);
  }
  else
  {
    callback_timer.start(short_toggle);
    callback_timer.stop(long_toggle);
  }

  state = !state;

  callback_timer.start(swap_timers);
}

void LedToggle()
{
  static bool state = false;

  if (state)
  {
    LED_On(0);
  }
  else
  {
    LED_Off(0);
  }

  state = !state;
}

int main()
{
  SystemCoreClockConfigure();                              // configure HSI as System Clock
  SystemCoreClockUpdate();

  LED_Initialize();
  Buttons_Initialize();

  // The LEDs will start flashing fast after 2 seconds.
  // After another 5 seconds they will start flashing slower.
  short_toggle = callback_timer.register_timer(LedToggle,   50,  etl::timer::mode::REPEATING);
  long_toggle  = callback_timer.register_timer(LedToggle,   100,  etl::timer::mode::REPEATING);
  start_timers = callback_timer.register_timer(StartTimers, 2000, etl::timer::mode::SINGLE_SHOT);
  swap_timers  = callback_timer.register_timer(SwapTimers,  1500, etl::timer::mode::SINGLE_SHOT);

  SysTick_Config(SystemCoreClock / 1000);

  callback_timer.enable(true);

  callback_timer.start(start_timers);

  while (true)
  {
    __NOP();
  }
}

extern "C"
{
  void SysTick_Handler()
  {
    const  uint32_t TICK   = 1U;
    static uint32_t nticks = TICK;

    if (callback_timer.tick(nticks))
    {
      nticks = TICK;
    }
    else
    {
      nticks += TICK;
    }
  }
}
