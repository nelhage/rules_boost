#include <boost/poly_collection/base_collection.hpp>

struct A {
    virtual ~A() = default;
};
struct B : A {};
struct C : A {};

int main()
{
  boost::base_collection<A> collection;
  collection.insert(B());
  collection.insert(C());
  return !(collection.size() == 2);
}
