#include <boost/pfr.hpp>

struct Foo {
  int bar = 1;
  int baz = 0;
};

int main() {
  Foo f{};
  return boost::pfr::get<1>(f);
}
