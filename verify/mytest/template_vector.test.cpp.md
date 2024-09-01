---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: template/template_dump.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09"
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
    PROBLEM: https://judge.yosupo.jp/problem/aplusb
    links:
    - https://judge.yosupo.jp/problem/aplusb
  bundledCode: "#line 1 \"verify/mytest/template_vector.test.cpp\"\n#define PROBLEM\
    \ \"https://judge.yosupo.jp/problem/aplusb\"\n\n#line 2 \"template/template_vector.hpp\"\
    \n\n#line 2 \"template/template_types.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\
    \u30EC\u30FC\u30C8\uFF08\u578B\uFF09\n * @docs docs/template/template_types.md\n\
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
    \ repi3, repi2, repi1)(__VA_ARGS__)\n#line 5 \"template/template_vector.hpp\"\n\
    \n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08vector\uFF09\n * @docs\
    \ docs/template/template_vector.md\n */\n\n#define ALL(a) (a).begin(), (a).end()\n\
    #define SZ(x) (ll)((x).size())\n#define SZI(x) (int)((x).size())\n\ntemplate <class\
    \ F>\nauto gen_vec(const int &n, const F &f)\n{\n  vc<decltype(f(0))> res(n);\n\
    \  repi(i, n) res[i] = f(i);\n  return res;\n}\n\n// https://qiita.com/Chippppp/items/13150f5e0ea99f444d97#%E5%A4%9A%E6%AC%A1%E5%85%83vector%E7%94%9F%E6%88%90%E9%96%A2%E6%95%B0\n\
    template <class T, size_t d, size_t i = 0>\nauto dvec(cauto (&sz)[d], const T\
    \ &init)\n{\n  if constexpr (i < d)\n    return vc(sz[i], dvec<T, d, i + 1>(sz,\
    \ init));\n  else\n    return init;\n}\n\ntemplate <class T = ll>\nT ctol(const\
    \ char &c, const string &s)\n{\n  repi(i, SZI(s)) if (s[i] == c) return i;\n \
    \ return -1;\n}\ntemplate <class T = ll>\nvc<T> stov(const string &s, const char\
    \ &first)\n{\n  return gen_vec(SZI(s), [&](int i) -> T\n                 { return\
    \ s[i] - first; });\n}\ntemplate <class T = ll>\nvc<T> stov(const string &s, const\
    \ string &t)\n{\n  return gen_vec(SZI(s), [&](int i) -> T\n                 {\
    \ return ctol(s[i], t); });\n}\n\ntemplate <class T>\nvc<T> concat(const vvc<T>\
    \ &vs)\n{\n  vc<T> res;\n  for (cauto &v : vs)\n    res.insert(res.end(), ALL(v));\n\
    \  return res;\n}\ntemplate <class T>\nvc<T> concat(const vc<T> &v) { return v;\
    \ }\ntemplate <class T, class... Ts>\nvc<T> concat(vc<T> v, const vc<Ts> &...vs)\n\
    {\n  (v.insert(v.end(), ALL(vs)), ...);\n  return v;\n}\n#line 2 \"template/template_dump.hpp\"\
    \n\n#line 4 \"template/template_dump.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\
    \u30EC\u30FC\u30C8\uFF08dump\uFF09\n * @docs docs/template/template_dump.md\n\
    \ */\n\n#ifdef LOCAL\n  #include <cpp-dump.hpp> // https://github.com/philip82148/cpp-dump\n\
    \  namespace cpp_dump::_detail\n  {\n    inline string export_var(\n        const\
    \ i128 &x, const string &indent, size_t last_line_length,\n        size_t current_depth,\
    \ bool fail_on_newline, const export_command &command\n    ) {\n      return export_var(i128tos(x),\
    \ indent, last_line_length, current_depth, fail_on_newline, command);\n    }\n\
    \  } // namespace cpp_dump::_detail\n  #define dump(...) cpp_dump(__VA_ARGS__)\n\
    \  namespace cp = cpp_dump;\n  CPP_DUMP_SET_OPTION_GLOBAL(log_label_func, cp::log_label::line());\n\
    \  CPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 10000);\n#else\n  #define dump(...)\n\
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
    \  test3();\n\n  ll A, B;\n  cin >> A >> B;\n  cout << A + B << endl;\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/aplusb\"\n\n#include \"\
    ../../template/template_vector.hpp\"\n#include \"../../template/template_dump.hpp\"\
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
    \  test3();\n\n  ll A, B;\n  cin >> A >> B;\n  cout << A + B << endl;\n}"
  dependsOn:
  - template/template_vector.hpp
  - template/template_types.hpp
  - template/template_rep.hpp
  - template/template_dump.hpp
  isVerificationFile: true
  path: verify/mytest/template_vector.test.cpp
  requiredBy: []
  timestamp: '2024-09-01 17:45:18+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/mytest/template_vector.test.cpp
layout: document
redirect_from:
- /verify/verify/mytest/template_vector.test.cpp
- /verify/verify/mytest/template_vector.test.cpp.html
title: verify/mytest/template_vector.test.cpp
---
