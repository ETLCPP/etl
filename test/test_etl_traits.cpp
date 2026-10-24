/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Documentation:

Copyright(c) 2022 John Wellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include "unit_test_framework.h"

#include "etl/platform.h"
#include "etl/char_traits.h"
#include "etl/mutex.h"
#include "etl/version.h"

namespace
{
  SUITE(test_etl_traits)
  {
    TEST(test_traits_values)
    {
      // Using...
      CHECK_EQUAL((ETL_USING_STL == 1), etl::traits::using_stl);
      CHECK_EQUAL((ETL_USING_STLPORT == 1), etl::traits::using_stlport);
      CHECK_EQUAL((ETL_USING_CPP11 == 1), etl::traits::using_cpp11);
      CHECK_EQUAL((ETL_USING_CPP14 == 1), etl::traits::using_cpp14);
      CHECK_EQUAL((ETL_USING_CPP17 == 1), etl::traits::using_cpp17);
      CHECK_EQUAL((ETL_USING_CPP20 == 1), etl::traits::using_cpp20);
      CHECK_EQUAL((ETL_USING_CPP23 == 1), etl::traits::using_cpp23);
      CHECK_EQUAL((ETL_USING_CPP26 == 1), etl::traits::using_cpp26);
      CHECK_EQUAL((ETL_USING_EXCEPTIONS == 1), etl::traits::using_exceptions);
      CHECK_EQUAL((ETL_USING_LIBC_WCHAR_H == 1), etl::traits::using_libc_wchar_h);
      CHECK_EQUAL((ETL_USING_GCC_COMPILER == 1), etl::traits::using_gcc_compiler);
      CHECK_EQUAL((ETL_USING_MICROSOFT_COMPILER == 1), etl::traits::using_microsoft_compiler);
      CHECK_EQUAL((ETL_USING_ARM5_COMPILER == 1), etl::traits::using_arm5_compiler);
      CHECK_EQUAL((ETL_USING_ARM6_COMPILER == 1), etl::traits::using_arm6_compiler);
      CHECK_EQUAL((ETL_USING_ARM7_COMPILER == 1), etl::traits::using_arm7_compiler);
      CHECK_EQUAL((ETL_USING_CLANG_COMPILER == 1), etl::traits::using_clang_compiler);
      CHECK_EQUAL((ETL_USING_GREEN_HILLS_COMPILER == 1), etl::traits::using_green_hills_compiler);
      CHECK_EQUAL((ETL_USING_IAR_COMPILER == 1), etl::traits::using_iar_compiler);
      CHECK_EQUAL((ETL_USING_INTEL_COMPILER == 1), etl::traits::using_intel_compiler);
      CHECK_EQUAL((ETL_USING_TEXAS_INSTRUMENTS_COMPILER == 1), etl::traits::using_texas_instruments_compiler);
      CHECK_EQUAL((ETL_USING_GENERIC_COMPILER == 1), etl::traits::using_generic_compiler);
      CHECK_EQUAL((ETL_USING_8BIT_TYPES == 1), etl::traits::has_8bit_types);
      CHECK_EQUAL((ETL_USING_64BIT_TYPES == 1), etl::traits::has_64bit_types);

      // Using builtins...
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_ASSIGNABLE == 1), etl::traits::using_builtin_is_assignable);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_CONSTRUCTIBLE == 1), etl::traits::using_builtin_is_constructible);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_NOTHROW_CONSTRUCTIBLE == 1), etl::traits::using_builtin_is_nothrow_constructible);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_NOTHROW_ASSIGNABLE == 1), etl::traits::using_builtin_is_nothrow_assignable);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE == 1), etl::traits::using_builtin_is_trivially_constructible);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_TRIVIALLY_ASSIGNABLE == 1), etl::traits::using_builtin_is_trivially_assignable);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE == 1), etl::traits::using_builtin_is_trivially_destructible);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_TRIVIALLY_COPYABLE == 1), etl::traits::using_builtin_is_trivially_copyable);
      CHECK_EQUAL((ETL_USING_BUILTIN_HAS_TRIVIAL_CONSTRUCTOR == 1), etl::traits::using_builtin_has_trivial_constructor);
      CHECK_EQUAL((ETL_USING_BUILTIN_HAS_TRIVIAL_DESTRUCTOR == 1), etl::traits::using_builtin_has_trivial_destructor);
      CHECK_EQUAL((ETL_USING_BUILTIN_HAS_VIRTUAL_DESTRUCTOR == 1), etl::traits::using_builtin_has_virtual_destructor);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_DESTRUCTIBLE == 1), etl::traits::using_builtin_is_destructible);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_NOTHROW_DESTRUCTIBLE == 1), etl::traits::using_builtin_is_nothrow_destructible);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_STANDARD_LAYOUT == 1), etl::traits::using_builtin_is_standard_layout);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_TRIVIAL == 1), etl::traits::using_builtin_is_trivial);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_LAYOUT_COMPATIBLE == 1), etl::traits::using_builtin_is_layout_compatible);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_POINTER_INTERCONVERTIBLE_BASE_OF == 1), etl::traits::using_builtin_is_pointer_interconvertible_base_of);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_UNION == 1), etl::traits::using_builtin_is_union);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_EMPTY == 1), etl::traits::using_builtin_is_empty);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_POLYMORPHIC == 1), etl::traits::using_builtin_is_polymorphic);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_ABSTRACT == 1), etl::traits::using_builtin_is_abstract);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_FINAL == 1), etl::traits::using_builtin_is_final);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_AGGREGATE == 1), etl::traits::using_builtin_is_aggregate);
      CHECK_EQUAL((ETL_USING_BUILTIN_HAS_UNIQUE_OBJECT_REPRESENTATIONS == 1), etl::traits::using_builtin_has_unique_object_representations);
      CHECK_EQUAL((ETL_USING_BUILTIN_UNDERLYING_TYPE == 1), etl::traits::using_builtin_underlying_type);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_CONSTANT_EVALUATED == 1), etl::traits::using_builtin_is_constant_evaluated);
      CHECK_EQUAL((ETL_USING_BUILTIN_CONSTANT_P == 1), etl::traits::using_builtin_constant_p);
      CHECK_EQUAL((ETL_USING_BUILTIN_MEMCPY == 1), etl::traits::using_builtin_memcpy);
      CHECK_EQUAL((ETL_USING_BUILTIN_MEMMOVE == 1), etl::traits::using_builtin_memmove);
      CHECK_EQUAL((ETL_USING_BUILTIN_MEMSET == 1), etl::traits::using_builtin_memset);
      CHECK_EQUAL((ETL_USING_BUILTIN_MEMCMP == 1), etl::traits::using_builtin_memcmp);
      CHECK_EQUAL((ETL_USING_BUILTIN_MEMCHR == 1), etl::traits::using_builtin_memchr);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_VIRTUAL_BASE_OF == 1), etl::traits::using_builtin_is_virtual_base_of);
      CHECK_EQUAL((ETL_USING_BUILTIN_IS_TRIVIALLY_RELOCATABLE == 1), etl::traits::using_builtin_is_trivially_relocatable);
      CHECK_EQUAL((ETL_USING_BUILTIN_BUILTIN_IS_CPP_TRIVIALLY_RELOCATABLE == 1), etl::traits::using_builtin_builtin_is_cpp_trivially_relocatable);
      CHECK_EQUAL((ETL_USING_BUILTIN_BIT_CAST == 1), etl::traits::using_builtin_bit_cast);

      // Has...
      CHECK_EQUAL((ETL_HAS_ATOMIC == 1), etl::traits::has_atomic);
      CHECK_EQUAL((ETL_HAS_MUTEX == 1), etl::traits::has_mutex);
      CHECK_EQUAL((ETL_HAS_NULLPTR == 1), etl::traits::has_nullptr);
      CHECK_EQUAL((ETL_HAS_CHAR8_T == 1), etl::traits::has_char8_t);
      CHECK_EQUAL((ETL_HAS_NATIVE_CHAR8_T == 1), etl::traits::has_native_char8_t);
      CHECK_EQUAL((ETL_HAS_NATIVE_CHAR16_T == 1), etl::traits::has_native_char16_t);
      CHECK_EQUAL((ETL_HAS_NATIVE_CHAR32_T == 1), etl::traits::has_native_char32_t);
      CHECK_EQUAL((ETL_HAS_STRING_TRUNCATION_CHECKS == 1), etl::traits::has_string_truncation_checks);
      CHECK_EQUAL((ETL_HAS_ERROR_ON_STRING_TRUNCATION == 1), etl::traits::has_error_on_string_truncation);
      CHECK_EQUAL((ETL_HAS_STRING_CLEAR_AFTER_USE == 1), etl::traits::has_string_clear_after_use);
      CHECK_EQUAL((ETL_HAS_ISTRING_REPAIR == 1), etl::traits::has_istring_repair);
      CHECK_EQUAL((ETL_HAS_IVECTOR_REPAIR == 1), etl::traits::has_ivector_repair);
      CHECK_EQUAL((ETL_HAS_IDEQUE_REPAIR == 1), etl::traits::has_ideque_repair);
      CHECK_EQUAL((ETL_HAS_MUTABLE_ARRAY_VIEW == 1), etl::traits::has_mutable_array_view);
      CHECK_EQUAL((ETL_HAS_VIRTUAL_MESSAGES == 1), etl::traits::has_virtual_messages);
      CHECK_EQUAL((ETL_HAS_PACKED == 1), etl::traits::has_packed);

      // Is...
      CHECK_EQUAL((ETL_IS_DEBUG_BUILD == 1), etl::traits::is_debug_build);

      // Other
      CHECK_EQUAL(__cplusplus, etl::traits::cplusplus);
      CHECK_EQUAL(ETL_VERSION_MAJOR, etl::traits::version_major);
      CHECK_EQUAL(ETL_VERSION_MINOR, etl::traits::version_minor);
      CHECK_EQUAL(ETL_VERSION_PATCH, etl::traits::version_patch);
      CHECK_EQUAL(ETL_VERSION_VALUE, etl::traits::version);
#if ETL_USING_CPP26
      CHECK_EQUAL(26, etl::traits::language_standard);
#elif ETL_USING_CPP23
      CHECK_EQUAL(23, etl::traits::language_standard);
#elif ETL_USING_CPP20
      CHECK_EQUAL(20, etl::traits::language_standard);
#elif ETL_USING_CPP17
      CHECK_EQUAL(17, etl::traits::language_standard);
#elif ETL_USING_CPP14
      CHECK_EQUAL(14, etl::traits::language_standard);
#endif

      CHECK_ARRAY_EQUAL(ETL_VERSION, etl::traits::version_string, etl::strlen(ETL_VERSION));
      CHECK_ARRAY_EQUAL(ETL_VERSION, etl::traits::version_wstring, etl::strlen(ETL_VERSION_W));
#if ETL_HAS_NATIVE_CHAR8_T
      CHECK_ARRAY_EQUAL(ETL_VERSION, etl::traits::version_u8string, etl::strlen(ETL_VERSION_U8));
#endif
      CHECK_ARRAY_EQUAL(ETL_VERSION, etl::traits::version_u16string, etl::strlen(ETL_VERSION_U16));
      CHECK_ARRAY_EQUAL(ETL_VERSION, etl::traits::version_u32string, etl::strlen(ETL_VERSION_U32));
    }
  }
} // namespace
