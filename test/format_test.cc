#define BOOST_TEST_MODULE format_test
#include <boost/format.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test_format )
{
  BOOST_TEST((boost::format("hello %d") % 1).str() == "hello 1");
}
