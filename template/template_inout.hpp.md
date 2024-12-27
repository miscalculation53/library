---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: template/template_rep.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_types.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09"
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: ds/coordinate_compression.hpp
    title: "\u5EA7\u6A19\u5727\u7E2E"
  - icon: ':heavy_check_mark:'
    path: math/extgcd.hpp
    title: "\u62E1\u5F35\u30E6\u30FC\u30AF\u30EA\u30C3\u30C9\u4E92\u9664\u6CD5 (extgcd)"
  - icon: ':heavy_check_mark:'
    path: math/modint/modint.hpp
    title: modint (32 bit)
  - icon: ':heavy_check_mark:'
    path: math/modint/modint64.hpp
    title: modint (64 bit)
  - icon: ':heavy_check_mark:'
    path: math/modint/modint_internal.hpp
    title: math/modint/modint_internal.hpp
  - icon: ':heavy_check_mark:'
    path: math/prime/factorize.hpp
    title: "\u7D20\u56E0\u6570\u5206\u89E3"
  - icon: ':heavy_check_mark:'
    path: math/prime/primality_test.hpp
    title: "\u7D20\u6570\u5224\u5B9A"
  - icon: ':heavy_check_mark:'
    path: math/prime/prime_power.hpp
    title: "\u7D20\u3079\u304D\u69CB\u9020\u4F53"
  - icon: ':heavy_check_mark:'
    path: math/quotients.hpp
    title: "\u5546\u5217\u6319"
  - icon: ':warning:'
    path: template/template.cpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5168\u4F53\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_all.hpp
    title: template/template_all.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/aoj/extgcd.test.cpp
    title: verify/aoj/extgcd.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/modint.test.cpp
    title: verify/mytest/modint.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/modint64.test.cpp
    title: verify/mytest/modint64.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_inout_top.test.cpp
    title: verify/mytest/template_inout_top.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/aplusb.test.cpp
    title: verify/yosupo/aplusb.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/associative_array_lower_bound.test.cpp
    title: verify/yosupo/associative_array_lower_bound.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/factorize.test.cpp
    title: verify/yosupo/factorize.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/kth_root_integer.test.cpp
    title: verify/yosupo/kth_root_integer.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/many_aplusb.test.cpp
    title: verify/yosupo/many_aplusb.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/many_aplusb_128bit.test.cpp
    title: verify/yosupo/many_aplusb_128bit.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/many_aplusb_tuple.test.cpp
    title: verify/yosupo/many_aplusb_tuple.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/predecessor_problem.test.cpp
    title: verify/yosupo/predecessor_problem.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/primality_test.test.cpp
    title: verify/yosupo/primality_test.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/quotients.test.cpp
    title: verify/yosupo/quotients.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yukicoder/yuki1550_dynamic_modint.test.cpp
    title: verify/yukicoder/yuki1550_dynamic_modint.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yukicoder/yuki1550_static_modint.test.cpp
    title: verify/yukicoder/yuki1550_static_modint.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yukicoder/yuki888.test.cpp
    title: verify/yukicoder/yuki888.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    _deprecated_at_docs: docs/template/template_inout.md
    document_title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\uFF09"
    links:
    - https://trap.jp/post/1224/
  bundledCode: "#line 2 \"template/template_inout.hpp\"\n\n#line 2 \"template/template_types.hpp\"\
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
    #define fem(...) for (auto &__VA_ARGS__)\n#line 5 \"template/template_inout.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\
    \uFF09\n * @docs docs/template/template_inout.md\n */\n/**\n * \u53C2\u8003\uFF1A\
    \n * https://trap.jp/post/1224/\n */\n\n// ---- \u5165\u529B ----\ntemplate <class\
    \ T, class U>\nistream &operator>>(istream &is, pair<T, U> &p)\n{\n  cin >> p.first\
    \ >> p.second;\n  return is;\n}\ntemplate <class T, size_t n>\nistream &operator>>(istream\
    \ &is, array<T, n> &a)\n{\n  for (size_t i = 0; i < n; i++)\n    cin >> a[i];\n\
    \  return is;\n}\ntemplate <class... Ts>\nistream &operator>>(istream &is, tuple<Ts...>\
    \ &t)\n{\n  apply([&](auto &...a)\n        { (is >> ... >> a); }, t);\n  return\
    \ is;\n}\n\ntemplate <class... Ts>\nvoid CIN(Ts &...a) { (cin >> ... >> a); }\n\
    \ntemplate <class T>\nvoid CINVEC(int n, vc<T> &v)\n{\n  v.resize(n);\n  repi(i,\
    \ n) cin >> v[i];\n}\ntemplate <class T, class... Ts>\nvoid CINVEC(int n, vc<T>\
    \ &v, vc<Ts> &...vs)\n{ CINVEC(n, v), CINVEC(n, vs...); }\n\ntemplate <class T>\n\
    void CINVEC2(int n, int m, vvc<T> &v)\n{\n  v.assign(n, vc<T>(m));\n  repi(i,\
    \ n) repi(j, m) cin >> v[i][j];\n}\ntemplate <class T, class... Ts>\nvoid CINVEC2(int\
    \ n, int m, vvc<T> &v, vvc<Ts> &...vs)\n{ CINVEC2(n, m, v), CINVEC2(n, m, vs...);\
    \ }\n\n#define IN(T, ...) T __VA_ARGS__; CIN(__VA_ARGS__)\n\n#define CHAR(...)\
    \ IN(char, __VA_ARGS__)\n#define INT(...) IN(int, __VA_ARGS__)\n#define LL(...)\
    \ IN(ll, __VA_ARGS__)\n#define STR(...) IN(string, __VA_ARGS__)\n#define ARR(T,\
    \ n, ...) array<T, n> __VA_ARGS__; CIN(__VA_ARGS__)\n\n#define VEC(T, n, ...)\
    \ vc<T> __VA_ARGS__; CINVEC(n, __VA_ARGS__)\n#define VEC2(T, n, m, ...) vvc<T>\
    \ __VA_ARGS__; CINVEC2(n, m, __VA_ARGS__)\n// ----------\n\n// ----- \u51FA\u529B\
    \ -----\n#ifdef INTERACTIVE\n#define ENDL endl\n#else\n#define ENDL '\\n'\n#endif\n\
    \ntemplate <class T>\nvoid PRINT(const T &a) { cout << a << ENDL; }\ntemplate\
    \ <class T, class... Ts>\nvoid PRINT(const T &a, const Ts &...b)\n{\n  cout <<\
    \ a;\n  (cout << ... << (cout << ' ', b));\n  cout << ENDL;\n}\n#define PRINTEXIT(...)\
    \ do { PRINT(__VA_ARGS__); exit(0); } while (false)\n#define PRINTRETURN(...)\
    \ do { PRINT(__VA_ARGS__); return; } while (false)\n\ntemplate <class T>\nvoid\
    \ PRINTVEC(const vc<T> &v)\n{\n  const int n = v.size();\n  repi(i, n) cout <<\
    \ v[i] << (i == n - 1 ? \"\" : \" \");\n  cout << ENDL;\n}\ntemplate <class T>\n\
    void PRINTVECT(const vc<T> &v) { for (auto &vi : v) cout << vi << ENDL; }\ntemplate\
    \ <class T>\nvoid PRINTVEC2(const vvc<T> &v) { for (auto &vi : v) PRINTVEC(vi);\
    \ }\n// ----------\n\n// ----- \u57FA\u6E96\u305A\u3089\u3057 -----\ntemplate\
    \ <class T, class U>\npair<T, U> operator+=(pair<T, U> &a, cauto &b)\n{\n  a.first\
    \ += b.first;\n  a.second += b.second;\n  return a;\n}\ntemplate <class T, class\
    \ U>\npair<T, U> operator+(pair<T, U> &a, cauto &b) { return a += b; }\n\ntemplate\
    \ <class T, size_t n>\narray<T, n> operator+=(array<T, n> &a, cauto &b)\n{\n \
    \ for (size_t i = 0; i < n; i++)\n    a[i] += b[i];\n  return a;\n}\ntemplate\
    \ <class T, size_t n>\narray<T, n> operator+(array<T, n> &a, cauto &b) { return\
    \ a += b; }\n\ntemplate <size_t... I>\nauto tuple_add_impl(auto &a, cauto &b,\
    \ const index_sequence<I...>)\n{\n  ((get<I>(a) += get<I>(b)), ...);\n  return\
    \ a;\n}\ntemplate <class... Ts>\ntuple<Ts...> operator+=(tuple<Ts...> &a, cauto\
    \ &b)\n{ return tuple_add_impl(a, b, make_index_sequence<tuple_size_v<tuple<Ts...>>>{});\
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
    \ vt;\n}\n// ----------\n"
  code: "#pragma once\n\n#include \"template_types.hpp\"\n#include \"template_rep.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\
    \uFF09\n * @docs docs/template/template_inout.md\n */\n/**\n * \u53C2\u8003\uFF1A\
    \n * https://trap.jp/post/1224/\n */\n\n// ---- \u5165\u529B ----\ntemplate <class\
    \ T, class U>\nistream &operator>>(istream &is, pair<T, U> &p)\n{\n  cin >> p.first\
    \ >> p.second;\n  return is;\n}\ntemplate <class T, size_t n>\nistream &operator>>(istream\
    \ &is, array<T, n> &a)\n{\n  for (size_t i = 0; i < n; i++)\n    cin >> a[i];\n\
    \  return is;\n}\ntemplate <class... Ts>\nistream &operator>>(istream &is, tuple<Ts...>\
    \ &t)\n{\n  apply([&](auto &...a)\n        { (is >> ... >> a); }, t);\n  return\
    \ is;\n}\n\ntemplate <class... Ts>\nvoid CIN(Ts &...a) { (cin >> ... >> a); }\n\
    \ntemplate <class T>\nvoid CINVEC(int n, vc<T> &v)\n{\n  v.resize(n);\n  repi(i,\
    \ n) cin >> v[i];\n}\ntemplate <class T, class... Ts>\nvoid CINVEC(int n, vc<T>\
    \ &v, vc<Ts> &...vs)\n{ CINVEC(n, v), CINVEC(n, vs...); }\n\ntemplate <class T>\n\
    void CINVEC2(int n, int m, vvc<T> &v)\n{\n  v.assign(n, vc<T>(m));\n  repi(i,\
    \ n) repi(j, m) cin >> v[i][j];\n}\ntemplate <class T, class... Ts>\nvoid CINVEC2(int\
    \ n, int m, vvc<T> &v, vvc<Ts> &...vs)\n{ CINVEC2(n, m, v), CINVEC2(n, m, vs...);\
    \ }\n\n#define IN(T, ...) T __VA_ARGS__; CIN(__VA_ARGS__)\n\n#define CHAR(...)\
    \ IN(char, __VA_ARGS__)\n#define INT(...) IN(int, __VA_ARGS__)\n#define LL(...)\
    \ IN(ll, __VA_ARGS__)\n#define STR(...) IN(string, __VA_ARGS__)\n#define ARR(T,\
    \ n, ...) array<T, n> __VA_ARGS__; CIN(__VA_ARGS__)\n\n#define VEC(T, n, ...)\
    \ vc<T> __VA_ARGS__; CINVEC(n, __VA_ARGS__)\n#define VEC2(T, n, m, ...) vvc<T>\
    \ __VA_ARGS__; CINVEC2(n, m, __VA_ARGS__)\n// ----------\n\n// ----- \u51FA\u529B\
    \ -----\n#ifdef INTERACTIVE\n#define ENDL endl\n#else\n#define ENDL '\\n'\n#endif\n\
    \ntemplate <class T>\nvoid PRINT(const T &a) { cout << a << ENDL; }\ntemplate\
    \ <class T, class... Ts>\nvoid PRINT(const T &a, const Ts &...b)\n{\n  cout <<\
    \ a;\n  (cout << ... << (cout << ' ', b));\n  cout << ENDL;\n}\n#define PRINTEXIT(...)\
    \ do { PRINT(__VA_ARGS__); exit(0); } while (false)\n#define PRINTRETURN(...)\
    \ do { PRINT(__VA_ARGS__); return; } while (false)\n\ntemplate <class T>\nvoid\
    \ PRINTVEC(const vc<T> &v)\n{\n  const int n = v.size();\n  repi(i, n) cout <<\
    \ v[i] << (i == n - 1 ? \"\" : \" \");\n  cout << ENDL;\n}\ntemplate <class T>\n\
    void PRINTVECT(const vc<T> &v) { for (auto &vi : v) cout << vi << ENDL; }\ntemplate\
    \ <class T>\nvoid PRINTVEC2(const vvc<T> &v) { for (auto &vi : v) PRINTVEC(vi);\
    \ }\n// ----------\n\n// ----- \u57FA\u6E96\u305A\u3089\u3057 -----\ntemplate\
    \ <class T, class U>\npair<T, U> operator+=(pair<T, U> &a, cauto &b)\n{\n  a.first\
    \ += b.first;\n  a.second += b.second;\n  return a;\n}\ntemplate <class T, class\
    \ U>\npair<T, U> operator+(pair<T, U> &a, cauto &b) { return a += b; }\n\ntemplate\
    \ <class T, size_t n>\narray<T, n> operator+=(array<T, n> &a, cauto &b)\n{\n \
    \ for (size_t i = 0; i < n; i++)\n    a[i] += b[i];\n  return a;\n}\ntemplate\
    \ <class T, size_t n>\narray<T, n> operator+(array<T, n> &a, cauto &b) { return\
    \ a += b; }\n\ntemplate <size_t... I>\nauto tuple_add_impl(auto &a, cauto &b,\
    \ const index_sequence<I...>)\n{\n  ((get<I>(a) += get<I>(b)), ...);\n  return\
    \ a;\n}\ntemplate <class... Ts>\ntuple<Ts...> operator+=(tuple<Ts...> &a, cauto\
    \ &b)\n{ return tuple_add_impl(a, b, make_index_sequence<tuple_size_v<tuple<Ts...>>>{});\
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
    \ vt;\n}\n// ----------"
  dependsOn:
  - template/template_types.hpp
  - template/template_rep.hpp
  isVerificationFile: false
  path: template/template_inout.hpp
  requiredBy:
  - math/modint/modint_internal.hpp
  - math/modint/modint.hpp
  - math/modint/modint64.hpp
  - math/prime/factorize.hpp
  - math/prime/primality_test.hpp
  - math/prime/prime_power.hpp
  - math/quotients.hpp
  - math/extgcd.hpp
  - ds/coordinate_compression.hpp
  - template/template.cpp
  - template/template_all.hpp
  timestamp: '2024-12-20 09:47:18+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo/many_aplusb_128bit.test.cpp
  - verify/yosupo/predecessor_problem.test.cpp
  - verify/yosupo/kth_root_integer.test.cpp
  - verify/yosupo/factorize.test.cpp
  - verify/yosupo/quotients.test.cpp
  - verify/yosupo/many_aplusb_tuple.test.cpp
  - verify/yosupo/associative_array_lower_bound.test.cpp
  - verify/yosupo/primality_test.test.cpp
  - verify/yosupo/aplusb.test.cpp
  - verify/yosupo/many_aplusb.test.cpp
  - verify/mytest/modint.test.cpp
  - verify/mytest/template_inout_top.test.cpp
  - verify/mytest/modint64.test.cpp
  - verify/aoj/extgcd.test.cpp
  - verify/yukicoder/yuki1550_static_modint.test.cpp
  - verify/yukicoder/yuki1550_dynamic_modint.test.cpp
  - verify/yukicoder/yuki888.test.cpp
documentation_of: template/template_inout.hpp
layout: document
redirect_from:
- /library/template/template_inout.hpp
- /library/template/template_inout.hpp.html
title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\uFF09"
---
## テンプレート（入出力）

### 入力

#### `pair`, `tuple`, `array` への入力

`cin >>` でできる。

#### 変数を宣言と同時に入力するマクロ

- `IN(T, ...)`：`T` 型の値（引数は複数）

  - `CHAR(...)`：`char` 型の値（引数は複数）

  - `INT(...)`：`int` 型の値（引数は複数）

  - `LL(...)`：`ll` 型の値（引数は複数）

  - `STR(...)`：`string` 型の値（引数は複数）

  - `ARR(T, n, ...)`：`array<T, n>` 型の値（第 $3$ 引数以降は複数）

- `VEC(T, n, ...)`：`vector<T>` 型の値（サイズは $n$）（第 $3$ 引数以降は複数）

- `VEC2(T, n, m, ...)`：`vector<T>` 型の値（サイズは $n \times m$）（第 $4$ 引数以降は複数）

一応、すでにある変数に入力するマクロ（`CIN` から始まる諸々）も副産物としてあるが、あまり使わないと思う。

マクロに型を入れる場合、カンマが入っている型（`pair<int, int>` など）には注意（`pll` 的なものを `using` する）。

### 出力

- `PRINT(...)`：出力（引数は複数、空白区切り、最後に改行）

- `PRINTVEC(vc<T> v)`：`v` を空白区切りで出力し、最後に改行

- `PRINTVECT(vc<T> v)`：`v` を改行区切りで出力
  - `vstr` にも使える

- `PRINTVEC2(vvc<T> v)`：$2$ 次元配列 `v` をいい感じに出力

- `PRINTEXIT`：`PRINT` して `exit(0)` する
  - あまり使わない（そもそも単一テストケースでないと使えない）、再帰関数の中身で使うくらいか？

- `PRINTRETURN`：`PRINT` して `return` する
  - `main` の外に `main2` を作っておけば、複数テストケースの場合も `PRINTRETURN` でよい

改行の際の flush は、`INTERACTIVE` が define されていれば flush しない、define されていなければ flush する。

このあたりの思想はテンプレート全体（template.cpp）も見るとよいかも。

### その他

#### 0-indexed に直す

`offset(vc<T> v, T add)` や `offset(vvc<T> v, T add)` で `v` の各要素に `add` が足される。

`T` は tuple-like な型でも可。

副産物として、tuple-like な型どうしが `+` で足し算できる。

#### 転置

`vc<tuple-like<T1, T2, ...>>` と `tuple-like<vc<T1>, vc<T2>, ...>` を相互変換するには `top()` を用いる。名前の由来は $\LaTeX$ の転置記号 $\top$ を出力するコマンド `\top`。

思想としては

```
A_1 A_2 ... A_N
B_1 B_2 ... B_N
```

と

```
A_1 B_1
A_2 B_2
:
A_N B_N
```

をうまく扱いたいというところからきている（前者が vector 2 つ、後者が pair の vector で、これを相互変換したい）。

---

### 使用例（入力）

#### 例 1：順列

```
5
1 3 4 2 5
```

```cpp
LL(N);
VEC(ll, N, P);
offset(P, -1);
// これで P = {0, 2, 3, 1, 4} となる
```

#### 例 2：vector 2 つ
  
```
N
A_1 ... A_N
B_1 ... B_N
```
```cpp
LL(N);
VEC(ll, N, A, B);
```

`A` と `B` をいっぺんに受け取れる。

#### 例 3：サイズの異なる vector 2 つ

```
N M
A_1 ... A_N
B_1 ... B_M
```

```cpp
LL(N, M);
VEC(ll, N, A);
VEC(ll, M, B);
```

サイズが異なるときはいっぺんに受け取れないので注意。

#### 例 4：pair の vector
  
```
N
A_1 B_1
:
A_N B_N
```

```cpp
LL(N);
VEC(pll, N, AB);
auto [A, B] = top(AB);
```
  
これで `AB` は `vc<pll>` に、`A`, `B` はそれぞれ `vc<ll>` になる。
  
ソートしてから `top` を噛ませるような使い方もできる。vector 2 つで与えられたものを、pair の vector にして、ソートして、vector 2 つに戻す、というようなこともできる。
  
### 例 5：グラフ
```
3
1 2 100
2 3 200
```

```cpp
LL(N);
VEC(tlll, N - 1, ABC);
offset(N, tlll{-1, -1, 0});
// これで ABC = { {0, 1, 100}, {1, 2, 200} } となる
```

一旦 `vc<pll>` なり `vc<tlll>` なりで受け取って、その配列をグラフライブラリに渡す、という設計を考えている。

####  例 6：二次元配列
  
```
N M
A[1][1] ... A[1][M]
:
A[N][1] ... A[N][M]
B[1][1] ... B[1][M]
:
B[N][1] ... B[N][M]
```

```cpp
LL(N, M);
VEC2(ll, N, M, A, B);
```

#### 例 7：二次元の文字

```
3 4
...#
..#.
#...
```

```cpp
LL(N, M);
VEC(string, N, S);
```
