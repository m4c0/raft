import casein;
import quack;
import raft;

// create elements
bool button(auto) { return false; }
void inputbox(auto) {}
void text(auto) {}

struct state {
  bool create_mode;
  int items[4];
};
class poc {
  state &s;
  raft::context *c;

  void create_input() const { inputbox(c); }
  void search_input() const { inputbox(c); }

  void title() const { text(c); }
  void input() const {
    if (s.create_mode) {
      create_input();
    } else {
      search_input();
    }
  }

  void item(auto i) const {
    raft::hgroup(c, [this] {
      if (button(c)) {
      }
      text(c);
    });
  }

  void header() const {
    raft::hgroup(c, [this] {
      title();
      input();
    });
  }
  void list() const {
    raft::vgroup(c, [this] {
      for (auto i : s.items) {
        item(i);
      }
    });
  }
  void footer() const { text(c); }
  void info() const {
    if (s.create_mode) {
      text(c);
    } else {
      text(c);
    }
  }

public:
  explicit constexpr poc(state &s, raft::context *c) : s{s}, c{c} {}
  void root() {
    raft::vgroup(c, [this] {
      header();
      list();
      footer();
      info();
    });
  }
};

extern "C" void casein_handle(const casein::event &e) {
  static state s{};

  static quack::renderer r{1};
  static raft::layout rl{&r, [](raft::context *c) { poc{s, c}.root(); }};

  r.process_event(e);
  rl.process_event(e);
}
