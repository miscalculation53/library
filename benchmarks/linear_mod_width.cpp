// g++-15 -std=c++17 -O2 -DNDEBUG -I . benchmarks/linear_mod_width.cpp -o /tmp/linear_mod_width_bench
#include "math/mod_of_linear/min_of_mod_of_linear.hpp"
#include "utils/larger_int.hpp"

using Clock = chrono::steady_clock;
volatile ull checksum_sink = 0;
struct Query { ll n, m, a, b; };

template <class W>
ll signed_core(ll n, ll m, ll a, ll b)
{
  ll ans = b;
  for (;;)
  {
    if (a > m / 2) b = (W(a) * (n - 1) + b) % m, a = m - a;
    chmin(ans, b);
    const ll wraps = (W(a) * (n - 1) + b) / m;
    if (wraps == 0 || ans == 0) return ans;
    n = wraps;
    b = (b - m) % a;
    if (b < 0) b += a;
    const ll next_a = (-m) % a;
    m = a;
    a = next_a < 0 ? next_a + m : next_a;
  }
}

template <bool each, class U>
U unsigned_core(U n, U m, U a, U b)
{
  using W = larger_int_t<U>;
  U ans = b;
  for (;;)
  {
    if constexpr (each && sizeof(U) > 4)
      if ((n | m) <= numeric_limits<uint>::max())
        return min(ans, U(unsigned_core<false>(uint(n), uint(m), uint(a), uint(b))));
    if (a > m / 2) b = (W(a) * (n - 1) + b) % m, a = m - a;
    chmin(ans, b);
    const U wraps = (W(a) * (n - 1) + b) / m;
    if (wraps == 0 || ans == 0) return ans;
    n = wraps;
    b = (m - b) % a;
    if (b) b = a - b;
    const U next_a = m % a;
    m = a;
    a = next_a ? m - next_a : 0;
  }
}

template <int method>
ll run(const Query &q)
{
  auto [n, m, a, b] = q;
  if constexpr (method == 4) return min_of_mod_of_linear(n, m, a, b);
  a %= m; b %= m;
  if (a < 0) a += m;
  if (b < 0) b += m;
  if constexpr (method == 1)
    if ((ull(n) | ull(m)) <= numeric_limits<uint>::max()) return signed_core<ull>(n, m, a, b);
  if constexpr (method == 2)
    if ((ull(n) | ull(m)) <= numeric_limits<uint>::max())
      return unsigned_core<false>(uint(n), uint(m), uint(a), uint(b));
  if constexpr (method == 3) return unsigned_core<true>(ull(n), ull(m), ull(a), ull(b));
  return signed_core<i128>(n, m, a, b);
}

int main()
{
  cout << "function,dataset,method,queries,median_ms\n";
  mt19937_64 rng(20260925);
  using Fn = ll (*)(const Query &);
  const array<Fn, 5> functions = {run<0>, run<1>, run<2>, run<3>, run<4>};
  const array<const char *, 5> names = {"wide", "entry_product64", "entry32", "loop32", "library"};
  for (string dataset : {"small31", "small32", "large63", "mixed", "short_n", "trivial"})
  {
    vc<Query> queries;
    for (int i = 0; i < 100000; ++i)
    {
      int bits = dataset == "small31" ? 31 : dataset == "small32" ? 32 : dataset == "mixed" ? (i % 2 ? 31 : 63) : 63;
      auto draw = [&]() { return ll(rng() >> (64 - bits)); };
      Query q{draw() | 1, draw() | 1, draw(), draw()};
      if (dataset == "short_n") q.n = 1 + i % 100;
      if (dataset == "trivial") q.n = 1;
      if (i % 2) q.a = -q.a;
      if (i % 3) q.b = -q.b;
      queries.push_back(q);
    }
    for (const auto &q : queries)
    {
      ll expected = functions[0](q);
      for (auto f : functions) if (f(q) != expected) abort();
    }
    array<vc<double>, 5> times;
    for (int repeat = 0; repeat < 7; ++repeat)
      for (int j = 0; j < 5; ++j)
      {
        int method = (j + 2 * repeat) % 5;
        ull sum = 0;
        auto begin = Clock::now();
        for (const auto &q : queries) sum += functions[method](q);
        double ms = chrono::duration<double, milli>(Clock::now() - begin).count();
        checksum_sink = sum;
        times[method].push_back(ms);
      }
    for (int i = 0; i < 5; ++i)
    {
      sort(ALL(times[i]));
      cout << "min_of_mod_of_linear," << dataset << ',' << names[i] << ',' << queries.size()
           << ',' << fixed << setprecision(3) << times[i][3] << '\n';
    }
  }
}
