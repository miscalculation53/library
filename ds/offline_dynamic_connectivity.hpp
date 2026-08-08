#pragma once

#include "../template/template_all_but_modint.hpp"

#include "flat_map.hpp"

#include "uf/uf_undo.hpp"

/**
 * @brief オフラインダイコネのテクニック
 * @docs docs/ds/offline_dynamic_connectivity.md
 */

// 時系列上の「追加」「削除」クエリを、「追加」「undo」クエリに置き換えて処理する
// tmax: 時刻の範囲が [0, tmax)
// span_obj: (l, r, obj) の配列
//   - 生存期間が [l, r) のオブジェクト obj
//   - 時刻 l では存在する、r では存在しない
// add(Obj obj): 現在の状態にオブジェクト obj を追加する処理
// undo(): 追加処理を 1 個分戻す処理
// run(int t): 時刻 0 <= t < tmax について順に行う処理
// 計算量: add, undo, run を O(tmax + |span_obj| log(tmax)) 回
template <class Obj, class I, class Add, class Undo, class Run>
void offline_dynamic_connectivity(int tmax, const vc<tuple<I, I, Obj>> &span_obj, Add add, Undo undo, Run run)
{
  assert(tmax >= 0);
  fec([l, r, x] : span_obj) assert(0 <= l && l <= r && r <= tmax);
  if (tmax == 0)
    return;
  const int tmax_ceil = bit_ceil(tmax);
  vvc<Obj> nodes(tmax_ceil + tmax);
  fe([l, r, x] : span_obj)
  {
    l += tmax_ceil, r += tmax_ceil;
    while (l < r)
    {
      if (l & 1)
        nodes[l++].eb(x);
      if (r & 1)
        nodes[--r].eb(x);
      l >>= 1, r >>= 1;
    }
  }
  auto dfs = [&](auto dfs, int i) -> void
  {
    fec(x : nodes[i]) add(x);
    repi(ni, 2 * i, 2 * i + 2)
    {
      if (ni < tmax_ceil + tmax)
        dfs(dfs, ni);
    }
    if (0 <= i - tmax_ceil && i - tmax_ceil < tmax)
      run(i - tmax_ceil);
    repi(_, nodes[i].size()) undo();
  };
  dfs(dfs, 1);
}

// 「ないなら追加、あるなら削除」でクエリが与えられる場合、生存期間の形に変換する
// time_obj は (time, obj) を並べた vector
// 注: 同じオブジェクトが同じとみなされるようにすること!! ((u, v) と (v, u) など)
template <class I = ll, class Time, class Obj>
vc<tuple<I, I, Obj>> add_del_to_span(int tmax, const vc<pair<Time, Obj>> &time_obj)
{
  FlatMap<Obj, vc<Time>> mp(unzip(time_obj).second);
  fec([ time, obj ] : sorted(time_obj)) mp[obj].eb(time);
  vc<tuple<I, I, Obj>> res;
  fec([obj, times] : mp)
  {
    repi(i, 0, times.size(), 2)
    {
      int l = times[i];
      int r = i + 1 < SZ<int>(times) ? times[i + 1] : tmax;
      res.eb(l, r, obj);
    }
  }
  return res;
}
