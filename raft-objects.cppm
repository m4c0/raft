export module raft:objects;
import quack;

namespace raft {
using pos = quack::pos;
using size = quack::size;
using area = quack::rect;

[[nodiscard]] constexpr const size &size_of(area &a) noexcept { return a; }

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
