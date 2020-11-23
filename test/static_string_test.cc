#include <boost/static_string/static_string.hpp>

int main()
{
  boost::static_string<5> s1("UVXYZ", 3);
  return s1 == "UVX" ? 0 : 1;
}
