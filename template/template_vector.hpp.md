---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: template/template_rep.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_types.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09"
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_vector.test.cpp
    title: verify/mytest/template_vector.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    _deprecated_at_docs: docs/template/template_vector.md
    document_title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08vector\uFF09"
    links:
    - https://qiita.com/Chippppp/items/13150f5e0ea99f444d97#%E5%A4%9A%E6%AC%A1%E5%85%83vector%E7%94%9F%E6%88%90%E9%96%A2%E6%95%B0
  bundledCode: "#line 2 \"template/template_vector.hpp\"\n\n#line 2 \"template/template_types.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09\n *\
    \ @docs docs/template/template_types.md\n */\n\n#include <bits/stdc++.h>\nusing\
    \ namespace std;\n\n#ifndef EPS\n#define EPS 1e-11\n#endif\nusing ld = decltype(EPS);\n\
    \nusing ll = long long;\nusing uint = unsigned int;\nusing ull = unsigned long\
    \ long;\nusing pll = pair<ll, ll>;\nusing tlll = tuple<ll, ll, ll>;\nusing tllll\
    \ = tuple<ll, ll, ll, ll>;\n\n#define vc vector\ntemplate <class T>\nusing vvc\
    \ = vc<vc<T>>;\ntemplate <class T>\nusing vvvc = vc<vc<vc<T>>>;\n\nusing vb =\
    \ vc<bool>;\nusing vl = vc<ll>;\nusing vpll = vc<pll>;\nusing vtlll = vc<tlll>;\n\
    using vtllll = vc<tllll>;\nusing vstr = vc<string>;\nusing vvb = vvc<bool>;\n\
    using vvl = vvc<ll>;\n\ntemplate <class T>\nusing pql = priority_queue<T, vc<T>,\
    \ greater<T>>;\ntemplate <class T>\nusing pqg = priority_queue<T>;\n\n#ifdef __SIZEOF_INT128__\n\
    \  using i128 = __int128_t;\n  i128 stoi128(const string &s)\n  {\n    i128 res\
    \ = 0;\n    if (s.front() == '-')\n    {\n      for (int i = 1; i < (int)s.size();\
    \ i++)\n        res = 10 * res + s[i] - '0';\n      res = -res;\n    }\n    else\n\
    \    {\n      for (auto &&c : s)\n        res = 10 * res + c - '0';\n    }\n \
    \   return res;\n  }\n  string i128tos(i128 x)\n  {\n    if (x == 0) return \"\
    0\";\n    string sign = \"\", res = \"\";\n    if (x < 0)\n      x = -x, sign\
    \ = \"-\";\n    while (x > 0)\n    {\n      res += '0' + x % 10;\n      x /= 10;\n\
    \    }\n    reverse(res.begin(), res.end());\n    return sign + res;\n  }\n  istream\
    \ &operator>>(istream &is, i128 &a)\n  {\n    string s;\n    is >> s;\n    a =\
    \ stoi128(s);\n    return is;\n  }\n  ostream &operator<<(ostream &os, const i128\
    \ &a)\n  {\n    os << i128tos(a);\n    return os;\n  }\n#endif\n\n#define cauto\
    \ const auto\n#line 2 \"template/template_rep.hpp\"\n\n#line 4 \"template/template_rep.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09\n * @docs\
    \ docs/template/template_rep.md\n */\n\n/**\n * \u53C2\u8003\uFF1A\n * https://trap.jp/post/1224/\n\
    */\n\n#define overload4(_1, _2, _3, _4, name, ...) name\n#define rep1(i, n) for\
    \ (ll i = 0; i < ll(n); i++)\n#define rep2(i, l, r) for (ll i = ll(l); i < ll(r);\
    \ i++)\n#define rep3(i, l, r, d) for (ll i = ll(l); (d) > 0 ? i < ll(r) : i >\
    \ ll(r); i += d)\n#define rep(...) overload4(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)\n\
    #define repi1(i, n) for (int i = 0; i < int(n); i++)\n#define repi2(i, l, r) for\
    \ (int i = int(l); i < int(r); i++)\n#define repi3(i, l, r, d) for (int i = int(l);\
    \ (d) > 0 ? i < int(r) : i > int(r); i += d)\n#define repi(...) overload4(__VA_ARGS__,\
    \ repi3, repi2, repi1)(__VA_ARGS__)\n#line 5 \"template/template_vector.hpp\"\n\
    \n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08vector\uFF09\n * @docs\
    \ docs/template/template_vector.md\n */\n\n#define ALL(a) (a).begin(), (a).end()\n\
    #define SZ(x) (ll)((x).size())\n#define SZI(x) (int)((x).size())\n\ntemplate <class\
    \ F>\nauto gen_vec(const int &n, const F &f)\n{\n  vc<decltype(f(0))> res(n);\n\
    \  repi(i, n) res[i] = f(i);\n  return res;\n}\n\n// https://qiita.com/Chippppp/items/13150f5e0ea99f444d97#%E5%A4%9A%E6%AC%A1%E5%85%83vector%E7%94%9F%E6%88%90%E9%96%A2%E6%95%B0\n\
    template <class T, size_t d, size_t i = 0>\nauto dvec(cauto (&sz)[d], const T\
    \ &init)\n{\n  if constexpr (i < d)\n    return vc(sz[i], dvec<T, d, i + 1>(sz,\
    \ init));\n  else\n    return init;\n}\n\ntemplate <class T = ll>\nT ctol(const\
    \ char &c, const string &s)\n{\n  repi(i, SZI(s)) if (s[i] == c) return i;\n \
    \ return -1;\n}\ntemplate <class T = ll>\nvc<T> stov(const string &s, const char\
    \ &first)\n{\n  return gen_vec(SZI(s), [&](int i) -> T\n                 { return\
    \ s[i] - first; });\n}\ntemplate <class T = ll>\nvc<T> stov(const string &s, const\
    \ string &t)\n{\n  return gen_vec(SZI(s), [&](int i) -> T\n                 {\
    \ return ctol(s[i], t); });\n}\n\ntemplate <class T>\nvc<T> concat(const vvc<T>\
    \ &vs)\n{\n  vc<T> res;\n  for (cauto &v : vs)\n    res.insert(res.end(), ALL(v));\n\
    \  return res;\n}\ntemplate <class T>\nvc<T> concat(const vc<T> &v) { return v;\
    \ }\ntemplate <class T, class... Ts>\nvc<T> concat(vc<T> v, const vc<Ts> &...vs)\n\
    {\n  (v.insert(v.end(), ALL(vs)), ...);\n  return v;\n}\n"
  code: "#pragma once\n\n#include \"template_types.hpp\"\n#include \"template_rep.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08vector\uFF09\n *\
    \ @docs docs/template/template_vector.md\n */\n\n#define ALL(a) (a).begin(), (a).end()\n\
    #define SZ(x) (ll)((x).size())\n#define SZI(x) (int)((x).size())\n\ntemplate <class\
    \ F>\nauto gen_vec(const int &n, const F &f)\n{\n  vc<decltype(f(0))> res(n);\n\
    \  repi(i, n) res[i] = f(i);\n  return res;\n}\n\n// https://qiita.com/Chippppp/items/13150f5e0ea99f444d97#%E5%A4%9A%E6%AC%A1%E5%85%83vector%E7%94%9F%E6%88%90%E9%96%A2%E6%95%B0\n\
    template <class T, size_t d, size_t i = 0>\nauto dvec(cauto (&sz)[d], const T\
    \ &init)\n{\n  if constexpr (i < d)\n    return vc(sz[i], dvec<T, d, i + 1>(sz,\
    \ init));\n  else\n    return init;\n}\n\ntemplate <class T = ll>\nT ctol(const\
    \ char &c, const string &s)\n{\n  repi(i, SZI(s)) if (s[i] == c) return i;\n \
    \ return -1;\n}\ntemplate <class T = ll>\nvc<T> stov(const string &s, const char\
    \ &first)\n{\n  return gen_vec(SZI(s), [&](int i) -> T\n                 { return\
    \ s[i] - first; });\n}\ntemplate <class T = ll>\nvc<T> stov(const string &s, const\
    \ string &t)\n{\n  return gen_vec(SZI(s), [&](int i) -> T\n                 {\
    \ return ctol(s[i], t); });\n}\n\ntemplate <class T>\nvc<T> concat(const vvc<T>\
    \ &vs)\n{\n  vc<T> res;\n  for (cauto &v : vs)\n    res.insert(res.end(), ALL(v));\n\
    \  return res;\n}\ntemplate <class T>\nvc<T> concat(const vc<T> &v) { return v;\
    \ }\ntemplate <class T, class... Ts>\nvc<T> concat(vc<T> v, const vc<Ts> &...vs)\n\
    {\n  (v.insert(v.end(), ALL(vs)), ...);\n  return v;\n}"
  dependsOn:
  - template/template_types.hpp
  - template/template_rep.hpp
  isVerificationFile: false
  path: template/template_vector.hpp
  requiredBy: []
  timestamp: '2024-09-01 17:45:18+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/mytest/template_vector.test.cpp
documentation_of: template/template_vector.hpp
layout: document
redirect_from:
- /library/template/template_vector.hpp
- /library/template/template_vector.hpp.html
title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08vector\uFF09"
---
## テンプレート（vector）

### マクロ

- `ALL(x)`
- `SZ(x)`：vector の長さを `ll` 型で
- `SZI(x)`：vector の長さを `int` 型で

### vector の生成

#### gen_vec

`gen_vec(int n, F f)`

長さ $n$ で、$i$ 番目が $f(i)$ の vector を返す。

### 多次元 vector の生成

#### dvec

多次元 vector を生成する。DP 配列の初期化などで使う。

使用例：

```cpp
// dp[i][j][k] (0<=i<N, 0<=j<M, 0<=k<2)
// 初期値は 0
// 初期値の部分を 0 と書くと値の型が int になるので注意
// N, M が ll の場合、定数は 2LL などとする
auto dp = make_vec({N, M, 2}, 0LL);
```

### 文字列から数値の vector に変換

#### ctol

`T ctol<T = ll>(char c, string s)`

文字と数値の関係を表す文字列 $s$ を使って、文字 $c$ を数値に変換する。たとえば、`ctol(c, "JOI")` なら、`J` $\to 0$, `O` $\to 1$, `I` $\to 2$ という変換をする。

厳密には、$s$ の中に初めて $c$ が現れる index（見つからなければ $-1$）を返す。（基本的に $s$ の文字が異なることを想定しているが、そうでなくても動作する。）

#### stov

```cpp
(1) vc<T = ll> stov(string s, char first)
(2) vc<T = ll> stov(string s, string t)
```

- (1)：$s$ の各文字 `s[i]` を `s[i] - first` として数値に変換し、それを並べた vector を返す。
  - `first` は基本的には `'A'`, `'a'`, `'0'` を使うことが多いと思う。

- (2)：$s$ の各文字 `s[i]` を `ctol(s[i], t)` として数値に変換し、それを並べた vector を返す。

### 結合

#### concat

```cpp
(1) vc<T> concat(vvc<T> vs)
(2) vc<T> concat(vc<T> v, vc<T1> v1, vc<T2> v2, ...)
```

vector を結合したものを返す。(2) では、`v1`, `v2`, $\dots$ の型が必ずしも `T` と一致していなくてもよい（`ll` と `int` など）。