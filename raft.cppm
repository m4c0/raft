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
  element *m_parent{};
  element *m_tail{};

public:
  constexpr group(const group &) = delete;
  constexpr group(group &&) = delete;
  constexpr group &operator=(const group &) = delete;
  constexpr group &operator=(group &&) = delete;

  group() {
    m_prev_in_chain = current_group();
    current_group() = this;

    if (m_prev_in_chain != nullptr) {
      m_parent = m_prev_in_chain->create_element();
    }
  }
  ~group() { current_group() = m_prev_in_chain; }

  element *create_element() {
    auto *e = next_element()++;
    e->parent = m_parent;

    if (m_tail == nullptr) {
      m_tail = e;
    } else {
      m_tail->next = e;
      m_tail = e;
    }
    return e;
  }
};

// lays out, raii-style
export struct vgroup : public group {};
export struct hgroup : public group {};

export template <typename Node> class layout {
  static constexpr const auto max_elements = 20;

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
      auto p = (float)(e.parent - first) / max_elements;
      auto n = (float)(e.next - first) / max_elements;
      auto r = e.parent == 0 ? 1.0f : 0.0f;
      return quack::colour{r, p, n, 1};
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