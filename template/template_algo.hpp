#pragma once

#ifndef INF
#define INF 4'000'000'000'000'000'037LL
#endif

#include "template_types.hpp"
#include "template_rep.hpp"
#include "template_vector.hpp"

/**
 * @brief テンプレート（アルゴリズム）
 * @docs docs/template/template_algo.md
 */

template <class V>
auto SUM(const V &v) { return accumulate(ALL(v), (decltype(v[0]))0); }
template <class T, class V>
T SUM(const V &v) { return accumulate(ALL(v), T(0)); }
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

template <class V>
void unique(V &v) { v.erase(std::unique(ALL(v)), v.end()); }
template <class V>
V uniqued(V v) { unique(v); return v; }

template <class V>
void sortunique(V &v)
{
  sort(ALL(v));
  unique(v);
}
template <class V>
V sortuniqued(V v) { sortunique(v); return v; }

// 01234 -> 12340
template <class V, class U>
void rotate(V &v, U k)
{ 
  const U n = v.size();
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

// 12
// 34 -> 246
// 56    135
// (反時計回り)
template <class VV, class U = ll>
VV rot90(const VV &a, U k = 1)
{
  if (a.empty())
    return {};
  const int n = a.size(), m = a[0].size();
  k = (k % 4 + 4) % 4;
  if (k == 0)
    return a;
  else if (k == 1)
  {
    VV b(m);
    repi(j, m) b[j].resize(n);
    repi(i, n)
    {
      assert(SZ<int>(a[i]) == m);
      repi(j, m) b[m - 1 - j][i] = a[i][j];
    }
    return b;
  }
  else if (k == 2)
  {
    VV b(n);
    repi(i, n) b[i].resize(m);
    repi(i, n)
    {
      assert(SZ<int>(a[i]) == m);
      repi(j, m) b[n - 1 - i][m - 1 - j] = a[i][j];
    }
    return b;
  }
  else
  {
    VV b(m);
    repi(j, m) b[j].resize(n);
    repi(i, n)
    {
      assert(SZ<int>(a[i]) == m);
      repi(j, m) b[j][n - 1 - i] = a[i][j];
    }
    return b;
  }
}

template <class T, class F = decltype(plus<>())>
vc<T> cuml(const vc<T> &v, F op = plus<>(), const T &e = 0)
{
  const int n = v.size();
  vc<T> res(n + 1, e);
  repi(i, n) res[i + 1] = op(res[i], v[i]);
  return res;
}
template <class T, class F = decltype(plus<>())>
vc<T> cumr(const vc<T> &v, const F &op = plus<>(), const T &e = 0)
{ return reversed(cuml<T, F>(reversed(v), op, e)); }
template <class T, const T infty = INF>
vc<T> cumlmax(const vc<T> &v)
{ return cuml(v, [](T a, T b) { return max(a, b); }, -infty); }
template <class T, const T infty = INF>
vc<T> cumrmax(const vc<T> &v)
{ return cumr(v, [](T a, T b) { return max(a, b); }, -infty); }
template <class T, const T infty = INF>
vc<T> cumlmin(const vc<T> &v)
{ return cuml(v, [](T a, T b) { return min(a, b); }, infty); }
template <class T, const T infty = INF>
vc<T> cumrmin(const vc<T> &v)
{ return cumr(v, [](T a, T b) { return min(a, b); }, infty); }

template <class T>
vc<T> adjd(const vc<T> &v)
{
  int n = v.size();
  vc<T> res(n + 1);
  res[0] = v[0];
  repi(i, 1, n) res[i] = v[i] - v[i - 1];
  res[n] = -v[n - 1];
  return res;
}

const vpll DRULgrid = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
const vpll DRULplane = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};