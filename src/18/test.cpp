#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test18_t11)
{
  auto r = solution1<7, 12>("test.txt");
  BOOST_CHECK_EQUAL(r, 22);
}

BOOST_AUTO_TEST_CASE(Test18_t12)
{
  auto r = solution1<71, 1024>("input.txt");
  BOOST_CHECK_EQUAL(r, 348);
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test18_t21)
{
  auto r = solution2<7>("test.txt");
  BOOST_CHECK_EQUAL(r, "6,1");
}

BOOST_AUTO_TEST_CASE(Test18_i2)
{
  auto r = solution2<71>("input.txt");
  BOOST_CHECK_EQUAL(r, "54,44");
}
