#include <boost/stacktrace.hpp>

int main()
{
  boost::stacktrace::safe_dump_to("./backtrace.dump");
  return 0;
}
