---
data:
  _extendedDependsOn:
  - icon: ':x:'
    path: template/template_types.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09"
  _extendedRequiredBy:
  - icon: ':x:'
    path: template/template_all.hpp
    title: template/template_all.hpp
  - icon: ':x:'
    path: template/template_inout.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\uFF09"
  _extendedVerifiedWith:
  - icon: ':x:'
    path: verify/mytest/template_inout_top.test.cpp
    title: verify/mytest/template_inout_top.test.cpp
  - icon: ':x:'
    path: verify/yosupo/many_aplusb.test.cpp
    title: verify/yosupo/many_aplusb.test.cpp
  - icon: ':x:'
    path: verify/yosupo/many_aplusb_tuple.test.cpp
    title: verify/yosupo/many_aplusb_tuple.test.cpp
  _isVerificationFailed: true
  _pathExtension: hpp
  _verificationStatusIcon: ':x:'
  attributes:
    _deprecated_at_docs: docs/template/template_rep.md
    document_title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09"
    links:
    - https://trap.jp/post/1224/
  bundledCode: "#line 2 \"template/template_rep.hpp\"\n\n/**\n * @brief \u30C6\u30F3\
    \u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09\n * @docs docs/template/template_rep.md\n\
    \ */\n\n/**\n * \u53C2\u8003\uFF1A\n * https://trap.jp/post/1224/\n*/\n\n#line\
    \ 2 \"template/template_types.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\
    \u30FC\u30C8\uFF08\u578B\uFF09\n * @docs docs/template/template_types.md\n */\n\
    \n#include <bits/stdc++.h>\nusing namespace std;\n\nusing ll = long long;\nusing\
    \ dbl = double;\nusing ld = long double;\nusing uint = unsigned int;\nusing ull\
    \ = unsigned long long;\nusing pll = pair<ll, ll>;\nusing tlll = tuple<ll, ll,\
    \ ll>;\nusing tllll = tuple<ll, ll, ll, ll>;\n\ntemplate <class T>\nusing vc =\
    \ vector<T>;\ntemplate <class T>\nusing vvc = vector<vector<T>>;\ntemplate <class\
    \ T>\nusing vvvc = vector<vector<vector<T>>>;\n\nusing vb = vc<bool>;\nusing vl\
    \ = vc<ll>;\nusing vpll = vc<pll>;\nusing vtlll = vc<tlll>;\nusing vtllll = vc<tllll>;\n\
    using vstr = vc<string>;\nusing vvb = vvc<bool>;\nusing vvl = vvc<ll>;\n\n#ifdef\
    \ __SIZEOF_INT128__\n  using i128 = __int128_t;\n  i128 stoi128(const string &s)\n\
    \  {\n    i128 res = 0;\n    if (s.front() == '-')\n    {\n      for (int i =\
    \ 1; i < (int)s.size(); i++)\n        res = 10 * res + s[i] - '0';\n      res\
    \ = -res;\n    }\n    else\n    {\n      for (auto &&c : s)\n        res = 10\
    \ * res + c - '0';\n    }\n    return res;\n  }\n  string i128tos(i128 x)\n  {\n\
    \    if (x == 0) return \"0\";\n    string sign = \"\", res = \"\";\n    if (x\
    \ < 0)\n      x = -x, sign = \"-\";\n    while (x > 0)\n    {\n      res += '0'\
    \ + x % 10;\n      x /= 10;\n    }\n    reverse(res.begin(), res.end());\n   \
    \ return sign + res;\n  }\n  istream &operator>>(istream &is, i128 &a)\n  {\n\
    \    string s;\n    is >> s;\n    a = stoi128(s);\n    return is;\n  }\n  ostream\
    \ &operator<<(ostream &os, const i128 &a)\n  {\n    os << i128tos(a);\n    return\
    \ os;\n  }\n#endif\n#line 14 \"template/template_rep.hpp\"\n\n#define overload4(_1,\
    \ _2, _3, _4, name, ...) name\n#define rep1(i, n) for (ll i = 0; i < ll(n); i++)\n\
    #define rep2(i, l, r) for (ll i = ll(l); i < ll(r); i++)\n#define rep3(i, l, r,\
    \ d) for (ll i = ll(l); (d) > 0 ? i < ll(r) : i > ll(r); i += d)\n#define rep(...)\
    \ overload4(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)\n#define repi1(i, n) for\
    \ (int i = 0; i < int(n); i++)\n#define repi2(i, l, r) for (int i = int(l); i\
    \ < int(r); i++)\n#define repi3(i, l, r, d) for (int i = int(l); (d) > 0 ? i <\
    \ int(r) : i > int(r); i += d)\n#define repi(...) overload4(__VA_ARGS__, repi3,\
    \ repi2, repi1)(__VA_ARGS__)\n"
  code: "#pragma once\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\
    rep\uFF09\n * @docs docs/template/template_rep.md\n */\n\n/**\n * \u53C2\u8003\
    \uFF1A\n * https://trap.jp/post/1224/\n*/\n\n#include \"template_types.hpp\"\n\
    \n#define overload4(_1, _2, _3, _4, name, ...) name\n#define rep1(i, n) for (ll\
    \ i = 0; i < ll(n); i++)\n#define rep2(i, l, r) for (ll i = ll(l); i < ll(r);\
    \ i++)\n#define rep3(i, l, r, d) for (ll i = ll(l); (d) > 0 ? i < ll(r) : i >\
    \ ll(r); i += d)\n#define rep(...) overload4(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)\n\
    #define repi1(i, n) for (int i = 0; i < int(n); i++)\n#define repi2(i, l, r) for\
    \ (int i = int(l); i < int(r); i++)\n#define repi3(i, l, r, d) for (int i = int(l);\
    \ (d) > 0 ? i < int(r) : i > int(r); i += d)\n#define repi(...) overload4(__VA_ARGS__,\
    \ repi3, repi2, repi1)(__VA_ARGS__)"
  dependsOn:
  - template/template_types.hpp
  isVerificationFile: false
  path: template/template_rep.hpp
  requiredBy:
  - template/template_inout.hpp
  - template/template_all.hpp
  timestamp: '2024-08-27 21:11:34+09:00'
  verificationStatus: LIBRARY_ALL_WA
  verifiedWith:
  - verify/mytest/template_inout_top.test.cpp
  - verify/yosupo/many_aplusb.test.cpp
  - verify/yosupo/many_aplusb_tuple.test.cpp
documentation_of: template/template_rep.hpp
layout: document
redirect_from:
- /library/template/template_rep.hpp
- /library/template/template_rep.hpp.html
title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09"
---
## テンプレート（rep）

指定する引数は Python の range と同じ。つまり、

- `rep(l, r, d)`: $l$ から始めて $d$ ずつ変えるのを $r$ まで（$r$ は含まない）
- `rep(l, r)`: `rep(l, r, 1)` と同じ
- `rep(r)`: `rep(0, r, 1)` と同じ

引数の数が違う `rep1`, `rep2`, `rep3` をすべて `rep` で呼べるようにしてある（参考： https://trap.jp/post/1224/ ）。

また、`rep` では `ll` 型を、`repi` では `int` 型を用いる。