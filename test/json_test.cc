#include <boost/json.hpp>

int main() {
  auto jv = boost::json::parse(R"({"hello": "world"})");
  return jv.as_object().at("hello") != "world";
}
