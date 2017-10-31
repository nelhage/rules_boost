#define BOOST_TEST_MODULE tokenizer_test
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test_tokenizer )
{
  std::string s = "foo bar";
  boost::tokenizer<> tok(s);
  BOOST_TEST(*(tok.begin()) == "foo");
}
