#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test22_t11)
{
  auto r = solution1("test.txt");
  BOOST_CHECK_EQUAL(r, 37327623);
}

BOOST_AUTO_TEST_CASE(Test22_i1)
{
  auto r = solution1("input.txt");
  BOOST_CHECK_EQUAL(r, 13022553808LL);
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test22_t2)
{
  auto r = solution2("test2.txt");
  BOOST_CHECK_EQUAL(r, 23);
}

BOOST_AUTO_TEST_CASE(Test22_i2)
{
  auto r = solution2("input.txt");
  BOOST_CHECK_EQUAL(r, 1555);
}
