#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/berlekamp_massey_fast.hpp"
#include "math/fps/berlekamp_massey.hpp"

template <class mint>
using P = FormalPowerSeries<mint>;

template <class mint>
P<mint> naive_remainder(P<mint> a, P<mint> b)
{
  a.shrink(), b.shrink();
  assert(!b.empty());
  while (a.sz() >= b.sz())
  {
    int k = a.sz() - b.sz();
    mint z = a.back() / b.back();
    for (int j = 0; j < b.sz(); ++j) a[k + j] -= z * b[j];
    a.shrink();
  }
  return a;
}

template <class mint>
bool valid(const vc<mint> &a, const vc<mint> &c)
{
  if (c.empty() || c[0] != mint(-1)) return false;
  auto prod = convolution(a, c);
  for (int i = (int)c.size() - 1; i < (int)a.size(); ++i)
    if (prod[i] != 0) return false;
  return true;
}

template <class mint>
void check_bm(const vc<mint> &a)
{
  auto c = berlekamp_massey_fast(a);
  auto ref = berlekamp_massey<FieldAddSubMulDiv<mint>>(a);
  assert(c.size() == ref.size());
  assert(valid(a, c));
  if (2 * (c.size() - 1) <= a.size()) assert(c == ref);
}

void exhaustive_binary()
{
  using mint = static_modint32<2>;
  for (int n = 0; n <= 10; ++n)
    for (int mask = 0; mask < (1 << n); ++mask)
    {
      vc<mint> a(n);
      for (int i = 0; i < n; ++i) a[i] = mask >> i & 1;
      auto c = berlekamp_massey_fast(a);
      int best = 0;
      for (;; ++best)
      {
        bool found = false;
        for (int bits = 0; bits < (1 << best) && !found; ++bits)
        {
          bool ok = true;
          for (int i = best; i < n && ok; ++i)
          {
            int sum = mask >> i & 1;
            for (int j = 1; j <= best; ++j)
              sum ^= (bits >> (j - 1) & 1) & (mask >> (i - j) & 1);
            if (sum) ok = false;
          }
          found |= ok;
        }
        if (found) break;
      }
      assert((int)c.size() == best + 1);
      assert(valid(a, c));
    }
}

template <class mint>
void random_checks(int max_n, int trials)
{
  mt19937 rng(20260908);
  auto random_poly = [&](int n)
  {
    P<mint> a(n);
    for (auto &x : a) x = rng() % mint::mod();
    a.shrink();
    return a;
  };
  for (int it = 0; it < trials; ++it)
  {
    int n = rng() % (max_n + 1), k = rng() % (max_n + 1);
    auto a = random_poly(n), b = random_poly(k);
    if (it % 3 == 0)
    {
      auto h = random_poly(1 + rng() % 20);
      a *= h, b *= h;
      a.shrink(), b.shrink();
    }
    auto [g, s, t] = polynomial_extgcd(a, b);
    auto lhs = s * a + t * b;
    lhs.shrink();
    assert(lhs == g);
    auto u = a, v = b;
    while (!v.empty())
    {
      auto rem = naive_remainder(u, v);
      u = std::move(v), v = std::move(rem);
    }
    if (!u.empty()) u /= u.back();
    assert(g == u);
    if (b.sz() > 1)
    {
      auto [ok, iv] = polynomial_inv_mod(a, b);
      assert(ok == (g.sz() == 1));
      if (ok)
      {
        assert(iv.sz() < b.sz());
        assert(naive_remainder(a * iv, b) == P<mint>{1});
      }
    }
    if (a.sz() > b.sz())
    {
      using H = internal::PolynomialHalfGCD<mint>;
      auto m = H::half(a, b);
      auto [f, h] = m.apply(a, b);
      auto det = m.a * m.d - m.b * m.c;
      det.shrink();
      assert(det.sz() == 1 && (det[0] == 1 || det[0] == mint(-1)));
      int mid = a.sz() / 2;
      assert(f.sz() > mid && h.sz() <= mid);
      // Compare the actual remainder pair with ordinary Euclid.
      u = a, v = b;
      while (v.sz() > mid)
      {
        auto rem = naive_remainder(u, v);
        u = std::move(v), v = std::move(rem);
      }
      assert(f == u && h == v);
    }
    a.resize(n);
    check_bm<mint>(a);
    if (it % 5 == 0)
    {
      fill(a.begin(), a.begin() + n / 2, mint(0));
      check_bm<mint>(a);
    }
  }
}

int main()
{
  exhaustive_binary();
  random_checks<static_modint32<2>>(150, 300);
  random_checks<static_modint32<3>>(150, 300);
  random_checks<modint998244353>(300, 800);
  random_checks<modint998244353>(1500, 30);
  random_checks<modint1000000007>(140, 30);
  using mint = modint998244353;
  for (int n : {0, 1, 2, 3, 63, 64, 65, 127, 128, 129, 1023, 1024, 1025, 4096})
  {
    vc<mint> a(n, 0);
    check_bm(a);
    for (int k : {0, n / 2, n - 1}) if (0 <= k && k < n)
    {
      a.assign(n, 0), a[k] = 1;
      check_bm(a);
    }
    a.assign(n, 1);
    check_bm(a);
  }
  for (int d : {1, 2, 8, 63, 64, 65, 128, 513})
  {
    int n = 4097;
    P<mint> q(d + 1), p(d);
    mt19937 rng(1000 + d);
    for (auto &x : q) x = rng();
    for (auto &x : p) x = rng();
    q[0] = 1;
    auto a = (p * q.inv(n)).resized(n);
    check_bm<mint>(a);
    // Terminal corruption forces a large, non-unique minimal recurrence.
    a.back() += 1;
    check_bm<mint>(a);
  }
  auto [ok, inv] = polynomial_inv_mod(P<mint>{0, 1}, P<mint>{1, 0, 1});
  assert(ok && inv == (P<mint>{0, -1}));
  assert(polynomial_inv_mod(P<mint>{}, P<mint>{7}).first);
  assert(polynomial_inv_mod(P<mint>{2, 3}, P<mint>{7}).second.empty());
  assert(!polynomial_inv_mod(P<mint>{0, 1}, P<mint>{0, 0, 1}).first);
  cout << "Hello World\n";
}
