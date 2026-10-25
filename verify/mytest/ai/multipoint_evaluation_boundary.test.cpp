#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/multipoint_evaluation.hpp"

// Test focus: cached transform doubling, sparse products, repeated points, and empty inputs.
template <class mint> void check_tree()
{
  using F = FormalPowerSeries<mint>;
  mt19937 rng(20261023);
  F f(513);
  for (auto &a : f) a = rng();
  for (int m : {127, 128, 129, 255, 256, 257, 513})
  {
    vc<mint> xs(m);
    for (int mode = 0; mode < 3; ++mode)
    {
      for (int i = 0; i < m; ++i)
        xs[i] = mode == 0 ? mint(rng()) : mode == 1 ? mint(i % 13) : mint(i % 3 ? rng() : 0);
      auto ys = multipoint_evaluation(f, xs);
      for (int i = 0; i < m; ++i) assert(ys[i] == f.eval(xs[i]));
    }
  }
}
int main()
{
  using mint = modint998244353;
  using F = FormalPowerSeries<mint>;
  assert(multipoint_evaluation(F{1, 2}, {}) == vc<mint>{});
  assert(multipoint_evaluation(F{1, 2}, {3}) == vc<mint>{7});
  check_tree<modint998244353>();
  check_tree<static_modint32<1811939329>>();
  check_tree<modint1000000007>();
  dynamic_modint32<28>::set_mod(998244353);
  check_tree<dynamic_modint32<28>>();
  PRINT("Hello World");
}
