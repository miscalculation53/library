#pragma once

// 2026-09-26: comparison-sort baseline, preserved for benchmarks.
#include "../template/template_all_but_modint.hpp"

namespace mo_reference
{

template <class I>
I mo_order_cost(const vc<pair<I, I>> &lrs, const vc<int> &ord)
{
  I res = 0;
  repi(i, SZ(lrs) - 1)
  {
    res += abs(lrs[ord[i + 1]].first - lrs[ord[i]].first);
    res += abs(lrs[ord[i + 1]].second - lrs[ord[i]].second);
  }
  return res;
}

template <class I>
vc<int> mo_order_params(const vc<pair<I, I>> &lrs, int b, int t)
{
  const int q = lrs.size();
  cauto &[ls, rs] = unzip(lrs);
  auto comp = [&](int i, int j)
  {
    int segi = (ls[i] + t * b / 2) / b, segj = (ls[j] + t * b / 2) / b;
    if (segi != segj)
      return segi < segj;
    return (segi & 1) ? (rs[i] > rs[j]) : (rs[i] < rs[j]);
  };
  vc<int> ord = permid<int>(q);
  sort(ALL(ord), comp);
  return ord;
}

template <class I>
vc<int> mo_order(const vc<pair<I, I>> &lrs)
{
  if (lrs.empty())
    return {};
  cauto &[ls, rs] = unzip(lrs);
  const int n = max(MAX(ls), MAX(rs));
  const int q = lrs.size();
  const int b1 = max(1, int(n / sqrt(q + 1)));
  const int b2 = max(1, int(sqrt(3) * n / sqrt(2 * q + 1)));
  const int b3 = max(1, int(sqrt(2) * n / sqrt(q + 1)));
  array<vc<int>, 6> ords = {
    mo_order_params(lrs, b1, 0),
    mo_order_params(lrs, b1, 1),
    mo_order_params(lrs, b2, 0),
    mo_order_params(lrs, b2, 1),
    mo_order_params(lrs, b3, 0),
    mo_order_params(lrs, b3, 1),
  };
  array<I, 6> costs;
  repi(i, 6) costs[i] = mo_order_cost(lrs, ords[i]);
  int j = ARGMIN(costs);
  return ords[j];
}

};
