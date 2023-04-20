#include "build.hpp"

int main(int argc, char **argv) {
  using namespace ecow;

  auto poc = unit::create<app>("poc");
  poc->add_unit<>("poc");
  poc->add_wsdep("casein", casein());
  poc->add_wsdep("quack", quack());
  return run_main(poc, argc, argv);
}
