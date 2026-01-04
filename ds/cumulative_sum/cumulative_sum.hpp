#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../math/algebra/algebra_basic_ops.hpp"

/**
 * @brief $1$ 次元累積和
 * @docs docs/ds/cumulative_sum/cumulative_sum.md
 */

template <class G = GroupAddSub<ll>>
struct CumulativeSum
{
  using S = typename G::S;

private:
  vc<S> s;

public:
  CumulativeSum() {}
  CumulativeSum(const vc<S> &a)
  {
    const int n = a.size();
    s.resize(n + 1, G::e());
    repi(i, n) s[i + 1] = G::op(s[i], a[i]);
  }
  // [0, r)
  S sum(int r) const
  {
    const int n = SZ<int>(s) - 1;
    assert(0 <= r && r <= n);
    return s[r];
  }
  // [l, r)
  S sum(int l, int r) const
  {
    const int n = SZ<int>(s) - 1;
    assert(0 <= l && l <= r && r <= n);
    return G::op(G::inv(s[l]), s[r]);
  }
};
