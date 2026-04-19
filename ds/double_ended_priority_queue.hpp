#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief double ended priority queue
 * @docs docs/ds/double_ended_priority_queue.md
 */

#include "erasable_priority_queue.hpp"

template <class T>
struct DoubleEndedPriorityQueue
{
private:
  ErasablePriorityQueue<T> mn;
  ErasablePriorityQueue<T, greater<T>> mx;

public:
  DoubleEndedPriorityQueue() {}
  template <class It>
  DoubleEndedPriorityQueue(It begi, It endi) : mn(begi, endi), mx(begi, endi) {}
  void push(const T &x) { mn.push(x), mx.push(x); }
  T extract_min()
  {
    T x = mn.top();
    mn.pop(), mx.erase(x);
    return x;
  }
  T extract_max()
  {
    T x = mx.top();
    mx.pop(), mn.erase(x);
    return x;
  }
  bool empty() const { return mn.empty(); }
  T get_min() const { return mn.top(); }
  T get_max() const { return mx.top(); }

  vc<T> content() const { return mn.content(); }
};
