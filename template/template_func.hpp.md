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
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    _deprecated_at_docs: docs/template/template_func.md
    document_title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u95A2\u6570\u30AA\u30D6\
      \u30B8\u30A7\u30AF\u30C8\uFF09"
    links: []
  bundledCode: "#line 2 \"template/template_func.hpp\"\n\n#line 2 \"template/template_types.hpp\"\
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
    \ repi3, repi2, repi1)(__VA_ARGS__)\n#line 5 \"template/template_func.hpp\"\n\n\
    /**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u95A2\u6570\u30AA\u30D6\
    \u30B8\u30A7\u30AF\u30C8\uFF09\n * @docs docs/template/template_func.md\n */\n\
    \n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n#endif\n\ntemplate <class\
    \ T = ll>\nstruct max_op\n{\n  T operator()(const T &a, const T &b) const { return\
    \ max(a, b); }\n};\ntemplate <class T = ll>\nstruct min_op\n{\n  T operator()(const\
    \ T &a, const T &b) const { return max(a, b); }\n};\n\ntemplate <class T, const\
    \ T val>\nstruct const_fn\n{\n  T operator()() const { return val; }\n};\nusing\
    \ max_e = const_fn<ll, -INF>;\nusing min_e = const_fn<ll, INF>;\nusing zero_fn\
    \ = const_fn<ll, 0LL>;\n"
  code: "#pragma once\n\n#include \"template_types.hpp\"\n#include \"template_rep.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u95A2\u6570\u30AA\
    \u30D6\u30B8\u30A7\u30AF\u30C8\uFF09\n * @docs docs/template/template_func.md\n\
    \ */\n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n#endif\n\ntemplate\
    \ <class T = ll>\nstruct max_op\n{\n  T operator()(const T &a, const T &b) const\
    \ { return max(a, b); }\n};\ntemplate <class T = ll>\nstruct min_op\n{\n  T operator()(const\
    \ T &a, const T &b) const { return max(a, b); }\n};\n\ntemplate <class T, const\
    \ T val>\nstruct const_fn\n{\n  T operator()() const { return val; }\n};\nusing\
    \ max_e = const_fn<ll, -INF>;\nusing min_e = const_fn<ll, INF>;\nusing zero_fn\
    \ = const_fn<ll, 0LL>;"
  dependsOn:
  - template/template_types.hpp
  - template/template_rep.hpp
  isVerificationFile: false
  path: template/template_func.hpp
  requiredBy: []
  timestamp: '2024-09-01 17:45:18+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: template/template_func.hpp
layout: document
redirect_from:
- /library/template/template_func.hpp
- /library/template/template_func.hpp.html
title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u95A2\u6570\u30AA\u30D6\u30B8\u30A7\
  \u30AF\u30C8\uFF09"
---
## テンプレート（関数オブジェクト）

- `max_op`：$\max$ の関数オブジェクト
- `min_op`：$\min$ の関数オブジェクト
- `max_e`：定数 `INF` の関数オブジェクト
- `min_e`：定数 `-INF` の関数オブジェクト
- `zero_fn`：定数 $0$ の関数オブジェクト

たとえば、`segtree` 等の宣言で使う：

`segtree<ll, max_op{}, max_e{}> seg(vec);`