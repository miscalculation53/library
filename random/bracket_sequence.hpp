#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 正しい括弧列のランダム生成
 * @docs docs/random/bracket_sequence.md
 */

// https://kanpurin.hatenablog.com/entry/2023/02/20/184752#kakkoretsu
string random_bracket_sequence(int n)
{
  string s;
  ll x = n, y = n;
  rep(_, 2 * n)
  {
    double p = double(y - x) * (y + 1) / (double(y - x + 1) * (x + y));
    if (randbool(p))
      y--, s += ")";
    else
      x--, s += "(";
  }
  return s;
}
