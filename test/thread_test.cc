#include <boost/thread.hpp>

void thread()
{
}

int main()
{
  boost::thread t{thread};
  t.join();
  return 0;
}
