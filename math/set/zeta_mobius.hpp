#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "../../algebra/algebra_basic_ops.hpp"

/**
 * @brief ゼータ・メビウス変換
 * @docs docs/math/set/zeta_mobius.md
 */

// ζa[s] = Σ{t ⊆ s} a[t]
// M は可換モノイド (Σ だと +)
// |a| = 2^n を仮定、O(n 2^n) 時間
// 破壊的変更を行う
template <class M>
void zeta_subset_destructive(vc<typename M::S> &a)
{
  const int len = a.size();
  if (len == 0)
    return;
  assert(has_single_bit(len));
  for (int d = 1; d < len; d *= 2)
    repi(iu, 0, len, d * 2) repi(i, iu, iu + d)
      a[i + d] = M::op(a[i + d], a[i]);
}
// μ は ζ の逆変換
// μa[s] = Σ{t ⊆ s} (-1)^{|s\t|} a[t]
// G は可換群 (Σ だと +)
// |a| = 2^n を仮定、O(n 2^n) 時間
// 破壊的変更を行う
template <class G>
void mobius_subset_destructive(vc<typename G::S> &a)
{
  const int len = a.size();
  if (len == 0)
    return;
  assert(has_single_bit(len));
  for (int d = len >> 1; d; d >>= 1)
    repi(iu, 0, len, d * 2) repi(i, iu, iu + d)
      a[i + d] = G::op(a[i + d], G::inv(a[i]));
}

// ζ'a[s] = Σ{s ⊆ t} a[t]
// M は可換モノイド (Σ だと +)
// |a| = 2^n を仮定、O(n 2^n) 時間
// 破壊的変更を行う
template <class M>
void zeta_supset_destructive(vc<typename M::S> &a)
{
  const int len = a.size();
  if (len == 0)
    return;
  assert(has_single_bit(len));
  for (int d = 1; d < len; d *= 2)
    repi(iu, 0, len, d * 2) repi(i, iu, iu + d)
      a[i] = M::op(a[i], a[i + d]);
}
// μ' は ζ' の逆変換
// μ'a[s] = Σ{s ⊆ t} (-1)^{|t\s|} a[t]
// G は可換群 (Σ だと +)
// |a| = 2^n を仮定、O(n 2^n) 時間
// 破壊的変更を行う
template <class G>
void mobius_supset_destructive(vc<typename G::S> &a)
{
  const int len = a.size();
  if (len == 0)
    return;
  assert(has_single_bit(len));
  for (int d = len >> 1; d; d >>= 1)
    repi(iu, 0, len, d * 2) repi(i, iu, iu + d)
      a[i] = G::op(a[i], G::inv(a[i + d]));
}

// ζa[s] = Σ{t ⊆ s} a[t]
// M は可換モノイド (Σ だと +)
// |a| = 2^n を仮定、O(n 2^n) 時間
template <class M>
vc<typename M::S> zeta_subset(const vc<typename M::S> &a)
{
  auto b = a;
  zeta_subset_destructive<M>(b);
  return b;
}
// μ は ζ の逆変換
// μa[s] = Σ{t ⊆ s} (-1)^{|s\t|} a[t]
// G は可換群 (Σ だと +)
// |a| = 2^n を仮定、O(n 2^n) 時間
template <class G>
vc<typename G::S> mobius_subset(const vc<typename G::S> &a)
{
  auto b = a;
  mobius_subset_destructive<G>(b);
  return b;
}

// ζ'a[s] = Σ{s ⊆ t} a[t]
// M は可換モノイド (Σ だと +)
// |a| = 2^n を仮定、O(n 2^n) 時間
template <class M>
vc<typename M::S> zeta_supset(const vc<typename M::S> &a)
{
  auto b = a;
  zeta_supset_destructive<M>(b);
  return b;
}
// μ' は ζ' の逆変換
// μ'a[s] = Σ{s ⊆ t} (-1)^{|t\s|} a[t]
// G は可換群 (Σ だと +)
// |a| = 2^n を仮定、O(n 2^n) 時間
template <class G>
vc<typename G::S> mobius_supset(const vc<typename G::S> &a)
{
  auto b = a;
  mobius_supset_destructive<G>(b);
  return b;
}
