#include <boost/nowide/args.hpp>
#include <boost/nowide/convert.hpp>
#include <boost/nowide/cstdio.hpp>
#include <boost/nowide/cstdlib.hpp>
#include <boost/nowide/filebuf.hpp>
#include <boost/nowide/filesystem.hpp>
#include <boost/nowide/fstream.hpp>
#include <boost/nowide/iostream.hpp>
#include <boost/nowide/iostream.hpp>
#include <boost/nowide/replacement.hpp>
#include <boost/nowide/stackstring.hpp>
#include <boost/nowide/stackstring.hpp>
#include <boost/nowide/stat.hpp>
#include <boost/nowide/utf8_codecvt.hpp>

int main(int argc, char **argv)
{
  std::string hello = "\xd7\xa9\xd7\x9c\xd7\x95\xd7\x9d";
  std::wstring whello = boost::nowide::widen(hello);
  boost::nowide::nowide_filesystem();
  return boost::nowide::narrow(whello) == hello ? 0 : 1;
}
