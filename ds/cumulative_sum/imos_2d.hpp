#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../math/algebra/algebra_basic_ops.hpp"

/**
 * @brief $2$ 次元 imos 法
 * @docs docs/ds/cumulative_sum/imos_2d.md
 */

// G は可換
template <class G = GroupAddSub<ll>>
struct Imos2D
{
  using S = typename G::S;

private:
  vvc<S> d;

public:
  Imos2D() {}
  Imos2D(int n, int m) : d(n, vc<S>(m, G::e())) {}

  // [li, ri) × [lj, rj) に v を足す
  void add(int li, int ri, int lj, int rj, const S &v)
  {
    if (d.empty())
      return;
    const int n = d.size(), m = d[0].size();
    assert(0 <= li && li <= ri && ri <= n);
    assert(0 <= lj && lj <= rj && rj <= m);
    d[li][lj] = G::op(d[li][lj], v);
    if (ri == n && rj == m)
      return;
    S iv = G::inv(v);
    if (rj != m)
      d[li][rj] = G::op(d[li][rj], iv);
    if (ri != n)
      d[ri][lj] = G::op(d[ri][lj], iv);
    if (ri != n && rj != m)
      d[ri][rj] = G::op(d[ri][rj], v);
  }

  // 現状の vector を返す
  vvc<S> content() 
  {
    if (d.empty())
      return {};
    const int n = d.size(), m = d[0].size();
    vvc<S> a(d);
    repi(i, n) repi(j, m - 1) a[i][j + 1] = G::op(a[i][j + 1], a[i][j]);
    repi(j, m) repi(i, m - 1) a[i + 1][j] = G::op(a[i + 1][j], a[i][j]);
    return a;
  }
};
