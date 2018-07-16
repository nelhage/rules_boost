#include <boost/numeric/odeint.hpp>

typedef std::vector< double > state_type;

const double gam = 0.15;

void harmonic_oscillator( const state_type &x , state_type &dxdt , const double /* t */ )
{
    dxdt[0] = x[1];
    dxdt[1] = -x[0] - gam*x[1];
}

int main()
{
  state_type x(2);
  x[0] = 1.0; // start at x=1.0, p=0.0
  x[1] = 0.0;  

  using namespace boost::numeric::odeint;
  size_t steps = integrate( harmonic_oscillator, x, 0.0, 10.0, 0.1 );
  if(steps == 0)
    return 1;

  return 0;
}
