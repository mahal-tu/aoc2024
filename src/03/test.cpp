#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test04_t1)
{
  BOOST_CHECK_EQUAL(solution<1>("test.txt"), 161);
}

BOOST_AUTO_TEST_CASE(Test04_i1)
{
  BOOST_CHECK_EQUAL(solution<1>("input.txt"), 170068701);
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test04_t2)
{
  BOOST_CHECK_EQUAL(solution<2>("test.txt"), 48);
}

BOOST_AUTO_TEST_CASE(Test04_i2)
{
  BOOST_CHECK_EQUAL(solution<2>("input.txt"), 78683433);
}
