#ifndef UNITTEST_CHECKS_H
#define UNITTEST_CHECKS_H

#if defined(__GNUC__) && !defined(__clang__) && !defined(__llvm__)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wsign-compare"
#endif

#if defined(__clang__) || defined(__llvm__)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wsign-compare"
#endif

#include "Config.h"
#include "TestResults.h"
#include "MemoryOutStream.h"
#include<iomanip> 

namespace UnitTest 
{
  template <typename T>
  typename std::enable_if<!std::is_pointer<T>::value, std::string>::type
    DisplayValue(const T& c)
  {
    std::ostringstream oss;

    oss << c;

    return oss.str();
  }

  template <>
  inline std::string DisplayValue(const char& c)
  {
    using type = std::char_traits<char>::int_type;

    std::ostringstream oss;

    oss << type(c) << " ('" << c << "')";

    return oss.str();
  }

#if (__cplusplus >= 202002L)
  template <>
  inline std::string DisplayValue(const char8_t& c)
  {
    using type = std::char_traits<char8_t>::int_type;

    std::ostringstream oss;

    oss << type(c);

    return oss.str();
  }
#endif

  template <>
  inline std::string DisplayValue(const wchar_t& c)
  {
    using type = std::char_traits<wchar_t>::int_type;

    std::ostringstream oss;

    oss << type(c);

    return oss.str();
  }

#if (__cplusplus >= 201103L)
  template <>
  inline std::string DisplayValue(const char16_t& c)
  {
    using type = std::char_traits<char16_t>::int_type;

    std::ostringstream oss;

    oss << type(c);

    return oss.str();
  }

  template <>
  inline std::string DisplayValue(const char32_t& c)
  {
    using type = std::char_traits<char32_t>::int_type;

    std::ostringstream oss;

    oss << type(c);

    return oss.str();
  }
#endif

  template <typename T>
  std::string DisplayValue(const T* c)
  {
    std::ostringstream oss;

    oss << c;

    return oss.str();
  }

  template <>
  inline std::string DisplayValue(const char* c)
  {
    std::ostringstream oss;

    oss << c;

    return oss.str();
  }

#if (__cplusplus >= 202002L)
  template <>
  inline std::string DisplayValue(const char8_t* c)
  {
    std::ostringstream oss;

    oss << static_cast<const void*>(c);

    return oss.str();
  }
#endif

  template <>
  inline std::string DisplayValue(const wchar_t* c)
  {
    std::ostringstream oss;

    oss << static_cast<const void*>(c);

    return oss.str();
  }

#if (__cplusplus >= 201103L)
  template <>
  inline std::string DisplayValue(const char16_t* c)
  {
    std::ostringstream oss;

    oss << static_cast<const void*>(c);

    return oss.str();
  }

  template <>
  inline std::string DisplayValue(const char32_t* c)
  {
    std::ostringstream oss;

    oss << static_cast<const void*>(c);

    return oss.str();
  }
#endif

   template< typename Value >
   bool Check(Value const& value)
   {
      return !!value; // doing double negative to avoid silly VS warnings
   }

   template< typename Value >
   bool CheckFalse(Value const& value)
   {
     return !value;
   }

   template< typename Expected, typename Actual >
   void CheckEqual(TestResults& results, Expected&& expected, Actual&& actual, TestDetails const& details)
   {
     if (!(expected == actual))
     {
       UnitTest::MemoryOutStream stream;
       stream << "Expected " 
         << DisplayValue(expected) << " but was " << DisplayValue(actual);

       results.OnTestFailure(details, stream.GetText());
     }
   }

   template< typename Expected, typename Actual >
   void CheckEqualHex(TestResults& results, Expected const& expected, Actual const& actual, TestDetails const& details)
   {
     if (!(expected == actual))
     {
       UnitTest::MemoryOutStream stream;
       stream << std::hex << std::uppercase << std::setfill('0') << "Expected 0x" << std::setw(2 * sizeof(Expected)) << expected << " but was 0x" << std::setw(2 * sizeof(Actual)) << actual;

       results.OnTestFailure(details, stream.GetText());
     }
   }

   template< typename Expected, typename Actual >
   void CheckNotEqual(TestResults& results, Expected const& expected, Actual const& actual, TestDetails const& details)
   {
     if (expected == actual)
     {
       UnitTest::MemoryOutStream stream;
       stream << "Expected not equal, but both values are" << actual;

       results.OnTestFailure(details, stream.GetText());
     }
   }

   template< typename Expected, typename Actual >
   void CheckNotEqualHex(TestResults& results, Expected const& expected, Actual const& actual, TestDetails const& details)
   {
     if (expected == actual)
     {
       UnitTest::MemoryOutStream stream;
       stream << std::hex << std::uppercase << std::setfill('0') << std::setw(2 * sizeof(Actual)) << "Expected not equal, but both values are " << actual;

       results.OnTestFailure(details, stream.GetText());
     }
   }

   UNITTEST_LINKAGE void CheckEqual(TestResults& results, char const* expected, char const* actual, TestDetails const& details);

   UNITTEST_LINKAGE void CheckEqual(TestResults& results, char* expected, char* actual, TestDetails const& details);

   UNITTEST_LINKAGE void CheckEqual(TestResults& results, char* expected, char const* actual, TestDetails const& details);

   UNITTEST_LINKAGE void CheckEqual(TestResults& results, char const* expected, char* actual, TestDetails const& details);

   template< typename Expected, typename Actual, typename Tolerance >
   bool AreClose(Expected const& expected, Actual const& actual, Tolerance const& tolerance)
   {
      return (actual >= (expected - tolerance)) && (actual <= (expected + tolerance));
   }

   template< typename Expected, typename Actual, typename Tolerance >
   void CheckClose(TestResults& results, Expected const& expected, Actual const& actual, Tolerance const& tolerance,
                   TestDetails const& details)
   {
      if (!AreClose(expected, actual, tolerance))
      {
         UnitTest::MemoryOutStream stream;
         stream << "Expected " << expected << " +/- " << tolerance << " but was " << actual;

         results.OnTestFailure(details, stream.GetText());
      }
   }

   template< typename Expected, typename Actual >
   void CheckArrayEqual(TestResults& results, Expected const& expected, Actual const& actual,
                        size_t const count, TestDetails const& details)
   {
     bool equal = true;
     for (size_t i = 0; i < count; ++i)
       equal &= (expected[i] == actual[i]);

     if (!equal)
     {
       UnitTest::MemoryOutStream stream;

       stream << "Expected [ ";

       for (size_t expectedIndex = 0; expectedIndex < count; ++expectedIndex)
         stream << DisplayValue(expected[expectedIndex]) << " ";

       stream << "] but was [ ";

       for (size_t actualIndex = 0; actualIndex < count; ++actualIndex)
         stream << DisplayValue(actual[actualIndex]) << " ";

       stream << "]";

       results.OnTestFailure(details, stream.GetText());
     }
   }

   template< typename Expected, typename Actual, typename Tolerance >
   bool ArrayAreClose(Expected const& expected, Actual const& actual, size_t const count, Tolerance const& tolerance)
   {
      bool equal = true;
      for (size_t i = 0; i < count; ++i)
         equal &= AreClose(expected[i], actual[i], tolerance);
      return equal;
   }

   template< typename Expected, typename Actual, typename Tolerance >
   void CheckArrayClose(TestResults& results, Expected const& expected, Actual const& actual,
                        size_t const count, Tolerance const& tolerance, TestDetails const& details)
   {
      bool equal = ArrayAreClose(expected, actual, count, tolerance);

      if (!equal)
      {
         UnitTest::MemoryOutStream stream;

         stream << "Expected [ ";
         for (size_t expectedIndex = 0; expectedIndex < count; ++expectedIndex)
            stream << expected[expectedIndex] << " ";
         stream << "] +/- " << tolerance << " but was [ ";

         for (size_t actualIndex = 0; actualIndex < count; ++actualIndex)
            stream << actual[actualIndex] << " ";
         stream << "]";

         results.OnTestFailure(details, stream.GetText());
      }
   }

   template< typename Expected, typename Actual, typename Tolerance >
   void CheckArray2DClose(TestResults& results, Expected const& expected, Actual const& actual,
                          size_t const rows, size_t const columns, Tolerance const& tolerance, TestDetails const& details)
   {
      bool equal = true;
      for (size_t i = 0; i < rows; ++i)
         equal &= ArrayAreClose(expected[i], actual[i], columns, tolerance);

      if (!equal)
      {
         UnitTest::MemoryOutStream stream;

         stream << "Expected [ ";

         for (size_t expectedRow = 0; expectedRow < rows; ++expectedRow)
         {
            stream << "[ ";
            for (size_t expectedColumn = 0; expectedColumn < columns; ++expectedColumn)
               stream << expected[expectedRow][expectedColumn] << " ";
            stream << "] ";
         }

         stream << "] +/- " << tolerance << " but was [ ";

         for (size_t actualRow = 0; actualRow < rows; ++actualRow)
         {
            stream << "[ ";
            for (size_t actualColumn = 0; actualColumn < columns; ++actualColumn)
               stream << actual[actualRow][actualColumn] << " ";
            stream << "] ";
         }

         stream << "]";

         results.OnTestFailure(details, stream.GetText());
      }
   }

}

#if defined(__GNUC__) && !defined(__clang__) && !defined(__llvm__)
#pragma GCC diagnostic pop
#endif

#if defined(__clang__) || defined(__llvm__)
#pragma clang diagnostic pop
#endif

#endif
