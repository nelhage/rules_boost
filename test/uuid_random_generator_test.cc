#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

int main()
{
  boost::uuids::random_generator gen;
  boost::uuids::uuid u = gen();
  return u.is_nil() ? 0 : 1;
}   
