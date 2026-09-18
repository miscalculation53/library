// g++-15 -std=c++20 -O2 -I. benchmark/bbla_extensions.cpp -o /tmp/bbla-extensions
// /tmp/bbla-extensions [n=1000] [noncyclic_n=256] [repetitions=3]
#include "math/linalg/bbla_rank_solve.hpp"
#include "math/linalg/bbla_invariant_factors.hpp"

using mint = modint998244353;
using F = FieldAddSubMulDiv<mint>;
using V = vc<mint>;

template <class Run>
void measure(const string &name, int n, int repetitions, int &calls, const Run &run)
{
  vc<double> times;
  vc<int> counts;
  repi(i, -1, repetitions)
  {
    mt.seed(20260911 + i);
    calls = 0;
    auto start = chrono::steady_clock::now();
    run(); // Also checks the result against the known matrix structure.
    double ms = chrono::duration<double, milli>(chrono::steady_clock::now() - start).count();
    if (i >= 0) { times.eb(ms); counts.eb(calls); }
  }
  sort(times.begin(), times.end());
  sort(counts.begin(), counts.end());
  cout << name << ',' << n << ',' << times[repetitions / 2] << ',' << counts[repetitions / 2] << '\n';
}

int main(int argc, char **argv)
{
  int n = argc > 1 ? stoi(argv[1]) : 1000;
  int k = argc > 2 ? stoi(argv[2]) : 256;
  int repetitions = argc > 3 ? stoi(argv[3]) : 3;
  assert(n >= 8 && k > 0 && k % 4 == 0 && repetitions > 0);
  cout << fixed << setprecision(3) << "case,n,median_ms,median_map_calls\n";
  int calls = 0;

  // n by (n+31), rank n-7, A=L*diag(I_r,0)*U with bidiagonal L,U.
  int cols = n + 31, r = n - 7;
  auto action = [&](V &v, int output_size)
  {
    calls++;
    repi(i, int(v.size()) - 1) v[i] += v[i + 1];
    v.resize(r); v.resize(output_size);
    for (int i = output_size - 1; i > 0; i--) v[i] += v[i - 1];
  };
  auto apply = [&](V &v) { action(v, n); };
  auto transpose = [&](V &v) { action(v, cols); };
  V b(cols, 1);
  apply(b);
  measure("rank_rectangular", n, repetitions, calls, [&]
  {
    assert(bbla::rank<F>(n, cols, apply, transpose) == r);
  });
  measure("solve_singular_rectangular", n, repetitions, calls, [&]
  {
    auto result = bbla::solve_general<F>(n, cols, apply, transpose, b);
    assert(result.solvable);
    apply(result.solution);
    assert(result.solution == b);
  });

  auto cyclic = [&](V &v) { calls++; rotate(v.begin(), v.begin() + 1, v.end()); };
  V p(n + 1); p[0] = -1; p[n] = 1;
  measure("charpoly_cyclic", n, repetitions, calls, [&]
  {
    assert(bbla::characteristic_polynomial<F>(n, cyclic) == p);
  });
  measure("invariants_cyclic", n, repetitions, calls, [&]
  {
    assert((bbla::invariant_factors<F>(n, cyclic) == vc<V>{p}));
  });

  auto blocks = [&](V &v)
  {
    calls++;
    repi(i, 4) rotate(v.begin() + i * (k / 4), v.begin() + i * (k / 4) + 1,
                      v.begin() + (i + 1) * (k / 4));
  };
  V block(k / 4 + 1); block[0] = -1; block.back() = 1;
  V characteristic(k + 1);
  characteristic[0] = 1; characteristic[k / 4] = -4; characteristic[k / 2] = 6;
  characteristic[3 * k / 4] = -4; characteristic[k] = 1;
  measure("charpoly_four_equal_blocks", k, repetitions, calls, [&]
  {
    assert(bbla::characteristic_polynomial<F>(k, blocks) == characteristic);
  });
  measure("invariants_four_equal_blocks", k, repetitions, calls, [&]
  {
    assert(bbla::invariant_factors<F>(k, blocks) == vc<V>(4, block));
  });
}
