#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test01_t1)
{
  BOOST_CHECK_EQUAL(solution<1>("test.txt"), 11);
}

BOOST_AUTO_TEST_CASE(Test01_i1)
{
  BOOST_CHECK_EQUAL(solution<1>("input.txt"), 1223326);
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test01_t2)
{
  BOOST_CHECK_EQUAL(solution<2>("test.txt"), 31);
}

BOOST_AUTO_TEST_CASE(Test01_i2)
{
  BOOST_CHECK_EQUAL(solution<2>("input.txt"), 21070419);
}
