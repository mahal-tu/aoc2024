#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test19_t11)
{
  auto r = solution1("test.txt");
  BOOST_CHECK_EQUAL(r, 6);
}

BOOST_AUTO_TEST_CASE(Test19_t12)
{
  auto r = solution1("input.txt");
  BOOST_CHECK_EQUAL(r, 228);
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test19_t21)
{
  auto r = solution2("test.txt");
  BOOST_CHECK_EQUAL(r, 16);
}

BOOST_AUTO_TEST_CASE(Test19_i2)
{
  auto r = solution2("input.txt");
  BOOST_CHECK_EQUAL(r, 584553405070389ULL);
}
