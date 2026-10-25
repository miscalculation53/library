#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/bbst/lazy_sequence.hpp"
#include "ds/bbst/lazy_ordered_map.hpp"
#include "algebra/acted_monoid/affine_sum.hpp"
#include "math/modint/modint.hpp"

struct Concat
{
  using S = string;
  static S op(const S &a, const S &b) { return a + b; }
  static S e() { return {}; }
};
struct AssignConcat : Concat
{
  struct F { bool assign; char c; }; // equality 演算を持たない作用。
  static S mapping(F f, const S &s) { return f.assign ? S(s.size(), f.c) : s; }
  static F composition(F f, F g) { return f.assign ? f : g; }
  static F id() { return {false, 0}; }
};

mt19937 test_rng(73419);
int pick(int n) { assert(n > 0); return test_rng() % n; }
pair<int, int> interval(int n)
{
  int l = pick(n + 1), r = pick(n + 1);
  if (l > r) swap(l, r);
  return {l, r};
}
string fold(const vector<string> &a, int l, int r)
{
  string s;
  for (int i = l; i < r; ++i) s += a[i];
  return s;
}

template <class Tree>
void check_boundaries(const Tree &t, const vector<string> &a)
{
  int n = a.size();
  for (int l = 0; l <= n; ++l)
    for (int r = l; r <= n; ++r)
    {
      string target = fold(a, l, r);
      auto prefix = [&](const string &s)
      {
        return s.size() <= target.size() && equal(s.begin(), s.end(), target.begin());
      };
      auto suffix = [&](const string &s)
      {
        return s.size() <= target.size() && equal(s.begin(), s.end(), target.end() - s.size());
      };
      assert(t.max_right_ok(l, prefix) == r);
      assert(t.min_left_ok(r, suffix) == l);
    }
}

template <bool Lazy>
void test_sequence()
{
  using Tree = conditional_t<Lazy, LazySequenceTree<AssignConcat>, SequenceTree<Concat>>;
  static_assert(!is_copy_constructible_v<Tree> && is_nothrow_move_constructible_v<Tree>);
  Tree t;
  vector<string> a;
  for (int step = 0; step < 6000; ++step)
  {
    int n = a.size(), type = pick(12);
    auto [l, r] = interval(n);
    if (type == 0 && n < 45)
    {
      string x(pick(3) + 1, char('a' + pick(4)));
      t.insert(l, x);
      a.insert(a.begin() + l, x);
    }
    else if (type == 1 && n)
    {
      int p = pick(n);
      t.erase(p);
      a.erase(a.begin() + p);
    }
    else if (type == 2 && n)
    {
      int p = pick(n);
      string x(pick(3) + 1, char('a' + pick(4)));
      t.set(p, x);
      a[p] = x;
    }
    else if (type == 3)
    {
      t.reverse(l, r);
      reverse(a.begin() + l, a.begin() + r);
    }
    else if (type == 4)
    {
      int k = pick(n - (r - l) + 1);
      t.move(l, r, k);
      vector<string> b(a.begin() + l, a.begin() + r);
      a.erase(a.begin() + l, a.begin() + r);
      a.insert(a.begin() + k, b.begin(), b.end());
    }
    else if (type == 5)
    {
      int m = l + pick(r - l + 1);
      t.rotate(l, m, r);
      rotate(a.begin() + l, a.begin() + m, a.begin() + r);
    }
    else if (type == 6)
    {
      auto right = t.split(l);
      assert(t.all_prod() == fold(a, 0, l));
      assert(right.all_prod() == fold(a, l, n));
      t.concat(std::move(right));
      assert(right.empty());
    }
    else if (type == 7)
    {
      auto middle = t.extract(l, r);
      assert(middle.all_prod() == fold(a, l, r));
      auto right = t.split(l);
      t.concat(std::move(middle));
      t.concat(std::move(right));
    }
    else if (type == 8)
    {
      if constexpr (Lazy)
      {
        char c = 'a' + pick(4);
        t.apply(l, r, {true, c});
        for (int i = l; i < r; ++i) a[i].assign(a[i].size(), c);
      }
    }
    else if (type == 9 && n && step % 17 == 0)
    {
      t.erase(l, r);
      a.erase(a.begin() + l, a.begin() + r);
    }
    else if (type == 10)
    {
      Tree other(vector<string>{"uv", "w"});
      if constexpr (Lazy) other.apply(0, 2, {true, 'x'});
      vector<string> b = Lazy ? vector<string>{"xx", "x"} : vector<string>{"uv", "w"};
      if (n < 45)
      {
        t.concat(std::move(other));
        a.insert(a.end(), b.begin(), b.end());
        assert(other.empty());
        other.insert(0, "q");
        assert(other.get(0) == "q");
      }
    }
    assert(t.size() == (int)a.size());
    assert(t.all_prod() == fold(a, 0, a.size()));
    auto [ql, qr] = interval(a.size());
    assert(t.prod(ql, qr) == fold(a, ql, qr));
    if (!a.empty()) { int p = pick(a.size()); assert(t.get(p) == a[p]); }
    if (step % 97 == 0)
    {
      assert(t.content() == a);
      check_boundaries(t, a);
      Tree moved = std::move(t);
      assert(t.empty());
      t = std::move(moved);
      assert(moved.empty());
    }
  }
  Tree replacement(vector<string>{"old"});
  replacement = std::move(t);
  assert(t.empty() && replacement.content() == a);
  replacement.clear();
  assert(replacement.empty() && replacement.prod(0, 0).empty());
  check_boundaries(replacement, {});
  for (int n : {0, 1, 2, 3, 64, 1000})
  {
    vector<string> b(n, "a");
    Tree built(b.begin(), b.end());
    assert(built.content() == b && built.all_prod() == string(n, 'a'));
  }
  if constexpr (Lazy)
  {
    Tree point(vector<string>{"abc", "de"});
    point.apply(0, 2, {true, 'x'});
    point.apply(0, {true, 'y'});
    assert(point.all_prod() == "yyyxx");
    point.apply(0, 2, AssignConcat::id());
    assert(point.all_prod() == "yyyxx");
  }
}

template <bool Lazy, class Compare>
void test_map(Compare cmp)
{
  using Tree = conditional_t<Lazy, LazyOrderedMapTree<int, AssignConcat, Compare>, OrderedMapTree<int, Concat, Compare>>;
  Tree t(cmp);
  map<int, string, Compare> ref(cmp);
  for (int step = 0; step < 5000; ++step)
  {
    int key = pick(45) - 22, type = pick(10);
    string x(pick(3) + 1, char('a' + pick(4)));
    vector<pair<int, string>> entries(ref.begin(), ref.end());
    int n = entries.size();
    auto [l, r] = interval(n);
    if (type == 0) assert(t.insert(key, x) == ref.emplace(key, x).second);
    else if (type == 1) assert(t.erase(key) == (ref.erase(key) != 0));
    else if (type == 2 && n)
    {
      int p = pick(n);
      t.set(entries[p].first, x);
      ref[entries[p].first] = x;
    }
    else if (type == 3 && n)
    {
      int p = pick(n);
      t.set_by_order(p, x);
      ref[entries[p].first] = x;
    }
    else if (type == 4 && n)
    {
      int p = pick(n);
      t.erase_by_order(p);
      ref.erase(entries[p].first);
    }
    else if (type == 5)
    {
      auto right = t.split_by_order(l);
      assert(t.size() == l && right.size() == n - l);
      t.join(std::move(right));
      assert(right.empty());
    }
    else if (type == 6)
    {
      int p = distance(ref.begin(), ref.lower_bound(key));
      auto right = t.split_by_key(key);
      assert(t.size() == p && right.size() == n - p);
      t.join(std::move(right));
    }
    else if constexpr (Lazy)
    {
      char c = 'a' + pick(4);
      if (type == 7)
      {
        t.apply_by_order(l, r, {true, c});
        for (int i = l; i < r; ++i) ref[entries[i].first].assign(entries[i].second.size(), c);
      }
      else if (type == 8)
      {
        int hi = pick(60) - 30;
        if (cmp(hi, key)) swap(hi, key);
        t.apply_by_key(key, hi, {true, c});
        for (auto it = ref.lower_bound(key); it != ref.lower_bound(hi); ++it)
          it->second.assign(it->second.size(), c);
      }
    }
    vector<string> values;
    for (const auto &e : ref) values.push_back(e.second);
    assert(t.size() == (int)ref.size());
    assert(t.all_prod() == fold(values, 0, values.size()));
    auto [ql, qr] = interval(values.size());
    assert(t.prod_by_order(ql, qr) == fold(values, ql, qr));
    int lo = pick(60) - 30, hi = pick(60) - 30;
    if (cmp(hi, lo)) swap(lo, hi);
    int bl = distance(ref.begin(), ref.lower_bound(lo)), br = distance(ref.begin(), ref.lower_bound(hi));
    assert(t.prod_by_key(lo, hi) == fold(values, bl, br));
    assert(t.order_of_key(key) == distance(ref.begin(), ref.lower_bound(key)));
    assert(t.upper_order_of_key(key) == distance(ref.begin(), ref.upper_bound(key)));
    assert(t.contains(key) == (ref.count(key) != 0));
    if (ref.count(key)) assert(t.get(key) == ref.at(key));
    if (!ref.empty())
    {
      int p = pick(ref.size());
      auto e = *next(ref.begin(), p);
      assert(t.get_by_order(p) == make_pair(e.first, e.second));
    }
    if (step % 151 == 0)
    {
      assert((t.content() == vector<pair<int, string>>(ref.begin(), ref.end())));
      check_boundaries(t, values);
      Tree moved = std::move(t);
      assert(t.empty());
      t = std::move(moved);
      assert(moved.empty());
    }
  }
  t.clear();
  check_boundaries(t, {});
  vector<pair<int, string>> data{{3, "c"}, {1, "a"}, {2, "b"}, {1, "ignored"}};
  Tree built(data, cmp);
  assert(built.size() == 3 && built.get(1) == "a");
  Tree another(cmp);
  another.insert(cmp(4, 0) ? 0 : 4, "d");
  built.join(std::move(another));
  assert(built.size() == 4 && another.empty());
  t = std::move(built);
  assert(built.empty() && t.size() == 4);
}

using Mint = modint998244353;
using AffineSum = ActedMonoidAffineSum<Mint>;
void test_affine()
{
  // 非可換な作用の合成、反転後の伝搬、遅延作用がある木同士の連結。
  vector<Mint> a(80);
  for (auto &x : a) x = pick(100);
  LazySequenceTree<AffineSum> t(a);
  LazyOrderedMapTree<int, AffineSum> mp;
  for (int i = 0; i < (int)a.size(); ++i) mp.insert(10 * i, a[i]);
  vector<Mint> b = a;
  for (int step = 0; step < 4000; ++step)
  {
    auto [l, r] = interval(a.size());
    if (step % 3)
    {
      Mint mul = pick(4), add = pick(10);
      t.apply(l, r, {mul, add});
      if (step % 2) mp.apply_by_order(l, r, {mul, add});
      else mp.apply_by_key(10 * l, 10 * r, {mul, add});
      for (int i = l; i < r; ++i) a[i] = mul * a[i] + add, b[i] = mul * b[i] + add;
    }
    else
    {
      t.reverse(l, r);
      reverse(a.begin() + l, a.begin() + r);
    }
    if (step % 7 == 0)
    {
      auto s = t.split(l);
      t.concat(std::move(s));
      auto m = mp.split_by_order(l);
      mp.join(std::move(m));
    }
    auto [ql, qr] = interval(a.size());
    Mint sa = 0, sb = 0;
    for (int i = ql; i < qr; ++i) sa += a[i], sb += b[i];
    assert(t.prod(ql, qr).val == sa && t.prod(ql, qr).len == qr - ql);
    assert(mp.prod_by_order(ql, qr).val == sb && mp.prod_by_key(10 * ql, 10 * qr).val == sb);
  }
}

void test_pending_updates()
{
  vector<string> a{"ab", "c", "de", "fg", "h"};
  LazySequenceTree<AssignConcat> t(a);
  t.apply(0, t.size(), {true, 'x'});
  t.reverse(0, t.size());
  reverse(a.begin(), a.end());
  for (auto &s : a) s.assign(s.size(), 'x');
  check_boundaries(t, a);
  t.insert(2, "new");
  assert(t.get(2) == "new");
  t.erase(0, t.size());
  assert(t.empty());

  LazyOrderedMapTree<int, AssignConcat> left, right;
  left.insert(10, "a");
  left.insert(20, "bb");
  right.insert(30, "ccc");
  right.insert(40, "d");
  left.apply_by_order(0, 2, {true, 'x'});
  right.apply_by_key(30, 50, {true, 'y'});
  left.join(std::move(right));
  check_boundaries(left, {"x", "xx", "yyy", "y"});
  left.apply_by_order(0, 4, {true, 'z'});
  left.insert(25, "new");
  assert(left.get(25) == "new");
  assert(left.all_prod() == "zzznewzzzz");

  OrderedMapTree<pair<int, int>, Concat> duplicates;
  duplicates.insert({2, 0}, "c");
  duplicates.insert({1, 1}, "b");
  duplicates.insert({1, 0}, "a");
  assert(duplicates.prod_by_order(0, 2) == "ab");
  assert(duplicates.prod_by_key({1, 0}, {2, 0}) == "ab");
}

struct Key
{
  int x;
  explicit Key(int x) : x(x) {}
};
struct KeyCompare
{
  bool descending;
  bool operator()(const Key &a, const Key &b) const { return descending ? a.x > b.x : a.x < b.x; }
};
void test_types_and_large()
{
  OrderedMapTree<Key, Concat, KeyCompare> mp(KeyCompare{true});
  mp.insert(Key(1), "a");
  mp.insert(Key(2), "b");
  assert(mp.all_prod() == "ba");
  auto right = mp.split_by_key(Key(1));
  mp.join(std::move(right));
  assert(mp.get_by_order(0).first.x == 2);

  using Stateful = OrderedMapTree<int, Concat, function<bool(int, int)>>;
  Stateful original([](int a, int b) { return a > b; });
  original.insert(1, "a");
  Stateful moved(std::move(original));
  original.insert(2, "b");
  original.insert(3, "c");
  assert(original.all_prod() == "cb");
  moved = std::move(original);
  original.insert(4, "d");
  original.insert(5, "e");
  assert(original.all_prod() == "ed" && moved.all_prod() == "cb");

  using T = SequenceTree<MonoidAdd<ll>>;
  vector<ll> a(200000);
  iota(a.begin(), a.end(), 0);
  T t(a);
  ll expected = ll(a.size()) * (a.size() - 1) / 2;
  t.reverse(0, t.size());
  assert(t.all_prod() == expected && t.get(0) == (int)a.size() - 1);
  for (int i = 0; i < 5000; ++i)
  {
    t.insert(i % 2 ? 0 : t.size(), 1);
    ++expected;
  }
  auto part = t.extract(123, 9999);
  assert(t.all_prod() + part.all_prod() == expected);
  t.concat(std::move(part));
  t.clear();
  assert(t.empty());
}

int main()
{
  for (unsigned seed : {1, 291, 89324})
  {
    bbst_detail::treap_random().seed(seed);
    test_sequence<false>();
    test_sequence<true>();
    test_map<false>(less<int>{});
    test_map<true>(less<int>{});
    test_map<false>(greater<int>{});
    test_map<true>(greater<int>{});
    test_affine();
    test_pending_updates();
  }
  test_types_and_large();
  cout << "Hello World" << endl;
}
