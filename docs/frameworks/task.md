---
title: "task"
---

{{< callout type="info">}}
  Header: `task.h`  
{{< /callout >}}

The base for tasks controlled by `etl::scheduler`.

## Types

`etl::task_priority_t`
The type for task priorities.

## task

```cpp
task(task_priority_t priority)
```
Constructor.  
Sets the task priority.

---

```cpp
virtual ~task();
```
Virtual destructor

---

```cpp
virtual uint32_t task_request_work() const = 0;
```
The derived task must override this.  
Should return a value that represents the amount of work to do. This may be the number of items in the task's message queue, for example.  
Return zero if the task requires no processing time.  

---

```cpp
virtual void task_process_work() = 0;
```
The derived task must override this.  
The task should process one unit of work.

---

```cpp
virtual void on_task_added()
```
The derived task may override this.  
By default, does nothing.  
Since: `19.4.0`

---

```cpp
void set_task_running(bool task_running)
```
Enables/disables the task from processing work.  
Enabled by default.

---

```cpp
bool task_is_running() const;
```
Returns `true` if the task is in the 'running' state.

```cpp
etl::task_priority_t get_task_priority() const
```
Returns the priority of the task.
