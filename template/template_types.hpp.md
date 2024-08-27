---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: template/template_all.hpp
    title: template/template_all.hpp
  - icon: ':heavy_check_mark:'
    path: template/template_inout.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_rep.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09"
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_inout_top.test.cpp
    title: verify/mytest/template_inout_top.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/many_aplusb.test.cpp
    title: verify/yosupo/many_aplusb.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/many_aplusb_tuple.test.cpp
    title: verify/yosupo/many_aplusb_tuple.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    _deprecated_at_docs: docs/template/template_types.md
    document_title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09"
    links: []
  bundledCode: "#line 2 \"template/template_types.hpp\"\n\n/**\n * @brief \u30C6\u30F3\
    \u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09\n * @docs docs/template/template_types.md\n\
    \ */\n\n#include <bits/stdc++.h>\nusing namespace std;\n\nusing ll = long long;\n\
    using dbl = double;\nusing ld = long double;\nusing uint = unsigned int;\nusing\
    \ ull = unsigned long long;\nusing pll = pair<ll, ll>;\nusing tlll = tuple<ll,\
    \ ll, ll>;\nusing tllll = tuple<ll, ll, ll, ll>;\n\ntemplate <class T>\nusing\
    \ vc = vector<T>;\ntemplate <class T>\nusing vvc = vector<vector<T>>;\ntemplate\
    \ <class T>\nusing vvvc = vector<vector<vector<T>>>;\n\nusing vb = vc<bool>;\n\
    using vl = vc<ll>;\nusing vpll = vc<pll>;\nusing vtlll = vc<tlll>;\nusing vtllll\
    \ = vc<tllll>;\nusing vstr = vc<string>;\nusing vvb = vvc<bool>;\nusing vvl =\
    \ vvc<ll>;\n\n#ifdef __SIZEOF_INT128__\n  using i128 = __int128_t;\n  i128 stoi128(const\
    \ string &s)\n  {\n    i128 res = 0;\n    if (s.front() == '-')\n    {\n     \
    \ for (int i = 1; i < (int)s.size(); i++)\n        res = 10 * res + s[i] - '0';\n\
    \      res = -res;\n    }\n    else\n    {\n      for (auto &&c : s)\n       \
    \ res = 10 * res + c - '0';\n    }\n    return res;\n  }\n  string i128tos(i128\
    \ x)\n  {\n    if (x == 0) return \"0\";\n    string sign = \"\", res = \"\";\n\
    \    if (x < 0)\n      x = -x, sign = \"-\";\n    while (x > 0)\n    {\n     \
    \ res += '0' + x % 10;\n      x /= 10;\n    }\n    reverse(res.begin(), res.end());\n\
    \    return sign + res;\n  }\n  istream &operator>>(istream &is, i128 &a)\n  {\n\
    \    string s;\n    is >> s;\n    a = stoi128(s);\n    return is;\n  }\n  ostream\
    \ &operator<<(ostream &os, const i128 &a)\n  {\n    os << i128tos(a);\n    return\
    \ os;\n  }\n#endif\n"
  code: "#pragma once\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\
    \u578B\uFF09\n * @docs docs/template/template_types.md\n */\n\n#include <bits/stdc++.h>\n\
    using namespace std;\n\nusing ll = long long;\nusing dbl = double;\nusing ld =\
    \ long double;\nusing uint = unsigned int;\nusing ull = unsigned long long;\n\
    using pll = pair<ll, ll>;\nusing tlll = tuple<ll, ll, ll>;\nusing tllll = tuple<ll,\
    \ ll, ll, ll>;\n\ntemplate <class T>\nusing vc = vector<T>;\ntemplate <class T>\n\
    using vvc = vector<vector<T>>;\ntemplate <class T>\nusing vvvc = vector<vector<vector<T>>>;\n\
    \nusing vb = vc<bool>;\nusing vl = vc<ll>;\nusing vpll = vc<pll>;\nusing vtlll\
    \ = vc<tlll>;\nusing vtllll = vc<tllll>;\nusing vstr = vc<string>;\nusing vvb\
    \ = vvc<bool>;\nusing vvl = vvc<ll>;\n\n#ifdef __SIZEOF_INT128__\n  using i128\
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
    \ &a)\n  {\n    os << i128tos(a);\n    return os;\n  }\n#endif"
  dependsOn: []
  isVerificationFile: false
  path: template/template_types.hpp
  requiredBy:
  - template/template_inout.hpp
  - template/template_all.hpp
  - template/template_rep.hpp
  timestamp: '2024-08-27 21:11:34+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/mytest/template_inout_top.test.cpp
  - verify/yosupo/many_aplusb.test.cpp
  - verify/yosupo/many_aplusb_tuple.test.cpp
documentation_of: template/template_types.hpp
layout: document
redirect_from:
- /library/template/template_types.hpp
- /library/template/template_types.hpp.html
title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09"
---
## テンプレート（型）

定義されている型はソースコードの通り。