#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../modint/binomial.hpp"
#include "../convolution/convolution.hpp"
#include "../modint/sqrt_mod.hpp"
#include "internal_ntt.hpp"

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
  FormalPowerSeries(vc<mint> &&f) : vc<mint>(std::move(f)) {}

  int sz() const { return size(); }
  void shrink()
  {
    while (!empty() && back() == 0)
      pop_back();
  }
  mint get(int i) const { return 0 <= i && i < sz() ? (*this)[i] : 0; }
  F resized(int n) const
  {
    assert(n >= 0);
    F res(n);
    copy_n(begin(), min(sz(), n), res.begin());
    return res;
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
  F operator*(const mint &k) const { F res(*this); res *= k; return res; }
  friend F operator*(const mint &k, const F &f) { return f * k; }
  F &operator/=(const mint &k)
  {
    *this *= k.inv();
    return *this;
  }
  F operator/(const mint &k) const { F res(*this); res /= k; return res; }
  F &operator+=(const F &g)
  {
    const int n = size(), m = g.size();
    resize(max(n, m));
    repi(i, m)(*this)[i] += g[i];
    return *this;
  }
  F operator+(const F &g) const { F res(*this); res += g; return res; }
  F &operator-=(const F &g)
  {
    const int n = size(), m = g.size();
    resize(max(n, m));
    repi(i, m)(*this)[i] -= g[i];
    return *this;
  }
  F operator-(const F &g) const { F res(*this); res -= g; return res; }
  F &operator*=(const F &g) { return *this = *this * g; }
  F operator*(const F &g) const { return convolution(*this, g); }

  F div_sparse_destructive(const F &g, int n)
  {
    assert(n >= 0);
    assert(g.get(0) != 0);
    mint iv = g.front().inv();
    vc<pair<int, mint>> gnz;
    repi(j, min(n, g.sz())) if (g[j] != 0) gnz.eb(j, g[j]);
    resize(n);
    if constexpr (internal::ordinary_mod32<mint>::value)
    {
      vc<pair<int, mint>> terms;
      for (auto [j, b] : gnz) if (j > 0 && j < n) terms.eb(j, b);
      const ull mod = mint::mod();
      const int block = mod <= (1U << 30) ? 16 : 4;
      repi(i, n)
      {
        ull sum = 0;
        int used = 0;
        for (const auto &[j, b] : terms)
        {
          if (j > i) break;
          sum += ull((*this)[i - j].val()) * b.val();
          if (++used == block) sum %= mod, used = 0;
        }
        (*this)[i] = ((*this)[i] - mint::raw(sum % mod)) * iv;
      }
      return *this;
    }
    repi(i, n)
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
    return *this;
  }
  F div_sparse(const F &g, int n) const { return F(*this).div_sparse_destructive(g, n); }

  // 定数項が非零
  F inv(int n) const
  {
    assert(n >= 0);
    assert(get(0) != 0);
    if (n == 0)
      return {};
    mint iv = get(0).inv();
    if (auto tail = internal::fps_tail_power(*this, n, iv, -iv * iv, iv * iv * iv)) return std::move(*tail);
    if (internal::fps_use_sparse(*this, n, internal::FPSSparseOperation::inv))
      return F{1}.div_sparse(*this, n);
    const int N = bit_ceil(n);
    if (n >= 512 && ll(n) * 4 <= ll(N) * 3 && ntt_ok<mint>(N))
      return internal::fps_inv_block(*this, n, 8);
    return internal::fps_inv_newton(*this, n);
  }
  F div(const F &g, int n) const
  {
    assert(n >= 0);
    assert(g.get(0) != 0);
    if (n == 0)
      return {};
    if (internal::fps_use_sparse(g, n, internal::FPSSparseOperation::div))
      return div_sparse(g, n);
    return (resized(n) * g.inv(n)).resized(n);
  }

  F div_poly(const F &g) const
  {
    assert(!g.empty() && g.back() != 0);
    const int k = sz() - g.sz() + 1;
    if (k <= 0)
      return {};
    F a(k), b(min(k, g.sz()));
    copy_n(this->rbegin(), k, a.begin());
    copy_n(g.rbegin(), b.sz(), b.begin());
    return (a * b.inv(k)).resized(k).rev();
  }
  pair<F, F> divmod(const F &g) const
  {
    F q = div_poly(g);
    const int l = sz() - q.sz();
    F r = resized(l) - (q.resized(min(l, q.sz())) * g.resized(min(l, g.sz()))).resized(l);
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
  F log(int n) const
  {
    assert(n >= 0);
    assert(get(0) == 1);
    if (n == 0)
      return {};
    F f = resized(n);
    return f.diff().div(f, n - 1).integ();
  }

  // 微分方程式 a(x)f'(x) + b(x)f(x) = 0, [x^0]f(x) = 1 を満たす f を d 項まで求める
  // 制約: [x^0]a(x) = 1、法は素数、0 <= d <= mod
  // 計算量: O( d * (a, b の非零の個数) )
  static F diff_eq(const F &a, const F &b, int d)
  {
    assert(a.get(0) == 1);
    assert(d >= 0 && ll(d) <= mint::mod());
    if (d == 0)
      return {};
    F f(d);
    f[0] = 1;
    auto terms = [d](const F &f)
    {
      vc<pair<int, mint>> res;
      repi(i, min(d, f.sz())) if (f[i] != 0) res.eb(i, f[i]);
      return res;
    };
    auto anz = terms(a), bnz = terms(b);
    if constexpr (internal::ordinary_mod32<mint>::value)
    {
      vc<mint> df(d);
      const ull mod = mint::mod();
      const int block = mod <= (1U << 30) ? 16 : 4;
      Binomial<mint>::reserve(d - 1);
      repi(t, 1, d)
      {
        ull sum = 0;
        int used = 0;
        for (const auto &[i, ai] : anz)
        {
          if (i == 0) continue;
          if (i > t) break;
          sum += ull(ai.val()) * df[t - i].val();
          if (++used == block) sum %= mod, used = 0;
        }
        for (const auto &[j, bj] : bnz)
        {
          if (j >= t) break;
          sum += ull(bj.val()) * f[t - 1 - j].val();
          if (++used == block) sum %= mod, used = 0;
        }
        f[t] = -mint::raw(sum % mod) * Binomial<mint>::inv_[t];
        df[t] = mint(t) * f[t];
      }
      return f;
    }
    repi(k, d - 1)
    {
      fec([i, ai] : anz)
      {
        if (i == 0) continue;
        if (i > k + 1) break;
        f[k + 1] -= ai * (k - i + 1) * f[k - i + 1];
      }
      fec([j, bj] : bnz)
      {
        if (j > k) break;
        f[k + 1] -= bj * f[k - j];
      }
      f[k + 1] *= Binomial<mint>::inv(k + 1);
    }
    return f;
  }
  F exp_sparse(int n) const
  {
    assert(n >= 0);
    assert(get(0) == 0);
    return diff_eq(F{1}, -resized(min(n, sz())).diff(), n);
  }
  // k < 0 のときは定数項が非零
  F pow_sparse(ll k, int n) const
  {
    assert(n >= 0);
    assert(k >= 0 || get(0) != 0);
    if (n == 0)
      return {};
    if (k == 0) return F{1}.resized(n);
    if (k == 1) return resized(n);
    if (k == -1) return F{1}.div_sparse(*this, n);
    auto [exi, d0, a0] = nz_front();
    if (!exi)
    {
      assert(k >= 0 && "k < 0 but [x^0]f(x) == 0");
      F res(n);
      if (k == 0)
        res[0] = 1;
      return res;
    }
    const int shift = k >= 0 ? mul_limited(d0, k, n) : 0;
    if (shift >= n) return F(n);
    const int need = n - shift;
    mint ia0 = a0.inv();
    F f(min(need, sz() - d0));
    copy_n(begin() + d0, f.sz(), f.begin());
    f *= ia0;
    F g = diff_eq(f, -mint(k) * f.diff(), need);
    mint factor = k >= 0 ? a0.pow(k) : ia0.pow(ull(-(k + 1)) + 1);
    return (g * factor) << shift;
  }

  // (存在するか, 平方根のひとつ)
  pair<bool, F> sqrt_sparse(int n) const
  {
    assert(n >= 0);
    auto [exi, d0, a0] = nz_front();
    if (!exi)
      return {true, F(n)};
    if (d0 % 2 != 0)
      return {false, {}};
    auto [ok, r] = sqrt_mod(a0);
    if (!ok)
      return {false, {}};
    if (d0 / 2 >= n)
      return {true, F(n)};
    mint i2 = Binomial<mint>::inv(2);
    const int need = n - d0 / 2;
    F f(min(need, sz() - d0));
    copy_n(begin() + d0, f.sz(), f.begin());
    f /= a0;
    F g = diff_eq(f, -i2 * f.diff(), need);
    return {true, (g * r) << (d0 / 2)};
  }

  // 定数項が 0
  F exp(int n) const
  {
    assert(n >= 0);
    assert(get(0) == 0);
    if (n == 0)
      return {};
    int first = 1;
    while (first < min(n, sz()) && (*this)[first] == 0) ++first;
    if (first >= min(n, sz())) return F{1}.resized(n);
    if (ll(first) * 2 >= n)
    {
      F res = resized(n);
      res[0] = 1;
      return res;
    }
    if (internal::fps_use_sparse(*this, n, internal::FPSSparseOperation::exp)) return exp_sparse(n);
    if (ntt_ok<mint>(bit_ceil(n)))
    {
      if (n < 512) return internal::fps_exp_bs(*this, n);
      return internal::fps_exp_block(*this, n, n <= 4096 ? 4 : 16);
    }
    // Maintain f and its reciprocal across Bostan-Schost updates.
    Binomial<mint>::reserve(n - 1);
    F f{1, get(1)}, g{1};
    for (int m = 2; m < n; m *= 2)
    {
      F fg2 = f * (g * g);
      g.resize(m);
      repi(i, m / 2, m) g[i] = -fg2.get(i);
      F q(m - 1);
      repi(i, 1, min(m, sz())) q[i - 1] = mint(i) * (*this)[i];
      F r = (f * q).circular_mod(m), s(m);
      repi(i, m)
        s[(i + 1) % m] = (i + 1 < m ? mint(i + 1) * f[i + 1] : mint(0)) - r[i];
      F t = (g * s).resized(m), u(min(m, n - m));
      repi(i, u.sz()) u[i] = get(m + i) - t[i] * Binomial<mint>::inv_[m + i];
      F v = (f * u).resized(u.sz());
      f.insert(f.end(), v.begin(), v.end());
    }
    return f.resized(n);
  }
  // k < 0 のときは定数項が非零
  F pow(ll k, int n) const
  {
    assert(n >= 0);
    assert(k >= 0 || get(0) != 0);
    if (n == 0)
      return {};
    if (k == 0) return F{1}.resized(n);
    if (k == 1) return resized(n);
    if (k == -1) return inv(n);
    auto [exists, d0, a0] = nz_front();
    if (!exists) return F(n);
    const int shift = k >= 0 ? mul_limited(d0, k, n) : 0;
    if (shift >= n) return F(n);
    const int need = n - shift;
    if (2 <= k && k <= 8)
    {
      F base(need);
      copy_n(begin() + d0, min(need, sz() - d0), base.begin());
      F res = base;
      for (int bit = bit_width(unsigned(k)) - 2; bit >= 0; --bit)
      {
        res = (res * res).resized(need);
        if (k >> bit & 1) res = (res * base).resized(need);
      }
      return res << shift;
    }
    mint iv = a0.inv();
    mint factor = k >= 0 ? a0.pow(k) : iv.pow(ull(-(k + 1)) + 1);
    F f(need);
    copy_n(begin() + d0, min(need, sz() - d0), f.begin());
    if (need <= 2 || mint::mod() != 2)
      if (auto tail = internal::fps_tail_power(f, need, factor, mint(k) * factor * iv,
          need <= 2 ? mint(0) : mint(k) * (mint(k) - 1) * Binomial<mint>::inv(2) * factor * iv * iv))
        return (std::move(*tail) << shift);
    if (internal::fps_use_sparse(*this, need, internal::FPSSparseOperation::pow, d0))
      return pow_sparse(k, n);
    f *= iv;
    F res = (f.log(need) * mint(k)).exp(need);
    return (res * factor) << shift;
  }

  pair<bool, F> sqrt(int n) const
  {
    assert(n >= 0);
    auto [exi, d0, a0] = nz_front();
    if (!exi)
      return {true, F(n)};
    if (d0 % 2 != 0)
      return {false, {}};
    auto [ok, r] = sqrt_mod(a0);
    if (!ok)
      return {false, {}};
    if (d0 / 2 >= n)
      return {true, F(n)};
    const int need = n - d0 / 2;
    F shifted(need);
    copy_n(begin() + d0, min(need, sz() - d0), shifted.begin());
    mint iv = a0.inv(), half = Binomial<mint>::inv(2);
    if (auto tail = internal::fps_tail_power(shifted, need, r, r * iv * half, -r * iv * iv * half * half * half))
      return {true, std::move(*tail) << (d0 / 2)};
    if (internal::fps_use_sparse(*this, need, internal::FPSSparseOperation::sqrt, d0))
      return sqrt_sparse(n);
    F f = std::move(shifted); f *= iv;
    F g = need >= 512 && ntt_ok<mint>(bit_ceil(need))
      ? internal::fps_sqrt_block(f, need, 16)
      : internal::fps_sqrt_newton(f, need);
    return {true, (g * r) << (d0 / 2)};
  }

  F pow_mod(ll k, const F &g) const
  {
    assert(k >= 0);
    if (k == 0)
      return F{1} % g;
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

  // (1 + cx^d) をかける
  F mul_bin_destructive(int d, mint c)
  {
    resize(sz() + d);
    rep(i, sz() - 1 - d, -1, -1)(*this)[i + d] += (*this)[i] * c;
    return *this;
  }
  // (1 + cx^d) をかけたもの
  F mul_bin(int d, mint c) const { return F(*this).mul_bin_destructive(d, c); }
  // (1 + cx^k) でわり、長さ n まで求める
  F div_bin_destructive(int k, mint c, int n)
  {
    assert(k >= 0 && n >= 0);
    assert(k > 0 || 1 + c != 0);
    resize(n);
    if (k == 0)
      return *this /= 1 + c;
    repi(i, max(0, n - k))(*this)[i + k] -= (*this)[i] * c;
    return *this;
  }
  F div_bin(int k, mint c, int n) const { return F(*this).div_bin_destructive(k, c, n); }
};
