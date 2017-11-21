#include <boost/numeric/conversion/cast.hpp>

int main()
{
  short a = 42;
  int b = 42;

  if (boost::numeric_cast<int>(a) != b) {
    return 1;
  }

  return 0;
}
