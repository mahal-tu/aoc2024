#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test23_t11)
{
  auto r = solution1("test.txt");
  BOOST_CHECK_EQUAL(r, 7);
}

BOOST_AUTO_TEST_CASE(Test23_i1)
{
  auto r = solution1("input.txt");
  BOOST_CHECK_EQUAL(r, 998);
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test23_t2)
{
  auto r = solution2("test.txt");
  BOOST_CHECK_EQUAL(r, "co,de,ka,ta,");
}

BOOST_AUTO_TEST_CASE(Test23_i2)
{
  auto r = solution2("input.txt");
  BOOST_CHECK_EQUAL(r, "cc,ff,fh,fr,ny,oa,pl,rg,uj,wd,xn,xs,zw,");
}
