#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test05_t1)
{
  BOOST_CHECK_EQUAL(solution<1>("test.txt"), 143);
}

BOOST_AUTO_TEST_CASE(Test05_i1)
{
  BOOST_CHECK_EQUAL(solution<1>("input.txt"), 6498);
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test05_t2)
{
  BOOST_CHECK_EQUAL(solution<2>("test.txt"), 123);
}

BOOST_AUTO_TEST_CASE(Test05_i2)
{
  BOOST_CHECK_EQUAL(solution<2>("input.txt"), 5017);
}
