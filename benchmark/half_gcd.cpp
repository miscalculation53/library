// g++-15 -std=c++20 -O2 -DNDEBUG -I. benchmark/half_gcd.cpp -o /tmp/half-gcd-bench
// /tmp/half-gcd-bench [repetitions=5] [max_random_n=65536] [max_fast_n=262144]
#include "math/fps/berlekamp_massey.hpp"
#include "math/fps/berlekamp_massey_fast.hpp"

using mint = modint998244353;
using F = FieldAddSubMulDiv<mint>;
using P = FormalPowerSeries<mint>;
using Clock = chrono::steady_clock;
volatile uint64_t sink = 0;

uint64_t checksum(const vc<mint> &a)
{
  uint64_t h = a.size();
  for (auto x : a) h = h * 1000000007ULL + x.val();
  return h;
}

bool valid(const vc<mint> &a, const vc<mint> &c)
{
  auto p = convolution(a, c);
  for (int i = (int)c.size() - 1; i < (int)a.size(); ++i)
    if (p[i] != 0) return false;
  return !c.empty() && c.front() == mint(-1);
}

template <class Fn>
double measure(Fn fn)
{
  auto t = Clock::now();
  auto res = fn();
  double ms = chrono::duration<double, milli>(Clock::now() - t).count();
  sink = checksum(res);
  return ms;
}

void bench(const string &name, const vc<mint> &a, int reps, bool slow = true)
{
  auto f = [&] { return berlekamp_massey_fast(a); };
  auto s = [&] { return berlekamp_massey<F>(a); };
  auto c = f(); // warmup, also checked outside the timed region
  if (!valid(a, c)) abort();
  if (slow)
  {
    auto r = s();
    if (r.size() != c.size() || !valid(a, r)) abort();
    if (2 * (c.size() - 1) <= a.size() && r != c) abort();
  }
  for (int rep = 0; rep < reps; ++rep)
  {
    double old_ms = -1, fast_ms;
    if (slow && rep % 2 == 0) old_ms = measure(s);
    fast_ms = measure(f);
    if (slow && rep % 2 == 1) old_ms = measure(s);
    cout << name << ',' << a.size() << ',' << c.size() - 1 << ',' << rep
         << ',' << old_ms << ',' << fast_ms << ',' << checksum(c) << endl;
  }
}

int main(int argc, char **argv)
{
  int reps = argc > 1 ? stoi(argv[1]) : 5;
  int max_random = argc > 2 ? stoi(argv[2]) : 65536;
  int max_fast = argc > 3 ? stoi(argv[3]) : 262144;
  cout << fixed << setprecision(6);
  cout << "case,n,degree,rep,bm_ms,half_gcd_ms,checksum\n";
  for (int n = 128; n <= max_fast; n *= 2)
  {
    mt19937 rng(20260908 + n);
    vc<mint> a(n);
    for (auto &x : a) x = rng();
    bench("random", a, reps, n <= max_random);
  }
  for (int n : {6000, 8000, 10000, 12000, 16000})
  {
    mt19937 rng(20260908 + n);
    vc<mint> a(n);
    for (auto &x : a) x = rng();
    bench("random", a, reps, n <= max_random);
  }
  for (int n : {4096, 65536})
    for (int d : {1, 8, 32, 128, 512})
    {
      mt19937 rng(20260908 + d);
      P p(d), q(d + 1);
      for (auto &x : p) x = rng();
      for (auto &x : q) x = rng();
      q[0] = 1;
      auto a = (p * q.inv(n)).pre(n);
      bench("recurrence", a, reps);
    }
  vc<mint> a(65536);
  bench("zero", a, reps);
  a.back() = 1;
  bench("last_impulse", a, reps);
  a.back() = 0, a[0] = 1;
  bench("first_impulse", a, reps);

  // The shared full extended-GCD core, exercised on dense coprime inputs.
  for (int n : {1024, 4096, 16384, 65536})
  {
    mt19937 rng(20260908 + n);
    P f(n), mod(n + 1);
    for (auto &x : f) x = rng();
    for (auto &x : mod) x = rng();
    mod.back() = 1;
    auto fn = [&]
    {
      auto [ok, iv] = polynomial_inv_mod(f, mod);
      if (!ok) abort();
      return iv;
    };
    auto iv = fn();
    if ((f * iv) % mod != P{1}) abort();
    for (int rep = 0; rep < reps; ++rep)
      cout << "inverse," << n << ',' << iv.sz() - 1 << ',' << rep
           << ",-1," << measure(fn) << ',' << checksum(iv) << endl;
  }
}
