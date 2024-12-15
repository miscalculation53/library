---
data:
  _extendedDependsOn:
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
  - icon: ':warning:'
    path: template/template.cpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5168\u4F53\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_all.hpp
    title: template/template_all.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/atcoder/abc340_f.test.cpp
    title: verify/atcoder/abc340_f.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/modint.test.cpp
    title: verify/mytest/modint.test.cpp
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
    path: verify/mytest/template_inout_top.test.cpp
    title: verify/mytest/template_inout_top.test.cpp
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
    path: verify/yukicoder/yuki1550_dynamic_modint.test.cpp
    title: verify/yukicoder/yuki1550_dynamic_modint.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yukicoder/yuki1550_static_modint.test.cpp
    title: verify/yukicoder/yuki1550_static_modint.test.cpp
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
    \ namespace std;\n\n#ifndef EPS\n#define EPS 1e-11\n#endif\nusing ld = decltype(EPS);\n\
    \nusing ll = long long;\nusing uint = unsigned int;\nusing ull = unsigned long\
    \ long;\nusing pll = pair<ll, ll>;\nusing tlll = tuple<ll, ll, ll>;\nusing tllll\
    \ = tuple<ll, ll, ll, ll>;\n\n#define vc vector\ntemplate <class T>\nusing vvc\
    \ = vc<vc<T>>;\ntemplate <class T>\nusing vvvc = vc<vc<vc<T>>>;\n\nusing vb =\
    \ vc<bool>;\nusing vl = vc<ll>;\nusing vpll = vc<pll>;\nusing vtlll = vc<tlll>;\n\
    using vtllll = vc<tllll>;\nusing vstr = vc<string>;\nusing vvb = vvc<bool>;\n\
    using vvl = vvc<ll>;\n\ntemplate <class T>\nusing pql = priority_queue<T, vc<T>,\
    \ greater<T>>;\ntemplate <class T>\nusing pqg = priority_queue<T>;\n\n#ifdef __SIZEOF_INT128__\n\
    using i128 = __int128_t;\ni128 stoi128(const string &s)\n{\n  i128 res = 0;\n\
    \  if (s.front() == '-')\n  {\n    for (int i = 1; i < (int)s.size(); i++)\n \
    \     res = 10 * res + s[i] - '0';\n    res = -res;\n  }\n  else\n  {\n    for\
    \ (auto &&c : s)\n      res = 10 * res + c - '0';\n  }\n  return res;\n}\nstring\
    \ i128tos(i128 x)\n{\n  if (x == 0) return \"0\";\n  string sign = \"\", res =\
    \ \"\";\n  if (x < 0)\n    x = -x, sign = \"-\";\n  while (x > 0)\n  {\n    res\
    \ += '0' + x % 10;\n    x /= 10;\n  }\n  reverse(res.begin(), res.end());\n  return\
    \ sign + res;\n}\nistream &operator>>(istream &is, i128 &a)\n{\n  string s;\n\
    \  is >> s;\n  a = stoi128(s);\n  return is;\n}\nostream &operator<<(ostream &os,\
    \ const i128 &a)\n{\n  os << i128tos(a);\n  return os;\n}\n#endif\n\n#define cauto\
    \ const auto\n#line 4 \"template/template_dump.hpp\"\n\n/**\n * @brief \u30C6\u30F3\
    \u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09\n * @docs docs/template/template_dump.md\n\
    \ */\n\n#ifdef LOCAL\n#include <cpp-dump.hpp> // https://github.com/philip82148/cpp-dump\n\
    namespace cpp_dump::_detail\n{\n  inline string export_var(\n      const i128\
    \ &x, const string &indent, size_t last_line_length,\n      size_t current_depth,\
    \ bool fail_on_newline, const export_command &command\n  ) {\n    return export_var(i128tos(x),\
    \ indent, last_line_length, current_depth, fail_on_newline, command);\n  }\n}\
    \ // namespace cpp_dump::_detail\n#define dump(...) cpp_dump(__VA_ARGS__)\nnamespace\
    \ cp = cpp_dump;\nCPP_DUMP_SET_OPTION_GLOBAL(log_label_func, cp::log_label::line());\n\
    CPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 10000);\n#define local(...) __VA_ARGS__\n\
    #else\n#define dump(...)\n#define local(...)\n#endif\n"
  code: "#pragma once\n\n#include \"template/template_types.hpp\"\n\n/**\n * @brief\
    \ \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09\n * @docs docs/template/template_dump.md\n\
    \ */\n\n#ifdef LOCAL\n#include <cpp-dump.hpp> // https://github.com/philip82148/cpp-dump\n\
    namespace cpp_dump::_detail\n{\n  inline string export_var(\n      const i128\
    \ &x, const string &indent, size_t last_line_length,\n      size_t current_depth,\
    \ bool fail_on_newline, const export_command &command\n  ) {\n    return export_var(i128tos(x),\
    \ indent, last_line_length, current_depth, fail_on_newline, command);\n  }\n}\
    \ // namespace cpp_dump::_detail\n#define dump(...) cpp_dump(__VA_ARGS__)\nnamespace\
    \ cp = cpp_dump;\nCPP_DUMP_SET_OPTION_GLOBAL(log_label_func, cp::log_label::line());\n\
    CPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 10000);\n#define local(...) __VA_ARGS__\n\
    #else\n#define dump(...)\n#define local(...)\n#endif"
  dependsOn:
  - template/template_types.hpp
  isVerificationFile: false
  path: template/template_dump.hpp
  requiredBy:
  - math/modint/modint.hpp
  - math/extgcd.hpp
  - template/template.cpp
  - template/template_all.hpp
  timestamp: '2024-12-10 13:41:04+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo/many_aplusb_128bit.test.cpp
  - verify/yosupo/kth_root_integer.test.cpp
  - verify/yosupo/many_aplusb_tuple.test.cpp
  - verify/yosupo/aplusb.test.cpp
  - verify/yosupo/many_aplusb.test.cpp
  - verify/mytest/modint.test.cpp
  - verify/mytest/template_algo.test.cpp
  - verify/mytest/template_vector.test.cpp
  - verify/mytest/template_inout_top.test.cpp
  - verify/mytest/template_binsearch.test.cpp
  - verify/mytest/template_bit.test.cpp
  - verify/mytest/template_math_mulpow.test.cpp
  - verify/mytest/template_sgn.test.cpp.cpp
  - verify/mytest/template_math_div.test.cpp
  - verify/atcoder/abc340_f.test.cpp
  - verify/yukicoder/yuki1550_static_modint.test.cpp
  - verify/yukicoder/yuki1550_dynamic_modint.test.cpp
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