#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"

struct CopyOnly
{
  int value;
  CopyOnly() = delete;
  explicit CopyOnly(int value) : value(value) {}
  CopyOnly(const CopyOnly &) = default;
  CopyOnly &operator=(const CopyOnly &) = delete;
};

int main()
{
  int calls = 0;
  vc<ll> actual;
  rep(i, (++calls, 5))
  {
    static_assert(is_same_v<decltype(i), ll>);
    if (i == 1) continue;
    actual.push_back(i);
  }
  assert(calls == 1 && actual == vc<ll>({0, 2, 3, 4}));
  actual.clear();
  rep(i, (++calls, 7), (++calls, -2), (++calls, -3)) actual.push_back(i);
  assert(calls == 4 && actual == vc<ll>({7, 4, 1}));
  int count = 0;
  repi(i, 4) repi(j, 5)
  {
    static_assert(is_same_v<decltype(i), int> && is_same_v<decltype(j), int>);
    if (j == 2) break;
    ++count;
  }
  assert(count == 8);
  rep(i, 0, 8)
  {
    i += 2;
    if (i == 5) break;
  }
  auto a = dvec({2, 3, 4}, ll(7));
  static_assert(is_same_v<decltype(a), vc<vc<vc<ll>>>>);
  assert(a.size() == 2 && a[0].size() == 3 && a[0][0].size() == 4);
  a[0][0][0] = 100;
  assert(a[1][0][0] == 7 && a[0][1][0] == 7);
  auto b = dvec<int, 3, 1>({2, 3, 4}, 8);
  assert(b == vc<vc<int>>(3, vc<int>(4, 8)));
  assert((dvec<int, 1, 2>({9}, 6) == 6));
  auto empty = dvec({3, 0, 4}, string("leaf"));
  assert(empty.size() == 3 && empty[0].empty());
  auto copies = dvec({2, 3}, CopyOnly(7));
  copies[0][0].value = 100;
  assert(copies[0][1].value == 7 && copies[1][0].value == 7);
  const ll empty_shape[]{2, 0, numeric_limits<ll>::max()};
  auto stopped = dvec(empty_shape, CopyOnly(7));
  assert(stopped.size() == 2 && stopped[0].empty() && stopped[1].empty());
  PRINT("Hello World");
}
