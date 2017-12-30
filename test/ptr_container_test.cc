#include <string>

#include <boost/ptr_container/clone_allocator.hpp>
#include <boost/ptr_container/exception.hpp>
#include <boost/ptr_container/indirect_fun.hpp>
#include <boost/ptr_container/nullable.hpp>
#include <boost/ptr_container/ptr_array.hpp>
#include <boost/ptr_container/ptr_circular_buffer.hpp>
#include <boost/ptr_container/ptr_container.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/ptr_container/ptr_inserter.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_map_adapter.hpp>
#include <boost/ptr_container/ptr_sequence_adapter.hpp>
#include <boost/ptr_container/ptr_set.hpp>
#include <boost/ptr_container/ptr_set_adapter.hpp>
#include <boost/ptr_container/ptr_unordered_map.hpp>
#include <boost/ptr_container/ptr_unordered_set.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/serialize_ptr_array.hpp>
#include <boost/ptr_container/serialize_ptr_circular_buffer.hpp>
#include <boost/ptr_container/serialize_ptr_container.hpp>
#include <boost/ptr_container/serialize_ptr_deque.hpp>
#include <boost/ptr_container/serialize_ptr_list.hpp>
#include <boost/ptr_container/serialize_ptr_map.hpp>
#include <boost/ptr_container/serialize_ptr_set.hpp>
#include <boost/ptr_container/serialize_ptr_unordered_map.hpp>
#include <boost/ptr_container/serialize_ptr_unordered_set.hpp>
#include <boost/ptr_container/serialize_ptr_vector.hpp>

int main()
{
    boost::ptr_map<std::string, int> map;
    return map.empty() ? 0 : 1;
}   
