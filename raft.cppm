export module raft;
import :elements;
import casein;
import quack;

namespace raft {
class group {
  static group *ms_current;

  group *m_parent;
  e_list m_eg{};

protected:
  void for_each(auto &&fn) {
    for (auto &e : m_eg) {
      fn(e);
    }
  }

public:
  constexpr group(const group &) = delete;
  constexpr group(group &&) = delete;
  constexpr group &operator=(const group &) = delete;
  constexpr group &operator=(group &&) = delete;

  group() {
    m_parent = ms_current;
    ms_current = this;
  }
  ~group() { ms_current = m_parent; }

  element *create_element() { return m_eg.create_element(); }

  [[nodiscard]] static group *current() noexcept { return ms_current; }
};
group *group::ms_current{};

export [[nodiscard]] element *create_element() {
  return group::current()->create_element();
}

// lays out, raii-style
export struct vgroup : public group {
  ~vgroup() {
    for_each([i = 0](auto &e) mutable {
      e.pos({0, (float)i});
      i++;
    });
  }
};
export struct hgroup : public group {
  ~hgroup() {
    for_each([i = 0](auto &e) mutable {
      e.pos({(float)i, 0});
      i++;
    });
  }
};

export template <typename Node> class layout {
  static constexpr const auto max_elements = 20;

  quack::instance_layout<element, max_elements> m_il;
  Node m_node;

  void execute_gui(const casein::event &e) {
    e_stack stack{m_il.data()};
    m_node(e);
  }

public:
  explicit layout(quack::renderer *r, Node &&a) : m_il{r}, m_node{a} {}

  void update_layout(const casein::event &e) {
    m_il.reset_grid();
    execute_gui(e);
    // update positions

    m_il.fill_colour([first = &m_il.at(0)](const auto &e) {
      auto n = (float)(&e - first) / max_elements;
      return quack::colour{0, 0, n, 1};
    });
    m_il.fill_pos([](const auto &e) { return e.pos(); });
    m_il.batch()->resize(max_elements, max_elements, max_elements,
                         max_elements);
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
