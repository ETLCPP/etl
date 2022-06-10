/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 John Wellbelove

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

#include "unit_test_framework.h"

#include "etl/debounce.h"

namespace
{
  SUITE(test_debounce)
  {
    TEST(test_debounce_default_4_0_0_nonbounce)
    {
      etl::debounce<4, 0, 0> key_state;

      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      //*******************************
      // Transition to set.
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      // Valid set.
      CHECK(key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      //*******************************
      // Transition to clear.
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      // Valid clear.
      CHECK(key_state.add(false));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());
    }

    //*************************************************************************
    TEST(test_debounce_default_4_4_0_nonbounce)
    {
      etl::debounce<4, 4, 0> key_state;

      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      //*******************************
      // Transition to set.
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      // Valid set.
      CHECK(key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      //*******************************
      // Transition to held.
      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      // Valid held.
      CHECK(key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      //*******************************
      // No transition to repeating.
      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      //*******************************
      // Transition to clear.
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      // Valid clear.
      CHECK(key_state.add(false));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());
    }

    //*************************************************************************
    TEST(test_debounce_default_4_4_3_nonbounce)
    {
      etl::debounce<4, 4, 3> key_state;
      
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());
      
      //*******************************
      // Transition to set.
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());
      
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());
      
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());
      
      // Valid set.
      CHECK(key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());
      
      //*******************************
      // Transition to held.
      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      // Valid held.
      CHECK(key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      //*******************************
      // Transition to repeating.
      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      // Valid repeat.
      CHECK(key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(key_state.is_repeating());

      // Valid repeat.
      CHECK(key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(key_state.is_repeating());

      //*******************************
      // Transition to clear.
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(key_state.is_repeating());
      
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(key_state.is_repeating());
      
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(key_state.is_repeating());
      
      // Valid clear.
      CHECK(key_state.add(false));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());
    }

    //*************************************************************************
    TEST(test_debounce_initial_set_4_4_0_nonbounce)
    {
      etl::debounce<4, 4> key_state(true);
      
      CHECK(key_state.is_set());
      
      //*******************************
      // Transition to clear.
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      
      CHECK(key_state.add(false));
      CHECK(!key_state.is_set());
      
      //*******************************
      // Transition to set.
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      
      CHECK(key_state.add(true));
      CHECK(key_state.is_set());
    }
    
    //*************************************************************************
    TEST(test_debounce_default_4_4_0_bounce)
    {
      etl::debounce<4, 4> key_state;
      
      CHECK(!key_state.is_set());
      
      //*******************************
      // Transition to set.
      // Bounce 1  
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      
      CHECK(!key_state.add(false));
      CHECK(!key_state.is_set());
      
      // Bounce 2
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      
      CHECK(!key_state.add(false));
      CHECK(!key_state.is_set());
      
      // Bounce 3
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      
      CHECK(!key_state.add(false));
      CHECK(!key_state.is_set());
      
      // Stopped bouncing
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      
      // Valid set.
      CHECK(key_state.add(true));
      CHECK(key_state.is_set());
      
      //*******************************
      // Transition to clear.
      // Bounce 1
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      
      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      
      // Bounce 2
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());      
      
      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());  
      
      // Bounce 3
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());      
      
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      
      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());      
      
      // Stopped bouncing
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      
      // Valid clear.
      CHECK(key_state.add(false));
      CHECK(!key_state.is_set());
    }

    TEST(test_debounce_default_4_0_0_nonbounce_variables)
    {
      etl::debounce<> key_state;

      key_state.set(4);

      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      //*******************************
      // Transition to set.
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      // Valid set.
      CHECK(key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      //*******************************
      // No transition to hold or repeat.
      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      //*******************************
      // Transition to clear.
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      // Valid clear.
      CHECK(key_state.add(false));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());
    }

    //*************************************************************************
    TEST(test_debounce_default_4_4_0_nonbounce_variables)
    {
      etl::debounce<> key_state;

      key_state.set(4, 4);

      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      //*******************************
      // Transition to set.
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());
            
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());
      
      // Valid set.
      CHECK(key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      //*******************************
      // Transition to held.
      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      // Valid held.
      CHECK(key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      //*******************************
      // No transition to repeating.
      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      //*******************************
      // Transition to clear.
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      // Valid clear.
      CHECK(key_state.add(false));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());
    }

    //*************************************************************************
    TEST(test_debounce_default_4_4_3_nonbounce_variables)
    {
      etl::debounce<> key_state;

      key_state.set(4, 4, 3);

      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      //*******************************
      // Transition to set.
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      // Valid set.
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      //*******************************
      // Transition to held.
      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());

      // Valid held.
      CHECK(key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      //*******************************
      // Transition to repeating.
      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(!key_state.is_repeating());

      // Valid repeat.
      CHECK(key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(key_state.is_repeating());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(key_state.is_repeating());

      // Valid repeat.
      CHECK(key_state.add(true));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(key_state.is_repeating());

      //*******************************
      // Transition to clear.
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(key_state.is_repeating());

      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(key_state.is_repeating());

      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());
      CHECK(key_state.is_held());
      CHECK(key_state.is_repeating());

      // Valid clear.
      CHECK(key_state.add(false));
      CHECK(!key_state.is_set());
      CHECK(!key_state.is_held());
      CHECK(!key_state.is_repeating());
    }

    //*************************************************************************
    TEST(test_debounce_initial_set_4_4_0_nonbounce_variables)
    {
      etl::debounce<> key_state(true);

      key_state.set(4, 4);

      CHECK(key_state.is_set());

      //*******************************
      // Transition to clear.
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());

      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());

      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());

      CHECK(key_state.add(false));
      CHECK(!key_state.is_set());

      //*******************************
      // Transition to set.
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());

      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());

      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());

      CHECK(key_state.add(true));
      CHECK(key_state.is_set());
    }

    //*************************************************************************
    TEST(test_debounce_default_4_4_0_bounce_variables)
    {
      etl::debounce<> key_state;

      key_state.set(4, 4);

      CHECK(!key_state.is_set());

      //*******************************
      // Transition to set.
      // Bounce 1  
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());

      CHECK(!key_state.add(false));
      CHECK(!key_state.is_set());

      // Bounce 2
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());

      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());

      CHECK(!key_state.add(false));
      CHECK(!key_state.is_set());

      // Bounce 3
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());

      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());

      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());

      CHECK(!key_state.add(false));
      CHECK(!key_state.is_set());

      // Stopped bouncing
      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());

      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());

      CHECK(!key_state.add(true));
      CHECK(!key_state.is_set());

      // Valid set.
      CHECK(key_state.add(true));
      CHECK(key_state.is_set());

      //*******************************
      // Transition to clear.
      // Bounce 1
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());

      // Bounce 2
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());

      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());

      // Bounce 3
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());

      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());

      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());

      CHECK(!key_state.add(true));
      CHECK(key_state.is_set());

      // Stopped bouncing
      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());

      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());

      CHECK(!key_state.add(false));
      CHECK(key_state.is_set());

      // Valid clear.
      CHECK(key_state.add(false));
      CHECK(!key_state.is_set());
    }
  };
}
