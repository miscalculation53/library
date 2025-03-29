#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../math/algebra/algebra_basic_ops.hpp"

/**
 * @brief クロネッカー冪の作用
 * @docs docs/math/set/kronecker_power.md
 */

// SR は半環
// k × k 行列 mat のクロネッカー冪を k^n 次元ベクトル v に作用させる
// v を破壊的に変更する
// O(n k^{n+1}) 時間
template <class SR, int k>
void kronecker_power_destructive
(const array<array<typename SR::S, k>, k> &mat, vc<typename SR::S> &v)
{
  const int len = v.size();
  if (len == 0)
    return;
  {
    int len_ = len;
    while (len_ % k == 0)
      len_ /= k;
    assert(len_ == 1 && "v.size() must be a power of k");
  }
  for (int d = 1; d < len; d *= k)
    repi(iu, 0, len, d * k) repi(i, iu, iu + d)
    {
      array<typename SR::S, k> nxt;
      fill(ALL(nxt), SR::e0());
      repi(r, k) repi(c, k)
        nxt[r] = SR::add(nxt[r], SR::mul(mat[r][c], v[i + c * d]));
      repi(j, k) v[i + j * d] = nxt[j];
    }
}

template <class SR, int k>
vc<typename SR::S> kronecker_power
(const array<array<typename SR::S, k>, k> &mat, const vc<typename SR::S> &v)
{
  auto v_ = v;
  kronecker_power_destructive<SR, k>(mat, v_);
  return v_;
}