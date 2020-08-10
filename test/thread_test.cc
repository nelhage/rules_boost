#include <boost/thread.hpp>

void thread()
{
    boost::thread_specific_ptr<int> ptr;
    if (!ptr.get()) {
        ptr.reset(new int);
    }
    *ptr = 17;
}

int main()
{
  boost::thread t{thread};
  t.join();
  return 0;
}
