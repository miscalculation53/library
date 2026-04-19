---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    _deprecated_at_docs: docs/template/template_random.md
    document_title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30E9\u30F3\u30C0\u30E0\
      \u751F\u6210\uFF09"
    links: []
  bundledCode: "#line 2 \"template/template_random.hpp\"\n\n#line 2 \"template/template_types.hpp\"\
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
    \u30FC\u30C8\uFF08rep\uFF09\n * @docs docs/template/template_rep.md\n */\n\n//\
    \ https://trap.jp/post/1224/\n\n#define overload4(_1, _2, _3, _4, name, ...) name\n\
    #define rep1(i, n) for (ll i = 0, nnnnn = ll(n); i < nnnnn; i++)\n#define rep2(i,\
    \ l, r) for (ll i = ll(l), rrrrr = ll(r); i < rrrrr; i++)\n#define rep3(i, l,\
    \ r, d) for (ll i = ll(l), rrrrr = ll(r), ddddd = ll(d); ddddd > 0 ? i < rrrrr\
    \ : i > rrrrr; i += d)\n#define rep(...) overload4(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)\n\
    #define repi1(i, n) for (int i = 0, nnnnn = int(n); i < nnnnn; i++)\n#define repi2(i,\
    \ l, r) for (int i = int(l), rrrrr = int(r); i < rrrrr; i++)\n#define repi3(i,\
    \ l, r, d) for (int i = int(l), rrrrr = int(r), ddddd = int(d); ddddd > 0 ? i\
    \ < rrrrr : i > rrrrr; i += d)\n#define repi(...) overload4(__VA_ARGS__, repi3,\
    \ repi2, repi1)(__VA_ARGS__)\n\n#define fe(...) for (auto __VA_ARGS__)\n#define\
    \ fec(...) for (cauto &__VA_ARGS__)\n#define fem(...) for (auto &__VA_ARGS__)\n\
    #line 5 \"template/template_random.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\
    \u30FC\u30C8\uFF08\u30E9\u30F3\u30C0\u30E0\u751F\u6210\uFF09\n * @docs docs/template/template_random.md\n\
    \ */\n\nmt19937_64 mt;\n\n// [l, r] \u304B\u3089\u7B49\u78BA\u7387\ntemplate <class\
    \ T = ll, class U1, class U2>\nT randint(U1 l, U2 r)\n{\n  assert(T(l) <= T(r));\n\
    \  return T(l) + mt() % (T(r) - T(l) + 1);\n}\n// [l, r) \u304B\u3089\u7B49\u78BA\
    \u7387\ntemplate <class T = ll, class U1, class U2>\nT randrange(U1 l, U2 r)\n\
    {\n  assert(T(l) < T(r));\n  return T(l) + mt() % (T(r) - T(l));\n}\n\n// [l,\
    \ r) \u304B\u3089\u76F8\u7570\u306A\u308B k \u500B\u3092\u9078\u3076\n// does_sort:\
    \ \u30BD\u30FC\u30C8\u3059\u308B\u304B\u3069\u3046\u304B\ntemplate <int k, bool\
    \ does_sort, class T = ll, class U1, class U2>\narray<T, k> random_sample_range_array(U1\
    \ l, U2 r)\n{\n  assert(T(r) - T(l) >= T(k));\n  array<T, k> res;\n  repi(i, k)\
    \ res[i] = randint<T>(T(l), T(r) - T(k));\n  sort(ALL(res));\n  repi(i, k) res[i]\
    \ += i;\n  if (!does_sort)\n    shuffle(ALL(res), mt);\n  return res;\n}\n// [l,\
    \ r) \u304B\u3089\u76F8\u7570\u306A\u308B k \u500B\u3092\u9078\u3076\n// does_sort:\
    \ \u30BD\u30FC\u30C8\u3059\u308B\u304B\u3069\u3046\u304B\ntemplate <bool does_sort,\
    \ class T = ll, class U1, class U2>\nvc<T> random_sample_range_vector(U1 l, U2\
    \ r, int k)\n{\n  assert(T(r) - T(l) >= T(k));\n  vc<T> res(k);\n  repi(i, k)\
    \ res[i] = randint<T>(T(l), T(r) - T(k));\n  sort(ALL(res));\n  repi(i, k) res[i]\
    \ += i;\n  if (!does_sort)\n    shuffle(ALL(res), mt);\n  return res;\n}\n"
  code: "#pragma once\n\n#include \"template_types.hpp\"\n#include \"template_rep.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30E9\u30F3\u30C0\
    \u30E0\u751F\u6210\uFF09\n * @docs docs/template/template_random.md\n */\n\nmt19937_64\
    \ mt;\n\n// [l, r] \u304B\u3089\u7B49\u78BA\u7387\ntemplate <class T = ll, class\
    \ U1, class U2>\nT randint(U1 l, U2 r)\n{\n  assert(T(l) <= T(r));\n  return T(l)\
    \ + mt() % (T(r) - T(l) + 1);\n}\n// [l, r) \u304B\u3089\u7B49\u78BA\u7387\ntemplate\
    \ <class T = ll, class U1, class U2>\nT randrange(U1 l, U2 r)\n{\n  assert(T(l)\
    \ < T(r));\n  return T(l) + mt() % (T(r) - T(l));\n}\n\n// [l, r) \u304B\u3089\
    \u76F8\u7570\u306A\u308B k \u500B\u3092\u9078\u3076\n// does_sort: \u30BD\u30FC\
    \u30C8\u3059\u308B\u304B\u3069\u3046\u304B\ntemplate <int k, bool does_sort, class\
    \ T = ll, class U1, class U2>\narray<T, k> random_sample_range_array(U1 l, U2\
    \ r)\n{\n  assert(T(r) - T(l) >= T(k));\n  array<T, k> res;\n  repi(i, k) res[i]\
    \ = randint<T>(T(l), T(r) - T(k));\n  sort(ALL(res));\n  repi(i, k) res[i] +=\
    \ i;\n  if (!does_sort)\n    shuffle(ALL(res), mt);\n  return res;\n}\n// [l,\
    \ r) \u304B\u3089\u76F8\u7570\u306A\u308B k \u500B\u3092\u9078\u3076\n// does_sort:\
    \ \u30BD\u30FC\u30C8\u3059\u308B\u304B\u3069\u3046\u304B\ntemplate <bool does_sort,\
    \ class T = ll, class U1, class U2>\nvc<T> random_sample_range_vector(U1 l, U2\
    \ r, int k)\n{\n  assert(T(r) - T(l) >= T(k));\n  vc<T> res(k);\n  repi(i, k)\
    \ res[i] = randint<T>(T(l), T(r) - T(k));\n  sort(ALL(res));\n  repi(i, k) res[i]\
    \ += i;\n  if (!does_sort)\n    shuffle(ALL(res), mt);\n  return res;\n}\n"
  dependsOn: []
  isVerificationFile: false
  path: template/template_random.hpp
  requiredBy: []
  timestamp: '1970-01-01 00:00:00+00:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: template/template_random.hpp
layout: document
redirect_from:
- /library/template/template_random.hpp
- /library/template/template_random.hpp.html
title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30E9\u30F3\u30C0\u30E0\u751F\u6210\
  \uFF09"
---
## テンプレート（ランダム生成）

主にランダムテストで使うことを目的としているが、解法が乱択の場合にも便利かも。

### グローバル変数

- `mt19937_64 mt`
  - `LOCAL` では seed は固定となり、オンラインジャッジでは毎回異なる seed となる（ようにテンプレの `init` 関数に書いている）

### 関数

これらはすべて `mt` を使っている。

#### randint, randrange

```cpp
(1) ll randint(ll l, ll r)
(2) ll randrange(ll l, ll r)
```

- (1)：$[l, r]$ から一様ランダムに選んだ整数を $1$ つ返す。
- (2)：$[l, r)$ から一様ランダムに選んだ整数を $1$ つ返す。

##### 制約

- (1)：$l \leq r$
- (2)：$l \lt r$

##### 計算量

- $O(1)$


#### random_sample_range

```cpp
(1) array<ll, k> random_sample_range_array<int k, bool does_sort>(ll l, ll r)
(2) vc<ll> random_sample_range_vector<bool does_sort>(ll l, ll r, int k)
```

$[l, r)$ の範囲から相異なる $k$ 個の整数を一様ランダムに選び、(1) は array で、(2) は vector で返す。`does_sort` が true ならソートされて返り、false ならシャッフルされて返る。

##### 制約

- $r - l \geq k$

##### 計算量

- $O(k\log k)$

##### 使用例

$1 \leq a \lt b \leq n$ なる $a, b$ を生成したいとき

```cpp
auto [a, b] = random_sample_range_array<2, true>(1, n + 1)
```