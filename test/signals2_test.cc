#define BOOST_TEST_MODULE signals2_test
#include <boost/signals2.hpp>
#include <boost/test/included/unit_test.hpp>

class FlagSetter
{
 public:
  FlagSetter(bool *flag) : flag(flag) {}

  void operator()()
  {
    *flag = true;
  }

  bool *flag;
};

BOOST_AUTO_TEST_CASE( test_signals2 )
{
  bool called = false;

  boost::signals2::signal<void ()> sig;
  FlagSetter fs (&called);
  sig.connect(fs);

  BOOST_TEST(!called);
  sig();
  BOOST_TEST(called);
}
