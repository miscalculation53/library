#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/sqrt_decomposition/range_sum.hpp"
#include "math/modint/modint.hpp"

struct Box
{
  ll x = 0;
  friend bool operator==(Box a, Box b) { return a.x == b.x; }
};
struct BoxGroup
{
  using S = Box;
  static S e() { return {}; }
  static S op(S a, S b) { return {a.x + b.x}; }
  static S inv(S a) { return {-a.x}; }
};
struct OrMonoid
{
  using S = ll;
  static S e() { return 0; }
  static S op(S a, S b) { return a | b; }
};

template <class G, int B, bool Set>
void check(int n)
{
  using S = typename G::S;
  mt19937 rng(20260926 + n);
  auto random_value = [&]
  {
    ll x = rng() % 201;
    if constexpr (Set) x -= 100;
    return S{x};
  };
  vc<S> a(n);
  for (auto &x : a) x = random_value();
  SqrtDecompositionRangeSum<G, B> ds(a), zero(n), empty;
  assert(empty.sum(0, 0) == G::e());
  assert(zero.sum(0, n) == G::e());
  repi(i, n) assert(zero.get(i) == G::e());
  auto exhaustive = [&]
  {
    for (int l = 0; l <= n; l++)
    {
      S expected = G::e();
      for (int r = l; r <= n; r++)
      {
        assert(ds.sum(l, r) == expected);
        if (r < n) expected = G::op(expected, a[r]);
      }
    }
  };
  exhaustive();
  if (n == 0) return;
  repi(q, 1000)
  {
    int p = rng() % n;
    S x = random_value();
    if constexpr (Set)
    {
      if (q % 2 == 0) ds.set(p, x), a[p] = x;
      else ds.add(p, x), a[p] = G::op(a[p], x);
    }
    else ds.add(p, x), a[p] = G::op(a[p], x);
    assert(ds.get(p) == a[p]);
    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r) swap(l, r);
    S expected = G::e();
    repi(i, l, r) expected = G::op(expected, a[i]);
    assert(ds.sum(l, r) == expected);
  }
  exhaustive();
}

template <int B>
void check_sizes()
{
  for (int n : {0, 1, 2, 3, 7, 16, 31, B - 1, B, B + 1, 2 * B + 3})
  {
    check<GroupAddSub<ll>, B, true>(n);
    check<GroupAddSub<modint998244353>, B, true>(n);
    check<BoxGroup, B, true>(n);
    check<OrMonoid, B, false>(n);
  }
}

int main()
{
  check_sizes<1>();
  check_sizes<3>();
  check_sizes<8>();
  check_sizes<512>();
  PRINT("Hello World");
}
