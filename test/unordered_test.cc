#define BOOST_TEST_MODULE unordered_test
#include <string>
#include <boost/unordered_map.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test_unordered )
{
  boost::unordered_map<std::string, int> m;

  m["one"] = 1;

  BOOST_TEST(m.at("one") == 1);
}
