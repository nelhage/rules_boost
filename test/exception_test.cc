#include <boost/exception/all.hpp>
#include <stdexcept>
#include <iostream>

int main(int, char *[])
{
  try {
    BOOST_THROW_EXCEPTION(std::runtime_error("success"));
  } catch (std::exception &e) {
    std::cout << boost::diagnostic_information(e) << '\n';
    return 0;
  }
  return 1;
}
