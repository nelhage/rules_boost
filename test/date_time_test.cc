#include <boost/date_time.hpp>

int main()
{
  std::string ud("20200131");
  boost::gregorian::date d1(boost::gregorian::from_undelimited_string(ud));
  return to_iso_extended_string(d1) == "2020-01-31" ? 0 : 1;
}
