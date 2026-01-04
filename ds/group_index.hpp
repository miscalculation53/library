#pragma once

#include "../template/template_all_but_modint.hpp"

#include "csr.hpp"

/**
 * @brief 添字を値で分類
 * @docs docs/ds/group_index.md
 */

template <class I = ll>
struct GroupIndex
{
private:
  int n, m;
  CSR<I> csr;

public:
  GroupIndex() {}
  template <class T>
  GroupIndex(const vc<T> &a) : n(a.size()), m(a.empty() ? 0 : MAX(a) + 1)
  {
    vc<pair<int, I>> ies(n);
    repi(i, n)
    {
      assert(0 <= a[i]);
      ies[i] = {a[i], i};
    }
    csr = CSR(m, ies);
  }

  // 値が val になる添字たち
  auto idxs(int val) const { return csr.at(val); }

  // 値が val になる添字のうち i 未満で最大のもの (なければ -1)
  I lt_max(int val, int i) const
  {
    auto is = idxs(val);
    ll j = ::lt_max(is, i);
    return j == -1 ? -1 : is[j];
  }
  // 値が val になる添字のうち i 以下で最大のもの (なければ -1)
  I leq_max(int val, int i) const
  {
    auto is = idxs(val);
    ll j = ::leq_max(is, i);
    return j == -1 ? -1 : is[j];
  }
  // 値が val になる添字のうち i 超過で最小のもの (なければ n)
  I gt_min(int val, int i) const
  {
    auto is = idxs(val);
    ll j = ::gt_min(is, i);
    return j == is.size() ? n : is[j];
  }
  // 値が val になる添字のうち i 以上で最小のもの (なければ n)
  I geq_min(int val, int i) const
  {
    auto is = idxs(val);
    ll j = ::geq_min(is, i);
    return j == is.size() ? n : is[j];
  }
  // 値が val になる i 未満の添字の個数
  // i 番目が val のとき、「これは何番目の val か？」に一致
  I lt_cnt(int val, int i) const { return ::lt_cnt(idxs(val), i); }
  // 値が val になる i 以下の添字の個数
  I leq_cnt(int val, int i) const { return ::leq_cnt(idxs(val), i); }
  // 値が val になる i 超過の添字の個数
  I gt_cnt(int val, int i) const { return ::gt_cnt(idxs(val), i); }
  // 値が val になる i 以上の添字の個数
  I geq_cnt(int val, int i) const { return ::geq_cnt(idxs(val), i); }
  // 値が val になる [l, r) の添字の個数
  I in_cnt(int val, int l, int r) const { return ::in_cnt(idxs(val), l, r); }

  // 値の種類数
  I num_of_distinct_values() const { return m; }

  auto &to_csr() const { return csr; }
  vvc<I> to_vv() const
  {
    auto res = csr.to_vv();
    vvc<I> res2(res.size());
    rep(i, res.size()) res2[i] = vc<I>(ALL(res[i]));
    return res2;
  }
};
