---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: true
  _pathExtension: cpp
  _verificationStatusIcon: ':x:'
  attributes: {}
  bundledCode: "Traceback (most recent call last):\n  File \"/opt/hostedtoolcache/Python/3.12.0/x64/lib/python3.12/site-packages/onlinejudge_verify/documentation/build.py\"\
    , line 71, in _render_source_code_stat\n    bundled_code = language.bundle(stat.path,\
    \ basedir=basedir, options={'include_paths': [basedir]}).decode()\n          \
    \         ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\
    \  File \"/opt/hostedtoolcache/Python/3.12.0/x64/lib/python3.12/site-packages/onlinejudge_verify/languages/cplusplus.py\"\
    , line 187, in bundle\n    bundler.update(path)\n  File \"/opt/hostedtoolcache/Python/3.12.0/x64/lib/python3.12/site-packages/onlinejudge_verify/languages/cplusplus_bundle.py\"\
    , line 401, in update\n    self.update(self._resolve(pathlib.Path(included), included_from=path))\n\
    \  File \"/opt/hostedtoolcache/Python/3.12.0/x64/lib/python3.12/site-packages/onlinejudge_verify/languages/cplusplus_bundle.py\"\
    , line 401, in update\n    self.update(self._resolve(pathlib.Path(included), included_from=path))\n\
    \                ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n \
    \ File \"/opt/hostedtoolcache/Python/3.12.0/x64/lib/python3.12/site-packages/onlinejudge_verify/languages/cplusplus_bundle.py\"\
    , line 260, in _resolve\n    raise BundleErrorAt(path, -1, \"no such header\"\
    )\nonlinejudge_verify.languages.cplusplus_bundle.BundleErrorAt: ../template/template_all_but_modint.hpp:\
    \ line -1: no such header\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/point_add_range_sum\"\n\
    \n#define SINGLE_TESTCASE\n// #define MULTI_TESTCASE\n// #define AOJ_TESTCASE\n\
    \n#ifndef LOCAL\n#define FAST_IO\n// #define FAST_CIO\n// #define INTERACTIVE\n\
    #endif\n\n#define INF 4'000'000'000'000'000'037LL\n#define EPS 1e-11\n\n#include\
    \ \"template/template_all_but_modint.hpp\"\n#include \"ds/fenwick_tree/fenwick_tree.hpp\"\
    \n\nvoid init() {}\n\nvoid main2()\n{\n  LL(N, Q);\n  VEC(ll, N, A);\n\n  FenwickTree<GroupAddSub<ll>>\
    \ fw(A);\n  rep(_, Q)\n  {\n    LL(t);\n    if (t == 0)\n    {\n      LL(p, x);\n\
    \      fw.add(p, x);\n    }\n    else if (t == 1)\n    {\n      LL(l, r);\n  \
    \    PRINT(fw.sum(l, r));\n    }\n  }\n}\n\nvoid test()\n{\n  \n}\n\n#include\
    \ \"template/template_main.hpp\"\nMain<init, main2, test> main_dummy;\nint main()\
    \ {}\n"
  dependsOn: []
  isVerificationFile: true
  path: verify/yosupo/point_add_range_sum.test.cpp
  requiredBy: []
  timestamp: '1970-01-01 00:00:00+00:00'
  verificationStatus: TEST_WRONG_ANSWER
  verifiedWith: []
documentation_of: verify/yosupo/point_add_range_sum.test.cpp
layout: document
redirect_from:
- /verify/verify/yosupo/point_add_range_sum.test.cpp
- /verify/verify/yosupo/point_add_range_sum.test.cpp.html
title: verify/yosupo/point_add_range_sum.test.cpp
---
