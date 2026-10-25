#pragma once

#include "common.hpp"

namespace priority_container_detail
{
template <class T, class Compare, class Allocator>
struct Access<multiset<T, Compare, Allocator>>
{
  using C = multiset<T, Compare, Allocator>;
  static constexpr bool can_erase = true, can_contains = true, can_front = true, can_back = true;
  static T front(const C &c) { return *c.begin(); }
  static T back(const C &c) { return *c.rbegin(); }
  static void push(C &c, const T &x) { c.insert(x); }
  static void pop_front(C &c) { c.erase(c.begin()); }
  static void pop_back(C &c) { c.erase(prev(c.end())); }
  static bool contains(const C &c, const T &x) { return c.find(x) != c.end(); }
  static void erase(C &c, const T &x)
  {
    auto it = c.find(x);
    assert(it != c.end());
    c.erase(it);
  }
  static void clear(C &c) { c.clear(); }
  template <class Comp>
  static C make(const Comp &comp) { return C(comp); }
};

} // namespace priority_container_detail
