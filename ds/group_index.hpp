#pragma once

#include "../template/template_all_but_modint.hpp"

#include "csr.hpp"

/**
 * @brief 添字を値で分類
 * @docs docs/ds/group_index.md
 */

template <class T = ll, bool compress = true, class I = ll>
struct GroupIndex
{
private:
  int n = 0;
  vc<T> vals;
  CSR<I> csr{vc<int>{}};

  int row_id(const T &val) const
  {
    if constexpr (compress)
    {
      auto it = lower_bound(ALL(vals), val);
      return it == vals.end() || *it != val ? -1 : int(it - vals.begin());
    }
    else
    {
      using U = common_type_t<T, int>;
      return 0 <= val && U(val) < U(csr.size()) ? int(val) : -1;
    }
  }

public:
  GroupIndex() = default;
  GroupIndex(const vc<T> &a) : n(a.size())
  {
    int m;
    vc<pair<int, I>> ies(n);
    if constexpr (compress)
    {
      vals = sortuniqued(a);
      m = vals.size();
      repi(i, n) ies[i] = {row_id(a[i]), i};
    }
    else
    {
      static_assert(is_integral_v<T>);
      m = a.empty() ? 0 : int(MAX(a)) + 1;
      repi(i, n)
      {
        assert(0 <= a[i]);
        ies[i] = {int(a[i]), i};
      }
    }
    csr = CSR<I>(m, ies);
    if constexpr (!compress)
      repi(val, m) if (!csr[val].empty()) vals.eb(T(val));
  }

  // 値が val になる添字たち
  auto idxs(const T &val) const { return csr.at(row_id(val)); }

  // 値が val になる添字のうち i 未満で最大のもの (なければ -1)
  I lt_max(const T &val, int i) const
  {
    auto is = idxs(val);
    ll j = ::lt_max(is, i);
    return j == -1 ? -1 : is[j];
  }
  // 値が val になる添字のうち i 以下で最大のもの (なければ -1)
  I leq_max(const T &val, int i) const
  {
    auto is = idxs(val);
    ll j = ::leq_max(is, i);
    return j == -1 ? -1 : is[j];
  }
  // 値が val になる添字のうち i 超過で最小のもの (なければ n)
  I gt_min(const T &val, int i) const
  {
    auto is = idxs(val);
    ll j = ::gt_min(is, i);
    return j == is.size() ? n : is[j];
  }
  // 値が val になる添字のうち i 以上で最小のもの (なければ n)
  I geq_min(const T &val, int i) const
  {
    auto is = idxs(val);
    ll j = ::geq_min(is, i);
    return j == is.size() ? n : is[j];
  }
  // 値が val になる i 未満の添字の個数
  // i 番目が val のとき、「これは何番目の val か？」に一致
  I lt_cnt(const T &val, int i) const { return ::lt_cnt(idxs(val), i); }
  // 値が val になる i 以下の添字の個数
  I leq_cnt(const T &val, int i) const { return ::leq_cnt(idxs(val), i); }
  // 値が val になる i 超過の添字の個数
  I gt_cnt(const T &val, int i) const { return ::gt_cnt(idxs(val), i); }
  // 値が val になる i 以上の添字の個数
  I geq_cnt(const T &val, int i) const { return ::geq_cnt(idxs(val), i); }
  // 値が val になる [l, r) の添字の個数
  I in_cnt(const T &val, int l, int r) const { return ::in_cnt(idxs(val), l, r); }

  // 出現する値の種類数
  I size() const { return vals.size(); }
  bool empty() const { return vals.empty(); }

  struct Iterator
  {
    using iterator_category = input_iterator_tag;
    using value_type = pair<T, decltype(declval<const CSR<I> &>().at(0))>;
    using difference_type = ptrdiff_t;
    using reference = value_type;
    using pointer = void;

  private:
    const GroupIndex *grp = nullptr;
    int i = 0;

  public:
    Iterator() = default;
    Iterator(const GroupIndex *grp, int i) : grp(grp), i(i) {}
    value_type operator*() const
    {
      int row = i;
      if constexpr (!compress) row = int(grp->vals[i]);
      return {grp->vals[i], grp->csr[row]};
    }
    Iterator &operator++()
    {
      ++i;
      return *this;
    }
    Iterator operator++(int)
    {
      auto res = *this;
      ++*this;
      return res;
    }
    bool operator==(const Iterator &other) const { return grp == other.grp && i == other.i; }
    bool operator!=(const Iterator &other) const { return !(*this == other); }
  };
  Iterator begin() const { return Iterator(this, 0); }
  Iterator end() const { return Iterator(this, vals.size()); }

  auto &to_csr() const { return csr; }
  vvc<I> to_vv() const { return csr.to_vv(); }
};

// 値の型をコンストラクタの引数から推論できる非圧縮版
template <class T = ll, class I = ll>
struct GroupIndexRaw : GroupIndex<T, false, I>
{
  GroupIndexRaw() = default;
  GroupIndexRaw(const vc<T> &a) : GroupIndex<T, false, I>(a) {}
};
