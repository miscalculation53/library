#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 消せる priority queue
 * @docs docs/ds/erasable_priority_queue.md
 */

// Compare は、less が昇順で greater が降順
// 消しすぎない方法は未実装
template <class T, class Compare = less<T>>
struct ErasablePriorityQueue
{
private:
  struct ReverseCompare
  {
    Compare comp;
    bool operator()(const T &a, const T &b) const { return comp(b, a); }
  };
  priority_queue<T, vc<T>, ReverseCompare> body, era;
  void normalize()
  {
    while (!era.empty() && body.top() == era.top())
      body.pop(), era.pop();
  }

public:
  ErasablePriorityQueue() {}
  template <class It>
  ErasablePriorityQueue(It begi, It endi) : body(begi, endi) {}
  void push(const T &x)
  {
    body.push(x);
    normalize();
  }
  void pop()
  {
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
  T top() const { return body.top(); }

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
