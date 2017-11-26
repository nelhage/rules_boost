#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;

int main()
{
  ptree pt;
  std::stringstream ss;
  pt.put("k", "v");
  json_parser::write_json(ss, pt, false);
  return ss.str() != "{\"k\":\"v\"}\n";
}
