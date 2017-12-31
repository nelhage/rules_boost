#include <boost/bimap.hpp>

#include <cassert>
#include <string>

int main()
{
    using boost::bimap;

    bimap<std::string, int> map;

    map.insert({"4", 4});

    auto iter = map.right.find(4);

    if (iter == map.right.end()) {
        return -1;
    }

    return iter->second == "4" ? 0 : -1;
}
