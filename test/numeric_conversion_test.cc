#define BOOST_TEST_MODULE numeric_conversion_test
#include <boost/numeric/conversion/cast.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test_numeric_conversion )
{
  short a = 42;
  int b = 42;
  BOOST_TEST(boost::numeric_cast<int>(a) == b);
}
