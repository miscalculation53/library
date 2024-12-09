#pragma once

#include "template_types.hpp"
#include "template_rep.hpp"
#include "template_vector.hpp"

/**
 * @brief テンプレート（二分探索）
 * @docs docs/template/template_binsearch.md
 */

#if __cplusplus < 202002L
// val <= v[i] となる最小の i (val 未満の値の個数)
template <class T = ll, class V, class... Args>
inline T LB(const V &v, Args&&... args)
{ return lower_bound(ALL(v), forward<Args>(args)...) - v.begin(); }
// val < v[i] となる最小の i (val 以下の値の個数)
template <class T = ll, class V, class... Args>
inline T UB(const V &v, Args&&... args)
{ return upper_bound(ALL(v), forward<Args>(args)...) - v.begin(); }
#else
// val <= v[i] となる最小の i (val 未満の値の個数)
// 引数: comp, proj
template <class T = ll, class V, class... Args>
inline T LB(const V &v, Args&&... args)
{ return ranges::lower_bound(v, forward<Args>(args)...) - v.begin(); }
// val < v[i] となる最小の i (val 以下の値の個数)
// 引数: comp, proj
template <class T = ll, class V, class... Args>
inline T UB(const V &v, Args&&... args)
{ return ranges::upper_bound(v, forward<Args>(args)...) - v.begin(); }
#endif

template <class T = ll>
pair<T, T> binsearch(cauto &judge, cauto &init_ok, cauto &init_ng)
{
  T ok(init_ok), ng(init_ng);
  assert(judge(ok));
  assert(!judge(ng));
  while (ok - ng != 1 && ng - ok != 1)
  {
    T mid = (ok & ng) + ((ok ^ ng) >> 1);
    (judge(mid) ? ok : ng) = mid;
  }
  return make_pair(ok, ng);
}
template <class T = ld>
T binsearch_real(cauto &judge, cauto &init_ok, cauto &init_ng, const int &iteration_count = 100)
{
  T ok(init_ok), ng(init_ng);
  assert(judge(ok));
  assert(!judge(ng));
  repi(_, iteration_count)
  {
    T mid = (ok + ng) / 2;
    (judge(mid) ? ok : ng) = mid;
  }
  return ok;
}
template <class T = ll>
pair<T, T> expsearch(cauto &judge, cauto &init_val, const bool &positive = true)
{
  T ok, ng;
  if (judge(init_val))
  {
    ok = init_val, ng = init_val + (positive ? 1 : -1);
    for (int i = 1; judge(ng); i++)
      ok = ng, ng = init_val + (positive ? 1 : -1) * (T(1) << i);
  }
  else
  {
    ng = init_val, ok = init_val + (positive ? 1 : -1);
    for (int i = 1; !judge(ok); i++)
      ng = ok, ok = init_val + (positive ? 1 : -1) * (T(1) << i);
  }
  while (ok - ng != 1 && ng - ok != 1)
  {
    T mid = (ok & ng) + ((ok ^ ng) >> 1);
    (judge(mid) ? ok : ng) = mid;
  }
  return make_pair(ok, ng);
}