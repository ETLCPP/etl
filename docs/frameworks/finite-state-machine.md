---
title: "Finite State Machine"
---

{{< callout type="info">}}
  Header: `fsm.h`  
{{< /callout >}}

This page documents version `20.0.0` and above.  

A finite state machine driven by the reception of events (messages) . The incoming messages will be automatically routed to specific handlers based on the message list defined in the template parameters. Optional on_entry and on_exit handlers are available.  

This FSM is slightly more involved to set up than the traditional simple table driven method, but provides great flexibility in implementation. It may also be faster due to the fact that all messages are routed with either O(1) or O(N) rather than scanning a lookup table and calling indirectly through function pointers.  

The `on_event` functions are not virtual. The template class uses [CRTP](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern) to directly call the derived class's functions.  

**Defines the following classes**  
```cpp
etl::ifsm_state
etl::fsm_state <<template>>
etl::fsm
etl::fsm_exception
etl::fsm_null_state_exception
etl::fsm_state_id_exception
etl::fsm_state_list_exception
```

---

```cpp
template <typename... TStates>
class fsm_state_pack 
```
A class to store FSM states.  
Static asserts if state IDs are not 0..N-1 and in order.  
Static asserts if there are no states.  
Static asserts if any state IDs are greater or equal to `etl::No_State_Change`.  
Since: `20.43.0`

Note: This header is a generated from `fsm_generator.h`. For C++03, to handle more than the standard 16 message types then a new one must be generated.  

## Types

```cpp
etl::fsm_state_id_t
```
By default is defined as `uint_least8_t`.  
If the user defines `ETL_FSM_STATE_ID_TYPE` then the type will be set to this.

```cpp
ifsm_state
```
The base for all FSM states.

---

```cpp
etl::fsm_state_id_t get_state_id() const
```
Returns the id of the state.

---

```cpp
virtual fsm_state_id_t process_event(const etl::imessage& message) = 0
```
The `etl::fsm_state` will override this.  
Processes the event message from the specified source.

---

```cpp
virtual fsm_state_id_t on_enter_state()
```
By default returns `No_State_Change`.  
The derived state should override this to provide alternative behaviour.

---

```cpp
virtual void on_exit_state()
```
By default does nothing.  
The derived state should override this to provide alternative behaviour.

```cpp
void add_child_state(etl::ifsm_state& state)
```
Adds the specified state as a child state.  
The first state added will be the default state.  
This is only of use when used with an `etl::hsfm` instance.  

---

```cpp
template <typename TSize>
void set_child_states(etl::ifsm_state** state_list, TSize size)
```
Sets the list of child states.  
Sets the default state to the first in the list.  
Each state is modified to set it's parent state pointer.
This is only of use when used with an `etl::hsfm` instance.  

## Constants

```cpp
static ETL_CONSTANT fsm_state_id_t No_State_Change
```
The return value to indicate that the state will not change.

## fsm_state
A templated state base. Inherits from `etl::ifsm_state`.

## Template parameters

**For C++03**  

`TContext` The FSM context class. i.e. The class derived from `etl::fsm`.  
`TDerived` The derived state.  
`T1`       The first message type.  
`T2...`    The additional message types.  

The maximum number of types can be set by running the generator for this file. The default is 16

**For C++11 and above**  
`TContext`          The FSM context class. i.e. The class derived from `etl::fsm`.  
`TDerived`          The derived state.  
`TMessage_Types...` The message types.  

This definition will automatically be selected if the compiler supports C++11 or greater.  
To use the older C++03 compatible definition, define `ETL_FSM_FORCE_CPP03` as a project setting or in the optional `etl_profile.h`.  

---

The derived class must define the following member functions.  

```cpp
etl::fsm_state_id_t on_event(const Type& msg);
```
`msg`    The event message. A const reference to the concrete message type.  
Replace Type with the concrete message type.  
And so on for all of the template parameter types.  
Returns the id of the next state.  

---

```c++
etl::fsm_state_id_t on_event_unknown(const etl::imessage& msg);
```
`msg`    The event message.  A const reference to the base message type.  
Called when a message type is received that is not in the template list.  
Returns the id of the next state.

## Member Functions

```cpp
TContext& get_fsm_context() const
```
Gets a reference to the FSM context.  
This is the class that was derived from `etl::fsm`.

## Enumerations

`STATE_ID` The id of this state.

## fsm
The state machine.  
Inherits from `etl::imessage_router`.  

---

```cpp
fsm(etl::message_router_id_t id)
```
Constructor.  
Sets the router id for the FSM.  
The FSM is not started.

---

```cpp
template <typename TSize>
void set_states(etl::ifsm_state** p_states, TSize size)
```
Set the states for the FSM.  
Emits an etl::fsm_state_list_exception if size is zero.  
Emits an etl::fsm_state_list_exception if states are in the incorrect id order.  
Emits an etl::fsm_null_state_exception if any of the state pointers is null.  

Note: The pointers to the states in the state list must be at the index specified by the state id.  
i.e. In the example below `stateList[StateId::IDLE] == &idle`  

---

```cpp
template <typename... TStates>
void set_states(etl::fsm_state_pack<TStates...>& state_pack)
```
Set the states for the FSM

---

```cpp
void start(bool call_on_enter_state = true)
```
Starts the FSM.  
If `call_on_enter_state` is `true` then on_enter_state is called for the initial state. Default is `true`.  
Can only be called once.  
Subsequent calls will do nothing.

---

```cpp
void receive(const etl::imessage& message)
```
Top level message handler for the FSM.

---

```cpp
void receive(etl::message_router_id_t destination_router_id, 
             const etl::imessage&     message)
```
Top level message handler for the FSM.  
If the destination id is not the FSM's id, then the message is ignored.

---

```cpp
bool accepts(etl::message_id_t id) const
```
Returns `true`.

---

```cpp
etl::fsm_state_id_t get_state_id() const
```
Gets the current state id.

---

```cpp
ifsm_state& get_state()
```
Gets a reference to the current state interface.  
Emits an `etl::etl::fsm_null_state_exception` if the current state is null.

---

```cpp
const ifsm_state& get_state() const
```
Gets a const reference to the current state interface.  
Emits an `etl::etl::fsm_null_state_exception` if the current state is null.

---

```cpp
bool is_started() const
```
Checks if the FSM has been started.

---

```cpp
void reset(bool call_on_exit_state = false)
```
Resets the FSM to its pre-started state.  
If `call_on_exit_state` is `true` then `on_exit_state` is called for the current state. Default is `false`.

---

```cpp
etl::fsm_state_id_t transition_to(etl::fsm_state_id_t new_state_id)
```
Invokes a state transition.  
Returns the new state id.

## Errors
```cpp
fsm_exception
```
Inherits from `etl::exception`.

---

```cpp
fsm_null_state_exception
```
Inherits from `etl::fsm_exception`

---

```cpp
fsm_state_id_exception
```
Inherits from `etl::fsm_exception`

---

```cpp
fsm_state_list_exception
```
Inherits from `etl::fsm_exception`

## Example
An example of a queued FSM can be found in the repository in `examples\QueuedFSM`

```cpp
const etl::message_router_id_t MOTOR_CONTROL = 0;

//***************************************************************************
// Events
struct EventId
{
  enum
  {
    START,
    STOP,
    STOPPED,
    SET_SPEED
  };
};

//***********************************
class Start : public etl::message<EventId::START>
{
};

//***********************************
class Stop : public etl::message<EventId::STOP>
{
public:

  Stop() : isEmergencyStop(false) {}
  Stop(bool emergency) : isEmergencyStop(emergency) {}

  const bool isEmergencyStop;
};

//***********************************
class SetSpeed : public etl::message<EventId::SET_SPEED>
{
public:

  SetSpeed(int speed) : speed(speed) {}

  const int speed;
};

//***********************************
class Stopped : public etl::message<EventId::STOPPED>
{
};

//***************************************************************************
// States
struct StateId
{
  enum
  {
    IDLE,
    RUNNING,
    WINDING_DOWN,
    NUMBER_OF_STATES
  };
};

//***********************************
// The motor control FSM.
//***********************************
class MotorControl : public etl::fsm
{
public:

  MotorControl()
    : fsm(MOTOR_CONTROL)
  {
  }

  void SetRunningLampOn()
  {
  }

  void SetRunningLampOff()
  {
  }

  void SetEmergencyLampOn()
  {
  }

  void SpeedChangeWarning()
  {
  }

  void LogUnknownEvent(etl::imessage& msg)
  {
  }
};

//***********************************
// The idle state.
// Accepts Start events.
//***********************************
class Idle : public etl::fsm_state<MotorControl, Idle, StateId::IDLE, Start>
{
public:

  //***********************************
  etl::fsm_state_id_t on_event(const Start& event)
  {
    return StateId::RUNNING;
  }

  //***********************************
  etl::fsm_state_id_t on_event_unknown(const etl::imessage& event)
  {
    get_fsm_context().LogUnknownEvent(event);

    return No_State_Change;
  }
};

//***********************************
// The running state.
// Accepts Stop and SetSpeed events.
//***********************************
class Running : public etl::fsm_state<MotorControl, Running, StateId::RUNNING, Stop, SetSpeed>
{
public:

  //***********************************
  etl::fsm_state_id_t on_event(const Stop& event)
  {
    if (event.isEmergencyStop)
    {
      get_fsm_context().SetEmergencyLampOn();      

      return StateId::IDLE;
    }
    else
    {
      return StateId::WINDING_DOWN;
    }
  }

  //***********************************
  etl::fsm_state_id_t on_event(const SetSpeed& event)
  {
    get_fsm_context().SpeedChangeWarning();

    return No_State_Change;
  }

  //***********************************
  etl::fsm_state_id_t on_event_unknown(const etl::imessage& event)
  {
    get_fsm_context().LogUnknownEvent(event);

    return STATE_ID;
  }

  //***********************************
  etl::fsm_state_id_t on_enter_state()
  {
    get_fsm_context().SetRunningLampOn();

    return No_State_Change;
  }

  //***********************************
  void on_exit_state()
  {
    get_fsm_context().SetRunningLampOff();

    return No_State_Change;
  }
};

//***********************************
// The winding down state.
// Accepts Stopped events.
//***********************************
class WindingDown : public etl::fsm_state<MotorControl, WindingDown, StateId::WINDING_DOWN, Stopped>
{
public:

  //***********************************
  etl::fsm_state_id_t on_event(etl::imessage_router& sender, const Stopped& event)
  {
    return StateId::IDLE;
  }

  //***********************************
  etl::fsm_state_id_t on_event_unknown(etl::imessage_router& sender, const etl::imessage& event)
  {
    get_fsm_context().LogUnknownEvent(event);

    return No_State_Change;
  }
};

// The states.
Idle        idle;
Running     running;
WindingDown windingDown;

// The states must be in state id order.
etl::ifsm_state* stateList[StateId::NUMBER_OF_STATES] =
{
  &idle, &running, &windingDown
};

The FSM.
MotorControl motorControl;

// Set the FSM's state list
motorControl.set_states(stateList, etl::size(stateList));

// Start the motor. The first state is 'IDLE'.
motorControl.start();

// Receive a Start event. The next state is 'RUNNING'.
motorControl.receive(Start());

// Receive a SetSpeed event. The state is still 'RUNNING'.
motorControl.receive(SetSpeed(100));

// Receive a Stop event. The next state is 'WINDING_DOWN'.
motorControl.receive(Stop);

// Receive a Stopped event. The next state is 'IDLE'.
motorControl.receive(Stopped);

// Receive a Start event. The next state is 'RUNNING'.
motorControl.receive(Start());

// Receive a Stop(emergency) event. The next state is 'IDLE'.
motorControl.receive(Stop(true));
```
