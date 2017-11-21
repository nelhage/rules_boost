#include <boost/format.hpp>

int main()
{
  if ((boost::format("hello %d") % 1).str() != "hello 1") {
    return 1;
  }

  return 0;
}
