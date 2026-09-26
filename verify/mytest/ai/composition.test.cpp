#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/composition.hpp"
#include "math/fps/compositional_inverse.hpp"

template <class mint>
using Fps = FormalPowerSeries<mint>;

template <class mint>
Fps<mint> naive_composition(const Fps<mint> &f, const Fps<mint> &g, int n)
{
  Fps<mint> h(n);
  repi(k, f.sz() - 1, -1, -1)
  {
    Fps<mint> next(n);
    repi(i, n) repi(j, min(n - i, g.sz())) next[i + j] += h[i] * g[j];
    if (n > 0) next[0] += f[k];
    h = move(next);
  }
  return h;
}

template <class mint>
void test_small()
{
  using F = Fps<mint>;
  mt19937 rng(912376);
  for (int n : {0, 1, 2, 3, 4, 5, 7, 8, 9, 15, 16, 17, 31, 32, 33, 63, 64, 65})
  {
    for (const F &f : vc<F>{{}, {0}, {2}, {1, 2, 3}})
      for (const F &g : vc<F>{{}, {0}, {0, 1}, {0, 0, 3}, {0, 2, 3}})
        assert(composition(f, g, n) == naive_composition(f, g, n));

    repi(t, 12)
    {
      F f(rng() % (n + 8)), g(rng() % (n + 8));
      fem(a : f) a = rng();
      fem(a : g) a = rng();
      if (!g.empty()) g[0] = 0;
      const F f_copy = f, g_copy = g;
      assert(composition(f, g, n) == naive_composition(f, g, n));
      assert(f == f_copy && g == g_copy);
    }
  }
}

template <class mint>
void test_transpose()
{
  using F = Fps<mint>;
  mt19937 rng(43165);
  // 998244353 の NTT と 1000000007 の CRT の両方を通す。
  for (int n : {1, 2, 3, 127, 128, 129, 511, 512, 513, 1024})
  {
    F g(n), w(n);
    fem(a : g) a = rng();
    fem(a : w) a = rng();
    g[0] = 0;
    for (int m : {1, n / 2 + 1, n, n + 7})
    {
      F f(m);
      fem(a : f) a = rng();
      const F h = composition(f, g, n);
      const F p = power_projection(n - 1, m, g, w.rev());
      mint lhs = 0, rhs = 0;
      repi(i, n) lhs += w[i] * h[i];
      repi(i, m) rhs += f[i] * p[i];
      assert(lhs == rhs);
    }
  }
}

template <class mint>
void test_identities()
{
  using F = Fps<mint>;
  mt19937 rng(831551);
  for (int n : {2, 17, 128, 257, 1024})
  {
    F f(n), g(n);
    fem(a : f) a = rng();
    fem(a : g) a = rng();
    g[0] = 0, g[1] = 1;
    assert((composition(f, F{0, 1}, n) == f));
    assert((composition(F{0, 1}, g, n) == g));
    assert(composition(f, F{}, n) == F{f[0]}.resized(n));
    F square = (g * g).resized(n);
    assert((composition(F{0, 0, 1}, g, n) == square));
    const F inverse = compositional_inv(g, n), identity = F{0, 1}.resized(n);
    assert(composition(g, inverse, n) == identity);
    assert(composition(inverse, g, n) == identity);
  }
}

int main()
{
  test_small<modint998244353>();
  test_small<modint1000000007>();
  test_small<static_modint32<17>>();
  test_small<static_modint32<2>>();
  using dynamic_mint = dynamic_modint32<912>;
  dynamic_mint::set_mod(1000000007);
  test_small<dynamic_mint>();
  test_transpose<modint998244353>();
  test_transpose<modint1000000007>();
  test_identities<modint998244353>();
  test_identities<modint1000000007>();
  cout << "Hello World" << endl;
}
