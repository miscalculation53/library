// g++-15 -std=c++17 -O2 -DNDEBUG -I . benchmarks/crt_mod_width.cpp -o /tmp/crt_mod_width_bench
#include "detail/crt_mod_width_reference.hpp"
#include "detail/crt_mod_width_candidate.hpp"
#include "math/modint/modint.hpp"
#include "math/prime/large/primality_test.hpp"

using Clock = chrono::steady_clock;
using mint = modint998244353;
volatile ull checksum_sink = 0;
struct Query { vl residues, moduli; };

template <int method>
pair<mint, mint> run(const Query &q)
{
  if constexpr (method == 1) return crt_mod_width_candidate<mint>(q.residues, q.moduli);
  else if constexpr (method == 2) return crt_mod<mint>(q.residues, q.moduli);
  else return crt_mod_before_width<mint>(q.residues, q.moduli);
}

int main()
{
  cout << "function,dataset,method,queries,median_ms\n";
  mt19937_64 rng(20260925);
  for (bool small : {true, false}) for (int size : {2, 4, 8, 16, 32})
  {
    vl moduli;
    for (ll p = small ? 1000000007LL : 1000000000039LL; moduli.size() < size_t(size); p += 2)
      if (is_prime(p)) moduli.push_back(p);
    vc<Query> queries;
    for (int i = 0; i < (size == 32 ? 10000 : 50000); ++i)
    {
      Query q{{}, moduli};
      for (ll p : moduli) q.residues.push_back(rng() % p);
      queries.push_back(q);
    }
    using Fn = pair<mint, mint> (*)(const Query &);
    const array<Fn, 3> functions = {run<0>, run<1>, run<2>};
    for (const auto &q : queries) if (functions[0](q) != functions[1](q) || functions[0](q) != functions[2](q)) abort();
    array<vc<double>, 3> times;
    for (int repeat = 0; repeat < 9; ++repeat)
      for (int j = 0; j < 3; ++j)
      {
        int method = (j + repeat) % 3;
        ull sum = 0;
        auto begin = Clock::now();
        for (const auto &q : queries) sum += functions[method](q).first.val();
        times[method].push_back(chrono::duration<double, milli>(Clock::now() - begin).count());
        checksum_sink = sum;
      }
    for (int method = 0; method < 3; ++method)
    {
      sort(ALL(times[method]));
      cout << "crt_mod," << (small ? "small31_" : "large40_") << size << ','
           << (array<const char *, 3>{"wide", "candidate", "library"}[method]) << ',' << queries.size() << ','
           << fixed << setprecision(3) << times[method][4] << '\n';
    }
  }
}
