#include <boost/any.hpp>

int main()
{
  boost::any a = 1;

  if (boost::any_cast<int>(a) != 1)
  {
    return 1;
  }

  return 0;
}
