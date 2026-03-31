---
title: "Visitor"
---

A tutorial on how the ETL's visitor pattern can be used.

## The Classes
```cpp
etl::visitor
etl::visitable
```

## Example
I'll use the familiar 'Shape' example.

First, you create the base for your 'Shape' visitor.

```cpp
//*****************************************************************
// Pre-declare the shapes.
//*****************************************************************
class Square;
class Circle;
class Triangle;

//*****************************************************************
// The shape visitor base class.
// Pure virtual 'visit' functions will be defined for the Square,
// Circle, and Triangle types.
//*****************************************************************
class Shape_Visitor : public etl:visitor<Square, Circle, Triangle>
{
};
```

Then, you define the Shape base class. It derives from the `etl::visitable` class that defines a pure virtual accept function that accepts a `Shape_Visitor`.

```cpp
//*****************************************************************
// The shape base class.
//*****************************************************************
class Shape : public etl::visitable<Shape_Visitor>
{
};
```

Next, you define the shapes `Square`, `Circle`, and `Triangle`. Each overrides the accept function that calls the visitor with itself as a parameter.

```cpp
//*****************************************************************
// The square class
//*****************************************************************
class Square : public Shape
{
  void accept(Shape_Visitor &visitor)
  {
    visitor.visit(*this);
  }
};

//*****************************************************************
// The circle class
//*****************************************************************
class Circle : public Shape
{
    void accept(Shape_Visitor &visitor)
    {
        visitor.visit(*this);
    }
};

//*****************************************************************
// The triangle class
//*****************************************************************
class Triangle : public Shape
{
  void accept(Shape_Visitor &visitor)
  {
    visitor.visit(*this);
  }
};
```

Now that you have the framework in place, you can do something with it. Here's an example that creates `Draw` and `Serialise` visitors and applies them to a vector of `Shape` objects.

```cpp
//*****************************************************************
// The 'draw' visitor.
//*****************************************************************
class Draw_Visitor : public Shape_Visitor
{
public:
  void visit(Square &square)
  {
    std::cout << "Draw the square\n";
  }

  void visit(Circle &circle)
  {
    std::cout << "Draw the circle\n";
  }

  void visit(Triangle &triangle)
  {
    std::cout << "Draw the triangle\n";
  }
};

//*****************************************************************
// The 'serialise' visitor.
//*****************************************************************
class Serialise_Visitor : public Shape_Visitor
{
public:
  void visit(Square &square)
  {
    std::cout << "Serialise the square\n";
  }

  void visit(Circle &circle)
  {
    std::cout << "Serialise the circle\n";
  }

  void visit(Triangle &triangle)
  {
    std::cout << "Serialise the triangle\n";
  }
};

//*****************************************************************
// The actual visitors.
//*****************************************************************
Draw_Visitor      draw_visitor;
Serialise_Visitor serialise_visitor;

//*****************************************************************
// The list of shapes.
//*****************************************************************
std::vector<Shape*> shape_list;

//*****************************************************************
// The Apply a visitor.
//*****************************************************************
void Apply(Shape_Visitor &visitor)
{
  for (auto pShape : shape_list)
  {
    // Send the visitor to the shape.
    pShape->accept(visitor);
  }  
}

//*****************************************************************
// Main
//*****************************************************************
int main()
{
  // Create some shapes.
  Square   square;
  Circle   circle;
  Triangle triangle;

  // Add them to the vector
  shape_list.push_back(&square);
  shape_list.push_back(&circle);
  shape_list.push_back(&triangle);

  // Apply the visitors.
  Apply(draw_visitor);
  Apply(serialise_visitor);

  return 0;
}
```

## Output
The output from the example is as follows.

```
Draw the square  
Draw the circle  
Draw the triangle  
Serialise the square  
Serialise the circle  
Serialise the triangle  
```

