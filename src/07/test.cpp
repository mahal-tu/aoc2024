#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test07_t1)
{
  BOOST_CHECK_EQUAL(solution<1>("test.txt"), 3749);
}

BOOST_AUTO_TEST_CASE(Test07_i1)
{
  BOOST_CHECK_EQUAL(solution<1>("input.txt"), 8401132154762);
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test07_t2)
{
  BOOST_CHECK_EQUAL(solution<2>("test.txt"), 11387);
}

BOOST_AUTO_TEST_CASE(Test07_i2)
{
  BOOST_CHECK_EQUAL(solution<2>("input.txt"), 95297119227552LL);
}
