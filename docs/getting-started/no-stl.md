---
title: "No STL"
weight: 2
---

It is possible to use the ETL without any reliance on the STL — 
This can be achieved by defining the project wide macro `ETL_NO_STL`.  

When this macro is defined then the ETL will used its own reverse engineered versions of traits and algorithms.
These traits and algorithms are available to the user of the ETL, regardless of whether the STL is used or not.  

When the STL is used, then, for the most part, the ETL will merely act as a wrapper around the STL definitions.  

For STL/ETL compatibility, some algorithms and classes in the ETL will use an STL or ETL defined version of a class. For example, if you are using the STL then a function using or returning a pair will use a std::pair.On the other hand, when `ETL_NO_STL` is defined, an `etl::pair` is used.  

If you wish your code to be able to compiler both with and without the STL, then you would use the `ETL_OR_STD` namespace macro, which resolves to either `std` or `etl`.  

If the C++ level of your compiler does not support a particular algorithm (e.g. C++03 compiler and `std::sorted_until`), then the ETL will revert to its own version.  

## Using pair, move, iterator tags or reverse_iterator + others  
When `ETL_NO_STL` is defined then the ETL will use its own versions, otherwise the STL versions will be used. This will affect the return types of certain functions.e.g. `etl::equal_range` will return `ETL_OR_STD::pair` which resolves to either `std::pair` or `etl::pair` ETL types and functions which are not defined in the ETL namespace when the STL is in use :-  

`pair`  
`make_pair`  
`reverse_iterator`  
`input_iterator_tag`  
`output_iterator_tag`  
`forward_iterator_tag`  
`bidirectional_iterator_tag`  
`random_access_iterator_tag`  
`begin`  
`cbegin`  
`rbegin`  
`crbegin`
`end`  
`cend`  
`rend`  
`crend`  
`size`  
`swap`  

## Note 
The ETL is not intended to be a complete clone of the STL.  
The ETL's version of STL features is not exhaustive, and many features are missing.  

That said, if there is an STL feature missing from the No STL option, then feel free to add it yourself and generate a pull request.
