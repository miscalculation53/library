---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':x:'
    path: template/template_all.hpp
    title: template/template_all.hpp
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
    _deprecated_at_docs: docs/template/template_dump.md
    document_title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09"
    links:
    - https://github.com/philip82148/cpp-dump
  bundledCode: "#line 2 \"template/template_dump.hpp\"\n\n/**\n * @brief \u30C6\u30F3\
    \u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09\n * @docs docs/template/template_dump.md\n\
    \ */\n\n#ifdef LOCAL\n  #include <cpp-dump.hpp> // https://github.com/philip82148/cpp-dump\n\
    \  #define dump(...) cpp_dump(__VA_ARGS__)\n  namespace cp = cpp_dump;\n  CPP_DUMP_SET_OPTION_GLOBAL(log_label_func,\
    \ cp::log_label::line());\n  CPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 10000);\n\
    #else\n  #define dump(...)\n#endif\n"
  code: "#pragma once\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\
    dump\uFF09\n * @docs docs/template/template_dump.md\n */\n\n#ifdef LOCAL\n  #include\
    \ <cpp-dump.hpp> // https://github.com/philip82148/cpp-dump\n  #define dump(...)\
    \ cpp_dump(__VA_ARGS__)\n  namespace cp = cpp_dump;\n  CPP_DUMP_SET_OPTION_GLOBAL(log_label_func,\
    \ cp::log_label::line());\n  CPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 10000);\n\
    #else\n  #define dump(...)\n#endif"
  dependsOn: []
  isVerificationFile: false
  path: template/template_dump.hpp
  requiredBy:
  - template/template_all.hpp
  timestamp: '2024-08-27 21:11:34+09:00'
  verificationStatus: LIBRARY_ALL_WA
  verifiedWith:
  - verify/mytest/template_inout_top.test.cpp
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