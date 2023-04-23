export module raft;
import :elements;
export import :objects;
import casein;
import quack;

namespace raft {
static constexpr const auto max_elements = 20;
using qil = quack::instance_layout<element, max_elements>;

export class context {
  e_stack m_stack;
  area m_rem_area{0, 0, max_elements, max_elements};
  const casein::event &m_event;
  e_list m_group;

public:
  context(qil &il, const casein::event &e) : m_stack{il.data()}, m_event{e} {}

  [[nodiscard]] constexpr auto &event() noexcept { return m_event; }
  [[nodiscard]] constexpr auto &rem_area() noexcept { return m_rem_area; }

  [[nodiscard]] auto &create_element() noexcept {
    auto *e = m_stack.alloc();
    e->data().area = m_rem_area;
    m_group.add_element(e);
    return e->data();
  }
};

export void hgroup(context *c, auto &&fn) {
  fn();
  c->rem_area().x++;
  c->rem_area().w--;
}
export void vgroup(context *c, auto &&fn) {
  fn();
  c->rem_area().y++;
  c->rem_area().h--;
}

export class layout {

  quack::instance_layout<element, max_elements> m_il;
  void (*m_node)(context *);

  void execute_gui(const casein::event &e) {
    context ctx{m_il, e};
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
