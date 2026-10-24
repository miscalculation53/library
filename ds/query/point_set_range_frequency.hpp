#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../fenwick_tree/fenwick_tree_01.hpp"
#include "../csr.hpp"
#include "../../algo/stable_argsort.hpp"

/**
 * @brief 一点更新・区間頻度（オフライン）
 * @docs docs/ds/query/point_set_range_frequency.md
 */

template <class T>
struct PointSetRangeFrequency
{
private:
  int n = 0;
  using P = pair<int, T>;
  using Q = tuple<int, int, T>;
  vc<variant<P, Q>> qs;
  vc<T> vs;

public:
  PointSetRangeFrequency() {}
  PointSetRangeFrequency(const vc<T> &a) : n(a.size()), vs(a) {}

  // p 番目を v にするクエリを追加する
  void point_set(int p, const T &v)
  {
    assert(0 <= p && p < n);
    qs.eb(P(p, v));
    vs.eb(v);
  }
  // [l, r) での x の頻度を答えるクエリを追加する
  void range_frequency(int l, int r, const T &x)
  {
    assert(0 <= l && l <= r && r <= n);
    qs.eb(Q(l, r, x));
  }

  // クエリにまとめて答える
  template <class I = ll>
  vc<I> run()
  {
    const int m = vs.size();
    auto order = stable_argsort(vs);
    vc<T> values;
    vc<int> ids(m);
    values.reserve(m);
    int prev = -1;
    for (int i : order)
    {
      if (prev == -1 || vs[prev] != vs[i]) values.eb(vs[i]);
      ids[i] = values.size() - 1;
      prev = i;
    }
    vc<pair<int, int>> entries(m);
    repi(p, n) entries[p] = {ids[p], p};
    int k = n;
    fec(q : qs)
      if (holds_alternative<P>(q))
      {
        cauto & [ p, v ] = get<P>(q);
        entries[k] = {ids[k], p};
        k++;
      }
    order = stable_argsort(entries, [](const auto &e)
    {
      return (uint64_t(e.first) << 32) | uint32_t(e.second);
    });
    vc<int> counts(values.size()), positions, handles(m);
    positions.reserve(m);
    prev = -1;
    for (int i : order)
    {
      if (prev == -1 || entries[prev] != entries[i])
      {
        counts[entries[i].first]++;
        positions.eb(entries[i].second);
      }
      handles[i] = positions.size() - 1;
      prev = i;
    }
    CSR<int> csr(counts);
    csr.get_elist() = move(positions);
    // 値ごとの添字列を平坦につなぎ、1本の01列として管理する。
    vc<bool> initial(csr.get_elist().size());
    vc<int> current(handles.begin(), handles.begin() + n);
    for (int h : current) initial[h] = true;
    FenwickTree01<int> fw(initial);
    vc<I> res;
    res.reserve(qs.size() - (m - n));
    k = n;
    fec(q : qs)
    {
      if (holds_alternative<P>(q))
      {
        cauto & [ p, v ] = get<P>(q);
        fw.set(current[p], false);
        current[p] = handles[k++];
        fw.set(current[p], true);
      }
      else if (holds_alternative<Q>(q))
      {
        cauto & [ l, r, x ] = get<Q>(q);
        const int j = LB(values, x);
        if (j == SZ(values) || values[j] != x)
        {
          res.eb(0);
          continue;
        }
        const int li = LB(csr[j], l), ri = LB(csr[j], r);
        res.eb(fw.sum(csr.offset(j) + li, csr.offset(j) + ri));
      }
    }
    return res;
  }
};
