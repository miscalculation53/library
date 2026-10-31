#pragma once

#include "../template/template_all_but_modint.hpp"

#include "segtree/segtree.hpp"

/**
 * @brief 区間の交差判定（追加・削除）
 * @docs docs/ds/interval_crossing.md
 */

// 半開区間 [l, r) を管理する
// 交差は a < l < b < r または l < a < r < b
// 更新する区間を先読みし、入力順の id で追加・削除する
template <class T = ll>
struct IntervalCrossing
{
  struct Interval
  {
    int id;
    T l, r;
  };

private:
  int n, size_ = 0;
  vc<pair<T, T>> intervals;
  vc<int> by_left, by_right, left_pos, right_pos;
  vc<bool> active;
  SegmentTree<MonoidMax<int>> max_right;
  SegmentTree<MonoidMin<int>> min_left;

  const T &endpoint(int id, bool right) const
  {
    return right ? intervals[id].second : intervals[id].first;
  }
  // order に並ぶ端点のうち l < x < r の範囲。
  pair<int, int> inside(const vc<int> &order, bool right, const T &l, const T &r) const
  {
    const int a = upper_bound(ALL(order), l, [&](const T &x, int id)
                              { return x < endpoint(id, right); }) - order.begin();
    const int b = lower_bound(ALL(order), r, [&](int id, const T &x)
                              { return endpoint(id, right) < x; }) - order.begin();
    return {a, b};
  }
  int find_crossing_id(const T &l, const T &r) const
  {
    assert(!(r < l));
    if (!(l < r)) return -1;
    const auto [a, b] = inside(by_left, false, l, r);
    const int rp = max_right.prod(a, b);
    if (rp >= 0 && r < intervals[by_right[rp]].second) return by_right[rp];
    const auto [c, d] = inside(by_right, true, l, r);
    const int lp = min_left.prod(c, d);
    if (lp < n && intervals[by_left[lp]].first < l) return by_left[lp];
    return -1;
  }

public:
  IntervalCrossing() : IntervalCrossing(vc<pair<T, T>>{}) {}
  explicit IntervalCrossing(const vc<pair<T, T>> &intervals)
      : n(intervals.size()), intervals(intervals), by_left(n), by_right(n),
        left_pos(n), right_pos(n), active(n, false), max_right(n), min_left(n)
  {
    for (const auto &[l, r] : intervals) assert(!(r < l));
    iota(ALL(by_left), 0);
    iota(ALL(by_right), 0);
    sort(ALL(by_left), [&](int i, int j) { return intervals[i].first < intervals[j].first; });
    sort(ALL(by_right), [&](int i, int j) { return intervals[i].second < intervals[j].second; });
    repi(i, n)
    {
      left_pos[by_left[i]] = i;
      right_pos[by_right[i]] = i;
    }
  }

  // 交差する登録区間の {id, l, r}
  // 該当する区間がなければ nullopt
  optional<Interval> find_crossing(const T &l, const T &r) const
  {
    const int id = find_crossing_id(l, r);
    if (id == -1) return nullopt;
    return Interval{id, intervals[id].first, intervals[id].second};
  }
  // 交差する登録区間の {id, l, r}
  // 該当する区間がなければ nullopt
  // id は登録中でもよい (自身との交差は生じないので)
  optional<Interval> find_crossing(int id) const
  {
    assert(0 <= id && id < n);
    return find_crossing(intervals[id].first, intervals[id].second);
  }
  // 現在の区間族がラミナーであるとして、追加後もラミナーであるか
  bool can_insert(const T &l, const T &r) const { return find_crossing_id(l, r) == -1; }
  // 現在の区間族がラミナーであるとして、追加後もラミナーであるか
  bool can_insert(int id) const
  {
    assert(0 <= id && id < n);
    return can_insert(intervals[id].first, intervals[id].second);
  }

  // 交差を含む状態も保持できる。ラミナー性を保つときは can_insert で判定する
  void insert(int id)
  {
    assert(0 <= id && id < n && !active[id]);
    active[id] = true;
    ++size_;
    max_right.set(left_pos[id], right_pos[id]);
    min_left.set(right_pos[id], left_pos[id]);
  }
  void erase(int id)
  {
    assert(0 <= id && id < n && active[id]);
    active[id] = false;
    --size_;
    max_right.set(left_pos[id], MonoidMax<int>::e());
    min_left.set(right_pos[id], MonoidMin<int>::e());
  }

  bool contains(int id) const
  {
    assert(0 <= id && id < n);
    return active[id];
  }
  int size() const { return size_; }
  bool empty() const { return size_ == 0; }
  vc<int> content() const
  {
    vc<int> ids;
    ids.reserve(size_);
    repi(i, n) if (active[i]) ids.eb(i);
    return ids;
  }
};
