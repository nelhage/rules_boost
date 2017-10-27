#define BOOST_TEST_MODULE circular_buffer_test
#include <boost/circular_buffer.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test_circular_buffer )
{
  boost::circular_buffer<int> cb(1);
  cb.push_back(1);
  BOOST_TEST(cb[0] == 1);
}
