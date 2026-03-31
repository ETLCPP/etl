---
title: "Contributing to the ETL"
weight: 6
---

Thanks for considering a contribution! Here’s what you need to know before opening a pull request:

If you are thinking of adding a new feature then raise this on the GitHub Issues page for discussion as the maintainers and users of the ETL may have questions or suggestions.
It is possible that the maintainer of the ETL or another contributor is already working on the same or a related feature.  
  
Take a look through our current issues and see if anything sparks your interest!  

Although the majority of the library has been written by me, I'm always open to contributions from other people. There are always a number of features and enhancements that either I'd like to see, or have been suggested by other users, but I only have a finite amount of time to give.

I accept contributions both big and small; If you have an idea for a funky non-standard container that you think would complement the library, then let us know. If you just want to add an enhancement to an existing class, tell us about that too.

This is a very friendly project and I'm not going to start shouting at you if I see issues with your idea or code. If there is a problem then we can have a dialog about what should be changed.  
Feel free to argue your case.

There are always a number of issues outstanding that could usefully be taken up whole, or in part, by someone else.

## Project Style
I care about code style and will flag issues in review, but I promise to be constructive about it, not aggressive. With that said, here are a few design rules I ask you to adhere to.

### Public API
The style of the public API of the class should mirror that of the STL.

### Class and function names
Lower case class and function names, separated by an underscore.
`funky_container`  
`do_something`

### Nameing convension
Use the same naming convention as the STL.
If appropriate for the class, a set of `typedef`s should often be present to define types such as...

`type`  
`value`  
`value_type`  
`size_type`  
`pointer`  
`const_pointer`  
`reference`  

If the code is only for C++11 and above you may use the more modern `using` syntax.

### Similarity to the STL
If a function does something similar to that in a standard class in the STL then use the same terminology.

`push_back`  
`insert_after`  
`erase`  
`assign`  
`clear`  
`size`  

### C++ standards
If a feature is only implementable for a certain C++ standard or above, wrap the code in

`#if #endif`, with one of the following:-  
`ETL_USING_CPP11` For C++11 or above  
`ETL_USING_CPP14` For C++14 or above  
`ETL_USING_CPP17` For C++17 or above  
`ETL_USING_CPP20` For C++20 or above  
`ETL_USING_CPP23` For C++23 or above  
  
i.e.
```cpp
#if ETL_USING_CPP11
...
#endif
```

### Macros
Macros should use `THIS_STYLE`.  

### Constants
Constants should use `This_Style`.  

### Namespace
All ETL classes are to be within the `etl` namespace.

### Private functionality
All classes private to the ETL should reside in a nested namespace prefixed with `private_`

```cpp
namespace etl
{
  namespace private_funky_container
  {
    struct helper
    {
      //…
    };
  }

  class funky_container
  {
    private_funky_container::helper helper;
  };
}
```

### Naming
Use descriptive names for internal variables and functions.

### Comments
Use comments to summarise or explain blocks of code that may be difficult to quickly understand.

### 8 bit types
Avoid using `int8_t` or `uint8_t`.
Some processors such as DSPs do not support 8 bit values. Use `int_least_8_t` and `uint_least8_t` in their place.
If `int8_t` or `uint8_t` must be used then surround the code with
```cpp
#if ETL_USING_8BIT_TYPES
#endif
```

### 64 bit types
If `int64_t` or `uint64_t` must be used 
Then surround the code with
```cpp
#if ETL_USING_64BIT_TYPES
#endif
```

### Bit lengths
Never assume the bit length of a type.  
If the bit length is important then either use a type from stdint.h or interrogate the type for its size.

### Mininmum and maximum values
Never assume the minimum or maximum values for a type.  
Use `etl::numberic_limits` or `etl::inyegral_limits` to look them up.

### Compile time asserts
Where ever possible, add `ETL_STATIC_ASSERT` to catch errors at compile time, rather than runtime.

### Run time asserts
Use the ETL macros to report runtime errors.  
This will ensure that the correct code is generated for the user selected error reporting method.

`ETL_ASSERT(!full(), ETL_ERROR(stack_full));`

### Constructors
Align the initialisation of member variables in constructors as below.

```cpp
class my_new_class
{
  public:

  my_new_class(int a_, int b_, int c_)
    : a(a_)
    , b(b_)
    , c(c_)
  {
  }
};
```

### Layout
When possible, try to tabluate the initialisation of multiple variables. It's a lot easier to read.
Don't be too strict about it, if it would result in giant amounts of whitespace.

```cpp
int              count   = 0;
double           divisor = 1.234;
etl::string<10>  name    = "Default";
etl::string_view view(name, begin(), name.end());
etl::intrusive_forward_list<data_link_type> quite_long_variable_name(data1, data2, data3);
```

### ETL errors
ETL errors are packaged in exception structures. Do not get confused with this terminology, it does not mean that it will *always* be thrown as an exception. It may just be passed as a parameter to an error handler.

```cpp
class stack_exception : public exception
{
public:

  stack_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
    : exception(reason_, file_name_, line_number_)
  {
  }
};

class stack_full : public stack_exception
{
public:

  stack_full(string_type file_name_, numeric_type line_number_)
    : stack_exception(ETL_ERROR_TEXT("stack:full", ETL_STACK_FILE_ID"A"), file_name_, line_number_)
  {
  }
};
```

Define the file id in `file_error_numbers.h`

`#define ETL_STACK_FILE_ID "33"`

## Unit Tests

Every class or function that is in the library must be unit tested. Do not submit your code unless you have a full set of tests that exercise as much of the functionality as you can possibly think of. Make sure that you cover all of the corner cases.

There is a project file for VS2022 for C++14, 17, 20, 23, and bash scripts that run the tests for C++11, 14, 17, 20, 23 under Linux with GCC and Clang.

## Bash Scripts

There are bash scripts to test the ETL under Linux.
These scripts are run from the test directory.

| `run-tests.sh` | Runs the set of unit tests with various options and compilers.                 |
| -------------- | ------------------------------------------------------------------------------ |
| Syntax         | `./runtests.sh <C++ Standard> <Optimisation> <Threads> <Sanitizer> <Compiler>` |
| C++ Standard   | 11, 14, 17, 20, or 23 (mandatory argument)                                     |
| Optimisation   | 0, 1, 2 or 3. Default = 0                                                      |
| Threads        | Number of threads to use. Default = 4                                          |
| Sanitizer      | s enables sanitizer checks, n disables. Default disabled                       |
| Compiler       | gcc or clang. Default All compilers                                            |

| `run-syntax-checks.sh` | Runs the set of header files through the compilers to test syntax correctness. |
| ---------------------- | ------------------------------------------------------------------------------ |
| Syntax                 | `./runtests.sh <C++ Standard> <Threads>`                                       |
| C++ Standard           | a, 03, 11, 14, 17, 20, or 23. a = All standards                                |
| Threads                | Number of threads to use. Default = 4                                          |
| Compiler               | gcc or clang. Default = All compilers                                          |

## Pull Requests

If you wish to create a pull request to the ETL repository on GitHub then create your changes in a branch that is based on `master`. When you think you have completed your modifications then raise a pull request.

When opening a new pull request, ensure that you include the following information.
- Is this brand new functionality, an enhancement to existing code?
- If it is a bug fix, What problem are you solving? Give an example of how to reproduce it.
- If it is a new feature or enhancement, what will be the benefit to the ETL or user of the library?
- If you are adding or modifying a feature, add *new* unit tests that test that feature.
- If you are fixing a bug, add a unit test that *fails* before the bug fix is implemented.
- Do not initiate a pull request until *all* of the units tests pass. See above for information on project files and test scripts.
- Branches should be based on the branch `master`.  
`development` can change quite frequently, so I will rebase the your PR against it before merging.
- For formatting help, you can use `clang-format`, or the convenience wrapper `treefmt`. See also [Source formatting]({{% ref "source-formatting.md" %}}).
- Update the relevent documentation markdown file, found in `etl/docs`.