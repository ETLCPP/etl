---
title: "successor"
---

{{< callout type="info">}}
  Header: `successor.h`  
{{< /callout >}}

Adds successor traits to a derived class.  
This template adds the ability for a class to store a successor to itself which allows the [Chain of Responsibility](https://refactoring.guru/design-patterns/chain-of-responsibility) design pattern to be implemented in a consistent fashion.  

**Note:**  
The successors do not have to be derived from `etl::successor`, but the member functions `append_successor` and `clear_successor_chain` cannot be used if they are not.  

```cpp
etl::successor<typename T>
```
`T` The successor type.


## Member types
```cpp
successor_type T
```

## Constructors
```cpp
successor()
```
Constructs a default successor.  
`has_successor()` will return `false`.

---

```cpp
successor(successor_type& s)
```
Constructs a successor from the supplied parameter `s`.  
`has_successor()` will return `true`.

## Member functions
```cpp
void set_successor(successor_type& s)
```
Sets the successor to `s`.  
Overwrites any previous successor.  
`has_successor()` will return `true`.

---

```cpp
template <typename... TSuccessors>
void set_successor(successor_type& s, TSuccessors&... rest)
```
Sets a series of successors to `s`.  
Overwrites any previous successor.  
Since `20.28.0`, C++11.

---

```cpp
template <typename TSuccessor>
void append_successor(TSuccessor& s)
```
Appends the successor to the end of the successor chain.  
Only valid to call if all successors are derived from `etl::successor`.  
Since `20.28.0`.

---

```cpp
template <typename TSuccessor, typename... TSuccessors>
void append_successor(TSuccessor& s, TSuccessors&... rest)
```
Appends the list of successors to the end of the successor chain.  
Only valid to call if all successors are derived from `etl::successor`.
Since `20.28.0`, C++11.

---

```cpp
successor_type& get_successor() const
```
Gets a reference to the successor.  
Undefined behaviour if a successor has not been set.

---

```cpp
bool has_successor() const
```
Returns `true` if a successor has been set.  

---

```cpp
void clear_successor()
```
Clears the successor.  
`has_successor()` will return `false`.  
Since `20.28.0`.

---

```cpp
void clear_successor_chain()
```
Clears the successor for every successor in the chain starting from this successor.  
Only valid to call if all successors are derived from `etl::successor`.  
Since `20.28.0`.

## Error types
`successor_exception` Derived from `etl::exception`
`successor_invalid`   Derived from `successor_exception`


## Example
```cpp
class IProcessor : public etl::successor<IProcessor>
{
  public:

  //*********************************
  virtual bool Check() = 0;

  //*********************************
  // Step though the successors until either the 
  // call is handled, or there are no successors left.
  bool Process()
  {
    bool handled = false;

    IProcessor* p = this;

    while (handled == false)
    {
      handled = p->Check();

      // Were we unable to handle call?
      if (handled == false)
      {
        // Do we have a successor?
        if (p->has_successor())
        {
          // Get the next successor.
          p = &(p->get_successor());
        }
        else
        {
          // We have reached the end of the chain.
          break;
        }
      }
    }

    return handled;
  }
};

//*********************************
class Processor1 : public IProcessor
{
  public:

  bool Check() override
  {
    return false;
  }
};

//*********************************
class Processor2 : public IProcessor
{
  public:

  bool Check() override
  {
    return false;
  }
};

//*********************************
class Processor3 : public IProcessor
{
  public:

  bool Check() override
  {
    return false;
  }
};

//*********************************
class Processor4 : public IProcessor
{
public:

  bool Check() override
  {
    return true;
  }
};

Processor1 processor1a;
Processor1 processor1b;
Processor2 processor2a;
Processor2 processor2b;
Processor3 processor3;
Processor4 processor4;

processor1a.append_successor(processor2a, processor3);
processor1b.append_successor(processor2b, processor4);

bool b1a = processor1a.Process(); // Returns false, nothing handles the process call.
bool b1b = processor1b.Process(); // Returns true, processor4 handles the process call.
```
