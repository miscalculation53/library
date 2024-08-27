---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: template/template_types.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09"
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: template/template_all.hpp
    title: template/template_all.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_inout_top.test.cpp
    title: verify/mytest/template_inout_top.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/aplusb.test.cpp
    title: verify/yosupo/aplusb.test.cpp
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
    _deprecated_at_docs: docs/template/template_dump.md
    document_title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09"
    links:
    - https://github.com/philip82148/cpp-dump
  bundledCode: "#line 2 \"template/template_dump.hpp\"\n\n#line 2 \"template/template_types.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09\n *\
    \ @docs docs/template/template_types.md\n */\n\n#include <bits/stdc++.h>\nusing\
    \ namespace std;\n\nusing ll = long long;\nusing dbl = double;\nusing ld = long\
    \ double;\nusing uint = unsigned int;\nusing ull = unsigned long long;\nusing\
    \ pll = pair<ll, ll>;\nusing tlll = tuple<ll, ll, ll>;\nusing tllll = tuple<ll,\
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
    \ &a)\n  {\n    os << i128tos(a);\n    return os;\n  }\n#endif\n#line 4 \"template/template_dump.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09\n * @docs\
    \ docs/template/template_dump.md\n */\n\n#ifdef LOCAL\n  #include <cpp-dump.hpp>\
    \ // https://github.com/philip82148/cpp-dump\n  namespace cpp_dump::_detail\n\
    \  {\n    inline string export_var(\n        const i128 &x, const string &indent,\
    \ size_t last_line_length,\n        size_t current_depth, bool fail_on_newline,\
    \ const export_command &command\n    ) {\n      return export_var(i128tos(x),\
    \ indent, last_line_length, current_depth, fail_on_newline, command);\n    }\n\
    \  } // namespace cpp_dump::_detail\n  #define dump(...) cpp_dump(__VA_ARGS__)\n\
    \  namespace cp = cpp_dump;\n  CPP_DUMP_SET_OPTION_GLOBAL(log_label_func, cp::log_label::line());\n\
    \  CPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 10000);\n#else\n  #define dump(...)\n\
    #endif\n"
  code: "#pragma once\n\n#include \"template/template_types.hpp\"\n\n/**\n * @brief\
    \ \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09\n * @docs docs/template/template_dump.md\n\
    \ */\n\n#ifdef LOCAL\n  #include <cpp-dump.hpp> // https://github.com/philip82148/cpp-dump\n\
    \  namespace cpp_dump::_detail\n  {\n    inline string export_var(\n        const\
    \ i128 &x, const string &indent, size_t last_line_length,\n        size_t current_depth,\
    \ bool fail_on_newline, const export_command &command\n    ) {\n      return export_var(i128tos(x),\
    \ indent, last_line_length, current_depth, fail_on_newline, command);\n    }\n\
    \  } // namespace cpp_dump::_detail\n  #define dump(...) cpp_dump(__VA_ARGS__)\n\
    \  namespace cp = cpp_dump;\n  CPP_DUMP_SET_OPTION_GLOBAL(log_label_func, cp::log_label::line());\n\
    \  CPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 10000);\n#else\n  #define dump(...)\n\
    #endif"
  dependsOn:
  - template/template_types.hpp
  isVerificationFile: false
  path: template/template_dump.hpp
  requiredBy:
  - template/template_all.hpp
  timestamp: '2024-08-28 00:55:59+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/mytest/template_inout_top.test.cpp
  - verify/yosupo/aplusb.test.cpp
  - verify/yosupo/many_aplusb_128bit.test.cpp
  - verify/yosupo/many_aplusb.test.cpp
  - verify/yosupo/many_aplusb_tuple.test.cpp
documentation_of: template/template_dump.hpp
layout: document
redirect_from:
- /library/template/template_dump.hpp
- /library/template/template_dump.hpp.html
title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09"
---
## テンプレート（dump）

- https://github.com/philip82148/cpp-dump
  - 解説記事 1： https://zenn.dev/sassan/articles/19db660e4da0a4
  - 解説記事 2： https://zenn.dev/sassan/articles/4878e79272ed61

上記のプログラムを手元で利用する。`LOCAL` が define されているときのみ dump 出力をする（特に、オンラインジャッジでは無効化される）。