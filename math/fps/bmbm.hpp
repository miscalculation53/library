#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "berlekamp_massey.hpp"
#include "bostan_mori.hpp"

/**
 * @brief BMBM
 * @docs docs/math/fps/bmbm.md
 */

// 入力: a[0], ..., a[d-1] と c[1], ..., c[d] (c[0] は適当な値)
// 出力: 母関数 p/q
template <class mint>
pair<FormalPowerSeries<mint>, FormalPowerSeries<mint>> linear_recurrence_gf(const vc<mint> &a, const vc<mint> &c)
{
  using F = FormalPowerSeries<mint>;
  const int d = SZ(c) - 1;
  assert(d >= 0);
  assert(SZ(a) >= d);
  F q = -F(c);
  q[0] = 1;
  F p = (F(a) * q).pre(d);
  return {p, q};
}

// 線型漸化的数列 a[0], a[1], ... の第 k 項を推測する
template <class mint>
mint bmbm(const vc<mint> &a, ll k, bool show_coefs = true)
{
  auto c = berlekamp_massey<FieldAddSubMulDiv<mint>>(a);
  if (show_coefs)
    dump(c | cp::index());
  auto [p, q] = linear_recurrence_gf(a, c);
  if (show_coefs)
    dump(p | cp::index(), q | cp::index());
  return bostan_mori(p, q, k);
}
