#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 直積全探索
 * @docs docs/itertools/direct_product.md
 */

template <class T = ll>
struct direct_product
{
private:
  vc<T> a;
public:
  direct_product(const vc<T> &a) : a(a)
  {
    fec(ai : a) assert(ai >= 1);
  }
  struct Iterator
  {
  private:
    vc<T> b;
    const direct_product &prod;

  public:
    Iterator(const vc<T> &b, const direct_product &prod) : b(b), prod(prod) {}
    vc<T> operator*() const { return b; }
    Iterator& operator++()
    {
      if (b.empty())
      {
        b = {0};
        return *this;
      }
      b.back()++;
      repi(i, SZ<int>(prod.a) - 1, 0, -1)
      {
        if (b[i] == prod.a[i])
        {
          b[i] = 0;
          b[i - 1]++;
        }
        else
          break;
      }
      return *this;
    }
    bool operator!=(const Iterator &other) const { return b != other.b; }
  };
  Iterator begin() const { return Iterator(vc<T>(a.size(), 0), *this); }
  Iterator end() const
  {
    if (a.empty())
      return Iterator({0}, *this);
    vc<T> c(a.size(), 0);
    c[0] = a[0];
    return Iterator(c, *this);
  }
};
