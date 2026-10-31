#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/segtree/lazy_segtree.hpp"
#include "ds/segtree/segtree_beats.hpp"
#include "ds/segtree/sparse_segtree.hpp"
#include "ds/segtree/sparse_lazy_segtree.hpp"
#include "ds/segtree/meldable_sparse_segtree.hpp"
#include "algebra/acted_monoid/add_sum.hpp"
#include "algebra/acted_monoid/chmin_chmax_add_min_max_sum.hpp"

struct Words
{
  using S = string;
  static inline int operations = 0;
  static S e() { return {}; }
  static S op(const S &a, const S &b) { ++operations; return a + b; }
};
struct AssignWords : Words
{
  using F = char;
  static F id() { return 0; }
  static F composition(F f, F g) { return f == 0 ? g : f; }
  static S mapping(F f, const S &s) { return f == 0 ? s : string(s.size(), f); }
};
struct Append
{
  int calls = 0;
  Append() = default;
  Append(const Append &) = delete;
  string operator()(const string &x, const string &s) { ++calls; return x + s; }
  string operator()(const string &, string &) = delete;
};

template <class Tree>
void check(Tree &seg, const vc<string> &a, int l, int r, bool no_op)
{
  string expected = "prefix:";
  repi(i, l, r) expected += a[i];
  Append append;
  Words::operations = 0;
  assert(seg.prod_apply(l, r, string("prefix:"), append) == expected);
  if (no_op) assert(Words::operations == 0);
  if (l == r) assert(append.calls == 0);
  else assert(append.calls <= 6 * int(bit_width(ull(a.size()))) + 4);
}

template <class Tree>
void point_updates(Tree &seg, vc<string> a, bool no_op)
{
  const int n = a.size();
  mt19937 rng(20261026);
  repi(l, n + 1) repi(r, l, n + 1) check(seg, a, l, r, no_op);
  repi(iter, 200)
  {
    if (n > 0)
    {
      int p = rng() % n;
      a[p] = string(rng() % 4, 'a' + rng() % 26);
      seg.set(p, a[p]);
    }
    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r) swap(l, r);
    check(as_const(seg), a, l, r, no_op);
  }
  auto move_only = seg.prod_apply(0, n, make_unique<string>("owned:"),
      [](unique_ptr<string> &x, const string &s) { *x += s; return move(x); });
  assert(*move_only == accumulate(a.begin(), a.end(), string("owned:")));
}

void test_sparse_and_meldable()
{
  for (int n : {0, 1, 2, 7, 16, 31})
  {
    SparseSegmentTree<Words> fast(n, ""), general(n, "x");
    point_updates(fast, vc<string>(n), true);
    point_updates(general, vc<string>(n, "x"), false);
    MeldableSparseSegmentTreePool<Words> pool(n);
    auto tree = pool.make_tree();
    point_updates(tree, vc<string>(n), true);
    assert(pool.node_count() == 0 || n > 0);
    const auto &read = tree;
    string expected = tree.prod(0, n);
    assert(read.prod_apply(0, n, string{}, [](const string &x, const string &s) { return x + s; }) == expected);
  }
  MeldableSparseSegmentTreePool<Words> pool(1LL << 40);
  auto a = pool.make_tree(100, "a"), b = pool.make_tree(200, "b");
  b.set(100, "x");
  a.merge(b);
  auto copy = a.clone();
  copy.erase(100);
  const auto count = pool.node_count();
  Append append;
  assert(a.prod_apply(0, 300, string{}, append) == "axb");
  assert(copy.prod_apply(0, 300, string{}, append) == "b");
  assert(b.prod_apply(0, 300, string("empty"), append) == "empty");
  assert(pool.node_count() == count);
}

template <class Tree>
void range_updates(Tree &seg, int n)
{
  vc<string> a(n, "a");
  mt19937 rng(998244353);
  repi(iter, 500)
  {
    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r) swap(l, r);
    char c = 'a' + rng() % 10;
    seg.apply(l, r, c);
    repi(i, l, r) a[i].assign(a[i].size(), c);
    if (n > 0 && iter % 3 == 0)
    {
      int p = rng() % n;
      seg.set(p, a[p] = string(1 + rng() % 3, c));
    }
    l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r) swap(l, r);
    check(seg, a, l, r, true);
  }
  repi(l, n + 1) repi(r, l, n + 1) check(seg, a, l, r, true);
  string expected = accumulate(a.begin(), a.end(), string{});
  auto nested = [&](const string &x, const string &s)
  {
    string before = s;
    assert(seg.prod(0, n) == expected);
    assert(s == before);
    return x + s;
  };
  assert(seg.prod_apply(0, n, string{}, nested) == expected);
  auto owned = seg.prod_apply(0, n, make_unique<string>(),
      [](unique_ptr<string> &x, const string &s) { *x += s; return move(x); });
  assert(*owned == expected);
}

void test_lazy()
{
  for (int n : {0, 1, 2, 7, 16, 31})
  {
    LazySegmentTree<AssignWords> lazy(vc<string>(n, "a"));
    range_updates(lazy, n);
    SparseLazySegmentTree<AssignWords> sparse(n, "a");
    // Partial uniform intervals use monoid powers; check values and order here.
    vc<string> a(n, "a");
    mt19937 rng(123456789);
    repi(iter, 500)
    {
      int l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      char c = 'a' + rng() % 10;
      sparse.apply(l, r, c);
      repi(i, l, r) a[i] = string(1, c);
      l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      check(as_const(sparse), a, l, r, false);
    }
    repi(l, n + 1) repi(r, l, n + 1) check(as_const(sparse), a, l, r, false);
    auto runs = sparse.content();
    auto concatenate = [&](const string &x, const string &s)
    {
      assert(sparse.content() == runs);
      return x + s;
    };
    assert(sparse.prod_apply(0, n, string{}, concatenate) == accumulate(a.begin(), a.end(), string{}));
    assert(sparse.content() == runs);
  }
}

void test_large_and_beats()
{
  const ll n = 1LL << 40;
  SparseSegmentTree<MonoidAdd<i128>> point(n, i128(1) << 50);
  point.set(12345, 7);
  auto sum = [](i128 x, i128 s) { return x + s; };
  assert(point.prod_apply(100, n - 10, i128(3), sum) == i128(3) + point.prod(100, n - 10));
  using Add = ActedMonoidAddSum<i128>;
  SparseLazySegmentTree<Add> lazy(n, Add::S(0));
  lazy.apply(100, n - 100, 9);
  lazy.apply(200, 300, -3);
  auto add_sum = [](i128 x, const Add::S &s) { return x + s.val; };
  assert(lazy.prod_apply(150, n - 150, i128(7), add_sum) == i128(7) + lazy.prod(150, n - 150).val);

  using AM = ActedMonoidChminChmaxAddMinMaxSum<ll>;
  using F = AM::F;
  vc<ll> a(35);
  iota(a.begin(), a.end(), -17);
  SegmentTreeBeats<AM> beats(a);
  mt19937 rng(123456789);
  repi(iter, 1000)
  {
    int l = rng() % 36, r = rng() % 36;
    if (l > r) swap(l, r);
    ll value = ll(rng() % 41) - 20;
    int type = rng() % 3;
    auto f = type == 0 ? F::chmin(value) : type == 1 ? F::chmax(value) : F::add(value);
    beats.apply(l, r, f);
    repi(i, l, r) a[i] = f(a[i]);
    l = rng() % 36, r = rng() % 36;
    if (l > r) swap(l, r);
    auto result = beats.prod_apply(l, r, 7LL, [](ll x, const AM::S &s) { return x + s.sum; });
    assert(result == accumulate(a.begin() + l, a.begin() + r, 7LL));
  }
}

int main()
{
  test_sparse_and_meldable();
  test_lazy();
  test_large_and_beats();
  cout << "Hello World\n";
}
