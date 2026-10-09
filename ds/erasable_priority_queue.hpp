#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 消せる priority queue
 * @docs docs/ds/erasable_priority_queue.md
 */

// Compare は、less が昇順で greater が降順
template <class T, class Compare = less<T>>
struct ErasablePriorityQueue
{
  using value_type = T;
  using compare_type = Compare;

private:
  struct ReverseCompare
  {
    Compare comp;
    bool operator()(const T &a, const T &b) const { return comp(b, a); }
  };
  Compare comp;
  priority_queue<T, vc<T>, ReverseCompare> body, era;
  void normalize()
  {
    while (!era.empty() && body.top() == era.top())
      body.pop(), era.pop();
  }

public:
  ErasablePriorityQueue() : ErasablePriorityQueue(Compare()) {}
  explicit ErasablePriorityQueue(const Compare &comp)
      : comp(comp), body(ReverseCompare{comp}), era(ReverseCompare{comp}) {}
  template <class It>
  ErasablePriorityQueue(It begi, It endi, const Compare &comp = Compare())
      : comp(comp), body(begi, endi, ReverseCompare{comp}), era(ReverseCompare{comp}) {}
  void push(const T &x)
  {
    body.push(x);
    normalize();
  }
  void pop()
  {
    assert(!empty());
    body.pop();
    normalize();
  }
  // x が存在することをユーザが保証する
  void erase(const T &x)
  {
    era.push(x);
    normalize();
  }
  bool empty() const { return body.empty(); }
  template <class I = ll>
  I size() const { return body.size() - era.size(); }
  T top() const { assert(!empty()); return body.top(); }
  void clear()
  {
    body = decltype(body)(ReverseCompare{comp});
    era = decltype(era)(ReverseCompare{comp});
  }

  vc<T> content() const
  {
    auto body2 = body, era2 = era;
    vc<T> res;
    while (!body2.empty())
    {
      if (!era2.empty() && body2.top() == era2.top())
        body2.pop(), era2.pop();
      else
        res.eb(body2.top()), body2.pop();
    }
    return res;
  }
};
