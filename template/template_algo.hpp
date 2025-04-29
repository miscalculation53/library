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

// 引数: vc<pair<T, U>>
// 返り値: vc<pair<T, vc<U>>
// T ごとに U をまとめたもの
// T は比較可能である必要がある
template <class T, class U>
vc<pair<T, vc<U>>> sortuniqued_group(vc<pair<T, U>> v)
{
  stable_sort(ALL(v), [&](cauto &p1, cauto &p2)
              { return p1.first < p2.first; });
  vc<pair<T, vc<U>>> res;
  fec([x, y] : v)
  {
    if (res.empty() || res.back().first != x)
      res.eb(x, vc{y});
    else
      res.back().second.eb(y);
  }
  return res;
}

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

template <class T>
struct MonoidAdd
{
  using S = T;
  static constexpr S op(S a, S b) { return a + b; }
  static constexpr S e() { return 0; }
};
template <class T, const T infty = INF>
struct MonoidMin
{
  using S = T;
  static constexpr S op(S a, S b) { return min(a, b); }
  static constexpr S e() { return infty; }
};
template <class T, const T infty = INF>
struct MonoidMax
{
  using S = T;
  static constexpr S op(S a, S b) { return max(a, b); }
  static constexpr S e() { return -infty; }
};

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
  vc<T> res(n + 1);
  res[0] = v[0];
  repi(i, 1, n) res[i] = v[i] - v[i - 1];
  res[n] = -v[n - 1];
  res.erase(res.end() - right_index, res.end());
  res.erase(res.begin(), res.begin() + left_index);
  return res;
}

const vpll DRULgrid = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
const vpll DRULplane = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
