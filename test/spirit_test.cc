// Example is based on num_list1.cpp from the boost.spirit repo:
// https://github.com/boostorg/spirit/blob/develop/example/x3/num_list/num_list1.cpp

#include <boost/spirit/home/x3.hpp>
#include <string>

namespace x3 = boost::spirit::x3;

int main()
{
    std::string csl("1,2,3,4");

    auto first = csl.begin();
    auto last = csl.end();

    auto res = x3::phrase_parse(first, last, x3::double_ >> *(',' >> x3::double_), x3::ascii::space);

    return res && (first == last) ? 0 : 1;
}