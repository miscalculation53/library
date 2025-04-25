#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../math/algebra/algebra_basic_ops.hpp"

/**
 * @brief クロネッカー冪の作用
 * @docs docs/math/set/kronecker_power.md
 */

// 線型写像 linear_map: T^k → T^k のテンソル冪を k^n 次元ベクトル v に作用させる
// linear_map は array<T, k>(array<T, k>) として渡す (破壊的に変更しても OK)
// v を破壊的に変更する
// linear_map の呼び出しが O(n k^{n-1}) 回
template <int k, class F, class T>
void tensor_power_array_destructive(const F &linear_map, vc<T> &v)
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
  {
    repi(iu, 0, len, d * k) repi(i, iu, iu + d)
    {
      array<T, k> tmp;
      repi(j, k) tmp[j] = v[i + j * d];
      tmp = linear_map(tmp);
      repi(j, k) v[i + j * d] = tmp[j];
    }
  }
}

// 線型写像 linear_map: T^k → T^k のテンソル冪を k^n 次元ベクトル v に作用させる
// linear_map は array<T, k>(array<T, k>) として渡す (破壊的に変更しても OK)
// linear_map の呼び出しが O(n k^{n-1}) 回
template <int k, class F, class T>
vc<T> tensor_power_array(const F &linear_map, vc<T> v)
{
  tensor_power_array_destructive<k>(linear_map, v);
  return v;
}

// SR は半環
// k × k 行列 mat のクロネッカー冪を k^n 次元ベクトル v に作用させる
// v を破壊的に変更する
// O(n k^{n+1}) 時間
template <class SR, int k>
void kronecker_power_array_destructive(const array<array<typename SR::S, k>, k> &mat, vc<typename SR::S> &v)
{
  using S = typename SR::S;
  auto linear_map = [&](const array<S, k> &arr) -> array<S, k>
  {
    array<S, k> res;
    fill(ALL(res), SR::e0());
    repi(i, k) repi(j, k) res[i] = SR::add(res[i], SR::mul(mat[i][j], arr[j]));
    return res;
  };
  tensor_power_array_destructive<k>(linear_map, v);
}

// SR は半環
// k × k 行列 mat のクロネッカー冪を k^n 次元ベクトル v に作用させる
// O(n k^{n+1}) 時間
template <class SR, int k>
vc<typename SR::S> kronecker_power_array(const array<array<typename SR::S, k>, k> &mat, vc<typename SR::S> v)
{
  kronecker_power_array_destructive<SR>(mat, v);
  return v;
}
