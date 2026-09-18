#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "../../algebra/algebra_basic_ops.hpp"
#include "../dot_product.hpp"

/**
 * @brief Berlekamp–Massey
 * @docs docs/math/fps/berlekamp_massey.md
 */

// https://codeforces.com/blog/entry/61306

// F は体 (FieldAddSubMulDiv<mint> など)
// 入力: a[0], ..., a[n-1]
// 出力: c[1], ..., c[d] であって a[i] = sum[j=1..d] c[j]a[i-j] を満たすもののうち d が最小のもの
//   (c[0] == -1 とする)
// 計算量: O(n^2) 時間、O(n) 空間
template <class F>
struct BerlekampMassey
{
  using S = typename F::S;
  vc<S> a, b, c, tmp;
  int pos = -1;
  S inv_x = F::e0();

  int degree() const { return c.size(); }

  void append(S value)
  {
    const int i = a.size(), d = c.size();
    a.eb(value);
    S y = F::add(value, F::minus(dot_product<F>(d, c.begin(), a.rbegin() + 1)));
    if (y == F::e0())
      return;
    
    if (c.empty())
    {
      c.assign(i + 1, F::e0());
      pos = i;
      inv_x = F::inv(y);
      return;
    }

    S z = F::mul(y, inv_x);
    int d2 = i - pos + b.size();
    if (d2 > d)
    {
      tmp = c;
      c.resize(d2, F::e0());
    }
    c[i - 1 - pos] = F::add(c[i - 1 - pos], z);
    const S minus_z = F::minus(z);
    repi(j, b.size()) c[i - pos + j] = F::add(c[i - pos + j], F::mul(minus_z, b[j]));
    if (d2 > d)
      pos = i, inv_x = F::inv(y), swap(tmp, b);
  }

  vc<S> coefficients() const
  {
    vc<S> res;
    res.reserve(c.size() + 1);
    res.eb(F::minus(F::e1()));
    res.insert(res.end(), c.begin(), c.end());
    return res;
  }
};

template <class F>
vc<typename F::S> berlekamp_massey(const vc<typename F::S> &a)
{
  BerlekampMassey<F> bm;
  bm.a.reserve(a.size());
  for (const auto &x : a) bm.append(x);
  return bm.coefficients();
}
