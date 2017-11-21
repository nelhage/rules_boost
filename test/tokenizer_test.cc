#include <string>
#include <boost/tokenizer.hpp>

int main()
{
  std::string s = "foo bar";
  boost::tokenizer<> tok(s);

  if (*(tok.begin()) != "foo") {
    return 1;
  }

  return 0;
}
