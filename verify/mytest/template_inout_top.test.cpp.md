---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: template/template_dump.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_inout.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\uFF09"
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
    PROBLEM: https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A
    links:
    - https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A
  bundledCode: "#line 1 \"verify/mytest/template_inout_top.test.cpp\"\n#define PROBLEM\
    \ \"https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A\"\n\n#line\
    \ 2 \"template/template_inout.hpp\"\n\n#line 2 \"template/template_types.hpp\"\
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
    \ const auto\n#line 2 \"template/template_rep.hpp\"\n\n#line 4 \"template/template_rep.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09\n * @docs\
    \ docs/template/template_rep.md\n */\n\n/**\n * \u53C2\u8003\uFF1A\n * https://trap.jp/post/1224/\n\
    */\n\n#define overload4(_1, _2, _3, _4, name, ...) name\n#define rep1(i, n) for\
    \ (ll i = 0, nnnnn = ll(n); i < nnnnn; i++)\n#define rep2(i, l, r) for (ll i =\
    \ ll(l), rrrrr = ll(r); i < rrrrr; i++)\n#define rep3(i, l, r, d) for (ll i =\
    \ ll(l), rrrrr = ll(r), ddddd = ll(d); ddddd > 0 ? i < rrrrr : i > rrrrr; i +=\
    \ d)\n#define rep(...) overload4(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)\n\
    #define repi1(i, n) for (int i = 0, nnnnn = int(n); i < nnnnn; i++)\n#define repi2(i,\
    \ l, r) for (int i = int(l), rrrrr = int(r); i < rrrrr; i++)\n#define repi3(i,\
    \ l, r, d) for (int i = int(l), rrrrr = int(r), ddddd = int(d); ddddd > 0 ? i\
    \ < rrrrr : i > rrrrr; i += d)\n#define repi(...) overload4(__VA_ARGS__, repi3,\
    \ repi2, repi1)(__VA_ARGS__)\n\n#define fe(...) for (auto __VA_ARGS__)\n#define\
    \ fec(...) for (cauto &__VA_ARGS__)\n#define fem(...) for (auto &__VA_ARGS__)\n\
    #line 5 \"template/template_inout.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\
    \u30FC\u30C8\uFF08\u5165\u51FA\u529B\uFF09\n * @docs docs/template/template_inout.md\n\
    \ */\n/**\n * \u53C2\u8003\uFF1A\n * https://trap.jp/post/1224/\n */\n\n// ----\
    \ \u5165\u529B ----\ntemplate <class T, class U>\nistream &operator>>(istream\
    \ &is, pair<T, U> &p)\n{\n  cin >> p.first >> p.second;\n  return is;\n}\ntemplate\
    \ <class T, size_t n>\nistream &operator>>(istream &is, array<T, n> &a)\n{\n \
    \ for (size_t i = 0; i < n; i++)\n    cin >> a[i];\n  return is;\n}\ntemplate\
    \ <class... Ts>\nistream &operator>>(istream &is, tuple<Ts...> &t)\n{\n  apply([&](auto\
    \ &...a)\n        { (is >> ... >> a); }, t);\n  return is;\n}\n\ntemplate <class...\
    \ Ts>\nvoid CIN(Ts &...a) { (cin >> ... >> a); }\n\ntemplate <class T>\nvoid CINVEC(int\
    \ n, vc<T> &v)\n{\n  v.resize(n);\n  repi(i, n) cin >> v[i];\n}\ntemplate <class\
    \ T, class... Ts>\nvoid CINVEC(int n, vc<T> &v, vc<Ts> &...vs)\n{ CINVEC(n, v),\
    \ CINVEC(n, vs...); }\n\ntemplate <class T>\nvoid CINVEC2(int n, int m, vvc<T>\
    \ &v)\n{\n  v.assign(n, vc<T>(m));\n  repi(i, n) repi(j, m) cin >> v[i][j];\n\
    }\ntemplate <class T, class... Ts>\nvoid CINVEC2(int n, int m, vvc<T> &v, vvc<Ts>\
    \ &...vs)\n{ CINVEC2(n, m, v), CINVEC2(n, m, vs...); }\n\n#define IN(T, ...) T\
    \ __VA_ARGS__; CIN(__VA_ARGS__)\n\n#define CHAR(...) IN(char, __VA_ARGS__)\n#define\
    \ INT(...) IN(int, __VA_ARGS__)\n#define LL(...) IN(ll, __VA_ARGS__)\n#define\
    \ STR(...) IN(string, __VA_ARGS__)\n#define ARR(T, n, ...) array<T, n> __VA_ARGS__;\
    \ CIN(__VA_ARGS__)\n\n#define VEC(T, n, ...) vc<T> __VA_ARGS__; CINVEC(n, __VA_ARGS__)\n\
    #define VEC2(T, n, m, ...) vvc<T> __VA_ARGS__; CINVEC2(n, m, __VA_ARGS__)\n//\
    \ ----------\n\n// ----- \u51FA\u529B -----\n#ifdef INTERACTIVE\n#define ENDL\
    \ endl\n#else\n#define ENDL '\\n'\n#endif\n\ntemplate <class T>\nvoid PRINT(const\
    \ T &a) { cout << a << ENDL; }\ntemplate <class T, class... Ts>\nvoid PRINT(const\
    \ T &a, const Ts &...b)\n{\n  cout << a;\n  (cout << ... << (cout << ' ', b));\n\
    \  cout << ENDL;\n}\n#define PRINTEXIT(...) do { PRINT(__VA_ARGS__); exit(0);\
    \ } while (false)\n#define PRINTRETURN(...) do { PRINT(__VA_ARGS__); return; }\
    \ while (false)\n\ntemplate <class T>\nvoid PRINTVEC(const vc<T> &v)\n{\n  const\
    \ int n = v.size();\n  repi(i, n) cout << v[i] << (i == n - 1 ? \"\" : \" \");\n\
    \  cout << ENDL;\n}\ntemplate <class T>\nvoid PRINTVECT(const vc<T> &v) { for\
    \ (auto &vi : v) cout << vi << ENDL; }\ntemplate <class T>\nvoid PRINTVEC2(const\
    \ vvc<T> &v) { for (auto &vi : v) PRINTVEC(vi); }\n// ----------\n\n// ----- \u57FA\
    \u6E96\u305A\u3089\u3057 -----\ntemplate <class T, class U>\npair<T, U> operator+=(pair<T,\
    \ U> &a, cauto &b)\n{\n  a.first += b.first;\n  a.second += b.second;\n  return\
    \ a;\n}\ntemplate <class T, class U>\npair<T, U> operator+(pair<T, U> &a, cauto\
    \ &b) { return a += b; }\n\ntemplate <class T, size_t n>\narray<T, n> operator+=(array<T,\
    \ n> &a, cauto &b)\n{\n  for (size_t i = 0; i < n; i++)\n    a[i] += b[i];\n \
    \ return a;\n}\ntemplate <class T, size_t n>\narray<T, n> operator+(array<T, n>\
    \ &a, cauto &b) { return a += b; }\n\ntemplate <size_t... I>\nauto tuple_add_impl(auto\
    \ &a, cauto &b, const index_sequence<I...>)\n{\n  ((get<I>(a) += get<I>(b)), ...);\n\
    \  return a;\n}\ntemplate <class... Ts>\ntuple<Ts...> operator+=(tuple<Ts...>\
    \ &a, cauto &b)\n{ return tuple_add_impl(a, b, make_index_sequence<tuple_size_v<tuple<Ts...>>>{});\
    \ }\ntemplate <class... Ts>\ntuple<Ts...> operator+(tuple<Ts...> &a, cauto &b)\
    \ { return a += b; }\n\ntemplate <class T>\nvoid offset(vc<T> &v, cauto &add)\
    \ { for (auto &vi : v) vi += add; }\ntemplate <class T>\nvoid offset(vvc<T> &v,\
    \ cauto &add) { for (auto &vi : v) for (auto &vij : vi) vij += add; }\n// ----------\n\
    \n// ----- \u8EE2\u7F6E -----\ntemplate <class T, const size_t m>\narray<vc<T>,\
    \ m> top(const vc<array<T, m>> &vt)\n{\n  const size_t n = vt.size();\n  array<vc<T>,\
    \ m> tv;\n  tv.fill(vc<T>(n));\n  for (size_t i = 0; i < n; i++)\n    for (size_t\
    \ j = 0; j < m; j++)\n      tv[j][i] = vt[i][j];\n  return tv;\n}\ntemplate <class\
    \ T, const size_t m>\nvc<array<T, m>> top(const array<vc<T>, m> &tv)\n{\n  if\
    \ (tv.empty()) return {};\n  const size_t n = tv[0].size();\n  vc<array<T, m>>\
    \ vt(n);\n  for (size_t j = 0; j < m; j++)\n  {\n    assert(tv[j].size() == n);\n\
    \    for (size_t i = 0; i < n; i++)\n      vt[i][j] = tv[j][i];\n  }\n  return\
    \ vt;\n}\n\ntemplate <class T, class U>\npair<vc<T>, vc<U>> top(const vc<pair<T,\
    \ U>> &vt)\n{\n  const size_t n = vt.size();\n  pair<vc<T>, vc<U>> tv;\n  tv.first.resize(n),\
    \ tv.second.resize(n);\n  for (size_t i = 0; i < n; i++)\n    tie(tv.first[i],\
    \ tv.second[i]) = vt[i];\n  return tv;\n}\ntemplate <class T, class U>\nvc<pair<T,\
    \ U>> top(const pair<vc<T>, vc<U>> &tv)\n{\n  const size_t n = tv.first.size();\n\
    \  assert(n == tv.second.size());\n  vc<pair<T, U>> vt(n);\n  for (size_t i =\
    \ 0; i < n; i++)\n    vt[i] = make_pair(tv.first[i], tv.second[i]);\n  return\
    \ vt;\n}\n\ntemplate <size_t... I>\nauto vt_to_tv_impl(auto &tv, cauto &t, index_sequence<I...>,\
    \ size_t index)\n{ ((get<I>(tv)[index] = get<I>(t)), ...); }\ntemplate <class...\
    \ Ts>\nauto top(const vc<tuple<Ts...>> &vt)\n{\n  const size_t n = vt.size();\n\
    \  tuple<vc<Ts>...> tv;\n  apply([&](auto &...v)\n        { ((v.resize(n)), ...);\
    \ }, tv);\n  for (size_t i = 0; i < n; i++)\n    vt_to_tv_impl(tv, vt[i], make_index_sequence<tuple_size_v<decltype(tv)>>{},\
    \ i);\n  return tv;\n}\n\ntemplate <size_t... I>\nauto tv_to_vt_impl(cauto &tv,\
    \ index_sequence<I...>, size_t index)\n{ return make_tuple(get<I>(tv)[index]...);\
    \ }\ntemplate <class... Ts>\nauto top(const tuple<vc<Ts>...> &tv)\n{\n  size_t\
    \ n = get<0>(tv).size();\n  apply([&](auto &...v)\n        { ((assert(v.size()\
    \ == n)), ...); }, tv);\n  vc<tuple<Ts...>> vt(n);\n  for (size_t i = 0; i < n;\
    \ i++)\n    vt[i] = tv_to_vt_impl(tv, index_sequence_for<Ts...>{}, i);\n  return\
    \ vt;\n}\n// ----------\n#line 2 \"template/template_dump.hpp\"\n\n#line 4 \"\
    template/template_dump.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\
    \uFF08dump\uFF09\n * @docs docs/template/template_dump.md\n */\n\n#ifdef LOCAL\n\
    #include <cpp-dump.hpp> // https://github.com/philip82148/cpp-dump\nnamespace\
    \ cpp_dump::_detail\n{\n  inline string export_var(\n      const i128 &x, const\
    \ string &indent, size_t last_line_length,\n      size_t current_depth, bool fail_on_newline,\
    \ const export_command &command\n  ) {\n    return export_var(i128tos(x), indent,\
    \ last_line_length, current_depth, fail_on_newline, command);\n  }\n} // namespace\
    \ cpp_dump::_detail\n#define dump(...) cpp_dump(__VA_ARGS__)\nnamespace cp = cpp_dump;\n\
    CPP_DUMP_SET_OPTION_GLOBAL(log_label_func, cp::log_label::line());\nCPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count,\
    \ 10000);\n#define local(...) __VA_ARGS__\n#else\n#define dump(...)\n#define local(...)\n\
    #endif\n#line 5 \"verify/mytest/template_inout_top.test.cpp\"\n\nvoid test1()\n\
    {\n  vc<pair<int, string>> vt_god = {\n    {1, \"a\"},\n    {2, \"bcd\"},\n  \
    \  {3, \"ef\"}\n  };\n  pair<vc<int>, vc<string>> tv_god = {\n    {1, 2, 3},\n\
    \    {\"a\", \"bcd\", \"ef\"}\n  };\n\n  auto tv_ans = top(vt_god);\n  auto vt_ans\
    \ = top(tv_god);\n  dump(tv_ans, vt_ans);\n  assert(tv_ans == tv_god);\n  assert(vt_ans\
    \ == vt_god);\n}\n\nvoid test2()\n{\n  vc<array<int, 5>> vt_god = {\n    {1, 2,\
    \ 3, 4, 5},\n    {6, 7, 8, 9, 10},\n    {11, 12, 13, 14, 15}\n  };\n  array<vc<int>,\
    \ 5> tv_god = {{\n    {1, 6, 11},\n    {2, 7, 12},\n    {3, 8, 13},\n    {4, 9,\
    \ 14},\n    {5, 10, 15}\n  }};\n\n  auto tv_ans = top(vt_god);\n  auto vt_ans\
    \ = top(tv_god);\n  dump(tv_ans, vt_ans);\n  assert(tv_ans == tv_god);\n  assert(vt_ans\
    \ == vt_god);\n}\n\nvoid test3()\n{\n  vc<tuple<int, int, int, int, string>> vt_god\
    \ = {\n    {1, 2, 3, 4, \"a\"},\n    {5, 6, 7, 8, \"bcd\"},\n    {9, 10, 11, 12,\
    \ \"ef\"}\n  };\n  tuple<vc<int>, vc<int>, vc<int>, vc<int>, vc<string>> tv_god\
    \ = {\n    {1, 5, 9},\n    {2, 6, 10},\n    {3, 7, 11},\n    {4, 8, 12},\n   \
    \ {\"a\", \"bcd\", \"ef\"}\n  };\n\n  auto tv_ans = top(vt_god);\n  auto vt_ans\
    \ = top(tv_god);\n  dump(tv_ans, vt_ans);\n  assert(tv_ans == tv_god);\n  assert(vt_ans\
    \ == vt_god);\n}\n\nint main()\n{\n  test1();\n  test2();\n  test3();\n\n  PRINT(\"\
    Hello World\");\n}\n"
  code: "#define PROBLEM \"https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A\"\
    \n\n#include \"../../template/template_inout.hpp\"\n#include \"../../template/template_dump.hpp\"\
    \n\nvoid test1()\n{\n  vc<pair<int, string>> vt_god = {\n    {1, \"a\"},\n   \
    \ {2, \"bcd\"},\n    {3, \"ef\"}\n  };\n  pair<vc<int>, vc<string>> tv_god = {\n\
    \    {1, 2, 3},\n    {\"a\", \"bcd\", \"ef\"}\n  };\n\n  auto tv_ans = top(vt_god);\n\
    \  auto vt_ans = top(tv_god);\n  dump(tv_ans, vt_ans);\n  assert(tv_ans == tv_god);\n\
    \  assert(vt_ans == vt_god);\n}\n\nvoid test2()\n{\n  vc<array<int, 5>> vt_god\
    \ = {\n    {1, 2, 3, 4, 5},\n    {6, 7, 8, 9, 10},\n    {11, 12, 13, 14, 15}\n\
    \  };\n  array<vc<int>, 5> tv_god = {{\n    {1, 6, 11},\n    {2, 7, 12},\n   \
    \ {3, 8, 13},\n    {4, 9, 14},\n    {5, 10, 15}\n  }};\n\n  auto tv_ans = top(vt_god);\n\
    \  auto vt_ans = top(tv_god);\n  dump(tv_ans, vt_ans);\n  assert(tv_ans == tv_god);\n\
    \  assert(vt_ans == vt_god);\n}\n\nvoid test3()\n{\n  vc<tuple<int, int, int,\
    \ int, string>> vt_god = {\n    {1, 2, 3, 4, \"a\"},\n    {5, 6, 7, 8, \"bcd\"\
    },\n    {9, 10, 11, 12, \"ef\"}\n  };\n  tuple<vc<int>, vc<int>, vc<int>, vc<int>,\
    \ vc<string>> tv_god = {\n    {1, 5, 9},\n    {2, 6, 10},\n    {3, 7, 11},\n \
    \   {4, 8, 12},\n    {\"a\", \"bcd\", \"ef\"}\n  };\n\n  auto tv_ans = top(vt_god);\n\
    \  auto vt_ans = top(tv_god);\n  dump(tv_ans, vt_ans);\n  assert(tv_ans == tv_god);\n\
    \  assert(vt_ans == vt_god);\n}\n\nint main()\n{\n  test1();\n  test2();\n  test3();\n\
    \n  PRINT(\"Hello World\");\n}"
  dependsOn:
  - template/template_inout.hpp
  - template/template_types.hpp
  - template/template_rep.hpp
  - template/template_dump.hpp
  isVerificationFile: true
  path: verify/mytest/template_inout_top.test.cpp
  requiredBy: []
  timestamp: '2024-12-10 18:45:11+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/mytest/template_inout_top.test.cpp
layout: document
redirect_from:
- /verify/verify/mytest/template_inout_top.test.cpp
- /verify/verify/mytest/template_inout_top.test.cpp.html
title: verify/mytest/template_inout_top.test.cpp
---
