
#include "platform.h"

#include "atomic.h"

struct spsc_queue_implementation
{
    enum
    {
        UNKNOWN,
        STD_ATOMIC,
        ETL_ATOMIC,
        CMSIS_RTOS2,
        FREERTOS,
        GCC,
        WIN32
    };
};

template <typename T, const size_t IMPLEMENTATION = spsc_queue_implementation::UNKNOWN>
class ispsc_queue;

template <typename T, const size_t SIZE, const size_t IMPLEMENTATION = spsc_queue_implementation::UNKNOWN>
class spsc_queue : public ispsc_queue;

#if ETL_ATOMIC_AVAILABLE
template <typename T>
class ispsc_queue<T, spsc_queue_implementation::STD_ATOMIC>
{

};

template <typename T, const size_t SIZE>
class spsc_queue<T, SIZE, spsc_queue_implementation::STD_ATOMIC>
  : public ispsc_queue<T, spsc_queue_implementation::STD_ATOMIC>
{

};
#endif

#if ETL_OPERATING_SYSTEM == ETL_CMSIS_RTOS2
template <typename T, const size_t SIZE>
class spsc_queue<T, SIZE, spsc_queue_implementation::CMSIS_RTOS2>
  : public ispsc_queue<T, spsc_queue_implementation::CMSIS_RTOS2>
{

};
#endif

#if ETL_OPERATING_SYSTEM == ETL_FREERTOS
template <typename T, const size_t SIZE>
class spsc_queue<T, SIZE, spsc_queue_implementation::FREERTOS> 
  : public ispsc_queue<T, spsc_queue_implementation::FREERTOS>
{

};
#endif
