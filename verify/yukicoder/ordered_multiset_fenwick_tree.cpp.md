---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links:
    - https://yukicoder.me/problems/no/649
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
  code: "#define PROBLEM \"https://yukicoder.me/problems/no/649\"\n\n#define SINGLE_TESTCASE\n\
    // #define MULTI_TESTCASE\n// #define AOJ_TESTCASE\n\n#ifndef LOCAL\n#define FAST_IO\n\
    // #define FAST_CIO\n// #define INTERACTIVE\n#endif\n\n#define INF 4'000'000'000'000'000'037LL\n\
    #define EPS 1e-11\n\n#include \"template/template_all_but_modint.hpp\"\n#include\
    \ \"ds/coordinate_compression.hpp\"\n#include \"ds/fenwick_tree/fenwick_tree.hpp\"\
    \n\nvoid init() {}\n\nvoid main2()\n{\n  LL(Q, K);\n  K--;\n\n  vl vals;\n  vc<pll>\
    \ queries(Q);\n  rep(q, Q)\n  {\n    LL(t);\n    if (t == 1)\n    {\n      LL(v);\n\
    \      vals.eb(v);\n      queries.at(q) = {t, v};\n    }\n    else if (t == 2)\n\
    \    {\n      queries.at(q) = {t, -1};\n    }\n  }\n  CoordinateCompression cc(vals);\n\
    \  FenwickTree<GroupAddSub<ll>> fw(cc.size());\n  fec([t, v] : queries)\n  {\n\
    \    if (t == 1)\n    {\n      ll i = cc.get_id(v);\n      fw.add(i, 1);\n   \
    \ }\n    else if (t == 2)\n    {\n      ll i = fw.kth_of_multiset(K);\n      if\
    \ (i == fw.size())\n        PRINT(-1);\n      else\n      {\n        PRINT(cc.get_val(i));\n\
    \        fw.add(i, -1);\n      }\n    }\n  }\n}\n\nvoid test()\n{\n  \n}\n\n#include\
    \ \"template/template_main.hpp\"\nMain<init, main2, test> main_dummy;\nint main()\
    \ {}\n"
  dependsOn: []
  isVerificationFile: false
  path: verify/yukicoder/ordered_multiset_fenwick_tree.cpp
  requiredBy: []
  timestamp: '1970-01-01 00:00:00+00:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: verify/yukicoder/ordered_multiset_fenwick_tree.cpp
layout: document
redirect_from:
- /library/verify/yukicoder/ordered_multiset_fenwick_tree.cpp
- /library/verify/yukicoder/ordered_multiset_fenwick_tree.cpp.html
title: verify/yukicoder/ordered_multiset_fenwick_tree.cpp
---
