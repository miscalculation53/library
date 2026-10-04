// Run with python3 benchmarks/runtime_width.py.
#include "runtime_width_reference.hpp"
#include "math/bigint.hpp"
#include "math/quadratic_equation_integer.hpp"
#include "math/linear_equations_integer.hpp"
#include "math/svp2d.hpp"
#include "math/modint/modint.hpp"
#include "math/rational.hpp"
#include "math/crt.hpp"

using Clock = chrono::steady_clock;
volatile ull width_sink = 0;
int repeats = 7;

template <class F, class G>
void measure(string function, string dataset, int count, F before, G after)
{
  if (before() != after()) abort();
  array<vc<double>, 2> times;
  for (int k = 0; k < repeats; ++k)
    for (int j = 0; j < 2; ++j)
    {
      int method = (j + k) % 2;
      auto start = Clock::now();
      width_sink = method ? after() : before();
      times[method].push_back(chrono::duration<double, milli>(Clock::now() - start).count());
    }
  for (int method = 0; method < 2; ++method)
  {
    sort(ALL(times[method]));
    cout << function << ',' << dataset << ',' << (method ? "candidate" : "before")
         << ',' << count << ',' << fixed << setprecision(3) << times[method][repeats / 2] << '\n';
  }
}

template <class Big>
Big scalar(Big x, ll v, int op)
{
  if (op == 0) x *= v;
  if (op == 1) x /= v;
  if (op == 2) x %= v;
  if (op == 3) x += v;
  if (op == 4) x -= v;
  if (op == 5) x.add_term(5, v);
  if (op == 6) x.sub_term(5, v);
  return x;
}

template <class Big>
ull scalar_many(const vc<Big> &values, const vl &scalars, int op)
{
  ull sum = 0;
  for (int i = 0; i < int(values.size()); ++i)
  {
    Big result = scalar(values[i], scalars[i], op);
    sum += result > 0;
    // Consume the complete value once per batch without formatting every result.
    if (i % 257 == 0) sum += hash<string>{}(result.to_string());
  }
  return sum;
}

void bigint_bench(mt19937_64 &rng)
{
  for (int digits : {12, 600, 6000})
    for (string size : {"small", "large", "mixed"})
    {
      const int n = digits == 12 ? 100000 : digits == 600 ? 6000 : 600;
      vc<BigInteger_before<>> before;
      vc<BigInteger<>> after;
      vl scalars;
      for (int i = 0; i < n; ++i)
      {
        string value(digits, '0');
        for (char &c : value) c += rng() % 10;
        value[0] = '1' + rng() % 9;
        if (i & 1) value = '-' + value;
        before.emplace_back(value);
        after.emplace_back(value);
        ll bound = size == "large" || (size == "mixed" && (i & 1)) ? (1LL << 60) : 1000000000;
        ll v = rng() % bound + 1;
        scalars.push_back(i & 2 ? -v : v);
      }
      for (int op = 0; op < 7; ++op)
      {
        for (int i = 0; i < n; ++i)
          if (scalar(before[i], scalars[i], op).to_string() != scalar(after[i], scalars[i], op).to_string())
            abort();
        measure("bigint_" + string(array<const char *, 7>{"mul", "div", "mod", "add", "sub", "add_term", "sub_term"}[op]),
                size + "_" + to_string(digits), n,
                [&] { return scalar_many(before, scalars, op); },
                [&] { return scalar_many(after, scalars, op); });
      }
    }
}

template <bool old>
auto quadratic(const array<ll, 3> &q)
{
  if constexpr (old) return quadratic_equation_integer_before(q[0], q[1], q[2]);
  else return quadratic_equation_integer(q[0], q[1], q[2]);
}
template <bool old>
auto linear(const array<ll, 6> &q)
{
  if constexpr (old) return linear_equations_integer_before(q[0], q[1], q[2], q[3], q[4], q[5]);
  else return linear_equations_integer(q[0], q[1], q[2], q[3], q[4], q[5]);
}
template <bool old>
auto svp(const array<ll, 4> &q)
{
  if constexpr (old) return svp2d_before(pair<ll, ll>{q[0], q[1]}, {q[2], q[3]});
  else return svp2d(pair<ll, ll>{q[0], q[1]}, {q[2], q[3]});
}

void equations_bench(mt19937_64 &rng)
{
  const int n = 100000;
  for (string size : {"small", "large", "mixed"})
  {
    vc<array<ll, 3>> qs(n), roots(n);
    vc<array<ll, 6>> ls(n), dependent(n);
    vc<array<ll, 4>> vs(n);
    for (int i = 0; i < n; ++i)
    {
      ll bound = size == "large" || (size == "mixed" && (i & 1)) ? (1LL << 50) : 1000000000;
      auto draw = [&] { return ll(rng() % (2 * bound + 1)) - bound; };
      for (ll &x : qs[i]) x = draw();
      // These equations always have integer roots, with coefficients in range.
      ll x = ll(rng() % 20001) - 10000, y = ll(rng() % 20001) - 10000;
      ll scale = bound > 1000000000 ? 1000000000 : 1;
      roots[i] = {scale, -scale * (x + y), scale * x * y};
      for (ll &v : ls[i]) v = draw();
      ll p = draw(), q = draw();
      dependent[i] = {p, q, 1, p, q, 1};
      for (ll &v : vs[i]) v = draw();
      if (vs[i][0] == 0 && vs[i][1] == 0) vs[i][0] = 1;
      if (vs[i][2] == 0 && vs[i][3] == 0) vs[i][2] = 1;
    }
    for (auto *data : {&qs, &roots})
    {
      for (const auto &q : *data) if (quadratic<true>(q) != quadratic<false>(q)) abort();
      auto run = [&](auto old) {
        ull sum = 0;
        for (auto &q : *data) { auto r = quadratic<decltype(old)::value>(q); sum += r.first + ull(r.second[0]) + ull(r.second[1]); }
        return sum;
      };
      measure(data == &qs ? "quadratic_random" : "quadratic_roots", size, n,
              [&] { return run(true_type{}); }, [&] { return run(false_type{}); });
    }
    for (auto *data : {&ls, &dependent})
    {
      for (const auto &q : *data)
      {
        auto a = linear<true>(q); auto b = linear<false>(q);
        if (a.dim != b.dim || a.sol != b.sol || a.basis != b.basis) abort();
      }
      auto run = [&](auto old) {
        ull sum = 0;
        for (auto &q : *data) { auto r = linear<decltype(old)::value>(q); sum += r.dim + ull(r.sol.first) + ull(r.sol.second); }
        return sum;
      };
      measure(data == &ls ? "linear_random" : "linear_dependent", size, n,
              [&] { return run(true_type{}); }, [&] { return run(false_type{}); });
    }
    for (const auto &q : vs) if (svp<true>(q) != svp<false>(q)) abort();
    auto run = [&](auto old) {
      ull sum = 0;
      for (auto &q : vs) { auto r = svp<decltype(old)::value>(q); sum += ull(r.first) + ull(r.second); }
      return sum;
    };
    measure("svp2d", size, n, [&] { return run(true_type{}); }, [&] { return run(false_type{}); });
  }
}

template <ll mod>
void modint_bench(mt19937_64 &rng)
{
  using Old = internal::modint_impl<internal::policy_static_before<mod>>;
  using New = static_modint64<mod>;
  const int n = 1000000;
  vc<Old> a; vc<New> b;
  for (int i = 0; i < n; ++i)
  {
    ll x = rng() % mod;
    a.emplace_back(x); b.emplace_back(x);
  }
  auto run = [](const auto &values) {
    using Mint = typename decay_t<decltype(values)>::value_type;
    Mint sum = 1;
    for (const auto &x : values) { sum *= x; sum += 1; }
    return ull(sum.val());
  };
  measure("static_modint64", to_string(mod), n, [&] { return run(a); }, [&] { return run(b); });
}

void rational_bench(mt19937_64 &rng)
{
  for (string size : {"small", "large", "mixed"})
  {
    const int n = 1000000;
    vc<array<ll, 4>> values(n);
    for (int i = 0; i < n; ++i)
    {
      ll bound = size == "large" || (size == "mixed" && (i & 1)) ? (1LL << 50) : 1000000000;
      values[i] = {ll(rng() % (2 * bound)) - bound, ll(rng() % bound) + 1,
                   ll(rng() % (2 * bound)) - bound, ll(rng() % bound) + 1};
    }
    auto run = [&](auto small) {
      ull sum = 0;
      for (auto [a, b, c, d] : values)
      {
        if constexpr (decltype(small)::value)
          if (-INT_MAX <= a && a <= INT_MAX && b <= INT_MAX &&
              -INT_MAX <= c && c <= INT_MAX && d <= INT_MAX)
          { sum += a * d < b * c; continue; }
        sum += i128(a) * d < i128(b) * c;
      }
      return sum;
    };
    measure("rational_compare", size, n, [&] { return run(false_type{}); }, [&] { return run(true_type{}); });
    auto evaluate = [&](auto small) {
      ull sum = 0;
      for (auto [a, x, b, unused] : values)
      {
        i128 value;
        if constexpr (decltype(small)::value)
        {
          constexpr ull offset = 1ULL << 31;
          if (((ull(a) + offset) | (ull(x) + offset) | (ull(b) + offset)) < 2 * offset)
            value = a * x + b;
          else value = i128(a) * x + b;
        }
        else value = i128(a) * x + b;
        sum += ull(value) ^ ull(u128(value) >> 64);
      }
      return sum;
    };
    measure("line_eval", size, n, [&] { return evaluate(false_type{}); }, [&] { return evaluate(true_type{}); });
  }
}

template <int n, bool large>
void crt_constexpr_bench(mt19937_64 &rng)
{
  constexpr array<ll, 4> primes = large
    ? array<ll, 4>{1000000000039LL, 1000000000061LL, 1000000000063LL, 1000000000091LL}
    : array<ll, 4>{1000000007, 1000000009, 998244353, 1000000033};
  constexpr auto moduli = [&] {
    array<ll, n> result{};
    for (int i = 0; i < n; ++i) result[i] = primes[i];
    return result;
  }();
  vc<array<ll, n>> residues(100000);
  for (auto &r : residues) for (int i = 0; i < n; ++i) r[i] = rng() % moduli[i];
  auto run = [&](auto old) {
    ull sum = 0;
    for (const auto &r : residues)
    {
      if constexpr (decltype(old)::value) sum += crt_mod_constexpr_before<modint998244353>(r, moduli).first.val();
      else sum += crt_mod_constexpr<modint998244353>(r, moduli).first.val();
    }
    return sum;
  };
  measure("crt_mod_constexpr", string(large ? "large40_" : "small31_") + to_string(n),
          residues.size(), [&] { return run(true_type{}); }, [&] { return run(false_type{}); });
}

int main(int argc, char **argv)
{
  if (argc > 1) repeats = stoi(argv[1]);
  mt19937_64 rng(20260930);
  cout << "function,dataset,method,queries,median_ms\n";
  bigint_bench(rng);
  equations_bench(rng);
  modint_bench<1000000007>(rng);
  modint_bench<4294967295LL>(rng);
  modint_bench<(1LL << 61) - 1>(rng);
  rational_bench(rng);
  crt_constexpr_bench<2, false>(rng);
  crt_constexpr_bench<4, false>(rng);
  crt_constexpr_bench<2, true>(rng);
  crt_constexpr_bench<4, true>(rng);
}
