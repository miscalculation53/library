---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
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
    \ docs/template/template_rep.md\n */\n\n// https://trap.jp/post/1224/\n\n#define\
    \ overload4(_1, _2, _3, _4, name, ...) name\n#define rep1(i, n) for (ll i = 0,\
    \ nnnnn = ll(n); i < nnnnn; i++)\n#define rep2(i, l, r) for (ll i = ll(l), rrrrr\
    \ = ll(r); i < rrrrr; i++)\n#define rep3(i, l, r, d) for (ll i = ll(l), rrrrr\
    \ = ll(r), ddddd = ll(d); ddddd > 0 ? i < rrrrr : i > rrrrr; i += d)\n#define\
    \ rep(...) overload4(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)\n#define repi1(i,\
    \ n) for (int i = 0, nnnnn = int(n); i < nnnnn; i++)\n#define repi2(i, l, r) for\
    \ (int i = int(l), rrrrr = int(r); i < rrrrr; i++)\n#define repi3(i, l, r, d)\
    \ for (int i = int(l), rrrrr = int(r), ddddd = int(d); ddddd > 0 ? i < rrrrr :\
    \ i > rrrrr; i += d)\n#define repi(...) overload4(__VA_ARGS__, repi3, repi2, repi1)(__VA_ARGS__)\n\
    \n#define fe(...) for (auto __VA_ARGS__)\n#define fec(...) for (cauto &__VA_ARGS__)\n\
    #define fem(...) for (auto &__VA_ARGS__)\n#line 12 \"template/template_math.hpp\"\
    \n\n#line 2 \"utils/is_integral_ext.hpp\"\n\n#line 4 \"utils/is_integral_ext.hpp\"\
    \n\n/**\n * @brief $128$ \u30D3\u30C3\u30C8\u6574\u6570\u3092\u542B\u3081\u305F\
    \u6574\u6570\u5224\u5B9A\n * @docs docs/utils/is_integral_ext.md\n */\n\ntemplate\
    \ <class T>\nconstexpr bool is_integral_ext = is_integral_v<T> || is_same_v<T,\
    \ i128> || is_same_v<T, u128>;\n\ntemplate <class T>\nconstexpr bool is_signed_ext\
    \ = is_signed_v<T> || is_same_v<T, i128>;\n\ntemplate <class T>\nconstexpr bool\
    \ is_unsigned_ext = is_signed_v<T> || is_same_v<T, u128>;\n#line 14 \"template/template_math.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09\
    \n * @docs docs/template/template_math.md\n */\n\ntemplate <class T, class U>\n\
    inline bool chmin(T &a, U b) { return a > b ? a = b, true : false; }\ntemplate\
    \ <class T, class U>\ninline bool chmax(T &a, U b) { return a < b ? a = b, true\
    \ : false; }\n\ntemplate <class T = ll, class U, class V, typename = enable_if_t<is_integral_ext<U>\
    \ && is_integral_ext<V>>>\ninline constexpr T divfloor(U a, V b) { return T(a)\
    \ / T(b) - (T(a) % T(b) && (T(a) ^ T(b)) < 0); }\ntemplate <class T = ll, class\
    \ U, class V, typename = enable_if_t<is_integral_ext<U> && is_integral_ext<V>>>\n\
    inline constexpr T divceil(U a, V b) { return T(a) / T(b) + (T(a) % T(b) && (T(a)\
    \ ^ T(b)) >= 0); }\ntemplate <class T = ll, class U, class V, typename = enable_if_t<is_integral_ext<U>\
    \ && is_integral_ext<V>>>\ninline constexpr T divround(U a, V b) { return divfloor<T>(2\
    \ * T(a) + T(b), 2 * T(b)); }\ntemplate <class T = ll, class U, class V, typename\
    \ = enable_if_t<is_integral_ext<U> && is_integral_ext<V>>>\ninline constexpr T\
    \ safemod(U a, V b) { return T(a) - T(b) * divfloor<T>(a, b); }\n\ntemplate <class\
    \ T = ll, class U, class V>\nconstexpr T ipow(U a, V b)\n{\n  assert(b >= 0);\n\
    \  if (b == 0)\n    return 1;\n  if (a == 0 || a == 1)\n    return a;\n  if (a\
    \ < 0 && a == -1)\n    return b & 1 ? -1 : 1;\n\n  T res = 1, tmp = a;\n  while\
    \ (true)\n  {\n    if (b & 1)\n      res *= tmp;\n    b >>= 1;\n    if (b == 0)\n\
    \      break;\n    tmp *= tmp;\n  }\n  return res;\n}\ntemplate <class T = ll,\
    \ class A, class B, class M>\nT mul_limited(A a, B b, M m)\n{\n  assert(a >= 0\
    \ && b >= 0 && m >= 0);\n  if (b == 0)\n    return 0;\n  return T(a) > T(m) /\
    \ T(b) ? T(m) : T(a) * T(b);\n}\ntemplate <class T = ll, class A, class B>\nT\
    \ mul_limited(A a, B b) { return mul_limited<T>(a, b, INF); }\ntemplate <class\
    \ T = ll, class A, class B, class M>\nT pow_limited(A a, B b, M m)\n{\n  assert(a\
    \ >= 0 && b >= 0 && m >= 0);\n  if (a <= 1 || b == 0)\n    return min(ipow<T>(a,\
    \ b), T(m));\n  \n  T res = 1, tmp = a;\n  while (true)\n  {\n    if (b & 1)\n\
    \    {\n      if (res > T(m) / tmp)\n        return m;\n      res *= tmp;\n  \
    \  }\n    b >>= 1;\n    if (b == 0)\n      break;\n    if (tmp > T(m) / tmp)\n\
    \      return m;\n    tmp *= tmp;\n  }\n  return res;\n}\ntemplate <class T =\
    \ ll, class A, class B>\nT pow_limited(A a, B b) { return pow_limited<T>(a, b,\
    \ INF); }\n\ntemplate <class T = ll, class A, class K>\nconstexpr T iroot(A a,\
    \ K k)\n{\n  assert(a >= 0 && k >= 1);\n  if (a <= 1 || k == 1)\n    return a;\n\
    \  if (k == 2)\n  {\n    if constexpr (sizeof(T) > sizeof(ull))\n    {\n     \
    \ if ((u128)a < ((u128)1 << 120))\n        return sqrtl(a);\n    }\n    else\n\
    \      return sqrtl(a);\n  }\n\n  auto isok = [&](T x) -> bool\n  {\n    if (x\
    \ == 0)\n      return true;\n    T res = 1, k2 = k;\n    while (true)\n    {\n\
    \      if (k2 & 1)\n      {\n        if (res > T(a) / x)\n          return false;\n\
    \        res *= x;\n      }\n      k2 >>= 1;\n      if (k2 == 0)\n        break;\n\
    \      if (x > T(a) / x)\n        return false;\n      x *= x;\n    }\n    return\
    \ res <= T(a);\n  };\n\n  T x = pow(a, 1.0 / k);\n  bool up = true;\n  while (!isok(x))\n\
    \    up = false, x--;\n  if (up)\n  {\n    while (x < numeric_limits<T>::max()\
    \ && isok(x + 1))\n      x++;\n  }\n  return x;\n}\ntemplate <class T = ll, class\
    \ A, class K>\nconstexpr T iroot_ceil(A a, K k)\n{\n  T x = iroot<T>(a, k);\n\
    \  return ipow<T>(x, k) == a ? x : x + 1;\n}\n\n// https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html\n\
    template <class D = decltype(EPS), class A>\nint SGN(A a, D eps = EPS) { return\
    \ int(a > eps) - int(a < -eps); }\n\n// \u4F4D\u53D6\u308A\u8A18\u6570\u6CD5\u3068\
    \u540C\u3058\u9806\u756A\uFF08\u4E0B\u4F4D\u6841\u304C\u5F8C\u308D\uFF09\n// 0\
    \ \u306B\u5BFE\u3057\u3066\u306F {0} \u304C\u8FD4\u308B\ntemplate <class T = ll,\
    \ class U, class V>\nvc<T> base_repr(U val, V base)\n{\n  assert(val >= 0);\n\
    \  assert(base >= 2);\n  if (val == 0)\n    return {0};\n  vc<T> a;\n  while (val\
    \ > 0)\n  {\n    a.emplace_back(val % base);\n    val /= base;\n  }\n  reverse(a.begin(),\
    \ a.end());\n  return a;\n}\n// \u4F4D\u53D6\u308A\u8A18\u6570\u6CD5\u3068\u540C\
    \u3058\u9806\u756A\uFF08\u4E0B\u4F4D\u6841\u304C\u5F8C\u308D\uFF09\ntemplate <class\
    \ T = ll, class U, class V>\nvc<T> base_repr(U val, V base, int n)\n{\n  assert(val\
    \ >= 0);\n  assert(base >= 2);\n  assert(n >= 0);\n  vc<T> a(n);\n  repi(i, n)\n\
    \  {\n    a[i] = val % base;\n    val /= base;\n  }\n  reverse(a.begin(), a.end());\n\
    \  return a;\n}\ntemplate <const bool use_upper = true, class U>\nstring base_repr_str(U\
    \ val, int base)\n{\n  assert(val >= 0);\n  assert(2 <= base && base <= 36);\n\
    \  auto a = base_repr(val, base);\n  string s = \"\";\n  for (cauto &ai : a)\n\
    \    s += (ai < 10 ? '0' + ai : (use_upper ? 'A' : 'a') + (ai - 10));\n  return\
    \ s;\n}\ntemplate <const bool use_upper = true, class U>\nstring base_repr_str(U\
    \ val, int base, int n)\n{\n  assert(val >= 0);\n  assert(2 <= base && base <=\
    \ 36);\n  assert(n >= 0);\n  auto a = base_repr(val, base, n);\n  string s = \"\
    \";\n  for (cauto &ai : a)\n    s += (ai < 10 ? '0' + ai : (use_upper ? 'A' :\
    \ 'a') + (ai - 10));\n  return s;\n}\n"
  code: "#pragma once\n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n#endif\n\
    #ifndef EPS\n#define EPS 1e-11\n#endif\n\n#include \"template_types.hpp\"\n#include\
    \ \"template_rep.hpp\"\n\n#include \"../utils/is_integral_ext.hpp\"\n\n/**\n *\
    \ @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09\n * @docs\
    \ docs/template/template_math.md\n */\n\ntemplate <class T, class U>\ninline bool\
    \ chmin(T &a, U b) { return a > b ? a = b, true : false; }\ntemplate <class T,\
    \ class U>\ninline bool chmax(T &a, U b) { return a < b ? a = b, true : false;\
    \ }\n\ntemplate <class T = ll, class U, class V, typename = enable_if_t<is_integral_ext<U>\
    \ && is_integral_ext<V>>>\ninline constexpr T divfloor(U a, V b) { return T(a)\
    \ / T(b) - (T(a) % T(b) && (T(a) ^ T(b)) < 0); }\ntemplate <class T = ll, class\
    \ U, class V, typename = enable_if_t<is_integral_ext<U> && is_integral_ext<V>>>\n\
    inline constexpr T divceil(U a, V b) { return T(a) / T(b) + (T(a) % T(b) && (T(a)\
    \ ^ T(b)) >= 0); }\ntemplate <class T = ll, class U, class V, typename = enable_if_t<is_integral_ext<U>\
    \ && is_integral_ext<V>>>\ninline constexpr T divround(U a, V b) { return divfloor<T>(2\
    \ * T(a) + T(b), 2 * T(b)); }\ntemplate <class T = ll, class U, class V, typename\
    \ = enable_if_t<is_integral_ext<U> && is_integral_ext<V>>>\ninline constexpr T\
    \ safemod(U a, V b) { return T(a) - T(b) * divfloor<T>(a, b); }\n\ntemplate <class\
    \ T = ll, class U, class V>\nconstexpr T ipow(U a, V b)\n{\n  assert(b >= 0);\n\
    \  if (b == 0)\n    return 1;\n  if (a == 0 || a == 1)\n    return a;\n  if (a\
    \ < 0 && a == -1)\n    return b & 1 ? -1 : 1;\n\n  T res = 1, tmp = a;\n  while\
    \ (true)\n  {\n    if (b & 1)\n      res *= tmp;\n    b >>= 1;\n    if (b == 0)\n\
    \      break;\n    tmp *= tmp;\n  }\n  return res;\n}\ntemplate <class T = ll,\
    \ class A, class B, class M>\nT mul_limited(A a, B b, M m)\n{\n  assert(a >= 0\
    \ && b >= 0 && m >= 0);\n  if (b == 0)\n    return 0;\n  return T(a) > T(m) /\
    \ T(b) ? T(m) : T(a) * T(b);\n}\ntemplate <class T = ll, class A, class B>\nT\
    \ mul_limited(A a, B b) { return mul_limited<T>(a, b, INF); }\ntemplate <class\
    \ T = ll, class A, class B, class M>\nT pow_limited(A a, B b, M m)\n{\n  assert(a\
    \ >= 0 && b >= 0 && m >= 0);\n  if (a <= 1 || b == 0)\n    return min(ipow<T>(a,\
    \ b), T(m));\n  \n  T res = 1, tmp = a;\n  while (true)\n  {\n    if (b & 1)\n\
    \    {\n      if (res > T(m) / tmp)\n        return m;\n      res *= tmp;\n  \
    \  }\n    b >>= 1;\n    if (b == 0)\n      break;\n    if (tmp > T(m) / tmp)\n\
    \      return m;\n    tmp *= tmp;\n  }\n  return res;\n}\ntemplate <class T =\
    \ ll, class A, class B>\nT pow_limited(A a, B b) { return pow_limited<T>(a, b,\
    \ INF); }\n\ntemplate <class T = ll, class A, class K>\nconstexpr T iroot(A a,\
    \ K k)\n{\n  assert(a >= 0 && k >= 1);\n  if (a <= 1 || k == 1)\n    return a;\n\
    \  if (k == 2)\n  {\n    if constexpr (sizeof(T) > sizeof(ull))\n    {\n     \
    \ if ((u128)a < ((u128)1 << 120))\n        return sqrtl(a);\n    }\n    else\n\
    \      return sqrtl(a);\n  }\n\n  auto isok = [&](T x) -> bool\n  {\n    if (x\
    \ == 0)\n      return true;\n    T res = 1, k2 = k;\n    while (true)\n    {\n\
    \      if (k2 & 1)\n      {\n        if (res > T(a) / x)\n          return false;\n\
    \        res *= x;\n      }\n      k2 >>= 1;\n      if (k2 == 0)\n        break;\n\
    \      if (x > T(a) / x)\n        return false;\n      x *= x;\n    }\n    return\
    \ res <= T(a);\n  };\n\n  T x = pow(a, 1.0 / k);\n  bool up = true;\n  while (!isok(x))\n\
    \    up = false, x--;\n  if (up)\n  {\n    while (x < numeric_limits<T>::max()\
    \ && isok(x + 1))\n      x++;\n  }\n  return x;\n}\ntemplate <class T = ll, class\
    \ A, class K>\nconstexpr T iroot_ceil(A a, K k)\n{\n  T x = iroot<T>(a, k);\n\
    \  return ipow<T>(x, k) == a ? x : x + 1;\n}\n\n// https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html\n\
    template <class D = decltype(EPS), class A>\nint SGN(A a, D eps = EPS) { return\
    \ int(a > eps) - int(a < -eps); }\n\n// \u4F4D\u53D6\u308A\u8A18\u6570\u6CD5\u3068\
    \u540C\u3058\u9806\u756A\uFF08\u4E0B\u4F4D\u6841\u304C\u5F8C\u308D\uFF09\n// 0\
    \ \u306B\u5BFE\u3057\u3066\u306F {0} \u304C\u8FD4\u308B\ntemplate <class T = ll,\
    \ class U, class V>\nvc<T> base_repr(U val, V base)\n{\n  assert(val >= 0);\n\
    \  assert(base >= 2);\n  if (val == 0)\n    return {0};\n  vc<T> a;\n  while (val\
    \ > 0)\n  {\n    a.emplace_back(val % base);\n    val /= base;\n  }\n  reverse(a.begin(),\
    \ a.end());\n  return a;\n}\n// \u4F4D\u53D6\u308A\u8A18\u6570\u6CD5\u3068\u540C\
    \u3058\u9806\u756A\uFF08\u4E0B\u4F4D\u6841\u304C\u5F8C\u308D\uFF09\ntemplate <class\
    \ T = ll, class U, class V>\nvc<T> base_repr(U val, V base, int n)\n{\n  assert(val\
    \ >= 0);\n  assert(base >= 2);\n  assert(n >= 0);\n  vc<T> a(n);\n  repi(i, n)\n\
    \  {\n    a[i] = val % base;\n    val /= base;\n  }\n  reverse(a.begin(), a.end());\n\
    \  return a;\n}\ntemplate <const bool use_upper = true, class U>\nstring base_repr_str(U\
    \ val, int base)\n{\n  assert(val >= 0);\n  assert(2 <= base && base <= 36);\n\
    \  auto a = base_repr(val, base);\n  string s = \"\";\n  for (cauto &ai : a)\n\
    \    s += (ai < 10 ? '0' + ai : (use_upper ? 'A' : 'a') + (ai - 10));\n  return\
    \ s;\n}\ntemplate <const bool use_upper = true, class U>\nstring base_repr_str(U\
    \ val, int base, int n)\n{\n  assert(val >= 0);\n  assert(2 <= base && base <=\
    \ 36);\n  assert(n >= 0);\n  auto a = base_repr(val, base, n);\n  string s = \"\
    \";\n  for (cauto &ai : a)\n    s += (ai < 10 ? '0' + ai : (use_upper ? 'A' :\
    \ 'a') + (ai - 10));\n  return s;\n}\n"
  dependsOn: []
  isVerificationFile: false
  path: template/template_math.hpp
  requiredBy: []
  timestamp: '1970-01-01 00:00:00+00:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
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

$\mathrm{floor}(a^{1/k})$ を返す。$a, k$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は整数
- $0 \leq a$
- $1 \leq k$

##### 計算量

- $a \in \lbrace 0, 1 \rbrace$ または $k = 1$ のとき、$O(1)$
- それ以外で $k = 2$ かつ $a < 2^{64}$ のとき、標準ライブラリの `sqrtl(a)` の計算量
- それ以外のとき、標準ライブラリの `pow(a, 1.0/k)` の計算量を $T$ として
  - $O(T + \log k)$

#### iroot_ceil

```cpp
T iroot_ceil<T = ll>(a, k)
```

$\mathrm{ceil}(a^{1/k})$ を返す。$a, k$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は整数
- $0 \leq a$
- $1 \leq k$

##### 計算量

- `iroot` と同じ

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

##### 計算量

- $O(1)$

#### pow_limited

```cpp
T pow_limited<T = ll>(a, b, m = INF)
```

$\min(a^b, m)$ を返す。$a, b, m$ は、型 `T` にキャストしてから計算する。

##### 制約

- `T` は整数
- $a, b, m \geq 0$

##### 計算量

- $a = 1$ のとき、$O(1)$
- それ以外のとき、$O(\log \min(b, \log_a m))$

### 符号関数（誤差を考慮）

https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html

#### SGN

```cpp
int SGN<D = ld>(a, D eps = EPS)
```

誤差 $\mathrm{eps}$ を考慮した $a$ の符号を、$-1, 0, 1$ で返す。

整数の場合もこれを使ってよい。

### 位取り記数法

#### base_repr, base_repr_str

```cpp
(1) vc<T=ll> base_repr(auto val, auto base)
(2) vc<T=ll> base_repr(auto val, auto base, int n)
(3) string base_repr_str<bool use_upper = true>(auto val, auto base)
(4) string base_repr_str<bool use_upper = true>(auto val, auto base, int n)
```

$\mathrm{val}$ を $\mathrm{base}$ 進法で表現する。

- (1)：leading-zeros のない vector で表す。順番は通常の位取り記数法と同じ（下位桁が後ろ）。$0$ に対しては $(0)$ を返す。
- (2)：必要なら leading-zeros をつけて長さ $n$ の vector で表す。順番は通常の位取り記数法と同じ（下位桁が後ろ）。
- (3)：leading-zeros のない string で表す。$10$ から $35$ は `A` から `Z`（`use_upper` が false の場合は `a` から `z`）で表す。
- (4)：必要なら leading-zeros をつけて長さ $n$ の string で表す。$10$ から $35$ は `A` から `Z`（`use_upper` が false の場合は `a` から `z`）で表す。

##### 制約

- $\mathrm{val} \geq 0$
- $\mathrm{base} \geq 2$
  - (3), (4) では $2 \leq \mathrm{base} \leq 36$
- (2), (4) では $n \geq 0$

##### 計算量

- (1), (3)：$O(\log_\mathrm{base} \mathrm{val})$
- (2), (4)：$O(n + \log_\mathrm{base} \mathrm{val})$

##### 余談

逆変換（$\mathrm{base}$ 進法で整数を表す string を整数型に変換する）は、標準ライブラリの `stoll` 等を用いれば十分である。

```cpp
stoll(str, nullptr, base)  // base 進法表現された str を整数型に
```

（雑談：`stoll` に基数変換あるのに `to_string` にないのはなんでなんだろうね）