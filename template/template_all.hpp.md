---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: template/template_types.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09"
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/yosupo_many_aplusb.test.cpp
    title: verify/yosupo_many_aplusb.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"template/template_all.hpp\"\n\n#line 2 \"template/template_types.hpp\"\
    \n\n#include <bits/stdc++.h>\nusing namespace std;\n\nusing ll = long long;\n\
    using dbl = double;\nusing ld = long double;\nusing uint = unsigned int;\nusing\
    \ ull = unsigned long long;\nusing pll = pair<ll, ll>;\nusing tlll = tuple<ll,\
    \ ll, ll>;\nusing tllll = tuple<ll, ll, ll, ll>;\n\n#define vc vector\nusing vl\
    \ = vc<ll>;\nusing vpll = vc<pll>;\nusing vtlll = vc<tlll>;\nusing vtllll = vc<tllll>;\n\
    using vvl = vc<vc<ll>>;\n\n#ifdef __SIZEOF_INT128__\n  using i128 = __int128_t;\n\
    \  i128 stoi128(const string &s)\n  {\n    i128 res = 0;\n    if (s.front() ==\
    \ '-')\n    {\n      for (int i = 1; i < (int)s.size(); i++)\n        res = 10\
    \ * res + s[i] - '0';\n      res = -res;\n    }\n    else\n    {\n      for (auto\
    \ &&c : s)\n        res = 10 * res + c - '0';\n    }\n    return res;\n  }\n \
    \ string i128tos(i128 x)\n  {\n    if (x == 0) return \"0\";\n    string sign\
    \ = \"\", res = \"\";\n    if (x < 0)\n      x = -x, sign = \"-\";\n    while\
    \ (x > 0)\n    {\n      res += '0' + x % 10;\n      x /= 10;\n    }\n    reverse(res.begin(),\
    \ res.end());\n    return sign + res;\n  }\n  istream &operator>>(istream &is,\
    \ i128 &a)\n  {\n    string s;\n    is >> s;\n    a = stoi128(s);\n    return\
    \ is;\n  }\n  ostream &operator<<(ostream &os, const i128 &a)\n  {\n    os <<\
    \ i128tos(a);\n    return os;\n  }\n#endif\n\n/**\n * @brief \u30C6\u30F3\u30D7\
    \u30EC\u30FC\u30C8\uFF08\u578B\uFF09\n * @docs docs/template/template_types.md\n\
    \ */\n#line 4 \"template/template_all.hpp\"\n"
  code: '#pragma once


    #include "template_types.hpp"'
  dependsOn:
  - template/template_types.hpp
  isVerificationFile: false
  path: template/template_all.hpp
  requiredBy: []
  timestamp: '2024-08-27 03:11:16+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo_many_aplusb.test.cpp
documentation_of: template/template_all.hpp
layout: document
redirect_from:
- /library/template/template_all.hpp
- /library/template/template_all.hpp.html
title: template/template_all.hpp
---
