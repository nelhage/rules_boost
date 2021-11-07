#include <boost/smart_ptr.hpp>

int main(int, char*[])
{
  boost::scoped_ptr<int> p(new int);
  *p = 0;
  return *p;
}
