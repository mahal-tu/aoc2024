#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test24_t11)
{
  auto r = solution1("test.txt");
  BOOST_CHECK_EQUAL(r, 2024);
}

BOOST_AUTO_TEST_CASE(Test24_i1)
{
  auto r = solution1("input.txt");
  BOOST_CHECK_EQUAL(r, 58367545758258ULL);
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test24_i2)
{
  auto r = solution2("input.txt");
  BOOST_CHECK_EQUAL(r, "bpf,fdw,hcc,hqc,qcw,z05,z11,z35,");
}
