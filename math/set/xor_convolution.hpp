#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "../algebra/algebra_basic_ops.hpp"

/**
 * @brief アダマール変換・xor 畳み込み
 * @docs docs/math/set/xor_convolution.md
 */

// アダマール変換
// |a| = 2^n を仮定、O(n 2^n) 時間
// G は可換群
// 破壊的変更を行う
template <class G>
void hadamard_abel_group_destructive(vc<typename G::S> &a)
{
  const int len = a.size();
  if (len == 0)
    return;
  assert(has_single_bit(len));
  for (int d = 1; d < len; d *= 2)
    repi(iu, 0, len, d * 2) repi(i, iu, iu + d)
    {
      auto x = G::op(a[i], a[i + d]);
      auto y = G::op(a[i], G::inv(a[i + d]));
      a[i] = x;
      a[i + d] = y;
    }
}

// アダマール変換 (inv == true のときは逆変換)
// |a| = 2^n を仮定、O(n 2^n) 時間
// K は体
// 破壊的変更を行う
template <class K>
void hadamard_field_destructive(vc<typename K::S> &a, bool inv)
{
  hadamard_abel_group_destructive<GroupOfRingAdd<K>>(a);
  if (inv)
  {
    const int lg = countr_zero(a.size());
    auto div = pow_monoid<MonoidOfSemiRingMul<K>>(K::inv(K::add(K::e1(), K::e1())), lg);
    repi(i, a.size()) a[i] = K::mul(a[i], div);
  }
}

// アダマール変換
// |a| = 2^n を仮定、O(n 2^n) 時間
// G は可換群
template <class G>
vc<typename G::S> hadamard_abel_group(const vc<typename G::S> &a)
{
  auto a_ = a;
  hadamard_abel_group_destructive<G>(a_);
  return a_;
}

// アダマール変換 (inv == true のときは逆変換)
// |a| = 2^n を仮定、O(n 2^n) 時間
// K は体
template <class K>
vc<typename K::S> hadamard_field(const vc<typename K::S> &a, bool inv)
{
  auto a_ = a;
  hadamard_field_destructive<K>(a_, inv);
  return a_;
}

// |a| = |b| = 2^n を仮定、O(n 2^n) 時間
// K は体
// 破壊的に変更する: a に結果 (xor conv) が、b に hadamard が入る
template <class K>
void xor_convolution_destructive
(vc<typename K::S> &a, vc<typename K::S> &b)
{
  assert(a.size() == b.size());
  hadamard_field_destructive<K>(a, false);
  hadamard_field_destructive<K>(b, false);
  repi(i, a.size()) a[i] = K::mul(a[i], b[i]);
  hadamard_field_destructive<K>(a, true);
}

// |a| = |b| = 2^n を仮定、O(n 2^n) 時間
// K は体
template <class K>
vc<typename K::S> xor_convolution
(const vc<typename K::S> &a, const vc<typename K::S> &b)
{
  auto a_ = a, b_ = b;
  xor_convolution_destructive<K>(a_, b_);
  return a_;
}