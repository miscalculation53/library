---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: template/template_all.hpp
    title: template/template_all.hpp
  - icon: ':heavy_check_mark:'
    path: template/template_dump.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_inout.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\uFF09"
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
    PROBLEM: https://judge.yosupo.jp/problem/kth_root_integer
    links:
    - https://judge.yosupo.jp/problem/kth_root_integer
  bundledCode: "#line 1 \"verify/yosupo/kth_root_integer.test.cpp\"\n#define PROBLEM\
    \ \"https://judge.yosupo.jp/problem/kth_root_integer\"\n\n// #define SINGLE_TESTCASE\n\
    #define MULTI_TESTCASE\n// #define AOJ_TESTCASE\n\n#define FAST_IO\n\n#define\
    \ INF 4'000'000'000'000'000'037\n#define EPS 1e-11\n\n#line 2 \"template/template_all.hpp\"\
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
    \ repi3, repi2, repi1)(__VA_ARGS__)\n#line 2 \"template/template_inout.hpp\"\n\
    \n#line 5 \"template/template_inout.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\
    \u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\uFF09\n * @docs docs/template/template_inout.md\n\
    \ */\n/**\n * \u53C2\u8003\uFF1A\n * https://trap.jp/post/1224/\n */\n\n// ----\
    \ \u5165\u529B ----\ntemplate <class T, class U>\nistream &operator>>(istream\
    \ &is, pair<T, U> &p)\n{\n  cin >> p.first >> p.second;\n  return is;\n}\ntemplate\
    \ <class T, size_t n>\nistream &operator>>(istream &is, array<T, n> &a)\n{\n \
    \ for (size_t i = 0; i < n; i++)\n    cin >> a[i];\n  return is;\n}\ntemplate\
    \ <class... Ts>\nistream &operator>>(istream &is, tuple<Ts...> &t)\n{\n  apply([&](auto\
    \ &...a)\n        { (is >> ... >> a); }, t);\n  return is;\n}\n\ntemplate <class...\
    \ Ts>\nvoid INPUT(Ts &...a) { (cin >> ... >> a); }\n\ntemplate <class T>\nvoid\
    \ INPUTVEC(int n, vc<T> &v)\n{\n  v.resize(n);\n  repi(i, n) cin >> v[i];\n}\n\
    template <class T, class... Ts>\nvoid INPUTVEC(int n, vc<T> &v, vc<Ts> &...vs)\n\
    { INPUTVEC(n, v), INPUTVEC(n, vs...); }\n\ntemplate <class T>\nvoid INPUTVEC2(int\
    \ n, int m, vvc<T> &v)\n{\n  v.assign(n, vc<T>(m));\n  repi(i, n) repi(j, m) cin\
    \ >> v[i][j];\n}\ntemplate <class T, class... Ts>\nvoid INPUTVEC2(int n, int m,\
    \ vvc<T> &v, vvc<Ts> &...vs)\n{ INPUTVEC2(n, m, v), INPUTVEC2(n, m, vs...); }\n\
    \n#define INT(...) int __VA_ARGS__; INPUT(__VA_ARGS__)\n#define LL(...) ll __VA_ARGS__;\
    \ INPUT(__VA_ARGS__)\n#define STR(...) string __VA_ARGS__; INPUT(__VA_ARGS__)\n\
    #define ARR(T, n, ...) array<T, n> __VA_ARGS__; INPUT(__VA_ARGS__)\n#define VEC(T,\
    \ n, ...) vc<T> __VA_ARGS__; INPUTVEC(n, __VA_ARGS__)\n#define VEC2(T, n, m, ...)\
    \ vvc<T> __VA_ARGS__; INPUTVEC2(n, m, __VA_ARGS__)\n// ----------\n\n// -----\
    \ \u51FA\u529B -----\n#ifdef FAST_IO\n  #define ENDL '\\n'\n#else\n  #define ENDL\
    \ endl\n#endif\n\ntemplate <class T>\nvoid PRINT(const T &a) { cout << a << ENDL;\
    \ }\ntemplate <class T, class... Ts>\nvoid PRINT(const T &a, const Ts &...b)\n\
    {\n  cout << a;\n  (cout << ... << (cout << ' ', b));\n  cout << ENDL;\n}\n#define\
    \ PRINTEXIT(...) do { PRINT(__VA_ARGS__); exit(0); } while (false)\n#define PRINTRETURN(...)\
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
    \ vt;\n}\n// ----------\n#line 2 \"template/template_math.hpp\"\n\n#ifndef INF\n\
    #define INF 4'000'000'000'000'000'037LL\n#endif\n#ifndef EPS\n#define EPS 1e-11\n\
    #endif\n\n#line 12 \"template/template_math.hpp\"\n\n/**\n * @brief \u30C6\u30F3\
    \u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09\n * @docs docs/template/template_math.md\n\
    \ */\n\ninline bool chmin(auto &a, cauto &b) { return a > b ? a = b, true : false;\
    \ }\ninline bool chmax(auto &a, cauto &b) { return a < b ? a = b, true : false;\
    \ }\n\ntemplate <class T = ll>\ninline T divfloor(cauto &a, cauto &b) { return\
    \ T(a) / T(b) - (T(a) % T(b) && (T(a) ^ T(b)) < 0); }\ntemplate <class T = ll>\n\
    inline T divceil(cauto &a, cauto &b) { return T(a) / T(b) + (T(a) % T(b) && (T(a)\
    \ ^ T(b)) >= 0); }\ntemplate <class T = ll>\ninline T safemod(cauto &a, cauto\
    \ &b) { return a - b * divfloor(a, b); }\n\ntemplate <class T = ll>\nconstexpr\
    \ T ipow(cauto &a, cauto &b)\n{\n  assert(b >= 0);\n  if (b == 0) return 1;\n\
    \  if (a == 0 || a == 1) return a;\n  if (a == -1) return b & 1 ? -1 : 1;\n\n\
    \  T res = 1;\n  repi(_, b) res *= T(a);\n  return res;\n}\ntemplate <class T\
    \ = ll>\nT mul_limited(cauto &a, cauto &b, cauto &m = INF)\n{\n  assert(a >= 0\
    \ && b >= 0 && m >= 0);\n  if (b == 0)\n    return 0;\n  return T(a) > T(m) /\
    \ T(b) ? T(m) : T(a) * T(b);\n}\ntemplate <class T = ll>\nT pow_limited(cauto\
    \ &a, cauto &b, cauto &m = INF)\n{\n  assert(a >= 0 && b >= 0 && m >= 0);\n  if\
    \ (a <= 1 || b == 0)\n    return min(ipow<T>(a, b), T(m));\n  \n  T res = 1;\n\
    \  repi(_, b)\n  {\n    if (res > T(m) / T(a))\n      return T(m);\n    res *=\
    \ T(a);\n  }\n  return res;\n}\n\ntemplate <class T = ll>\nconstexpr T iroot(cauto\
    \ &a, cauto &k)\n{\n  assert(a >= 0 && k >= 1);\n  if (a <= 1 || k == 1)\n   \
    \ return a;\n\n  auto isok = [&](const T &x) -> bool\n  {\n    if (x == 0)\n \
    \     return true;\n    T tmp = 1;\n    repi(_, k)\n    {\n      if (tmp > T(a)\
    \ / x)\n        return false;\n      tmp *= x;\n    }\n    return tmp <= T(a);\n\
    \  };\n\n  T ok = 0, ng = 1;\n  while (isok(ng))\n    ok = ng, ng <<= 1;\n  while\
    \ (ng - ok > 1)\n  {\n    T mid = ((ng - ok) >> 1) + ok;\n    if (isok(mid))\n\
    \      ok = mid;\n    else\n      ng = mid;\n  }\n  return ok;\n}\n\n// https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html\n\
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
    #endif\n#line 13 \"verify/yosupo/kth_root_integer.test.cpp\"\n\nvoid init() {}\n\
    \nvoid main2()\n{\n  ull A, K;\n  cin >> A >> K;\n  PRINT(iroot<ull>(A, K));\n\
    }\n\nvoid test()\n{\n  /*\n  #ifdef LOCAL\n  rep(t, 100000)\n  {\n    dump(t);\n\
    \n    // ----- generate cases -----\n    ll N = 1 + rand() % 5;\n    vl A(N);\n\
    \    rep(i, N) A.at(i) = 1 + rand() % 10;\n    // --------------------------\n\
    \n    // ------ check output ------\n    auto god = naive(A);\n    auto ans =\
    \ solve(A);\n    if (god != ans)\n    {\n      dump(N, A);\n      dump(god, ans);\n\
    \      exit(0);\n    }\n    // --------------------------\n  }\n  dump(\"ok\"\
    );\n  #endif\n  //*/\n}\n\nint main()\n{\n  #if defined FAST_IO and not defined\
    \ LOCAL\n    cerr << \"[FAST_IO]\\n\\n\";\n    cin.tie(0);\n    ios::sync_with_stdio(false);\n\
    \  #endif\n  cout << fixed << setprecision(20);\n\n  init();\n  test();\n\n  #if\
    \ defined AOJ_TESTCASE or (defined LOCAL and defined SINGLE_TESTCASE)\n    cerr\
    \ << \"[AOJ_TESTCASE]\\n\\n\";\n    while (true) main2();\n  #elif defined SINGLE_TESTCASE\n\
    \    cerr << \"[SINGLE_TESTCASE]\\n\\n\";\n    main2();\n  #elif defined MULTI_TESTCASE\n\
    \    cerr << \"[MULTI_TESTCASE]\\n\\n\";\n    int T;\n    cin >> T;\n    while\
    \ (T--) main2();\n  #endif\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/kth_root_integer\"\n\n\
    // #define SINGLE_TESTCASE\n#define MULTI_TESTCASE\n// #define AOJ_TESTCASE\n\n\
    #define FAST_IO\n\n#define INF 4'000'000'000'000'000'037\n#define EPS 1e-11\n\n\
    #include \"../../template/template_all.hpp\"\n\nvoid init() {}\n\nvoid main2()\n\
    {\n  ull A, K;\n  cin >> A >> K;\n  PRINT(iroot<ull>(A, K));\n}\n\nvoid test()\n\
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
  - template/template_rep.hpp
  - template/template_inout.hpp
  - template/template_math.hpp
  - template/template_dump.hpp
  isVerificationFile: true
  path: verify/yosupo/kth_root_integer.test.cpp
  requiredBy: []
  timestamp: '2024-08-31 12:06:34+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/yosupo/kth_root_integer.test.cpp
layout: document
redirect_from:
- /verify/verify/yosupo/kth_root_integer.test.cpp
- /verify/verify/yosupo/kth_root_integer.test.cpp.html
title: verify/yosupo/kth_root_integer.test.cpp
---