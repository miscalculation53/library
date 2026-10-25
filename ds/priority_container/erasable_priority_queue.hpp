#pragma once

#include "../erasable_priority_queue.hpp"
#include "common.hpp"

template <class T, class Compare = less<T>>
using MinErasablePriorityQueue = priority_container_detail::SingleEndedQueue<
    ErasablePriorityQueue<T, Compare>, Compare, false, false>;
template <class T, class Compare = less<T>>
using MaxErasablePriorityQueue = priority_container_detail::SingleEndedQueue<
    ErasablePriorityQueue<T, reverse_compare_t<Compare>>, Compare, true, true>;

namespace priority_container_detail
{
template <class T, class Compare>
struct Access<ErasablePriorityQueue<T, Compare>>
{
  using C = ErasablePriorityQueue<T, Compare>;
  static constexpr bool can_erase = true, can_contains = false, can_front = true, can_back = false;
  static T front(const C &c) { return c.top(); }
  static void push(C &c, const T &x) { c.push(x); }
  static void pop_front(C &c) { c.pop(); }
  static void erase(C &c, const T &x) { c.erase(x); }
  static void clear(C &c) { c.clear(); }
  template <class Comp>
  static C make(const Comp &comp) { return C(comp); }
};

template <class T, class RawCompare, class Compare, bool Back>
struct ContainerForEnd<ErasablePriorityQueue<T, RawCompare>, Compare, Back>
{
  using type = conditional_t<Back, MaxErasablePriorityQueue<T, Compare>, MinErasablePriorityQueue<T, Compare>>;
};
} // namespace priority_container_detail
