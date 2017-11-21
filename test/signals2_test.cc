#include <boost/signals2.hpp>

class FlagSetter
{
 public:
  FlagSetter(bool *flag) : flag(flag) {}

  void operator()()
  {
    *flag = true;
  }

  bool *flag;
};

int main()
{
  bool called = false;

  boost::signals2::signal<void ()> sig;
  FlagSetter fs (&called);
  sig.connect(fs);

  sig();

  if (!called) {
    return 1;
  }

  return 0;
}
