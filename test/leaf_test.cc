#include <boost/leaf.hpp>

enum class Errors {
  Unimplemented,
};

boost::leaf::result<int> foo() {
  return boost::leaf::new_error(Errors::Unimplemented);
}

int main() {
  return boost::leaf::try_handle_all(
      foo, [](boost::leaf::match<Errors, Errors::Unimplemented>) { return 0; },
      [](boost::leaf::error_info const &unmatched) { return 2; });
}
