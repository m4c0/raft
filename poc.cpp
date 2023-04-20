import casein;
import quack;

struct element {};
struct state {
  bool create_mode;
  int items[40];
};

// lays out, raii-style
struct vgroup {};
struct hgroup {};

// create elements
void inputbox(const casein::event &e) {}
void text(const casein::event &e) {}

class poc {
  state &s;
  const casein::event &e;

  void create_input() { inputbox(e); }
  void search_input() { inputbox(e); }

  void title() { text(e); }
  void input() {
    if (s.create_mode) {
      create_input();
    } else {
      search_input();
    }
  }

  void item(auto i) {}

  void header() {
    hgroup hg{};

    title();
    input();
  }
  void list() {
    vgroup vg{};

    for (auto i : s.items) {
      item(i);
    }
  }
  void footer() {}
  void info() {}

public:
  explicit constexpr poc(state &s, const casein::event &e) : s{s}, e{e} {}
  void root() {
    vgroup vg{};

    header();
    list();
    footer();
    info();
  }
};

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
  static state s{};

  static quack::renderer r{1};
  static raft_layout<1> rl{&r};

  r.process_event(e);
  rl.process_event(e);
}
