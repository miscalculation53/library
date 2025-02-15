#pragma once

#include "../../template/template_all.hpp"

/**
 * @brief 二項係数
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
  PowerTable(T base) : base(base), pw(1, 1) {}

  void reserve(int n)
  {
    if (mod != T::mod())
      pw = {1};
    int i = SZ<int>(pw);
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