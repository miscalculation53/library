#pragma once

#include "template/template_all_but_modint.hpp"
#include "math/modint/modint.hpp"
#include "math/modint/modint64.hpp"
#include "math/prime/prime_power.hpp"
#include "math/prime/large/primality_test.hpp"
#include "math/prime/large/factorize.hpp"

/**
 * @brief オイラーのファイ関数・カーマイケル関数
 * @docs docs/math/prime/large/euler_phi_carmichael.md
 */

template <class P>
ll euler_phi(const vc<PrimePower<P>> &fac)
{
  ll res = 1;
  fec(pp : fac)
  {
    res *= pp.pe / pp.p * (pp.p - 1);
  }
  return res;
}

template <class P>
ll carmichael(const vc<PrimePower<P>> &fac)
{
  ll res = 1;
  fec(pp : fac)
  {
    ll tmp = pp.pe / pp.p * (pp.p - 1);
    if (pp.p == 2 && pp.e >= 3)
      tmp /= 2;
    res = lcm(res, tmp);
  }
  return res;
}
