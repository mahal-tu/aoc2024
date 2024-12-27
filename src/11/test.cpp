#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test11_t1)
{
  BOOST_CHECK_EQUAL(solution1("test.txt"), 55312);
}

BOOST_AUTO_TEST_CASE(Test11_i1)
{
  BOOST_CHECK_EQUAL(solution1("input.txt"), 224529);
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test11_i2)
{
  BOOST_CHECK_EQUAL(solution2("input.txt"), 266820198587914ULL);
}
