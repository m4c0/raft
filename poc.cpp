import casein;
import quack;
import raft;

// create elements
bool button(const casein::event &e) {
  raft::current_group()->create_element();
  return false;
}
void inputbox(const casein::event &e) {
  raft::current_group()->create_element();
}
void text(const casein::event &e) { raft::current_group()->create_element(); }

struct state {
  bool create_mode;
  int items[4];
};
class poc {
  state &s;
  const casein::event &e;

  void create_input() const { inputbox(e); }
  void search_input() const { inputbox(e); }

  void title() const { text(e); }
  void input() const {
    if (s.create_mode) {
      create_input();
    } else {
      search_input();
    }
  }

  void item(auto i) const {
    raft::hgroup hg{};

    if (button(e)) {
    }
    text(e);
  }

  void header() const {
    raft::hgroup hg{};

    title();
    input();
  }
  void list() const {
    raft::vgroup vg{};

    for (auto i : s.items) {
      item(i);
    }
  }
  void footer() const { text(e); }
  void info() const {
    if (s.create_mode) {
      text(e);
    } else {
      text(e);
    }
  }

public:
  explicit constexpr poc(state &s, const casein::event &e) : s{s}, e{e} {}
  void root() {
    raft::vgroup vg{};

    header();
    list();
    footer();
    info();
  }
};

extern "C" void casein_handle(const casein::event &e) {
  static state s{};

  static quack::renderer r{1};
  static raft::layout rl{&r, [](auto ee) { poc(s, ee).root(); }};

  r.process_event(e);
  rl.process_event(e);
}
