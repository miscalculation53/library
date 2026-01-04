#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 二項係数テーブル
 * @docs docs/math/modint/binomial_table.md
 */

template <class mint>
struct BinomialTable
{
private:
  static decltype(mint::mod()) mod;
  static vvc<mint> table;

public:
  static void reserve(int n)
  {
    if (mod != mint::mod())
    {
      mod = mint::mod();
      table = {{1}};
    }
    int i = table.size();
    if (n < i)
      return;
    table.resize(n + 1);
    for (; i <= n; i++)
    {
      table[i].resize(i + 1);
      table[i][0] = 1, table[i][i] = 1;
      rep(j, 1, i) table[i][j] = table[i - 1][j - 1] + table[i - 1][j];
    }
  }
  static mint C(int n, int k)
  {
    if (n < k)
      return 0;
    if (n < 0 || k < 0)
      return 0;
    reserve(n);
    return table[n][k];
  }
  static mint H(int n, int k)
  {
    if (n == 0 && k == 0)
      return 1;
    return C(n + k - 1, k);
  }
};
template <class mint> decltype(mint::mod()) BinomialTable<mint>::mod{};
template <class mint> vvc<mint> BinomialTable<mint>::table{};
