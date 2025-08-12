#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 分割統治による区間積クエリ
 * @docs docs/ds/static_range/dc_range_prod.md
 */

#include "ds/group_index.hpp"

// M: モノイド
// v の [l, r) の積を答えるクエリにまとめて答える
// ただし、各クエリについては ans == x • y となる (x, y) を返す
// 使用場面：最終的な答えとしては一般の元の積がいらない場合
template <class M, class I>
vc<pair<typename M::S, typename M::S>> dc_range_prod_left_right
(const vc<typename M::S> &v, const vc<pair<I, I>> &lrs)
{
  using S = typename M::S;
  const int n = v.size(), q = lrs.size();
  vc<pair<S, S>> res(q);
  vc<int> mids(q, n + 1);
  rep(qi, q)
  {
    auto [l, r] = lrs[qi];
    if (l == r)
    {
      res[qi] = pair{M::e(), M::e()};
      continue;
    }
    if (l + 1 == r)
    {
      res[qi] = pair{v[l], M::e()};
      continue;
    }
    const int j = msb_pos(l ^ (r - 1));
    mids[qi] = (l | (1 << j)) & ~((1 << j) - 1);
  }
  vc<S> dat(n);
  GroupIndex grp(mids);
  repi(mid, 1, n)
  {
    auto qis = grp.idxs(mid);
    int l = n + 1, r = -1;
    fec(qi : qis)
    {
      auto [l_, r_] = lrs[qi];
      chmin(l, l_), chmax(r, r_);
    }
    dat[mid - 1] = v[mid - 1];
    repi(i, mid - 2, l - 1, -1) dat[i] = M::op(v[i], dat[i + 1]);
    dat[mid] = v[mid];
    repi(i, mid + 1, r) dat[i] = M::op(dat[i - 1], v[i]);
    fec(qi : qis)
    {
      auto [l_, r_] = lrs[qi];
      res[qi] = {dat[l_], dat[r_ - 1]};
    }
  }
  return res;
}

// M: モノイド
// v の [l, r) の積を答えるクエリにまとめて答える
template <class M, class I>
vc<typename M::S> dc_range_prod
(const vc<typename M::S> &v, const vc<pair<I, I>> &lrs)
{
  auto tmp = dc_range_prod_left_right<M, I>(v, lrs);
  const int q = lrs.size();
  vc<typename M::S> res(q);
  repi(i, q) res[i] = M::op(tmp[i].first, tmp[i].second);
  return res;
}
