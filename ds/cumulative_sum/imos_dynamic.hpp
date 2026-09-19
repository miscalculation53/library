#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../algebra/algebra_basic_ops.hpp"

/**
 * @brief $1$ 次元動的 imos 法（取得した位置まで差分を展開）
 * @docs docs/ds/cumulative_sum/imos_dynamic.md
 */

// G は可換群。
template <class G>
struct ImosDynamic
{
  using S = typename G::S;

private:
  int built = 0;
  // 展開済みの部分は値、それより後ろは差分。
  vc<S> d;

public:
  ImosDynamic() : ImosDynamic(0) {}
  ImosDynamic(int n)
  {
    assert(n >= 0);
    d.assign(n, G::e());
  }
  ImosDynamic(const vc<S> &a) : d(a)
  {
    for (int i = SZ<int>(a) - 1; i > 0; --i)
      d[i] = G::op(G::inv(a[i - 1]), a[i]);
  }

  // 未展開の区間 [l, r) に v を足す。
  void add(int l, int r, const S &v)
  {
    const int n = d.size();
    assert(0 <= l && l <= r && r <= n);
    if (l == r)
      return;
    assert(built <= l);
    d[l] = G::op(d[l], v);
    if (r < n)
      d[r] = G::op(d[r], G::inv(v));
  }
  // 位置 i の値を v にする。取得済みの位置にも使える。
  void set(int i, const S &v)
  {
    S old = get(i);
    if (built == i + 1 && built < SZ<int>(d))
      d[built] = G::op(d[built], G::op(old, G::inv(v)));
    d[i] = v;
  }
  // [0, i] の差分を展開し、位置 i の値を返す。
  S get(int i)
  {
    assert(0 <= i && i < SZ<int>(d));
    for (; built <= i; ++built)
      if (built > 0)
        d[built] = G::op(d[built - 1], d[built]);
    return d[i];
  }

  // 全体をコピー上で展開し、元の更新可能範囲を保つ。
  vc<S> content() const
  {
    auto copy = *this;
    if (!d.empty())
      copy.get(SZ<int>(d) - 1);
    return std::move(copy.d);
  }
};
