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
  [[nodiscard]] constexpr auto pos() const noexcept { return m_pos; }

  constexpr void pos(quack::pos p) noexcept { m_pos = p; }
};

class e_iterator {
  element *m_ref{};

public:
  constexpr e_iterator() noexcept = default;
  constexpr e_iterator(element *r) noexcept : m_ref{r} {}

  [[nodiscard]] constexpr element &operator*() noexcept { return *m_ref; }
  [[nodiscard]] constexpr e_iterator operator++() noexcept {
    return {m_ref->next()};
  }
  [[nodiscard]] constexpr bool operator==(const e_iterator &o) const noexcept {
    return m_ref == o.m_ref;
  }
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

class e_group {
  element *m_parent{};
  element *m_head{};
  element *m_tail{};

public:
  [[nodiscard]] constexpr e_iterator begin() noexcept { return {m_head}; }
  [[nodiscard]] constexpr e_iterator end() noexcept { return {}; }

  [[nodiscard]] element *create_element() noexcept {
    auto e = e_stack::instance().alloc();
    if (m_tail == nullptr) {
      m_head = e;
      m_tail = e;
    } else {
      m_tail->m_next = e;
      m_tail = e;
    }
    return e;
  }
};
} // namespace raft
