#include <iostream>

#include "etl/observer.h"

// Position data.
struct Position
{
  int x;
  int y;
};

// Button data.
struct Button
{
  enum
  {
    Down,
    Up
  };

  int state;
};

// Wheel data.
struct Wheel
{
  int delta;
};

// Definition of a mouse observer.
typedef etl::observer<const Position&, Button, Wheel> Mouse_Observer;

// Definition of an event handler for mouse notifications.
class Event_Handler1 : public Mouse_Observer
{
public:

  // Handle a position notification.
  void notification(const Position& position)
  {
    std::cout << "Event_Handler1 : Position = " << position.x << "," << position.y << "\n";
  }

  // Handle a button notification.
  void notification(Button button)
  {
    std::cout << "Event_Handler1 : Button = " << ((button.state == Button::Up) ? "Up\n" : "Down\n");
  }

  // Handle a wheel notification.
  void notification(Wheel wheel)
  {
    std::cout << "Event_Handler1 : Wheel delta = " << wheel.delta << "\n";
  }
};

// Definition of a second event handler for mouse notifications.
class Event_Handler2 : public Mouse_Observer
{
public:

  // Handler a position notification.
  void notification(const Position& position)
  {
    std::cout << "Event_Handler2 : Position = " << position.x << "," << position.y << "\n";
  }

  // Handle a button notification.
  void notification(Button button)
  {
    std::cout << "Event_Handler2 : Button = " << ((button.state == Button::Up) ? "Up\n" : "Down\n");
  }

  // Handle a wheel notification.
  void notification(Wheel wheel)
  {
    // Not interested in wheel deltas.
  }
};

// The observable mouse driver.
const int MAX_MOUSE_OBSERVERS = 2;

class Mouse_Driver : public etl::observable<Mouse_Observer, MAX_MOUSE_OBSERVERS>
{
public:

  // Notify observers about a position event.
  void Position_Event()
  {
    Position position = { 100, 200 };
    notify_observers(position);
  }

  // Notify observers about a button up event.
  void Button_Event_Up()
  {
    Button button = { Button::Up };
    notify_observers(button);
  }

  // Notify observers about a button down event.
  void Button_Event_Down()
  {
    Button button = { Button::Down };
    notify_observers(button);
  }

  // Notify observers about a wheel up event.
  void Wheel_Event_Up()
  {
    Wheel wheel = { 50 };
    notify_observers(wheel);
  }

  // Notify observers about a wheel down event.
  void Wheel_Event_Down()
  {
    Wheel wheel = { -25 };
    notify_observers(wheel);
  }
};

int main()
{
  Mouse_Driver   mouse_driver;
  Event_Handler1 event_handler1;
  Event_Handler2 event_handler2;

  // Tell the mouse driver about the observers.
  mouse_driver.add_observer(event_handler1);
  mouse_driver.add_observer(event_handler2);

  // Generate some events.
  mouse_driver.Button_Event_Down();
  mouse_driver.Button_Event_Up();
  mouse_driver.Position_Event();
  mouse_driver.Wheel_Event_Down();
  mouse_driver.Wheel_Event_Up();
 
  return 0;
}
