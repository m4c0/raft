import casein;
import quack;

extern "C" void casein_handle(const casein::event &e) {
  static quack::renderer r{1};
  static quack::instance_layout<bool, 1> il{&r};

  r.process_event(e);
  il.process_event(e);
}
