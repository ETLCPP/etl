#ifndef UNITTEST_EXCEPTIONMACROS_H
#define UNITTEST_EXCEPTIONMACROS_H

#include "Config.h"

#ifndef UNITTEST_NO_EXCEPTIONS
   #define UNITTEST_IMPL_TRY(x) try x
   #define UNITTEST_IMPL_THROW(x) throw x
   #define UNITTEST_IMPL_RETHROW(ExceptionType) catch(ExceptionType&) { throw; }
   #define UNITTEST_IMPL_CATCH(ExceptionType, ExceptionName, CatchBody) catch(ExceptionType& ExceptionName) CatchBody
   #define UNITTEST_IMPL_CATCH_ALL(CatchBody) catch(...) CatchBody
#else
   #define UNITTEST_IMPL_TRY(x) x
   #define UNITTEST_IMPL_THROW(x)
   #define UNITTEST_IMPL_RETHROW(ExceptionType)
   #define UNITTEST_IMPL_CATCH(ExceptionType, ExceptionName, CatchBody)
   #define UNITTEST_IMPL_CATCH_ALL(CatchBody)
#endif

#endif
