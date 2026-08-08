#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 並列二分探索
 * @docs docs/algo/parallel_binsearch.md
 */

// vc<bool> judge(vc<ll>): q 個の質問にまとめて答える
// (oks, ngs) を返す
template <class T = ll, class Judge, class InitOk, class InitNg>
pair<vc<T>, vc<T>> parallel_binsearch(int q, const Judge &judge, InitOk init_ok, InitNg init_ng, bool check_ok = true, bool check_ng = true)
{
  if (check_ok)
  {
    auto res = judge(vc<T>(q, init_ok));
    assert(all_of(ALL(res), LMD(x, x)));
  }
  if (check_ng)
  {
    auto res = judge(vc<T>(q, init_ng));
    assert(all_of(ALL(res), LMD(x, !x)));
  }
  vc<T> oks(q, init_ok), ngs(q, init_ng);
  while (true)
  {
    vc<T> mids(q);
    bool end = true;
    repi(i, q)
    {
      if (!internal::binsearch_adjacent(oks[i], ngs[i]))
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
vc<T> parallel_binsearch_real(int q, const Judge &judge, InitOk init_ok, InitNg init_ng, int iteration_count = 100, bool check_ok = true, bool check_ng = true)
{
  if (check_ok)
  {
    auto res = judge(vc<T>(q, init_ok));
    assert(all_of(ALL(res), LMD(x, x)));
  }
  if (check_ng)
  {
    auto res = judge(vc<T>(q, init_ng));
    assert(all_of(ALL(res), LMD(x, !x)));
  }
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
