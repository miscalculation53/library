#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 自作 queue
 * @docs docs/ds/my_queue.md
 */

template <class T>
struct MyQueue
{
private:
  vc<T> d;
  int pos = 0;

public:
  void reserve(int n) { d.reserve(n); }
  template <class I = ll>
  I size() const { return SZ<I>(d) - pos; }
  bool empty() const { return pos == SZ<int>(d); }
  void push(const T &t) { d.eb(t); }
  T front() const { return d[pos]; }
  T &front() { return d[pos]; }
  void clear()
  {
    d.clear();
    pos = 0;
  }
  void pop() { pos++; }
  T operator[](int i) const { return d[pos + i]; }
  T &operator[](int i) { return d[pos + i]; }
  T at(int i) const
  {
    assert(0 <= i && i < size<int>());
    return d[pos + i];
  }
  T &at(int i)
  {
    assert(0 <= i && i < size<int>());
    return d[pos + i];
  }

  vc<T> content() { return {d.begin() + pos, d.end()}; }
};
