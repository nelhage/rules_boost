#include <boost/container/allocator.hpp>
#include <boost/container/flat_map.hpp>

int main()
{
  boost::container::flat_map<int, int, std::less<int>, boost::container::allocator<std::pair<int, int>>> m;
  m.emplace(1, 42);
  m.emplace(2, 0);
  return m.at(2);
}
