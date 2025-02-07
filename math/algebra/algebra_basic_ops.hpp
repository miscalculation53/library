#pragma once

#include "../../template/template_all.hpp"
#include "algebra_base.hpp"

/**
 * @brief 代数的構造（四則演算と min, max）
 * @docs docs/math/algebra/algebra_basic_ops.md
 */

template <class T>
struct MonoidAdd
{
  using S = T;
  static constexpr S op(S a, S b) { return a + b; }
  static constexpr S e() { return 0; }
};
template <class T>
struct MonoidMul
{
  using S = T;
  static constexpr S op(S a, S b) { return a * b; }
  static constexpr S e() { return 1; }
};
template <class T, const T infty = INF>
struct MonoidMin
{
  using S = T;
  static constexpr S op(S a, S b) { return min(a, b); }
  static constexpr S e() { return infty; }
};
template <class T, const T infty = INF>
struct MonoidMax
{
  using S = T;
  static constexpr S op(S a, S b) { return max(a, b); }
  static constexpr S e() { return -infty; }
};

template <class T>
struct GroupAddSub
{
  using S = T;
  static constexpr S op(S a, S b) { return a + b; }
  static constexpr S e() { return 0; }
  static constexpr S inv(S a) { return -a; }
};
template <class T>
struct GroupMulDiv
{
  using S = T;
  static constexpr S op(S a, S b) { return a * b; }
  static constexpr S e() { return 1; }
  static constexpr S inv(S a) { return 1 / a; }
};

template <class T, const T infty = INF>
struct SemiRingMinPlus
{
  using S = T;
  static constexpr S add(S a, S b) { return min(a, b); }
  static constexpr S e0() { return infty; }
  static constexpr S mul(S a, S b) { return a + b; }
  static constexpr S e1() { return 0; }
};
template <class T, const T infty = INF>
struct SemiRingMaxPlus
{
  using S = T;
  static constexpr S add(S a, S b) { return max(a, b); }
  static constexpr S e0() { return -infty; }
  static constexpr S mul(S a, S b) { return a + b; }
  static constexpr S e1() { return 0; }
};

template <class T>
struct RingAddSubMul
{
  using S = T;
  static constexpr S add(S a, S b) { return a + b; }
  static constexpr S minus(S a) { return -a; }
  static constexpr S e0() { return 0; }
  static constexpr S mul(S a, S b) { return a * b; }
  static constexpr S e1() { return 1; }
};