#pragma once

#include "common.hpp"

/**
 * @brief 両端への挿入で順序を保つ優先度付き deque
 * @docs docs/ds/priority_container/monotone_deque.md
 */

// 毎回の追加値が現在の両端のどちらかに置ける場合に使う。
template <class T, class Compare = less<T>>
struct MonotonePriorityDeque
{
  using value_type = T;

private:
  deque<T> data;
  Compare comp;

public:
  MonotonePriorityDeque() = default;
  explicit MonotonePriorityDeque(const Compare &comp) : comp(comp) {}
  int size() const { return data.size(); }
  bool empty() const { return data.empty(); }
  T front() const { assert(!empty()); return data.front(); }
  T back() const { assert(!empty()); return data.back(); }
  void push(const T &x)
  {
    if (empty() || !comp(x, data.back())) data.push_back(x);
    else
    {
      assert(!comp(data.front(), x));
      data.push_front(x);
    }
  }
  void pop_front() { assert(!empty()); data.pop_front(); }
  void pop_back() { assert(!empty()); data.pop_back(); }
  void clear() { data.clear(); }
};
