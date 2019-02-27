#include <string>
#include <iostream>

#include <boost/locale/encoding.hpp>
#include <boost/locale/encoding_utf.hpp>

int main(int, char**) {
    std::string utf8_string = boost::locale::conv::to_utf<char>
        ("N""\xf6""el", "iso-8859-1");
    if (utf8_string != "Nöel") {
        std::cerr << "conversion failed!" << std::endl;
        return 1;
    }
    return 0;
}
