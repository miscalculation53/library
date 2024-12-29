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
  - icon: ':heavy_check_mark:'
    path: template/template_vector.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08vector\uFF09"
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
  bundledCode: "#line 1 \"verify/mytest/template_vector.test.cpp\"\n#define PROBLEM\
    \ \"https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A\"\n\n#line\
    \ 2 \"template/template_vector.hpp\"\n\n#line 2 \"template/template_types.hpp\"\
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
    \u30FC\u30C8\uFF08rep\uFF09\n * @docs docs/template/template_rep.md\n */\n\n/**\n\
    \ * \u53C2\u8003\uFF1A\n * https://trap.jp/post/1224/\n*/\n\n#define overload4(_1,\
    \ _2, _3, _4, name, ...) name\n#define rep1(i, n) for (ll i = 0, nnnnn = ll(n);\
    \ i < nnnnn; i++)\n#define rep2(i, l, r) for (ll i = ll(l), rrrrr = ll(r); i <\
    \ rrrrr; i++)\n#define rep3(i, l, r, d) for (ll i = ll(l), rrrrr = ll(r), ddddd\
    \ = ll(d); ddddd > 0 ? i < rrrrr : i > rrrrr; i += d)\n#define rep(...) overload4(__VA_ARGS__,\
    \ rep3, rep2, rep1)(__VA_ARGS__)\n#define repi1(i, n) for (int i = 0, nnnnn =\
    \ int(n); i < nnnnn; i++)\n#define repi2(i, l, r) for (int i = int(l), rrrrr =\
    \ int(r); i < rrrrr; i++)\n#define repi3(i, l, r, d) for (int i = int(l), rrrrr\
    \ = int(r), ddddd = int(d); ddddd > 0 ? i < rrrrr : i > rrrrr; i += d)\n#define\
    \ repi(...) overload4(__VA_ARGS__, repi3, repi2, repi1)(__VA_ARGS__)\n\n#define\
    \ fe(...) for (auto __VA_ARGS__)\n#define fec(...) for (cauto &__VA_ARGS__)\n\
    #define fem(...) for (auto &__VA_ARGS__)\n#line 2 \"template/template_math.hpp\"\
    \n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n#endif\n#ifndef EPS\n\
    #define EPS 1e-11\n#endif\n\n#line 12 \"template/template_math.hpp\"\n\n/**\n\
    \ * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09\n * @docs\
    \ docs/template/template_math.md\n */\n\ninline bool chmin(auto &a, cauto &b)\
    \ { return a > b ? a = b, true : false; }\ninline bool chmax(auto &a, cauto &b)\
    \ { return a < b ? a = b, true : false; }\n\ntemplate <class T = ll>\ninline constexpr\
    \ T divfloor(cauto &a, cauto &b) { return T(a) / T(b) - (T(a) % T(b) && (T(a)\
    \ ^ T(b)) < 0); }\ntemplate <class T = ll>\ninline constexpr T divceil(cauto &a,\
    \ cauto &b) { return T(a) / T(b) + (T(a) % T(b) && (T(a) ^ T(b)) >= 0); }\ntemplate\
    \ <class T = ll>\ninline constexpr T divround(cauto &a, cauto &b) { return divfloor<T>(2\
    \ * T(a) + T(b), 2 * T(b)); }\ntemplate <class T = ll>\ninline constexpr T safemod(cauto\
    \ &a, cauto &b) { return T(a) - T(b) * divfloor<T>(a, b); }\n\ntemplate <class\
    \ T = ll>\nconstexpr T ipow(auto a, auto b)\n{\n  assert(b >= 0);\n  if (b ==\
    \ 0) return 1;\n  if (a == 0 || a == 1) return a;\n  if (a == -1) return b & 1\
    \ ? -1 : 1;\n\n  T res = 1, tmp = a;\n  while (true)\n  {\n    if (b & 1)\n  \
    \    res *= tmp;\n    b >>= 1;\n    if (b == 0)\n      break;\n    tmp *= tmp;\n\
    \  }\n  return res;\n}\ntemplate <class T = ll>\nT mul_limited(cauto &a, cauto\
    \ &b, cauto &m = INF)\n{\n  assert(a >= 0 && b >= 0 && m >= 0);\n  if (b == 0)\n\
    \    return 0;\n  return T(a) > T(m) / T(b) ? T(m) : T(a) * T(b);\n}\ntemplate\
    \ <class T = ll>\nT pow_limited(cauto &a, cauto &b, cauto &m = INF)\n{\n  assert(a\
    \ >= 0 && b >= 0 && m >= 0);\n  if (a <= 1 || b == 0)\n    return min(ipow<T>(a,\
    \ b), T(m));\n  \n  T res = 1;\n  repi(_, b)\n  {\n    if (res > T(m) / T(a))\n\
    \      return T(m);\n    res *= T(a);\n  }\n  return res;\n}\n\ntemplate <class\
    \ T = ll>\nconstexpr T iroot(cauto &a, cauto &k)\n{\n  assert(a >= 0 && k >= 1);\n\
    \  if (a <= 1 || k == 1)\n    return a;\n\n  auto isok = [&](const T &x) -> bool\n\
    \  {\n    if (x == 0)\n      return true;\n    T tmp = 1;\n    repi(_, k)\n  \
    \  {\n      if (tmp > T(a) / x)\n        return false;\n      tmp *= x;\n    }\n\
    \    return tmp <= T(a);\n  };\n\n  T ok = 0, ng = 1;\n  while (isok(ng))\n  \
    \  ok = ng, ng <<= 1;\n  while (ng - ok > 1)\n  {\n    T mid = ((ng - ok) >> 1)\
    \ + ok;\n    if (isok(mid))\n      ok = mid;\n    else\n      ng = mid;\n  }\n\
    \  return ok;\n}\n\n// https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html\n\
    template <class D = decltype(EPS)>\nint sgn(cauto &a, const D &eps = EPS) { return\
    \ int(a > eps) - int(a < -eps); }\n\n// \u4F4D\u53D6\u308A\u8A18\u6570\u6CD5\u3068\
    \u540C\u3058\u9806\u756A\uFF08\u4E0B\u4F4D\u6841\u304C\u5F8C\u308D\uFF09\n// 0\
    \ \u306B\u5BFE\u3057\u3066\u306F {0} \u304C\u8FD4\u308B\ntemplate <class T = ll>\n\
    vc<T> base_repr(auto val, auto base)\n{\n  assert(val >= 0);\n  assert(base >=\
    \ 2);\n  if (val == 0)\n    return {0};\n  vc<T> a;\n  while (val > 0)\n  {\n\
    \    a.emplace_back(val % base);\n    val /= base;\n  }\n  reverse(a.begin(),\
    \ a.end());\n  return a;\n}\n// \u4F4D\u53D6\u308A\u8A18\u6570\u6CD5\u3068\u540C\
    \u3058\u9806\u756A\uFF08\u4E0B\u4F4D\u6841\u304C\u5F8C\u308D\uFF09\ntemplate <class\
    \ T = ll>\nvc<T> base_repr(auto val, auto base, int n)\n{\n  assert(val >= 0);\n\
    \  assert(base >= 2);\n  assert(n >= 0);\n  vc<T> a(n);\n  repi(i, n)\n  {\n \
    \   a[i] = val % base;\n    val /= base;\n  }\n  reverse(a.begin(), a.end());\n\
    \  return a;\n}\nstring base_repr_str(auto val, int base, bool use_upper = true)\n\
    {\n  assert(val >= 0);\n  assert(2 <= base && base <= 36);\n  auto a = base_repr(val,\
    \ base);\n  string s = \"\";\n  for (cauto &ai : a)\n    s += (ai < 10 ? '0' +\
    \ ai : (use_upper ? 'A' : 'a') + (ai - 10));\n  return s;\n}\nstring base_repr_str(auto\
    \ val, int base, int n, bool use_upper = true)\n{\n  assert(val >= 0);\n  assert(2\
    \ <= base && base <= 36);\n  assert(n >= 0);\n  auto a = base_repr(val, base,\
    \ n);\n  string s = \"\";\n  for (cauto &ai : a)\n    s += (ai < 10 ? '0' + ai\
    \ : (use_upper ? 'A' : 'a') + (ai - 10));\n  return s;\n}\n#line 6 \"template/template_vector.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08vector\uFF09\n *\
    \ @docs docs/template/template_vector.md\n */\n\n#define ALL(a) (a).begin(), (a).end()\n\
    template <class T = ll>\ninline T SZ(cauto &x) { return x.size(); }\n\ntemplate\
    \ <class F>\nauto gen_vec(const int &n, const F &f)\n{\n  vc<decltype(f(0))> res(n);\n\
    \  repi(i, n) res[i] = f(i);\n  return res;\n}\n\n// https://qiita.com/Chippppp/items/13150f5e0ea99f444d97#%E5%A4%9A%E6%AC%A1%E5%85%83vector%E7%94%9F%E6%88%90%E9%96%A2%E6%95%B0\n\
    template <class T, size_t d, size_t i = 0>\nauto dvec(cauto (&sz)[d], const T\
    \ &init)\n{\n  if constexpr (i < d)\n    return vc(sz[i], dvec<T, d, i + 1>(sz,\
    \ init));\n  else\n    return init;\n}\n\ntemplate <class T = ll>\nT ctol(const\
    \ char &c, const string &s)\n{\n  repi(i, SZ<int>(s)) if (s[i] == c) return i;\n\
    \  return -1;\n}\ntemplate <class T = ll>\nvc<T> stov(const string &s, const char\
    \ &first)\n{\n  return gen_vec(SZ<int>(s), [&](int i) -> T\n                 {\
    \ return s[i] - first; });\n}\ntemplate <class T = ll>\nvc<T> stov(const string\
    \ &s, const string &t)\n{\n  return gen_vec(SZ<int>(s), [&](int i) -> T\n    \
    \             { return ctol(s[i], t); });\n}\n\ntemplate <class T>\nvc<T> concat(const\
    \ vvc<T> &vs)\n{\n  vc<T> res;\n  for (cauto &v : vs)\n    res.insert(res.end(),\
    \ ALL(v));\n  return res;\n}\ntemplate <class T>\nvc<T> concat(const vc<T> &v)\
    \ { return v; }\ntemplate <class T, class... Ts>\nvc<T> concat(vc<T> v, const\
    \ vc<Ts> &...vs)\n{\n  (v.insert(v.end(), ALL(vs)), ...);\n  return v;\n}\n\n\
    template <class T>\nT vecget(const vc<T> &v, cauto &i, const T &dflt_negative\
    \ = -INF, const T &dflt_positive = INF)\n{\n  if (i < 0)\n    return dflt_negative;\n\
    \  if (i >= SZ<int>(v))\n    return dflt_positive;\n  return v[i];\n}\n#line 2\
    \ \"template/template_dump.hpp\"\n\n#line 4 \"template/template_dump.hpp\"\n\n\
    /**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09\n * @docs\
    \ docs/template/template_dump.md\n */\n\n#ifdef LOCAL\n#include <cpp-dump.hpp>\
    \ // https://github.com/philip82148/cpp-dump\nnamespace cpp_dump::_detail\n{\n\
    \  inline string export_var(\n      const i128 &x, const string &indent, size_t\
    \ last_line_length,\n      size_t current_depth, bool fail_on_newline, const export_command\
    \ &command\n  ) {\n    return export_var(i128tos(x), indent, last_line_length,\
    \ current_depth, fail_on_newline, command);\n  }\n} // namespace cpp_dump::_detail\n\
    #define dump(...) cpp_dump(__VA_ARGS__)\nnamespace cp = cpp_dump;\nCPP_DUMP_SET_OPTION_GLOBAL(log_label_func,\
    \ cp::log_label::line());\nCPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 10000);\n\
    #define local(...) __VA_ARGS__\n#else\n#define dump(...)\n#define local(...)\n\
    #endif\n#line 5 \"verify/mytest/template_vector.test.cpp\"\n\nvoid test1()\n{\n\
    \  auto dp = dvec({3, 4, 5}, 0LL);\n  dump(dp);\n  assert(SZ(dp) == 3);\n  rep(i,\
    \ 3)\n  {\n    assert(SZ(dp.at(i)) == 4);\n    rep(j, 4) assert(SZ(dp.at(i).at(j))\
    \ == 5);\n  }\n}\n\nvoid test2()\n{\n  assert(ctol('J', \"JOI\") == 0);\n  assert(ctol('O',\
    \ \"JOI\") == 1);\n  assert(ctol('I', \"JOI\") == 2);\n  assert(ctol('?', \"JOI\"\
    ) == -1);\n\n  vl v = {0, 1, 2, 3, 4};\n  auto v1 = stov(\"ABCDE\", 'A');\n  auto\
    \ v2 = stov(\"abcde\", 'a');\n  auto v3 = stov(\"01234\", '0');\n  assert(v ==\
    \ v1 && v == v2 && v == v3);\n\n  vl w = {0, 1, 2, 1, 0, 2};\n  auto w1 = stov(\"\
    RSPSRP\", \"RSP\");\n  assert(w == w1);\n}\n\nvoid test3()\n{\n  vvl vv = {\n\
    \    {0, 1},\n    {2, 3, 4},\n    {5}\n  };\n  vl v = {0, 1, 2, 3, 4, 5};\n\n\
    \  vl v1 = concat(vv);\n  vl v2 = concat(vv.at(0), vv.at(1), vv.at(2));\n  vl\
    \ v3 = concat(vv.at(0), vc{2, 3, 4}, vc{5});\n  dump(v, v1, v2, v3);\n  assert(v\
    \ == v1 && v == v2 && v == v3);\n}\n\nint main()\n{\n  test1();\n  test2();\n\
    \  test3();\n\n  cout << \"Hello World\" << endl;\n}\n"
  code: "#define PROBLEM \"https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A\"\
    \n\n#include \"../../template/template_vector.hpp\"\n#include \"../../template/template_dump.hpp\"\
    \n\nvoid test1()\n{\n  auto dp = dvec({3, 4, 5}, 0LL);\n  dump(dp);\n  assert(SZ(dp)\
    \ == 3);\n  rep(i, 3)\n  {\n    assert(SZ(dp.at(i)) == 4);\n    rep(j, 4) assert(SZ(dp.at(i).at(j))\
    \ == 5);\n  }\n}\n\nvoid test2()\n{\n  assert(ctol('J', \"JOI\") == 0);\n  assert(ctol('O',\
    \ \"JOI\") == 1);\n  assert(ctol('I', \"JOI\") == 2);\n  assert(ctol('?', \"JOI\"\
    ) == -1);\n\n  vl v = {0, 1, 2, 3, 4};\n  auto v1 = stov(\"ABCDE\", 'A');\n  auto\
    \ v2 = stov(\"abcde\", 'a');\n  auto v3 = stov(\"01234\", '0');\n  assert(v ==\
    \ v1 && v == v2 && v == v3);\n\n  vl w = {0, 1, 2, 1, 0, 2};\n  auto w1 = stov(\"\
    RSPSRP\", \"RSP\");\n  assert(w == w1);\n}\n\nvoid test3()\n{\n  vvl vv = {\n\
    \    {0, 1},\n    {2, 3, 4},\n    {5}\n  };\n  vl v = {0, 1, 2, 3, 4, 5};\n\n\
    \  vl v1 = concat(vv);\n  vl v2 = concat(vv.at(0), vv.at(1), vv.at(2));\n  vl\
    \ v3 = concat(vv.at(0), vc{2, 3, 4}, vc{5});\n  dump(v, v1, v2, v3);\n  assert(v\
    \ == v1 && v == v2 && v == v3);\n}\n\nint main()\n{\n  test1();\n  test2();\n\
    \  test3();\n\n  cout << \"Hello World\" << endl;\n}"
  dependsOn:
  - template/template_vector.hpp
  - template/template_types.hpp
  - template/template_rep.hpp
  - template/template_math.hpp
  - template/template_dump.hpp
  isVerificationFile: true
  path: verify/mytest/template_vector.test.cpp
  requiredBy: []
  timestamp: '2024-12-30 04:05:44+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/mytest/template_vector.test.cpp
layout: document
redirect_from:
- /verify/verify/mytest/template_vector.test.cpp
- /verify/verify/mytest/template_vector.test.cpp.html
title: verify/mytest/template_vector.test.cpp
---
