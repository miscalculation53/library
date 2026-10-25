#pragma once

#include "../template/template_all_but_modint.hpp"

#include "flat_map.hpp"

#include "uf/uf_undo.hpp"

/**
 * @brief オフラインダイコネのテクニック
 * @docs docs/ds/offline_dynamic_connectivity.md
 */

// 追加・削除を時刻の昇順に登録する。同じオブジェクトを複数個追加でき、erase_if は直近の追加を (あれば) 1 個分削除する
// 注: 同じオブジェクトが同じとみなされるようにすること!! ((u, v) と (v, u) など)
// odc.run(add, undo, run) に渡すもの:
// - add(Obj obj): 現在の状態にオブジェクト obj を追加する処理
// - undo(): 追加処理を 1 個分戻す処理
// - run(int t): 時刻 0 <= t < tmax について順に行う処理
// 計算量: 生存区間数を m として add, undo を O(m log(tmax)) 回、run を tmax 回
template <class Obj>
struct OfflineDynamicConnectivity
{
private:
  template <class T, class = void>
  struct HasLess : false_type {};
  template <class T>
  struct HasLess<T, void_t<decltype(bool(declval<const T &>() < declval<const T &>()))>> : true_type {};

  struct Operation
  {
    int time;
    bool is_add;
    Obj obj;
    Operation(int time, bool is_add, const Obj &obj) : time(time), is_add(is_add), obj(obj) {}
  };
  int tmax, last_time = 0;
  vc<Operation> operations;
  vc<tuple<int, int, Obj>> span_obj;

  void check_time(int t)
  {
    assert(last_time <= t && t <= tmax);
    last_time = t;
  }

  template <class Add, class Undo, class Run>
  void run_spans(const vc<tuple<int, int, Obj>> &source, Add &add, Undo &undo, Run &run) const
  {
    assert(tmax >= 0);
    fec([l, r, x] : source) assert(0 <= l && l <= r && r <= tmax);
    if (tmax == 0)
      return;
    const int tmax_ceil = bit_ceil(tmax);
    vvc<Obj> nodes(tmax_ceil + tmax);
    fec([l, r, x] : source)
    {
      if (l == r)
        continue;
      // 右端の余白には query がないので、最後まで残る区間を延ばせる。
      int li = int(l) + tmax_ceil;
      int ri = (r == tmax ? tmax_ceil : int(r)) + tmax_ceil;
      while (li < ri)
      {
        if (li & 1)
          nodes[li++].eb(x);
        if (ri & 1)
          nodes[--ri].eb(x);
        li >>= 1, ri >>= 1;
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

public:
  explicit OfflineDynamicConnectivity(int tmax) : tmax(tmax) { assert(tmax >= 0); }

  // 既存の生存区間を受け取る。時刻が int なら vector ごと移動する。
  template <class I = int>
  OfflineDynamicConnectivity(int tmax, vc<tuple<I, I, Obj>> spans) : tmax(tmax)
  {
    assert(tmax >= 0);
    fec([l, r, obj] : spans) assert(0 <= l && l <= r && r <= tmax);
    if constexpr (is_same_v<I, int>)
      span_obj = std::move(spans);
    else
    {
      span_obj.reserve(spans.size());
      fem([l, r, obj] : spans) span_obj.eb(int(l), int(r), std::move_if_noexcept(obj));
    }
  }

  // 時刻 t から obj を 1 個追加する操作を登録する。
  void add(int t, const Obj &obj)
  {
    check_time(t);
    static_assert(HasLess<Obj>::value, "add requires Obj comparison with <");
    operations.eb(t, true, obj);
  }

  // 時刻 t から obj が存在すれば 1 個削除する操作を登録する。
  void erase_if(int t, const Obj &obj)
  {
    check_time(t);
    static_assert(HasLess<Obj>::value, "erase_if requires Obj comparison with <");
    operations.eb(t, false, obj);
  }

  // 生存期間 [l, r) の obj を 1 個登録する。add/erase_if とは独立に扱う。
  void add_span(int l, int r, const Obj &obj)
  {
    assert(0 <= l && l <= r && r <= tmax);
    if (l < r)
      span_obj.eb(l, r, obj);
  }

  // 対象ごとに登録順を保って操作をまとめ、生存期間に変換する。
  vc<tuple<int, int, Obj>> spans() const
  {
    vc<tuple<int, int, Obj>> res;
    res.reserve(span_obj.size());
    fec(span : span_obj) if (get<0>(span) < get<1>(span)) res.eb(span);
    if constexpr (HasLess<Obj>::value)
    {
      vc<int> order = permid<int>(operations.size());
      stable_sort(ALL(order), [&](int i, int j) { return operations[i].obj < operations[j].obj; });
      vc<int> starts;
      for (int i = 0, j; i < SZ<int>(order); i = j)
      {
        j = i + 1;
        while (j < SZ<int>(order) && !(operations[order[i]].obj < operations[order[j]].obj)) j++;
        int representative = -1;
        starts.clear();
        repi(k, i, j)
        {
          int id = order[k];
          const auto &op = operations[id];
          if (op.is_add)
          {
            if (starts.empty()) representative = id;
            starts.eb(op.time);
          }
          else if (!starts.empty())
          {
            int l = starts.back();
            starts.pop_back();
            if (l < op.time) res.eb(l, op.time, operations[representative].obj);
          }
        }
        fec(l : starts) if (l < tmax) res.eb(l, tmax, operations[representative].obj);
      }
    }
    return res;
  }

  // add(obj): 状態へ対象を反映する。undo(): 直前の反映を 1 回分戻す。
  // run(t): 時刻 t の状態から答えを取得する。
  template <class Add, class Undo, class Run>
  void run(Add add, Undo undo, Run run) const
  {
    if (operations.empty())
      run_spans(span_obj, add, undo, run);
    else
      run_spans(spans(), add, undo, run);
  }
};

// 「ないなら追加、あるなら削除」でクエリが与えられる場合、生存期間の形に変換する
// time_obj は (time, obj) を並べた vector
// 注: 同じオブジェクトが同じとみなされるようにすること!! ((u, v) と (v, u) など)
template <class I = ll, class Time, class Obj>
vc<tuple<I, I, Obj>> add_del_to_span(int tmax, const vc<pair<Time, Obj>> &time_obj)
{
  assert(tmax >= 0);
  fec([time, obj] : time_obj) assert(0 <= time && time <= tmax);
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
