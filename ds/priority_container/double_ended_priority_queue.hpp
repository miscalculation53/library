#pragma once

#include "../double_ended_priority_queue.hpp"
#include "common.hpp"

namespace priority_container_detail
{
template <class T, class Compare>
struct Access<DoubleEndedPriorityQueue<T, Compare>>
{
  using C = DoubleEndedPriorityQueue<T, Compare>;
  static constexpr bool can_erase = true, can_contains = false, can_front = true, can_back = true;
  static T front(const C &c) { return c.get_min(); }
  static T back(const C &c) { return c.get_max(); }
  static void push(C &c, const T &x) { c.push(x); }
  static void pop_front(C &c) { c.extract_min(); }
  static void pop_back(C &c) { c.extract_max(); }
  static void erase(C &c, const T &x) { c.erase(x); }
  static void clear(C &c) { c.clear(); }
  template <class Comp>
  static C make(const Comp &comp) { return C(comp); }
};
} // namespace priority_container_detail
