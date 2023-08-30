#include <boost/icl/interval_set.hpp>
#include <boost/icl/interval.hpp>

#include <cassert>

int main()
{

  const unsigned int DOMAIN_MAX = 100;

  using Interval = boost::icl::discrete_interval<unsigned int>;
  
  boost::icl::interval_set<unsigned int> full_domain;  
  full_domain += Interval::right_open(0, DOMAIN_MAX);

  boost::icl::interval_set<unsigned int> working_domain(full_domain);

  for(unsigned int i=0; i<3; ++i)
    working_domain -= Interval::right_open(i*15, i*15 + 7);

  boost::icl::interval_set<unsigned int> inverted_working_domain = boost::icl::flip(full_domain, working_domain);
  
  unsigned int inverted_domain_discrete_cardinality = 0;
  for(auto& range : inverted_working_domain)
    inverted_domain_discrete_cardinality += boost::icl::cardinality(range);
  
  assert(inverted_domain_discrete_cardinality == 21);

  return 0;
}