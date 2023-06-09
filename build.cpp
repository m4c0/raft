#include "build.hpp"

int main(int argc, char **argv) {
  using namespace ecow;

  auto poc = unit::create<app>("poc");
  poc->add_requirement(native);
  poc->add_wsdep("casein", casein());
  poc->add_wsdep("quack", quack());
  poc->add_ref(raft());
  poc->add_unit<>("poc");
  return run_main(poc, argc, argv);
}
