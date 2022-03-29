#ifndef UNITTEST_CHECKS_H
#define UNITTEST_CHECKS_H

#include "Config.h"
#include "TestResults.h"
#include "MemoryOutStream.h"

namespace UnitTest {


   template< typename Value >
   bool Check(Value const& value)
   {
      return !!value; // doing double negative to avoid silly VS warnings
   }


   template< typename Expected, typename Actual >
   void CheckEqual(TestResults& results, Expected const& expected, Actual const& actual, TestDetails const& details)
   {
      if (!(expected == actual))
      {
         UnitTest::MemoryOutStream stream;
         stream << "Expected " << expected << " but was " << actual;

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
            stream << expected[expectedIndex] << " ";

         stream << "] but was [ ";

         for (size_t actualIndex = 0; actualIndex < count; ++actualIndex)
            stream << actual[actualIndex] << " ";

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

#endif
