export module raft:elements;
import quack;

namespace raft {
class e_group;
class element {
  element *m_next{};
  element *m_firstborn{};
  quack::pos m_pos{};

  friend class e_group;

public:
  constexpr element() noexcept = default;

  [[nodiscard]] constexpr element *next() noexcept { return m_next; }
  constexpr void next(element *n) noexcept { m_next = n; }

  [[nodiscard]] constexpr auto pos() const noexcept { return m_pos; }
  constexpr void pos(quack::pos p) noexcept { m_pos = p; }
};

class buffer_overflow {};
class e_stack {
  static e_stack *ms_instance;

  element *m_alloc_ptr{};
  element *m_alloc_limit{};

public:
  template <unsigned N> explicit e_stack(element (&data)[N]) {
    m_alloc_ptr = &data[0];
    m_alloc_limit = &data[N];
    ms_instance = this;
  }
  [[nodiscard]] element *alloc() {
    if (m_alloc_ptr == m_alloc_limit)
      throw buffer_overflow{};

    return m_alloc_ptr++;
  }

  [[nodiscard]] static e_stack &instance() noexcept { return *ms_instance; }
};
e_stack *e_stack::ms_instance = nullptr;

class e_iterator {
  element *m_ref{};

public:
  constexpr e_iterator() noexcept = default;
  constexpr e_iterator(element *r) noexcept : m_ref{r} {}

  [[nodiscard]] constexpr element &operator*() noexcept { return *m_ref; }
  constexpr e_iterator operator++() noexcept {
    m_ref = m_ref->next();
    return *this;
  }

  [[nodiscard]] constexpr bool operator==(const e_iterator &o) const noexcept {
    return m_ref == o.m_ref;
  }
};
static_assert([] {
  element e0{};
  element e1{};
  e0.next(&e1);

  if (&*e_iterator{&e0} != &e0)
    return false;
  if (&*++e_iterator{&e0} != &e1)
    return false;
  if (e_iterator{&e0} != e_iterator{&e0})
    return false;
  if (e_iterator{&e1} != e_iterator{&e1})
    return false;
  if (++e_iterator{&e1} != e_iterator{})
    return false;

  for (e_iterator it{&e0}; it != e_iterator{}; ++it) {
    // Does this explode?
  }

  return true;
}());

class e_group {
  element *m_parent{};
  element *m_head{};
  element *m_tail{};

public:
  [[nodiscard]] constexpr e_iterator begin() noexcept { return {m_head}; }
  [[nodiscard]] constexpr e_iterator end() noexcept { return {}; }

  constexpr void add_element(element *e) noexcept {
    if (m_tail == nullptr) {
      m_head = e;
      m_tail = e;
    } else {
      m_tail->m_next = e;
      m_tail = e;
    }
  }
  [[nodiscard]] element *create_element() noexcept {
    auto e = e_stack::instance().alloc();
    add_element(e);
    return e;
  }
};

static_assert([] {
  element e[4]{};

  e_group grp{};
  grp.add_element(&e[0]);
  grp.add_element(&e[1]);
  grp.add_element(&e[2]);
  grp.add_element(&e[3]);

  if (e[0].next() != &e[1])
    return false;
  if (e[1].next() != &e[2])
    return false;
  if (e[2].next() != &e[3])
    return false;
  if (e[3].next() != nullptr)
    return false;

  for (auto it = grp.begin(); it != grp.end(); ++it) {
    // Do we iterate or do we explode?
  }
  for (auto &i : grp) {
    // Do we iterate or do we explode?
  }

  return true;
}());
} // namespace raft
