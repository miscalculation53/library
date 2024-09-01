#pragma once

#include "template_types.hpp"
#include "template_rep.hpp"

/**
 * @brief テンプレート（vector）
 * @docs docs/template/template_vector.md
 */

#define ALL(a) (a).begin(), (a).end()
#define SZ(x) (ll)((x).size())
#define SZI(x) (int)((x).size())

template <class F>
auto gen_vec(const int &n, const F &f)
{
  vc<decltype(f(0))> res(n);
  repi(i, n) res[i] = f(i);
  return res;
}

// https://qiita.com/Chippppp/items/13150f5e0ea99f444d97#%E5%A4%9A%E6%AC%A1%E5%85%83vector%E7%94%9F%E6%88%90%E9%96%A2%E6%95%B0
template <class T, size_t d, size_t i = 0>
auto dvec(cauto (&sz)[d], const T &init)
{
  if constexpr (i < d)
    return vc(sz[i], dvec<T, d, i + 1>(sz, init));
  else
    return init;
}

template <class T = ll>
T ctol(const char &c, const string &s)
{
  repi(i, SZI(s)) if (s[i] == c) return i;
  return -1;
}
template <class T = ll>
vc<T> stov(const string &s, const char &first)
{
  return gen_vec(SZI(s), [&](int i) -> T
                 { return s[i] - first; });
}
template <class T = ll>
vc<T> stov(const string &s, const string &t)
{
  return gen_vec(SZI(s), [&](int i) -> T
                 { return ctol(s[i], t); });
}

template <class T>
vc<T> concat(const vvc<T> &vs)
{
  vc<T> res;
  for (cauto &v : vs)
    res.insert(res.end(), ALL(v));
  return res;
}
template <class T>
vc<T> concat(const vc<T> &v) { return v; }
template <class T, class... Ts>
vc<T> concat(vc<T> v, const vc<Ts> &...vs)
{
  (v.insert(v.end(), ALL(vs)), ...);
  return v;
}