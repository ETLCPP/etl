#ifndef UNITTEST_CHECKMACROS_H
#define UNITTEST_CHECKMACROS_H

#include "HelperMacros.h"
#include "ExceptionMacros.h"
#include "Checks.h"
#include "AssertException.h"
#include "RequiredCheckException.h"
#include "MemoryOutStream.h"
#include "TestDetails.h"
#include "CurrentTest.h"
#include "ReportAssertImpl.h"

#define UNITTEST_CHECK(value)                                      \
   UNITTEST_MULTILINE_MACRO_BEGIN                                                                                                     \
   UNITTEST_IMPL_TRY                                                                                                                             \
   ({                                                                                                                                 \
      if (!UnitTest::Check(value))                                                                                                    \
         UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), #value); \
   })                                                                                                                                 \
   UNITTEST_IMPL_RETHROW (UnitTest::RequiredCheckException)                                                                                                               \
   UNITTEST_IMPL_CATCH (std::exception, e,                                                                                                       \
   {                                                                                                                                  \
      UnitTest::MemoryOutStream UnitTest_message;                                                                                     \
      UnitTest_message << "Unhandled exception (" << e.what() << ") in CHECK(" #value ")";                                            \
      UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__),             \
                                                      UnitTest_message.GetText());                                                    \
   })                                                                                                                                 \
   UNITTEST_IMPL_CATCH_ALL                                                                                                                       \
      ({                                                                                                                              \
      UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__),             \
                                                      "Unhandled exception in CHECK(" #value ")");                                    \
   })                                                                                                                                 \
   UNITTEST_MULTILINE_MACRO_END

#define UNITTEST_CHECK_EQUAL(expected, actual)                                                                                                                \
   UNITTEST_MULTILINE_MACRO_BEGIN                                                                                                                    \
   UNITTEST_IMPL_TRY                                                                                                                                            \
   ({                                                                                                                                                \
      UnitTest::CheckEqual(*UnitTest::CurrentTest::Results(), expected, actual, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)); \
   })                                                                                                                                                \
   UNITTEST_IMPL_RETHROW (UnitTest::RequiredCheckException)                                                                                                               \
   UNITTEST_IMPL_CATCH (std::exception, e,                                                                                                                      \
   {                                                                                                                                                 \
      UnitTest::MemoryOutStream UnitTest_message;                                                                                                    \
      UnitTest_message << "Unhandled exception (" << e.what() << ") in CHECK_EQUAL(" #expected ", " #actual ")";                                     \
      UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__),                            \
                                                      UnitTest_message.GetText());                                                                   \
   })                                                                                                                                                \
   UNITTEST_IMPL_CATCH_ALL                                                                                                                                      \
   ({                                                                                                                                                \
      UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__),                            \
                                                      "Unhandled exception in CHECK_EQUAL(" #expected ", " #actual ")");                             \
   })                                                                                                                                                \
   UNITTEST_MULTILINE_MACRO_END

#define UNITTEST_CHECK_CLOSE(expected, actual, tolerance)                                                                                                                \
   UNITTEST_MULTILINE_MACRO_BEGIN                                                                                                                               \
   UNITTEST_IMPL_TRY                                                                                                                                                       \
   ({                                                                                                                                                           \
      UnitTest::CheckClose(*UnitTest::CurrentTest::Results(), expected, actual, tolerance, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)); \
   })                                                                                                                                                           \
   UNITTEST_IMPL_RETHROW (UnitTest::RequiredCheckException)                                                                                                               \
   UNITTEST_IMPL_CATCH (std::exception, e,                                                                                                                                 \
   {                                                                                                                                                            \
      UnitTest::MemoryOutStream UnitTest_message;                                                                                                               \
      UnitTest_message << "Unhandled exception (" << e.what() << ") in CHECK_CLOSE(" #expected ", " #actual ")";                                                \
      UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__),                                       \
                                                      UnitTest_message.GetText());                                                                              \
   })                                                                                                                                                           \
   UNITTEST_IMPL_CATCH_ALL                                                                                                                                                 \
   ({                                                                                                                                                           \
      UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__),                                       \
                                                      "Unhandled exception in CHECK_CLOSE(" #expected ", " #actual ")");                                        \
   })                                                                                                                                                           \
   UNITTEST_MULTILINE_MACRO_END

#define UNITTEST_CHECK_ARRAY_EQUAL(expected, actual, count)                                                                                                               \
   UNITTEST_MULTILINE_MACRO_BEGIN                                                                                                                                \
   UNITTEST_IMPL_TRY                                                                                                                                                        \
      ({                                                                                                                                                         \
      UnitTest::CheckArrayEqual(*UnitTest::CurrentTest::Results(), expected, actual, count, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)); \
   })                                                                                                                                                            \
   UNITTEST_IMPL_RETHROW (UnitTest::RequiredCheckException)                                                                                                               \
   UNITTEST_IMPL_CATCH (std::exception, e,                                                                                                                                  \
   {                                                                                                                                                             \
      UnitTest::MemoryOutStream UnitTest_message;                                                                                                                \
      UnitTest_message << "Unhandled exception (" << e.what() << ") in CHECK_ARRAY_EQUAL(" #expected ", " #actual ")";                                           \
      UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__),                                        \
                                                      UnitTest_message.GetText());                                                                               \
   })                                                                                                                                                            \
   UNITTEST_IMPL_CATCH_ALL                                                                                                                                                  \
      ({                                                                                                                                                         \
      UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__),                                        \
                                                      "Unhandled exception in CHECK_ARRAY_EQUAL(" #expected ", " #actual ")");                                   \
   })                                                                                                                                                            \
   UNITTEST_MULTILINE_MACRO_END

#define UNITTEST_CHECK_ARRAY_CLOSE(expected, actual, count, tolerance)                                                                                                               \
   UNITTEST_MULTILINE_MACRO_BEGIN                                                                                                                                           \
   UNITTEST_IMPL_TRY                                                                                                                                                                   \
      ({                                                                                                                                                                    \
      UnitTest::CheckArrayClose(*UnitTest::CurrentTest::Results(), expected, actual, count, tolerance, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)); \
   })                                                                                                                                                                       \
   UNITTEST_IMPL_RETHROW (UnitTest::RequiredCheckException)                                                                                                               \
   UNITTEST_IMPL_CATCH (std::exception, e,                                                                                                                                             \
   {                                                                                                                                                                        \
      UnitTest::MemoryOutStream UnitTest_message;                                                                                                                           \
      UnitTest_message << "Unhandled exception (" << e.what() << ") in CHECK_ARRAY_CLOSE(" #expected ", " #actual ")";                                                      \
      UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__),                                                   \
                                                      UnitTest_message.GetText());                                                                                          \
   })                                                                                                                                                                       \
   UNITTEST_IMPL_CATCH_ALL                                                                                                                                                             \
      ({                                                                                                                                                                    \
      UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__),                                                   \
                                                      "Unhandled exception in CHECK_ARRAY_CLOSE(" #expected ", " #actual ")");                                              \
   })                                                                                                                                                                       \
   UNITTEST_MULTILINE_MACRO_END

#define UNITTEST_CHECK_ARRAY2D_CLOSE(expected, actual, rows, columns, tolerance)                                                                                                               \
   UNITTEST_MULTILINE_MACRO_BEGIN                                                                                                                                                     \
   UNITTEST_IMPL_TRY                                                                                                                                                                             \
      ({                                                                                                                                                                              \
      UnitTest::CheckArray2DClose(*UnitTest::CurrentTest::Results(), expected, actual, rows, columns, tolerance, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)); \
   })                                                                                                                                                                                 \
   UNITTEST_IMPL_RETHROW (UnitTest::RequiredCheckException)                                                                                                               \
   UNITTEST_IMPL_CATCH (std::exception, e,                                                                                                                                                       \
   {                                                                                                                                                                                  \
      UnitTest::MemoryOutStream UnitTest_message;                                                                                                                                     \
      UnitTest_message << "Unhandled exception (" << e.what() << ") in CHECK_ARRAY2D_CLOSE(" #expected ", " #actual ")";                                                              \
      UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__),                                                             \
                                                      UnitTest_message.GetText());                                                                                                    \
   })                                                                                                                                                                                 \
   UNITTEST_IMPL_CATCH_ALL                                                                                                                                                                       \
      ({                                                                                                                                                                              \
      UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__),                                                             \
                                                      "Unhandled exception in CHECK_ARRAY2D_CLOSE(" #expected ", " #actual ")");                                                      \
   })                                                                                                                                                                                 \
   UNITTEST_MULTILINE_MACRO_END

#ifndef UNITTEST_DISABLE_SHORT_MACROS
   #ifdef CHECK
      #error CHECK already defined, re-configure with UNITTEST_ENABLE_SHORT_MACROS set to 0 and use UNITTEST_CHECK instead
   #else
      #define CHECK UNITTEST_CHECK
   #endif

   #ifdef CHECK_EQUAL
      #error CHECK_EQUAL already defined, re-configure with UNITTEST_ENABLE_SHORT_MACROS set to 0 and use UNITTEST_CHECK_EQUAL instead
   #else
      #define CHECK_EQUAL UNITTEST_CHECK_EQUAL
   #endif

   #ifdef CHECK_CLOSE
      #error CHECK_CLOSE already defined, re-configure with UNITTEST_ENABLE_SHORT_MACROS set to 0 and use UNITTEST_CHECK_CLOSE instead
   #else
      #define CHECK_CLOSE UNITTEST_CHECK_CLOSE
   #endif

   #ifdef CHECK_ARRAY_EQUAL
      #error CHECK_ARRAY_EQUAL already defined, re-configure with UNITTEST_ENABLE_SHORT_MACROS set to 0 and use UNITTEST_CHECK_ARRAY_EQUAL instead
   #else
      #define CHECK_ARRAY_EQUAL UNITTEST_CHECK_ARRAY_EQUAL
   #endif

   #ifdef CHECK_ARRAY_CLOSE
      #error CHECK_ARRAY_CLOSE already defined, re-configure with UNITTEST_ENABLE_SHORT_MACROS set to 0 and use UNITTEST_CHECK_ARRAY_CLOSE instead
   #else
      #define CHECK_ARRAY_CLOSE UNITTEST_CHECK_ARRAY_CLOSE
   #endif

   #ifdef CHECK_ARRAY2D_CLOSE
      #error CHECK_ARRAY2D_CLOSE already defined, re-configure with UNITTEST_ENABLE_SHORT_MACROS set to 0 and use UNITTEST_CHECK_ARRAY2D_CLOSE instead
   #else
      #define CHECK_ARRAY2D_CLOSE UNITTEST_CHECK_ARRAY2D_CLOSE
   #endif
#endif

// CHECK_THROW and CHECK_ASSERT only exist when UNITTEST_NO_EXCEPTIONS isn't defined (see config.h)
#ifndef UNITTEST_NO_EXCEPTIONS

#define UNITTEST_CHECK_THROW(expression, ExpectedExceptionType)                                                                                                                                      \
   UNITTEST_MULTILINE_MACRO_BEGIN                                                                                                                                                           \
   bool caught_ = false;                                                                                                                                                                    \
   try { expression; }                                                                                                                                                                      \
   catch (ExpectedExceptionType const&) { caught_ = true; }                                                                                                                                 \
   catch (...) {}                                                                                                                                                                           \
   if (!caught_)                                                                                                                                                                            \
      UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), "Expected exception: \"" #ExpectedExceptionType "\" not thrown"); \
   UNITTEST_MULTILINE_MACRO_END


#define UNITTEST_CHECK_ASSERT(expression)                       \
   UNITTEST_MULTILINE_MACRO_BEGIN                      \
   UnitTest::Detail::ExpectAssert(true);               \
   CHECK_THROW(expression, UnitTest::AssertException); \
   UnitTest::Detail::ExpectAssert(false);              \
   UNITTEST_MULTILINE_MACRO_END
#endif

#ifndef UNITTEST_DISABLE_SHORT_MACROS
   #ifdef CHECK_THROW
      #error CHECK_THROW already defined, re-configure with UNITTEST_ENABLE_SHORT_MACROS set to 0 and use UNITTEST_CHECK_THROW instead
   #else
      #define CHECK_THROW UNITTEST_CHECK_THROW
   #endif

   #ifdef CHECK_ASSERT
      #error CHECK_ASSERT already defined, re-configure with UNITTEST_ENABLE_SHORT_MACROS set to 0 and use UNITTEST_CHECK_ASSERT instead
   #else
      #define CHECK_ASSERT UNITTEST_CHECK_ASSERT
   #endif
#endif

#endif

