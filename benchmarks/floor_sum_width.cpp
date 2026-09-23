// g++-15 -std=c++17 -O2 -DNDEBUG -I . benchmarks/floor_sum_width.cpp -o /tmp/floor_sum_width_bench
// /tmp/floor_sum_width_bench > benchmarks/floor_sum_width.csv
#include "math/mod_of_linear/floor_sum.hpp"
#include "detail/floor_sum_width_reference.hpp"

using Clock = chrono::steady_clock;
volatile ull checksum_sink = 0;
struct Query { ull n, m, a, b; };

template <int method, class R, class T>
R run_floor(const Query &q)
{
  if constexpr (method == 4) return floor_sum<R>(T(q.n), T(q.m), T(q.a), T(q.b));
  else return floor_sum_width_reference::floor_sum<method, R>(T(q.n), T(q.m), T(q.a), T(q.b));
}

template <class R, class T>
void bench_floor(const string &dataset, const vc<Query> &queries, const char *result, const char *input)
{
  using Fn = R (*)(const Query &);
  const array<Fn, 7> functions = {run_floor<0, R, T>, run_floor<1, R, T>, run_floor<2, R, T>,
                                run_floor<3, R, T>, run_floor<4, R, T>, run_floor<5, R, T>, run_floor<6, R, T>};
  const array<const char *, 7> names = {"wide", "entry32", "loop32", "product64", "library", "entry_product64", "entry_bound64"};
  for (const auto &q : queries)
  {
    const R expected = functions[0](q);
    for (auto f : functions) if (f(q) != expected) abort();
  }
  array<vc<double>, 7> times;
  for (int repeat = 0; repeat < 7; ++repeat)
    for (int j = 0; j < 7; ++j)
    {
      int method = (j + 2 * repeat) % 7;
      R sum = 0;
      auto begin = Clock::now();
      for (const auto &q : queries) sum += functions[method](q);
      double ms = chrono::duration<double, milli>(Clock::now() - begin).count();
      checksum_sink = ull(sum);
      times[method].push_back(ms);
    }
  for (int i = 0; i < 7; ++i)
  {
    sort(ALL(times[i]));
    cout << "floor_sum," << dataset << ',' << input << ',' << result << ',' << names[i]
         << ',' << queries.size() << ',' << fixed << setprecision(3) << times[i][3] << '\n';
  }
}

int main()
{
  cout << "function,dataset,input,result,method,queries,median_ms\n";
  mt19937_64 rng(20260925);
  const int count = 100000;
  for (string dataset : {"small31", "small32", "large63", "large64", "mixed", "large_n", "large_coeff", "trivial"})
  {
    vc<Query> queries;
    for (int i = 0; i < count; ++i)
    {
      int bits = dataset == "small31" ? 31 : dataset == "small32" ? 32 :
                 dataset == "large63" ? 63 : dataset == "mixed" ? (i % 2 ? 31 : 63) : 64;
      auto draw = [&]() { return rng() >> (64 - bits); };
      Query q{draw(), draw() | 1, draw(), draw()};
      if (dataset == "large_n") q.m >>= 33, q.m |= 1, q.a %= q.m, q.b %= q.m;
      if (dataset == "large_coeff") q.n >>= 33, q.m >>= 33, q.m |= 1;
      if (dataset == "trivial") q.n = i % 2, q.m = 1;
      queries.push_back(q);
    }
    bench_floor<ull, ull>(dataset, queries, "u64", "u64");
    if (dataset == "small31")
    {
      bench_floor<ull, uint>(dataset, queries, "u64", "u32");
      bench_floor<u128, ull>(dataset, queries, "u128", "u64");
      bench_floor<ull, u128>(dataset, queries, "u64", "u128");
    }
  }
}
