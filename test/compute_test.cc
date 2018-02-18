#include <iostream>
#include <stdlib.h>

#include <boost/compute/core.hpp>

namespace compute = boost::compute;

int main()
{
    char *env = getenv("RULES_BOOST_TEST_COMPUTE");
    if(env == nullptr || strlen(env) == 0) {
        std::cerr << "Skipping boost::comput test; Set RULES_BOOST_TEST_COMPUTE=1 to enable." << std::endl;
        return 0;
    }
    // get the default device
    compute::device device = compute::system::default_device();

    // print the device's name and platform
    std::cout << "hello from " << device.name();
    std::cout << " (platform: " << device.platform().name() << ")" << std::endl;

    return 0;
}
