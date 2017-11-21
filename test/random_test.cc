#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

int main()
{
  boost::random::mt19937 gen;
  boost::random::uniform_int_distribution<> dist(1, 6);

  if (dist(gen) > 6) {
    return 1;
  }

  return 0;
}
