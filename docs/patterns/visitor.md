---
title: "visitor"
---

{{< callout type="info">}}
  Header: `visitor.h`  
{{< /callout >}}

A set of template classes to enable the easy creation of objects using the Visitor pattern.  
The purpose of these classes is to create a base classes with pure virtual functions for each supplied type.   
Any derived class that tries to instantiate an object from it will then be forced to supply an overridden version for each and every function.  

There are two templated classes; `visitor` and `visitable`.

## visitable
The visitable base class.  

Classes derived from this are visitable by the types declared in the template parameter list. A pure virtual `accept` function will be created for each visitor type.  

**C++03**  
Up to four visitor types may be specified. 

```cpp
template <typename T1, typename T2 = void, typename T3 = void, typename T4 = void>
class visitable;
```

**C++11 and above**  
Any number of visitor types may be specified.

```cpp
template <typename... Types>
class visitable;
```

## visitor
The visitor base class.  
Classes derived from this can be passed to objects derived from `visitable`.  
A pure virtual `visit` function will be created for each specified type.  

**C++03**  
 Up to sixteen types may be specified in the template parameter list

```cpp
template <typename T1,         typename T2  = void, typename T3  = void, typename T4  = void,
      0    typename T5  = void, typename T6  = void, typename T7  = void, typename T8  = void,
          typename T9  = void, typename T10 = void, typename T11 = void, typename T12 = void, 
          typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
class visitor;
```

**C++11 and above**
Any number of types may be specified.  

```cpp
template <typename... Types>
class visitor;
```

## is_visitor
Detects whether a type is a visitor.  
Since: `20.37.0`.

```cpp
template <typename T>
struct is_visitor;
```
Derived from `etl::bool_constant`

**C++17 and above**  
```cpp
template <typename T>
struct is_visitor_v;
```

## Example

Before: `20.37.0`
```cpp
All arguments are passed by reference.

class Square;
class Circle;
class Triangle;

using ShapeVisitor = etl::visitor<Square, Circle, Triangle>;

class Shape : public etl::visitable<ShapeVisitor>
{
  // Shape based objects are visitable by a ShapeVisitor
};

class Square : public Shape
{
};

class Circle : public Shape
{
};

class Triangle : public Shape
{
};

class Visitor : public ShapeVisitor
{
public:

  void visit(Square& s) override
  {
    // Square passed by reference
  }

  void visit(Circle& c) override
  {
    // Square passed by reference
  }

  void visit(Triangle& t) override
  {
    // Square passed by reference
  }
};

Visitor visitor;

Square   square;
Circle   circle;
Triangle triangle;

square.accept(visitor);   // visitor's visit(Square) is called.
circle.accept(visitor);   // visitor's visit(Circle&) is called.
triangle.accept(visitor); // visitor's visit(const Triangle&) is called.
```

---

Since: `20.37.0`  
Arguments are passed according to the template parameters.

```cpp
class Square;
class Circle;
class Triangle;

using ShapeVisitor = etl::visitor<Square&, Circle&, const Triangle&>;

class Shape : public etl::visitable<ShapeVisitor>
{
  // Shape based objects are visitable by a ShapeVisitor
};

class Square : public Shape
{
};

class Circle : public Shape
{
};

class Triangle : public Shape
{
};

class Visitor : public ShapeVisitor
{
public:

  void visit(Square s) override
  {
    // Square passed by value
  }

  void visit(Circle& c) override
  {
    // Square passed by reference
  }

  void visit(const Triangle& t) override
  {
    // Square passed by const reference
  }
};

Visitor visitor;

Square   square;
Circle   circle;
Triangle triangle;

square.accept(visitor);   // visitor's visit(Square) is called.
circle.accept(visitor);   // visitor's visit(Circle&) is called.
triangle.accept(visitor); // visitor's visit(const Triangle&) is called.
```
