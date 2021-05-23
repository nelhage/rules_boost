#include <boost/interprocess/anonymous_shared_memory.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

int main()
{
  boost::interprocess::named_mutex mtx{boost::interprocess::open_or_create,
                                       "rules_boost_interprocess_test"};
  mtx.lock();
  mtx.unlock();
  return 0;
}
