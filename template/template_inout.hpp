#pragma once

#include "template_types.hpp"
#include "template_rep.hpp"

/**
 * @brief テンプレート（入出力）
 * @docs docs/template/template_inout.md
 */
/**
 * 参考：
 * https://trap.jp/post/1224/
 */

// ---- 入力 ----
template <class T, class U>
istream &operator>>(istream &is, pair<T, U> &p)
{
  cin >> p.first >> p.second;
  return is;
}
template <class T, size_t n>
istream &operator>>(istream &is, array<T, n> &a)
{
  for (size_t i = 0; i < n; i++)
    cin >> a[i];
  return is;
}
template <class... Ts>
istream &operator>>(istream &is, tuple<Ts...> &t)
{
  apply([&](auto &...a)
        { (is >> ... >> a); }, t);
  return is;
}

template <class... Ts>
void INPUT(Ts &...a) { (cin >> ... >> a); }

template <class T>
void INPUTVEC(int n, vc<T> &v)
{
  v.resize(n);
  repi(i, n) cin >> v[i];
}
template <class T, class... Ts>
void INPUTVEC(int n, vc<T> &v, vc<Ts> &...vs)
{ INPUTVEC(n, v), INPUTVEC(n, vs...); }

template <class T>
void INPUTVEC2(int n, int m, vvc<T> &v)
{
  v.assign(n, vc<T>(m));
  repi(i, n) repi(j, m) cin >> v[i][j];
}
template <class T, class... Ts>
void INPUTVEC2(int n, int m, vvc<T> &v, vvc<Ts> &...vs)
{ INPUTVEC2(n, m, v), INPUTVEC2(n, m, vs...); }

#define INT(...) int __VA_ARGS__; INPUT(__VA_ARGS__)
#define LL(...) ll __VA_ARGS__; INPUT(__VA_ARGS__)
#define STR(...) string __VA_ARGS__; INPUT(__VA_ARGS__)
#define ARR(T, n, ...) array<T, n> __VA_ARGS__; INPUT(__VA_ARGS__)
#define VEC(T, n, ...) vc<T> __VA_ARGS__; INPUTVEC(n, __VA_ARGS__)
#define VEC2(T, n, m, ...) vvc<T> __VA_ARGS__; INPUTVEC2(n, m, __VA_ARGS__)
// ----------

// ----- 出力 -----
#ifdef FAST_IO
  #define ENDL '\n'
#else
  #define ENDL endl
#endif

template <class T>
void PRINT(const T &a) { cout << a << ENDL; }
template <class T, class... Ts>
void PRINT(const T &a, const Ts &...b)
{
  cout << a;
  (cout << ... << (cout << ' ', b));
  cout << ENDL;
}
#define PRINTEXIT(...) do { PRINT(__VA_ARGS__); exit(0); } while (false)
#define PRINTRETURN(...) do { PRINT(__VA_ARGS__); return; } while (false)

template <class T>
void PRINTVEC(const vc<T> &v)
{
  const int n = v.size();
  repi(i, n) cout << v[i] << (i == n - 1 ? "" : " ");
  cout << ENDL;
}
template <class T>
void PRINTVECT(const vc<T> &v) { for (auto &vi : v) cout << vi << ENDL; }
template <class T>
void PRINTVEC2(const vvc<T> &v) { for (auto &vi : v) PRINTVEC(vi); }
// ----------

// ----- 基準ずらし -----
template <class T, class U>
pair<T, U> operator+=(pair<T, U> &a, cauto &b)
{
  a.first += b.first;
  a.second += b.second;
  return a;
}
template <class T, class U>
pair<T, U> operator+(pair<T, U> &a, cauto &b) { return a += b; }

template <class T, size_t n>
array<T, n> operator+=(array<T, n> &a, cauto &b)
{
  for (size_t i = 0; i < n; i++)
    a[i] += b[i];
  return a;
}
template <class T, size_t n>
array<T, n> operator+(array<T, n> &a, cauto &b) { return a += b; }

template <size_t... I>
auto tuple_add_impl(auto &a, cauto &b, const index_sequence<I...>)
{
  ((get<I>(a) += get<I>(b)), ...);
  return a;
}
template <class... Ts>
tuple<Ts...> operator+=(tuple<Ts...> &a, cauto &b)
{ return tuple_add_impl(a, b, make_index_sequence<tuple_size_v<tuple<Ts...>>>{}); }
template <class... Ts>
tuple<Ts...> operator+(tuple<Ts...> &a, cauto &b) { return a += b; }

template <class T>
void offset(vc<T> &v, cauto &add) { for (auto &vi : v) vi += add; }
template <class T>
void offset(vvc<T> &v, cauto &add) { for (auto &vi : v) for (auto &vij : vi) vij += add; }
// ----------

// ----- 転置 -----
template <class T, const size_t m>
array<vc<T>, m> top(const vc<array<T, m>> &vt)
{
  const size_t n = vt.size();
  array<vc<T>, m> tv;
  tv.fill(vc<T>(n));
  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < m; j++)
      tv[j][i] = vt[i][j];
  return tv;
}
template <class T, const size_t m>
vc<array<T, m>> top(const array<vc<T>, m> &tv)
{
  if (tv.empty()) return {};
  const size_t n = tv[0].size();
  vc<array<T, m>> vt(n);
  for (size_t j = 0; j < m; j++)
  {
    assert(tv[j].size() == n);
    for (size_t i = 0; i < n; i++)
      vt[i][j] = tv[j][i];
  }
  return vt;
}

template <class T, class U>
pair<vc<T>, vc<U>> top(const vc<pair<T, U>> &vt)
{
  const size_t n = vt.size();
  pair<vc<T>, vc<U>> tv;
  tv.first.resize(n), tv.second.resize(n);
  for (size_t i = 0; i < n; i++)
    tie(tv.first[i], tv.second[i]) = vt[i];
  return tv;
}
template <class T, class U>
vc<pair<T, U>> top(const pair<vc<T>, vc<U>> &tv)
{
  const size_t n = tv.first.size();
  assert(n == tv.second.size());
  vc<pair<T, U>> vt(n);
  for (size_t i = 0; i < n; i++)
    vt[i] = make_pair(tv.first[i], tv.second[i]);
  return vt;
}

template <size_t... I>
auto vt_to_tv_impl(auto &tv, cauto &t, index_sequence<I...>, size_t index)
{ ((get<I>(tv)[index] = get<I>(t)), ...); }
template <class... Ts>
auto top(const vc<tuple<Ts...>> &vt)
{
  const size_t n = vt.size();
  tuple<vc<Ts>...> tv;
  apply([&](auto &...v)
        { ((v.resize(n)), ...); }, tv);
  for (size_t i = 0; i < n; i++)
    vt_to_tv_impl(tv, vt[i], make_index_sequence<tuple_size_v<decltype(tv)>>{}, i);
  return tv;
}

template <size_t... I>
auto tv_to_vt_impl(cauto &tv, index_sequence<I...>, size_t index)
{ return make_tuple(get<I>(tv)[index]...); }
template <class... Ts>
auto top(const tuple<vc<Ts>...> &tv)
{
  size_t n = get<0>(tv).size();
  apply([&](auto &...v)
        { ((assert(v.size() == n)), ...); }, tv);
  vc<tuple<Ts...>> vt(n);
  for (size_t i = 0; i < n; i++)
    vt[i] = tv_to_vt_impl(tv, index_sequence_for<Ts...>{}, i);
  return vt;
}
// ----------