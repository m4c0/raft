#include "../casein/build.hpp"
#include "../ecow/ecow.hpp"
#include "../quack/build.hpp"

auto raft() {
  using namespace ecow;

  auto m = unit::create<mod>("raft");
  m->add_wsdep("casein", casein());
  m->add_wsdep("quack", quack());
  return m;
}
