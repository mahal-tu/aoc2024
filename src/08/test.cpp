#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test08_t1)
{
  BOOST_CHECK_EQUAL(solution<1>("test.txt"), 14);
}

BOOST_AUTO_TEST_CASE(Test08_i1)
{
  BOOST_CHECK_EQUAL(solution<1>("input.txt"), 344);
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test08_t2)
{
  BOOST_CHECK_EQUAL(solution<2>("test.txt"), 34);
}

BOOST_AUTO_TEST_CASE(Test08_i2)
{
  BOOST_CHECK_EQUAL(solution<2>("input.txt"), 1182);
}
