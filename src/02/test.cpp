#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test03_t1)
{
  BOOST_CHECK_EQUAL(solution<1>("test.txt"), 2);
}

BOOST_AUTO_TEST_CASE(Test03_i1)
{
  BOOST_CHECK_EQUAL(solution<1>("input.txt"), 549);
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test03_t2)
{
  BOOST_CHECK_EQUAL(solution<2>("test.txt"), 4);
}

BOOST_AUTO_TEST_CASE(Test03_i2)
{
  BOOST_CHECK_EQUAL(solution<2>("input.txt"), 589);
}
