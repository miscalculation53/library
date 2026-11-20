#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/cumulative_sum/sparse_imos.hpp"

using G = GroupAddSub<ll>;

template <class I>
void test_wide(I n)
{
  SparseImos<G, I> a(n, 4);
  a.add(0, 0, 10);
  a.add(n, n, 10);
  a.add(10, n, 3);
  a.add(n - 2, n - 1, -5);
  using Interval = tuple<I, I, ll>;
  const vc<Interval> snapshot{{0, 10, 4}, {10, n - 2, 7}, {n - 2, n - 1, 2}, {n - 1, n, 7}};
  assert(as_const(a).content() == snapshot);
  auto copy = a;
  a.add(0, n, -1);
  assert((a.content() == vc<Interval>{{0, 10, 3}, {10, n - 2, 6}, {n - 2, n - 1, 1}, {n - 1, n, 6}}));
  assert(copy.content() == snapshot);
}

void test_random()
{
  constexpr int n = 73;
  SparseImos<G, int> a(n, -3);
  mt19937 rng(5701);
  vc<ll> values(n, -3);
  vc<bool> boundary(n + 1);
  boundary[n] = true;
  repi(iter, 1200)
  {
    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (r < l) swap(l, r);
    ll w = ll(rng() % 21) - 10;
    a.add(l, r, w);
    repi(i, l, r) values[i] += w;
    if (l < r) boundary[l] = boundary[r] = true;

    if (iter % 20 != 0 && iter != 1199) continue;
    int p = 0;
    for (auto [left, right, value] : as_const(a).content())
    {
      assert(left == p && left < right && right <= n && boundary[right]);
      repi(i, left, right)
      {
        assert(i == left || !boundary[i]);
        assert(value == values[i]);
      }
      p = right;
    }
    assert(p == n);
  }
}

int main()
{
  SparseImos<G> empty;
  empty.add(0, 0, 1);
  assert(as_const(empty).content().empty());

  SparseImos<G> cancelled(10);
  cancelled.add(3, 7, 5);
  cancelled.add(3, 7, -5);
  assert((cancelled.content() == vc<tuple<ll, ll, ll>>{{0, 3, 0}, {3, 7, 0}, {7, 10, 0}}));

  test_wide((i128)1 << 100);
  test_wide((u128)1 << 127);
  test_random();
  PRINT("Hello World");
}
