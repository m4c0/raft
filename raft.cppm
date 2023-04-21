export module raft;
import casein;
import quack;

export namespace raft {
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

template <typename Node> class layout {
  static constexpr const auto max_elements = 128;

  quack::instance_layout<element, max_elements> m_il;
  Node m_node;

public:
  explicit layout(quack::renderer *r, Node &&a) : m_il{r}, m_node{a} {}

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
} // namespace raft
