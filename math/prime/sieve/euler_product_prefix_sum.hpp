#pragma once

#include "math/prime/sieve/dirichlet_prefix_sum.hpp"
#include "ds/fenwick_tree/fenwick_tree.hpp"

/**
 * @brief 一般の局所因子の Euler 積と累積和
 * @docs docs/math/prime/sieve/euler_product_prefix_sum.md
 */

// factors: (次数d, 係数a) の列の列 
// seed * Π_i (1 + Σ_j a[ij] d[ij]^{-s}) を計算する
// R は可換環
// 高速な計算のためには、**seed の [2, √N] はゼロであるべき**
// 計算量の例: 
// - 2 <= i <= √N で d[ij] = Θ(i^j) のとき O(N^{2/3} log N) 時間
// - 上記でさらに i が素数しか動かないとき O(N^{2/3}) 時間
//
// 一般には m[i] = min_j{ a[ij] } としておおむね O(Σ_{i,j} min{√(N/d[ij]), N/(d[ij] m[i])}) 時間
template <class R>
DirichletPrefixSum<R> euler_product_prefix_sum(const DirichletPrefixSum<R> &seed,
  const vvc<pair<ll, typename R::S>> &factors)
{
  using S = typename R::S;
  const ll n = seed.n();
  if (n == 0) return {};
  const internal::dirichlet_prefix_sum_index coordinates(n);
  const int k = coordinates.k;
  const int size = coordinates.size();
  vc<ll> x(size + 1, 0);
  vc<S> blocks(size, R::e0()), change(blocks);
  // [2,first) の係数は零。seed の疎さも安全な範囲で利用する。
  ll first = ll(k) + 1;
  for (int i = 0; i < size; i++)
  {
    x[i + 1] = coordinates.value(i);
    blocks[i] = R::add(seed.F(x[i + 1]), R::minus(seed.F(x[i])));
    if (1 <= i && i < k && blocks[i] != R::e0()) first = min(first, ll(i) + 1);
  }
  FenwickTree<GroupOfRingAdd<R>> bit(blocks);
  vc<int> order(factors.size());
  iota(order.begin(), order.end(), 0);
  vc<ll> minimum(factors.size(), LLONG_MAX);
  for (size_t i = 0; i < factors.size(); i++)
    for (const auto &[d, a] : factors[i])
    {
      assert(d >= 1);
      if (d >= 2 && d <= n && a != R::e0()) minimum[i] = min(minimum[i], d);
    }
  sort(order.begin(), order.end(), [&](int i, int j) { return minimum[i] > minimum[j]; });
  vc<int> touched;
  vc<bool> used(size, false);
  auto add_change = [&](int i, const S &a)
  {
    if (a == R::e0()) return;
    if (!used[i]) used[i] = true, touched.push_back(i);
    change[i] = R::add(change[i], a);
  };
  for (int fi : order)
  {
    for (const auto &[d, a] : factors[fi])
    {
      if (d > n || a == R::e0()) continue;
      add_change(coordinates.index(d), R::mul(a, blocks[0]));
      const ll limit = n / d;
      const ll b = min(limit, max(first - 1, ll(iroot(limit, 2))));
      // 小さい元の係数は個別に押し出す。b<=√N なので係数を O(1) で読める。
      for (ll v = first; v <= b; v++)
        if (blocks[v - 1] != R::e0())
          add_change(coordinates.index(v * d), R::mul(a, blocks[v - 1]));
      // 大きい元の係数は、更新先のブロックごとに区間和を取得する。
      for (int i = size - 1; i >= 0 && x[i + 1] / d > b; i--)
      {
        const ll lo = max(b, x[i] / d), hi = x[i + 1] / d;
        const S value = bit.sum(coordinates.index(lo) + 1, coordinates.index(hi) + 1);
        add_change(i, R::mul(a, value));
      }
    }
    // 同じ局所因子の項を組み合わせないよう、全項を読んでから反映する。
    for (int i : touched)
    {
      if (change[i] != R::e0())
      {
        blocks[i] = R::add(blocks[i], change[i]);
        bit.add(i, change[i]);
      }
      change[i] = R::e0(); used[i] = false;
    }
    touched.clear();
    first = min(first, minimum[fi]);
  }
  DirichletPrefixSum<R> result(n);
  S prefix = R::e0();
  for (int i = 0; i < size; i++)
  {
    prefix = R::add(prefix, blocks[i]);
    result.setF(x[i + 1], prefix);
  }
  return result;
}

// factors: (次数d, 係数a) の列の列 
// seed * Π_i (1 + Σ_j a[ij] d[ij]^{-s}) を計算する
// R は可換環
// 高速な計算のためには、**seed の [2, √N] はゼロであるべき**
// 計算量の例: 
// - 2 <= i <= √N で d[ij] = Θ(i^j) のとき O(N^{2/3} log N) 時間
// - 上記でさらに i が素数しか動かないとき O(N^{2/3}) 時間
//
// 一般には m[i] = min_j{ a[ij] } としておおむね O(Σ_{i,j} min{√(N/d[ij]), N/(d[ij] m[i])}) 時間
template <class R>
DirichletPrefixSum<R> euler_product_prefix_sum(ll n,
  const vvc<pair<ll, typename R::S>> &factors)
{ return euler_product_prefix_sum(DirichletPrefixSum<R>::unit(n), factors); }
