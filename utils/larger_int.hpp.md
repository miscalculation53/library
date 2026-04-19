---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    _deprecated_at_docs: docs/utils/larger_int.md
    document_title: "$2$ \u500D\u306E\u30D3\u30C3\u30C8\u6570\u306E\u6574\u6570\u578B"
    links: []
  bundledCode: "#line 2 \"utils/larger_int.hpp\"\n\n#line 2 \"template/template_types.hpp\"\
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
    }\n#endif\n\n#define cauto const auto\n#line 4 \"utils/larger_int.hpp\"\n\n/**\n\
    \ * @brief $2$ \u500D\u306E\u30D3\u30C3\u30C8\u6570\u306E\u6574\u6570\u578B\n\
    \ * @docs docs/utils/larger_int.md\n */\n\ntemplate <class T>\nstruct larger_int;\n\
    \n#define LARGER_INT(T, U) \\\n  template <>            \\\n  struct larger_int<T>\
    \   \\\n  {                      \\\n    using type = U;      \\\n  };\n\nLARGER_INT(signed\
    \ char, short)\nLARGER_INT(short, int)\nLARGER_INT(int, long long)\nLARGER_INT(long,\
    \ __int128_t)\nLARGER_INT(long long, __int128_t)\n\nLARGER_INT(unsigned char,\
    \ unsigned short)\nLARGER_INT(unsigned short, unsigned int)\nLARGER_INT(unsigned\
    \ int, unsigned long long)\nLARGER_INT(unsigned long, __uint128_t)\nLARGER_INT(unsigned\
    \ long long, __uint128_t)\n\n#undef LARGER_INT\n\ntemplate <class T>\nusing larger_int_t\
    \ = typename larger_int<T>::type;\n"
  code: "#pragma once\n\n#include \"../template/template_types.hpp\"\n\n/**\n * @brief\
    \ $2$ \u500D\u306E\u30D3\u30C3\u30C8\u6570\u306E\u6574\u6570\u578B\n * @docs docs/utils/larger_int.md\n\
    \ */\n\ntemplate <class T>\nstruct larger_int;\n\n#define LARGER_INT(T, U) \\\n\
    \  template <>            \\\n  struct larger_int<T>   \\\n  {               \
    \       \\\n    using type = U;      \\\n  };\n\nLARGER_INT(signed char, short)\n\
    LARGER_INT(short, int)\nLARGER_INT(int, long long)\nLARGER_INT(long, __int128_t)\n\
    LARGER_INT(long long, __int128_t)\n\nLARGER_INT(unsigned char, unsigned short)\n\
    LARGER_INT(unsigned short, unsigned int)\nLARGER_INT(unsigned int, unsigned long\
    \ long)\nLARGER_INT(unsigned long, __uint128_t)\nLARGER_INT(unsigned long long,\
    \ __uint128_t)\n\n#undef LARGER_INT\n\ntemplate <class T>\nusing larger_int_t\
    \ = typename larger_int<T>::type;\n"
  dependsOn: []
  isVerificationFile: false
  path: utils/larger_int.hpp
  requiredBy: []
  timestamp: '1970-01-01 00:00:00+00:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: utils/larger_int.hpp
layout: document
redirect_from:
- /library/utils/larger_int.hpp
- /library/utils/larger_int.hpp.html
title: "$2$ \u500D\u306E\u30D3\u30C3\u30C8\u6570\u306E\u6574\u6570\u578B"
---
