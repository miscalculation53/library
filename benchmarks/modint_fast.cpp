// g++-15 -std=c++17 -O2 -DNDEBUG -I . benchmarks/modint_fast.cpp -o /tmp/modint_fast_bench
// /tmp/modint_fast_bench > benchmarks/modint_fast.csv
#include "math/modint/modint_fast.hpp"

using Clock = chrono::steady_clock;
volatile ull modint_fast_checksum = 0;

template <int p>
void benchmark(int count)
{
  using normal = static_modint32<p>;
  using fast = fast_modint<p>;
  auto start = Clock::now();
  fast::precompute();
  const double build_ms = chrono::duration<double, milli>(Clock::now() - start).count();
  mt19937_64 rng(12345);
  vc<pair<uint, uint>> input(count);
  for (auto &[a, e] : input) a = 1 + rng() % (p - 1), e = rng() % p;
  auto run = [&](auto sample, bool inverse)
  {
    using mint = decltype(sample);
    ull sum = 0;
    for (auto [a, e] : input)
      sum += (inverse ? mint::raw(a).inv() : mint::raw(a).pow(e)).val();
    return sum;
  };
  for (bool inverse : {false, true})
  {
    const ull expected = run(normal(), inverse);
    array<vc<double>, 2> times;
    for (int round = 0; round < 5; ++round)
      for (int j = 0; j < 2; ++j)
      {
        const int method = (round + j) % 2;
        start = Clock::now();
        const ull actual = method ? run(fast(), inverse) : run(normal(), inverse);
        const double ns = chrono::duration<double, nano>(Clock::now() - start).count() / count;
        if (actual != expected) abort();
        modint_fast_checksum = actual;
        times[method].push_back(ns);
      }
    for (auto &t : times) sort(t.begin(), t.end());
    cout << p << ',' << (inverse ? "inv" : "pow") << ',' << count << ',' << build_ms << ','
         << times[0][2] << ',' << times[1][2] << ',' << times[0][2] / times[1][2] << '\n';
  }
}

int main(int argc, char **argv)
{
  const int count = argc > 1 ? stoi(argv[1]) : 1000000;
  assert(count > 0);
  cout << "mod,operation,queries,precompute_ms,normal_ns,fast_ns,speedup\n";
  benchmark<998244353>(count);
  benchmark<1000000007>(count);
  benchmark<2147483647>(count);
}
