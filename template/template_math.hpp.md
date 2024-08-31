---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: template/template_rep.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_types.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09"
  _extendedRequiredBy:
  - icon: ':warning:'
    path: template/template.cpp
    title: template/template.cpp
  - icon: ':heavy_check_mark:'
    path: template/template_all.hpp
    title: template/template_all.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_math_div.test.cpp
    title: verify/mytest/template_math_div.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_math_mulpow.test.cpp
    title: verify/mytest/template_math_mulpow.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_sgn.test.cpp.cpp
    title: verify/mytest/template_sgn.test.cpp.cpp
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
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    _deprecated_at_docs: docs/template/template_math.md
    document_title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09"
    links:
    - https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html
  bundledCode: "#line 2 \"template/template_math.hpp\"\n\n#ifndef INF\n#define INF\
    \ 4'000'000'000'000'000'037LL\n#endif\n#ifndef EPS\n#define EPS 1e-11\n#endif\n\
    \n#line 2 \"template/template_types.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\
    \u30EC\u30FC\u30C8\uFF08\u578B\uFF09\n * @docs docs/template/template_types.md\n\
    \ */\n\n#include <bits/stdc++.h>\nusing namespace std;\n\n#ifndef EPS\n#define\
    \ EPS 1e-11\n#endif\nusing ld = decltype(EPS);\n\nusing ll = long long;\nusing\
    \ uint = unsigned int;\nusing ull = unsigned long long;\nusing pll = pair<ll,\
    \ ll>;\nusing tlll = tuple<ll, ll, ll>;\nusing tllll = tuple<ll, ll, ll, ll>;\n\
    \n#define vc vector\ntemplate <class T>\nusing vvc = vc<vc<T>>;\ntemplate <class\
    \ T>\nusing vvvc = vc<vc<vc<T>>>;\n\nusing vb = vc<bool>;\nusing vl = vc<ll>;\n\
    using vpll = vc<pll>;\nusing vtlll = vc<tlll>;\nusing vtllll = vc<tllll>;\nusing\
    \ vstr = vc<string>;\nusing vvb = vvc<bool>;\nusing vvl = vvc<ll>;\n\ntemplate\
    \ <class T>\nusing pql = priority_queue<T, vc<T>, greater<T>>;\ntemplate <class\
    \ T>\nusing pqg = priority_queue<T>;\n\n#ifdef __SIZEOF_INT128__\n  using i128\
    \ = __int128_t;\n  i128 stoi128(const string &s)\n  {\n    i128 res = 0;\n   \
    \ if (s.front() == '-')\n    {\n      for (int i = 1; i < (int)s.size(); i++)\n\
    \        res = 10 * res + s[i] - '0';\n      res = -res;\n    }\n    else\n  \
    \  {\n      for (auto &&c : s)\n        res = 10 * res + c - '0';\n    }\n   \
    \ return res;\n  }\n  string i128tos(i128 x)\n  {\n    if (x == 0) return \"0\"\
    ;\n    string sign = \"\", res = \"\";\n    if (x < 0)\n      x = -x, sign = \"\
    -\";\n    while (x > 0)\n    {\n      res += '0' + x % 10;\n      x /= 10;\n \
    \   }\n    reverse(res.begin(), res.end());\n    return sign + res;\n  }\n  istream\
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
    \ repi3, repi2, repi1)(__VA_ARGS__)\n#line 12 \"template/template_math.hpp\"\n\
    \n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09\n\
    \ * @docs docs/template/template_math.md\n */\n\ninline bool chmin(auto &a, cauto\
    \ &b) { return a > b ? a = b, true : false; }\ninline bool chmax(auto &a, cauto\
    \ &b) { return a < b ? a = b, true : false; }\n\ntemplate <class T = ll>\ninline\
    \ T divfloor(cauto &a, cauto &b) { return T(a) / T(b) - (T(a) % T(b) && (T(a)\
    \ ^ T(b)) < 0); }\ntemplate <class T = ll>\ninline T divceil(cauto &a, cauto &b)\
    \ { return T(a) / T(b) + (T(a) % T(b) && (T(a) ^ T(b)) >= 0); }\ntemplate <class\
    \ T = ll>\ninline T safemod(cauto &a, cauto &b) { return a - b * divfloor(a, b);\
    \ }\n\ntemplate <class T = ll>\nconstexpr T ipow(cauto &a, cauto &b)\n{\n  assert(b\
    \ >= 0);\n  if (b == 0) return 1;\n  if (a == 0 || a == 1) return a;\n  if (a\
    \ == -1) return b & 1 ? -1 : 1;\n\n  T res = 1;\n  repi(_, b) res *= T(a);\n \
    \ return res;\n}\ntemplate <class T = ll>\nT mul_limited(cauto &a, cauto &b, cauto\
    \ &m = INF)\n{\n  assert(a >= 0 && b >= 0 && m >= 0);\n  if (b == 0)\n    return\
    \ 0;\n  return T(a) > T(m) / T(b) ? T(m) : T(a) * T(b);\n}\ntemplate <class T\
    \ = ll>\nT pow_limited(cauto &a, cauto &b, cauto &m = INF)\n{\n  assert(a >= 0\
    \ && b >= 0 && m >= 0);\n  if (a <= 1 || b == 0)\n    return min(ipow<T>(a, b),\
    \ T(m));\n  \n  T res = 1;\n  repi(_, b)\n  {\n    if (res > T(m) / T(a))\n  \
    \    return T(m);\n    res *= T(a);\n  }\n  return res;\n}\n\ntemplate <class\
    \ T = ll>\nconstexpr T iroot(cauto &a, cauto &k)\n{\n  assert(a >= 0 && k >= 1);\n\
    \  if (a <= 1 || k == 1)\n    return a;\n\n  auto isok = [&](const T &x) -> bool\n\
    \  {\n    if (x == 0)\n      return true;\n    T tmp = 1;\n    repi(_, k)\n  \
    \  {\n      if (tmp > T(a) / x)\n        return false;\n      tmp *= x;\n    }\n\
    \    return tmp <= T(a);\n  };\n\n  T ok = 0, ng = 1;\n  while (isok(ng))\n  \
    \  ok = ng, ng <<= 1;\n  while (ng - ok > 1)\n  {\n    T mid = ((ng - ok) >> 1)\
    \ + ok;\n    if (isok(mid))\n      ok = mid;\n    else\n      ng = mid;\n  }\n\
    \  return ok;\n}\n\n// https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html\n\
    template <class D = decltype(EPS)>\nint sgn(cauto &a, const D &eps = EPS) { return\
    \ int(a > eps) - int(a < -eps); }\n"
  code: "#pragma once\n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n#endif\n\
    #ifndef EPS\n#define EPS 1e-11\n#endif\n\n#include \"template_types.hpp\"\n#include\
    \ \"template_rep.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\
    \u6F14\u7B97\uFF09\n * @docs docs/template/template_math.md\n */\n\ninline bool\
    \ chmin(auto &a, cauto &b) { return a > b ? a = b, true : false; }\ninline bool\
    \ chmax(auto &a, cauto &b) { return a < b ? a = b, true : false; }\n\ntemplate\
    \ <class T = ll>\ninline T divfloor(cauto &a, cauto &b) { return T(a) / T(b) -\
    \ (T(a) % T(b) && (T(a) ^ T(b)) < 0); }\ntemplate <class T = ll>\ninline T divceil(cauto\
    \ &a, cauto &b) { return T(a) / T(b) + (T(a) % T(b) && (T(a) ^ T(b)) >= 0); }\n\
    template <class T = ll>\ninline T safemod(cauto &a, cauto &b) { return a - b *\
    \ divfloor(a, b); }\n\ntemplate <class T = ll>\nconstexpr T ipow(cauto &a, cauto\
    \ &b)\n{\n  assert(b >= 0);\n  if (b == 0) return 1;\n  if (a == 0 || a == 1)\
    \ return a;\n  if (a == -1) return b & 1 ? -1 : 1;\n\n  T res = 1;\n  repi(_,\
    \ b) res *= T(a);\n  return res;\n}\ntemplate <class T = ll>\nT mul_limited(cauto\
    \ &a, cauto &b, cauto &m = INF)\n{\n  assert(a >= 0 && b >= 0 && m >= 0);\n  if\
    \ (b == 0)\n    return 0;\n  return T(a) > T(m) / T(b) ? T(m) : T(a) * T(b);\n\
    }\ntemplate <class T = ll>\nT pow_limited(cauto &a, cauto &b, cauto &m = INF)\n\
    {\n  assert(a >= 0 && b >= 0 && m >= 0);\n  if (a <= 1 || b == 0)\n    return\
    \ min(ipow<T>(a, b), T(m));\n  \n  T res = 1;\n  repi(_, b)\n  {\n    if (res\
    \ > T(m) / T(a))\n      return T(m);\n    res *= T(a);\n  }\n  return res;\n}\n\
    \ntemplate <class T = ll>\nconstexpr T iroot(cauto &a, cauto &k)\n{\n  assert(a\
    \ >= 0 && k >= 1);\n  if (a <= 1 || k == 1)\n    return a;\n\n  auto isok = [&](const\
    \ T &x) -> bool\n  {\n    if (x == 0)\n      return true;\n    T tmp = 1;\n  \
    \  repi(_, k)\n    {\n      if (tmp > T(a) / x)\n        return false;\n     \
    \ tmp *= x;\n    }\n    return tmp <= T(a);\n  };\n\n  T ok = 0, ng = 1;\n  while\
    \ (isok(ng))\n    ok = ng, ng <<= 1;\n  while (ng - ok > 1)\n  {\n    T mid =\
    \ ((ng - ok) >> 1) + ok;\n    if (isok(mid))\n      ok = mid;\n    else\n    \
    \  ng = mid;\n  }\n  return ok;\n}\n\n// https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html\n\
    template <class D = decltype(EPS)>\nint sgn(cauto &a, const D &eps = EPS) { return\
    \ int(a > eps) - int(a < -eps); }"
  dependsOn:
  - template/template_types.hpp
  - template/template_rep.hpp
  isVerificationFile: false
  path: template/template_math.hpp
  requiredBy:
  - template/template.cpp
  - template/template_all.hpp
  timestamp: '2024-08-31 12:06:34+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/mytest/template_math_div.test.cpp
  - verify/mytest/template_math_mulpow.test.cpp
  - verify/mytest/template_sgn.test.cpp.cpp
  - verify/yosupo/kth_root_integer.test.cpp
  - verify/yosupo/aplusb.test.cpp
  - verify/yosupo/many_aplusb_128bit.test.cpp
  - verify/yosupo/many_aplusb.test.cpp
  - verify/yosupo/many_aplusb_tuple.test.cpp
documentation_of: template/template_math.hpp
layout: document
redirect_from:
- /library/template/template_math.hpp
- /library/template/template_math.hpp.html
title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09"
---
## テンプレート（演算）

### 定数

これらは必要に応じて書き換える（そのために `#define` で定義している）。

- `INF` $= 4 \times 10^{18} + 37$
  - $\in [2^{61}, 2^{62})$
  - `INF + INF` はオーバーフローしない
  - 一応素数

- `EPS` $= 10^{-11}$ (double)

### chmin, chmax

更新されたかどうかの bool が返る。

### 除算

負の場合の除算（切り捨て・切り上げ・あまり）にも対応したもの。

#### divfloor

`T divfloor<T = ll>(a, b)`

$\lfloor a / b \rfloor$ を返す。$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は符号つき整数
- $b \neq 0$
- $a, b$ は型 `T` の上限・下限付近ではない

#### divceil

`T divceil<T = ll>(a, b)`

$\lceil a / b \rceil$ を返す。$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は符号つき整数
- $b \neq 0$
- $a, b$ は型 `T` の上限・下限付近ではない

#### safemod

`T safemod<T = ll>(a, b)`

$b > 0$ のとき、$a \bmod b$ を $[0, b)$ の範囲で返す。（基本的には $b > 0$ での使用を想定している。）

$b < 0$ のとき、$-\{(-a) \bmod (-b)\}$ を返す。すなわち、$(b, 0]$ の範囲の値が返る。

いずれの場合も、$a - x$ が $b$ の倍数となるような $x$ のうち $0$ と $b$ の間（$b$ は含まない）にあるものを返す、と解釈できる。

$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は符号つき整数
- $b \neq 0$
- $a, b$ は型 `T` の上限・下限付近ではない

### 累乗・ルート

#### ipow

`T ipow<T = ll>(a, b)`

$a^b$ を返す（$0^0$ は $1$ とする）。$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は整数
- $b \geq 0$
- $a^b$ が `T` に収まる

##### 計算量

- $a \in \{-1,0,1\}$ のとき、$O(1)$
- それ以外のとき、$O(b)$

#### iroot

`T iroot<T = ll>(a, k)`

$\mathrm{floor}(a^{1/k})$ を返す。$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は整数
- $0 \leq a$
- $1 \leq k$

##### 計算量

- $a,k \geq 2$ のとき、$O(\log a)$
- それ以外のとき、$O(1)$

### 上限つきの乗算・累乗

（メモ：`__builtin_mul_overflow` というものもある）

#### mul_limited

`T mul_limited<T = ll>(a, b, m = INF)`

$\min(a \times b, m)$ を返す。$a, b, m$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は整数
- $a, b, m \geq 0$

#### pow_limited

`T pow_limited<T = ll>(a, b, m = INF)`

$\min(a^b, m)$ を返す。$a, b, m$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は整数
- $a, b, m \geq 0$

### 符号関数（誤差を考慮）

https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html

#### sgn

`int sgn<D = ld>(a, D eps = EPS)`

誤差 $\mathrm{eps}$ を考慮した $a$ の符号を、$-1, 0, 1$ で返す。

整数の場合もこれを使ってよい。