#pragma once

#include "../template/template_all.hpp"

/**
 * @brief 直積
 * @docs docs/itertools/bit.md
 */

template <class T>
struct bsubsets
{
private:
  T x;
public:
  bsubsets(T x) : x(x) {}
  struct Iterator
  {
  private:
    T y;
    bool is_end;
    const bsubsets &bs;
  public:
    Iterator(T y, bool is_end, const bsubsets &bs) : y(y), is_end(is_end), bs(bs) {}
    T operator*() const { return y; }
    Iterator& operator++()
    {
      if (y == 0)
        is_end = true;
      y = (y - 1) & bs.x;
      return *this;
    }
    bool operator!=(const Iterator &other) const { return y != other.y || is_end != other.is_end; }
  };
  Iterator begin() const { return Iterator(x, false, *this); }
  Iterator end() const { return Iterator(x, true, *this); }
};
template <class T>
struct bsupsets
{
private:
  int n;
  T x;
public:
  bsupsets(int n, T x) : n(n), x(x) {}
  struct Iterator
  {
  private:
    T y;
    const bsupsets &bs;
  public:
    Iterator(T y, const bsupsets &bs) : y(y), bs(bs) {}
    T operator*() const { return y; }
    Iterator& operator++()
    {
      y = (y + 1) | bs.x;
      return *this;
    }
    bool operator!=(const Iterator &other) const { return y != other.y; }
  };
  Iterator begin() const { return Iterator(x, *this); }
  Iterator end() const { return Iterator((T(1) << n) | x, *this); }
};