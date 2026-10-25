#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../algebra/algebra_basic_ops.hpp"

/**
 * @brief $2$ 次元動的 imos 法（取得した位置まで原点側の差分を展開）
 * @docs docs/ds/cumulative_sum/imos_dynamic_2d.md
 */

// G は可換群。
template <class G>
struct ImosDynamic2D
{
  using S = typename G::S;

private:
  int n, m;
  // 展開済みの部分は値、その外側は差分。
  vvc<S> d;
  // 行 i の差分の [0, built[i]) の積。
  vc<S> row_sum;
  // 展開済みの接頭辞の長さ。広義単調減少。
  vc<int> built;

public:
  ImosDynamic2D() : ImosDynamic2D(0, 0) {}
  ImosDynamic2D(int n, int m) : n(n), m(m)
  {
    assert(n >= 0 && m >= 0);
    d.assign(n, vc<S>(m, G::e()));
    row_sum.assign(n, G::e());
    built.assign(n, 0);
  }
  ImosDynamic2D(const vvc<S> &a)
      : ImosDynamic2D(a.size(), a.empty() ? 0 : a[0].size())
  {
    for (const auto &row : a)
      assert(SZ<int>(row) == m);
    repi(i, n) repi(j, m) add(i, i + 1, j, j + 1, a[i][j]);
  }

  // 未展開の矩形 [li, ri) × [lj, rj) に v を足す。
  void add(int li, int ri, int lj, int rj, const S &v)
  {
    assert(0 <= li && li <= ri && ri <= n);
    assert(0 <= lj && lj <= rj && rj <= m);
    if (li == ri || lj == rj)
      return;
    // 展開済みの領域は原点側に閉じているので、左上の 1 点で判定できる。
    assert(built[li] <= lj);
    d[li][lj] = G::op(d[li][lj], v);
    S iv = G::inv(v);
    if (ri < n)
      d[ri][lj] = G::op(d[ri][lj], iv);
    if (rj < m)
      d[li][rj] = G::op(d[li][rj], iv);
    if (ri < n && rj < m)
      d[ri][rj] = G::op(d[ri][rj], v);
  }
  // 位置 (i, j) の値を v にする。取得済みの位置にも使える。
  void set(int i, int j, const S &v)
  {
    S delta = G::op(G::inv(get(i, j)), v), idelta = G::inv(delta);
    d[i][j] = v;
    // 展開済みの隣接点の値を保ち、未展開の差分と行の累積値を補正する。
    if (built[i] == j + 1)
    {
      row_sum[i] = G::op(row_sum[i], delta);
      if (j + 1 < m)
        d[i][j + 1] = G::op(d[i][j + 1], idelta);
    }
    if (i + 1 < n)
    {
      if (built[i + 1] <= j)
        d[i + 1][j] = G::op(d[i + 1][j], idelta);
      if (built[i + 1] == j + 1)
        row_sum[i + 1] = G::op(row_sum[i + 1], idelta);
      if (built[i + 1] <= j + 1 && j + 1 < m)
        d[i + 1][j + 1] = G::op(d[i + 1][j + 1], delta);
    }
  }
  // [0, i] × [0, j] の差分を展開し、位置 (i, j) の値を返す。
  S get(int i, int j)
  {
    assert(0 <= i && i < n && 0 <= j && j < m);
    int li = i + 1;
    while (li > 0 && built[li - 1] <= j)
      --li;
    for (int x = li; x <= i; ++x)
      for (int &y = built[x]; y <= j; ++y)
      {
        row_sum[x] = G::op(row_sum[x], d[x][y]);
        d[x][y] = x == 0 ? row_sum[x] : G::op(d[x - 1][y], row_sum[x]);
      }
    return d[i][j];
  }

  // 全体をコピー上で展開し、元の更新可能範囲を保つ。
  vvc<S> content() const
  {
    auto copy = *this;
    if (n > 0 && m > 0)
      copy.get(n - 1, m - 1);
    return std::move(copy.d);
  }
};
