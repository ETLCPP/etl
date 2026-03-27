---
title: "Embedded Template Library"
---

---

{{< callout type="warning">}}
**This documentation is still under development, and very much incomplete.**
{{< /callout >}}

{{< callout emoji="❓">}}
 [***Is the ETL free?***]({{< ref "Is the ETL free.md" >}})
{{< /callout >}}

## Motivation
 

C++ is a powerful language for embedded systems development, with templates offering a great deal of flexibility and type safety. While the C++ Standard Library provides a wealth of well-tested functionality, it’s often not well suited to environments with strict deterministic behavior and limited resources.  

In many embedded applications, dynamic memory allocation is discouraged or outright prohibited, making standard STL containers and many other components impractical or unusable.  

What’s needed is a template library specifically designed for embedded systems — one that allows developers to define fixed or maximum sizes for containers and other objects at compile time. Additionally, since many older (but still in use) embedded toolchains lack full support for standards beyond C++03, it's valuable to have access to a library that backports select features from later versions of the C++ Standard Library.  

## About the ETL
 

The Embedded Template Library (ETL) is not intended as a full replacement for the C++ Standard Template Library (STL), but rather as a complementary solution tailored specifically for embedded systems.  

### Its design goals include:

- Providing a set of containers with fixed or maximum sizes defined at compile-time.
- Offering APIs that closely resemble those of the STL, enabling familiar and consistent usage.
- Maintaining compatibility with C++98 while implementing many features introduced in later standards
(C++11/14/17/20/23) where possible.
- Ensuring deterministic behavior, which is critical in real-time and resource-constrained environments.
- Introducing additional components and utilities useful in embedded contexts but absent from the STL.
- The ETL avoids dynamic memory allocation entirely; the heap is never used. All non-intrusive containers have a fixed capacity, allowing memory requirements to be fully determined at compile-time. This makes the ETL ideal for lower-resource embedded applications where predictability, performance, and memory control are essential.

The library is compatible with any compiler that supports C++03 or later.

Help on integrating the ETL with your project may be found here.

## Key Features of the ETL
 
- Actively Maintained: Developed and maintained on GitHub since 2014.
- Open Source: MIT licensed.
- No STL Dependency: Designed to operate independently of the C++ Standard Template Library.
- No Dynamic Memory Allocation: All storage is allocated either at compile-time or on the stack; heap usage is entirely avoided.
- RTTI and Virtual Functions: No runtime type information (RTTI) is required. Virtual functions are used sparingly and only when strictly necessary.
- Header-Only Library: All functionality is provided via header files; No separate compilation needed.
- Fixed-Capacity Containers: Offers STL-like containers with fixed or maximum capacity, plus additional non-standard container types.
- Cache Efficiency: Containers use contiguous memory layouts for optimal cache performance.
- Compact Codebase: Shared base classes (based on type) help reduce overall container code size.
- Compile-Time Features:  
Templated compile-time constants
Template-based design pattern base classes (e.g., Visitor, Observer)
Type-safe smart enumerations
Type-safe typedefs and constants
- Embedded System Frameworks:  
Message routing  
Finite state machines  
Task scheduling  
- C++11 Backports:  
Implements many C++11 features (type traits, algorithms, containers) for use in C++03 environments.
- Utilities:  
CRC calculations (8, 16, 32 & 64-bit)  
Checksums and hash functions
- Variants (type-safe unions)
- Extensive template support utilities
- Robust Error Handling: Configurable error checking using asserts, exceptions, error handlers, or no checks; user’s choice.
- Thoroughly Tested:  
Over 10,000 unit tests
Tested with Visual Studio 2022, GCC 12, and Clang 14
Continuous integration via GitHub Actions.
- Readable and Well-Documented: Clean, maintainable source code with clear documentation.
- Support: Free email support available. A Slack group is available. Paid support on request.
- Archived: A snapshot of the ETL is preserved in the Arctic Code Vault for long-term digital preservation.

## Support the ETL
Maintaining the ETL can take a lot of man-hours of work, but unfortunately it doesn't pay the bills. When I have to take on paying work, the ETL gets a lot less attention. So if you have found the library is an important component in your work and you would like to help out, then please consider by supporting the project.

[**Is the ETL free?**]({{< relref "is the ETL free.md" >}})

Any help porting the library to work under different platforms and compilers would be gratefully received.  
I am especially interested in people who are using Keil, IAR, Green Hills, TI Code Composer etc, bare metal
or RTOS, and DSPs.

Many thanks.  
John.
