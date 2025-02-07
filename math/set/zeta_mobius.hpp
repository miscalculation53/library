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
template <class M>
vc<typename M::S> zeta_subset(const vc<typename M::S> &a)
{
  if (a.empty())
    return {};
  assert(has_single_bit(a.size()));
  const int n = countr_zero(a.size());
  auto b = a;
  repi(i, n) repi(s, 1 << n)
  {
    if (!btest(s, i))
    {
      int t = s;
      bset(t, i);
      b[t] = M::op(b[t], b[s]);
    }
  }
  return b;
}
// μ は ζ の逆変換
// μa[s] = Σ{t ⊆ s} (-1)^{|s\t|} a[t]
// G は可換群 (Σ だと +)
// |a| = 2^n を仮定、O(n 2^n) 時間
template <class G>
vc<typename G::S> mobius_subset(const vc<typename G::S> &a)
{
  if (a.empty())
    return {};
  assert(has_single_bit(a.size()));
  const int n = countr_zero(a.size());
  auto b = a;
  repi(i, n) repi(s, 1 << n)
  {
    if (!btest(s, i))
    {
      int t = s;
      bset(t, i);
      b[t] = G::op(b[t], G::inv(b[s]));
    }
  }
  return b;
}

// ζ'a[s] = Σ{s ⊆ t} a[t]
// M は可換モノイド (Σ だと +)
// |a| = 2^n を仮定、O(n 2^n) 時間
template <class M>
vc<typename M::S> zeta_supset(const vc<typename M::S> &a)
{
  if (a.empty())
    return {};
  assert(has_single_bit(a.size()));
  const int n = countr_zero(a.size());
  auto b = a;
  repi(i, n) repi(s, 1 << n)
  {
    if (!btest(s, i))
    {
      int t = s;
      bset(t, i);
      b[s] = M::op(b[s], b[t]);
    }
  }
  return b;
}
// μ' は ζ' の逆変換
// μ'a[s] = Σ{s ⊆ t} (-1)^{|t\s|} a[t]
// G は可換群 (Σ だと +)
// |a| = 2^n を仮定、O(n 2^n) 時間
template <class G>
vc<typename G::S> mobius_supset(const vc<typename G::S> &a)
{
  if (a.empty())
    return {};
  assert(has_single_bit(a.size()));
  const int n = countr_zero(a.size());
  auto b = a;
  repi(i, n) repi(s, 1 << n)
  {
    if (!btest(s, i))
    {
      int t = s;
      bset(t, i);
      b[s] = G::op(b[s], G::inv(b[t]));
    }
  }
  return b;
}