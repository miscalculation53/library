#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 並列二分探索
 * @docs docs/algo/parallel_binsearch.md
 */

// vc<bool> judge(vc<ll>): q 個の質問にまとめて答える
// (oks, ngs)
template <class T = ll, class Judge, class InitOk, class InitNg>
pair<vc<T>, vc<T>> parallel_binsearch(int q, const Judge &judge, const InitOk &init_ok, const InitNg &init_ng)
{
  vc<T> oks(q, init_ok), ngs(q, init_ng);
  while (true)
  {
    vc<T> mids(q);
    bool end = true;
    repi(i, q)
    {
      if (oks[i] - ngs[i] != 1 && ngs[i] - oks[i] != 1)
        end = false;
      mids[i] = (oks[i] & ngs[i]) + ((oks[i] ^ ngs[i]) >> 1);
    }
    if (end)
      break;
    auto res = judge(mids);
    repi(i, q) (res[i] ? oks[i] : ngs[i]) = mids[i];
  }
  return {oks, ngs};
}
// vc<bool> judge(vc<ld>): q 個の質問にまとめて答える
template <class T = ld, class Judge, class InitOk, class InitNg>
vc<T> parallel_binsearch_real(int q, const Judge &judge, const InitOk &init_ok, const InitNg &init_ng, int iteration_count = 100)
{
  vc<T> oks(q, init_ok), ngs(q, init_ng);
  repi(_, iteration_count)
  {
    vc<T> mids(q);
    repi(i, q) mids[i] = (oks[i] + ngs[i]) / 2;
    auto res = judge(mids);
    repi(i, q) (res[i] ? oks[i] : ngs[i]) = mids[i];
  }
  return oks;
}