#include <boost/assign.hpp>
#include <boost/assign/ptr_map_inserter.hpp>
#include <boost/assign/ptr_list_inserter.hpp>
#include <boost/assign/ptr_list_of.hpp>
#include <list>
 
int main()
{
    const std::list<int> two = boost::assign::list_of(1)(2);
    return two.size() == 2 ? 0 : 1;
}   
