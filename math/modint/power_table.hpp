#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 累乗テーブル
 * @docs docs/math/modint/power_table.md
 */

template <class mint>
struct PowerTable
{
private:
  decltype(mint::mod()) mod;
  mint base;
  vc<mint> pw;

public:
  PowerTable() {}
  PowerTable(const mint &base) : mod(mint::mod()), base(base), pw(1, 1) {}

  void reserve(int n)
  {
    if (mod != mint::mod())
    {
      mod = mint::mod();
      pw = {1};
    }
    int i = pw.size();
    if (n < i)
      return;
    pw.resize(n + 1);
    for (; i <= n; i++)
      pw[i] = pw[i - 1] * base;
  }

  mint pow(int n)
  {
    reserve(n);
    return pw[n];
  }
};