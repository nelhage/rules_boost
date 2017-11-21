#include <string>
#include <boost/tr1/unordered_map.hpp>

int main()
{
  std::tr1::unordered_map<std::string, int> m;

  m["one"] = 1;

  if (m.at("one") != 1) {
    return 1;
  }

  return 0;
}
