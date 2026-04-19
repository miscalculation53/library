---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    _deprecated_at_docs: docs/utils/make_unsigned_ext.md
    document_title: "$128$ \u30D3\u30C3\u30C8\u6574\u6570\u306B\u5BFE\u5FDC\u3057\u305F\
      \ make_singed \u3068 make_unsigned"
    links: []
  bundledCode: "#line 2 \"utils/make_unsigned_ext.hpp\"\n\n#line 2 \"template/template_types.hpp\"\
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
    }\n#endif\n\n#define cauto const auto\n#line 4 \"utils/make_unsigned_ext.hpp\"\
    \n\n/**\n * @brief $128$ \u30D3\u30C3\u30C8\u6574\u6570\u306B\u5BFE\u5FDC\u3057\
    \u305F make_singed \u3068 make_unsigned\n * @docs docs/utils/make_unsigned_ext.md\n\
    \ */\n\ntemplate <class T>\nstruct make_unsigned_ext\n{\n  using type = make_unsigned_t<T>;\n\
    };\ntemplate <>\nstruct make_unsigned_ext<i128>\n{\n  using type = u128;\n};\n\
    template <class T>\nusing make_unsigned_ext_t = typename make_unsigned_ext<T>::type;\n\
    \ntemplate <class T>\nstruct make_signed_ext\n{\n  using type = make_signed_t<T>;\n\
    };\ntemplate <>\nstruct make_signed_ext<u128>\n{\n  using type = i128;\n};\ntemplate\
    \ <class T>\nusing make_signed_ext_t = typename make_signed_ext<T>::type;\n"
  code: "#pragma once\n\n#include \"../template/template_types.hpp\"\n\n/**\n * @brief\
    \ $128$ \u30D3\u30C3\u30C8\u6574\u6570\u306B\u5BFE\u5FDC\u3057\u305F make_singed\
    \ \u3068 make_unsigned\n * @docs docs/utils/make_unsigned_ext.md\n */\n\ntemplate\
    \ <class T>\nstruct make_unsigned_ext\n{\n  using type = make_unsigned_t<T>;\n\
    };\ntemplate <>\nstruct make_unsigned_ext<i128>\n{\n  using type = u128;\n};\n\
    template <class T>\nusing make_unsigned_ext_t = typename make_unsigned_ext<T>::type;\n\
    \ntemplate <class T>\nstruct make_signed_ext\n{\n  using type = make_signed_t<T>;\n\
    };\ntemplate <>\nstruct make_signed_ext<u128>\n{\n  using type = i128;\n};\ntemplate\
    \ <class T>\nusing make_signed_ext_t = typename make_signed_ext<T>::type;\n"
  dependsOn: []
  isVerificationFile: false
  path: utils/make_unsigned_ext.hpp
  requiredBy: []
  timestamp: '1970-01-01 00:00:00+00:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: utils/make_unsigned_ext.hpp
layout: document
redirect_from:
- /library/utils/make_unsigned_ext.hpp
- /library/utils/make_unsigned_ext.hpp.html
title: "$128$ \u30D3\u30C3\u30C8\u6574\u6570\u306B\u5BFE\u5FDC\u3057\u305F make_singed\
  \ \u3068 make_unsigned"
---
