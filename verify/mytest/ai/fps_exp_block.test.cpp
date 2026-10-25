#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/fps.hpp"

// Dense coefficients near the modulus stress deferred sums and reused workspace.
template <class mint> void check_exp()
{
  using F = FormalPowerSeries<mint>;
  mt19937 rng(20261023);
  for (int n : {511, 512, 513, 1024, 1025})
  {
    F h(n);
    for (int i = 1; i < n; ++i) h[i] = mint(mint::mod() - 1 - rng() % 23);
    F expected = h.exp_sparse(n);
    assert(h.exp(n) == expected);
    for (int blocks : {4, 16, 32})
      assert(internal::fps_exp_block(h, n, blocks) == expected);
    assert(internal::fps_exp_block(h, n, 16, false) == expected);
    h.resize(n + 7, mint(19));
    assert(h.exp(n) == expected);
  }
  F zero(513);
  assert(internal::fps_exp_block(zero, 513, 16) == F{1}.resized(513));
}

int main()
{
  check_exp<modint998244353>();
  check_exp<static_modint32<1811939329>>();
  check_exp<static_modint32<2113929217>>();
  check_exp<static_modint64<998244353LL>>();
  PRINT("Hello World");
}
