#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/compositional_inverse.hpp"

template <class mint>
using Fps = FormalPowerSeries<mint>;

template <class mint>
Fps<mint> product(const Fps<mint> &f, const Fps<mint> &g, int n)
{
  Fps<mint> h(n);
  repi(i, min(n, f.sz())) repi(j, min(n - i, g.sz())) h[i + j] += f[i] * g[j];
  return h;
}

template <class mint>
Fps<mint> compose(const Fps<mint> &f, const Fps<mint> &g, int n)
{
  Fps<mint> h(n);
  repi(i, f.sz() - 1, -1, -1)
  {
    h = product(h, g, n);
    if (n > 0) h[0] += f[i];
  }
  return h;
}

template <class mint>
Fps<mint> dense(int n, int seed = 1)
{
  Fps<mint> f(n);
  repi(i, n) f[i] = mint((ll)(i + seed) * (i + seed + 7) + 1);
  return f;
}

template <class mint>
void test_lengths()
{
  using F = Fps<mint>;
  const F a{1, 2, 3};
  assert(a.resized(0).empty());
  assert((a.resized(2) == F{1, 2}));
  assert(a.resized(3) == a);
  assert((a.resized(5) == F{1, 2, 3, 0, 0}));
  assert((a.resized(5).rev() == F{0, 0, 3, 2, 1}));
  assert(F{}.resized(5) == F(5));
  assert((a == F{1, 2, 3}));

  for (int n : {0, 1, 2, 3, 8, 19})
  {
    const F zero(n), one = F{1}.resized(n);
    assert(F{1}.inv(n) == one);
    assert(F{1}.log(n) == zero);
    assert(F{}.exp(n) == one);
    assert(F{}.exp_sparse(n) == one);
    assert(F{}.pow(0, n) == one);
    assert(F{}.pow_sparse(0, n) == one);
    assert(F{}.pow(3, n) == zero);
    assert(F{}.pow_sparse(3, n) == zero);
    assert(F{}.sqrt(n) == make_pair(true, zero));
    assert(F{}.sqrt_sparse(n) == make_pair(true, zero));
    assert(F{1}.sqrt(n).second == one || F{1}.sqrt(n).second == -one);

    const F f{1, 1};
    assert((f * f == F{1, 2, 1}));
    assert((f.pow(2, n) == F{1, 2, 1}.resized(n)));
    assert(f.pow(2, n) == f.resized(30).pow(2, n));
    assert(f.pow(2, n) == f.pow_sparse(2, n));
    assert(f.log(n).exp(n) == f.resized(n));
    F exponential(n);
    repi(i, n) exponential[i] = Binomial<mint>::finv(i);
    assert((F{0, 1}.exp(n) == exponential));
    assert((F{0, 1}.exp_sparse(n) == exponential));

    for (const F &numerator : vc<F>{{}, {0}, {2}, {3, 4, 5}})
    {
      for (const F &denominator : vc<F>{{2}, {1, -1}, {3, 2, 1}})
      {
        const F quotient = numerator.div(denominator, n);
        assert(quotient.sz() == n);
        assert(product(quotient, denominator, n) == numerator.resized(n));
        assert(numerator.div_sparse(denominator, n) == quotient);
        F changed = numerator;
        assert(changed.div_sparse_destructive(denominator, n) == quotient);
        assert(changed == quotient);
      }
      for (int k : {0, 1, 3, 30})
      {
        F denominator(k + 1);
        denominator[0] = 1;
        denominator[k] += 2;
        const F quotient = numerator.div_bin(k, mint(2), n);
        assert(quotient.sz() == n);
        assert(product(quotient, denominator, n) == numerator.resized(n));
        F changed = numerator;
        assert(changed.div_bin_destructive(k, mint(2), n) == quotient);
        assert(changed == quotient);
      }
    }
    F self{2, 3, 4};
    self.div_sparse_destructive(self, n);
    assert(self == one);
  }
}

template <class mint>
void test_dense_lengths(int size)
{
  using F = Fps<mint>;
  F f = dense<mint>(size), e = dense<mint>(size, 2);
  f[0] = 1, e[0] = 0;
  for (int n : {0, 1, 7, size - 1, size + 13})
  {
    const F one = F{1}.resized(n);
    assert(f.inv(n).sz() == n);
    assert(product(f, f.inv(n), n) == one);
    for (const F &numerator : vc<F>{{}, {3}, e})
    {
      const F quotient = numerator.div(f, n);
      assert(quotient.sz() == n);
      assert(product(quotient, f, n) == numerator.resized(n));
    }
    const F exponential = e.exp(n);
    assert(exponential.sz() == n);
    assert(exponential == e.exp_sparse(n));
    if (n > 0) assert(exponential.log(n) == e.resized(n));
    assert(f.log(n).sz() == n);
    assert(f.log(n).exp(n) == f.resized(n));
    assert(f.pow(2, n) == product(f, f, n));
    assert(f.pow(-1, n) == f.inv(n));
    const F shifted = f << 3;
    assert(shifted.pow(2, n) == product(shifted, shifted, n));
    assert(shifted.pow(LLONG_MAX, n) == F(n));
  }

  const int n = 12;
  const F sparse{2, 3};
  for (const F &base : {sparse, f * mint(2)})
  {
    const F lowest = base.pow(LLONG_MIN, n);
    const F highest = base.pow(LLONG_MAX, n);
    assert(lowest.sz() == n);
    assert(product(product(lowest, highest, n), base, n) == F{1}.resized(n));
    assert(base.pow_sparse(LLONG_MIN, n) == lowest);
  }
}

template <class mint>
void test_sqrt_lengths()
{
  using F = Fps<mint>;
  for (const F &base : vc<F>{{3}, {1, 2, 3}, dense<mint>(220)})
  {
    for (int shift : {0, 2, 15})
    {
      const F root = base << shift, square = product(root, root, 2 * root.sz() - 1);
      for (int n : {0, 1, 3, 16, root.sz() + 11})
      {
        const auto [ok, actual] = square.sqrt(n);
        const auto [sparse_ok, sparse_actual] = square.sqrt_sparse(n);
        const F expected = root.resized(n);
        assert(ok && actual.sz() == n);
        assert(actual == expected || actual == -expected);
        assert(sparse_ok && sparse_actual.sz() == n);
        assert(sparse_actual == expected || sparse_actual == -expected);
      }
    }
  }
  mint nonsquare = 2;
  while (nonsquare.pow((mint::mod() - 1) / 2) != -1) ++nonsquare;
  for (int n : {0, 1, 3, 8})
  {
    for (const F &f : vc<F>{{0, 1}, {0, 0, nonsquare}})
    {
      assert(f.sqrt(n) == make_pair(false, F{}));
      assert(f.sqrt_sparse(n) == make_pair(false, F{}));
    }
  }
}

template <class mint>
void test_compositional_lengths()
{
  using F = Fps<mint>;
  for (const F &f : vc<F>{{0, 2}, {0, 3, 1}, {0, 1, 2, 3, 4}})
  {
    for (int n : {0, 1, 2, 5, 16, 33})
    {
      const F g = compositional_inv(f, n);
      assert(g.sz() == n);
      const F identity = F{0, 1}.resized(n);
      assert(compose(f, g, n) == identity);
      assert(compose(g, f, n) == identity);
    }
  }
}

int main()
{
  test_lengths<modint998244353>();
  test_lengths<modint1000000007>();
  test_dense_lengths<modint998244353>(384);
  test_dense_lengths<modint1000000007>(3008);
  test_sqrt_lengths<modint998244353>();
  test_sqrt_lengths<modint1000000007>();
  test_compositional_lengths<modint998244353>();
  test_compositional_lengths<modint1000000007>();
  PRINT("Hello World");
}
