/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 John Wellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef ETL_TIMER_CONFIG_INCLUDED
#define ETL_TIMER_CONFIG_INCLUDED

#include "platform.h"
#include "nullptr.h"
#include "timer.h"

#include <stdint.h>

//*****************************************************************************
// Definitions common to intrusive timers.
//*****************************************************************************

#if defined(ETL_IN_UNIT_TEST)
namespace
{
  class etl_unit_test_intrusive_timer;
}
#endif

namespace etl
{
  namespace private_timer
  {
    class register_list;
    class active_list;
    class expired_list;
  } // namespace private_timer

  class intrusive_timer;
  class imessage_router;

  namespace timer
  {
    class node;

    //***************************************************************************
    /// A timer configuration data structure.
    //***************************************************************************
    class config
    {
    public:

      friend class private_timer::register_list;
      friend class private_timer::active_list;
      friend class private_timer::expired_list;

      friend class node;
      friend class intrusive_timer;

      //*******************************************
      /// Default constructor.
      //*******************************************
      ETL_CONSTEXPR config() ETL_NOEXCEPT
        : interval(etl::timer::interval::No_Interval)
        , mode(etl::timer::mode::No_Mode)
        , id(etl::timer::id::No_Timer)
      {
      }

      //*******************************************
      /// Constructor.
      //*******************************************
      ETL_CONSTEXPR config(etl::timer::interval::type interval_, etl::timer::mode::type mode_) ETL_NOEXCEPT
        : interval(interval_)
        , mode(mode_)
        , id(etl::timer::id::No_Timer)
      {
      }

      //*******************************************
      /// Returns the interval of the timer.
      //*******************************************
      ETL_CONSTEXPR etl::timer::interval::type get_interval() const ETL_NOEXCEPT
      {
        return interval;
      }

      //*******************************************
      /// Returns the mode of the timer.
      //*******************************************
      ETL_CONSTEXPR etl::timer::mode::type get_mode() const ETL_NOEXCEPT
      {
        return mode;
      }

      //*******************************************
      /// Returns the id of the timer.
      //*******************************************
      ETL_CONSTEXPR etl::timer::id::type get_id() const ETL_NOEXCEPT
      {
        return id;
      }

      //*******************************************
      /// Clears the timer configuration.
      //*******************************************
      ETL_CONSTEXPR14 void clear() ETL_NOEXCEPT
      {
        interval = etl::timer::interval::No_Interval;
        mode     = etl::timer::mode::No_Mode;
        id       = etl::timer::id::No_Timer;
      }

    private:

      etl::timer::interval::type interval; ///< The interval of the timer.
      etl::timer::mode::type     mode;     ///< Whether the timer is repeating or single-shot.
      etl::timer::id::type       id;       ///< The id of the timer.
    };
  } // namespace timer
} // namespace etl

#endif
