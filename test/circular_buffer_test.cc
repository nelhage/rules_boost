#include <boost/circular_buffer.hpp>

int main()
{
  boost::circular_buffer<int> cb(1);
  cb.push_back(1);

  if (cb[0] != 1) {
    return 1;
  }

  return 0;
}
