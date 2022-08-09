#include <boost/flyweight.hpp>

int main()
{
  boost::flyweight<int> s1(1);
  boost::flyweight<int> s2(1);

  if (&s1.get() != &s2.get())
  {
    return 1;
  }

  return 0;
}
