import casein;
import quack;

struct element {};

template <unsigned MaxElements> class raft_layout {
  quack::instance_layout<element, MaxElements> m_il;

public:
  explicit raft_layout(quack::renderer *r) : m_il{r} {}

  void update_layout() {
    m_il.reset_grid();
    // update layout (create elements?)
    // fill
    // batch->resize?
  }

  void process_event(const casein::event &e) {
    m_il.process_event(e);
    switch (e.type()) {
    case casein::RESIZE_WINDOW:
      update_layout();
      break;
    default:
      break;
    }
  }
};

extern "C" void casein_handle(const casein::event &e) {
  static quack::renderer r{1};
  static raft_layout<1> rl{&r};

  r.process_event(e);
  rl.process_event(e);
}
