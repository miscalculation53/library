---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: template/template_all.hpp
    title: template/template_all.hpp
  - icon: ':heavy_check_mark:'
    path: template/template_types.hpp
    title: template/template_types.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/many_aplusb
    links:
    - https://judge.yosupo.jp/problem/many_aplusb
  bundledCode: "#line 1 \"verify/yosupo_many_aplusb.test.cpp\"\n#define PROBLEM \"\
    https://judge.yosupo.jp/problem/many_aplusb\"\n\n#line 2 \"template/template_all.hpp\"\
    \n\n#line 2 \"template/template_types.hpp\"\n\n#include <bits/stdc++.h>\nusing\
    \ namespace std;\n\nusing ll = long long;\nusing dbl = double;\nusing ld = long\
    \ double;\nusing uint = unsigned int;\nusing ull = unsigned long long;\nusing\
    \ pll = pair<ll, ll>;\nusing tlll = tuple<ll, ll, ll>;\nusing tllll = tuple<ll,\
    \ ll, ll, ll>;\n\n#define vc vector\nusing vl = vc<ll>;\nusing vpll = vc<pll>;\n\
    using vtlll = vc<tlll>;\nusing vtllll = vc<tllll>;\nusing vvl = vc<vc<ll>>;\n\n\
    #ifdef __SIZEOF_INT128__\n  using i128 = __int128_t;\n  i128 stoi128(const string\
    \ &s)\n  {\n    i128 res = 0;\n    if (s.front() == '-')\n    {\n      for (int\
    \ i = 1; i < (int)s.size(); i++)\n        res = 10 * res + s[i] - '0';\n     \
    \ res = -res;\n    }\n    else\n    {\n      for (auto &&c : s)\n        res =\
    \ 10 * res + c - '0';\n    }\n    return res;\n  }\n  string i128tos(i128 x)\n\
    \  {\n    if (x == 0) return \"0\";\n    string sign = \"\", res = \"\";\n   \
    \ if (x < 0)\n      x = -x, sign = \"-\";\n    while (x > 0)\n    {\n      res\
    \ += '0' + x % 10;\n      x /= 10;\n    }\n    reverse(res.begin(), res.end());\n\
    \    return sign + res;\n  }\n  istream &operator>>(istream &is, i128 &a)\n  {\n\
    \    string s;\n    is >> s;\n    a = stoi128(s);\n    return is;\n  }\n  ostream\
    \ &operator<<(ostream &os, const i128 &a)\n  {\n    os << i128tos(a);\n    return\
    \ os;\n  }\n#endif\n#line 4 \"verify/yosupo_many_aplusb.test.cpp\"\n\n// #define\
    \ SINGLE_TESTCASE\n#define MULTI_TESTCASE\n// #define AOJ_TESTCASE\n\n#define\
    \ FAST_IO\n\nvoid init() {}\n\nvoid main2()\n{\n  ll A, B;\n  cin >> A >> B;\n\
    \  cout << A + B << \"\\n\";\n}\n\nvoid test()\n{\n  /*\n  #ifdef LOCAL\n  rep(t,\
    \ 100000)\n  {\n    dump(t);\n\n    // ----- generate cases -----\n    ll N =\
    \ 1 + rand() % 5;\n    vl A(N);\n    rep(i, N) A.at(i) = 1 + rand() % 10;\n  \
    \  // --------------------------\n\n    // ------ check output ------\n    auto\
    \ god = naive(A);\n    auto ans = solve(A);\n    if (god != ans)\n    {\n    \
    \  dump(N, A);\n      dump(god, ans);\n      exit(0);\n    }\n    // --------------------------\n\
    \  }\n  dump(\"ok\");\n  #endif\n  //*/\n}\n\nint main()\n{\n  #if defined FAST_IO\
    \ and not defined LOCAL\n    cerr << \"[FAST_IO]\\n\\n\";\n    cin.tie(0);\n \
    \   ios::sync_with_stdio(false);\n  #endif\n  cout << fixed << setprecision(20);\n\
    \n  init();\n  test();\n\n  #if defined AOJ_TESTCASE or (defined LOCAL and defined\
    \ SINGLE_TESTCASE)\n    cerr << \"[AOJ_TESTCASE]\\n\\n\";\n    while (true) main2();\n\
    \  #elif defined SINGLE_TESTCASE\n    cerr << \"[SINGLE_TESTCASE]\\n\\n\";\n \
    \   main2();\n  #elif defined MULTI_TESTCASE\n    cerr << \"[MULTI_TESTCASE]\\\
    n\\n\";\n    int T;\n    cin >> T;\n    while (T--) main2();\n  #endif\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/many_aplusb\"\n\n#include\
    \ \"../template/template_all.hpp\"\n\n// #define SINGLE_TESTCASE\n#define MULTI_TESTCASE\n\
    // #define AOJ_TESTCASE\n\n#define FAST_IO\n\nvoid init() {}\n\nvoid main2()\n\
    {\n  ll A, B;\n  cin >> A >> B;\n  cout << A + B << \"\\n\";\n}\n\nvoid test()\n\
    {\n  /*\n  #ifdef LOCAL\n  rep(t, 100000)\n  {\n    dump(t);\n\n    // ----- generate\
    \ cases -----\n    ll N = 1 + rand() % 5;\n    vl A(N);\n    rep(i, N) A.at(i)\
    \ = 1 + rand() % 10;\n    // --------------------------\n\n    // ------ check\
    \ output ------\n    auto god = naive(A);\n    auto ans = solve(A);\n    if (god\
    \ != ans)\n    {\n      dump(N, A);\n      dump(god, ans);\n      exit(0);\n \
    \   }\n    // --------------------------\n  }\n  dump(\"ok\");\n  #endif\n  //*/\n\
    }\n\nint main()\n{\n  #if defined FAST_IO and not defined LOCAL\n    cerr << \"\
    [FAST_IO]\\n\\n\";\n    cin.tie(0);\n    ios::sync_with_stdio(false);\n  #endif\n\
    \  cout << fixed << setprecision(20);\n\n  init();\n  test();\n\n  #if defined\
    \ AOJ_TESTCASE or (defined LOCAL and defined SINGLE_TESTCASE)\n    cerr << \"\
    [AOJ_TESTCASE]\\n\\n\";\n    while (true) main2();\n  #elif defined SINGLE_TESTCASE\n\
    \    cerr << \"[SINGLE_TESTCASE]\\n\\n\";\n    main2();\n  #elif defined MULTI_TESTCASE\n\
    \    cerr << \"[MULTI_TESTCASE]\\n\\n\";\n    int T;\n    cin >> T;\n    while\
    \ (T--) main2();\n  #endif\n}\n"
  dependsOn:
  - template/template_all.hpp
  - template/template_types.hpp
  isVerificationFile: true
  path: verify/yosupo_many_aplusb.test.cpp
  requiredBy: []
  timestamp: '2024-08-26 23:40:19+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/yosupo_many_aplusb.test.cpp
layout: document
redirect_from:
- /verify/verify/yosupo_many_aplusb.test.cpp
- /verify/verify/yosupo_many_aplusb.test.cpp.html
title: verify/yosupo_many_aplusb.test.cpp
---
