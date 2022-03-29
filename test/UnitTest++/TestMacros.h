#ifndef UNITTEST_TESTMACROS_H
#define UNITTEST_TESTMACROS_H

#include "Config.h"
#include "TestSuite.h"
#include "ExceptionMacros.h"
#include "ExecuteTest.h"
#include "AssertException.h"
#include "TestDetails.h"
#include "MemoryOutStream.h"

#ifndef UNITTEST_POSIX
#define UNITTEST_THROW_SIGNALS_POSIX_ONLY
#else
#include "Posix/SignalTranslator.h"
#endif

#define UNITTEST_SUITE(Name)                  \
   namespace Suite ## Name {                  \
      namespace UnitTestSuite {               \
         inline char const* GetSuiteName () { \
            return #Name;                     \
         }                                    \
      }                                       \
   }                                          \
   namespace Suite ## Name

#define UNITTEST_IMPL_TEST(Name, List)                                                   \
   class Test ## Name : public UnitTest::Test                                            \
   {                                                                                     \
   public:                                                                               \
      Test ## Name() : Test(#Name, UnitTestSuite::GetSuiteName(), __FILE__, __LINE__) {} \
   private:                                                                              \
      virtual void RunImpl() const;                                                      \
   } static test ## Name ## Instance;                                                    \
                                                                                         \
   static UnitTest::ListAdder adder ## Name (List, &test ## Name ## Instance);           \
                                                                                         \
   void Test ## Name::RunImpl() const


#define UNITTEST_TEST(Name) UNITTEST_IMPL_TEST(Name, UnitTest::Test::GetTestList())


#define UNITTEST_IMPL_TEST_FIXTURE(Fixture, Name, List)                                                                             \
   class Fixture ## Name ## Helper : public Fixture                                                                      \
   {                                                                                                                     \
   public:                                                                                                               \
      explicit Fixture ## Name ## Helper(UnitTest::TestDetails const& details) : m_details(details) {}                   \
      void RunImpl();                                                                                                    \
      UnitTest::TestDetails const& m_details;                                                                            \
      virtual ~Fixture ## Name ## Helper();										                                                 \
   private:                                                                                                              \
      Fixture ## Name ## Helper(Fixture ## Name ## Helper const&);                                                       \
      Fixture ## Name ## Helper& operator =(Fixture ## Name ## Helper const&);                                           \
   };                                                                                                                    \
   Fixture ## Name ## Helper::~Fixture ## Name ## Helper(){}                                                             \
                                                                                                                         \
   class Test ## Fixture ## Name : public UnitTest::Test                                                                 \
   {                                                                                                                     \
   public:                                                                                                               \
      Test ## Fixture ## Name() : Test(#Name, UnitTestSuite::GetSuiteName(), __FILE__, __LINE__) {}                      \
   private:                                                                                                              \
      virtual void RunImpl() const;                                                                                      \
   } static test ## Fixture ## Name ## Instance;                                                                         \
                                                                                                                         \
   static UnitTest::ListAdder adder ## Fixture ## Name (List, &test ## Fixture ## Name ## Instance);                     \
                                                                                                                         \
   void Test ## Fixture ## Name::RunImpl() const                                                                         \
   {                                                                                                                     \
      volatile bool ctorOk = false;                                                                                      \
      UNITTEST_IMPL_TRY                                                                                                             \
         ({                                                                                                              \
         Fixture ## Name ## Helper fixtureHelper(m_details);                                                             \
         ctorOk = true;                                                                                                  \
         UnitTest::ExecuteTest(fixtureHelper, m_details, false);                                                         \
      })                                                                                                                 \
      UNITTEST_IMPL_CATCH (UnitTest::AssertException, e,                                                                            \
      {                                                                                                                  \
         (void)e;                                                                                                        \
      })                                                                                                                 \
      UNITTEST_IMPL_CATCH (std::exception, e,                                                                                       \
      {                                                                                                                  \
         UnitTest::MemoryOutStream stream;                                                                               \
         stream << "Unhandled exception: " << e.what();                                                                  \
         UnitTest::CurrentTest::Results()->OnTestFailure(m_details, stream.GetText());                                   \
      })                                                                                                                 \
      UNITTEST_IMPL_CATCH_ALL                                                                                                       \
         ({                                                                                                              \
         if (ctorOk)                                                                                                     \
         {                                                                                                               \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(m_details, __LINE__),                  \
                                                            "Unhandled exception while destroying fixture " #Fixture);   \
         }                                                                                                               \
         else                                                                                                            \
         {                                                                                                               \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(m_details, __LINE__),                  \
                                                            "Unhandled exception while constructing fixture " #Fixture); \
         }                                                                                                               \
      })                                                                                                                 \
   }                                                                                                                     \
   void Fixture ## Name ## Helper::RunImpl()

#define UNITTEST_TEST_FIXTURE(Fixture,Name) UNITTEST_IMPL_TEST_FIXTURE(Fixture, Name, UnitTest::Test::GetTestList())

#ifndef UNITTEST_DISABLE_SHORT_MACROS
   #ifdef SUITE
      #error SUITE already defined, re-configure with UNITTEST_ENABLE_SHORT_MACROS set to 0 and use UNITTEST_SUITE instead
   #else
      #define SUITE UNITTEST_SUITE
   #endif

   #ifdef TEST
      #error TEST already defined, re-configure with UNITTEST_ENABLE_SHORT_MACROS set to 0 and use UNITTEST_TEST instead
   #else
      #define TEST UNITTEST_TEST
   #endif

   #ifdef TEST_FIXTURE
      #error TEST_FIXTURE already defined, re-configure with UNITTEST_ENABLE_SHORT_MACROS set to 0 and use UNITTEST_TEST_FIXTURE instead
   #else
      #define TEST_FIXTURE UNITTEST_TEST_FIXTURE
   #endif
#endif

#endif
