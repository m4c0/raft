import casein;
import quack;

struct element {
  element *next;
  element *parent;
};

class group;
group *&current_group() {
  static group *ptr{};
  return ptr;
}

class group {
  group *m_prev_in_chain;
  unsigned m_child_count{};

public:
  constexpr group(const group &) = delete;
  constexpr group(group &&) = delete;
  constexpr group &operator=(const group &) = delete;
  constexpr group &operator=(group &&) = delete;

  group() {
    m_prev_in_chain = current_group();
    current_group() = this;
  }
  ~group() { current_group() = m_prev_in_chain; }

  void create_element() { m_child_count++; }
};

// lays out, raii-style
struct vgroup : public group {};
struct hgroup : public group {};

// create elements
bool button(const casein::event &e) {
  current_group()->create_element();
  return false;
}
void inputbox(const casein::event &e) { current_group()->create_element(); }
void text(const casein::event &e) { current_group()->create_element(); }

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
    hgroup hg{};

    if (button(e)) {
    }
    text(e);
  }

  void header() const {
    hgroup hg{};

    title();
    input();
  }
  void list() const {
    vgroup vg{};

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
    vgroup vg{};

    header();
    list();
    footer();
    info();
  }
};

template <typename Node> class raft_layout {
  static constexpr const auto max_elements = 128;

  quack::instance_layout<element, max_elements> m_il;
  Node m_node;

public:
  explicit raft_layout(quack::renderer *r, Node &&a) : m_il{r}, m_node{a} {}

  void update_layout(const casein::event &e) {
    m_il.reset_grid();
    m_node(e);
    // update layout (create elements?)
    // fill
    // batch->resize?
  }

  void process_event(const casein::event &e) {
    m_il.process_event(e);
    switch (e.type()) {
    case casein::RESIZE_WINDOW:
      update_layout(e);
      break;
    default:
      break;
    }
  }
};

extern "C" void casein_handle(const casein::event &e) {
  static state s{};

  static quack::renderer r{1};
  static raft_layout rl{&r, [](auto ee) { poc(s, ee).root(); }};

  r.process_event(e);
  rl.process_event(e);
}
