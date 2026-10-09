#pragma once

#include "common.hpp"

template <class T, class Compare = less<T>, class Sequence = vector<T>>
using MinPriorityQueue = priority_container_detail::SingleEndedQueue<
    priority_queue<T, Sequence, reverse_compare_t<Compare>>, Compare, false, true>;
template <class T, class Compare = less<T>, class Sequence = vector<T>>
using MaxPriorityQueue = priority_container_detail::SingleEndedQueue<
    priority_queue<T, Sequence, Compare>, Compare, true, false>;

namespace priority_container_detail
{
template <class T, class Sequence, class Compare>
struct Access<priority_queue<T, Sequence, Compare>>
{
  using C = priority_queue<T, Sequence, Compare>;
  static constexpr bool can_erase = false, can_contains = false, can_front = false, can_back = true;
  static T back(const C &c) { return c.top(); }
  static void push(C &c, const T &x) { c.push(x); }
  static void pop_back(C &c) { c.pop(); }
  static void clear(C &c) { while (!c.empty()) c.pop(); }
  template <class Comp>
  static C make(const Comp &comp) { return C(comp); }
};

template <class T, class Sequence, class RawCompare, class Compare, bool Back>
struct ContainerForEnd<priority_queue<T, Sequence, RawCompare>, Compare, Back>
{
  using type = conditional_t<Back, MaxPriorityQueue<T, Compare, Sequence>, MinPriorityQueue<T, Compare, Sequence>>;
};

} // namespace priority_container_detail
