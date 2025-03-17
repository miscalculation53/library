#pragma once

#include "../../template/template_all.hpp"
#include "../algebra/algebra_basic_ops.hpp"

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
  if (a.empty())
    return;
  assert(has_single_bit(a.size()));
  const int n = a.size();
  for (int w = 1; w < n; w <<= 1)
    repi(k, 0, n, w * 2) repi(i, w)
      a[k + w + i] = M::op(a[k + w + i], a[k + i]);
}
// μ は ζ の逆変換
// μa[s] = Σ{t ⊆ s} (-1)^{|s\t|} a[t]
// G は可換群 (Σ だと +)
// |a| = 2^n を仮定、O(n 2^n) 時間
// 破壊的変更を行う
template <class G>
void mobius_subset_destructive(vc<typename G::S> &a)
{
  if (a.empty())
    return;
  assert(has_single_bit(a.size()));
  const int n = a.size();
  for (int w = n >> 1; w; w >>= 1)
    repi(k, 0, n, w * 2) repi(i, w)
      a[k + w + i] = G::op(a[k + w + i], G::inv(a[k + i]));
}

// ζ'a[s] = Σ{s ⊆ t} a[t]
// M は可換モノイド (Σ だと +)
// |a| = 2^n を仮定、O(n 2^n) 時間
// 破壊的変更を行う
template <class M>
void zeta_supset_destructive(vc<typename M::S> &a)
{
  if (a.empty())
    return;
  assert(has_single_bit(a.size()));
  const int n = a.size();
  for (int w = 1; w < n; w <<= 1)
    repi(k, 0, n, w * 2) repi(i, w)
      a[k + i] = M::op(a[k + i], a[k + w + i]);
}
// μ' は ζ' の逆変換
// μ'a[s] = Σ{s ⊆ t} (-1)^{|t\s|} a[t]
// G は可換群 (Σ だと +)
// |a| = 2^n を仮定、O(n 2^n) 時間
// 破壊的変更を行う
template <class G>
void mobius_supset_destructive(vc<typename G::S> &a)
{
  if (a.empty())
    return;
  assert(has_single_bit(a.size()));
  const int n = a.size();
  for (int w = n >> 1; w; w >>= 1)
    repi(k, 0, n, w * 2) repi(i, w)
      a[k + i] = G::op(a[k + i], G::inv(a[k + w + i]));
}

// ζa[s] = Σ{t ⊆ s} a[t]
// M は可換モノイド (Σ だと +)
// |a| = 2^n を仮定、O(n 2^n) 時間
template <class M>
vc<typename M::S> zeta_subset(const vc<typename M::S> &a)
{
  auto b = a;
  zeta_subset_destructive(b);
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
  mobius_subset_destructive(b);
  return b;
}

// ζ'a[s] = Σ{s ⊆ t} a[t]
// M は可換モノイド (Σ だと +)
// |a| = 2^n を仮定、O(n 2^n) 時間
template <class M>
vc<typename M::S> zeta_supset(const vc<typename M::S> &a)
{
  auto b = a;
  zeta_supset_destructive(b);
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
  mobius_supset_destructive(b);
  return b;
}