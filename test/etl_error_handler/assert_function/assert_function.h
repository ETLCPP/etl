#include "etl/exception.h"

extern void AssertFunction(const etl::exception& /*e*/);

#define ETL_ASSERT_FUNCTION AssertFunction
