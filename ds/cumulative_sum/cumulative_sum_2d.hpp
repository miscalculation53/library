#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../algebra/algebra_basic_ops.hpp"

/**
 * @brief $2$ 次元累積和
 * @docs docs/ds/cumulative_sum/cumulative_sum_2d.md
 */

// G は可換
template <class G = GroupAddSub<ll>>
struct CumulativeSum2D
{
  using S = typename G::S;

private:
  vvc<S> s;

public:
  CumulativeSum2D() {}
  CumulativeSum2D(const vvc<S> &a)
  {
    const int n = a.size();
    if (n == 0)
      return;
    const int m = a[0].size();
    s.assign(n + 1, vc<S>(m + 1, G::e()));
    repi(i, n) repi(j, m) s[i + 1][j + 1] = G::op(s[i + 1][j], a[i][j]);
    repi(j, m) repi(i, n) s[i + 1][j + 1] = G::op(s[i + 1][j + 1], s[i][j + 1]);
  }
  // [0, ri) × [0, rj)
  // 4 引数のほうの順番は li, ri, lj, rj
  S sum(int ri, int rj)
  {
    if (s.empty())
      return G::e();
    const int n = SZ<int>(s) - 1;
    const int m = SZ<int>(s[0]) - 1;
    assert(0 <= ri && ri <= n);
    assert(0 <= rj && rj <= m);
    return s[ri][rj];
  }
  // [li, ri) × [lj, rj)
  // 4 引数のほうの順番は li, ri, lj, rj
  S sum(int li, int ri, int lj, int rj)
  {
    if (s.empty())
      return G::e();
    const int n = SZ<int>(s) - 1;
    const int m = SZ<int>(s[0]) - 1;
    assert(0 <= li && li <= ri && ri <= n);
    assert(0 <= lj && lj <= rj && rj <= m);
    return G::op(G::op(s[ri][rj], s[li][lj]), G::inv(G::op(s[ri][lj], s[li][rj])));
  }
};
