#pragma once

#include "../template/template_all_but_modint.hpp"

#include "../math/algebra/algebra_base.hpp"

/**
 * @brief SWAG
 * @docs docs/ds/swag.md
 */

template <class M>
struct FoldableStack
{
  using S = typename M::S;

private:
  stack<pair<S, S>> sta;

public:
  FoldableStack() {}

  template <class I = ll>
  I size() const { return sta.size(); }
  bool empty() const { return sta.empty(); }

  S all_prod() const { return sta.empty() ? M::e() : sta.top().second; }
  S top() const
  {
    assert(!sta.empty());
    return sta.top().first;
  }
  void push(const S &x) { sta.emplace(x, M::op(all_prod(), x)); }
  void pop()
  {
    assert(!sta.empty());
    sta.pop();
  }

  vc<S> content() const
  {
    auto tmp(*this);
    vc<S> res;
    while (!tmp.empty())
    {
      res.eb(tmp.top());
      tmp.pop();
    }
    return reversed(res);
  }
};

template <class M>
struct FoldableQueue
{
  using S = typename M::S;

private:
  FoldableStack<OppositeMonoid<M>> sta1;
  FoldableStack<M> sta2;
  void move2to1()
  {
    if (!sta1.empty())
      return;
    while (!sta2.empty())
    {
      sta1.push(sta2.top());
      sta2.pop();
    }
  }

public:
  FoldableQueue() {}

  template <class I = ll>
  I size() const { return sta1.template size<I>() + sta2.template size<I>(); }
  bool empty() const { return size<int>() == 0; }

  S all_prod() const { return M::op(sta1.all_prod(), sta2.all_prod()); }
  pair<S, S> all_prod_left_right() const { return {sta1.all_prod(), sta2.all_prod()}; }
  S front()
  {
    move2to1();
    return sta1.top();
  }
  void push(const S &x) { sta2.push(x); }
  void pop()
  {
    move2to1();
    sta1.pop();
  }

  vc<S> content() const
  {
    auto tmp(*this);
    vc<S> res;
    while (!tmp.empty())
    {
      res.eb(tmp.front());
      tmp.pop();
    }
    return res;
  }
};

template <class M>
struct FoldableDeque
{
  using S = typename M::S;

private:
  FoldableStack<OppositeMonoid<M>> sta1;
  FoldableStack<M> sta2;
  void move2to1()
  {
    if (!sta1.empty())
      return;
    const int k = sta2.size();
    vc<S> vec(k / 2, M::e());
    repi(i, k / 2)
    {
      vec[i] = sta2.top();
      sta2.pop();
    }
    repi(_, k - k / 2)
    {
      sta1.push(sta2.top());
      sta2.pop();
    }
    repi(i, k / 2 - 1, -1, -1) sta2.push(vec[i]);
  }
  void move1to2()
  {
    if (!sta2.empty())
      return;
    const int k = sta1.size();
    vc<S> vec(k / 2, M::e());
    repi(i, k / 2)
    {
      vec[i] = sta1.top();
      sta1.pop();
    }
    repi(_, k - k / 2)
    {
      sta2.push(sta1.top());
      sta1.pop();
    }
    repi(i, k / 2 - 1, -1, -1) sta1.push(vec[i]);
  }

public:
  FoldableDeque() {}

  template <class I = ll>
  I size() const { return sta1.template size<I>() + sta2.template size<I>(); }
  bool empty() const { return size<int>() == 0; }

  S all_prod() const { return M::op(sta1.all_prod(), sta2.all_prod()); }
  pair<S, S> all_prod_left_right() const { return {sta1.all_prod(), sta2.all_prod()}; }
  S front()
  {
    move2to1();
    return sta1.top();
  }
  S back()
  {
    move1to2();
    return sta2.top();
  }
  void push_front(const S &x) { sta1.push(x); }
  void push_back(const S &x) { sta2.push(x); }
  void pop_front()
  {
    move2to1();
    sta1.pop();
  }
  void pop_back()
  {
    move1to2();
    sta2.pop();
  }

  vc<S> content() const
  {
    auto tmp(*this);
    vc<S> res;
    while (!tmp.empty())
    {
      res.eb(tmp.front());
      tmp.pop_front();
    }
    return res;
  }
};
