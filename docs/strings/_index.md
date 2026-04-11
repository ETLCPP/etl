---
title: "Strings"
weight: 100
---

## String

The library defines a set of string templates that have been specially tailored for embedded systems.  
They have a maximum capacity fixed at compile time and make no calls to `malloc`/`free` or  `new`/`delete`0.  
They are completely deterministic.  
The ETL provides `etl::string`, `etl::wstring`, `etl::u16string` and `etl::u32string`.  

## String View

The ETL defines string view classes that provide a lost cost view into a string.  
The ETL provides `etl::string_view`, `etl::wstring_view`, `etl::u16string_view` and `etl::u32string_view`.  

## To String

The ETL defines a utility to create strings according to a format.  
The ETL provides `etl::to_string`, `etl::to_wstring`, `etl::to_u16string` and `etl::to_u32string`.  

## To Arithmetic

The ETL defines a utility to convert strings to values.

## String Stream

The ETL defines string stream  classes that stream values into a string.  
The ETL provides `etl::string_stream`, `etl::wstring_stream`, `etl::u16string_stream`, and `etl::u32string_stream`.  

## String Utilities

The ETL provides a set of string to examine and manipulate strings.  

## Format Specification

A class is defined to format the output of `etl::to_string`, `etl::to_wstring`, `etl::to_u16string`, and `etl::to_u32string`.  
