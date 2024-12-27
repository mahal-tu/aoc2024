#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test20_t11)
{
  auto r = solution<2, 1>("test.txt");
  BOOST_CHECK_EQUAL(r, 44);
}

BOOST_AUTO_TEST_CASE(Test20_t12)
{
  auto r = solution<2,100>("input.txt");
  BOOST_CHECK_EQUAL(r, 1293);
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test20_t21)
{
  auto r = solution<20, 50>("test.txt");
  BOOST_CHECK_EQUAL(r, 285);
}

BOOST_AUTO_TEST_CASE(Test20_i2)
{
  auto r = solution<20, 100>("input.txt");
  BOOST_CHECK_EQUAL(r, 977747);
}
