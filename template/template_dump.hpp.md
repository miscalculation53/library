---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
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
    }\n#endif\n\n#define cauto const auto\n#line 4 \"template/template_dump.hpp\"\n\
    \n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09\n * @docs\
    \ docs/template/template_dump.md\n */\n\n#ifdef LOCAL\n#include <cpp-dump.hpp>\
    \ // https://github.com/philip82148/cpp-dump\nnamespace cpp_dump::_detail\n{\n\
    \  inline string export_var(\n      const i128 &x, const string &indent, size_t\
    \ last_line_length,\n      size_t current_depth, bool fail_on_newline, const export_command\
    \ &command\n  ) {\n    return export_var(i128tos(x), indent, last_line_length,\
    \ current_depth, fail_on_newline, command);\n  }\n} // namespace cpp_dump::_detail\n\
    #define dump(...) cpp_dump(__VA_ARGS__)\nnamespace cp = cpp_dump;\nCPP_DUMP_SET_OPTION_GLOBAL(log_label_func,\
    \ cp::log_label::line());\nCPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 1000);\n\
    #define local(...) __VA_ARGS__\n#define oj(...)\n#define local_oj(a, b) (a)\n\
    #else\n#define dump(...)\n#define local(...)\n#define oj(...) __VA_ARGS__\n#define\
    \ local_oj(a, b) (b)\n#endif\n\ntemplate <class T, class Sequence>\nvc<T> content(queue<T,\
    \ Sequence> que)\n{\n  vc<T> res;\n  while (!que.empty())\n  {\n    res.eb(que.front());\n\
    \    que.pop();\n  }\n  return res;\n}\ntemplate <class T, class Sequence, class\
    \ Compare>\nvc<T> content(priority_queue<T, Sequence, Compare> pque)\n{\n  vc<T>\
    \ res;\n  while (!pque.empty())\n  {\n    res.eb(pque.top());\n    pque.pop();\n\
    \  }\n  return res;\n}\n"
  code: "#pragma once\n\n#include \"template_types.hpp\"\n\n/**\n * @brief \u30C6\u30F3\
    \u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09\n * @docs docs/template/template_dump.md\n\
    \ */\n\n#ifdef LOCAL\n#include <cpp-dump.hpp> // https://github.com/philip82148/cpp-dump\n\
    namespace cpp_dump::_detail\n{\n  inline string export_var(\n      const i128\
    \ &x, const string &indent, size_t last_line_length,\n      size_t current_depth,\
    \ bool fail_on_newline, const export_command &command\n  ) {\n    return export_var(i128tos(x),\
    \ indent, last_line_length, current_depth, fail_on_newline, command);\n  }\n}\
    \ // namespace cpp_dump::_detail\n#define dump(...) cpp_dump(__VA_ARGS__)\nnamespace\
    \ cp = cpp_dump;\nCPP_DUMP_SET_OPTION_GLOBAL(log_label_func, cp::log_label::line());\n\
    CPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 1000);\n#define local(...) __VA_ARGS__\n\
    #define oj(...)\n#define local_oj(a, b) (a)\n#else\n#define dump(...)\n#define\
    \ local(...)\n#define oj(...) __VA_ARGS__\n#define local_oj(a, b) (b)\n#endif\n\
    \ntemplate <class T, class Sequence>\nvc<T> content(queue<T, Sequence> que)\n\
    {\n  vc<T> res;\n  while (!que.empty())\n  {\n    res.eb(que.front());\n    que.pop();\n\
    \  }\n  return res;\n}\ntemplate <class T, class Sequence, class Compare>\nvc<T>\
    \ content(priority_queue<T, Sequence, Compare> pque)\n{\n  vc<T> res;\n  while\
    \ (!pque.empty())\n  {\n    res.eb(pque.top());\n    pque.pop();\n  }\n  return\
    \ res;\n}\n"
  dependsOn: []
  isVerificationFile: false
  path: template/template_dump.hpp
  requiredBy: []
  timestamp: '1970-01-01 00:00:00+00:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
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

---

その他マクロ

- `local()`：`LOCAL` のときのみ `()` 内が存在する
- `oj()`：`LOCAL` でないときのみ `()` 内が存在する
- `local_oj(a, b)`：`LOCAL` のときは `a`, `LOCAL` でないときは `b`

---

関数 `content()`：std の queue または priority_queue を引数に取り、中身を表す vector を返す。`dump(content(que))` のように使う。
