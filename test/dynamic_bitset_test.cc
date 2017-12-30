#include <boost/dynamic_bitset.hpp>

int main()
{
  boost::dynamic_bitset<unsigned short> set;
  return set.empty() ? 0 : 1;
}
