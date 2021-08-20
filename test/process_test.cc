// work-around issue in Boost 1.77
// https://github.com/boostorg/process/pull/215
#include <algorithm>

#include <boost/process.hpp>

int main() 
{
    using namespace boost::process;

    ipstream pipe_stream;
    child c("echo hi", std_out > pipe_stream);

    std::string line;
    std::string all;

    while (pipe_stream && std::getline(pipe_stream, line) && !line.empty())
        all += line + '\n';

    c.wait();

    auto iter = all.find("hi");
    if (iter != std::string::npos) {
        return 0;
    } else {
        return -1;
    }
}
