#define BOOST_TEST_MODULE bind_test
#include <boost/bind.hpp>
#include <boost/test/included/unit_test.hpp>

int f(int a, int b)
{
  return a + b;
}

BOOST_AUTO_TEST_CASE( test_bind )
{
  BOOST_TEST(boost::bind(f, 1, 2)() == 3);
}
