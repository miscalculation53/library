#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 素べき構造体
 * @docs docs/math/prime/prime_power.md
 */

template <class P>
struct PrimePower
{
  P p;
  int e;
  P pe;

  PrimePower() : p(-1), e(-1), pe(-1) {}
  PrimePower(P p, int e = 1) : p(p), e(e), pe(ipow(p, e)) {}
  PrimePower(P p, int e, P pe) : p(p), e(e), pe(pe) {}
  template <class P2>
  PrimePower(const PrimePower<P2> &pp) : p(pp.p), e(pp.e), pe(pp.pe) {}

  template <class P2>
  bool operator==(const PrimePower<P2> &rhs) const
  { return p == rhs.p && e == rhs.e && pe == rhs.pe; }
  template <class P2>
  bool operator!=(const PrimePower<P2> &rhs) const { return *this != rhs; }

  void mul_p() { e++, pe = ull(pe) * ull(p); }
  void div_p() { e--, pe /= p; }
};
#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT(PrimePower<int>, p, e, pe);
CPP_DUMP_DEFINE_EXPORT_OBJECT(PrimePower<ll>, p, e, pe);
#endif

// n が m で割り切れる回数 e について、(e, m^e, n/m^e)
tuple<int, ll, ll> ord_pow_div(ll n, ll m)
{
  assert(m >= 2);
  if (m == 2)
  {
    int e = countr_zero(n);
    return {e, 1LL << e, n >> e};
  }
  if (n % m != 0)
    return {0, 1, n};
  n /= m;
  if (n % m != 0)
    return {1, m, n};
  n /= m;
  ll m2 = m * m;
  auto [f, m2f, nn] = ord_pow_div(n, m2);
  int e = 2 + 2 * f;
  ll me = m2f * m2;
  if (nn % m == 0)
    e++, me *= m, nn /= m;
  return {e, me, nn};
}

// 相異なる素因数
template <class P>
vc<P> factors(const vc<PrimePower<P>> &fac)
{
  vc<P> res(fac.size());
  repi(i, fac.size()) res[i] = fac[i].p;
  return res;
}

// 引数 fac は素因数分解形
template <class P>
vc<ll> divisors(const vc<PrimePower<P>> &fac)
{
  vc<ll> res;
  auto dfs = [&](auto dfs, ll d, int i) -> void
  {
    if (i == SZ<int>(fac))
    {
      res.emplace_back(d);
      return;
    }
    auto &pp = fac[i];
    ull nd = d;
    repi(j, pp.e + 1)
    {
      dfs(dfs, nd, i + 1);
      nd *= pp.p;
    }
  };
  dfs(dfs, 1, 0);
  sort(ALL(res));
  return res;
}

template <class P>
vc<PrimePower<P>> factorized_mul
(const vc<PrimePower<P>> &fac1, const vc<PrimePower<P>> &fac2)
{
  const int n = fac1.size(), m = fac2.size();
  vc<PrimePower<P>> fac;
  fac.reserve(n + m);
  int i = 0, j = 0;
  while (i < n && j < m)
  {
    if (fac1[i].p < fac2[j].p)
      fac.emplace_back(fac1[i++]);
    else if (fac1[i].p > fac2[j].p)
      fac.emplace_back(fac2[j++]);
    else
    {
      fac.emplace_back(fac1[i].p, fac1[i].e + fac2[j].e, ull(fac1[i].pe) * ull(fac2[j].pe));
      i++, j++;
    }
  }
  fac.insert(fac.end(), fac1.begin() + i, fac1.end());
  fac.insert(fac.end(), fac2.begin() + j, fac2.end());
  return fac;
}