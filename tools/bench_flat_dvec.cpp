// g++-15 -std=c++17 -O2 -DNDEBUG -I . tools/bench_flat_dvec.cpp -o /tmp/bench_flat_dvec
// /tmp/bench_flat_dvec 9
#include "ds/flat_dvec.hpp"
#include "template/template_vector.hpp"

using Clock = chrono::steady_clock;
volatile ll answer_sink = 0;

// 比較用に、以前の長さ基準の配置を残す。
template <class T, size_t D>
struct SmallFirstDvec
{
  array<size_t, D> dims{}, strides{};
  vc<T> dat;
  SmallFirstDvec(const array<ll, D> &sz, const T &init)
  {
    array<size_t, D> order;
    for (size_t d = 0; d < D; d++) dims[d] = sz[d], order[d] = d;
    sort(order.begin(), order.end(), [&](size_t a, size_t b)
         { return dims[a] != dims[b] ? dims[a] < dims[b] : a > b; });
    size_t n = 1;
    for (size_t d : order) strides[d] = n, n *= dims[d];
    dat.assign(n, init);
  }
  template <class... I>
  T &operator()(I... is)
  {
    array<size_t, D> ids{size_t(is)...};
    size_t pos = 0;
    for (size_t d = 0; d < D; d++) pos += ids[d] * strides[d];
    return dat[pos];
  }
};

template <class T, size_t D>
struct NestedType { using type = vc<typename NestedType<T, D - 1>::type>; };
template <class T>
struct NestedType<T, 0> { using type = T; };

template <class T, size_t D>
struct NestedDvec
{
  typename NestedType<T, D>::type dat;
  NestedDvec(const array<ll, D> &sz, const T &init)
  {
    ll sizes[D];
    copy(sz.begin(), sz.end(), sizes);
    dat = dvec(sizes, init);
  }
  template <class V, class I, class... Rest>
  static decltype(auto) get(V &v, I i, Rest... rest)
  {
    if constexpr (sizeof...(Rest) == 0) return (v[i]);
    else return get(v[i], rest...);
  }
  template <class... I>
  T &operator()(I... is) { return get(dat, is...); }
};

enum class Mode { nested, row_major, small_first };
template <Mode mode, class T, size_t D>
using Table = conditional_t<mode == Mode::nested, NestedDvec<T, D>,
                           conditional_t<mode == Mode::row_major, FlatDvec<T, D>, SmallFirstDvec<T, D>>>;
enum class Kind { knapsack, lcs, slimes, capped_knapsack, digit_sum };
struct Case
{
  string name;
  Kind kind;
  int n = 0, m = 0, k = 0;
  vc<ll> a, b;
  string s, t;
  bool reverse_indices = false;
  Case(string name, Kind kind) : name(std::move(name)), kind(kind) {}
};
struct Measurement { double init_ms, dp_ms, total_ms; ll answer; };
double milliseconds(Clock::time_point a, Clock::time_point b)
{
  return chrono::duration<double, milli>(b - a).count();
}

template <class F>
int solve_lcs(const Case &c, const F &dp)
{
  for (int i = 0; i < c.n; i++)
    for (int j = 0; j < c.m; j++)
      dp(i + 1, j + 1) = c.s[i] == c.t[j] ? dp(i, j) + 1 : max(dp(i, j + 1), dp(i + 1, j));
  return dp(c.n, c.m);
}

template <class DP, size_t D, class T, class F>
Measurement measure(const array<ll, D> &shape, const T &init, const F &solve)
{
  auto start = Clock::now();
  DP dp(shape, init);
  auto ready = Clock::now();
  ll answer = solve(dp);
  auto end = Clock::now();
  answer_sink = answer;
  return {milliseconds(start, ready), milliseconds(ready, end), milliseconds(start, end), answer};
}

template <Mode mode>
[[gnu::noinline]] Measurement run_case(const Case &c)
{
  if (c.kind == Kind::knapsack)
    return measure<Table<mode, ll, 2>>(array<ll, 2>{c.n + 1, c.m + 1}, 0LL, [&](auto &dp)
    {
      for (int i = 0; i < c.n; i++)
        for (int w = 0; w <= c.m; w++)
        {
          ll best = dp(i, w);
          if (w >= c.a[i]) best = max(best, dp(i, w - c.a[i]) + c.b[i]);
          dp(i + 1, w) = best;
        }
      return dp(c.n, c.m);
    });
  if (c.kind == Kind::lcs)
    return measure<Table<mode, int, 2>>(c.reverse_indices ? array<ll, 2>{c.m + 1, c.n + 1} : array<ll, 2>{c.n + 1, c.m + 1}, 0, [&](auto &dp)
    {
      if (c.reverse_indices) return solve_lcs(c, [&](int i, int j) -> int & { return dp(j, i); });
      return solve_lcs(c, [&](int i, int j) -> int & { return dp(i, j); });
    });
  if (c.kind == Kind::slimes)
    return measure<Table<mode, ll, 2>>(array<ll, 2>{c.n, c.n}, 0LL, [&](auto &dp)
    {
      vc<ll> prefix(c.n + 1, 0);
      for (int i = 0; i < c.n; i++) prefix[i + 1] = prefix[i] + c.a[i];
      for (int len = 2; len <= c.n; len++)
        for (int l = 0; l + len <= c.n; l++)
        {
          int r = l + len - 1;
          ll best = LLONG_MAX / 4;
          for (int mid = l; mid < r; mid++) best = min(best, dp(l, mid) + dp(mid + 1, r));
          dp(l, r) = best + prefix[r + 1] - prefix[l];
        }
      return dp(0, c.n - 1);
    });
  if (c.kind == Kind::capped_knapsack)
    return measure<Table<mode, int, 3>>(array<ll, 3>{c.n + 1, c.m + 1, c.k + 1}, 0, [&](auto &dp)
    {
      for (int i = 0; i < c.n; i++)
        for (int w = 0; w <= c.m; w++)
          for (int k = 0; k <= c.k; k++)
          {
            int best = dp(i, w, k);
            if (k && w >= c.a[i]) best = max(best, dp(i, w - c.a[i], k - 1) + int(c.b[i]));
            dp(i + 1, w, k) = best;
          }
      return dp(c.n, c.m, c.k);
    });
  return measure<Table<mode, int, 3>>(array<ll, 3>{c.n + 1, c.m, 2}, 0, [&](auto &dp)
  {
    constexpr int mod = 1000000007;
    dp(0, 0, 0) = 1;
    for (int i = 0; i < c.n; i++)
      for (int r = 0; r < c.m; r++)
        for (int less = 0; less < 2; less++)
        {
          int value = dp(i, r, less);
          int bound = c.s[i] - '0', limit = less ? 9 : bound;
          for (int digit = 0; digit <= limit; digit++)
          {
            int nr = (r + digit) % c.m, nl = less || digit < bound;
            int &target = dp(i + 1, nr, nl);
            target += value;
            if (target >= mod) target -= mod;
          }
        }
    return (dp(c.n, 0, 0) + dp(c.n, 0, 1) - 1LL + mod) % mod;
  });
}

Measurement dispatch(Mode mode, const Case &c)
{
  if (mode == Mode::nested) return run_case<Mode::nested>(c);
  if (mode == Mode::row_major) return run_case<Mode::row_major>(c);
  return run_case<Mode::small_first>(c);
}

void check_samples()
{
  vc<pair<Case, ll>> samples;
  Case knapsack{"sample_knapsack", Kind::knapsack};
  knapsack.n = 3; knapsack.m = 8; knapsack.a = {3, 4, 5}; knapsack.b = {30, 50, 60};
  samples.push_back({knapsack, 90});
  Case lcs{"sample_lcs", Kind::lcs};
  lcs.s = "axyb"; lcs.t = "abyxb"; lcs.n = lcs.s.size(); lcs.m = lcs.t.size();
  samples.push_back({lcs, 3});
  lcs.reverse_indices = true;
  samples.push_back({lcs, 3});
  Case slimes{"sample_slimes", Kind::slimes};
  slimes.n = 4; slimes.a = {10, 20, 30, 40};
  samples.push_back({slimes, 190});
  Case capped{"sample_capped", Kind::capped_knapsack};
  capped.n = 3; capped.m = 10; capped.k = 2; capped.a = {4, 3, 6}; capped.b = {20, 40, 100};
  samples.push_back({capped, 140});
  for (auto [s, d, expected] : vc<tuple<string, int, ll>>{{"30", 4, 6}, {"1000000009", 1, 2}, {"98765432109876543210", 58, 635270834}})
  {
    Case digit{"sample_digit", Kind::digit_sum};
    digit.s = s; digit.n = s.size(); digit.m = d;
    samples.push_back({digit, expected});
  }
  for (const auto &[c, expected] : samples)
    for (Mode mode : {Mode::nested, Mode::row_major, Mode::small_first})
      if (dispatch(mode, c).answer != expected) throw runtime_error("sample mismatch: " + c.name);
}

vc<Case> make_cases(unsigned seed)
{
  mt19937 rng(seed);
  vc<Case> cases;
  Case knapsack{"knapsack_100_100000", Kind::knapsack};
  knapsack.n = 100; knapsack.m = 100000;
  for (int i = 0; i < knapsack.n; i++)
    knapsack.a.push_back(1 + rng() % 2000), knapsack.b.push_back(1 + rng() % 1000000000);
  cases.push_back(knapsack);
  Case lcs{"lcs_1000_3000", Kind::lcs};
  lcs.n = 1000; lcs.m = 3000;
  for (int i = 0; i < lcs.n; i++) lcs.s += char('a' + rng() % 4);
  for (int j = 0; j < lcs.m; j++) lcs.t += char('a' + rng() % 4);
  cases.push_back(lcs);
  lcs.name = "lcs_1000_3000_reversed_indices"; lcs.reverse_indices = true;
  cases.push_back(lcs);
  lcs.reverse_indices = false;
  swap(lcs.n, lcs.m); swap(lcs.s, lcs.t); lcs.name = "lcs_3000_1000";
  cases.push_back(lcs);
  lcs.name = "lcs_3000_1000_reversed_indices"; lcs.reverse_indices = true;
  cases.push_back(lcs);
  Case slimes{"slimes_400", Kind::slimes};
  slimes.n = 400;
  for (int i = 0; i < slimes.n; i++) slimes.a.push_back(1 + rng() % 1000000000);
  cases.push_back(slimes);
  Case capped{"capped_knapsack_50_10000_10", Kind::capped_knapsack};
  capped.n = 50; capped.m = 10000; capped.k = 10;
  for (int i = 0; i < capped.n; i++)
    capped.a.push_back(1 + rng() % 1000), capped.b.push_back(1 + rng() % 100);
  cases.push_back(capped);
  Case digit{"digit_sum_10000_100", Kind::digit_sum};
  digit.n = 10000; digit.m = 100; digit.s += char('1' + rng() % 9);
  for (int i = 1; i < digit.n; i++) digit.s += char('0' + rng() % 10);
  cases.push_back(digit);
  // 桁数が余りの状態数より小さい場合も調べる。
  digit.name = "digit_sum_40_100"; digit.n = 40; digit.s.resize(digit.n);
  cases.push_back(digit);
  return cases;
}

double median(vc<double> values)
{
  sort(values.begin(), values.end());
  return values[values.size() / 2];
}

int main(int argc, char **argv)
{
  int repeats = argc > 1 ? stoi(argv[1]) : 9;
  unsigned seed = argc > 2 ? stoul(argv[2]) : 20261005;
  if (repeats < 1) throw runtime_error("repeats must be positive");
  check_samples();
  cout << "case,mode,init_ms,dp_ms,total_ms,answer,repeats,seed\n" << fixed << setprecision(6);
  for (const Case &c : make_cases(seed))
  {
    array<Mode, 3> modes{Mode::nested, Mode::row_major, Mode::small_first};
    array<vc<Measurement>, 3> timings;
    ll expected = dispatch(Mode::row_major, c).answer;
    for (Mode mode : modes)
      if (dispatch(mode, c).answer != expected) throw runtime_error("warmup mismatch: " + c.name);
    for (int repeat = 0; repeat < repeats; repeat++)
    {
      // 実行順を交代し、後に測る配置だけが有利になる影響を減らす。
      rotate(modes.begin(), modes.begin() + 1, modes.end());
      for (Mode mode : modes)
      {
        auto measured = dispatch(mode, c);
        if (measured.answer != expected) throw runtime_error("answer mismatch: " + c.name);
        timings[size_t(mode)].push_back(measured);
      }
    }
    for (size_t mode = 0; mode < 3; mode++)
    {
      vc<double> init, dp, total;
      for (const auto &m : timings[mode]) init.push_back(m.init_ms), dp.push_back(m.dp_ms), total.push_back(m.total_ms);
      const char *name = mode == 0 ? "nested" : mode == 1 ? "row_major" : "small_first";
      cout << c.name << ',' << name << ',' << median(init) << ',' << median(dp) << ',' << median(total)
           << ',' << expected << ',' << repeats << ',' << seed << '\n';
    }
  }
}
