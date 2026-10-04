#include "ds/segtree/segtree.hpp"
#include "ds/segtree/lazy_segtree.hpp"
#include "ds/segtree/dual_segtree.hpp"
#include "algebra/acted_monoid/add_sum.hpp"
#include "segtree_range_candidates.hpp"

// Run with: python3 benchmark/segtree_range.py
// RNG, construction, and validation are outside the timed region.
// Five trials, median time, alternating the two implementations' execution order.
// The variants replace enumeration loops; lazy boundary paths keep the existing code.
struct AffineMonoid
{
  struct S { ull a = 1, b = 0; };
  static S e() { return {}; }
  // Arithmetic modulo 2^64: associative, noncommutative, with defined overflow.
  static S op(S x, S y) { return {y.a * x.a, y.a * x.b + y.b}; }
};
struct AffineAction
{
  using S = ull;
  using F = AffineMonoid::S;
  static S e() { return 0; }
  static F id() { return {}; }
  static S mapping(F f, S x) { return f.a * x + f.b; }
  static F composition(F f, F g) { return AffineMonoid::op(g, f); }
};

ull digest(ull x) { return x; }
ull digest(AffineMonoid::S x) { return x.a ^ x.b; }
struct Query { int l, r, p; ull x; };
volatile ull sink = 0;

void require(bool condition)
{
  if (!condition) { cerr << "result mismatch\n"; abort(); }
}

// Check outputs after every operation before benchmarking, including noncommutative actions.
void validate()
{
  mt19937 rng(73109);
  for (int n : {1, 2, 3, 19, 64, 65})
  {
    vc<AffineMonoid::S> init(n);
    SegmentTree<AffineMonoid> a(init);
    RangeSegmentTree<AffineMonoid> b(init);
    DualSegmentTree<AffineAction> c(vc<ull>(n, 0));
    RangeDualSegmentTree<AffineAction> d(vc<ull>(n, 0));
    vl zero(n);
    LazySegmentTree<ActedMonoidAddSum<ll>> e(zero);
    RangeLazySegmentTree<ActedMonoidAddSum<ll>> f(zero);
    for (int step = 0; step < 1000; ++step)
    {
      int p = rng() % n, l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      AffineMonoid::S v{rng(), rng()};
      a.set(p, v); b.set(p, v);
      auto x = a.prod(l, r), y = b.prod(l, r);
      require(x.a == y.a && x.b == y.b);
      c.apply(l, r, v); d.apply(l, r, v);
      if (step % 3 == 0) { c.set(p, step); d.set(p, step); }
      if (step % 3 == 1) { c.apply(p, v); d.apply(p, v); }
      require(c.get(p) == d.get(p));
      e.apply(l, r, 2); f.apply(l, r, 2);
      if (step % 3 == 0) { e.set(p, step); f.set(p, step); }
      if (step % 3 == 1) { e.apply(p, 3); f.apply(p, 3); }
      require(e.get(p).val == f.get(p).val && e.prod(l, r).val == f.prod(l, r).val);
    }
  }
}

template <class MakeA, class MakeB, class Run>
void compare(string name, int n, int count, MakeA make_a, MakeB make_b, Run run)
{
  array<double, 5> times_a, times_b;
  auto timed = [&](auto &tree)
  {
    const auto start = chrono::steady_clock::now();
    ull sum = run(tree);
    const auto finish = chrono::steady_clock::now();
    sink = sum;
    return make_pair(chrono::duration<double, milli>(finish - start).count(), sum);
  };
  // Warm up each code path, then alternate execution order across trials.
  { auto a = make_a(); auto b = make_b(); require(run(a) == run(b)); }
  for (int round = 0; round < 5; ++round)
  {
    auto a = make_a(); auto b = make_b();
    pair<double, ull> x, y;
    if (round % 2) { y = timed(b); x = timed(a); }
    else { x = timed(a); y = timed(b); }
    require(x.second == y.second);
    times_a[round] = x.first; times_b[round] = y.first;
  }
  sort(times_a.begin(), times_a.end()); sort(times_b.begin(), times_b.end());
  cout << name << ',' << n << ',' << count << ',' << fixed << setprecision(3)
       << times_a[2] << ',' << times_b[2] << ',' << times_b[2] / times_a[2] << endl;
}

template <class M>
void standard_bench(string name, int n, const vc<Query> &queries, const vc<typename M::S> &init,
                    const vc<typename M::S> &values)
{
  auto a = [&] { return SegmentTree<M>(init); };
  auto b = [&] { return RangeSegmentTree<M>(init); };
  compare(name + "_prod", n, queries.size(), a, b, [&](auto &tree)
  {
    ull sum = 0;
    for (auto q : queries) sum += digest(tree.prod(q.l, q.r));
    return sum;
  });
  compare(name + "_set", n, queries.size(), a, b, [&](auto &tree)
  {
    for (int k = 0; k < int(queries.size()); ++k) tree.set(queries[k].p, values[k]);
    return digest(tree.all_prod());
  });
}

int main()
{
  validate();
  constexpr int count = 200000;
  cout << "workload,n,queries,current_ms,range_ms,ratio" << endl;
  mt19937_64 rng(20261003);
  for (int n : {1024, 200000, 1048576})
  {
    vc<Query> queries(count);
    for (auto &q : queries)
    {
      q.l = rng() % (n + 1); q.r = rng() % (n + 1);
      if (q.l > q.r) swap(q.l, q.r);
      q.p = rng() % n; q.x = rng() % 1000;
    }
    vc<ull> init(n), values(count);
    vc<AffineMonoid::S> affine(n), affine_values(count);
    for (auto &x : init) x = rng() % 1000;
    for (auto &x : values) x = rng() % 1000;
    for (auto &x : affine) x = {rng(), rng()};
    for (auto &x : affine_values) x = {rng(), rng()};
    standard_bench<MonoidAdd<ull>>("sum", n, queries, init, values);
    standard_bench<AffineMonoid>("affine", n, queries, affine, affine_values);

    compare("dual_apply_get", n, count,
            [&] { return DualSegmentTree<AffineAction>(init); },
            [&] { return RangeDualSegmentTree<AffineAction>(init); }, [&](auto &tree)
    {
      ull sum = 0;
      for (int k = 0; k < count; ++k)
      {
        auto q = queries[k];
        if (k % 2) sum += tree.get(q.p);
        else tree.apply(q.l, q.r, affine_values[k]);
      }
      return sum;
    });
    using AM = ActedMonoidAddSum<ull>;
    compare("lazy_apply_prod", n, count,
            [&] { return LazySegmentTree<AM>(init); },
            [&] { return RangeLazySegmentTree<AM>(init); }, [&](auto &tree)
    {
      ull sum = 0;
      for (int k = 0; k < count; ++k)
      {
        auto q = queries[k];
        if (k % 2) sum += tree.prod(q.l, q.r).val;
        else tree.apply(q.l, q.r, q.x);
      }
      return sum;
    });
    compare("lazy_point_set_get", n, count,
            [&] { return LazySegmentTree<AM>(init); },
            [&] { return RangeLazySegmentTree<AM>(init); }, [&](auto &tree)
    {
      ull sum = 0;
      for (int k = 0; k < count; ++k)
      {
        auto q = queries[k];
        if (k % 2) sum += tree.get(q.p).val;
        else tree.set(q.p, q.x);
      }
      return sum;
    });
  }
}
