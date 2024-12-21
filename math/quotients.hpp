#pragma once

#include "../template/template_all.hpp"

/**
 * @brief 商列挙
 * @docs docs/math/quotients.md
 */

// (y, l, r)
// y は floor(n / x^d) (x: 整数) として表せる整数
// y == floor(n / x^d) <=> l < x <= r
struct quotients
{
private:
  ll n;
  int d;

public:
  quotients(ll n, int d = 1) : n(n), d(d)
  {
    assert(n >= 1 && d >= 1);
  }
  struct Iterator
  {
  private:
    ll y, l, r;
    const quotients &q;

  public:
    Iterator(ll y, ll l, ll r, const quotients &q) : y(y), l(l), r(r), q(q) {}
    tuple<ll, ll, ll> operator*() const { return {y, l, r}; }
    Iterator& operator++()
    {
      if (l == 0)
        y = l = r = -1;
      else
      {
        r = l;
        y = q.n / ipow(l, q.d);
        l = iroot(q.n / (y + 1), q.d);
      }
      return *this;
    }
    bool operator!=(const Iterator &other) const { return y != other.y; }
  };
  Iterator begin() const { return Iterator(1, iroot(n / 2, d), iroot(n, d), *this); }
  Iterator end() const { return Iterator(-1, -1, -1, *this); }
};