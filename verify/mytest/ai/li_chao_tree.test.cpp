#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "convex/li_chao_tree.hpp"
#include "math/rational.hpp"

template <class T>
struct Entry
{
  T l, r, a, b;
  int id;
  bool whole;
};

template <bool Min, class U, class T, class Result>
void check_result(const vc<Entry<T>> &entries, const T &x, const Result &result)
{
  optional<U> best;
  bool found = false;
  for (const auto &e : entries)
  {
    if (!e.whole && !(e.l <= x && x < e.r)) continue;
    U value = U(e.a) * U(x) + U(e.b);
    if (!best || (Min ? value < *best : *best < value)) best = value;
    found |= result.second.id == e.id && result.second.a == e.a && result.second.b == e.b;
  }
  if (!best)
  {
    assert(result.second.id == -1);
    U inf = resolved_infty<U>();
    assert(result.first == (Min ? inf : -inf));
  }
  else
  {
    assert(found && result.first == *best);
    assert(result.first == U(result.second.a) * U(x) + U(result.second.b));
  }
}

template <bool Min, class T, class U = larger_int_t<T>>
void random_checks()
{
  using Compare = conditional_t<Min, less<T>, greater<T>>;
  mt19937_64 rng(987654321);
  for (int trial = 0; trial < 60; ++trial)
  {
    vc<T> xs;
    for (int x = -32; x <= 32; ++x)
      if (trial % 2 == 0 || rng() % 3 == 0) xs.push_back(T(x));
    xs.push_back(T(0)), xs.push_back(T(0));
    shuffle(xs.begin(), xs.end(), rng);
    LiChaoTreeCompressed<T, U, Compare> compressed(xs);
    xs = compressed.coordinates();
    assert(is_sorted(xs.begin(), xs.end()));
    assert(adjacent_find(xs.begin(), xs.end()) == xs.end());
    LiChaoTree<T, U, Compare> dynamic(T(-32), T(33));
    if (trial % 2) dynamic.reserve(200);
    vc<Entry<T>> entries;
    for (int epoch = 0; epoch < 2; ++epoch)
    {
      auto check = [&]()
      {
        for (int x = -32; x <= 32; ++x)
          check_result<Min, U>(entries, T(x), dynamic.query(T(x)));
        for (int k = 0; k < int(xs.size()); ++k)
        {
          assert(compressed.lower_bound(xs[k]) == k);
          check_result<Min, U>(entries, xs[k], compressed.query(xs[k]));
          check_result<Min, U>(entries, xs[k], compressed.query_index(k));
        }
      };
      check();
      for (int i = 0; i < 45; ++i)
      {
        T a = T(int(rng() % 41) - 20), b = T(int(rng() % 101) - 50);
        T l = T(int(rng() % 101) - 50), r = T(int(rng() % 101) - 50);
        if (r < l) swap(l, r);
        int id = i % 3 == 0 ? i : 1000 + i;
        int type = rng() % 3;
        if (type == 0)
        {
          if (i % 3 == 0) dynamic.add_line(a, b), compressed.add_line(a, b);
          else dynamic.add_line(a, b, id), compressed.add_line(a, b, id);
        }
        else if (type == 1)
        {
          if (i % 3 == 0) dynamic.add_segment(l, r, a, b), compressed.add_segment(l, r, a, b);
          else dynamic.add_segment(l, r, a, b, id), compressed.add_segment(l, r, a, b, id);
        }
        else
        {
          int il = rng() % (xs.size() + 1), ir = rng() % (xs.size() + 1);
          if (ir < il) swap(il, ir);
          l = il == int(xs.size()) ? T(33) : xs[il];
          r = ir == int(xs.size()) ? T(33) : xs[ir];
          if (i % 3 == 0) dynamic.add_segment(l, r, a, b), compressed.add_segment_index(il, ir, a, b);
          else dynamic.add_segment(l, r, a, b, id), compressed.add_segment_index(il, ir, a, b, id);
        }
        entries.push_back({l, r, a, b, id, type == 0});
        check();
      }
      auto copied_dynamic = dynamic;
      auto copied_compressed = compressed;
      dynamic.clear(), compressed.clear();
      for (T x : xs)
      {
        check_result<Min, U>(entries, x, copied_dynamic.query(x));
        check_result<Min, U>(entries, x, copied_compressed.query(x));
      }
      assert(compressed.coordinates() == xs);
      entries.clear();
    }
  }
}

template <bool Min, class T, class U>
void boundaries()
{
  using Compare = conditional_t<Min, less<T>, greater<T>>;
  T lo = numeric_limits<T>::lowest(), hi = numeric_limits<T>::max();
  vc<T> xs{lo, T(lo + 1), T(-1), T(0), T(1), T(hi - 1)};
  LiChaoTree<T, U, Compare> dynamic(lo, hi);
  LiChaoTreeCompressed<T, U, Compare> compressed(xs);
  vc<Entry<T>> entries;
  auto add = [&](T l, T r, T a, T b, bool whole)
  {
    int id = entries.size();
    entries.push_back({l, r, a, b, id, whole});
    if (whole) dynamic.add_line(a, b), compressed.add_line(a, b);
    else dynamic.add_segment(l, r, a, b), compressed.add_segment(l, r, a, b);
    for (T x : xs)
    {
      check_result<Min, U>(entries, x, dynamic.query(x));
      check_result<Min, U>(entries, x, compressed.query(x));
    }
  };
  // Force traversal to both ends before adding any whole lines.
  add(lo, T(lo + 1), T(0), T(7), false);
  add(T(hi - 1), hi, T(0), T(-7), false);
  add(T(-1), T(0), T(0), T(3), false);
  add(T(0), T(1), T(0), T(-3), false);
  // Narrow coordinate types also exercise integer promotion in midpoint arithmetic.
  add(lo, hi, lo, hi, true);
  add(lo, hi, hi, lo, true);
  add(lo, hi, T(0), lo, true);
  add(lo, hi, T(0), hi, true);

  // Compressed coordinates can include the largest representable value.
  LiChaoTreeCompressed<T, U, Compare> endpoint({hi});
  endpoint.add_line(lo, hi);
  assert(endpoint.query(hi).first == U(lo) * U(hi) + U(hi));
}

template <bool Min>
void single_point_and_ids()
{
  using Compare = conditional_t<Min, less<>, greater<>>;
  LiChaoTree<int, ll, Compare> dynamic(5, 6);
  LiChaoTreeCompressed<int, ll, Compare> compressed({5, 5, 5});
  auto check = [&](int id, ll value)
  {
    for (auto result : {dynamic.query(5), compressed.query(5), compressed.query_index(0)})
      assert(result.second.id == id && result.first == value);
  };
  dynamic.add_segment(6, 8, 0, 100, 99);
  compressed.add_segment(6, 8, 0, 100, 99);
  dynamic.add_segment(5, 5, 0, 100);
  compressed.add_segment(5, 5, 0, 100);
  dynamic.add_line(2, 3), compressed.add_line(2, 3);
  check(2, 13);
  dynamic.add_line(2, 3, 800), compressed.add_line(2, 3, 800);
  dynamic.add_segment(4, 6, 0, Min ? -7 : 70), compressed.add_segment(4, 6, 0, Min ? -7 : 70);
  check(4, Min ? -7 : 70);
  dynamic.clear(), compressed.clear();
  dynamic.add_line(0, 9), compressed.add_line(0, 9);
  check(0, 9);

  LiChaoTreeCompressed<int> empty({});
  empty.add_line(1, 2);
  empty.add_segment(-1, 1, 1, 2);
  empty.add_segment_index(0, 0, 1, 2);
  assert(empty.coordinates().empty() && empty.lower_bound(0) == 0);
  empty.clear();
}

constexpr i128 large_empty_value() { return i128(1) << 100; }

template <bool Min>
void sentinels()
{
  using Compare = conditional_t<Min, less<int>, greater<int>>;
  LiChaoTree<int, i128, Compare, 123, large_empty_value> dynamic(-2, 3);
  LiChaoTreeCompressed<int, i128, Compare, 123, large_empty_value> compressed({-2, 0, 2});
  for (auto result : {dynamic.query(0), compressed.query(0)})
  {
    assert(result.first == (Min ? large_empty_value() : -large_empty_value()));
    assert(result.second.a == 0 && result.second.b == (Min ? 123 : -123) && result.second.id == -1);
  }
  // A valid value can lie beyond the chosen empty-set sentinel.
  using WideCompare = conditional_t<Min, less<ll>, greater<ll>>;
  LiChaoTree<ll, i128, WideCompare, 7, 11> wide(-3, 4);
  LiChaoTreeCompressed<ll, i128, WideCompare, 7, 11> wide_c({-3, 0, 3});
  ll b = Min ? numeric_limits<ll>::max() : numeric_limits<ll>::lowest();
  wide.add_line(0, b), wide_c.add_line(0, b);
  assert(wide.query(0).first == b && wide_c.query(0).first == b);
  // The value type may be smaller when all evaluations fit it.
  LiChaoTree<ll, int, conditional_t<Min, less<>, greater<>>, INF, 100> narrow(-2, 3);
  narrow.add_line(0, Min ? 200 : -200);
  assert(narrow.query(0).first == (Min ? 200 : -200));
}

template <bool Min, class T, class U = larger_int_t<T>>
void noninteger_coordinates()
{
  using Compare = conditional_t<Min, less<>, greater<>>;
  vc<T> xs{T(-3) / T(2), T(-1) / T(2), T(0), T(1) / T(2), T(5) / T(2)};
  LiChaoTreeCompressed<T, U, Compare> tree(xs);
  vc<Entry<T>> entries;
  for (int i = 0; i < 8; ++i)
  {
    T l = T(i - 4) / T(2), r = T(i - 1) / T(2);
    T a = T(i - 3) / T(2), b = T(i % 3 - 1) / T(2);
    entries.push_back({l, r, a, b, i, i % 3 == 0});
    if (entries.back().whole) tree.add_line(a, b);
    else tree.add_segment(l, r, a, b);
    for (int k = 0; k < int(xs.size()); ++k)
    {
      check_result<Min, U>(entries, xs[k], tree.query(xs[k]));
      check_result<Min, U>(entries, xs[k], tree.query_index(k));
    }
  }
}

void coordinate_i128()
{
  // Explicit U avoids requesting a nonexistent wider built-in integer.
  i128 lo = numeric_limits<i128>::lowest(), hi = numeric_limits<i128>::max();
  LiChaoTree<i128, i128> tree(lo, hi);
  tree.add_segment(lo, lo + 1, 0, -1);
  tree.add_segment(hi - 1, hi, 0, -2);
  tree.add_line(0, 5);
  assert(tree.query(lo).first == -1);
  assert(tree.query(lo + 1).first == 5);
  assert(tree.query(0).first == 5);
  assert(tree.query(hi - 1).first == -2);
}

int main()
{
  static_assert(is_same_v<decltype(declval<LiChaoTree<ll>>().query(0).first), i128>);
  static_assert(is_same_v<decltype(declval<LiChaoTreeCompressed<int>>().query(0).first), ll>);
  random_checks<true, int>();
  random_checks<false, int>();
  random_checks<true, ll>();
  random_checks<false, ll>();
  boundaries<true, signed char, int>();
  boundaries<false, signed char, int>();
  boundaries<true, int, ll>();
  boundaries<false, int, ll>();
  boundaries<true, ll, i128>();
  boundaries<false, ll, i128>();
  single_point_and_ids<true>();
  single_point_and_ids<false>();
  sentinels<true>();
  sentinels<false>();
  noninteger_coordinates<true, long double>();
  noninteger_coordinates<false, long double>();
  noninteger_coordinates<true, Rational<ll>>();
  noninteger_coordinates<false, Rational<ll>>();
  coordinate_i128();
  cout << "Hello World\n";
}
