#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test16_t11)
{
  auto r = solution1("test.txt");
  BOOST_CHECK_EQUAL(r, 7036);
}

BOOST_AUTO_TEST_CASE(Test16_t12)
{
  auto r = solution1("test2.txt");
  BOOST_CHECK_EQUAL(r, 11048);
}

#if 1
BOOST_AUTO_TEST_CASE(Test16_i1)
{
  auto r = solution1("input.txt");
  BOOST_CHECK_EQUAL(r, 106512);
}
#endif

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test16_t21)
{
  auto r = solution2("test.txt");
  BOOST_CHECK_EQUAL(r, 45);
}

BOOST_AUTO_TEST_CASE(Test16_t22)
{
  auto r = solution2("test2.txt");
  BOOST_CHECK_EQUAL(r, 64);
}

#if 1
BOOST_AUTO_TEST_CASE(Test16_i2)
{
  auto r = solution2("input.txt");
  BOOST_CHECK_EQUAL(r, 563);
}
#endif