#pragma once

#include "../template/template_all_but_modint.hpp"
#include "algebra_base.hpp"
#include "../math/dot_product.hpp"

/**
 * @brief 代数的構造（多項式環）
 * @docs docs/algebra/polynomial_ring.md
 */

// R は環
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
    if constexpr (internal::dot_product_mod32<R>::value && n >= 16)
      repi(p, n) c[p] = dot_product<R>(p + 1, a.begin(), b.rbegin() + (n - 1 - p));
    else
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

// R は環
// 打ち切らない (掛け算も)
template <class R>
struct PolynomialRingVector
{
  using S = vc<typename R::S>;

private:
  static constexpr void normalize(S &a)
  {
    while (!a.empty() && a.back() == R::e0()) a.pop_back();
  }

public:
  static constexpr S add(const S &a, const S &b)
  {
    S c(max(a.size(), b.size()), R::e0());
    repi(i, a.size()) c[i] = R::add(c[i], a[i]);
    repi(i, b.size()) c[i] = R::add(c[i], b[i]);
    normalize(c);
    return c;
  }
  static constexpr S e0() { return {}; }
  static constexpr S minus(const S &a)
  {
    S b(a.size());
    repi(i, a.size()) b[i] = R::minus(a[i]);
    normalize(b);
    return b;
  }
  static constexpr S mul(const S &a, const S &b)
  {
    const int n = a.size(), m = b.size();
    if (n == 0 || m == 0)
      return {};
    S c(n + m - 1, R::e0());
    if constexpr (internal::dot_product_mod32<R>::value)
    {
      if (min(n, m) >= 16)
      {
        repi(p, n + m - 1)
        {
          const int l = max(0, p - m + 1), r = min(n, p + 1);
          c[p] = dot_product<R>(r - l, a.begin() + l, b.rbegin() + (m - 1 - p + l));
        }
        normalize(c);
        return c;
      }
    }
    repi(i, n) repi(j, m) c[i + j] = R::add(c[i + j], R::mul(a[i], b[j]));
    normalize(c);
    return c;
  }
  static constexpr S e1()
  {
    S a{R::e1()};
    normalize(a);
    return a;
  }
};
