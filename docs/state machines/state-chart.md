---
title: "state_chart"
---

{{< callout type="info">}}
  Header: `state_chart.h`  
{{< /callout >}}

State Chart  
A finite state machine driven by the reception of events. The incoming event will call the optional action based on a transition table. Optional on_entry and on_exit handlers can be declared in a state table.
An optional parameter may be sent to event handlers.  

This FSM is simpler, both in implementation and use, than the message based FSM class defined elsewhere in the ETL. `See etl::fsm`.  

**Defines the following classes**  
```cpp
etl::istate_chart
etl::state_chart<TObject>
etl::state_chart<TObject, TParameter>
```

`TParameter` defines how a data parameter will be passed to event handlers.  
This may be by value, pointer, reference or const reference.  
For C++11 or above, you may pass an rvalue reference.

---

```cpp
istate_chart<TParameter>
```
The base for all state charts.  
Since: `20.23.0`

## Types
```cpp
event_id_t
```
The type for event ids.

---

```cpp
state_id_t
```
The type for state ids.

## Member Functions

```cpp
event_it_t get_state_id() const
```
Returns the id of the state.

---

```cpp
virtual void process_event(event_id_t event_id, TParameter data) = 0
```
`etl::state_chart` will override this.  
Processes the event message.  
If a data parameter has been configured for `etl::state_chart`, then one will be default constructed.

---

```cpp
virtual void start(bool on_entry_initial = true)
```
Starts the state chart.

---

```cpp
istate_chart<void>
```
The base for all state charts.  
Since: `20.23.0`

## Types
```cpp
event_id_t
```
The type for event ids.

---

```cpp
state_id_t
```
The type for state ids.

## Member Functions
```cpp
event_it_t get_state_id() const
```
Returns the id of the state.

---

```cpp
virtual void process_event(event_id_t event_id) = 0
```
`etl::state_chart` will override this.  
Processes the event message.  
If a data parameter has been configured for `etl::state_chart`, then one will be default constructed.

---

```cpp
virtual void start(bool on_entry_initial = true)
```
Starts the state chart.

---

```cpp
state_chart
```
For run-time defined transition and state tables.  
Inherits from `etl::istate_chart<TParameter>` or `etl::istate_chart<void>`.

## Template parameters
`TObject`     The object type that supplies actions, guards, on entry and on exit functionality.
`TParameter`  The type that will be passed as a parameter (optional).

## Types

`TParameter` parameter_t

## Constructor
```cpp
state_chart(TObject&          object,
            const transition* transition_table_begin,
            const transition* transition_table_end,
            const state*      state_table_begin,
            const state*      state_table_end,
            const state_id_t  state_id)
```
`object`  
The instance of the that supplies actions, guards, on entry and on exit functionality.  

`transition_table_begin`  
`transition_table_end`  
The table of transitions, defining the relationship between events and state changes; defines option actions and guards.
Note: The transition table must have the same lifetime as the state chart. i.e. It must exist while the state chart exists.

`state_table_begin`  
`state_table_end`  
Sets the optional state table defining any `on entry` and `on exit` for states.  
Not all states have to have entries in the table. States with no  'on entry' and 'on_exit' functionality may be omitted.  
Note: The state table must have the same lifetime as the state chart. i.e. It must exist while the state chart exists. 

`state_id`  
The initial state id.  
Note: The constructors do not call the `on entry` function for the initial state.  

`state_chart_ct` void parameter.  
`state_chart_ctp` non-void parameter.  

---

**For compile-time defined transition and state tables.**  
```cpp
A templated class. Inherits from `etl::istate_chart<TParameter>` or `etl::istate_chart<void>`.
```
`TObject`  
The object type that supplies actions, guards, on entry and on exit functionality.  

`TParameter`  
The type that will be passed as a parameter (state_chart_ctp).  

`TObject_Ref`  
A reference to the TObject instance.  

`Transition_Table_Begin`  
A pointer to the start of the transition table.  

`Transition_Table_Size`  
The number of elements in the transition table.  

`State_Table_Begin`  
A pointer to the start of the state table.  

`State_Table_Size`  
The number of elements in the transition table.  

`Initial_State`  
The initial state.

## Types
```cpp
TParameter parameter_t
```

## Constructor

```cpp
state_chart()
```

## Member Functions

```cpp
void start(bool on_entry_initial = true)
```
Starts the state chart.  
if on_entry_initial is `true` and the initial state has an on_entry method, then this will be called.  
The function does nothing after the first call.

---

```cpp
void process_event(event_id_t event_id)
```
Triggers the state chart with the event.  
For `void` parameter.

---

```cpp
void process_event(event_id_t event_id, parameter_t data)
```
Triggers the state chart with the event.  
Passes the parameter to the event handler.  
For non-void parameter.

---

```cpp
TObject& get_object()
const TObject& get_object() const
```
Gets the implementation object instance.

---

```cpp
void set_transition_table(const transition* transition_table_begin,
                          const transition* transition_table_end)
```
Not defined for `state_chart_ct` or `state_chart_ctp`.  
The table of transitions, defining the relationship between events and state changes; defines option actions and guards.  
Note: The transition table must have the same scope as the state chart. i.e. It must exist while the state chart exists.

---

```cpp
void set_state_table(const state* state_table_begin,
                     const state* state_table_end)
```
Not defined for `state_chart_ct` or `state_chart_ctp`.  
Sets the optional state table defining any on_entry and on_exit for states.  
Not all states have to have entries in the table. States with no  on_entry and on_exit functionality may be omitted.  
Note: The state table must have the same scope as the state chart. i.e. It must exist while the state chart exists. 

## Member Types

`transition`

---

**For `etl::state_chart<TImplementation>`**  

```cpp
transition(const state_id_t current_state_id,
           const event_id_t event_id,
           const state_id_t next_state_id,
           void (TObject::* const action)() = nullptr,
           bool (TObject::* const guard)()  = nullptr)
```

```cpp
transition(const event_id_t event_id,
           const state_id_t next_state_id,
           void (TObject::* const action)() = nullptr,
           bool (TObject::* const guard)()  = nullptr)
```

---

**For etl::state_chart<TImplementation, TParameter>**  
```cpp
transition(const state_id_t current_state_id,
           const event_id_t event_id,
           const state_id_t next_state_id,
           void (TObject::* const action)(data_parameter_type) = nullptr,
           bool (TObject::* const guard)()  = nullptr)
```

---

```cpp
transition(const event_id_t event_id,
           const state_id_t next_state_id,
           void (TObject::* const action)(data_parameter_type) = nullptr,
           bool (TObject::* const guard)()  = nullptr)
```
`current_state_id`  
The state id that the state chart must be in for the event to be processed.  
If the second constructor form is used then the 'current state' is considered to be don't care.  

`event_id`  
The event id for this transition.  

`next_state_id`  
The state id that the state chart will be in after the event.  

`action`  
An optional pointer to the action that will be called when the transition occurs.  

`guard`  
An optional pointer to the guard for this transition. The transition will only occur if the guard returns `true`.  
If the guard returns `false` then the scan of the transition table continues from the next position.

## state

```cpp
ETL_CONSTEXPR state(const state_id_t state_id_,
                    void (TObject::* const on_entry)() = ETL_NULLPTR,
                    void (TObject::* const on_exit)()  = ETL_NULLPTR)
```
`state_id`  
The ID of the state.  

`on_entry`  
An optional pointer to the function that will be called when a state is entered.  

`on_exit`  
An optional pointer to the function that will be called when a state is entered.

## Example

```cpp
class MotorControl : public etl::state_chart<MotorControl>
{
  void OnStart();
  void OnStop();
  void OnSetSpeed();
  bool StartGuard();

  static const etl::array<MotorControl::transition, 5> transitionTable;
};

const etl::array<MotorControl::transition, 5> MotorControl::transitionTable =
{
  MotorControl::transition(IDLE,
                           START,
                           RUNNING,
                           &MotorControl::OnStart, 
                           &MotorControl::StartGuard),

  MotorControl::transition(RUNNING, 
                           STOP,                                                    
                           WINDING_DOWN, 
                           &MotorControl::OnStop),

  MotorControl::transition(WINDING_DOWN,
                           STOPPED,
                           IDLE),

  MotorControl::transition(RUNNING,
                           EMERGENCY_STOP,
                           IDLE,
                           &MotorControl::OnStop),

  MotorControl::transition(RUNNING,
                           SET_SPEED,
                           RUNNING,
                           &MotorControl::OnSetSpeed)
};
```

## state

```cpp
state(const state_id_t state_id,
      void (TObject::* const on_entry)() = nullptr,
      void (TObject::* const on_exit)()  = nullptr)
```
`state_id`  
The id for this state.  

`on_entry`  
An optional pointer to the function that will be called when the state chart enters the state.  

`on_exit`  
An optional pointer to the function that will be called when the state chart exits the state.  

## Example

```cpp
class MotorControl : public etl::state_chart<MotorControl>
{
  void OnExitIdle();
  void OnEnterStopped();
  void OnEnterRunning);
  void OnExitRunning();

  static const etl::array<MotorControl::state, 3> stateTable;
};

const etl::array<MotorControl::state, 3> MotorControl::stateTable =
{
  MotorControl::state(IDLE,    nullptr,                       &MotorControl::OnExitIdle),
  MotorControl::state(STOPPED, &MotorControl::OnEnterStopped, nullptr),
  MotorControl::state(RUNNING, &MotorControl::OnEnterRunning, &MotorControl::OnExitRunning)
};
```

### Notes

**Order of execution**  
When an event is processed, the execution occurs in the following order.  
(Assuming all functions have been declared in the transition and state tables)  

A call to the guard function.  
A call to the action function.  
A call to the current state's exit function (if the transition changes the state).  
A call to the next state's entry function (if the transition changes the state).  

**Usage**  
The state chart may either used with inheritance or composition.

```cpp
class Implementation : public etl::state_chart<Implementation>

class Implementation
{
   etl::state_chart<Implementation> stateChart;
};
```

Use inheritance when the implementation is a state machine.  
Use composition if the implementation contains a state machine.
