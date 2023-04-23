export module raft;
import :elements;
export import :objects;
import casein;
import quack;

namespace raft {
export struct context {
  e_stack stack;
  const casein::event &event;
};

export void hgroup(context *c, auto &&fn) { fn(); }
export void vgroup(context *c, auto &&fn) { fn(); }

export class layout {
  static constexpr const auto max_elements = 20;

  quack::instance_layout<element, max_elements> m_il;
  void (*m_node)(context *);

  void execute_gui(const casein::event &e) {
    context ctx{
        .stack{m_il.data()},
        .event{e},
    };
    m_node(&ctx);
  }

public:
  explicit layout(quack::renderer *r, decltype(m_node) a)
      : m_il{r}, m_node{a} {}

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
    m_il.fill_pos([](const auto &e) { return e.data().area; });
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
