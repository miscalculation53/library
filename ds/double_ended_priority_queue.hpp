#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief double ended priority queue
 * @docs docs/ds/double_ended_priority_queue.md
 */

#include "erasable_priority_queue.hpp"
#include "../utils/reverse_compare.hpp"

template <class T, class Compare = less<T>>
struct DoubleEndedPriorityQueue
{
  using value_type = T;
  using compare_type = Compare;

private:
  ErasablePriorityQueue<T, Compare> mn;
  ErasablePriorityQueue<T, reverse_compare_t<Compare>> mx;

public:
  DoubleEndedPriorityQueue() : DoubleEndedPriorityQueue(Compare()) {}
  explicit DoubleEndedPriorityQueue(const Compare &comp) : mn(comp), mx(reverse_compare(comp)) {}
  template <class It>
  DoubleEndedPriorityQueue(It begi, It endi, const Compare &comp = Compare())
      : mn(begi, endi, comp), mx(begi, endi, reverse_compare(comp)) {}
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
  template <class I = ll>
  I size() const { return mn.template size<I>(); }
  T get_min() const { return mn.top(); }
  T get_max() const { return mx.top(); }
  // x が存在することをユーザが保証する。
  void erase(const T &x) { mn.erase(x), mx.erase(x); }
  void clear() { mn.clear(), mx.clear(); }

  vc<T> content() const { return mn.content(); }
};
