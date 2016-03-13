//***********************************************************************************
// A version of the Blink demo, but with delays stored in two instances of etl::list
//***********************************************************************************

#undef min
#undef max

#include <list.h>
#include <container.h>

void setup()
{
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
}

void iterate(const etl::ilist<int>& delays)
{
    etl::ilist<int>::const_iterator itr;

    // Iterate through the list.
    itr = delays.begin();

    while (itr != delays.end())
    {
      digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(100);               // wait
      digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
      delay(*itr++);            // wait
    }
}

void loop()
{
  int delay_times1[] = { 900, 800, 700, 600, 500, 400, 300, 200, 100 };
  int delay_times2[] = { 400, 300, 200, 100 };

  // Fill the first delay list, then reverse it.
  // Notice how we don't have to know the size of the array!
  const size_t size1 = sizeof(etl::array_size(delay_times1));
  etl::list<int, size1> delays1(etl::begin(delay_times1), etl::end(delay_times1));
  delays1.reverse();

  // Fill the second delay list,
  const size_t size2 = sizeof(etl::array_size(delay_times2));
  etl::list<int, size2> delays2(etl::begin(delay_times2), etl::end(delay_times2));

  while (true)
  {
    iterate(delays1);
    iterate(delays2);
  }
}
