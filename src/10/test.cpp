#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test10_t1)
{
  BOOST_CHECK_EQUAL(solution1("test.txt"), 36);
}

BOOST_AUTO_TEST_CASE(Test10_i1)
{
  BOOST_CHECK_EQUAL(solution1("input.txt"), 582);
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test10_t2)
{
  BOOST_CHECK_EQUAL(solution2("test.txt"), 81);
}

BOOST_AUTO_TEST_CASE(Test10_i2)
{
  BOOST_CHECK_EQUAL(solution2("input.txt"), 1302);
}
