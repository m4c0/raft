export module raft;
import :elements;
import casein;
import quack;

namespace raft {
class root_group;
class group {
  static group *ms_current;

  group *m_parent{};
  element *m_elem{};
  e_list m_eg{};

  group(int) { ms_current = this; }

  friend class root_group;

protected:
  void for_each(auto &&fn) {
    for (auto &e : m_eg) {
      fn(e.data());
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

    m_elem = m_parent->create_element();
    m_elem->data().hidden = true;
  }
  ~group() { ms_current = m_parent; }

  element *create_element() { return m_eg.create_element(); }

  [[nodiscard]] static group *current() noexcept { return ms_current; }
};
group *group::ms_current{};

export [[nodiscard]] element *create_element() {
  return group::current()->create_element();
}

struct root_group : public group {
  root_group() : group(0) {}
};

// lays out, raii-style
export struct vgroup : public group {
  ~vgroup() {
    for_each([i = 0.0f](auto &d) mutable {
      d.pos = {0, i};
      i += d.size.w;
    });
  }
};
export struct hgroup : public group {
  ~hgroup() {
    for_each([i = 0.0f](auto &d) mutable {
      d.pos = {i, 0};
      i += d.size.h;
    });
  }
};

export template <typename Node> class layout {
  static constexpr const auto max_elements = 20;

  quack::instance_layout<element, max_elements> m_il;
  Node m_node;

  void execute_gui(const casein::event &e) {
    e_stack stack{m_il.data()};
    root_group root{};
    m_node(e);
  }

public:
  explicit layout(quack::renderer *r, Node &&a) : m_il{r}, m_node{a} {}

  void update_layout(const casein::event &e) {
    m_il.reset_grid();
    execute_gui(e);
    // update positions

    m_il.fill_colour([first = &m_il.at(0)](const auto &e) {
      if (e.data().hidden)
        return quack::colour{};

      auto n = (float)(&e - first) / max_elements;
      return quack::colour{0, 0, n, 1};
    });
    m_il.fill_pos([](const auto &e) { return e.data().pos; });
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
