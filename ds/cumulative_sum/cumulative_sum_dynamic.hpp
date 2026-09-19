#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../algebra/algebra_basic_ops.hpp"

/**
 * @brief $1$ 次元動的累積和（取得した接頭辞を確定）
 * @docs docs/ds/cumulative_sum/cumulative_sum_dynamic.md
 */

template <class G>
struct CumulativeSumDynamic
{
  using S = typename G::S;

private:
  int fixed = 0;
  vc<S> a, s;

public:
  CumulativeSumDynamic() : CumulativeSumDynamic(0) {}
  CumulativeSumDynamic(int n)
  {
    assert(n >= 0);
    a.assign(n, G::e());
    s.assign(n + 1, G::e());
  }
  CumulativeSumDynamic(const vc<S> &a) : a(a), s(a.size() + 1, G::e()) {}

  // 確定済みの接頭辞より後ろを更新する。
  void set(int i, const S &v)
  {
    assert(fixed <= i && i < SZ<int>(a));
    a[i] = v;
  }
  void add(int i, const S &v)
  {
    assert(fixed <= i && i < SZ<int>(a));
    a[i] = G::op(a[i], v);
  }
  // 点取得では確定範囲を広げない。
  S get(int i) const
  {
    assert(0 <= i && i < SZ<int>(a));
    return a[i];
  }

  // [0, r) を確定し、その積を返す。
  S sum(int r)
  {
    assert(0 <= r && r <= SZ<int>(a));
    for (; fixed < r; ++fixed)
      s[fixed + 1] = G::op(s[fixed], a[fixed]);
    return s[r];
  }
  // [0, r) を確定し、[l, r) の積を返す。空区間は確定範囲を広げない。
  S sum(int l, int r)
  {
    assert(0 <= l && l <= r && r <= SZ<int>(a));
    if (l == r)
      return G::e();
    sum(r);
    return G::op(G::inv(s[l]), s[r]);
  }

  vc<S> content() const { return a; }
};
