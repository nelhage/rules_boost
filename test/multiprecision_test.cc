#include <boost/multiprecision/cpp_int.hpp>

int main()
{
  boost::multiprecision::uint128_t a(123);
  int b = 123;
  if (a != b) {
    return 1;
  }

  return 0;
}
