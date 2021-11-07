#include <boost/throw_exception.hpp>
#include <stdexcept>

int main(int, char *[])
{
  try {
    BOOST_THROW_EXCEPTION(std::runtime_error("success"));
  } catch (const std::runtime_error &) {
    return 0;
  }
  return 1;
}
