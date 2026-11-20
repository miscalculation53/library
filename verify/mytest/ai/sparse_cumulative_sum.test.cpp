#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/cumulative_sum/sparse_cumulative_sum.hpp"

struct PermutationGroup
{
  using S = array<int, 3>;
  static S e() { return {0, 1, 2}; }
  static S op(const S &p, const S &q) { return {p[q[0]], p[q[1]], p[q[2]]}; }
  static S inv(const S &p)
  {
    S q;
    repi(i, 3) q[p[i]] = i;
    return q;
  }
};

void test_boundaries()
{
  using G = GroupAddSub<ll>;
  const SparseCumulativeSum<G> empty;
  assert(empty.sum(0) == 0);
  assert(empty.sum(-100, 100) == 0);
  assert(empty.content().empty());

  const SparseCumulativeSum<G> sums({{12, 7}, {-4, 2}, {12, 3}, {1, 8}, {1, -8}});
  assert(sums.sum(-4) == 0);
  assert(sums.sum(12) == 2);
  assert(sums.sum(0, 13) == 10);
  assert(sums.sum(12, 12) == 0);
  assert(sums.sum(13, 18) == 0);
  assert((sums.content() == vc<pair<ll, ll>>{{-4, 2}, {1, 0}, {12, 10}}));
}

template <class I>
void test_order(I base)
{
  using G = PermutationGroup;
  using S = G::S;
  constexpr int n = 87;
  const vc<S> transpositions{{1, 0, 2}, {0, 2, 1}, {2, 1, 0}};
  mt19937 rng(14857);
  vc<pair<I, S>> points;
  vc<S> a(n, G::e());
  vc<bool> present(n);
  repi(i, 1000)
  {
    int x = 1 + rng() % (n / 2 - 1) * 2;
    const S &w = transpositions[rng() % transpositions.size()];
    points.eb(base + x, w);
    a[x] = G::op(a[x], w);
    present[x] = true;
  }

  const SparseCumulativeSum<G, I> sums(points);
  auto copy = sums;
  const auto moved = std::move(copy);
  vc<pair<I, S>> expected;
  repi(x, n)
    if (present[x]) expected.eb(base + x, a[x]);
  assert(sums.content() == expected);
  assert(moved.content() == expected);

  repi(i, 1500)
  {
    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (r < l) swap(l, r);
    S prefix = G::e(), range = G::e();
    repi(p, r) prefix = G::op(prefix, a[p]);
    repi(p, l, r) range = G::op(range, a[p]);
    assert(sums.sum(base + r) == prefix);
    assert(sums.sum(base + l, base + r) == range);
  }
}

int main()
{
  test_boundaries();
  test_order(-((i128)1 << 100));
  test_order((u128)1 << 127);
  PRINT("Hello World");
}
