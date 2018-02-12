#include <boost/mp11.hpp>
#include <string>

using namespace boost::mp11;

struct Functor {

    std::string& a;

    void operator()(char) {
        a += "char";
    }

    void operator()(int) {
        a += "int";
    }

    void operator()(long) {
        a += "long";
    }
};


int main()
{
    std::string ret;
    Functor f{ret};

    using L = std::tuple<char, int, long>;
    tuple_for_each(L(), f);

    return ret == "charintlong" ? 0 : -1;
}

