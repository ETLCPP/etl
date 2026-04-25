---
title: "Messages"
---

{{< callout type="info">}}
  Header: `message.h`  
{{< /callout >}}

Message types used for many of the framework classes.

---

```cpp
etl::message_id_t
```
The type used for message ids.  
By default can hold a value between 0 and 255.  
If `ETL_MESSAGE_ID_TYPE` is defined then this type will be used instead.

---

```cpp
etl::message_router_id_t
```
The type used for message router ids.  
Can hold a value between 0 and 255.

---

The message classes are the common communication method across all of the message capable frameworks.
They are identified by a unique id number that specialises the base class.

## imessage

The base class for messages. 
It is this class that is passed around, usually by const reference.  
The class is abstract.  

---

```cpp
etl::message_id_t get_message_id() const ETL_NOEXCEPT = 0;
```
Returns the id of the message.

---

## message

```cpp
message<size_t ID, typename TParent = etl::imessage>
```
Requires an integral id as the template parameter.  
Inherits from `TParent`, which defaults to `etl::imessage`.  
`TParent` allows additional base interfaces or  functionality to be included.  
static asserts if `TParent` is not a base of `etl::imessage`.

---

```cpp
ID
```
The id of the message as an enum.
Can be accessed by `etl::message` instances.

---

```cpp
TParent
```
The class that it inherits from. It must ultimately derive from `etl::imessage`.  
The default is `etl::imessage`.

---

## Example

```cpp
enum
{
  START,
  STOP,
  SET_SPEED
};

struct MyInterface : public etl::imessage
{
  virtual void DoStuff() = 0;
};

// Start implements MyIterface
struct Start : public etl::message<START, MyInterface>
{
  void DoStuff() override
  {
    // Do stuff here.  
  }
};

struct Stop : public etl::message<STOP>
{
  bool isEmergencyStop;
};

struct SetSpeed : public etl::message<SET_SPEED>
{
  uint32_t speed;
};

void Receive(const etl::imessage& msg);
```
