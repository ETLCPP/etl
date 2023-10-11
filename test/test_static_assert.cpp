#include "unit_test_framework.h"

#include "etl/static_assert.h"

namespace
{
  SUITE(test_static_assert)
  {
    //*************************************************************************
    TEST(test_static_assert_2arguments)
    {
      ETL_STATIC_ASSERT(true, "condition must be true");
    }
  }
}
