#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "../../algebra/algebra_basic_ops.hpp"

/**
 * @brief Berlekamp–Massey
 * @docs docs/math/fps/berlekamp_massey.md
 */

// https://codeforces.com/blog/entry/61306

// F は体 (FieldAddSubMulDiv<mint> など)
// 入力: a[0], ..., a[n-1]
// 出力: c[1], ..., c[d] であって a[i] = sum[j=1..d] c[j]a[i-j] を満たすもののうち d が最小のもの
//   (c[0] == -1 とする)
// 計算量: O(n^2) 時間、n = 10000 で 86 ms くらい
template <class F>
vc<typename F::S> berlekamp_massey(const vc<typename F::S> &a)
{
  using S = typename F::S;
  const int n = a.size();
  vc<S> b, c;
  int pos = -1;
  S x = F::e0();
  repi(i, n)
  {
    const int d = c.size();
    S y = a[i];
    repi(j, d) y = F::add(y, F::minus(F::mul(c[j], a[i - 1 - j])));
    if (y == F::e0())
      continue;
    
    if (c.empty())
    {
      c.assign(i + 1, F::e0());
      pos = i;
      x = y;
      continue;
    }

    S z = F::mul(y, F::inv(x));
    int d2 = i - pos + b.size();
    vc<S> tmp;
    if (d2 >= d)
    {
      tmp = c;
      c.resize(d2, F::e0());
    }
    c[i - 1 - pos] = F::add(c[i - 1 - pos], z);
    repi(j, b.size()) c[i - pos + j] = F::add(c[i - pos + j], F::minus(F::mul(z, b[j])));
    if (d2 >= d)
      pos = i, x = y, swap(tmp, b);
  }
  c.insert(c.begin(), F::minus(F::e1()));
  return c;
}
