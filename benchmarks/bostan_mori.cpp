// g++-15 -std=c++17 -O2 -DNDEBUG -I . benchmarks/bostan_mori.cpp -o /tmp/bostan_mori_bench
// /tmp/bostan_mori_bench > benchmarks/bostan_mori.csv
#include "math/fps/bostan_mori.hpp"
#include "detail/bostan_mori_reference.hpp"

using Clock = chrono::steady_clock;
volatile ull bostan_mori_checksum = 0;

template <class mint>
void compare(int d, const string &shape, mt19937_64 &rng)
{
  using F = FormalPowerSeries<mint>;
  using Fn = mint (*)(const F &, const F &, ll);
  const array<Fn, 2> methods = {bostan_mori_reference<mint>, bostan_mori<mint>};
  const int samples = d <= 1024 ? 3 : 1;
  vc<pair<F, F>> inputs;
  for (int sample = 0; sample < samples; ++sample)
  {
    F p(shape == "improper" ? 2 * d + 7 : d), q(d + 1);
    for (auto &x : p) x = 1 + rng() % (mint::mod() - 1);
    for (auto &x : q) x = 1 + rng() % (mint::mod() - 1);
    if (shape == "sparse")
      for (int i = 1; i < d; ++i) if (i != d / 2) q[i] = 0;
    inputs.emplace_back(move(p), move(q));
  }
  vc<ll> indices = {0, d / 2, d, 4LL * d, 1000000000LL, 1000000000000000000LL};
  if (shape != "dense") indices = {d, 1000000000000000000LL};
  sort(indices.begin(), indices.end());
  indices.erase(unique(indices.begin(), indices.end()), indices.end());
  for (ll k : indices)
  {
    ull expected = 0;
    for (const auto &[p, q] : inputs)
    {
      mint ref = methods[0](p, q, k);
      if (methods[1](p, q, k) != ref) abort();
      expected += ref.val();
    }
    auto measure = [&](int method, int repeat)
    {
      ull sum = 0;
      const auto start = Clock::now();
      for (int t = 0; t < repeat; ++t)
        for (const auto &[p, q] : inputs) sum += methods[method](p, q, k).val();
      const double ms = chrono::duration<double, milli>(Clock::now() - start).count();
      if (sum != expected * repeat) abort();
      bostan_mori_checksum ^= sum;
      return ms;
    };
    const double pilot = measure(0, 1);
    const int repeat = clamp(int(4.0 / max(pilot, 0.001)), 1, 1024);
    array<vc<double>, 2> times;
    for (int round = 0; round < 7; ++round)
      for (int j = 0; j < 2; ++j)
      {
        const int method = (j + round) % 2;
        times[method].push_back(measure(method, repeat) / (repeat * samples));
      }
    for (auto &t : times) sort(t.begin(), t.end());
    cout << mint::mod() << ',' << d << ',' << shape << ',' << k << ','
         << samples << ',' << repeat << ',' << times[0][3] << ',' << times[1][3] << ','
         << times[0][3] / times[1][3] << ',' << expected << '\n';
    cout.flush();
  }
}

template <class mint>
void benchmark()
{
  mt19937_64 rng(20260926);
  for (int d : {1, 2, 4, 8, 16, 32, 63, 64, 65, 256, 1024, 8192, 65536})
  {
    if (mint::mod() == 1000000007 && d > 8192) continue;
    compare<mint>(d, "dense", rng);
  }
  for (int d : {64, 8192})
    for (const string shape : {"sparse", "improper"}) compare<mint>(d, shape, rng);
}

int main()
{
  cout << fixed << setprecision(6);
  cout << "mod,d,shape,k,samples,repeat,original_ms,hybrid_ms,speedup,checksum\n";
  benchmark<modint998244353>();
  benchmark<modint1000000007>();
}
