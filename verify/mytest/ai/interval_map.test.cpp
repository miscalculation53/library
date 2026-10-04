#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/interval_map.hpp"

using Row = tuple<int, int, int>;

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
    assert((it != mp.end()) == value.has_value());
    if (it != mp.end()) assert(it->l <= p && p < it->r && it->x == *value);
    const auto jt = mp.next_it(p);
    auto expected = find_if(want.begin(), want.end(), [&](const Row &row) { return p < std::get<1>(row); });
    assert((jt == mp.end()) == (expected == want.end()));
    if (jt != mp.end()) assert((Row{jt->l, jt->r, jt->x}) == *expected);
  }
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
  strings.set(-1, 2, strings.get_it(0)->x);
  assert(strings.content().size() == 2 && strings.get_it(0)->l == -1);
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
  assert(mp.get_it(hi) == mp.end() && mp.next_it(hi) == mp.end());
  mp.erase(lo + 1, hi - 1);
  assert(mp.content().size() == 2 && mp.get(lo + 1) == "empty");
  mp.set(lo, hi, mp.begin()->x);
  assert(mp.content().size() == 1);
  mp.erase(lo, hi);
  assert(mp.empty());

  IntervalMap<int, ull> unsigned_mp;
  const ull umax = numeric_limits<ull>::max();
  unsigned_mp.set(0, umax, 3);
  unsigned_mp.erase(1, umax - 1);
  assert(unsigned_mp.content().size() == 2 && unsigned_mp[umax - 1] == 3 && unsigned_mp[umax] == 0);

  IntervalMap<int, i128> wide;
  const i128 big = i128(1) << 100;
  wide.set(-big, big, 4);
  wide.erase(-1, 1);
  assert(wide.content().size() == 2 && wide[-big] == 4 && wide[0] == 0);

  IntervalMap<int, double> real;
  real.set(-0.5, 1.25, 3);
  real.set(1.25, 2.5, 3);
  real.erase(0.25, 0.75);
  assert(real.content().size() == 2 && real[0.5] == 0 && real[1.25] == 3);

  IntervalMap<Value, Key> generic(Value("empty"));
  generic.set(Key{1}, Key{4}, Value("a"));
  generic.set(Key{4}, Key{6}, generic.begin()->x);
  generic.erase(Key{2}, Key{3});
  assert(generic.content().size() == 2 && generic[Key{2}].value == "empty");
  assert(generic.next_it(Key{2})->l.value == 3);
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
  test_updates();
  test_random();
  test_construction();
  test_types();
  test_many_segments();
  cout << "Hello World\n";
}
