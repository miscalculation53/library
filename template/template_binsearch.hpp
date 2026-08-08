#pragma once

#include "template_types.hpp"
#include "template_rep.hpp"
#include "template_vector.hpp"

/**
 * @brief テンプレート（二分探索）
 * @docs docs/template/template_binsearch.md
 */

template <class T>
struct is_random_access_iterator
{
  static constexpr bool value = is_same_v<
    typename iterator_traits<T>::iterator_category,
    random_access_iterator_tag
  >;
};
template <class T>
constexpr bool is_random_access_iterator_v = is_random_access_iterator<T>::value;

// --- LB, UB ---

#if __cplusplus < 202002L
struct identity
{
  template <class T>
  constexpr T &&operator()(T &&t) const noexcept
  { return forward<T>(t); }
};
namespace internal
{
  template <class T = ll, class V, class Judge>
  inline T bound_helper(const V &v, Judge judge)
  {
    int l = -1, r = v.size();
    while (r - l > 1)
    {
      int m = (l + r) / 2;
      if (judge(m))
        l = m;
      else
        r = m;
    }
    return r;
  }
};
// val <= v[i] となる最小の i (val 未満の値の個数)
template <class T = ll, class V, class Value, class Comp = less<>, class Proj = identity>
inline T LB(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
{
  return internal::bound_helper(v, [&](int i) -> bool
                                { return comp(proj(*(v.begin() + i)), val); });
}
// val < v[i] となる最小の i (val 以下の値の個数)
template <class T = ll, class V, class Value, class Comp = less<>, class Proj = identity>
inline T UB(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
{
  return internal::bound_helper(v, [&](int i) -> bool
                                { return !comp(val, proj(*(v.begin() + i))); });
}
#define DEFAULT_COMP less<>
#else
// val <= v[i] となる最小の i (val 未満の値の個数)
template <class T = ll, class V, class Value, class Comp = ranges::less, class Proj = identity>
inline T LB(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
{ return ranges::lower_bound(v, val, comp, proj) - v.begin(); }
// val < v[i] となる最小の i (val 以下の値の個数)
template <class T = ll, class V, class Value, class Comp = ranges::less, class Proj = identity>
inline T UB(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
{ return ranges::upper_bound(v, val, comp, proj) - v.begin(); }
#define DEFAULT_COMP ranges::less
#endif

// --- vector 等の lt, leq, gt, geq ---

// v[i] < val となる最大の i (なければ -1)
template <class T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>
inline auto lt_max(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>
{ return LB<T>(v, val, comp, proj) - 1; }
// v[i] <= val となる最大の i (なければ -1)
template <class T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>
inline auto leq_max(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>
{ return UB<T>(v, val, comp, proj) - 1; }
// val < v[i] となる最小の i (なければ n)
template <class T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>
inline auto gt_min(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>
{ return UB<T>(v, val, comp, proj); }
// val <= v[i] となる最小の i (なければ n)
template <class T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>
inline auto geq_min(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>
{ return LB<T>(v, val, comp, proj); }
// v[i] < val となる i の個数
template <class T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>
inline auto lt_cnt(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>
{ return LB<T>(v, val, comp, proj); }
// v[i] <= val となる i の個数
template <class T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>
inline auto leq_cnt(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>
{ return UB<T>(v, val, comp, proj); }
// val < v[i] となる i の個数
template <class T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>
inline auto gt_cnt(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>
{ return SZ<T>(v) - UB<T>(v, val, comp, proj); }
// val <= v[i] となる i の個数
template <class T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>
inline auto geq_cnt(const V &v, const Value &val, Comp comp = {}, Proj proj = {})
-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>
{ return SZ<T>(v) - LB<T>(v, val, comp, proj); }
// l <= v[i] < r となる i の個数
template <class T = ll, class V, class L, class R, class Comp = DEFAULT_COMP, class Proj = identity>
inline auto in_cnt(const V &v, L l, R r, Comp comp = {}, Proj proj = {})
-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>
{
  if (l > r)
    return 0;
  return lt_cnt<T>(v, r, comp, proj) - lt_cnt<T>(v, l, comp, proj);
}

// --- set 等の lt, leq, gt, geq ---

// *it < val となる最大の it (なければ end())
template <class V, class Value>
inline auto lt_max(const V &v, const Value &val)
-> enable_if_t<!is_random_access_iterator_v<typename V::iterator>, typename V::const_iterator>
{
  auto it = v.lower_bound(val);
  return it == v.begin() ? v.end() : prev(it);
}
// *it <= val となる最大の it (なければ end())
template <class V, class Value>
inline auto leq_max(const V &v, const Value &val)
-> enable_if_t<!is_random_access_iterator_v<typename V::iterator>, typename V::const_iterator>
{
  auto it = v.upper_bound(val);
  return it == v.begin() ? v.end() : prev(it);
}
// val < *it となる最小の it (なければ end())
template <class V, class Value>
inline auto gt_min(const V &v, const Value &val)
-> enable_if_t<!is_random_access_iterator_v<typename V::iterator>, typename V::const_iterator>
{ return v.upper_bound(val); }
// val <= *it となる最小の it (なければ end())
template <class V, class Value>
inline auto geq_min(const V &v, const Value &val)
-> enable_if_t<!is_random_access_iterator_v<typename V::iterator>, typename V::const_iterator>
{ return v.lower_bound(val); }

// --- 自作二分探索 ---

namespace internal
{
template <class T>
bool binsearch_adjacent(T a, T b)
{
  if (a < b)
    return a + 1 == b;
  if (b < a)
    return b + 1 == a;
  return false;
}
};

// (ok, ng)
template <class T = ll, class Judge, class InitOk, class InitNg>
pair<T, T> binsearch(const Judge &judge, InitOk init_ok, InitNg init_ng, bool check_ok = true, bool check_ng = true)
{
  T ok(init_ok), ng(init_ng);
  if (check_ok)
    assert(judge(ok));
  if (check_ng)
    assert(!judge(ng));
  while (!internal::binsearch_adjacent(ok, ng))
  {
    T mid = (ok & ng) + ((ok ^ ng) >> 1);
    (judge(mid) ? ok : ng) = mid;
  }
  return {ok, ng};
}
template <class T = ld, class Judge, class InitOk, class InitNg>
T binsearch_real(const Judge &judge, InitOk init_ok, InitNg init_ng, int iteration_count = 100, bool check_ok = true, bool check_ng = true)
{
  T ok(init_ok), ng(init_ng);
  if (check_ok)
    assert(judge(ok));
  if (check_ng)
    assert(!judge(ng));
  repi(_, iteration_count)
  {
    T mid = (ok + ng) / 2;
    (judge(mid) ? ok : ng) = mid;
  }
  return ok;
}
// (ok, ng)
template <class T = ll, class Judge, class InitVal>
pair<T, T> expsearch(const Judge &judge, InitVal init_val, bool positive = true)
{
  T cur(init_val), step = 1;
  const bool cur_ok = judge(cur);
  auto advance = [&](T x, T d, bool pos) -> T
  {
    if (pos)
      return x > numeric_limits<T>::max() - d ? numeric_limits<T>::max() : x + d;
    else
      return x < numeric_limits<T>::lowest() + d ? numeric_limits<T>::lowest() : x - d;
  };
  T prv = advance(cur, 1, !positive);
  if (prv != cur && judge(prv) != cur_ok)
  {
    if (cur_ok)
      return {cur, prv};
    else
      return {prv, cur};
  }
  while (true)
  {
    T nxt = advance(cur, step, positive);
    assert(nxt != cur && "the boundary must exist in the searched direction");
    if (nxt == cur || judge(nxt) != cur_ok)
    {
      T ok = cur_ok ? cur : nxt;
      T ng = cur_ok ? nxt : cur;
      return binsearch<T>(judge, ok, ng, false, false);
    }
    cur = nxt;
    if (step > numeric_limits<T>::max() / 2)
      step = numeric_limits<T>::max();
    else
      step *= 2;
  }
}
