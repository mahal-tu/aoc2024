#include "solution.cpp"

#define BOOST_TEST_MODULE Test
#include <boost/test/included/unit_test.hpp>

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test21_t11)
{
  auto r = solution1("test.txt");
  BOOST_CHECK_EQUAL(r, 126384);
}

BOOST_AUTO_TEST_CASE(Test21_t12)
{
  auto r = solution1("input.txt");
  BOOST_CHECK_EQUAL(r, 278748);
}

// ----------------------------------------------------------------------------

#if 1
BOOST_AUTO_TEST_CASE(Test21_i2)
{
  auto r = solution2("input.txt");
  BOOST_CHECK_EQUAL(r, 337744744231414LL);
}
#endif
