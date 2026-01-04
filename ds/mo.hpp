#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief Mo's algorithm
 * @docs docs/ds/mo.md
 */

namespace internal
{

template <class I>
int mo_order_cost(const vc<pair<I, I>> &lrs, const vc<int> &ord)
{
  int res = 0;
  repi(i, SZ(lrs) - 1)
  {
    res += abs(lrs[ord[i + 1]].first - lrs[ord[i]].first);
    res += abs(lrs[ord[i + 1]].second - lrs[ord[i]].second);
  }
  return res;
}

template <class I>
vc<int> mo_order_params(const vc<pair<I, I>> &lrs, int b, int t)
{
  const int q = lrs.size();
  cauto &[ls, rs] = top(lrs);
  auto comp = [&](int i, int j)
  {
    int segi = (ls[i] + t * b / 2) / b, segj = (ls[j] + t * b / 2) / b;
    if (segi != segj)
      return segi < segj;
    return (segi & 1) ? (rs[i] > rs[j]) : (rs[i] < rs[j]);
  };
  vc<int> ord = permid<int>(q);
  sort(ALL(ord), comp);
  return ord;
}

template <class I>
vc<int> mo_order(int n, const vc<pair<I, I>> &lrs)
{
  const int q = lrs.size();
  const int b1 = max(1, int(n / sqrt(q + 1)));
  const int b2 = max(1, int(sqrt(3) * n / sqrt(2 * q + 1)));
  array<vc<int>, 4> ords = {
    mo_order_params(lrs, b1, 0),
    mo_order_params(lrs, b1, 1),
    mo_order_params(lrs, b2, 0),
    mo_order_params(lrs, b2, 1)
  };
  array<int, 4> costs;
  repi(i, 4) costs[i] = mo_order_cost(lrs, ords[i]);
  int j = ARGMAX(costs);
  return ords[j];
}

};

// add_l(l, r): 今の区間が [l+1, r) であるとき、l を追加して [l, r) にする
// add_r(l, r): 今の区間が [l, r) であるとき、r を追加して [l, r+1) にする
// del_l(l, r): 今の区間が [l, r) であるとき、l を削除して [l+1, r) にする
// del_r(l, r): 今の区間が [l, r+1) であるとき、r を削除して [l, r) にする
// rem(qid): 今が qid 番目の区間だとしてその部分の答えを確定させる
template <class I, class ADD_L, class ADD_R, class DEL_L, class DEL_R, class REM>
void mo(int n, const vc<pair<I, I>> &lrs, ADD_L add_l, ADD_R add_r, DEL_L del_l, DEL_R del_r, REM rem)
{
  fec([ l, r ] : lrs) { assert(0 <= l && l <= n && 0 <= r && r <= n); }
  vc<int> ord = internal::mo_order(n, lrs);
  cauto & [ ls, rs ] = top(lrs);
  int l = 0, r = 0;
  fe(i : ord)
  {
    while (ls[i] < l)
      add_l(--l, r);
    while (r < rs[i])
      add_r(l, r++);
    while (l < ls[i])
      del_l(l++, r);
    while (rs[i] < r)
      del_r(l, --r);
    rem(i);
  }
}

// add(i, isleft): 今の区間に i を追加する (isleft は左に追加するかどうか)
// del(i, isleft): 今の区間から i を削除する (isleft は左から削除するかどうか)
// rem(qid): 今が qid 番目の区間だとしてその部分の答えを確定させる
template <class I, class ADD, class DEL, class REM>
void mo(int n, const vc<pair<I, I>> &lrs, ADD add, DEL del, REM rem)
{
  auto add_l = [&](int l, int) { add(l, true); };
  auto add_r = [&](int, int r) { add(r, false); };
  auto del_l = [&](int l, int) { del(l, true); };
  auto del_r = [&](int, int r) { del(r, false); };
  mo(n, lrs, add_l, add_r, del_l, del_r, rem);
}

// slider は構造体内部に l, r を持ち、
//   lpp(), rpp(), lmm(), rmm() と、それを用いた set(nl, nr) を実装する
// rem(qid): 今が qid 番目の区間だとしてその部分の答えを確定させる
template <class I, class Slider, class REM>
void mo(int n, const vc<pair<I, I>> &lrs, Slider &slider, REM rem)
{
  fec([ l, r ] : lrs) { assert(0 <= l && r <= n); }
  vc<int> ord = internal::mo_order(n, lrs);
  fe(i : ord)
  {
    slider.set(lrs[i].first, lrs[i].second);
    rem(i);
  }
}
