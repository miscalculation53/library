#pragma once

#include "../../template/template_all.hpp"

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

  PrimePower() {}
  PrimePower(P p, int e = 1) : p(p), e(e), pe(ipow(p, e)) {}
  PrimePower(P p, int e, P pe) : p(p), e(e), pe(pe) {}
  template <class P2>
  PrimePower(const PrimePower<P2> &pp) : p(pp.p), e(pp.e), pe(pp.pe) {}

  void mul_p() { e++, pe = ull(pe) * ull(p); }
  void div_p() { e--, pe /= p; }
};
#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT(PrimePower<int>, p, e, pe);
CPP_DUMP_DEFINE_EXPORT_OBJECT(PrimePower<ll>, p, e, pe);
#endif

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
    else if (fac1[i].p > fac2[i].p)
      fac.emplace_back(fac2[j++]);
    else
    {
      fac.emplace_back(fac1[i].p, fac1[i].e + fac2[j].e, ull(fac1[i].pe) * ull(fac2[j].pe));
      i++, j++;
    }
  }
  fac.insert(fac.end(), ALL(fac1));
  fac.insert(fac.end(), ALL(fac2));
  return fac;
}