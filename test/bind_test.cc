#include <boost/bind/bind.hpp>

int f(int a, int b)
{
  return a + b;
}

int main()
{
  if (boost::bind(f, 1, 2)() != 3) {
    return 1;
  }

  return 0;
}
