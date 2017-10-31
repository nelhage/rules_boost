#define BOOST_TEST_MODULE random_test
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test_random )
{
  boost::random::mt19937 gen;
  boost::random::uniform_int_distribution<> dist(1, 6);
  BOOST_TEST(dist(gen) <= 6);
}
