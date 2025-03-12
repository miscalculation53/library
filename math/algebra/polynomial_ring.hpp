#pragma once

#include "../../template/template_all.hpp"
#include "algebra_base.hpp"

/**
 * @brief 代数的構造（多項式環）
 * @docs docs/math/algebra/polynomial_ring.md
 */

// サイズ n (つまり n-1 次) で打ち切る
template <class R, int n>
struct PolynomialRingArray
{
  using S = array<typename R::S, n>;
  static constexpr S add(const S &a, const S &b)
  {
    S c;
    repi(i, n) c[i] = R::add(a[i], b[i]);
    return c;
  }
  static constexpr S e0()
  {
    S a;
    fill(ALL(a), R::e0());
    return a;
  }
  static constexpr S minus(const S &a)
  {
    S b;
    repi(i, n) b[i] = R::minus(a[i]);
    return b;
  }
  static constexpr S mul(const S &a, const S &b)
  {
    S c;
    fill(ALL(c), R::e0());
    repi(i, n) repi(j, n - i) c[i + j] = R::add(c[i + j], R::mul(a[i], b[j]));
    return c;
  }
  static constexpr S e1()
  {
    S a;
    fill(ALL(a), R::e0());
    if constexpr (n >= 1)
      a[0] = R::e1();
    return a;
  }
};

// 打ち切らない (掛け算も)
template <class R>
struct PolynomialRingVector
{
  using S = vc<typename R::S>;
  static constexpr S add(const S &a, const S &b)
  {
    S c(max(a.size(), b.size()), R::e0());
    repi(i, a.size()) c[i] = R::add(c[i], a[i]);
    repi(i, b.size()) c[i] = R::add(c[i], b[i]);
    return c;
  }
  static constexpr S e0() { return {}; }
  static constexpr S minus(const S &a)
  {
    S b(a.size());
    repi(i, a.size()) b[i] = R::minus(a[i]);
    return b;
  }
  static constexpr S mul(const S &a, const S &b)
  {
    const int n = a.size(), m = b.size();
    S c(n + m - 1, R::e0());
    repi(i, n) repi(j, m) c[i + j] = R::add(c[i + j], R::mul(a[i], b[j]));
    return c;
  }
  static constexpr S e1() { return {R::e1()}; }
};