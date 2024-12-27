#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------
#if 1
BOOST_AUTO_TEST_CASE(Test09_t1)
{
  auto r = solution1("test.txt");
  BOOST_CHECK_EQUAL(r, 1928);
}

BOOST_AUTO_TEST_CASE(Test09_i1)
{
  auto r = solution1("input.txt");
  BOOST_CHECK_EQUAL(r, 6519155389266LL);
}
#endif
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test09_t2)
{
  auto r = solution2("test.txt");
  BOOST_CHECK_EQUAL(r, 2858);
}

BOOST_AUTO_TEST_CASE(Test09_i2)
{
  auto r = solution2("input.txt");
  BOOST_CHECK_EQUAL(r, 6547228115826LL);
}
