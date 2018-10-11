#include <string>
#include <boost/hana.hpp>
namespace hana = boost::hana;

struct Fish { std::string name; };
struct Cat  { std::string name; };
struct Dog  { std::string name; };

int main(){
  auto animals = hana::make_tuple(Fish{"Nemo"}, Cat{"Garfield"}, Dog{"Snoopy"});
  return hana::size(animals).value == 3 ? 0 : 1;
}
