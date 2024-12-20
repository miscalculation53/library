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
    title: math/prime/primality_test.hpp
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
  - icon: ':heavy_check_mark:'
    path: template/template_vector.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08vector\uFF09"
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
    path: verify/mytest/template_math_div.test.cpp
    title: verify/mytest/template_math_div.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_math_mulpow.test.cpp
    title: verify/mytest/template_math_mulpow.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_sgn.test.cpp.cpp
    title: verify/mytest/template_sgn.test.cpp.cpp
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
    \ T>\nusing pqg = priority_queue<T>;\n\n#ifdef __SIZEOF_INT128__\nusing i128 =\
    \ __int128_t;\nusing u128 = __uint128_t;\ni128 stoi128(const string &s)\n{\n \
    \ i128 res = 0;\n  if (s.front() == '-')\n  {\n    for (int i = 1; i < (int)s.size();\
    \ i++)\n      res = 10 * res + s[i] - '0';\n    res = -res;\n  }\n  else\n  {\n\
    \    for (auto &&c : s)\n      res = 10 * res + c - '0';\n  }\n  return res;\n\
    }\nstring i128tos(i128 x)\n{\n  if (x == 0) return \"0\";\n  string sign = \"\"\
    , res = \"\";\n  if (x < 0)\n    x = -x, sign = \"-\";\n  while (x > 0)\n  {\n\
    \    res += '0' + x % 10;\n    x /= 10;\n  }\n  reverse(res.begin(), res.end());\n\
    \  return sign + res;\n}\nistream &operator>>(istream &is, i128 &a)\n{\n  string\
    \ s;\n  is >> s;\n  a = stoi128(s);\n  return is;\n}\nostream &operator<<(ostream\
    \ &os, const i128 &a)\n{\n  os << i128tos(a);\n  return os;\n}\n#endif\n\n#define\
    \ cauto const auto\n#line 2 \"template/template_rep.hpp\"\n\n#line 4 \"template/template_rep.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09\n * @docs\
    \ docs/template/template_rep.md\n */\n\n/**\n * \u53C2\u8003\uFF1A\n * https://trap.jp/post/1224/\n\
    */\n\n#define overload4(_1, _2, _3, _4, name, ...) name\n#define rep1(i, n) for\
    \ (ll i = 0, nnnnn = ll(n); i < nnnnn; i++)\n#define rep2(i, l, r) for (ll i =\
    \ ll(l), rrrrr = ll(r); i < rrrrr; i++)\n#define rep3(i, l, r, d) for (ll i =\
    \ ll(l), rrrrr = ll(r), ddddd = ll(d); ddddd > 0 ? i < rrrrr : i > rrrrr; i +=\
    \ d)\n#define rep(...) overload4(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)\n\
    #define repi1(i, n) for (int i = 0, nnnnn = int(n); i < nnnnn; i++)\n#define repi2(i,\
    \ l, r) for (int i = int(l), rrrrr = int(r); i < rrrrr; i++)\n#define repi3(i,\
    \ l, r, d) for (int i = int(l), rrrrr = int(r), ddddd = int(d); ddddd > 0 ? i\
    \ < rrrrr : i > rrrrr; i += d)\n#define repi(...) overload4(__VA_ARGS__, repi3,\
    \ repi2, repi1)(__VA_ARGS__)\n\n#define fe(...) for (auto __VA_ARGS__)\n#define\
    \ fec(...) for (cauto &__VA_ARGS__)\n#define fem(...) for (auto &__VA_ARGS__)\n\
    #line 12 \"template/template_math.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\
    \u30FC\u30C8\uFF08\u6F14\u7B97\uFF09\n * @docs docs/template/template_math.md\n\
    \ */\n\ninline bool chmin(auto &a, cauto &b) { return a > b ? a = b, true : false;\
    \ }\ninline bool chmax(auto &a, cauto &b) { return a < b ? a = b, true : false;\
    \ }\n\ntemplate <class T = ll>\ninline constexpr T divfloor(cauto &a, cauto &b)\
    \ { return T(a) / T(b) - (T(a) % T(b) && (T(a) ^ T(b)) < 0); }\ntemplate <class\
    \ T = ll>\ninline constexpr T divceil(cauto &a, cauto &b) { return T(a) / T(b)\
    \ + (T(a) % T(b) && (T(a) ^ T(b)) >= 0); }\ntemplate <class T = ll>\ninline constexpr\
    \ T divround(cauto &a, cauto &b) { return divfloor<T>(2 * T(a) + T(b), 2 * T(b));\
    \ }\ntemplate <class T = ll>\ninline constexpr T safemod(cauto &a, cauto &b) {\
    \ return T(a) - T(b) * divfloor<T>(a, b); }\n\ntemplate <class T = ll>\nconstexpr\
    \ T ipow(auto a, auto b)\n{\n  assert(b >= 0);\n  if (b == 0) return 1;\n  if\
    \ (a == 0 || a == 1) return a;\n  if (a == -1) return b & 1 ? -1 : 1;\n\n  T res\
    \ = 1, tmp = a;\n  while (b > 0)\n  {\n    if (b & 1)\n      res *= tmp;\n   \
    \ tmp *= tmp;\n    b >>= 1;\n  }\n  return res;\n}\ntemplate <class T = ll>\n\
    T mul_limited(cauto &a, cauto &b, cauto &m = INF)\n{\n  assert(a >= 0 && b >=\
    \ 0 && m >= 0);\n  if (b == 0)\n    return 0;\n  return T(a) > T(m) / T(b) ? T(m)\
    \ : T(a) * T(b);\n}\ntemplate <class T = ll>\nT pow_limited(cauto &a, cauto &b,\
    \ cauto &m = INF)\n{\n  assert(a >= 0 && b >= 0 && m >= 0);\n  if (a <= 1 || b\
    \ == 0)\n    return min(ipow<T>(a, b), T(m));\n  \n  T res = 1;\n  repi(_, b)\n\
    \  {\n    if (res > T(m) / T(a))\n      return T(m);\n    res *= T(a);\n  }\n\
    \  return res;\n}\n\ntemplate <class T = ll>\nconstexpr T iroot(cauto &a, cauto\
    \ &k)\n{\n  assert(a >= 0 && k >= 1);\n  if (a <= 1 || k == 1)\n    return a;\n\
    \n  auto isok = [&](const T &x) -> bool\n  {\n    if (x == 0)\n      return true;\n\
    \    T tmp = 1;\n    repi(_, k)\n    {\n      if (tmp > T(a) / x)\n        return\
    \ false;\n      tmp *= x;\n    }\n    return tmp <= T(a);\n  };\n\n  T ok = 0,\
    \ ng = 1;\n  while (isok(ng))\n    ok = ng, ng <<= 1;\n  while (ng - ok > 1)\n\
    \  {\n    T mid = ((ng - ok) >> 1) + ok;\n    if (isok(mid))\n      ok = mid;\n\
    \    else\n      ng = mid;\n  }\n  return ok;\n}\n\n// https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html\n\
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
    }\n"
  code: "#pragma once\n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n#endif\n\
    #ifndef EPS\n#define EPS 1e-11\n#endif\n\n#include \"template_types.hpp\"\n#include\
    \ \"template_rep.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\
    \u6F14\u7B97\uFF09\n * @docs docs/template/template_math.md\n */\n\ninline bool\
    \ chmin(auto &a, cauto &b) { return a > b ? a = b, true : false; }\ninline bool\
    \ chmax(auto &a, cauto &b) { return a < b ? a = b, true : false; }\n\ntemplate\
    \ <class T = ll>\ninline constexpr T divfloor(cauto &a, cauto &b) { return T(a)\
    \ / T(b) - (T(a) % T(b) && (T(a) ^ T(b)) < 0); }\ntemplate <class T = ll>\ninline\
    \ constexpr T divceil(cauto &a, cauto &b) { return T(a) / T(b) + (T(a) % T(b)\
    \ && (T(a) ^ T(b)) >= 0); }\ntemplate <class T = ll>\ninline constexpr T divround(cauto\
    \ &a, cauto &b) { return divfloor<T>(2 * T(a) + T(b), 2 * T(b)); }\ntemplate <class\
    \ T = ll>\ninline constexpr T safemod(cauto &a, cauto &b) { return T(a) - T(b)\
    \ * divfloor<T>(a, b); }\n\ntemplate <class T = ll>\nconstexpr T ipow(auto a,\
    \ auto b)\n{\n  assert(b >= 0);\n  if (b == 0) return 1;\n  if (a == 0 || a ==\
    \ 1) return a;\n  if (a == -1) return b & 1 ? -1 : 1;\n\n  T res = 1, tmp = a;\n\
    \  while (b > 0)\n  {\n    if (b & 1)\n      res *= tmp;\n    tmp *= tmp;\n  \
    \  b >>= 1;\n  }\n  return res;\n}\ntemplate <class T = ll>\nT mul_limited(cauto\
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
    }"
  dependsOn:
  - template/template_types.hpp
  - template/template_rep.hpp
  isVerificationFile: false
  path: template/template_math.hpp
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
  - template/template_vector.hpp
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
  - verify/mytest/template_math_mulpow.test.cpp
  - verify/mytest/modint64.test.cpp
  - verify/mytest/template_sgn.test.cpp.cpp
  - verify/mytest/template_math_div.test.cpp
  - verify/aoj/extgcd.test.cpp
  - verify/yukicoder/yuki1550_static_modint.test.cpp
  - verify/yukicoder/yuki1550_dynamic_modint.test.cpp
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

```cpp
T divfloor<T = ll>(a, b)
```

$\lfloor a / b \rfloor$ を返す。$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は符号つき整数
- $b \neq 0$
- $a, b$ は型 `T` の上限・下限付近ではない

#### divceil

```cpp
T divceil<T = ll>(a, b)
```

$\lceil a / b \rceil$ を返す。$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は符号つき整数
- $b \neq 0$
- $a, b$ は型 `T` の上限・下限付近ではない

#### divround

```cpp
T divround<T = ll>(a, b)
```

$a / b$ を四捨五入した整数を返す（$.5$ は切り上げ）。$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は符号つき整数
- $b \neq 0$
- $a, b$ は型 `T` の上限・下限付近ではない

#### safemod

```cpp
T safemod<T = ll>(a, b)
```

$b > 0$ のとき、$a \bmod b$ を $[0, b)$ の範囲で返す。（基本的には $b > 0$ での使用を想定している。）

$b < 0$ のとき、$-\lbrace (-a) \bmod (-b) \rbrace$ を返す。すなわち、$(b, 0]$ の範囲の値が返る。

いずれの場合も、$a - x$ が $b$ の倍数となるような $x$ のうち $0$ と $b$ の間（$b$ は含まない）にあるものを返す、と解釈できる。

$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は符号つき整数
- $b \neq 0$
- $a, b$ は型 `T` の上限・下限付近ではない

### 累乗・ルート

#### ipow

```cpp
T ipow<T = ll>(a, b)
```

$a^b$ を返す（$0^0$ は $1$ とする）。$a, b$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は整数
- $b \geq 0$
- $a^b$ が `T` に収まる

##### 計算量

- $a \in \lbrace -1,0,1 \rbrace$ のとき、$O(1)$
- それ以外のとき、$O(\log b)$

#### iroot

```cpp
T iroot<T = ll>(a, k)
```

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

```cpp
T mul_limited<T = ll>(a, b, m = INF)
```

$\min(a \times b, m)$ を返す。$a, b, m$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は整数
- $a, b, m \geq 0$

#### pow_limited

```cpp
T pow_limited<T = ll>(a, b, m = INF)
```

$\min(a^b, m)$ を返す。$a, b, m$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は整数
- $a, b, m \geq 0$

### 符号関数（誤差を考慮）

https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html

#### sgn

```cpp
int sgn<D = ld>(a, D eps = EPS)
```

誤差 $\mathrm{eps}$ を考慮した $a$ の符号を、$-1, 0, 1$ で返す。

整数の場合もこれを使ってよい。