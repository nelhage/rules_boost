// Example originates from the boost.context repo: 
// https://github.com/boostorg/context/blob/develop/example/callcc/fibonacci.cpp

#include <cstdlib>
#include <iostream>
#include <memory>

#include <boost/context/continuation.hpp>

namespace ctx = boost::context;

int main() {
    int a;
    ctx::continuation c=ctx::callcc(
        [&a](ctx::continuation && c){
            a=0;
            int b=1;
            for(;;){
                c=c.resume();
                int next=a+b;
                a=b;
                b=next;
            }
            return std::move( c);
        });
    for ( int j = 0; j < 10; ++j) {
        std::cout << a << " ";
        c=c.resume();
    }
    std::cout << std::endl;
    std::cout << "main: done" << std::endl;
    return EXIT_SUCCESS;
}