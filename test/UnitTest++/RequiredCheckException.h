#ifndef UNITTEST_REQUIREDCHECKEXCEPTION_H
#define UNITTEST_REQUIREDCHECKEXCEPTION_H

#include "Config.h"
#ifndef UNITTEST_NO_EXCEPTIONS

#include "HelperMacros.h"
#include <exception>

namespace UnitTest {

   class UNITTEST_LINKAGE RequiredCheckException : public std::exception
   {
   public:
      RequiredCheckException();
      virtual ~RequiredCheckException() throw();
   };

}

#endif

#endif
