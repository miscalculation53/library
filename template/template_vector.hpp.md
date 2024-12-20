---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: template/template_math.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_rep.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_types.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09"
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: math/extgcd.hpp
    title: "\u62E1\u5F35\u30E6\u30FC\u30AF\u30EA\u30C3\u30C9\u4E92\u9664\u6CD5 (extgcd)"
  - icon: ':heavy_check_mark:'
    path: math/modint/modint.hpp
    title: modint (32 bit)
  - icon: ':heavy_check_mark:'
    path: math/modint/modint64.hpp
    title: modint (64 bit)
  - icon: ':heavy_check_mark:'
    path: math/modint/modint_internal.hpp
    title: math/modint/modint_internal.hpp
  - icon: ':heavy_check_mark:'
    path: math/prime/primality_test.hpp
    title: "\u7D20\u6570\u5224\u5B9A"
  - icon: ':warning:'
    path: template/template.cpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5168\u4F53\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_algo.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30A2\u30EB\u30B4\u30EA\u30BA\
      \u30E0\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_all.hpp
    title: template/template_all.hpp
  - icon: ':heavy_check_mark:'
    path: template/template_binsearch.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u4E8C\u5206\u63A2\u7D22\uFF09"
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/aoj/extgcd.test.cpp
    title: verify/aoj/extgcd.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/modint.test.cpp
    title: verify/mytest/modint.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/modint64.test.cpp
    title: verify/mytest/modint64.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_algo.test.cpp
    title: verify/mytest/template_algo.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_binsearch.test.cpp
    title: verify/mytest/template_binsearch.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_bit.test.cpp
    title: verify/mytest/template_bit.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_vector.test.cpp
    title: verify/mytest/template_vector.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/aplusb.test.cpp
    title: verify/yosupo/aplusb.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/kth_root_integer.test.cpp
    title: verify/yosupo/kth_root_integer.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/many_aplusb.test.cpp
    title: verify/yosupo/many_aplusb.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/many_aplusb_128bit.test.cpp
    title: verify/yosupo/many_aplusb_128bit.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/many_aplusb_tuple.test.cpp
    title: verify/yosupo/many_aplusb_tuple.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/primality_test.test.cpp
    title: verify/yosupo/primality_test.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yukicoder/yuki1550_dynamic_modint.test.cpp
    title: verify/yukicoder/yuki1550_dynamic_modint.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yukicoder/yuki1550_static_modint.test.cpp
    title: verify/yukicoder/yuki1550_static_modint.test.cpp
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
    using i128 = __int128_t;\nusing u128 = __uint128_t;\ni128 stoi128(const string\
    \ &s)\n{\n  i128 res = 0;\n  if (s.front() == '-')\n  {\n    for (int i = 1; i\
    \ < (int)s.size(); i++)\n      res = 10 * res + s[i] - '0';\n    res = -res;\n\
    \  }\n  else\n  {\n    for (auto &&c : s)\n      res = 10 * res + c - '0';\n \
    \ }\n  return res;\n}\nstring i128tos(i128 x)\n{\n  if (x == 0) return \"0\";\n\
    \  string sign = \"\", res = \"\";\n  if (x < 0)\n    x = -x, sign = \"-\";\n\
    \  while (x > 0)\n  {\n    res += '0' + x % 10;\n    x /= 10;\n  }\n  reverse(res.begin(),\
    \ res.end());\n  return sign + res;\n}\nistream &operator>>(istream &is, i128\
    \ &a)\n{\n  string s;\n  is >> s;\n  a = stoi128(s);\n  return is;\n}\nostream\
    \ &operator<<(ostream &os, const i128 &a)\n{\n  os << i128tos(a);\n  return os;\n\
    }\n#endif\n\n#define cauto const auto\n#line 2 \"template/template_rep.hpp\"\n\
    \n#line 4 \"template/template_rep.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\
    \u30FC\u30C8\uFF08rep\uFF09\n * @docs docs/template/template_rep.md\n */\n\n/**\n\
    \ * \u53C2\u8003\uFF1A\n * https://trap.jp/post/1224/\n*/\n\n#define overload4(_1,\
    \ _2, _3, _4, name, ...) name\n#define rep1(i, n) for (ll i = 0, nnnnn = ll(n);\
    \ i < nnnnn; i++)\n#define rep2(i, l, r) for (ll i = ll(l), rrrrr = ll(r); i <\
    \ rrrrr; i++)\n#define rep3(i, l, r, d) for (ll i = ll(l), rrrrr = ll(r), ddddd\
    \ = ll(d); ddddd > 0 ? i < rrrrr : i > rrrrr; i += d)\n#define rep(...) overload4(__VA_ARGS__,\
    \ rep3, rep2, rep1)(__VA_ARGS__)\n#define repi1(i, n) for (int i = 0, nnnnn =\
    \ int(n); i < nnnnn; i++)\n#define repi2(i, l, r) for (int i = int(l), rrrrr =\
    \ int(r); i < rrrrr; i++)\n#define repi3(i, l, r, d) for (int i = int(l), rrrrr\
    \ = int(r), ddddd = int(d); ddddd > 0 ? i < rrrrr : i > rrrrr; i += d)\n#define\
    \ repi(...) overload4(__VA_ARGS__, repi3, repi2, repi1)(__VA_ARGS__)\n\n#define\
    \ fe(...) for (auto __VA_ARGS__)\n#define fec(...) for (cauto &__VA_ARGS__)\n\
    #define fem(...) for (auto &__VA_ARGS__)\n#line 2 \"template/template_math.hpp\"\
    \n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n#endif\n#ifndef EPS\n\
    #define EPS 1e-11\n#endif\n\n#line 12 \"template/template_math.hpp\"\n\n/**\n\
    \ * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09\n * @docs\
    \ docs/template/template_math.md\n */\n\ninline bool chmin(auto &a, cauto &b)\
    \ { return a > b ? a = b, true : false; }\ninline bool chmax(auto &a, cauto &b)\
    \ { return a < b ? a = b, true : false; }\n\ntemplate <class T = ll>\ninline constexpr\
    \ T divfloor(cauto &a, cauto &b) { return T(a) / T(b) - (T(a) % T(b) && (T(a)\
    \ ^ T(b)) < 0); }\ntemplate <class T = ll>\ninline constexpr T divceil(cauto &a,\
    \ cauto &b) { return T(a) / T(b) + (T(a) % T(b) && (T(a) ^ T(b)) >= 0); }\ntemplate\
    \ <class T = ll>\ninline constexpr T divround(cauto &a, cauto &b) { return divfloor<T>(2\
    \ * T(a) + T(b), 2 * T(b)); }\ntemplate <class T = ll>\ninline constexpr T safemod(cauto\
    \ &a, cauto &b) { return T(a) - T(b) * divfloor<T>(a, b); }\n\ntemplate <class\
    \ T = ll>\nconstexpr T ipow(auto a, auto b)\n{\n  assert(b >= 0);\n  if (b ==\
    \ 0) return 1;\n  if (a == 0 || a == 1) return a;\n  if (a == -1) return b & 1\
    \ ? -1 : 1;\n\n  T res = 1, tmp = a;\n  while (b > 0)\n  {\n    if (b & 1)\n \
    \     res *= tmp;\n    tmp *= tmp;\n    b >>= 1;\n  }\n  return res;\n}\ntemplate\
    \ <class T = ll>\nT mul_limited(cauto &a, cauto &b, cauto &m = INF)\n{\n  assert(a\
    \ >= 0 && b >= 0 && m >= 0);\n  if (b == 0)\n    return 0;\n  return T(a) > T(m)\
    \ / T(b) ? T(m) : T(a) * T(b);\n}\ntemplate <class T = ll>\nT pow_limited(cauto\
    \ &a, cauto &b, cauto &m = INF)\n{\n  assert(a >= 0 && b >= 0 && m >= 0);\n  if\
    \ (a <= 1 || b == 0)\n    return min(ipow<T>(a, b), T(m));\n  \n  T res = 1;\n\
    \  repi(_, b)\n  {\n    if (res > T(m) / T(a))\n      return T(m);\n    res *=\
    \ T(a);\n  }\n  return res;\n}\n\ntemplate <class T = ll>\nconstexpr T iroot(cauto\
    \ &a, cauto &k)\n{\n  assert(a >= 0 && k >= 1);\n  if (a <= 1 || k == 1)\n   \
    \ return a;\n\n  auto isok = [&](const T &x) -> bool\n  {\n    if (x == 0)\n \
    \     return true;\n    T tmp = 1;\n    repi(_, k)\n    {\n      if (tmp > T(a)\
    \ / x)\n        return false;\n      tmp *= x;\n    }\n    return tmp <= T(a);\n\
    \  };\n\n  T ok = 0, ng = 1;\n  while (isok(ng))\n    ok = ng, ng <<= 1;\n  while\
    \ (ng - ok > 1)\n  {\n    T mid = ((ng - ok) >> 1) + ok;\n    if (isok(mid))\n\
    \      ok = mid;\n    else\n      ng = mid;\n  }\n  return ok;\n}\n\n// https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html\n\
    template <class D = decltype(EPS)>\nint sgn(cauto &a, const D &eps = EPS) { return\
    \ int(a > eps) - int(a < -eps); }\n\n// \u4F4D\u53D6\u308A\u8A18\u6570\u6CD5\u3068\
    \u540C\u3058\u9806\u756A\uFF08\u4E0B\u4F4D\u6841\u304C\u5F8C\u308D\uFF09\ntemplate\
    \ <class T = ll>\nvc<T> b_ary(cauto &x, const int &b)\n{\n  vc<T> a;\n  while\
    \ (x > 0)\n  {\n    a.emplace_back(x % b);\n    x /= b;\n  }\n  reverse(a.begin(),\
    \ a.end());\n  return a;\n}\n// \u4F4D\u53D6\u308A\u8A18\u6570\u6CD5\u3068\u540C\
    \u3058\u9806\u756A\uFF08\u4E0B\u4F4D\u6841\u304C\u5F8C\u308D\uFF09\ntemplate <class\
    \ T>\nvc<T> b_ary(cauto &x, const int &b, const int &n)\n{\n  vc<T> a(n);\n  repi(i,\
    \ n)\n  {\n    a[i] = x % b;\n    x /= b;\n  }\n  reverse(a.begin(), a.end());\n\
    \  return a;\n}\nstring b_ary_str(cauto &x, const int &b, bool use_upper = true)\n\
    {\n  auto a = b_ary(x, b);\n  string s = \"\";\n  for (cauto &ai : a)\n    s +=\
    \ (ai < 10 ? '0' + ai : (use_upper ? 'A' : 'a') + (ai - 10));\n  return s;\n}\n\
    string b_ary_str(cauto &x, const int &b, const int &n, bool use_upper = true)\n\
    {\n  auto a = b_ary(x, b, n);\n  string s = \"\";\n  for (cauto &ai : a)\n   \
    \ s += (ai < 10 ? '0' + ai : (use_upper ? 'A' : 'a') + (ai - 10));\n  return s;\n\
    }\n#line 6 \"template/template_vector.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\
    \u30EC\u30FC\u30C8\uFF08vector\uFF09\n * @docs docs/template/template_vector.md\n\
    \ */\n\n#define ALL(a) (a).begin(), (a).end()\n#define SZ(x) (ll)((x).size())\n\
    #define SZI(x) (int)((x).size())\n\ntemplate <class F>\nauto gen_vec(const int\
    \ &n, const F &f)\n{\n  vc<decltype(f(0))> res(n);\n  repi(i, n) res[i] = f(i);\n\
    \  return res;\n}\n\n// https://qiita.com/Chippppp/items/13150f5e0ea99f444d97#%E5%A4%9A%E6%AC%A1%E5%85%83vector%E7%94%9F%E6%88%90%E9%96%A2%E6%95%B0\n\
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
    {\n  (v.insert(v.end(), ALL(vs)), ...);\n  return v;\n}\n\ntemplate <class T>\n\
    T vecget(const vc<T> &v, cauto &i, const T &dflt_negative = -INF, const T &dflt_positive\
    \ = INF)\n{\n  if (i < 0)\n    return dflt_negative;\n  if (i >= SZI(v))\n   \
    \ return dflt_positive;\n  return v[i];\n}\n"
  code: "#pragma once\n\n#include \"template_types.hpp\"\n#include \"template_rep.hpp\"\
    \n#include \"template_math.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\
    \u30C8\uFF08vector\uFF09\n * @docs docs/template/template_vector.md\n */\n\n#define\
    \ ALL(a) (a).begin(), (a).end()\n#define SZ(x) (ll)((x).size())\n#define SZI(x)\
    \ (int)((x).size())\n\ntemplate <class F>\nauto gen_vec(const int &n, const F\
    \ &f)\n{\n  vc<decltype(f(0))> res(n);\n  repi(i, n) res[i] = f(i);\n  return\
    \ res;\n}\n\n// https://qiita.com/Chippppp/items/13150f5e0ea99f444d97#%E5%A4%9A%E6%AC%A1%E5%85%83vector%E7%94%9F%E6%88%90%E9%96%A2%E6%95%B0\n\
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
    {\n  (v.insert(v.end(), ALL(vs)), ...);\n  return v;\n}\n\ntemplate <class T>\n\
    T vecget(const vc<T> &v, cauto &i, const T &dflt_negative = -INF, const T &dflt_positive\
    \ = INF)\n{\n  if (i < 0)\n    return dflt_negative;\n  if (i >= SZI(v))\n   \
    \ return dflt_positive;\n  return v[i];\n}"
  dependsOn:
  - template/template_types.hpp
  - template/template_rep.hpp
  - template/template_math.hpp
  isVerificationFile: false
  path: template/template_vector.hpp
  requiredBy:
  - math/modint/modint_internal.hpp
  - math/modint/modint.hpp
  - math/modint/modint64.hpp
  - math/prime/primality_test.hpp
  - math/extgcd.hpp
  - template/template.cpp
  - template/template_all.hpp
  - template/template_algo.hpp
  - template/template_binsearch.hpp
  timestamp: '2024-12-20 09:47:18+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo/many_aplusb_128bit.test.cpp
  - verify/yosupo/kth_root_integer.test.cpp
  - verify/yosupo/many_aplusb_tuple.test.cpp
  - verify/yosupo/primality_test.test.cpp
  - verify/yosupo/aplusb.test.cpp
  - verify/yosupo/many_aplusb.test.cpp
  - verify/mytest/modint.test.cpp
  - verify/mytest/template_algo.test.cpp
  - verify/mytest/template_vector.test.cpp
  - verify/mytest/template_binsearch.test.cpp
  - verify/mytest/template_bit.test.cpp
  - verify/mytest/modint64.test.cpp
  - verify/aoj/extgcd.test.cpp
  - verify/yukicoder/yuki1550_static_modint.test.cpp
  - verify/yukicoder/yuki1550_dynamic_modint.test.cpp
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

```cpp
gen_vec(int n, F f)
```

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

### 取得

#### vecget

```cpp
T vecget(vc<T> v, int i, T dflt_negative = -INF, T dflt_positive = INF)
```

範囲外も考慮した vector の取得。範囲内なら通常の `[]` や `at` と同じで、範囲外なら負方向は `dflt_negative`, 正方向は `dflt_positive` を返す。

用途としては、ソート済み配列が無限に広がっていると考えたい場合（特に、`binsearch` や `expsearch` に渡すとき）。`LB` や `UB` だと頭が壊れるとき用に。