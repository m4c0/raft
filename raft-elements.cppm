export module raft:elements;
import quack;

namespace raft {
class element {
  element *m_next{};
  element *m_firstborn{};
  quack::pos m_pos{};

public:
  constexpr element() noexcept = default;

  [[nodiscard]] constexpr element *next() noexcept { return m_next; }
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

class e_node {
  element *m_parent{};
  element *m_head{};
  element *m_tail{};

public:
  constexpr e_node() = default;

  [[nodiscard]] constexpr e_iterator begin() noexcept { return {m_head}; }
  [[nodiscard]] constexpr e_iterator end() noexcept { return {}; }
};
} // namespace raft
