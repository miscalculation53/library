#pragma once

#ifndef INF
#define INF 4'000'000'000'000'000'037LL
#endif

#include "template_types.hpp"
#include "template_rep.hpp"
#include "template_vector.hpp"
#include "../utils/is_integral_ext.hpp"
#include "../utils/resolved_value.hpp"

/**
 * @brief テンプレート（アルゴリズム）
 * @docs docs/template/template_algo.md
 */

template <class V>
auto SUM(const V &v)
{
  typename V::value_type s{};
  fec(vi : v) s += vi;
  return s;
}
template <class T, class V>
T SUM(const V &v)
{
  T s{};
  fec(vi : v) s += vi;
  return s;
}
template <class V>
auto MAX(const V &v) { return *max_element(ALL(v)); }
template <class V>
auto MIN(const V &v) { return *min_element(ALL(v)); }
template <class I = ll, class V>
I ARGMAX(const V &v) { return max_element(ALL(v)) - v.begin(); }
template <class I = ll, class V>
I ARGMIN(const V &v) { return min_element(ALL(v)) - v.begin(); }

template<class T = ll, class V>
T mex(const V &a)
{
  int n = a.size();
  vector<bool> exists(n, false);
  repi(i, n) if (0 <= a[i] && a[i] < n) exists[a[i]] = true;
  repi(x, n) if (!exists[x]) return x;
  return n;
}

// (0, 1. ..., n-1) の順列か判定
template <class I>
bool is_permutation(const vc<I> &p)
{
  const int n = p.size();
  vc<bool> b(n, false);
  repi(i, n)
  {
    if (!(0 <= p[i] && p[i] < n))
      return false;
    b[p[i]] = true;
  }
  return all_of(ALL(b), [](bool bi)
                { return bi; });
}

template <class T = ll>
vc<T> permid(const int &n, const int &base_index = 0)
{
  vc<T> p(n);
  repi(i, n) p[i] = i + base_index;
  return p;
}
template <class T>
vc<T> perminv(const vc<T> &p)
{
  if (p.empty())
    return {};
  const int n = p.size();
  vc<T> q(MAX(p) + 1);
  repi(i, n) if (p[i] >= 0) q[p[i]] = i;
  return q;
}
// a[p[i]] for all i
template <class T, class U>
vc<T> permuted(const vc<T> &a, const vc<U> &p)
{
  const int n = p.size();
  vc<T> res(n);
  repi(i, n)
  {
    assert(0 <= p[i] && p[i] < U(a.size()));
    res[i] = a[p[i]];
  }
  return res;
}
// p[q[r[i]]] for all i など
template <class T, class U, class... Ts>
vc<T> permuted(const vc<T> &p, const vc<U> &q, const vc<Ts> &...rs)
{
  return permuted(permuted(p, q), rs...);
}

template <class V>
V reversed(const V &v) { return V(v.rbegin(), v.rend()); }

#if __cplusplus < 202002L
template <class V, class... Args>
V sorted(V v, Args&&... args)
{
  sort(ALL(v), forward<Args>(args)...);
  return v;
}
#else
template <class V, class... Args>
V sorted(V v, Args&&... args)
{
  ranges::sort(v, forward<Args>(args)...);
  return v;
}
#endif

template <class V, class Equal = equal_to<>>
void unique(V &v, Equal equal = {}) { v.erase(std::unique(ALL(v), equal), v.end()); }
template <class V, class Equal = equal_to<>>
V uniqued(V v, Equal equal = {}) { unique(v, equal); return v; }

template <class V, class Compare = less<>, class Equal = equal_to<>>
void sortunique(V &v, Compare comp = {}, Equal equal = {})
{
  sort(ALL(v), comp);
  unique(v, equal);
}
template <class V, class Compare = less<>, class Equal = equal_to<>>
V sortuniqued(V v, Compare comp = {}, Equal equal = {})
{ sortunique(v, comp, equal); return v; }

// 01234 -> 12340
template <class V, class U>
void rotate(V &v, U k)
{ 
  const U n = v.size();
  if (n == 0)
    return;
  k = (k % n + n) % n;
  std::rotate(v.begin(), v.begin() + k, v.end());
}
// 01234 -> 12340
template <class V, class U>
V rotated(V v, U k) { rotate(v, k); return v; }

template <class T>
vvc<T> top(const vvc<T> &a)
{
  if (a.empty())
    return {};
  const int n = a.size(), m = a[0].size();
  vvc<T> b(m, vc<T>(n));
  repi(i, n)
  {
    assert(SZ<int>(a[i]) == m);
    repi(j, m) b[j][i] = a[i][j];
  }
  return b;
}
vstr top(const vstr &a)
{
  vvc<char> a_(a.size());
  repi(i, SZ<int>(a)) a_[i] = {ALL(a[i])};
  vvc<char> b_ = top(a_);
  vstr b(b_.size());
  repi(i, SZ<int>(b)) b[i] = {ALL(b_[i])};
  return b;
}

template <class T, class = void>
struct has_e0 : false_type {};
template <class T>
struct has_e0<T, void_t<decltype(T::e0())>> : true_type {};
template <class T>
inline constexpr bool has_e0_v = has_e0<T>::value;

template <class T>
struct MonoidAdd
{
  using S = T;
  static constexpr S op(S a, S b) { return a + b; }
  static constexpr S e()
  {
    if constexpr (has_e0_v<S>)
      return S::e0();
    else
      return {};
  }
  template <class I, class = decltype(declval<S>() * declval<I>())>
  static constexpr S pow(const S &a, I k) { return a * k; }
};
template <class T, auto infty = INF>
struct MonoidMin
{
  using S = T;
  static constexpr S op(S a, S b) { return min(a, b); }
  static constexpr decltype(auto) e() { return resolved_value<T, infty>(); }
  template <class I>
  static constexpr S pow(const S &a, I k) { return k == 0 ? e() : a; }
};
template <class T, auto infty = INF>
struct MonoidMax
{
  using S = T;
  static constexpr S op(S a, S b) { return max(a, b); }
  static constexpr S e() { return -resolved_value<T, infty>(); }
  template <class I>
  static constexpr S pow(const S &a, I k) { return k == 0 ? e() : a; }
};

namespace internal
{
  template <class M, class I, class = void>
  struct HasMonoidPow : false_type
  {
  };
  template <class M, class I>
  struct HasMonoidPow<M, I, void_t<decltype(M::pow(declval<const typename M::S &>(), declval<I>()))>> : true_type
  {
  };
}

template <class M, class I>
typename M::S pow_monoid(typename M::S a, I k)
{
  if constexpr (is_signed_ext<I>)
    assert(k >= 0);
  if constexpr (internal::HasMonoidPow<M, I>::value)
    return M::pow(a, k);
  else
  {
    typename M::S c = M::e();
    for (; k; k >>= 1)
    {
      if (k & 1)
        c = M::op(c, a);
      a = M::op(a, a);
    }
    return c;
  }
}

template <class G, class I>
typename G::S pow_group(typename G::S a, I k)
{
  if constexpr (is_signed_ext<I>)
  {
    if (k < 0)
    {
      a = G::inv(a);
      return G::op(pow_monoid<G>(a, -(k + 1)), a);
    }
  }
  return pow_monoid<G>(a, k);
}

// left_index が 0 なら、長さ n+1 で a.front() が e()
// left_index が 1 なら、長さ n で e() がない
template <class M>
vc<typename M::S> cuml(const vc<typename M::S> &v, int left_index = 0)
{
  const int n = v.size();
  vc<typename M::S> res(n + 1);
  res[0] = M::e();
  repi(i, n) res[i + 1] = M::op(res[i], v[i]);
  res.erase(res.begin(), res.begin() + left_index);
  return res;
}
// right_index が 0 なら、長さ n+1 で a.back() が e()
// right_index が 1 なら、長さ n で e() がない
template <class M>
vc<typename M::S> cumr(const vc<typename M::S> &v, int right_index = 0)
{ return reversed(cuml<M>(reversed(v), right_index)); }
template <class T>
vc<T> cumlsum(const vc<T> &v, int left_index = 0)
{ return cuml<MonoidAdd<T>>(v, left_index); }
template <class T>
vc<T> cumrsum(const vc<T> &v, int right_index = 0)
{ return cumr<MonoidAdd<T>>(v, right_index); }
template <class T>
vc<T> cumlmin(const vc<T> &v, int left_index = 0)
{ return cuml<MonoidMin<T>>(v, left_index); }
template <class T>
vc<T> cumrmin(const vc<T> &v, int right_index = 0)
{ return cumr<MonoidMin<T>>(v, right_index); }
template <class T>
vc<T> cumlmax(const vc<T> &v, int left_index = 0)
{ return cuml<MonoidMax<T>>(v, left_index); }
template <class T>
vc<T> cumrmax(const vc<T> &v, int right_index = 0)
{ return cumr<MonoidMax<T>>(v, right_index); }

// デフォルトでは長さ n+1
// left_index, right_index をそれぞれ 1 にすると、左右が削除される
template <class T>
vc<T> adjd(const vc<T> &v, int left_index = 0, int right_index = 0)
{
  int n = v.size();
  assert(0 <= left_index && 0 <= right_index && left_index + right_index <= n + 1);
  vc<T> res(n + 1);
  if (n == 0)
  {
    res[0] = T{};
    res.erase(res.end() - right_index, res.end());
    res.erase(res.begin(), res.begin() + left_index);
    return res;
  }
  res[0] = v[0];
  repi(i, 1, n) res[i] = v[i] - v[i - 1];
  res[n] = -v[n - 1];
  res.erase(res.end() - right_index, res.end());
  res.erase(res.begin(), res.begin() + left_index);
  return res;
}

constexpr array<pll, 4> DRULgrid = {{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};
constexpr array<pll, 4> DRULplane = {{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}};
