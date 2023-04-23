export module raft:objects;
import quack;

namespace raft {
export using pos = quack::pos;
export using size = quack::size;
export using area = quack::rect;

export [[nodiscard]] constexpr size &size_of(area &a) noexcept { return a; }

static_assert([] {
  area a{1, 2, 3, 4};
  size &s = size_of(a);

  if (s.w != 3)
    return false;

  s.w = 5;
  if (s.w != 5)
    return false;
  if (a.w != 5)
    return false;

  return true;
}());
} // namespace raft
