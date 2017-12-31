#include <boost/foreach.hpp>
#include <boost/foreach_fwd.hpp>
#include <vector>

int main()
{
  std::vector<int> vec{ 1, 2 };
  int sum = 0;
  BOOST_FOREACH(int n, vec)
  {
    sum += n;
  }

  return sum == 3 ? 0 : 1;
}
