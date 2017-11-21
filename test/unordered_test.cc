#include <string>
#include <boost/unordered_map.hpp>

int main()
{
  boost::unordered_map<std::string, int> m;

  m["one"] = 1;

  if (m.at("one") != 1) {
    return 1;
  }

  return 0;
}
