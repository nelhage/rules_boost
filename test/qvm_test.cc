#include <boost/qvm/all.hpp>

int main() {
  boost::qvm::quat<double> rx = boost::qvm::rotx_quat(3.14159265358979323846);
  double v[3] = {1, 1, 0};
  boost::qvm::vec<double, 3> w = rx*boost::qvm::vref(v);
  return boost::qvm::A<1>(w) != -1;
}
