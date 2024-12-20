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

  void mul_p() { e++, pe *= p; }
  void div_p() { e--, pe /= p; }
};
#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT(PrimePower<int>, p, e, pe);
CPP_DUMP_DEFINE_EXPORT_OBJECT(PrimePower<ll>, p, e, pe);
#endif