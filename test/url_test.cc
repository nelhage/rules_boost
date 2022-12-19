#include <boost/url.hpp>
#include <cassert>

int main()
{
    boost::urls::url_view u2 = boost::urls::parse_uri( "http://www.example.com/#" ).value();
    assert(u2.fragment().empty());
    assert(u2.has_fragment());
}
