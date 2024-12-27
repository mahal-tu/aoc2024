#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test12_t1)
{
  BOOST_CHECK_EQUAL(solution1("test.txt"), 1930);
}

BOOST_AUTO_TEST_CASE(Test12_i1)
{
  BOOST_CHECK_EQUAL(solution1("input.txt"), 1396562);
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test12_t2)
{
  BOOST_CHECK_EQUAL(solution2("test.txt"), 1206);
}

BOOST_AUTO_TEST_CASE(Test12_t22)
{
  BOOST_CHECK_EQUAL(solution2("test2.txt"), 368);
}

BOOST_AUTO_TEST_CASE(Test12_t23)
{
  BOOST_CHECK_EQUAL(solution2("test3.txt"), 236);
}

BOOST_AUTO_TEST_CASE(Test12_i2)
{
  BOOST_CHECK_EQUAL(solution2("input.txt"), 844132);
}
