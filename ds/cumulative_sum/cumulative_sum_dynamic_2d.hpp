#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../algebra/algebra_basic_ops.hpp"

/**
 * @brief $2$ 次元動的累積和（取得した原点側の矩形を確定）
 * @docs docs/ds/cumulative_sum/cumulative_sum_dynamic_2d.md
 */

// G は可換群。
template <class G>
struct CumulativeSumDynamic2D
{
  using S = typename G::S;

private:
  int n, m;
  vvc<S> a, s;
  vc<S> row_sum;
  // 行 i の [0, fixed[i]) が確定済み。fixed は広義単調減少。
  vc<int> fixed;

public:
  CumulativeSumDynamic2D() : CumulativeSumDynamic2D(0, 0) {}
  CumulativeSumDynamic2D(int n, int m) : n(n), m(m)
  {
    assert(n >= 0 && m >= 0);
    a.assign(n, vc<S>(m, G::e()));
    s.assign(n + 1, vc<S>(m + 1, G::e()));
    row_sum.assign(n, G::e());
    fixed.assign(n, 0);
  }
  CumulativeSumDynamic2D(const vvc<S> &a)
      : CumulativeSumDynamic2D(a.size(), a.empty() ? 0 : a[0].size())
  {
    for (const auto &row : a)
      assert(SZ<int>(row) == m);
    this->a = a;
  }

  void set(int i, int j, const S &v)
  {
    assert(0 <= i && i < n && 0 <= j && j < m);
    assert(fixed[i] <= j);
    a[i][j] = v;
  }
  void add(int i, int j, const S &v)
  {
    assert(0 <= i && i < n && 0 <= j && j < m);
    assert(fixed[i] <= j);
    a[i][j] = G::op(a[i][j], v);
  }
  // 点取得では確定範囲を広げない。
  S get(int i, int j) const
  {
    assert(0 <= i && i < n && 0 <= j && j < m);
    return a[i][j];
  }

  // [0, ri) × [0, rj) を確定し、その積を返す。
  S sum(int ri, int rj)
  {
    assert(0 <= ri && ri <= n && 0 <= rj && rj <= m);
    if (ri == 0 || rj == 0)
      return G::e();
    int li = ri;
    while (li > 0 && fixed[li - 1] < rj)
      --li;
    for (int i = li; i < ri; ++i)
      for (int &j = fixed[i]; j < rj; ++j)
      {
        row_sum[i] = G::op(row_sum[i], a[i][j]);
        s[i + 1][j + 1] = G::op(s[i][j + 1], row_sum[i]);
      }
    return s[ri][rj];
  }
  // [0, ri) × [0, rj) を確定し、[li, ri) × [lj, rj) の積を返す。
  // 空矩形は確定範囲を広げない。
  S sum(int li, int ri, int lj, int rj)
  {
    assert(0 <= li && li <= ri && ri <= n);
    assert(0 <= lj && lj <= rj && rj <= m);
    if (li == ri || lj == rj)
      return G::e();
    sum(ri, rj);
    return G::op(G::op(s[ri][rj], s[li][lj]), G::inv(G::op(s[ri][lj], s[li][rj])));
  }

  vvc<S> content() const { return a; }
};
