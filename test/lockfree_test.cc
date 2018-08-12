#include <boost/lockfree/queue.hpp>

int main()
{
  boost::lockfree::queue<int> queue(128);
  return 0;
}
