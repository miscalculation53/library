#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 逆元をまとめて求める
 * @docs docs/math/modint/inv_many.md
 */

// 体 R を明示し、R::e1, R::mul, R::inv を使う。
template <class R>
vc<typename R::S> inv_many(const vc<typename R::S> &a)
{
  const int n = a.size();
  if (n == 0) return {};
  vc<typename R::S> p(n + 1, R::e1());
  repi(i, n) p[i + 1] = R::mul(p[i], a[i]);
  auto ip = R::inv(p.back());
  assert(R::mul(ip, p.back()) == R::e1());
  auto res = a;
  repi(i, n - 1, -1, -1)
  {
    res[i] = R::mul(ip, p[i]);
    ip = R::mul(ip, a[i]);
  }
  return res;
}
