#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algo/two_pointers.hpp"
#include "ds/interval_crossing.hpp"

template <class T>
bool crosses(const pair<T, T> &x, const pair<T, T> &y)
{
  const auto &[a, b] = x;
  const auto &[c, d] = y;
  return (a < c && c < b && b < d) || (c < a && a < d && d < b);
}

template <class T>
void check_query(const IntervalCrossing<T> &ds, const vc<pair<T, T>> &intervals,
                 const vc<bool> &active, const T &l, const T &r)
{
  bool exists = false;
  repi(i, intervals.size()) if (active[i] && crosses(pair<T, T>{l, r}, intervals[i])) exists = true;
  const auto result = ds.find_crossing(l, r);
  assert(result.has_value() == exists && ds.can_insert(l, r) == !exists);
  if (result)
  {
    const auto &[id, left, right] = *result;
    assert(0 <= id && id < int(intervals.size()) && active[id]);
    assert(!(left < intervals[id].first) && !(intervals[id].first < left));
    assert(!(right < intervals[id].second) && !(intervals[id].second < right));
    assert(crosses(pair<T, T>{l, r}, pair<T, T>{left, right}));
  }
}

template <class T>
void check_state(const IntervalCrossing<T> &ds, const vc<pair<T, T>> &intervals,
                 const vc<bool> &active)
{
  vc<int> ids;
  repi(i, intervals.size())
  {
    assert(ds.contains(i) == active[i]);
    if (active[i]) ids.eb(i);
    const auto &[l, r] = intervals[i];
    check_query(ds, intervals, active, l, r);
    const auto result = ds.find_crossing(i);
    assert(ds.can_insert(i) == ds.can_insert(l, r));
    assert(!result.has_value() == ds.can_insert(i));
    if (result)
    {
      const auto &[id, left, right] = *result;
      assert(0 <= id && id < int(intervals.size()) && active[id]);
      assert(!(left < intervals[id].first) && !(intervals[id].first < left));
      assert(!(right < intervals[id].second) && !(intervals[id].second < right));
      assert(crosses(intervals[i], pair<T, T>{left, right}));
    }
  }
  assert(ds.content() == ids && ds.size() == int(ids.size()) && ds.empty() == ids.empty());
}

// 全部分集合を通じ、端点の共有・包含・接触・空区間・重複を照合する。
void test_exhaustive()
{
  vc<pair<int, int>> intervals;
  repi(l, 4) repi(r, l, 4) intervals.eb(l, r);
  intervals.eb(0, 2);
  intervals.eb(1, 3);
  IntervalCrossing ds(intervals);
  vc<bool> active(intervals.size());
  repi(mask, 1 << intervals.size())
  {
    if (mask)
    {
      const int id = countr_zero(unsigned(mask));
      if (active[id]) ds.erase(id);
      else ds.insert(id);
      active[id] = !active[id];
    }
    check_state(ds, intervals, active);
    for (int l = -1; l <= 4; ++l) for (int r = l; r <= 4; ++r)
      check_query(ds, intervals, active, l, r);
  }
}

// 交差を含む状態への更新、任意の削除、コピー後の独立した更新を照合する。
void test_random()
{
  mt19937 rng(20261016);
  repi(trial, 80)
  {
    const int n = 1 + rng() % 50;
    vc<pair<int, int>> intervals(n);
    for (auto &[l, r] : intervals)
    {
      l = int(rng() % 41) - 20;
      r = int(rng() % 41) - 20;
      if (r < l) swap(l, r);
    }
    IntervalCrossing ds(intervals);
    vc<bool> active(n);
    repi(q, 160)
    {
      const int id = rng() % n;
      if (active[id]) ds.erase(id);
      else ds.insert(id);
      active[id] = !active[id];
      check_state(ds, intervals, active);
      int l = int(rng() % 61) - 30, r = int(rng() % 61) - 30;
      if (r < l) swap(l, r);
      check_query(ds, intervals, active, l, r);
      if (q % 40 == 0)
      {
        auto copy = ds;
        if (active[id]) copy.erase(id);
        else copy.insert(id);
        check_state(ds, intervals, active);
        ds = move(copy);
        active[id] = !active[id];
        check_state(ds, intervals, active);
      }
    }
  }
}

template <class T>
bool laminar(const vc<pair<T, T>> &intervals, int l, int r)
{
  repi(i, l, r) repi(j, l, i) if (crosses(intervals[i], intervals[j])) return false;
  return true;
}

// 既存の尺取り法で求める全境界を総当たりと照合する。
void test_two_pointers()
{
  mt19937 rng(12001016);
  repi(trial, 400)
  {
    const int n = rng() % 35;
    vc<pair<int, int>> intervals(n);
    for (auto &[l, r] : intervals)
    {
      l = rng() % 30;
      r = rng() % 30;
      if (r < l) swap(l, r);
    }
    IntervalCrossing ds(intervals);
    auto add_r = [&](int, int r) { ds.insert(r); };
    auto del_l = [&](int l, int) { ds.erase(l); };
    auto judge = [&](int l, int r)
    {
      assert(ds.size() == r - l);
      assert(laminar(intervals, l, max(l, r - 1)));
      return l == r || ds.can_insert(r - 1);
    };
    const auto rok = right_ok<int>(n, add_r, del_l, judge);
    repi(l, n + 1)
    {
      int r = l;
      while (r < n && laminar(intervals, l, r + 1)) ++r;
      assert(rok[l] == r);
    }
    ds = IntervalCrossing(intervals);
    const auto lok = left_ok<int>(n, add_r, del_l, judge);
    repi(r, n + 1)
    {
      int l = r;
      while (l && laminar(intervals, l - 1, r)) --l;
      assert(lok[r] == l);
    }
  }
}

struct Key
{
  int value;
  Key() = delete;
  explicit Key(int value) : value(value) {}
  friend bool operator<(const Key &x, const Key &y) { return x.value < y.value; }
};

// 取得結果は区間のコピーであり、削除や元オブジェクトの置き換え後も保持できる。
void test_result()
{
  IntervalCrossing ds(vc<pair<int, int>>{{0, 4}, {1, 6}});
  ds.insert(0);
  const auto result = ds.find_crossing(1);
  static_assert(is_same_v<remove_cv_t<decltype(result)>, optional<IntervalCrossing<int>::Interval>>);
  assert(result && result->id == 0 && result->l == 0 && result->r == 4);
  const auto [id, l, r] = *result;
  assert(id == 0 && l == 0 && r == 4);
  ds.erase(id);
  assert(!ds.find_crossing(1));
  ds = IntervalCrossing<int>();
  assert(result->id == 0 && result->l == 0 && result->r == 4);
}

void test_types()
{
  IntervalCrossing<> empty;
  assert(empty.empty() && empty.content().empty() && empty.can_insert(-10, 10));
  assert(!empty.find_crossing(0, 0));
  IntervalCrossing<int> no_candidates(vc<pair<int, int>>{});
  assert(no_candidates.empty() && no_candidates.can_insert(0, 1));

  const ll lo = numeric_limits<ll>::min(), hi = numeric_limits<ll>::max();
  const vc<pair<ll, ll>> intervals{{lo, hi}, {lo, 0}, {0, hi}, {-1, 1}, {lo, lo}, {hi, hi}};
  IntervalCrossing ds(intervals);
  vc<bool> active(intervals.size());
  repi(i, intervals.size())
  {
    ds.insert(i);
    active[i] = true;
    check_state(ds, intervals, active);
    check_query(ds, intervals, active, lo, hi);
  }
  const u128 big = u128(1) << 100;
  IntervalCrossing<u128> wide(vc<pair<u128, u128>>{{0, big}, {1, big + 1}});
  wide.insert(0);
  const auto wide_result = wide.find_crossing(1);
  assert(wide_result && wide_result->id == 0 && wide_result->l == 0 && wide_result->r == big);
  assert(!wide.find_crossing(big, big + 2));

  IntervalCrossing real(vc<pair<double, double>>{{-0.5, 1.25}, {0.25, 2.5}, {1.25, 2.5}});
  real.insert(0);
  const auto real_result = real.find_crossing(1);
  assert(real_result && real_result->id == 0 && real_result->l == -0.5 && real_result->r == 1.25);
  assert(real.can_insert(2));

  const vc<pair<Key, Key>> keys{{Key(0), Key(4)}, {Key(1), Key(6)}, {Key(4), Key(6)}};
  IntervalCrossing generic(keys);
  generic.insert(0);
  const auto generic_result = generic.find_crossing(1);
  assert(generic_result && generic_result->id == 0 && generic_result->l.value == 0 && generic_result->r.value == 4);
  assert(generic.can_insert(2));
  generic.insert(1);
  generic.erase(0);
  assert(generic.can_insert(2) && generic.can_insert(Key(2), Key(3)));
  generic.erase(1);
  assert(generic.empty());
}

int main()
{
  test_exhaustive();
  test_random();
  test_two_pointers();
  test_result();
  test_types();
  cout << "Hello World\n";
}
