---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: template/template_dump.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_math.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_rep.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_types.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09"
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/aplusb
    links:
    - https://judge.yosupo.jp/problem/aplusb
  bundledCode: "#line 1 \"verify/mytest/template_math_mulpow.test.cpp\"\n#define PROBLEM\
    \ \"https://judge.yosupo.jp/problem/aplusb\"\n\n#line 2 \"template/template_math.hpp\"\
    \n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n#endif\n#ifndef EPS\n\
    #define EPS 1e-11\n#endif\n\n#line 2 \"template/template_types.hpp\"\n\n/**\n\
    \ * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09\n * @docs docs/template/template_types.md\n\
    \ */\n\n#include <bits/stdc++.h>\nusing namespace std;\n\n#ifndef EPS\n#define\
    \ EPS 1e-11\n#endif\nusing ld = decltype(EPS);\n\nusing ll = long long;\nusing\
    \ uint = unsigned int;\nusing ull = unsigned long long;\nusing pll = pair<ll,\
    \ ll>;\nusing tlll = tuple<ll, ll, ll>;\nusing tllll = tuple<ll, ll, ll, ll>;\n\
    \n#define vc vector\ntemplate <class T>\nusing vvc = vc<vc<T>>;\ntemplate <class\
    \ T>\nusing vvvc = vc<vc<vc<T>>>;\n\nusing vb = vc<bool>;\nusing vl = vc<ll>;\n\
    using vpll = vc<pll>;\nusing vtlll = vc<tlll>;\nusing vtllll = vc<tllll>;\nusing\
    \ vstr = vc<string>;\nusing vvb = vvc<bool>;\nusing vvl = vvc<ll>;\n\ntemplate\
    \ <class T>\nusing pql = priority_queue<T, vc<T>, greater<T>>;\ntemplate <class\
    \ T>\nusing pqg = priority_queue<T>;\n\n#ifdef __SIZEOF_INT128__\n  using i128\
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
    \ &a)\n  {\n    os << i128tos(a);\n    return os;\n  }\n#endif\n\n#define cauto\
    \ const auto\n#line 2 \"template/template_rep.hpp\"\n\n#line 4 \"template/template_rep.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09\n * @docs\
    \ docs/template/template_rep.md\n */\n\n/**\n * \u53C2\u8003\uFF1A\n * https://trap.jp/post/1224/\n\
    */\n\n#define overload4(_1, _2, _3, _4, name, ...) name\n#define rep1(i, n) for\
    \ (ll i = 0; i < ll(n); i++)\n#define rep2(i, l, r) for (ll i = ll(l); i < ll(r);\
    \ i++)\n#define rep3(i, l, r, d) for (ll i = ll(l); (d) > 0 ? i < ll(r) : i >\
    \ ll(r); i += d)\n#define rep(...) overload4(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)\n\
    #define repi1(i, n) for (int i = 0; i < int(n); i++)\n#define repi2(i, l, r) for\
    \ (int i = int(l); i < int(r); i++)\n#define repi3(i, l, r, d) for (int i = int(l);\
    \ (d) > 0 ? i < int(r) : i > int(r); i += d)\n#define repi(...) overload4(__VA_ARGS__,\
    \ repi3, repi2, repi1)(__VA_ARGS__)\n#line 12 \"template/template_math.hpp\"\n\
    \n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09\n\
    \ * @docs docs/template/template_math.md\n */\n\ninline bool chmin(auto &a, cauto\
    \ &b) { return a > b ? a = b, true : false; }\ninline bool chmax(auto &a, cauto\
    \ &b) { return a < b ? a = b, true : false; }\n\ntemplate <class T = ll>\ninline\
    \ T divfloor(cauto &a, cauto &b) { return T(a) / T(b) - (T(a) % T(b) && (T(a)\
    \ ^ T(b)) < 0); }\ntemplate <class T = ll>\ninline T divceil(cauto &a, cauto &b)\
    \ { return T(a) / T(b) + (T(a) % T(b) && (T(a) ^ T(b)) >= 0); }\ntemplate <class\
    \ T = ll>\ninline T safemod(cauto &a, cauto &b) { return a - b * divfloor(a, b);\
    \ }\n\ntemplate <class T = ll>\nconstexpr T ipow(cauto &a, cauto &b)\n{\n  assert(b\
    \ >= 0);\n  if (b == 0) return 1;\n  if (a == 0 || a == 1) return a;\n  if (a\
    \ == -1) return b & 1 ? -1 : 1;\n\n  T res = 1;\n  repi(_, b) res *= T(a);\n \
    \ return res;\n}\ntemplate <class T = ll>\nT mul_limited(cauto &a, cauto &b, cauto\
    \ &m = INF)\n{\n  assert(a >= 0 && b >= 0 && m >= 0);\n  if (b == 0)\n    return\
    \ 0;\n  return T(a) > T(m) / T(b) ? T(m) : T(a) * T(b);\n}\ntemplate <class T\
    \ = ll>\nT pow_limited(cauto &a, cauto &b, cauto &m = INF)\n{\n  assert(a >= 0\
    \ && b >= 0 && m >= 0);\n  if (a <= 1 || b == 0)\n    return min(ipow<T>(a, b),\
    \ T(m));\n  \n  T res = 1;\n  repi(_, b)\n  {\n    if (res > T(m) / T(a))\n  \
    \    return T(m);\n    res *= T(a);\n  }\n  return res;\n}\n\ntemplate <class\
    \ T = ll>\nconstexpr T iroot(cauto &a, cauto &k)\n{\n  assert(a >= 0 && k >= 1);\n\
    \  if (a <= 1 || k == 1)\n    return a;\n\n  auto isok = [&](const T &x) -> bool\n\
    \  {\n    if (x == 0)\n      return true;\n    T tmp = 1;\n    repi(_, k)\n  \
    \  {\n      if (tmp > T(a) / x)\n        return false;\n      tmp *= x;\n    }\n\
    \    return tmp <= T(a);\n  };\n\n  T ok = 0, ng = 1;\n  while (isok(ng))\n  \
    \  ok = ng, ng <<= 1;\n  while (ng - ok > 1)\n  {\n    T mid = ((ng - ok) >> 1)\
    \ + ok;\n    if (isok(mid))\n      ok = mid;\n    else\n      ng = mid;\n  }\n\
    \  return ok;\n}\n\n// https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html\n\
    template <class D = decltype(EPS)>\nint sgn(cauto &a, const D &eps = EPS) { return\
    \ int(a > eps) - int(a < -eps); }\n#line 2 \"template/template_dump.hpp\"\n\n\
    #line 4 \"template/template_dump.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\
    \u30FC\u30C8\uFF08dump\uFF09\n * @docs docs/template/template_dump.md\n */\n\n\
    #ifdef LOCAL\n  #include <cpp-dump.hpp> // https://github.com/philip82148/cpp-dump\n\
    \  namespace cpp_dump::_detail\n  {\n    inline string export_var(\n        const\
    \ i128 &x, const string &indent, size_t last_line_length,\n        size_t current_depth,\
    \ bool fail_on_newline, const export_command &command\n    ) {\n      return export_var(i128tos(x),\
    \ indent, last_line_length, current_depth, fail_on_newline, command);\n    }\n\
    \  } // namespace cpp_dump::_detail\n  #define dump(...) cpp_dump(__VA_ARGS__)\n\
    \  namespace cp = cpp_dump;\n  CPP_DUMP_SET_OPTION_GLOBAL(log_label_func, cp::log_label::line());\n\
    \  CPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 10000);\n#else\n  #define dump(...)\n\
    #endif\n#line 5 \"verify/mytest/template_math_mulpow.test.cpp\"\n\nmt19937 mt;\n\
    \nvoid test1()\n{\n  for (int t = 0; t < 100000; t++)\n  {\n    int a = mt() %\
    \ (mt() % 2 == 0 ? 1 << 15 : 1 << 30);\n    int b = mt() % (mt() % 2 == 0 ? 1\
    \ << 15 : 1 << 30);\n    int m = mt() % (1 << 30);\n    int god = min(ll(a) *\
    \ ll(b), ll(m));\n    int ans = mul_limited<int>(a, b, m);\n    dump(t, god ==\
    \ m);\n    assert(god == ans);\n  }\n}\n\nvoid test2()\n{\n  for (int t = 0; t\
    \ < 100000; t++)\n  {\n    int a = mt() % 11;\n    int b = mt() % 19;\n    int\
    \ m = mt() % (mt() % 2 == 0 ? 10 : 1 << 30);\n    int god = min(ipow(a, b), (ll)m);\n\
    \    int ans = pow_limited<int>(a, b, m);\n    dump(t, a, b, ipow(a, b), m, god,\
    \ ans);\n    assert(god == ans);\n  }\n}\n\nint main()\n{\n  test1();\n  test2();\n\
    \n  ll A, B;\n  cin >> A >> B;\n  cout << A + B << endl;\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/aplusb\"\n\n#include \"\
    ../../template/template_math.hpp\"\n#include \"../../template/template_dump.hpp\"\
    \n\nmt19937 mt;\n\nvoid test1()\n{\n  for (int t = 0; t < 100000; t++)\n  {\n\
    \    int a = mt() % (mt() % 2 == 0 ? 1 << 15 : 1 << 30);\n    int b = mt() % (mt()\
    \ % 2 == 0 ? 1 << 15 : 1 << 30);\n    int m = mt() % (1 << 30);\n    int god =\
    \ min(ll(a) * ll(b), ll(m));\n    int ans = mul_limited<int>(a, b, m);\n    dump(t,\
    \ god == m);\n    assert(god == ans);\n  }\n}\n\nvoid test2()\n{\n  for (int t\
    \ = 0; t < 100000; t++)\n  {\n    int a = mt() % 11;\n    int b = mt() % 19;\n\
    \    int m = mt() % (mt() % 2 == 0 ? 10 : 1 << 30);\n    int god = min(ipow(a,\
    \ b), (ll)m);\n    int ans = pow_limited<int>(a, b, m);\n    dump(t, a, b, ipow(a,\
    \ b), m, god, ans);\n    assert(god == ans);\n  }\n}\n\nint main()\n{\n  test1();\n\
    \  test2();\n\n  ll A, B;\n  cin >> A >> B;\n  cout << A + B << endl;\n}"
  dependsOn:
  - template/template_math.hpp
  - template/template_types.hpp
  - template/template_rep.hpp
  - template/template_dump.hpp
  isVerificationFile: true
  path: verify/mytest/template_math_mulpow.test.cpp
  requiredBy: []
  timestamp: '2024-08-31 12:06:34+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/mytest/template_math_mulpow.test.cpp
layout: document
redirect_from:
- /verify/verify/mytest/template_math_mulpow.test.cpp
- /verify/verify/mytest/template_math_mulpow.test.cpp.html
title: verify/mytest/template_math_mulpow.test.cpp
---
