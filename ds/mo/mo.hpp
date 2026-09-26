#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "../../algo/radix_sort.hpp"

/**
 * @brief Mo's algorithm
 * @docs docs/ds/mo/mo.md
 */

namespace internal
{

template <class I>
ll mo_order_cost(const vc<pair<I, I>> &lrs, const vc<int> &ord)
{
  ll res = 0;
  repi(i, SZ(lrs) - 1)
  {
    res += abs(ll(lrs[ord[i + 1]].first) - ll(lrs[ord[i]].first));
    res += abs(ll(lrs[ord[i + 1]].second) - ll(lrs[ord[i]].second));
  }
  return res;
}

template <class I>
vc<int> mo_order_params(const vc<pair<I, I>> &lrs, const vc<int> &by_right, int n, int b, int t)
{
  const int q = lrs.size();
  const ll offset = ll(t) * b / 2;
  const int blocks = (n + offset) / b + 1;
  vc<int> block(q), cursor(blocks), ord(q);
  repi(i, q)
    cursor[block[i] = (ll(lrs[i].first) + offset) / b]++;
  int sum = 0;
  repi(k, blocks)
  {
    int count = cursor[k];
    cursor[k] = sum + ((k & 1) ? count : 0);
    sum += count;
  }
  // 偶数ブロックは右端の昇順、奇数ブロックは降順に配置する。
  for (int i : by_right)
  {
    int k = block[i];
    ord[(k & 1) ? --cursor[k] : cursor[k]++] = i;
  }
  return ord;
}

template <class I>
vc<int> mo_order(const vc<pair<I, I>> &lrs)
{
  const int q = lrs.size();
  if (q <= 1)
    return permid<int>(q);
  int n = 0;
  fec([l, r] : lrs) chmax(n, int(max(l, r)));
  // 右端の整列を 6 通りの候補で共用する。
  auto by_right = radix_argsort(lrs, [](const auto &lr) { return lr.second; });
  const int b1 = max(1, int(n / sqrt(q + 1.0)));
  const int b2 = max(1, int(sqrt(3) * n / sqrt(2.0 * q + 1)));
  const int b3 = max(1, int(sqrt(2) * n / sqrt(q + 1.0)));
  vc<int> best;
  ll best_cost = LLONG_MAX;
  for (int b : {b1, b2, b3})
    for (int t : {0, 1})
    {
      auto ord = mo_order_params(lrs, by_right, n, b, t);
      ll cost = mo_order_cost(lrs, ord);
      if (cost < best_cost)
        best_cost = cost, best = move(ord);
    }
  return best;
}

};

// add_l(l, r): 今の区間が [l+1, r) であるとき、l を追加して [l, r) にする
// add_r(l, r): 今の区間が [l, r) であるとき、r を追加して [l, r+1) にする
// del_l(l, r): 今の区間が [l, r) であるとき、l を削除して [l+1, r) にする
// del_r(l, r): 今の区間が [l, r+1) であるとき、r を削除して [l, r) にする
// rem(qid): 今が qid 番目の区間だとしてその部分の答えを確定させる
template <class I, class AddL, class AddR, class DelL, class DelR, class Rem>
void mo(int n, const vc<pair<I, I>> &lrs, const AddL &add_l, const AddR &add_r, const DelL &del_l, const DelR &del_r, const Rem &rem)
{
  fec([ l, r ] : lrs) { assert(0 <= l && l <= n && 0 <= r && r <= n); }
  vc<int> ord = internal::mo_order(lrs);
  int l = 0, r = 0;
  fe(i : ord)
  {
    const int li = lrs[i].first, ri = lrs[i].second;
    while (li < l)
      add_l(--l, r);
    while (r < ri)
      add_r(l, r++);
    while (l < li)
      del_l(l++, r);
    while (ri < r)
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
  vc<int> ord = internal::mo_order(lrs);
  fe(i : ord)
  {
    slider.set(lrs[i].first, lrs[i].second);
    rem(i);
  }
}
