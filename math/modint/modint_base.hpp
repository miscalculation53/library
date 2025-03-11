#pragma once

#include "../../template/template_all.hpp"

namespace internal
{

#define REF static_cast<mint &>(*this)
#define CREF static_cast<const mint &>(*this)
#define VAL *static_cast<const mint *>(this)

template <class mint>
struct modint_base
{
  mint &operator+=(const mint &rhs)
  {
    mint &self = REF;
    self._v += rhs._v;
    if (self._v >= self.umod())
      self._v -= self.umod();
    return self;
  }
  mint &operator-=(const mint &rhs)
  {
    mint &self = REF;
    self._v -= rhs._v;
    if (self._v >= self.umod())
      self._v += self.umod();
    return self;
  }
  mint &operator/=(const mint &rhs)
  {
    mint &self = REF;
    return self = self * rhs.inv();
  }

  mint &operator++()
  {
    mint &self = REF;
    self._v++;
    if (self._v == self.umod())
      self._v = 0;
    return self;
  }
  mint &operator--()
  {
    mint &self = REF;
    if (self._v == 0)
      self._v = self.umod();
    self._v--;
    return self;
  }
  mint operator++(int)
  {
    mint res = VAL;
    ++REF;
    return res;
  }
  mint operator--(int)
  {
    mint res = VAL;
    --REF;
    return res;
  }

  mint operator+() const { return VAL; }
  mint operator-() const { return mint() - VAL; }

  mint pow(ll n) const
  {
    assert(n >= 0);
    mint x = VAL, r = 1;
    while (n)
    {
      if (n & 1)
        r *= x;
      x *= x;
      n >>= 1;
    }
    return r;
  }

  friend mint operator+(const mint &lhs, const mint &rhs)
  { return mint(lhs) += rhs; }
  friend mint operator-(const mint &lhs, const mint &rhs)
  { return mint(lhs) -= rhs; }
  friend mint operator*(const mint &lhs, const mint &rhs)
  { return mint(lhs) *= rhs; }
  friend mint operator/(const mint &lhs, const mint &rhs)
  { return mint(lhs) /= rhs; }
  friend bool operator==(const mint &lhs, const mint &rhs)
  { return mint(lhs).eq(rhs); }
  friend bool operator!=(const mint &lhs, const mint &rhs)
  { return mint(lhs).neq(rhs); }
private:
  bool eq(const mint &rhs) { return REF._v == rhs._v; }
  bool neq(const mint &rhs) { return REF._v != rhs._v; }
};

}

#if defined LOCAL or not defined FAST_IO
template <typename T, std::enable_if_t<std::is_base_of_v<internal::modint_base<T>, T>, int> = 0>
istream &operator>>(istream &is, T &x)
{
  ll a;
  is >> a;
  x = a;
  return is;
}
template <typename T, std::enable_if_t<std::is_base_of_v<internal::modint_base<T>, T>, int> = 0>
ostream &operator<<(ostream &os, const T &x)
{
  os << x.val();
  return os;
}
#else
template <typename T, std::enable_if_t<std::is_base_of_v<internal::modint_base<T>, T>, int> = 0>
void rd1(T &x)
{
  ll a;
  fastio::rd1(a);
  x = a;
}
template <typename T, std::enable_if_t<std::is_base_of_v<internal::modint_base<T>, T>, int> = 0>
void wt1(const T &x) { fastio::wt1(x.val()); }
#endif