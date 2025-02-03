#pragma once

#include "../template/template_all.hpp"

/**
 * @brief 代数的構造の struct
 * @docs docs/math/algebraic_struct.md
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
  using G = T;
  static constexpr G op(G a, G b) { return a + b; }
  static constexpr G e() { return 0; }
  static constexpr G inv(G a) { return -a; }
};
template <class T>
struct GroupMulDiv
{
  using G = T;
  static constexpr G op(G a, G b) { return a * b; }
  static constexpr G e() { return 1; }
  static constexpr G inv(G a) { return 1 / a; }
};

template <class T, const T infty = INF>
struct SemiRingMinPlus
{
  using R = T;
  static constexpr R add(R a, R b) { return min(a, b); }
  static constexpr R e0() { return infty; }
  static constexpr R mul(R a, R b) { return a + b; }
};
template <class T, const T infty = INF>
struct SemiRingMaxPlus
{
  using R = T;
  static constexpr R add(R a, R b) { return max(a, b); }
  static constexpr R e0() { return -infty; }
  static constexpr R mul(R a, R b) { return a + b; }
};

template <class T>
struct RingAddSubMul
{
  using R = T;
  static constexpr R add(R a, R b) { return a + b; }
  static constexpr R minus(const R &a) { return -a; }
  static constexpr R e0() { return 0; }
  static constexpr R mul(R a, R b) { return a * b; }
};