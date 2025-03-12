#pragma once

#include "../../template/template_all.hpp"
#include "zeta_mobius.hpp"

/**
 * @brief and/or 畳み込み
 * @docs docs/math/set/and_or_convolution.md
 */

// R は環
// |a| = |b| = 2^n を仮定、O(n 2^n) 時間
// 破壊的変更を行う: a に結果 (and conv) が、b には zeta_supset が入る
template <class R>
void and_convolution_destructive
(vc<typename R::S> &a, vc<typename R::S> &b)
{
  assert(a.size() == b.size());
  zeta_supset_destructive<MonoidOfSemiRingAdd<R>>(a);
  zeta_supset_destructive<MonoidOfSemiRingAdd<R>>(b);
  repi(i, a.size()) a[i] = R::mul(a[i], b[i]);
  mobius_supset_destructive<GroupOfRingAdd<R>>(a);
}
// R は環
// |a| = |b| = 2^n を仮定、O(n 2^n) 時間
// 破壊的変更を行う: a に結果 (or conv) が、b には zeta_subset が入る
template <class R>
void or_convolution_destructive
(vc<typename R::S> &a, vc<typename R::S> &b)
{
  assert(a.size() == b.size());
  zeta_subset_destructive<MonoidOfSemiRingAdd<R>>(a);
  zeta_subset_destructive<MonoidOfSemiRingAdd<R>>(b);
  repi(i, a.size()) a[i] = R::mul(a[i], b[i]);
  mobius_subset_destructive<GroupOfRingAdd<R>>(a);
}

// R は環
// |a| = |b| = 2^n を仮定、O(n 2^n) 時間
template <class R>
vc<typename R::S> and_convolution
(const vc<typename R::S> &a, const vc<typename R::S> &b)
{
  auto a_ = a, b_ = b;
  and_convolution_destructive<R>(a_, b_);
  return a_;
}
// R は環
// |a| = |b| = 2^n を仮定、O(n 2^n) 時間
template <class R>
vc<typename R::S> or_convolution
(const vc<typename R::S> &a, const vc<typename R::S> &b)
{
  auto a_ = a, b_ = b;
  or_convolution_destructive<R>(a_, b_);
  return a_;
}