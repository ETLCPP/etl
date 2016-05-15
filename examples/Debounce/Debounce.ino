//***********************************************************************************
// A debounce demo that reads a key and toggles the LED.
// Set the pin to the correct one for your key.
//***********************************************************************************

#include <debounce.h>

const int SAMPLE_TIME    = 1;    // The sample time in ms.
const int DEBOUNCE_COUNT = 50;   // The number of samples that must agree before a key state change is recognised.   50 = 50ms for 1ms sample time.
const int HOLD_COUNT     = 1000; // The number of samples that must agree before a key held state is recognised.   1000 = 1s for 1ms sample time.
const int REPEAT_COUNT   = 200;  // The number of samples that must agree before a key repeat state is recognised.  200 = 200ms for 1ms sample time.
const int KEY            = XX;   // The pin that the key is attached to.

void setup()
{
  // Initialize LED pin as an output and set off.
  pinMode(LED_BUILTIN , OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  // Initialize KEY pin as an input.
  pinMode(KEY, INPUT);
}

void loop()
{
  static int led_state = LOW; 
  static etl::debounce<DEBOUNCE_COUNT, HOLD_COUNT, REPEAT_COUNT> key_state;

  if (key_state.add(digitalRead(KEY) == HIGH)) // Assumes 'HIGH' is 'pressed' and 'LOW' is 'released'.
  {
    if (key_state.is_set())
    {
      led_state = (led_state == LOW ? HIGH : LOW); // Toggle the LED state on every validated press or repeat.
      digitalWrite(LED_BUILTIN, led_state);
    }
  }
  
  delay(SAMPLE_TIME); // Wait 1ms
}
