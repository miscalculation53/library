#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../csr.hpp"

/**
 * @brief rollback 可能な Mo's algorithm
 * @docs docs/ds/mo_rollback.md
 */

// add_l(l, r): 今の区間が [l+1, r) であるとき、l を追加して [l, r) にする
// add_r(l, r): 今の区間が [l, r) であるとき、r を追加して [l, r+1) にする
// snapshot(): snapshot を撮る
// rollback(): snapshot を撮った状態に巻き戻す
// reset(): 初期状態に戻す
// rem(qid): 今が qid 番目の区間だとしてその部分の答えを確定させる
template <class I, class AddL, class AddR, class Snapshot, class Rollback, class Reset, class Rem>
void mo_rollback(int n, const vc<pair<I, I>> &lrs, const AddL &add_l, const AddR &add_r, const Snapshot &snapshot, const Rollback &rollback, const Reset &reset, const Rem &rem)
{
  fec([ l, r ] : lrs) { assert(0 <= l && l <= n && 0 <= r && r <= n); }
  cauto & [ ls, rs ] = unzip(lrs);
  const int q = lrs.size();
  const int b = max<int>(1, n / sqrt(q + 1));

  vc<pair<int, int>> ies;
  ies.reserve(q);
  repi(qid, q)
  {
    const int l = ls[qid], r = rs[qid], bid = l / b;
    if (r - l < b)
    {
      repi(i, l, r) add_r(l, i);
      rem(qid);
      reset();
    }
    else
      ies.eb(bid, qid);
  }
  CSR<int> csr(n / b, ies);

  repi(bid, n / b)
  {
    auto row = csr[bid];
    sort(ALL(row), [&](int i, int j)
         { return rs[i] < rs[j]; });
    int r = b * (bid + 1);
    fec(qid : row)
    {
      const int li = ls[qid], ri = rs[qid];
      while (r < ri)
        add_r(li, r++);
      snapshot();
      repi(l, b * (bid + 1) - 1, li - 1, -1) add_l(l, ri);
      rem(qid);
      rollback();
    }
    reset();
  }
}

// add(i, isleft): 今の区間に i を追加する (isleft は左に追加するかどうか)
// snapshot(): 今の状態の snapshot を撮る
// rollback(): 直前に snapshot を撮った状態に巻き戻す
// reset(): 初期状態に戻す
// rem(qid): 今が qid 番目の区間だとしてその部分の答えを確定させる
template <class I, class Add, class Snapshot, class Rollback, class Reset, class Rem>
void mo_rollback(int n, const vc<pair<I, I>> &lrs, const Add &add, const Snapshot &snapshot, const Rollback &rollback, const Reset &reset, const Rem &rem)
{
  cauto add_l = [&](int l, int) { add(l, true); };
  cauto add_r = [&](int, int r) { add(r, false); };
  mo_rollback(n, lrs, add_l, add_r, snapshot, rollback, reset, rem);
}
