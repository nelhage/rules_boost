#include <boost/scope_exit.hpp>

void f(bool *flag)
{
  bool commit = false;
  BOOST_SCOPE_EXIT(&commit, &flag) {
    *flag = true;
  } BOOST_SCOPE_EXIT_END
}

int main()
{
  bool flag = false;
  f(&flag);

  if (!flag) {
    return 1;
  }

  return 0;
}
