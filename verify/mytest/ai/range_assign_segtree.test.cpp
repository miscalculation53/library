#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algebra/affine_function.hpp"
#include "algebra/bit.hpp"
#include "ds/segtree/range_assign_segtree.hpp"
#include "math/modint/modint.hpp"

using M = MonoidAffineFunction<modint998244353>;
using S = M::S;
using Tree = RangeAssignSegmentTree<M>;

bool equal_functions(const S &a, const S &b) { return a.b == b.b && a.c == b.c; }

void check_product(const Tree &seg, const vc<S> &a, int l, int r)
{
  S res = M::e();
  for (int i = l; i < r; ++i) res = M::op(res, a[i]);
  assert(equal_functions(seg.prod(l, r), res));
  if (l == 0 && r == seg.size()) assert(equal_functions(seg.all_prod(), res));
  if (l + 1 == r) assert(equal_functions(seg.get(l), a[l]));

  using Row = array<modint998244353, 2>;
  auto apply = [](const Row &x, const S &f) -> Row { return {x[0] * f.b, x[0] * f.c + x[1]}; };
  const Row x{5, 8};
  assert(seg.prod_apply(l, r, x, apply) == apply(x, res));
}

void test_bounds_and_copy()
{
  Tree empty;
  empty.set(0, 0, {2, 3});
  assert(empty.size() == 0 && empty.content().empty());
  check_product(empty, {}, 0, 0);
  check_product(Tree(3), vc<S>(3, M::e()), 0, 3);

  Tree seg(64, {2, 3});
  seg.set(32, 64, {4, 5});
  auto copy = seg;
  seg.set(15, 49, {0, 7});
  vc<S> a(64, {4, 5});
  fill(a.begin(), a.begin() + 32, S{2, 3});
  fill(a.begin() + 15, a.begin() + 49, S{0, 7});
  for (int l = 0; l <= 64; ++l)
    for (int r = l; r <= 64; ++r) check_product(seg, a, l, r);
  const auto values = seg.content();
  assert(equal(values.begin(), values.end(), a.begin(), a.end(), equal_functions));
  assert(equal_functions(copy.get(15), {2, 3}));
  assert(equal_functions(copy.get(48), {4, 5}));
}

void test_random()
{
  mt19937 rng(5598201);
  auto gen = [&]() -> S { return {rng() % 9, rng() % 9}; };
  for (int n : {1, 2, 3, 31, 32, 33, 63, 64, 65, 127, 128, 129})
  {
    vc<S> a(n, M::e());
    for (auto &x : a) x = gen();
    Tree seg(a);
    check_product(seg, a, 0, n);

    for (int t = 0; t < 2000; ++t)
    {
      const S x = t % 5 == 0 ? M::e() : gen();
      int l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      if (t % 17 == 0) l = 0, r = n;
      if (t % 8 == 0)
      {
        l = rng() % n;
        r = l + 1;
        seg.set(l, x);
      }
      else seg.set(l, r, x);
      fill(a.begin() + l, a.begin() + r, x);

      l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      check_product(seg, a, l, r);
      check_product(seg, a, 0, n);
      l = rng() % n;
      check_product(seg, a, l, l + 1);
    }
  }
}

void test_bool()
{
  RangeAssignSegmentTree<MonoidOr<bool>> seg(vc<bool>{false, true, false, false, true});
  seg.set(0, 4, true);
  assert(seg.prod(1, 3));
  seg.set(2, 5, false);
  assert(seg.content() == (vc<bool>{true, true, false, false, false}));
  assert(!seg.prod(2, 5) && seg.all_prod());
  assert(!seg.prod_apply(2, 5, false, [](bool x, const bool &v) { return x || v; }));
  assert(seg.max_right_ok(2, [](bool v) { return !v; }) == 5);
  assert(seg.min_left_ok(5, [](bool v) { return !v; }) == 2);
  dump(as_const(seg));
}

struct StringMonoid
{
  using S = string;
  static S e() { return {}; }
  static S op(const S &a, const S &b) { return a + b; }
};

void test_search()
{
  const auto empty_ok = [](const string &s) { return s.empty(); };
  RangeAssignSegmentTree<StringMonoid> empty;
  assert(empty.max_right_ok(0, empty_ok) == 0 && empty.min_left_ok(0, empty_ok) == 0);

  constexpr int n = 65;
  RangeAssignSegmentTree<StringMonoid> seg(n, "ab");
  vc<string> a(n, "ab");
  mt19937 rng(159262);
  for (int t = 0; t < 300; ++t)
  {
    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r) swap(l, r);
    if (t % 8 == 0) l = 0, r = n;
    const string value(1 + rng() % 4, 'a' + rng() % 8);
    seg.set(l, r, value);
    fill(a.begin() + l, a.begin() + r, value);

    for (int trial = 0; trial < 10; ++trial)
    {
      l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      string target;
      for (int p = l; p < r; ++p) target += a[p];
      const auto prefix_ok = [&](const string &x)
      {
        return x.size() <= target.size() && equal(x.begin(), x.end(), target.begin());
      };
      const auto suffix_ok = [&](const string &x)
      {
        return x.size() <= target.size() && equal(x.rbegin(), x.rend(), target.rbegin());
      };
      const auto &read = seg;
      assert(read.max_right_ok(l, prefix_ok) == r);
      assert(read.min_left_ok(r, suffix_ok) == l);
    }
  }

  auto calls = 0;
  auto apply = [&](unique_ptr<string> &x, const string &v)
  {
    ++calls;
    *x += v;
    return std::move(x);
  };
  seg.set(0, n, "xy");
  auto value = seg.prod_apply(1, 16, make_unique<string>("start:"), apply);
  assert(*value == "start:" + seg.prod(1, 16) && calls > 0);
  calls = 0;
  value = seg.prod_apply(n, n, std::move(value), apply);
  assert(*value == "start:" + seg.prod(1, 16) && calls == 0);
}

struct CountedSum
{
  using S = ll;
  static inline int count = 0;
  static S e() { return 0; }
  static S op(S a, S b) { ++count; return a + b; }
};

void test_operation_counts()
{
  constexpr int height = 11, n = 1 << height;
  RangeAssignSegmentTree<CountedSum> seg(vc<ll>(n, 1));
  const auto add = [](ll x, const ll &v) { return x + v; };
  CountedSum::count = 0;
  assert(seg.prod_apply(1, n - 1, 0LL, add) == n - 2 && CountedSum::count == 0);

  seg.set(0, n / 2, 2);
  seg.set(n / 2, n, 3);
  CountedSum::count = 0;
  seg.set(1, n - 1, 4);
  assert(seg.all_prod() == 4LL * (n - 2) + 5 && CountedSum::count <= 3 * height - 3);
  assert(seg.get(0) == 2 && seg.get(n - 1) == 3);

  seg.set(0, n, 3);
  CountedSum::count = 0;
  assert(seg.prod_apply(2, 17, 0LL, add) == 45 && CountedSum::count <= 2);
}

int main()
{
  test_bounds_and_copy();
  test_random();
  test_bool();
  test_search();
  test_operation_counts();
  cout << "Hello World\n";
}
