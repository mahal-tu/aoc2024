#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test15_t11)
{
  auto r = solution1("test.txt");
  BOOST_CHECK_EQUAL(r, 2028);
}

BOOST_AUTO_TEST_CASE(Test15_t12)
{
  auto r = solution1("test2.txt");
  BOOST_CHECK_EQUAL(r, 10092);
}

BOOST_AUTO_TEST_CASE(Test15_i1)
{
  auto r = solution1("input.txt");
  BOOST_CHECK_EQUAL(r, 1446158);
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test15_t2)
{
  auto r = solution2("test2.txt");
  BOOST_CHECK_EQUAL(r, 9021);
}

BOOST_AUTO_TEST_CASE(Test15_i2)
{
  auto r = solution2("input.txt");
  BOOST_CHECK_EQUAL(r, 1446175);
}
