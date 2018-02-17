#include <iostream>

#include <boost/compute/core.hpp>

namespace compute = boost::compute;

int main()
{
    // get the default device
    compute::device device = compute::system::default_device();

    // print the device's name and platform
    std::cout << "hello from " << device.name();
    std::cout << " (platform: " << device.platform().name() << ")" << std::endl;

    return 0;
}