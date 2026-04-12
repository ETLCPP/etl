---
title: "Macros"
---

Many of the features or options in the ETL can be selected by defining the appropriate macros.
Some macros are defined by the ETL.

Some of the following macros are presented as C++ constants.
See ETL Traits

## User defined

These may be defined in the project settings or the user created `etl_profile.h`.

| Macro                                         |Description                                                     |
| --------------------------------------------- | -------------------------------------------------------------- |
| `ETL_NO_CHECKS`                               | `ETL_ASSERT` has no effect.                                      |
| `ETL_THROW_EXCEPTIONS`                        | `ETL_ASSERT` throws the specified exception.                     |
| `ETL_THROW_EXCEPTIONS` + `ETL_LOG_ERRORS`     | `ETL_ASSERT` calls the error handler then throws an exception.   |
| `ETL_LOG_ERRORS`                              | `ETL_ASSERT` calls the error handler then asserts.               |
| `ETL_LOG_ERRORS` + `NDEBUG`                   | `ETL_ASSERT` calls the error handler.                            |
| `ETL_CHECK_PUSH_POP`                            | Pushes and pops to containers are checked for bounds.
| `ETL_VERBOSE_ERRORS`                            | If this is defined then error messages are output in their long form. |
| `ETL_BITSET_ELEMENT_TYPE`                       | If this is defined, then it will become the type used for elements in the `bitset` class.<br/>Default is `uint_least8_t` |
| `ETL_FSM_STATE_ID_TYPE`                         | If this is defined, then it will become the type used for FSM state id numbers.<br/>`Default is uint_least8_t`.
| `ETL_MESSAGE_ID_TYPE`                           | If this is defined, then it will become the type used for message id numbers.<br/>`Default is uint_least8_t`.
| `ETL_TIMER_SEMAPHORE_TYPE`                      | If this is defined, then it will become the type used for the type for the timer guard variable.<br/>This must be a type that cannot be interrupted during a read/modify/write cycle.<br/>Default is `etl::atomic_uint32_t`. |
| `ETL_ISTRING_REPAIR_ENABLE`                     | Define this if you wish to `memcpy` ETL strings and repair them via an `istring` pointer or reference.<br/>Warning: This will make the container a virtual class.|
| `ETL_IVECTOR_REPAIR_ENABLE`                     | Define this if you wish to memcpy ETL vectors and repair them via an `ivector` pointer or reference.<br/>Warning: This will make the container a virtual class.|
| `ETL_IDEQUE_REPAIR_ENABLE`                      | Define this if you wish to memcpy ETL deques and repair them via an `ideque` pointer or reference.<br/>Warning: This will make the container a virtual class.|
| `ETL_STLPORT`                                   | This must be defined in the user library profile when using STLPort as the standard library implementation.|
| `ETL_NO_STL`                                    | If defined, the ETL will not use definitions from the STL.<br/>Instead it will use its own reverse engineered versions.|
| `ETL_FORCE_EXPLICIT_STRING_CONVERSION_FROM_CHAR` | If defined, the ETL will force `string`, `wstring`, `u8string`, `u16string`, `u32string`, and `string_view` to have explicit construction from a character pointer.|
| `ETL_STRING_TRUNCATION_IS_ERROR`                | If defined, then a string truncation will result in an `etl::string_truncation` error being emitted.|
| `ETL_ENABLE_ERROR_ON_STRING_TRUNCATION`      | See above. |
| `ETL_ARRAY_VIEW_IS_MUTABLE`                    | If defined, then `etl::array_view` is mutable. |
| `ETL_POLYMORPHIC_BITSET`                     | Defining any one of these will make the corresponding container polymorphic, turning the protected non-virtual destructor to public virtual.|
| `ETL_POLYMORPHIC_DEQUE`        | |                   
| `ETL_POLYMORPHIC_FLAT_MAP`       | |                 
| `ETL_POLYMORPHIC_FLAT_MULTIMAP` | |
| `ETL_POLYMORPHIC_FLAT_SET` | |
| `ETL_POLYMORPHIC_FLAT_MULTISET` | |
| `ETL_POLYMORPHIC_FORWARD_LIST` | |
| `ETL_POLYMORPHIC_LIST` | |
| `ETL_POLYMORPHIC_MAP` | |
| `ETL_POLYMORPHIC_MULTIMAP` | |
| `ETL_POLYMORPHIC_SET` | |
| `ETL_POLYMORPHIC_MULTISET` | |
| `ETL_POLYMORPHIC_QUEUE` | |
| `ETL_POLYMORPHIC_STACK` | |
| `ETL_POLYMORPHIC_REFERENCE_FLAT_MAP` | |
| `ETL_POLYMORPHIC_REFERENCE_FLAT_MULTIMAP` | |
| `ETL_POLYMORPHIC_REFERENCE_FLAT_SET` | |
| `ETL_POLYMORPHIC_REFERENCE_FLAT_MULTISET` | |
| `ETL_POLYMORPHIC_UNORDERED_MAP` | |
| `ETL_POLYMORPHIC_UNORDERED_MULTIMAP` | |
| `ETL_POLYMORPHIC_UNORDERED_SET` | |
| `ETL_POLYMORPHIC_UNORDERED_MULTISET` | |
| `ETL_POLYMORPHIC_STRINGS` | |
| `ETL_POLYMORPHIC_POOL` | |
| `ETL_POLYMORPHIC_VECTOR` | |
| `ETL_POLYMORPHIC_CONTAINERS` | If defined then all containers are polymorphic.|
| `ETL_POLYMORPHIC_MESSAGES` | If defined then `etl::imessage` is virtual. |
| `ETL_MESSAGES_ARE_VIRTUAL` | `ETL_MESSAGES_ARE_VIRTUAL` is deprecated and may be removed.<br/>Only valid before 19.4.1<br/>Messages are virtual, by default, from 19.4.1|
| `ETL_USE_TYPE_TRAITS_BUILTINS` | Forces the ETL to use calls compiler built-ins.<br/>Sets all of them to be 1 if not already defined.<br/>If not defined `ETL_USE_BUILTIN_IS_ASSIGNABLE` then<br/>`ETL_USE_BUILTIN_IS_ASSIGNABLE = 1`<br/><br/>If not defined ETL_USE_BUILTIN_IS_CONSTRUCTIBLE then<br/>`ETL_USE_BUILTIN_IS_CONSTRUCTIBLE = 1`<br/><br/>If not `ETL_USE_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE` then<br/>`ETL_USE_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE = 1`<br/><br/>If not defined `ETL_USE_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE` then<br/>`ETL_USE_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE = 1`<br/><br/>If not defined `ETL_USE_BUILTIN_IS_TRIVIALLY_COPYABLE` then<br/>`ETL_USE_BUILTIN_IS_TRIVIALLY_COPYABLE = 1`<br/><br/>`ETL_TARGET_DEVICE_GENERIC`<br/>Only `ETL_TARGET_DEVICE_ARM_CORTEX_M0` and `ETL_TARGET_DEVICE_ARM` and `ETL_TARGET_DEVICE_ARM_CORTEX_M0_PLUS` are currently used in the ETL code to disable `etl::atomic`.|
| `ETL_NO_LIBC_WCHAR_H` | Define if the libc++ used has not been compiled for `wchar_t` support.|

## ETL defined
**Defined in platform.h**  

| Macro                                |Description                                                     |
| ------------------------------------ | -------------------------------------------------------------- |
| `ETL_DEBUG`                          | This is defined as `1` if `DEBUG` or `_DEBUG` is defined. Otherwise `0`.|
| `ETL_8BIT_SUPPORT`                   | This is defined as `1` if the platform supports 8 bit char types, otherwise 0.<br/>Deprecated.|
| `ETL_CONSTEXPR`                      | If `ETL_CPP11_SUPPORTED` is defined as `1` then this macro is defined as `constexpr`, otherwise defined as blank.|
| `ETL_CONSTEXPR11`                    | If `ETL_CPP11_SUPPORTED` is defined as `1` then this macro is defined as `constexpr`, otherwise defined as blank.|
| `ETL_CONSTEXPR14`                    | If `ETL_CPP14_SUPPORTED` is defined as `1` then this macro is defined as `constexpr`, otherwise defined as blank.|
| `ETL_CONSTEXPR17`                    | If `ETL_CPP17_SUPPORTED` is defined as `1` then this macro is defined as `constexpr`, otherwise defined as blank.|
| `ETL_IF_CONSTEXPR`                   | If `ETL_CPP17_SUPPORTED` is defined as `1` then this macro is defined as `constexpr`, otherwise defined as blank.|
| `ETL_CONSTANT`                       | If `ETL_CPP11_SUPPORTED` is defined as `1` then this macro is defined as `constexpr`, otherwise defined as `const`.|
| `ETL_NOEXCEPT`                       | If `ETL_CPP11_SUPPORTED` is defined as `1` then this macro is defined as `noexcept`, otherwise defined as blank.|
| `ETL_NOEXCEPT_EXPR(expression)`      | If `ETL_CPP11_SUPPORTED` is defined as `1` then this macro is defined as `noexcept(expression)`, otherwise defined as blank.|
| `ETL_NODISCARD`                      | If `ETL_CPP11_SUPPORTED` is defined as `1` then this macro is defined as `[[nodiscard]]`, otherwise defined as blank.|
| `ETL_DEPRECATED`                     | If `ETL_CPP14_SUPPORTED` is defined as `1` then this macro is defined as `[[deprecated]]`, otherwise defined as blank.|
| `ETL_DEPRECATED_REASON(reason)`      | If `ETL_CPP14_SUPPORTED` is defined as `1` then this macro is defined as `[[deprecated(reason)]]`, otherwise defined as blank.|
| `ETL_FALLTHROUGH`                    | If `ETL_CPP17_SUPPORTED` is defined as `1` then this macro is defined as `[[falltrough]]`, otherwise defined as blank.|
| `ETL_NORETURN`                       | If `ETL_CPP11_SUPPORTED` is defined as `1` then this macro is defined as `[[noreturn]]`, otherwise defined as blank.|
| `ETL_OR_STD`                         |    If ETL_NO_STL is defined and ETL_IN_UNIT_TEST is not then `ETL_OR_STD` is defined as `etl`, otherwise it is defined as `std`.|
| `ETL_IN_UNIT_TEST`                   | If defined, then the code is being compiled in the unit tests.<br/>For internal ETL use only.|
| `ETL_HAS_ATOMIC`                     | This is defined as `1` if the compiler supplies an atomic class, otherwise `0`.|
| `ETL_INLINE_VAR`                     | If `ETL_CPP17_SUPPORTED` is defined as `1` then this macro is defined as `inline`, otherwise defined as blank.|
| `ETL_USING_STL`                      | This macro will be defined as `0` & `1` dependant of whether `ETL_NO_STL` is defined or not.|
| `ETL_NOT_USING_STL`                  | Inversion of `ETL_USING_STL`.|
| `ETL_USING_STLPORT`                  | This macro will be defined as `0` & `1` dependant of whether `ETL_STLPORT` is defined or not.|
| `ETL_NOT_USING_STLPORT`              | Inversion of `ETL_USING_STLPORT`.|
| `ETL_USING_8BIT_TYPES`               | This macro will be defined as `0` & `1` dependant of whether `CHAR_BIT == 8` or not.|   
| `ETL_NOT_USING_8BIT_TYPES`           | Inversion of `ETL_USING_8BIT_TYPES`.|
| `ETL_USING_64BIT_TYPES`              | This macro will be defined as `0` & `1` dependant of whether `ETL_NO_64BIT_TYPES` is defined or not.|
| `ETL_NOT_USING_64BIT_TYPES`          | Inversion of `ETL_USING_64BIT_TYPES`.|
| `ETL_HAS_ISTRING_REPAIR`             | Set to `1` if the repair functionality for `etl::istring` is enabled, otherwise `0`.|
| `ETL_HAS_IVECTOR_REPAIR`             | Set to `1` if the repair functionality for `etl::ivector` is enabled, otherwise `0`.|
| `ETL_HAS_IDEQUE_REPAIR`              | Set to `1` if the repair functionality for `etl::ideque` is enabled, otherwise `0`.|
| `ETL_IS_DEBUG_BUILD`                 | Set to `1` if in a debug build, otherwise `0`.|
| `ETL_HAS_POLYMORPHIC_MESSAGES`       | Set to `1` if messages are polymorphic, otherwise `0`.|
| `ETL_HAS_ERROR_ON_STRING_TRUNCATION` | Set to `1` if truncated strings are an error, otherwise `0`.|
| `ETL_USING_LIBC_WCHAR_H`             | These macros will be defined as `0` & `1` dependant of whether `ETL_NO_LIBC_WCHAR_H` is defined or not.|
| `ETL_NOT_USING_LIBC_WCHAR_H`         | Inversion of `ETL_USING_LIBC_WCHAR_H`.|
| `ETL_USING_CPP11`                    | This is defined as `1` if the compiler supports C++11, otherwise `0`.|
| `ETL_USING_CPP14`                    | This is defined as `1` if the compiler supports C++14, otherwise `0`.|
| `ETL_USING_CPP17`                    | This is defined as `1` if the compiler supports C++17, otherwise `0`.|
| `ETL_USING_CPP20`                    | This is defined as `1` if the compiler supports C++20, otherwise `0`.|
| `ETL_USING_CPP23`                    | This is defined as `1` if the compiler supports C++23, otherwise `0`.|

These may be user defined in `etl_profile.h`, or automatically determined in `platform.h`.

| Macro                                |Description                                                     |
| ------------------------------------ | -------------------------------------------------------------- |
| `ETL_CPP11_SUPPORTED`                | This is defined as `1` if the compiler supports C++11, otherwise `0`.|
| `ETL_CPP14_SUPPORTED`                | This is defined as `1` if the compiler supports C++14, otherwise `0`.|
| `ETL_CPP17_SUPPORTED`                | This is defined as `1` if the compiler supports C++17, otherwise `0`.|
| `ETL_CPP20_SUPPORTED`                | This is defined as `1` if the compiler supports C++20, otherwise `0`.|
| `ETL_CPP23_SUPPORTED`                | This is defined as `1` if the compiler supports C++23, otherwise `0`.|
| `ETL_NO_NULLPTR_SUPPORT`             | This is defined as `1` if compiler does not support `nullptr`, otherwise `0`.|
| `ETL_NO_LARGE_CHAR_SUPPORT`          | This is defined as `1` if the compiler does not support `char16_t` or `char32_t` types, otherwise `0`.|
| `ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED` | This is defined as `1` if compiler supports the `std::is_trivially_xxx` set of traits, otherwise `0`.|
|                                      | |
| `ETL_COMPILER_IAR`                   | One of these will be defined.|
| `ETL_COMPILER_GREEN_HILLS`           | |
| `ETL_COMPILER_INTEL`                 | |
| `ETL_COMPILER_MICROSOFT`             | |
| `ETL_COMPILER_GCC`                   | |
| `ETL_COMPILER_CLANG`                 | |
| `ETL_COMPILER_ARM`                   | |
| `ETL_COMPILER_TEXAS_INSTRUMENTS`     | |
| `ETL_COMPILER_GENERIC`               | |
|                                      | |
| `ETL_COMPILER_VERSION                | These will be defined as the compiler version numbers, if available.|
| `ETL_COMPILER_FULL_VERSION`          | |
|                                      | |
| `ETL_DEVELOPMENT_OS_WINDOWS`         | One of these will be defined.|
| `ETL_DEVELOPMENT_OS_LINUX`           | |
| `ETL_DEVELOPMENT_OS_UNIX`            | |
| `ETL_DEVELOPMENT_OS_APPLE`           | |
| `ETL_DEVELOPMENT_OS_BSD`             | |
| `ETL_DEVELOPMENT_OS_GENERIC`         | |
|                                      | |
| `ETL_NO_CPP_NAN_SUPPORT`             | If defined, indicates that the compiler does not support nan(), nanf() or nanl().<br/>Automatically defined if using CodeWorks for ARM.|

