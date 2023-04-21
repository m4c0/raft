export module raft;
import casein;
import quack;

namespace raft {
struct element {
  element *next;
  element *parent;
};
element *&next_element() {
  static element *ptr{};
  return ptr;
}

class group;
export group *&current_group() {
  static group *ptr{};
  return ptr;
}

class group {
  group *m_prev_in_chain;
  element *m_tail{};

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

  void create_element() {
    auto *e = next_element()++;

    if (m_tail == nullptr) {
      m_tail = e;
    } else {
      m_tail->next = e;
      m_tail = e;
    }
  }
};

// lays out, raii-style
export struct vgroup : public group {};
export struct hgroup : public group {};

export template <typename Node> class layout {
  static constexpr const auto max_elements = 16;

  quack::instance_layout<element, max_elements> m_il;
  Node m_node;

public:
  explicit layout(quack::renderer *r, Node &&a) : m_il{r}, m_node{a} {}

  void update_layout(const casein::event &e) {
    m_il.reset_grid();
    next_element() = &m_il.at(0);

    m_node(e);
    // update layout (create elements?)
    // fill
    // batch->resize?
    m_il.fill_colour([first = &m_il.at(0)](const auto &e) {
      return quack::colour{0, 1, (float)(e.next - first) / max_elements, 1};
    });
    m_il.batch()->positions().map([](auto *c) {
      for (auto i = 0; i < max_elements; i++) {
        c[i] = {0, (float)i};
      }
    });
    m_il.batch()->resize(1, max_elements, 1, max_elements);
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
