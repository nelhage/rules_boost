#include <boost/rational.hpp>

int main()
{
  boost::rational<int> half(1,2);
  if (boost::rational_cast<double>(half) != 0.5) {
    return 1;
  }

  return 0;
}
