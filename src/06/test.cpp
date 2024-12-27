#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test06_t1)
{
  BOOST_CHECK_EQUAL(solution<1>("test.txt"), 41);
}

BOOST_AUTO_TEST_CASE(Test06_i1)
{
  BOOST_CHECK_EQUAL(solution<1>("input.txt"), 5331);
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test06_t2)
{
  BOOST_CHECK_EQUAL(solution<2>("test.txt"), 6);
}

BOOST_AUTO_TEST_CASE(Test06_i2)
{
  BOOST_CHECK_EQUAL(solution<2>("input.txt"), 1812);
}
