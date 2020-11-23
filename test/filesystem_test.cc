#include <boost/filesystem.hpp>

int main(int argc, char* argv[])
{
  boost::filesystem::path p;
  p /= "comp1";
  p /= "comp2";
  return p.compare(std::string{"comp1/comp2"});
}
