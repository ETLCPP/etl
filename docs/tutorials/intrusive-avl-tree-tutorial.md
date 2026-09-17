---
title: "Intrusive AVL Tree"
---

A tutorial on how `etl::intrusive_avl_tree` can give ordered, O(log(N)) access
to objects that already exist elsewhere, without a second container to track
them.

## Overview

Imagine a set of driver/sensor tasks, each owned by its own subsystem. They
are created and destroyed at unpredictable times as modules attach and
detach, so there is no compile-time upper bound on how many exist at once.
A scheduler needs to visit the tasks that are currently due, in ascending
deadline order, on every tick.

A normal fixed-capacity ordered container (`etl::set`/`etl::map`) would need
a second, separately-sized place to store pointers to these tasks. Instead,
embedding an `etl::intrusive_avl_tree` link directly into each task object
gives the scheduler ordered, O(log(N)) access to exactly the tasks that
exist right now - nothing more is allocated, and nothing needs to be kept
in sync by hand. That's the payoff of "intrusive": the tree links the
objects that already exist, rather than storing copies of them.

The example below also demonstrates two things that fall out of the design
for free:
- **Move support.** When the `std::vector` that owns the tasks reallocates,
  every task is moved into new storage - and each move takes over its
  task's exact tree position, with no rebalancing and no code of its own.
- **Automatic removal on destruction.** When a task object is destroyed
  while still linked to the tree - e.g. because its module was unplugged -
  the tree unlinks it automatically. No explicit `erase()` call is needed,
  and no dangling entry is left behind.

## Example

```cpp
#include "etl/intrusive_avl_tree.h"

#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>

// A driver/sensor task that already exists as a long-lived object owned by
// its own subsystem. Inheriting from the tree's link_type is all it takes
// to make it schedulable - no separate fixed-capacity container is needed
// to track which tasks exist.
class SensorTask : public etl::intrusive_avl_tree_base<0>::link_type
{
public:
  SensorTask(uint32_t id_, uint32_t deadline_, uint32_t period_)
    : id(id_), deadline(deadline_), period(period_)
  {
  }

  // Orders primarily by deadline; ties are broken by id so two tasks due
  // on the same tick still occupy distinct tree positions.
  struct Locate
  {
    uint32_t target_deadline;
    uint32_t target_id;

    int operator()(const SensorTask& node) const
    {
      if (target_deadline != node.deadline)
      {
        return int(target_deadline) - int(node.deadline);
      }
      return int(target_id) - int(node.id);
    }
  };

  uint32_t id;
  uint32_t deadline; // next tick at which this task must be serviced
  uint32_t period;
};

typedef etl::intrusive_avl_tree<SensorTask> Schedule;

// The only place a task gets linked - called whenever a module attaches,
// or whenever a task is rescheduled after being serviced.
void schedule_task(Schedule& schedule, SensorTask& task)
{
  schedule.find_or_insert(SensorTask::Locate{ task.deadline, task.id },
                           [&task] { return &task; });
}

// Visits every currently-due task in ascending deadline order, services it,
// then reschedules it at its next deadline.
void service_due_tasks(Schedule& schedule, uint32_t now)
{
  for (auto it = schedule.begin(); (it != schedule.end()) && (it->deadline <= now); )
  {
    SensorTask& task = *it;
    it = schedule.erase(it); // O(log N), rebalances

    std::cout << "servicing task " << task.id << " (was due at " << task.deadline << ")\n";

    task.deadline = now + task.period;
    schedule_task(schedule, task); // reschedule at the new deadline
  }
}

void print_schedule(const Schedule& schedule)
{
  for (auto it = schedule.begin(); it != schedule.end(); ++it)
  {
    std::cout << "  task " << it->id << " due at " << it->deadline << "\n";
  }
}

int main()
{
  Schedule schedule;

  // Driver registry: a plain std::vector that a subsystem uses to own its
  // tasks for whatever reason it already needs to - the tree does not
  // require this vector to exist, it's just a stand-in for "wherever the
  // real owner happens to keep the object".
  std::vector<SensorTask> driver_registry;
  driver_registry.reserve(2);

  driver_registry.emplace_back(1, 100, 1000);
  schedule_task(schedule, driver_registry.back());

  driver_registry.emplace_back(2, 300, 1000);
  schedule_task(schedule, driver_registry.back());

  std::cout << "Initial schedule (ascending deadline):\n";
  print_schedule(schedule);

  std::cout << "\nschedule.size() before growth: " << schedule.size() << "\n";

  // driver_registry.reserve()'d capacity was 2; this emplace_back forces a
  // reallocation. std::vector move-constructs every existing SensorTask
  // into the new storage. Each move steals its task's exact tree position
  // (O(1), no rebalancing) - the tree needs no code of its own to survive
  // the underlying storage moving.
  driver_registry.emplace_back(3, 200, 1100);
  schedule_task(schedule, driver_registry.back());

  std::cout << "schedule.size() after growth:  " << schedule.size() << "\n\n";

  std::cout << "Schedule after growth (still correctly ordered):\n";
  print_schedule(schedule);

  std::cout << "\nServicing due tasks at tick 250:\n";
  service_due_tasks(schedule, 250);

  std::cout << "\nSchedule after servicing:\n";
  print_schedule(schedule);

  // Unplugging a module destroys its SensorTask. The link_type destructor
  // notices it is still linked and unlinks + rebalances the tree
  // automatically - no explicit schedule.erase() call is made here.
  std::cout << "\nschedule.size() before unplugging task 2: " << schedule.size() << "\n";

  auto pos = std::find_if(driver_registry.begin(), driver_registry.end(),
                           [](const SensorTask& t) { return t.id == 2; });
  driver_registry.erase(pos); // ~SensorTask() -> ~link_type() auto-unlinks

  std::cout << "schedule.size() after unplugging task 2:  " << schedule.size() << "\n";

  return 0;
}
```

## Program output

```
Initial schedule (ascending deadline):
  task 1 due at 100
  task 2 due at 300

schedule.size() before growth: 2
schedule.size() after growth:  3

Schedule after growth (still correctly ordered):
  task 1 due at 100
  task 3 due at 200
  task 2 due at 300

Servicing due tasks at tick 250:
servicing task 1 (was due at 100)
servicing task 3 (was due at 200)

Schedule after servicing:
  task 2 due at 300
  task 1 due at 1250
  task 3 due at 1350

schedule.size() before unplugging task 2: 3
schedule.size() after unplugging task 2:  2
```
