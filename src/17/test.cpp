#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test17_t11)
{
  auto r = solution1("test.txt");
  BOOST_CHECK_EQUAL(r, "4,6,3,5,6,3,5,2,1,0,");
}

BOOST_AUTO_TEST_CASE(Test17_t12)
{
  auto r = solution1("test2.txt");
  BOOST_CHECK_EQUAL(r, "4,2,5,6,7,7,7,7,3,1,0,");
}

BOOST_AUTO_TEST_CASE(Test17_t13)
{
  auto r = solution1("test3.txt");
  BOOST_CHECK_EQUAL(r, "0,1,2,");
}

BOOST_AUTO_TEST_CASE(Test17_i1)
{
  auto r = solution1("input.txt");
  BOOST_CHECK_EQUAL(r, "1,5,0,3,7,3,0,3,1,");
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test17_t21)
{
  auto r = solution2("test4.txt");
  BOOST_CHECK_EQUAL(r, 117440);
}

BOOST_AUTO_TEST_CASE(Test17_i2)
{
  auto r = solution2("input.txt");
  BOOST_CHECK_EQUAL(r, 105981155568026LL);
}
