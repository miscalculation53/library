#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../modint/binomial.hpp"
#include "../convolution/convolution.hpp"
#include "../modint/sqrt_mod.hpp"

/**
 * @brief 形式的冪級数
 * @docs docs/math/fps/fps.md
 */

template <class mint>
struct FormalPowerSeries : vc<mint>
{
  using F = FormalPowerSeries;
  using vc<mint>::vc;
  using vc<mint>::operator=;
  using vc<mint>::size;
  using vc<mint>::empty;
  using vc<mint>::back;
  using vc<mint>::pop_back;
  using vc<mint>::begin;
  using vc<mint>::resize;
  using vc<mint>::front;

  FormalPowerSeries(const vc<mint> &f) : vc<mint>(f) {}

  int sz() const { return size(); }
  void shrink()
  {
    while (!empty() && back() == 0)
      pop_back();
  }
  mint get(int i) const { return 0 <= i && i < sz() ? (*this)[i] : 0; }
  F pre(int len) const
  {
    assert(len >= 0);
    return F(begin(), begin() + min(sz(), len));
  }
  F rev(int d = -1) const
  {
    F res(*this);
    if (d >= 0)
      res.resize(d);
    reverse(ALL(res));
    return res;
  }
  int cnt_nz() const { return count_if(ALL(*this), LMD(x, x != 0)); }
  tuple<bool, int, mint> nz_front() const
  {
    repi(i, sz()) if ((*this)[i] != 0) return {true, i, (*this)[i]};
    return {false, -1, 0};
  }
  vc<pair<int, mint>> nz() const
  {
    vc<pair<int, mint>> res;
    repi(i, sz()) if ((*this)[i] != 0) res.eb(i, (*this)[i]);
    return res;
  }

  mint eval(const mint &x) const
  {
    mint res = 0;
    repi(i, sz() - 1, -1, -1) res = res * x + (*this)[i];
    return res;
  }

  F operator-() const
  {
    F res(*this);
    fem(a : res) a = -a;
    return res;
  }
  F &operator*=(const mint &k)
  {
    fem(a : *this) a *= k;
    return *this;
  }
  F operator*(const mint &k) const { return F(*this) *= k; }
  friend F operator*(const mint &k, const F &f) { return f * k; }
  F &operator/=(const mint &k)
  {
    *this *= k.inv();
    return *this;
  }
  F operator/(const mint &k) const { return F(*this) /= k; }
  F &operator+=(const F &g)
  {
    const int n = size(), m = g.size();
    resize(max(n, m));
    repi(i, m)(*this)[i] += g[i];
    return *this;
  }
  F operator+(const F &g) const { return F(*this) += g; }
  F &operator-=(const F &g)
  {
    const int n = size(), m = g.size();
    resize(max(n, m));
    repi(i, m)(*this)[i] -= g[i];
    return *this;
  }
  F operator-(const F &g) const { return F(*this) -= g; }
  F &operator*=(const F &g) { return *this = *this * g; }
  F operator*(const F &g) const { return convolution(*this, g); }

  F div_sparse_destructive(const F &g, int d = -1)
  {
    assert(g.get(0) != 0);
    if (d < 0)
      d = sz();
    mint iv = g.front().inv();
    auto gnz = g.nz();
    resize(d);
    repi(i, d)
    {
      fec([j, b] : gnz)
      {
        if (j == 0)
          continue;
        if (j > i)
          break;
        (*this)[i] -= (*this)[i - j] * b;
      }
      (*this)[i] *= iv;
    }
    return pre(d);
  }
  F div_sparse(const F &g, int d = -1) const { return F(*this).div_sparse_destructive(g, d); }

  // 定数項が非零
  F inv(int d = -1) const
  {
    assert(get(0) != 0);
    if (d < 0)
      d = sz();
    if (cnt_nz() <= 200)
      return F{1}.div_sparse(*this, d);
    F f, g2, g{front().inv()};
    for (int m = 1; m < d; m *= 2)
    {
      if (ntt_ok<mint>(2 * m))
      {
        f = pre(2 * m), g2 = F(g);
        f.resize(2 * m), ntt(f);
        g2.resize(2 * m), ntt(g2);
        repi(i, 2 * m) f[i] *= g2[i];
        intt(f);
        f >>= m;
        f.resize(2 * m), ntt(f);
        repi(i, 2 * m) f[i] *= g2[i];
        intt(f);
        mint iz = mint(2 * m).inv();
        iz *= -iz;
        repi(i, m) f[i] *= iz;
        g.insert(g.end(), f.begin(), f.begin() + m);
      }
      else
        g = (g * mint(2) - g * g * pre(2 * m)).pre(2 * m);
    }
    return g.pre(d);
  }
  F &operator/=(const F &g)
  {
    if (cnt_nz() <= 200)
    {
      div_sparse_destructive(g);
      return *this;
    }
    *this *= g.inv();
    return *this;
  }
  F operator/(const F &g) const { return F(*this) /= g; }

  F div_poly(const F &g) const
  {
    const int k = sz() - g.sz() + 1;
    if (k <= 0)
      return {};
    return (rev().pre(k) * g.rev().inv(k)).pre(k).rev();
  }
  pair<F, F> divmod(const F &g) const
  {
    F q = div_poly(g);
    const int l = sz() - q.sz();
    F r = pre(l) - (q.pre(l) * g.pre(l)).pre(l);
    r.shrink();
    return {q, r};
  }
  F operator%(const F &g) const { return divmod(g).second; }
  F &operator%=(const F &g) { return *this = *this % g; }

  // mod (x^n - 1)
  F circular_mod(int n) const
  {
    F res(n);
    repi(i, sz()) res[i % n] += (*this)[i];
    return res;
  }

  F operator<<(int k) const
  {
    F res(sz() + k);
    repi(i, sz()) res[i + k] = (*this)[i];
    return res;
  }
  F operator>>(int k) const
  {
    F res(max(0, sz() - k));
    repi(i, sz() - k) res[i] = (*this)[i + k];
    return res;
  }
  F &operator<<=(int k) { return *this = *this << k; }
  F &operator>>=(int k) { return *this = *this >> k; }

  // 微分 sum[i=1..n] i*a[i] x^{i-1}
  F diff() const
  {
    F res(max(0, sz() - 1));
    repi(i, 1, size()) res[i - 1] = (*this)[i] * i;
    return res;
  }
  // 積分 sum[i=0..n] a[i]/(i+1) * x^{i+1}
  F integ() const
  {
    F res(sz() + 1);
    repi(i, size()) res[i + 1] = (*this)[i] * Binomial<mint>::inv(i + 1);
    return res;
  }
  // 定数項が 1
  F log(int d = -1) const
  {
    assert(get(0) == 1);
    if (d < 0)
      d = sz();
    F f = pre(d);
    return (f.diff() / f).pre(d - 1).integ();
  }

  // 微分方程式 a(x)f'(x) + b(x)f(x) = 0, [x^0]f(x) = 1 を満たす f を d 項まで求める
  // 制約: [x^0]a(x) = 1
  // 計算量: O( d * (a, b の非零の個数) )
  static F diff_eq(const F &a, const F &b, int d)
  {
    assert(a.get(0) == 1);
    assert(d >= 0);
    if (d == 0)
      return {};
    F f(d);
    f[0] = 1;
    auto anz = a.nz(), bnz = b.nz();
    repi(k, d - 1)
    {
      fec([i, ai] : anz)
      {
        if (0 <= k - i + 1)
          f[k + 1] -= ai * (k - i + 1) * f[k - i + 1];
      }
      fec([j, bj] : bnz)
      {
        if (0 <= k - j && k - j < k + 1)
          f[k + 1] -= bj * f[k - j];
      }
      f[k + 1] *= Binomial<mint>::inv(k + 1);
    }
    return f;
  }
  F exp_sparse(int d = -1) const
  {
    assert(get(0) == 0);
    if (d < 0)
      d = sz();
    return diff_eq(F{1}, -diff(), d);
  }
  // k < 0 のときは定数項が非零
  F pow_sparse(ll k, int d = -1) const
  {
    if (d < 0)
      d = sz();
    auto [exi, d0, a0] = nz_front();
    if (!exi)
    {
      assert(k >= 0 && "k < 0 but [x^0]f(x) == 0");
      F res(d);
      if (k == 0 && d > 0)
        res[0] = 1;
      return res;
    }
    mint ia0 = a0.inv();
    F f = ((*this) >> d0) * ia0;
    if (k >= 0)
    {
      F g = diff_eq(f, -k * f.diff(), d - mul_limited(d0, k, d));
      F h = (g * a0.pow(k)) << mul_limited(d0, k, d);
      return h.pre(d);
    }
    else
    {
      assert(d0 == 0 && "k < 0 but [x^0]f(x) == 0");
      F g = diff_eq(f, -k * f.diff(), d);
      F h = (g * ia0.pow(-k));
      return h.pre(d);
    }
  }
  // (存在するか, 平方根のひとつ)
  pair<bool, F> sqrt_sparse(int d = -1) const
  {
    if (d < 0)
      d = sz();
    auto [exi, d0, a0] = nz_front();
    if (!exi)
      return {true, F(d)};
    if (d0 % 2 != 0)
      return {false, {}};
    if (d0 >= d)
      return {true, F(d)};
    auto [ok, r] = sqrt_mod(a0);
    if (!ok)
      return {false, {}};
    mint i2 = Binomial<mint>::inv(2);
    F f = ((*this) >> d0) / a0;
    F g = diff_eq(f, -i2 * f.diff(), d - d0 / 2);
    return {true, ((g * r) << (d0 / 2)).pre(d)};
  }

  // 定数項が 0
  F exp(int d = -1) const
  {
    assert(get(0) == 0);
    if (d < 0)
      d = sz();
    if (ntt_ok<mint>(2 * d))
    {
      if (cnt_nz() <= 320)
        return exp_sparse(d);
      // https://arxiv.org/pdf/1301.5804.pdf
      F f{1}, g{1};
      F f2, g2, f3, q, s, h, u;
      g2 = {0};
      for (int m = 1; m < d; m *= 2)
      {
        mint im = mint(m).inv(), i2m = mint(2 * m).inv();
        f2 = f, f2.resize(2 * m), ntt(f2);

        // a
        f3 = f, ntt(f3);
        repi(i, m) f3.at(i) *= g2.at(i);
        intt(f3);
        f3 >>= m / 2;
        f3.resize(m), ntt(f3);
        repi(i, m) f3.at(i) *= g2.at(i);
        intt(f3);
        repi(i, m / 2) f3.at(i) *= -im * im;
        g.insert(g.end(), f3.begin(), f3.begin() + m / 2);
        g2 = g, g2.resize(2 * m), ntt(g2);

        // b, c
        q = diff(), q.resize(2 * m), fill(q.begin() + m - 1, q.end(), 0);
        ntt(q);
        repi(i, 2 * m) q.at(i) *= f2.at(i);
        intt(q);
        q = q.circular_mod(m);
        repi(i, m) q.at(i) *= i2m;

        // d, e
        q.resize(m + 1);
        s = ((f.diff() - q) << 1).circular_mod(m);
        s.resize(2 * m), ntt(s);
        repi(i, 2 * m) s.at(i) *= g2.at(i);
        intt(s);
        repi(i, m) s.at(i) *= i2m;
        s.resize(m);

        // f, g
        h = *this, h.resize(2 * m), s.resize(2 * m);
        u = (h - (s << (m - 1)).integ()) >> m;
        ntt(u);
        repi(i, 2 * m) u.at(i) *= f2.at(i);
        intt(u);
        repi(i, m) u.at(i) *= i2m;
        u.resize(m);

        // h
        f.insert(f.end(), u.begin(), u.end());
      }
      return f.pre(d);
    }
    else
    {
      if (cnt_nz() <= 3000)
        return exp_sparse(d);
      F f{1};
      for (int m = 1; m < d; m *= 2)
      {
        f = (f * (pre(2 * m) + F{1} - f.log(2 * m))).pre(2 * m);
      }
      return f.pre(d);
    }
  }
  // k < 0 のときは定数項が非零
  F pow(ll k, int d = -1) const
  {
    if (ntt_ok<mint>(2 * d))
    {
      if (cnt_nz() <= 100)
        return pow_sparse(k, d);
    }
    else
    {
      if (cnt_nz() <= 1300)
        return pow_sparse(k, d);
    }
    if (d < 0)
      d = sz();
    if (k == 0)
    {
      F res(d);
      res[0] = 1;
      return res;
    }
    if (k < 0)
    {
      assert(get(0) != 0);
      mint iv = get(0).inv();
      F res = ((*this * iv).log(d) * mint(k)).exp(d);
      res = (res * iv.pow(-k)).pre(d);
      if (res.sz() < d)
        res.resize(d);
      return res;
    }
    repi(i, sz())
    {
      if ((*this)[i] != 0)
      {
        mint iv = (*this)[i].inv();
        F res = (((*this * iv) >> i).log(d) * mint(k)).exp(d);
        res *= (*this)[i].pow(k);
        res = (res << (i * k)).pre(d);
        if (res.sz() < d)
          res.resize(d);
        return res;
      }
      if (mul_limited(i + 1, k, d) >= d)
        return F(d);
    }
    return F(d);
  }
  pair<bool, F> sqrt(int d = -1) const
  {
    if (cnt_nz() <= 200)
      return sqrt_sparse(d);
    if (d < 0)
      d = sz();
    auto [exi, d0, a0] = nz_front();
    if (!exi)
      return {true, F(d)};
    if (d0 % 2 != 0)
      return {false, {}};
    if (d0 >= d)
      return {true, F(d)};
    auto [ok, r] = sqrt_mod(a0);
    if (!ok)
      return {false, {}};
    mint i2 = Binomial<mint>::inv(2);
    F f = ((*this) >> d0) / a0, g{1};
    for (int m = 1; m < d; m *= 2)
      g = (g + f.pre(2 * m) * g.inv(2 * m)) * i2;
    return {true, ((g * r) << (d0 / 2)).pre(d)};
  }

  F pow_mod(ll k, const F &g) const
  {
    assert(k >= 0);
    if (k == 0)
      return (*this) % g;
    if (k & 1)
      return (*this) * pow_mod(k - 1, g) % g;
    F h = pow_mod(k / 2, g);
    return h * h % g;
  }

  // 各係数 a_n を n! で割ったもの
  F egf() const
  {
    F res(*this);
    repi(i, sz()) res[i] *= Binomial<mint>::finv(i);
    return res;
  }
  // 各係数 a_n に n! をかけたもの
  F ogf() const
  {
    F res(*this);
    repi(i, sz()) res[i] *= Binomial<mint>::fac(i);
    return res;
  }
};
