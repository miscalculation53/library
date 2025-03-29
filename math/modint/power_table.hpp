#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 累乗テーブル
 * @docs docs/math/modint/power_table.md
 */

template <class T>
struct PowerTable
{
private:
  decltype(T::mod()) mod;
  T base;
  vc<T> pw;

public:
  PowerTable() {}
  PowerTable(T base) : mod(T::mod()), base(base), pw(1, 1) {}

  void reserve(int n)
  {
    if (mod != T::mod())
    {
      mod = T::mod();
      pw = {1};
    }
    int i = pw.size();
    if (n < i)
      return;
    pw.resize(n + 1);
    for (; i <= n; i++)
      pw[i] = pw[i - 1] * base;
  }

  T pow(int n)
  {
    reserve(n);
    return pw[n];
  }
};