#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test14_t1)
{
  auto r = solution1<11, 7>("test.txt");
  BOOST_CHECK_EQUAL(r, 12);
}

BOOST_AUTO_TEST_CASE(Test14_i1)
{
  auto r = solution1<101, 103>("input.txt");
  BOOST_CHECK_EQUAL(r, 210587128LL);
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test14_i2)
{
  auto r = solution2<101, 103>("input.txt");
  BOOST_CHECK_EQUAL(r, 7286LL);
}
