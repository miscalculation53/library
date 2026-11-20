#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/segtree/dual_segtree.hpp"
#include "ds/segtree/sparse_dual_segtree.hpp"

struct AffineAction final
{
  static constexpr int mod = 1009;
  struct S
  {
    int value;
    explicit S(int value) : value(value) {}
    CPP_DUMP_DEFINE_DATA(value);
  };
  struct F
  {
    int b, c;
    F(int b, int c) : b(b), c(c) {}
    CPP_DUMP_DEFINE_DATA(b, c);
  };
  static S mapping(const F &f, const S &x) { return S((f.b * x.value + f.c) % mod); }
  static F composition(const F &f, const F &g) { return {f.b * g.b % mod, (f.b * g.c + f.c) % mod}; }
  static F id() { return {1, 0}; }
};

void test_against_dense()
{
  using A = AffineAction;
  for (int n : {1, 2, 3, 33, 73})
  {
    mt19937 rng(13815 + n);
    vc<A::S> a(n, A::S(17));
    SparseDualSegmentTree<A> sparse(n, a.front());
    DualSegmentTree<A> dense(a);
    size_t updates = 0;
    repi(iter, 2500)
    {
      int p = rng() % n, l = rng() % (n + 1), r = rng() % (n + 1);
      if (r < l) swap(l, r);
      A::F f(rng() % A::mod, rng() % A::mod);
      switch (rng() % 4)
      {
      case 0:
        sparse.apply(l, r, f);
        dense.apply(l, r, f);
        repi(i, l, r) a[i] = A::mapping(f, a[i]);
        updates += l < r;
        break;
      case 1:
        sparse.apply(p, f);
        dense.apply(p, f);
        a[p] = A::mapping(f, a[p]);
        ++updates;
        break;
      case 2:
        sparse.set(p, A::S(f.c));
        dense.set(p, A::S(f.c));
        a[p] = A::S(f.c);
        ++updates;
        break;
      }
      assert(as_const(sparse).get(p).value == a[p].value);
      assert(dense.get(p).value == a[p].value);
      if (iter % 29 != 0) continue;
      auto intervals = as_const(sparse).content();
      assert(intervals.size() <= 2 * updates + 1);
      ll end = 0;
      for (const auto &[l, r, x] : intervals)
      {
        assert(l == end && l < r && r <= n);
        repi(i, l, r) assert(x.value == a[i].value);
        end = r;
      }
      assert(end == n);
    }
  }
}

void test_large()
{
  using A = AffineAction;
  using S = A::S;
  constexpr ll n = (1LL << 60) + 17;
  SparseDualSegmentTree<A> a(n, S(3));
  a.apply(10, n, {1, 4});
  a.apply(0, n, {2, 1});
  a.apply(n - 1, {3, 0});
  a.set(12, S(8));
  assert(as_const(a).get(0).value == 7);
  assert(as_const(a).get(11).value == 15);
  assert(as_const(a).get(12).value == 8);
  assert(as_const(a).get(13).value == 15);
  assert(as_const(a).get(n - 1).value == 45);

  auto copy = a;
  auto moved = std::move(copy);
  moved.reserve(10000);
  moved.apply(12, n - 1, {2, 4});
  assert(moved.get(12).value == 20);
  assert(moved.get(13).value == 34);
  assert(moved.get(n - 1).value == 45);
  assert(a.get(12).value == 8);
  assert(a.get(13).value == 15);

  mt19937_64 rng(80295);
  SparseDualSegmentTree<A> broad(n, S(1));
  vc<tuple<ll, ll, A::F>> updates;
  repi(i, 1700)
  {
    ll l = rng() % (n + 1), r = rng() % (n + 1);
    if (r < l) swap(l, r);
    if (i % 7 == 0) l = 0, r = n;
    A::F f(rng() % A::mod, rng() % A::mod);
    broad.apply(l, r, f);
    updates.eb(l, r, f);
    ll p = rng() % n;
    S expected(1);
    for (const auto &[from, to, action] : updates)
      if (from <= p && p < to) expected = A::mapping(action, expected);
    assert(as_const(broad).get(p).value == expected.value);
  }
}

int main()
{
  SparseDualSegmentTree<AffineAction> empty(0, AffineAction::S(3));
  empty.apply(0, 0, {2, 4});
  assert(as_const(empty).content().empty());
  test_against_dense();
  test_large();
  PRINT("Hello World");
}
