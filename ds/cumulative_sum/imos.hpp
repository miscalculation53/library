#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../math/algebra/algebra_basic_ops.hpp"

/**
 * @brief $1$ 次元 imos 法
 * @docs docs/ds/cumulative_sum/imos.md
 */

// G は可換
template <class G = GroupAddSub<ll>>
struct Imos
{
  using S = typename G::S;

private:
  vc<S> d;

public:
  Imos() {}
  Imos(int n) : d(n, G::e()) {}
  Imos(const vc<S> &a)
  {
    const int n = a.size();
    d.assign(n, G::e());
    repi(i, n) add(i, i + 1, a[i]);
  }

  // [l, r) に v を足す
  void add(int l, int r, const S &v)
  {
    const int n = d.size();
    assert(0 <= l && l <= r && r <= n);
    if (l == r)
      return;
    d[l] = G::op(d[l], v);
    if (r != n)
      d[r] = G::op(d[r], G::inv(v));
  }

  // 現状の vector を返す
  vc<S> content() { return cuml<G>(d, 1); }
};
