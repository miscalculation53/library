#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/interval_map.hpp"

using Row = tuple<int, int, int>;

// 全区間を走査し、点と区間の位置関係から5種類の検索結果を照合する。
template <class T, class I>
void check_search(const IntervalMap<T, I> &mp, const I p)
{
  using Result = optional<typename IntervalMap<T, I>::iterator>;
  static_assert(is_same_v<decltype(mp.get_it(p)), Result>);
  static_assert(is_same_v<decltype(mp.leq_max_it(p)), Result>);
  static_assert(is_same_v<decltype(mp.geq_min_it(p)), Result>);
  static_assert(is_same_v<decltype(mp.lt_max_it(p)), Result>);
  static_assert(is_same_v<decltype(mp.gt_min_it(p)), Result>);
  const array<Result, 5> actual{mp.get_it(p), mp.leq_max_it(p), mp.geq_min_it(p),
                                mp.lt_max_it(p), mp.gt_min_it(p)};
  array<Result, 5> expected{};
  for (auto it = mp.begin(); it != mp.end(); ++it)
  {
    if (!(p < it->l) && p < it->r) expected[0] = it;
    if (!(p < it->l)) expected[1] = it;
    if (!expected[2] && p < it->r) expected[2] = it;
    if (!(p < it->r)) expected[3] = it;
    if (!expected[4] && p < it->l) expected[4] = it;
  }
  assert(actual == expected);
}

struct Mirror
{
  set<Row> rows;
  map<int, ll> lengths;
  ll squared_lengths = 0;

  void add(int l, int r, int x)
  {
    assert(l < r && rows.emplace(l, r, x).second);
    lengths[x] += r - l;
    squared_lengths += ll(r - l) * (r - l);
  }
  void del(int l, int r, int x)
  {
    assert(l < r && rows.erase(Row{l, r, x}) == 1);
    lengths[x] -= r - l;
    squared_lengths -= ll(r - l) * (r - l);
  }
};

// 未登録と「既定値を明示的に登録した区間」も区別して照合する。
void check(const IntervalMap<int, int> &mp, const vc<optional<int>> &a,
           const Mirror &mirror, int offset = 0, int unit = 0)
{
  vc<Row> want, actual;
  const int n = a.size();
  for (int l = 0, r; l < n; l = r)
  {
    for (r = l + 1; r < n && a[l] == a[r]; ++r) {}
    if (a[l]) want.emplace_back(l + offset, r + offset, *a[l]);
  }
  for (const auto &[l, r, x] : mp) actual.emplace_back(l, r, x);
  assert(actual == want);
  assert(mp.content() == want);
  assert(mp.empty() == want.empty());
  assert(mirror.rows == set<Row>(want.begin(), want.end()));
  map<int, ll> lengths;
  ll squared_lengths = 0;
  for (auto [l, r, x] : want)
  {
    lengths[x] += r - l;
    squared_lengths += ll(r - l) * (r - l);
  }
  for (auto [x, len] : mirror.lengths) assert(len == lengths[x]);
  assert(squared_lengths == mirror.squared_lengths);

  for (int p = offset - 1; p <= offset + n + 1; ++p)
  {
    const optional<int> value = offset <= p && p < offset + n ? a[p - offset] : nullopt;
    assert(mp.get(p) == value.value_or(unit));
    assert(mp[p] == value.value_or(unit));
    const auto it = mp.get_it(p);
    assert(it.has_value() == value.has_value());
    if (it) assert((*it)->l <= p && p < (*it)->r && (*it)->x == *value);
    check_search(mp, p);
  }
}

void test_search_boundaries()
{
  IntervalMap<int, int> mp;
  for (int p = -1; p <= 14; ++p) check_search(mp, p);
  mp.set(2, 8, 1);
  mp.set(10, 12, 2);
  mp.set(12, 14, 3);  // 異なる値の区間が端点で接する。
  for (int p = -1; p <= 15; ++p) check_search(mp, p);
  assert((*mp.get_it(5))->l == 2);
  assert((*mp.leq_max_it(5))->l == 2);
  assert((*mp.geq_min_it(5))->l == 2);
  assert(!mp.lt_max_it(5));
  assert((*mp.gt_min_it(5))->l == 10);
  assert(!mp.get_it(8) && (*mp.lt_max_it(8))->l == 2);
  assert((*mp.leq_max_it(9))->l == 2 && (*mp.geq_min_it(9))->l == 10);
  assert((*mp.lt_max_it(12))->l == 10 && (*mp.leq_max_it(12))->l == 12);
  assert((*mp.geq_min_it(12))->l == 12 && !mp.gt_min_it(12));
}

void test_updates()
{
  IntervalMap<int, int> mp;
  vc<optional<int>> a(12);
  Mirror mirror;
  auto add = [&](int l, int r, int x) { mirror.add(l, r, x); };
  auto del = [&](int l, int r, int x) { mirror.del(l, r, x); };
  auto update = [&](int l, int r, int x)
  {
    mp.set(l, r, x, add, del);
    for (int p = l; p < r; ++p) a[p] = x;
    check(mp, a, mirror);
  };
  auto erase = [&](int l, int r)
  {
    mp.erase(l, r, add, del);
    for (int p = l; p < r; ++p) a[p].reset();
    check(mp, a, mirror);
  };
  check(mp, a, mirror);
  update(2, 10, 1);
  update(4, 8, 2);  // 両端に古い値が残る。
  update(4, 8, 1);  // 左右を同時に併合。
  update(5, 6, 1);  // 既存区間の内部に同値を代入。
  erase(4, 8);
  update(4, 8, 1);  // 穴を埋めて併合。
  update(0, 2, 1);
  update(10, 12, 1);
  update(3, 9, 0);  // 既定値も登録区間になる。
  erase(0, 3);
  erase(9, 12);
  erase(5, 7);
  update(0, 12, 2);
  int events = 0;
  auto count = [&](int, int, int) { ++events; };
  mp.set(5, 5, 3, count, count);
  mp.erase(5, 5, count, count);
  assert(events == 0);
  check(mp, a, mirror);
  erase(0, 12);
  erase(2, 10);
}

void test_random()
{
  mt19937 rng(20261003);
  for (int trial = 0; trial < 100; ++trial)
  {
    IntervalMap<int, int> mp(-7);
    vc<optional<int>> a(40);
    Mirror mirror;
    auto add = [&](int l, int r, int x) { mirror.add(l, r, x); };
    auto del = [&](int l, int r, int x) { mirror.del(l, r, x); };
    for (int q = 0; q < 400; ++q)
    {
      int l = rng() % 41, r = rng() % 41;
      if (l > r) swap(l, r);
      if (rng() % 4 == 0)
      {
        mp.erase(l - 20, r - 20, add, del);
        for (int p = l; p < r; ++p) a[p].reset();
      }
      else
      {
        const int x = rng() % 4 == 0 ? -7 : int(rng() % 3);
        mp.set(l - 20, r - 20, x, add, del);
        for (int p = l; p < r; ++p) a[p] = x;
      }
      check(mp, a, mirror, -20, -7);
      if (q % 50 == 0)
      {
        auto copy = mp;
        mp = IntervalMap<int, int>();
        mp = move(copy);
        check(mp, a, mirror, -20, -7);
      }
    }
  }
}

void test_construction()
{
  const vc<int> values{0, 0, 2, 2, 2, 0, 1, 1};
  IntervalMap<int, int> mp(values, -1);
  Mirror mirror;
  for (auto [l, r, x] : mp) mirror.add(l, r, x);
  vc<optional<int>> a(values.begin(), values.end());
  check(mp, a, mirror, 0, -1);
  mp.set(2, 7, 0, [&](int l, int r, int x) { mirror.add(l, r, x); },
            [&](int l, int r, int x) { mirror.del(l, r, x); });
  fill(a.begin() + 2, a.begin() + 7, 0);
  check(mp, a, mirror, 0, -1);
  IntervalMap inferred(values);
  static_assert(is_same_v<decltype(inferred), IntervalMap<int>>);
  assert(inferred.content().size() == 4);
  assert(IntervalMap<int>(vc<int>{}).empty());
  IntervalMap<string> strings(vc<string>{"a", "a", "b"}, "empty");
  strings.set(-1, 2, (*strings.get_it(0))->x);
  assert(strings.content().size() == 2 && (*strings.get_it(0))->l == -1);
  strings.erase(0, 2);
  assert(strings.get(0) == "empty" && strings.get(-1) == "a" && strings.get(2) == "b");
}

struct Key
{
  int value;
  friend bool operator<(Key a, Key b) { return a.value < b.value; }
};
struct Value
{
  string value;
  Value() = delete;
  explicit Value(string value) : value(move(value)) {}
  bool operator==(const Value &other) const { return value == other.value; }
};

void test_types()
{
  const ll lo = numeric_limits<ll>::min(), hi = numeric_limits<ll>::max();
  IntervalMap<string> mp("empty");
  mp.set(lo, 0, "a");
  mp.set(0, hi, "a");
  assert(mp.content().size() == 1 && mp.begin()->l == lo && mp.begin()->r == hi);
  assert(mp[lo] == "a" && mp[hi - 1] == "a" && mp[hi] == "empty");
  assert(!mp.get_it(hi) && !mp.geq_min_it(hi));
  for (ll p : {lo, lo + 1, -1LL, 0LL, 1LL, hi - 1, hi}) check_search(mp, p);
  mp.erase(lo + 1, hi - 1);
  assert(mp.content().size() == 2 && mp.get(lo + 1) == "empty");
  mp.set(lo, hi, mp.begin()->x);
  assert(mp.content().size() == 1);
  mp.erase(lo, hi);
  assert(mp.empty());
  check_search(mp, lo);
  check_search(mp, hi);

  IntervalMap<int, ull> unsigned_mp;
  const ull umax = numeric_limits<ull>::max();
  unsigned_mp.set(0, umax, 3);
  unsigned_mp.erase(1, umax - 1);
  assert(unsigned_mp.content().size() == 2 && unsigned_mp[umax - 1] == 3 && unsigned_mp[umax] == 0);
  for (ull p : {0ULL, 1ULL, 2ULL, umax - 1, umax}) check_search(unsigned_mp, p);

  IntervalMap<int, i128> wide;
  const i128 big = i128(1) << 100;
  wide.set(-big, big, 4);
  wide.erase(-1, 1);
  assert(wide.content().size() == 2 && wide[-big] == 4 && wide[0] == 0);
  for (i128 p : {-big, -big + 1, i128(-1), i128(0), i128(1), big - 1, big}) check_search(wide, p);

  IntervalMap<int, double> real;
  real.set(-0.5, 1.25, 3);
  real.set(1.25, 2.5, 3);
  real.erase(0.25, 0.75);
  assert(real.content().size() == 2 && real[0.5] == 0 && real[1.25] == 3);
  for (double p : {-1.0, -0.5, 0.0, 0.25, 0.5, 0.75, 1.25, 2.5, 3.0}) check_search(real, p);
  assert(!real.lt_max_it(0.0) && (*real.gt_min_it(0.0))->l == 0.75);

  IntervalMap<Value, Key> generic(Value("empty"));
  generic.set(Key{1}, Key{4}, Value("a"));
  generic.set(Key{4}, Key{6}, generic.begin()->x);
  generic.erase(Key{2}, Key{3});
  assert(generic.content().size() == 2 && generic[Key{2}].value == "empty");
  assert((*generic.geq_min_it(Key{2}))->l.value == 3);
  for (int p = 0; p <= 7; ++p) check_search(generic, Key{p});
}

void test_many_segments()
{
  IntervalMap<int> mp;
  const int n = 20000;
  for (int i = 0; i < n; ++i) mp.set(2 * i, 2 * i + 1, i % 2);
  assert(mp.content().size() == n);
  mp.set(-1, 2 * n, 7);
  assert(mp.content().size() == 1 && mp.begin()->l == -1 && mp.begin()->r == 2 * n);
}

int main()
{
  test_search_boundaries();
  test_updates();
  test_random();
  test_construction();
  test_types();
  test_many_segments();
  cout << "Hello World\n";
}
