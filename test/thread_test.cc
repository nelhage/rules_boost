#define BOOST_TEST_MODULE thread_test
#include <boost/test/included/unit_test.hpp>
#include <boost/thread.hpp>

void thread()
{
}

BOOST_AUTO_TEST_CASE( test_thread )
{
  boost::thread t{thread};
  t.join();
}
