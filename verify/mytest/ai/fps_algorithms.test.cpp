#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/interpolation.hpp"
#include "math/convolution/middle_product.hpp"

template <class mint>
void check_algorithms()
{
  using F = FormalPowerSeries<mint>;
  mt19937 rng(41);
  for (int n : {1, 2, 3, 31, 32, 33, 63, 64, 65, 127, 128, 129, 511, 512, 513, 1024, 1025, 4096, 4097})
  {
    F f(n), h(n);
    for (auto &x : f) x = 1 + rng() % (mint::mod() - 1);
    f[0] = 1;
    h = f; h[0] = 0;
    assert((f * f.inv(n)).resized(n) == F{1}.resized(n));
    assert(h.exp(n).log(n) == h);
    if (n == 513 || n == 4097)
    {
      auto shifted = f << (n / 5);
      assert(shifted.pow(2, n) == shifted.pow_sparse(2, n));
      assert(shifted.pow(8, n) == F(n));
    }
    auto square = (f * f).resized(n);
    auto [ok, root] = square.sqrt(n);
    assert(ok && (root * root).resized(n) == square);
    for (int k : {0, 1, 2, 3, 8, 9, -1, -5})
      assert(f.pow(k, min(n, 129)) == f.pow_sparse(k, min(n, 129)));
    // The output depends only on the requested prefix.
    auto longer = f; longer.resize(2 * n + 5, mint(19));
    assert(longer.inv(n) == f.inv(n));
    int prefix = min(n, 129);
    assert(longer.pow(9, prefix) == f.pow(9, prefix));
    assert(longer.pow_sparse(9, prefix) == f.pow_sparse(9, prefix));
    assert(longer.sqrt_sparse(prefix) == f.sqrt_sparse(prefix));
    longer[0] = 0;
    assert(longer.exp(n) == h.exp(n));
    // An exponential with valuation >= n/2 is exactly 1+h mod x^n.
    F late(n); for (int i = (n + 1) / 2; i < n; ++i) late[i] = f[i];
    assert(late.exp(n) == (late + F{1}).resized(n));
    for (int d : {(n + 1) / 2, (n + 2) / 3})
    {
      F tail(n); tail[0] = 1;
      for (int i = max(1, d); i < n; ++i) tail[i] = f[i];
      assert(tail.inv(n) == F{1}.div_sparse(tail, n));
      assert(tail.pow(9, n) == tail.pow_sparse(9, n));
      assert(tail.pow(-5, n) == tail.pow_sparse(-5, n));
      auto [exists, root] = tail.sqrt(n);
      assert(exists && (root * root).resized(n) == tail);
    }
  }
  for (int n : {0, 1, 2, 31, 32, 33, 63, 64, 65, 127, 128, 129, 511, 512, 513})
  {
    F f(n); for (auto &x : f) x = rng();
    for (int points : {0, 1, 5, 64, 65, 127, 129})
    {
      vc<mint> xs(points);
      for (int i = 0; i < points; ++i) xs[i] = i % 13; // Includes repeated points and zero.
      auto values = multipoint_evaluation(f, xs);
      for (int i = 0; i < points; ++i) assert(values[i] == f.eval(xs[i]));
    }
    vc<mint> xs(n), ys(n);
    for (int i = 0; i < n; ++i) xs[i] = i, ys[i] = f.eval(xs[i]);
    assert(interpolation(xs, ys) == f);
  }
  for (int n : {1, 32, 64, 65, 128, 257, 513})
    for (int m : {1, 7, 32, 64, 129, 257, 513}) if (m <= n)
    {
      vc<mint> a(n), b(m);
      for (auto &x : a) x = rng();
      for (auto &x : b) x = rng();
      auto value = middle_product(a, b);
      for (int i = 0; i <= n - m; ++i)
      {
        mint expected = 0;
        for (int j = 0; j < m; ++j) expected += a[i+j] * b[j];
        assert(value[i] == expected);
      }
    }
}

int main()
{
  check_algorithms<modint998244353>();
  check_algorithms<static_modint32<1811939329>>();
  check_algorithms<static_modint32<1000000007>>();
  dynamic_modint32<27>::set_mod(998244353);
  check_algorithms<dynamic_modint32<27>>();
  using M = static_modint32<17>; using F = FormalPowerSeries<M>;
  for (int n = 0; n <= 17; ++n)
  {
    F h(n); for (int i = 1; i < n; ++i) h[i] = i;
    auto g = h.exp(n), expected = h.exp_sparse(n);
    assert(g == expected);
    F f = h; if (n) f[0] = 1;
    if (n) assert((f * f.inv(n)).resized(n) == F{1}.resized(n));
  }
  PRINT("Hello World");
  return 0;
}
