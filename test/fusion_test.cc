#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/make_list.hpp>

int main()
{
  using namespace boost::fusion;

  if (at_c<0>(as_vector(make_list(1, "asdf"))) != 1)
  {
    return 1;
  }

  return 0;
}
