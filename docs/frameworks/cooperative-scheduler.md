---
title: "scheduler"
---

{{< callout type="info">}}
  Header: `scheduler.h`  
{{< /callout >}}

A lightweight cooperative multi-tasking scheduler.  
Can be used stand-alone or in conjunction with a messaging system such as the ETL's message router or FSM as the back-end handler. For use when a complex OS or RTOS is overkill.  

Calls an 'idle' callback whenever the scheduler returns `true`.  
Calls a 'watchdog' callback whenever the scheduler returns.  

The scheduler makes use of `etl::task`.

## Scheduling Policies
A number of built-in scheduling policies are available.
Note: The tasks are stored in decreasing priority id order. i.e. Higher id = higher priority.

### Sequential Single
```cpp
etl::scheduler_policy_sequential_single
```
A sequential algorithm that calls a task if it has work to do , starting from the highest priority task.  
On return, it moves to the next task in the list. At the end of the list it returns the idle status.  
If no tasks with work were found then the idle status is set to `true`.  

### Sequential Multiple
```cpp
etl::scheduler_policy_sequential_multiple
```
A sequential algorithm that calls a task if it has work to do , starting from the highest priority task.  
On return, it calls the task again if it still has work, otherwise it moves to the next task in the list.  
At the end of the list it returns the idle status.  
If no tasks with work were found then the idle status is set to `true`.  

### Highest Priority
```cpp
etl::scheduler_policy_highest_priority
```
An algorithm that calls the highest priority task that has work to do.  
Returns the idle status.  
If no tasks with work were found then the idle status is set to `true`.  


### Most Work
```cpp
etl::scheduler_policy_most_work
```
An algorithm that calls the task that has the most work to do, starting from the highest priority task.  
Returns the idle status.  
If no tasks with work were found then the idle status is set to `true`.  

### Custom Scheduler
Creating a custom scheduler policy is simple.  
Just create a structure with the following signature and add your own scheduling algorithm.  
To get 'idle' callbacks then the policy must regularly return `true`.  
To get 'watchdog' callbacks then the policy must regularly return.  
```cpp
struct scheduler_policy_custom
{
  bool schedule_tasks(etl::ivector<etl::task*>& task_list)
  {
    bool idle = true;

    //**************************************
    // Add your scheduling policy here.
    // Set 'idle' to false if any tasks were run.
    //**************************************

    return idle;
  }
};
```

## ischeduler
Can be used as a reference to all scheduler instances.
```cpp
ischeduler()
```
Constructor

```cpp
void set_idle_callback(etl::ifunction<void>& callback)
```
Sets the function to be called when idle.

---

```cpp
void set_watchdog_callback(etl::ifunction<void>& callback)
```
Sets the function to be called to reset the watchdog.

---

```cpp
void set_scheduler_running(bool scheduler_running)
```
Sets the running state of the scheduler.

---

```cpp
bool scheduler_is_running() const;
```
Gets the running state of the scheduler.

---

```cpp
void exit_scheduler()
```
Instructs the scheduler to exit after the next idle call.

---

```cpp
void add_task(etl::task& task)
```
Adds a task to the task list.  
The task list is in priority order where higher id = higher priority. Tasks with duplicate ids are in insert order.  
Emits an `etl::scheduler_too_many_tasks_exception` if the task list is full.  

---

```cpp
template <typename TSize>
void add_task_list(etl::task** p_tasks, TSize size);
```
Emits an `etl::scheduler_too_many_tasks_exception` if the task list is full.  
Emits an `etl::scheduler_null_task_exception` if any of the task pointers is null.

---

```cpp
virtual void start()
```
Starts the scheduler.

## scheduler
Inherits from `etl::ischeduler`.

```cpp
template <typename TSchedulerPolicy, size_t MAX_TASKS_>
class scheduler
```
`TSchedulerPolicy` The policy to use schedule tasks.
`MAX_TASKS`        The maximum number of tasks to schedule.

---

```cpp
scheduler()
```
Constructor.

---

```cpp
void start()
```
Starts the scheduler.  
Emits an `etl::scheduler_no_tasks_exception` if there are no tasks in the list.

## Errors

```cpp
scheduler_exception
```
Inherits from `etl::exception`

---

```cpp
scheduler_no_tasks_exception
```
Inherits from `etl::scheduler_exception`

---

```cpp
scheduler_null_task_exception
```
Inherits from `etl::scheduler_exception`

## Overview of use
As a basic example, you will have to define the following...

**Tasks**  
For each of your tasks, derive a class from etl::task and overide the two virtual functions uint32_t task_request_work() const and void task_process_work(). task_request_work returns a value that informs the scheduler that this task has work to process. The return value should be non-zero if the task has work. This meaning of this is user defined, it could be the number of messages in the task's queue, or just a 0 = no work, 1 = have work. task_process_work allows the task to process any work that it has. How much work it processes on each call is user defined; often it will be one 'unit' of work and letting the policy determine which task gets the next opportunity to process more.  

**Task list**  
An array of pointers to the tasks. Passed to the scheduler with `add_task_list`.  
Use add_task to add additional tasks.

**The scheduler**  
An instance of `etl::scheduler` with the required scheduling policy.  
Initialise the task list by calling add_task_list.

**Callbacks**  
If you wish to get callbacks for 'idle' or 'watchdog' then define callback functions and call set_idle_callback and set_watchdog_callback to tell the scheduler.  
The callbacks may be global, static or member function, wrapped in an etl::function.

**Starting the scheduler**  
The scheduler is started by calling `start()`.

## Example

An example of the scheduler can be found in the repository in `examples/Scheduler`.
