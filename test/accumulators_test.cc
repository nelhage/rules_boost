#include <iostream>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/mean.hpp>

using namespace boost::accumulators;
const int N = 5;
const int M = 2;

int main()
{
  accumulator_set<int, features<tag::count, tag::mean>> acc;

  for (int i = 0; i < N; i++) {
    acc(i);
  }

  return count(acc) == N && mean(acc) == M ? 0 : -1;
}
