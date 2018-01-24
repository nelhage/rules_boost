#include <boost/logic/tribool.hpp>

int main()
{
  boost::logic::tribool a(false);

  if (a == true) {
    return 1;
  } else if(a == false) {
      return 0;
  } else {
      return 2;
  }

}
