#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test25_t11)
{
  auto r = solution1("test.txt");
  BOOST_CHECK_EQUAL(r, 3);
}

BOOST_AUTO_TEST_CASE(Test25_i1)
{
  auto r = solution1("input.txt");
  BOOST_CHECK_EQUAL(r, 3201);
}
