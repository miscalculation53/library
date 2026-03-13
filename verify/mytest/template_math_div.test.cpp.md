---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: template/template_dump.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09"
  - icon: ':question:'
    path: template/template_math.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09"
  - icon: ':question:'
    path: template/template_rep.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09"
  - icon: ':question:'
    path: template/template_types.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09"
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A
    links:
    - https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A
  bundledCode: "#line 1 \"verify/mytest/template_math_div.test.cpp\"\n#define PROBLEM\
    \ \"https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A\"\n\n#line\
    \ 2 \"template/template_math.hpp\"\n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n\
    #endif\n#ifndef EPS\n#define EPS 1e-11\n#endif\n\n#line 2 \"template/template_types.hpp\"\
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
    }\n#endif\n\n#define cauto const auto\n#line 2 \"template/template_rep.hpp\"\n\
    \n#line 4 \"template/template_rep.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\
    \u30FC\u30C8\uFF08rep\uFF09\n * @docs docs/template/template_rep.md\n */\n\n//\
    \ https://trap.jp/post/1224/\n\n#define overload4(_1, _2, _3, _4, name, ...) name\n\
    #define rep1(i, n) for (ll i = 0, nnnnn = ll(n); i < nnnnn; i++)\n#define rep2(i,\
    \ l, r) for (ll i = ll(l), rrrrr = ll(r); i < rrrrr; i++)\n#define rep3(i, l,\
    \ r, d) for (ll i = ll(l), rrrrr = ll(r), ddddd = ll(d); ddddd > 0 ? i < rrrrr\
    \ : i > rrrrr; i += d)\n#define rep(...) overload4(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)\n\
    #define repi1(i, n) for (int i = 0, nnnnn = int(n); i < nnnnn; i++)\n#define repi2(i,\
    \ l, r) for (int i = int(l), rrrrr = int(r); i < rrrrr; i++)\n#define repi3(i,\
    \ l, r, d) for (int i = int(l), rrrrr = int(r), ddddd = int(d); ddddd > 0 ? i\
    \ < rrrrr : i > rrrrr; i += d)\n#define repi(...) overload4(__VA_ARGS__, repi3,\
    \ repi2, repi1)(__VA_ARGS__)\n\n#define fe(...) for (auto __VA_ARGS__)\n#define\
    \ fec(...) for (cauto &__VA_ARGS__)\n#define fem(...) for (auto &__VA_ARGS__)\n\
    #line 12 \"template/template_math.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\
    \u30FC\u30C8\uFF08\u6F14\u7B97\uFF09\n * @docs docs/template/template_math.md\n\
    \ */\n\ntemplate <class T, class U>\ninline bool chmin(T &a, U b) { return a >\
    \ b ? a = b, true : false; }\ntemplate <class T, class U>\ninline bool chmax(T\
    \ &a, U b) { return a < b ? a = b, true : false; }\n\ntemplate <class T = ll,\
    \ class U, class V>\ninline constexpr T divfloor(U a, V b) { return T(a) / T(b)\
    \ - (T(a) % T(b) && (T(a) ^ T(b)) < 0); }\ntemplate <class T = ll, class U, class\
    \ V>\ninline constexpr T divceil(U a, V b) { return T(a) / T(b) + (T(a) % T(b)\
    \ && (T(a) ^ T(b)) >= 0); }\ntemplate <class T = ll, class U, class V>\ninline\
    \ constexpr T divround(U a, V b) { return divfloor<T>(2 * T(a) + T(b), 2 * T(b));\
    \ }\ntemplate <class T = ll, class U, class V>\ninline constexpr T safemod(U a,\
    \ V b) { return T(a) - T(b) * divfloor<T>(a, b); }\n\ntemplate <class T = ll,\
    \ class U, class V>\nconstexpr T ipow(U a, V b)\n{\n  assert(b >= 0);\n  if (b\
    \ == 0)\n    return 1;\n  if (a == 0 || a == 1)\n    return a;\n  if (a < 0 &&\
    \ a == -1)\n    return b & 1 ? -1 : 1;\n\n  T res = 1, tmp = a;\n  while (true)\n\
    \  {\n    if (b & 1)\n      res *= tmp;\n    b >>= 1;\n    if (b == 0)\n     \
    \ break;\n    tmp *= tmp;\n  }\n  return res;\n}\ntemplate <class T = ll, class\
    \ A, class B, class M>\nT mul_limited(A a, B b, M m)\n{\n  assert(a >= 0 && b\
    \ >= 0 && m >= 0);\n  if (b == 0)\n    return 0;\n  return T(a) > T(m) / T(b)\
    \ ? T(m) : T(a) * T(b);\n}\ntemplate <class T = ll, class A, class B>\nT mul_limited(A\
    \ a, B b) { return mul_limited<T>(a, b, INF); }\ntemplate <class T = ll, class\
    \ A, class B, class M>\nT pow_limited(A a, B b, M m)\n{\n  assert(a >= 0 && b\
    \ >= 0 && m >= 0);\n  if (a <= 1 || b == 0)\n    return min(ipow<T>(a, b), T(m));\n\
    \  \n  T res = 1, tmp = a;\n  while (true)\n  {\n    if (b & 1)\n    {\n     \
    \ if (res > T(m) / tmp)\n        return m;\n      res *= tmp;\n    }\n    b >>=\
    \ 1;\n    if (b == 0)\n      break;\n    if (tmp > T(m) / tmp)\n      return m;\n\
    \    tmp *= tmp;\n  }\n  return res;\n}\ntemplate <class T = ll, class A, class\
    \ B>\nT pow_limited(A a, B b) { return pow_limited<T>(a, b, INF); }\n\ntemplate\
    \ <class T = ll, class A, class K>\nconstexpr T iroot(A a, K k)\n{\n  assert(a\
    \ >= 0 && k >= 1);\n  if (a <= 1 || k == 1)\n    return a;\n  if (k == 2)\n  {\n\
    \    if constexpr (sizeof(T) > sizeof(ull))\n    {\n      if ((u128)a < ((u128)1\
    \ << 120))\n        return sqrtl(a);\n    }\n    else\n      return sqrtl(a);\n\
    \  }\n\n  auto isok = [&](T x) -> bool\n  {\n    if (x == 0)\n      return true;\n\
    \    T res = 1, k2 = k;\n    while (true)\n    {\n      if (k2 & 1)\n      {\n\
    \        if (res > T(a) / x)\n          return false;\n        res *= x;\n   \
    \   }\n      k2 >>= 1;\n      if (k2 == 0)\n        break;\n      if (x > T(a)\
    \ / x)\n        return false;\n      x *= x;\n    }\n    return res <= T(a);\n\
    \  };\n\n  T x = pow(a, 1.0 / k);\n  bool up = true;\n  while (!isok(x))\n   \
    \ up = false, x--;\n  if (up)\n  {\n    while (x < numeric_limits<T>::max() &&\
    \ isok(x + 1))\n      x++;\n  }\n  return x;\n}\ntemplate <class T = ll, class\
    \ A, class K>\nconstexpr T iroot_ceil(A a, K k)\n{\n  T x = iroot<T>(a, k);\n\
    \  return ipow<T>(x, k) == a ? x : x + 1;\n}\n\n// https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html\n\
    template <class D = decltype(EPS), class A>\nint SGN(A a, D eps = EPS) { return\
    \ int(a > eps) - int(a < -eps); }\n\n// \u4F4D\u53D6\u308A\u8A18\u6570\u6CD5\u3068\
    \u540C\u3058\u9806\u756A\uFF08\u4E0B\u4F4D\u6841\u304C\u5F8C\u308D\uFF09\n// 0\
    \ \u306B\u5BFE\u3057\u3066\u306F {0} \u304C\u8FD4\u308B\ntemplate <class T = ll,\
    \ class U, class V>\nvc<T> base_repr(U val, V base)\n{\n  assert(val >= 0);\n\
    \  assert(base >= 2);\n  if (val == 0)\n    return {0};\n  vc<T> a;\n  while (val\
    \ > 0)\n  {\n    a.emplace_back(val % base);\n    val /= base;\n  }\n  reverse(a.begin(),\
    \ a.end());\n  return a;\n}\n// \u4F4D\u53D6\u308A\u8A18\u6570\u6CD5\u3068\u540C\
    \u3058\u9806\u756A\uFF08\u4E0B\u4F4D\u6841\u304C\u5F8C\u308D\uFF09\ntemplate <class\
    \ T = ll, class U, class V>\nvc<T> base_repr(U val, V base, int n)\n{\n  assert(val\
    \ >= 0);\n  assert(base >= 2);\n  assert(n >= 0);\n  vc<T> a(n);\n  repi(i, n)\n\
    \  {\n    a[i] = val % base;\n    val /= base;\n  }\n  reverse(a.begin(), a.end());\n\
    \  return a;\n}\ntemplate <const bool use_upper = true, class U>\nstring base_repr_str(U\
    \ val, int base)\n{\n  assert(val >= 0);\n  assert(2 <= base && base <= 36);\n\
    \  auto a = base_repr(val, base);\n  string s = \"\";\n  for (cauto &ai : a)\n\
    \    s += (ai < 10 ? '0' + ai : (use_upper ? 'A' : 'a') + (ai - 10));\n  return\
    \ s;\n}\ntemplate <const bool use_upper = true, class U>\nstring base_repr_str(U\
    \ val, int base, int n)\n{\n  assert(val >= 0);\n  assert(2 <= base && base <=\
    \ 36);\n  assert(n >= 0);\n  auto a = base_repr(val, base, n);\n  string s = \"\
    \";\n  for (cauto &ai : a)\n    s += (ai < 10 ? '0' + ai : (use_upper ? 'A' :\
    \ 'a') + (ai - 10));\n  return s;\n}\n#line 2 \"template/template_dump.hpp\"\n\
    \n#line 4 \"template/template_dump.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\
    \u30FC\u30C8\uFF08dump\uFF09\n * @docs docs/template/template_dump.md\n */\n\n\
    #ifdef LOCAL\n#include <cpp-dump.hpp> // https://github.com/philip82148/cpp-dump\n\
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
    \ res;\n}\n#line 5 \"verify/mytest/template_math_div.test.cpp\"\n\nmt19937 mt;\n\
    ll randll() { return (mt() << 30) + mt(); }\n\ntemplate <class T>\nvoid test1()\n\
    {\n  for (int t = 0; t < 100000; t++)\n  {\n    dump(t);\n    T a = randll(),\
    \ b = randll();\n    if (b == 0)\n      continue;\n    if (mt() % 2) a *= -1;\n\
    \    if (mt() % 2) b *= -1;\n\n    T c = divfloor<T>(a, b), d = divceil<T>(a,\
    \ b), e = safemod<T>(a, b);\n    dump(a, b, c, d, e);\n    if (b > 0)\n    {\n\
    \      assert(a >= b * c && a < b * (c + 1));\n      assert(a > b * (d - 1) &&\
    \ a <= b * d);\n      assert(0 <= e && e < b);\n      assert((a - e) % b == 0);\n\
    \    }\n    else\n    {\n      assert(a < b * c && a >= b * (c + 1));\n      assert(a\
    \ <= b * (d - 1) && a > b * d);\n      assert(b < e && e <= 0);\n      assert((a\
    \ - e) % b == 0);\n    }\n  }\n}\n\nint main()\n{\n  test1<ll>();\n  test1<i128>();\n\
    \n  cout << \"Hello World\" << endl;\n}\n"
  code: "#define PROBLEM \"https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A\"\
    \n\n#include \"template/template_math.hpp\"\n#include \"template/template_dump.hpp\"\
    \n\nmt19937 mt;\nll randll() { return (mt() << 30) + mt(); }\n\ntemplate <class\
    \ T>\nvoid test1()\n{\n  for (int t = 0; t < 100000; t++)\n  {\n    dump(t);\n\
    \    T a = randll(), b = randll();\n    if (b == 0)\n      continue;\n    if (mt()\
    \ % 2) a *= -1;\n    if (mt() % 2) b *= -1;\n\n    T c = divfloor<T>(a, b), d\
    \ = divceil<T>(a, b), e = safemod<T>(a, b);\n    dump(a, b, c, d, e);\n    if\
    \ (b > 0)\n    {\n      assert(a >= b * c && a < b * (c + 1));\n      assert(a\
    \ > b * (d - 1) && a <= b * d);\n      assert(0 <= e && e < b);\n      assert((a\
    \ - e) % b == 0);\n    }\n    else\n    {\n      assert(a < b * c && a >= b *\
    \ (c + 1));\n      assert(a <= b * (d - 1) && a > b * d);\n      assert(b < e\
    \ && e <= 0);\n      assert((a - e) % b == 0);\n    }\n  }\n}\n\nint main()\n\
    {\n  test1<ll>();\n  test1<i128>();\n\n  cout << \"Hello World\" << endl;\n}"
  dependsOn:
  - template/template_math.hpp
  - template/template_types.hpp
  - template/template_rep.hpp
  - template/template_dump.hpp
  isVerificationFile: true
  path: verify/mytest/template_math_div.test.cpp
  requiredBy: []
  timestamp: '2025-04-30 22:43:54+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/mytest/template_math_div.test.cpp
layout: document
redirect_from:
- /verify/verify/mytest/template_math_div.test.cpp
- /verify/verify/mytest/template_math_div.test.cpp.html
title: verify/mytest/template_math_div.test.cpp
---
