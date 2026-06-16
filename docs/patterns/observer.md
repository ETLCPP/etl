---
title: "observer"
---

A set of template classes to enable the easy creation of objects using the [Observer Pattern](https://refactoring.guru/design-patterns/observer).  

The observer pattern is a software design pattern in which an object maintains a list of observers, and notifies of any changes. It is often used to implement event handling systems.  

There are two templated classes:-  

## observer
Derive observers from this class.  

**C++03**  
Up to eight different notification messages may be defined.

```cpp
template <typename T1,        typename T2 = void, typename T3 = void, typename T4 = void,
          typename T5 = void, typename T6 = void, typename T7 = void, typename T8 = void>
class observer
```

---


**C++11 and above**  
Any number of notification messages may be defined.  

```cpp
template <typename... Types>
class observer
```

---

`observer` defines a pure virtual notification member function for each defined type, taking the type as a parameter.  
The parameter type may be `void` (Since `20.39.0`).  
Derived classes are forced to implement these functions.  

**Example**  

```cpp
// An observer type that handles int (by value), std::string (by reference)
// and std::complex (by const reference).
typedef etl::observer<int, std::string&, const std::complex&> observer_type;

class my_observer : public observer_type
{
public:
  void notification(int i);
  void notification(std::string& s);
  void notification(const std::complex);
};
```

## observable
This class is observable from a class derived from `observer`.  
Derive observable classes from this.

```cpp
template <typename TObserver, const size_t MAX_OBSERVERS>
class observable
```

```cpp
void add_observer(TObserver& observer)
```
**Description**
Adds an observer to the list. If the maximum number of observers have already been added then an `etl::observer_list_full` is emitted.

---

```cpp
void remove_observer(TObserver& observer)
```
Remove an observer from the list. If the observer is not in the list then there is no change.  
Do not call this from within a notification override.

---

```cpp
void enable_observer(TObserver& observer, bool state = true)
```
Enables / disables an observer according to the parameter value.  
The default is `true`.  
May be called from a notification override.

---

```cpp
void disable_observer(TObserver& observer)
```
Disables an observer from being called from notify_observers.  
May be called from a notification override.

---

```cpp
void clear_observers()
```
Removes all observers from the list.

---

```cpp
size_type number_of_observers() const
```
Returns the number of observers currently in the list.

---

```cpp
template <typename TNotification>
void notify_observers(TNotification data)
```
By default, the parameter is passed by as the actual data.  
To pass by reference or rvalue reference, add the type as a template parameter.  
```cpp
notify_observers<const MyData&>(data);
```
