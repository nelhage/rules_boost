#define BOOST_TEST_MODULE scope_exit_test
#include <boost/scope_exit.hpp>
#include <boost/test/included/unit_test.hpp>

void f(bool *flag)
{
  bool commit = false;
  BOOST_SCOPE_EXIT(&commit, &flag) {
    *flag = true;
  } BOOST_SCOPE_EXIT_END
}

BOOST_AUTO_TEST_CASE( test_scope_exit )
{
  bool flag = false;
  f(&flag);
  BOOST_TEST(flag);
}
