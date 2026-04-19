---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    _deprecated_at_docs: docs/template/template_inout.md
    document_title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\uFF09"
    links:
    - https://judge.yosupo.jp/submission/170706
    - https://judge.yosupo.jp/submission/21623
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
    #line 2 \"template/template_dump.hpp\"\n\n#line 4 \"template/template_dump.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09\n * @docs\
    \ docs/template/template_dump.md\n */\n\n#ifdef LOCAL\n#include <cpp-dump.hpp>\
    \ // https://github.com/philip82148/cpp-dump\nnamespace cpp_dump::_detail\n{\n\
    \  inline string export_var(\n      const i128 &x, const string &indent, size_t\
    \ last_line_length,\n      size_t current_depth, bool fail_on_newline, const export_command\
    \ &command\n  ) {\n    return export_var(i128tos(x), indent, last_line_length,\
    \ current_depth, fail_on_newline, command);\n  }\n} // namespace cpp_dump::_detail\n\
    #define dump(...) cpp_dump(__VA_ARGS__)\nnamespace cp = cpp_dump;\nCPP_DUMP_SET_OPTION_GLOBAL(log_label_func,\
    \ cp::log_label::line());\nCPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 1000);\n\
    #define local(...) __VA_ARGS__\n#define oj(...)\n#define local_oj(a, b) (a)\n\
    #else\n#define dump(...)\n#define local(...)\n#define oj(...) __VA_ARGS__\n#define\
    \ local_oj(a, b) (b)\n#endif\n\ntemplate <class T, class Sequence>\nvc<T> content(queue<T,\
    \ Sequence> que)\n{\n  vc<T> res;\n  while (!que.empty())\n  {\n    res.eb(que.front());\n\
    \    que.pop();\n  }\n  return res;\n}\ntemplate <class T, class Sequence, class\
    \ Compare>\nvc<T> content(priority_queue<T, Sequence, Compare> pque)\n{\n  vc<T>\
    \ res;\n  while (!pque.empty())\n  {\n    res.eb(pque.top());\n    pque.pop();\n\
    \  }\n  return res;\n}\n#line 6 \"template/template_inout.hpp\"\n\n/**\n * @brief\
    \ \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\uFF09\n * @docs\
    \ docs/template/template_inout.md\n */\n\n// https://judge.yosupo.jp/submission/170706\
    \ (maspy \u3055\u3093)\n// https://judge.yosupo.jp/submission/21623  (Nyaan \u3055\
    \u3093)\n#if defined FAST_IO and not defined LOCAL\nnamespace fastio {\nstatic\
    \ constexpr uint32_t SIZ = 1 << 17;\nchar ibuf[SIZ];\nchar obuf[SIZ];\nchar out[100];\n\
    // pointer of ibuf, obuf\nuint32_t pil = 0, pir = 0, por = 0;\n\nstruct Pre {\n\
    \  char num[10000][4];\n  constexpr Pre() : num() {\n    for (int i = 0; i < 10000;\
    \ i++) {\n      int n = i;\n      for (int j = 3; j >= 0; j--) {\n        num[i][j]\
    \ = n % 10 | '0';\n        n /= 10;\n      }\n    }\n  }\n} constexpr pre;\n\n\
    inline void load() {\n  memcpy(ibuf, ibuf + pil, pir - pil);\n  pir = pir - pil\
    \ + fread(ibuf + pir - pil, 1, SIZ - pir + pil, stdin);\n  pil = 0;\n  if (pir\
    \ < SIZ) ibuf[pir++] = '\\n';\n}\n\ninline void flush() {\n  fwrite(obuf, 1, por,\
    \ stdout);\n  por = 0;\n}\n\nvoid rd1(char &c) {\n  do {\n    if (pil + 1 > pir)\
    \ load();\n    c = ibuf[pil++];\n  } while (isspace(c));\n}\n\nvoid rd1(string\
    \ &x) {\n  x.clear();\n  char c;\n  do {\n    if (pil + 1 > pir) load();\n   \
    \ c = ibuf[pil++];\n  } while (isspace(c));\n  do {\n    x += c;\n    if (pil\
    \ == pir) load();\n    c = ibuf[pil++];\n  } while (!isspace(c));\n}\n\ntemplate\
    \ <typename T>\nvoid rd1_real(T &x) {\n  string s;\n  rd1(s);\n  x = stod(s);\n\
    }\n\ntemplate <typename T>\nvoid rd1_integer(T &x) {\n  if (pil + 100 > pir) load();\n\
    \  char c;\n  do\n    c = ibuf[pil++];\n  while (c < '-');\n  bool minus = 0;\n\
    \  if constexpr (is_signed<T>::value || is_same_v<T, i128>) {\n    if (c == '-')\
    \ { minus = 1, c = ibuf[pil++]; }\n  }\n  x = 0;\n  while ('0' <= c) { x = x *\
    \ 10 + (c & 15), c = ibuf[pil++]; }\n  if constexpr (is_signed<T>::value || is_same_v<T,\
    \ i128>) {\n    if (minus) x = -x;\n  }\n}\n\nvoid rd1(int &x) { rd1_integer(x);\
    \ }\nvoid rd1(ll &x) { rd1_integer(x); }\nvoid rd1(i128 &x) { rd1_integer(x);\
    \ }\nvoid rd1(uint &x) { rd1_integer(x); }\nvoid rd1(ull &x) { rd1_integer(x);\
    \ }\nvoid rd1(u128 &x) { rd1_integer(x); }\nvoid rd1(double &x) { rd1_real(x);\
    \ }\nvoid rd1(long double &x) { rd1_real(x); }\n// void rd1(f128 &x) { rd1_real(x);\
    \ }\n\ntemplate <class T, class U>\nvoid rd1(pair<T, U> &p) {\n  return rd1(p.first),\
    \ rd1(p.second);\n}\ntemplate <size_t N = 0, typename T>\nvoid rd1_tuple(T &t)\
    \ {\n  if constexpr (N < std::tuple_size<T>::value) {\n    auto &x = std::get<N>(t);\n\
    \    rd1(x);\n    rd1_tuple<N + 1>(t);\n  }\n}\ntemplate <class... T>\nvoid rd1(tuple<T...>\
    \ &tpl) {\n  rd1_tuple(tpl);\n}\n\ntemplate <size_t N = 0, typename T>\nvoid rd1(array<T,\
    \ N> &x) {\n  for (auto &d: x) rd1(d);\n}\ntemplate <class T>\nvoid rd1(vc<T>\
    \ &x) {\n  for (auto &d: x) rd1(d);\n}\n\nvoid read() {}\ntemplate <class H, class...\
    \ T>\nvoid read(H &h, T &... t) {\n  rd1(h), read(t...);\n}\n\nvoid wt1(const\
    \ char c) {\n  if (por == SIZ) flush();\n  obuf[por++] = c;\n}\nvoid wt1(const\
    \ string s) {\n  for (char c: s) wt1(c);\n}\nvoid wt1(const char *s) {\n  size_t\
    \ len = strlen(s);\n  for (size_t i = 0; i < len; i++) wt1(s[i]);\n}\n\ntemplate\
    \ <typename T>\nvoid wt1_integer(T x) {\n  if (por > SIZ - 100) flush();\n  if\
    \ (x < 0) { obuf[por++] = '-', x = -x; }\n  int outi;\n  for (outi = 96; x >=\
    \ 10000; outi -= 4) {\n    memcpy(out + outi, pre.num[x % 10000], 4);\n    x /=\
    \ 10000;\n  }\n  if (x >= 1000) {\n    memcpy(obuf + por, pre.num[x], 4);\n  \
    \  por += 4;\n  } else if (x >= 100) {\n    memcpy(obuf + por, pre.num[x] + 1,\
    \ 3);\n    por += 3;\n  } else if (x >= 10) {\n    int q = (x * 103) >> 10;\n\
    \    obuf[por] = q | '0';\n    obuf[por + 1] = (x - q * 10) | '0';\n    por +=\
    \ 2;\n  } else\n    obuf[por++] = x | '0';\n  memcpy(obuf + por, out + outi +\
    \ 4, 96 - outi);\n  por += 96 - outi;\n}\n\ntemplate <typename T>\nvoid wt1_real(T\
    \ x) {\n  ostringstream oss;\n  oss << fixed << setprecision(15) << double(x);\n\
    \  string s = oss.str();\n  wt1(s);\n}\n\nvoid wt1(int x) { wt1_integer(x); }\n\
    template <class T, enable_if_t<is_integral_v<T>, int> = 0>\nvoid wt1(T x) { wt1_integer(x);\
    \ }\nvoid wt1(i128 x) { wt1_integer(x); }\nvoid wt1(u128 x) { wt1_integer(x);\
    \ }\nvoid wt1(double x) { wt1_real(x); }\nvoid wt1(long double x) { wt1_real(x);\
    \ }\n// void wt1(f128 x) { wt1_real(x); }\n\ntemplate <class T, class U>\nvoid\
    \ wt1(const pair<T, U> &val) {\n  wt1(val.first);\n  wt1(' ');\n  wt1(val.second);\n\
    }\ntemplate <size_t N = 0, typename T>\nvoid wt1_tuple(const T &t) {\n  if constexpr\
    \ (N < std::tuple_size<T>::value) {\n    if constexpr (N > 0) { wt1(' '); }\n\
    \    const auto x = std::get<N>(t);\n    wt1(x);\n    wt1_tuple<N + 1>(t);\n \
    \ }\n}\ntemplate <class... T>\nvoid wt1(const tuple<T...> &tpl) {\n  wt1_tuple(tpl);\n\
    }\ntemplate <class T, size_t S>\nvoid wt1(const array<T, S> &val) {\n  auto n\
    \ = val.size();\n  for (size_t i = 0; i < n; i++) {\n    if (i) wt1(' ');\n  \
    \  wt1(val[i]);\n  }\n}\ntemplate <class T>\nvoid wt1(const vector<T> &val) {\n\
    \  auto n = val.size();\n  for (size_t i = 0; i < n; i++) {\n    if (i) wt1('\
    \ ');\n    wt1(val[i]);\n  }\n}\n\nvoid write() {}\ntemplate <class Head, class...\
    \ Tail>\nvoid write(Head &&head, Tail &&... tail) {\n  wt1(head);\n  write(std::forward<Tail>(tail)...);\n\
    }\n\nvoid print() { wt1('\\n'); }\ntemplate <class Head, class... Tail>\nvoid\
    \ print(Head &&head, Tail &&... tail) {\n  wt1(head);\n  if (sizeof...(Tail))\
    \ wt1(' ');\n  print(std::forward<Tail>(tail)...);\n}\n\n} // namespace fastio\n\
    \n#endif\n\n#if defined FAST_IO and not defined LOCAL\nstruct Dummy {\n  Dummy()\
    \ { atexit(fastio::flush); }\n} dummy;\n#endif\n\n// https://trap.jp/post/1224/\n\
    \n// ---- \u5165\u529B ----\n#if defined LOCAL or not defined FAST_IO\ntemplate\
    \ <class T, class U>\nistream &operator>>(istream &is, pair<T, U> &p)\n{\n  is\
    \ >> p.first >> p.second;\n  return is;\n}\ntemplate <class... Ts>\nistream &operator>>(istream\
    \ &is, tuple<Ts...> &t)\n{\n  apply([&](auto &...a)\n        { (is >> ... >> a);\
    \ }, t);\n  return is;\n}\ntemplate <class T, size_t n>\nistream &operator>>(istream\
    \ &is, array<T, n> &a)\n{\n  for (size_t i = 0; i < n; i++)\n    is >> a[i];\n\
    \  return is;\n}\ntemplate <class T>\nistream &operator>>(istream &is, vc<T> &a)\n\
    {\n  const size_t n = a.size();\n  for (size_t i = 0; i < n; i++)\n    is >> a[i];\n\
    \  return is;\n}\n#endif\n\nnamespace internal\n{\n\n#if defined LOCAL or not\
    \ defined FAST_IO\ntemplate <class... Ts>\nvoid CIN(Ts &...a) { (cin >> ... >>\
    \ a); }\n#endif\n\n#if defined FAST_IO and not defined LOCAL\ntemplate <class...\
    \ Ts>\nvoid READnodump(Ts &...a) { fastio::read(a...); }\n#else\ntemplate <class...\
    \ Ts>\nvoid READnodump(Ts &...a) { CIN(a...); }\n#endif\n\ntemplate <class T>\n\
    void READVECnodump(int n, vc<T> &v)\n{\n  v.resize(n);\n  READnodump(v);\n}\n\
    template <class T, class... Ts>\nvoid READVECnodump(int n, vc<T> &v, vc<Ts> &...vs)\n\
    { READVECnodump(n, v), READVECnodump(n, vs...); }\n\ntemplate <class T>\nvoid\
    \ READVEC2nodump(int n, int m, vvc<T> &v)\n{\n  v.assign(n, vc<T>(m));\n  READnodump(v);\n\
    }\ntemplate <class T, class... Ts>\nvoid READVEC2nodump(int n, int m, vvc<T> &v,\
    \ vvc<Ts> &...vs)\n{ READVEC2nodump(n, m, v), READVEC2nodump(n, m, vs...); }\n\
    \ntemplate <class T>\nvoid READJAGnodump(int n, vvc<T> &v)\n{\n  v.resize(n);\n\
    \  repi(i, n)\n  {\n    int k;\n    READnodump(k);\n    READVECnodump(k, v[i]);\n\
    \  }\n}\ntemplate <class T, class... Ts>\nvoid READJAGnodump(int n, vvc<T> &v,\
    \ vvc<Ts> &...vs)\n{ READJAGnodump(n, v), READJAGnodump(n, vs...); }\n\n}; //\
    \ namespace internal\n\n#define READ(...) internal::READnodump(__VA_ARGS__); dump(__VA_ARGS__)\n\
    \n#define IN(T, ...) T __VA_ARGS__; READ(__VA_ARGS__)\n\n#define CHAR(...) IN(char,\
    \ __VA_ARGS__)\n#define INT(...) IN(int, __VA_ARGS__)\n#define LL(...) IN(ll,\
    \ __VA_ARGS__)\n#define STR(...) IN(string, __VA_ARGS__)\n#define ARR(T, n, ...)\
    \ array<T, n> __VA_ARGS__; READ(__VA_ARGS__)\n\n#define READVEC(...) internal::READVECnodump(__VA_ARGS__);\
    \ dump(__VA_ARGS__)\n#define READVEC2(...) internal::READVEC2nodump(__VA_ARGS__);\
    \ dump(__VA_ARGS__)\n\n#define VEC(T, n, ...) vc<T> __VA_ARGS__; READVEC(n, __VA_ARGS__)\n\
    #define VEC2(T, n, m, ...) vvc<T> __VA_ARGS__; READVEC2(n, m, __VA_ARGS__)\n\n\
    #define READJAG(...) internal::READJAGnodump(__VA_ARGS__); dump(__VA_ARGS__)\n\
    \n#define JAG(T, n, ...) vvc<T> __VA_ARGS__; READJAG(n, __VA_ARGS__)\n\n// ----------\n\
    \n// ----- \u51FA\u529B -----\n#ifdef INTERACTIVE\n#define ENDL endl\n#else\n\
    #define ENDL '\\n'\n#endif\n\n#if defined LOCAL or not defined FAST_IO\ntemplate\
    \ <class T, class U>\nostream &operator<<(ostream &os, const pair<T, U> &p)\n\
    {\n  os << p.first << ' ' << p.second;\n  return os;\n}\n\nnamespace internal\n\
    {\n\ntemplate <size_t N = 0, typename T>\nvoid cout_tuple(ostream &os, const T\
    \ &t) {\n  if constexpr (N < std::tuple_size<T>::value) {\n    if constexpr (N\
    \ > 0) { os << ' '; }\n    const auto x = std::get<N>(t);\n    os << x;\n    cout_tuple<N\
    \ + 1>(os, t);\n  }\n}\n\n}; // namespace internal\n\ntemplate <class... Ts>\n\
    ostream &operator<<(ostream &os, const tuple<Ts...> &t)\n{\n  internal::cout_tuple(os,\
    \ t);\n  return os;\n}\ntemplate <class T, size_t n>\nostream &operator<<(ostream\
    \ &os, const array<T, n> &a)\n{\n  for (size_t i = 0; i < n; i++)\n  {\n    if\
    \ (i)\n      os << ' ';\n    os << a[i];\n  }\n  return os;\n}\ntemplate <class\
    \ T>\nostream &operator<<(ostream &os, const vc<T> &v)\n{\n  const size_t n =\
    \ v.size();\n  for (size_t i = 0; i < n; i++)\n  {\n    if (i)\n      os << '\
    \ ';\n    os << v[i];\n  }\n  return os;\n}\n\nnamespace internal\n{\n\ntemplate\
    \ <class T>\nvoid COUTW() {}\ntemplate <class... Ts>\nvoid COUTW(const Ts &...a)\
    \ { (cout << ... << a); }\n\ntemplate <class T>\nvoid COUTP() { cout << ENDL;\
    \ }\ntemplate <class T>\nvoid COUTP(const T &a) { cout << a << ENDL; }\ntemplate\
    \ <class T, class... Ts>\nvoid COUTP(const T &a, const Ts &...b)\n{\n  cout <<\
    \ a;\n  (cout << ... << (cout << ' ', b));\n  cout << ENDL;\n}\n\n}; // namespace\
    \ internal\n#endif\n\n#if defined FAST_IO and not defined LOCAL\n#define WRITE\
    \ fastio::write\n#define PRINT fastio::print\n#else\n#define WRITE internal::COUTW\n\
    #define PRINT internal::COUTP\n#endif\n#define PRINTEXIT(...) do { PRINT(__VA_ARGS__);\
    \ exit(0); } while (false)\n#define PRINTRETURN(...) do { PRINT(__VA_ARGS__);\
    \ return; } while (false)\n\ntemplate <class T>\nvoid PRINTV(const vc<T> &v) {\
    \ for (auto &vi : v) PRINT(vi); }\n#define PRINTVEXIT(...) do { PRINTV(__VA_ARGS__);\
    \ exit(0); } while (false)\n#define PRINTVRETURN(...) do { PRINTV(__VA_ARGS__);\
    \ return; } while (false)\n// ----------\n\n// ----- \u57FA\u6E96\u305A\u3089\u3057\
    \ -----\ntemplate <class T, class U, class P>\npair<T, U> operator+=(pair<T, U>\
    \ &a, const P &b)\n{\n  a.first += b.first;\n  a.second += b.second;\n  return\
    \ a;\n}\ntemplate <class T, class U, class P>\npair<T, U> operator+(pair<T, U>\
    \ &a, const P &b) { return a += b; }\n\ntemplate <class T, size_t n, class A>\n\
    array<T, n> operator+=(array<T, n> &a, const A &b)\n{\n  for (size_t i = 0; i\
    \ < n; i++)\n    a[i] += b[i];\n  return a;\n}\ntemplate <class T, size_t n, class\
    \ A>\narray<T, n> operator+(array<T, n> &a, const A &b) { return a += b; }\n\n\
    namespace internal\n{\n\ntemplate <size_t... I, class A, class B>\nauto tuple_add_impl(A\
    \ &a, const B &b, const index_sequence<I...>)\n{\n  ((get<I>(a) += get<I>(b)),\
    \ ...);\n  return a;\n}\n\n}; // namespace internal\n\ntemplate <class... Ts,\
    \ class Tp>\ntuple<Ts...> operator+=(tuple<Ts...> &a, const Tp &b)\n{ return internal::tuple_add_impl(a,\
    \ b, make_index_sequence<tuple_size_v<tuple<Ts...>>>{}); }\ntemplate <class...\
    \ Ts, class Tp>\ntuple<Ts...> operator+(tuple<Ts...> &a, const Tp &b) { return\
    \ a += b; }\n\ntemplate <class T, class Add>\nvoid offset(vc<T> &v, const Add\
    \ &add) { for (auto &vi : v) vi += add; }\ntemplate <class T, class Add>\nvoid\
    \ offset(vvc<T> &v, const Add &add) { for (auto &vi : v) for (auto &vij : vi)\
    \ vij += add; }\n// ----------\n\n// ----- \u8EE2\u7F6E -----\ntemplate <class\
    \ T, const size_t m>\narray<vc<T>, m> unzip(const vc<array<T, m>> &vt)\n{\n  const\
    \ size_t n = vt.size();\n  array<vc<T>, m> tv;\n  tv.fill(vc<T>(n));\n  for (size_t\
    \ i = 0; i < n; i++)\n    for (size_t j = 0; j < m; j++)\n      tv[j][i] = vt[i][j];\n\
    \  return tv;\n}\ntemplate <class T, const size_t m>\nvc<array<T, m>> zip(const\
    \ array<vc<T>, m> &tv)\n{\n  if (tv.empty()) return {};\n  const size_t n = tv[0].size();\n\
    \  vc<array<T, m>> vt(n);\n  for (size_t j = 0; j < m; j++)\n  {\n    assert(tv[j].size()\
    \ == n);\n    for (size_t i = 0; i < n; i++)\n      vt[i][j] = tv[j][i];\n  }\n\
    \  return vt;\n}\n\ntemplate <class T, class U>\npair<vc<T>, vc<U>> unzip(const\
    \ vc<pair<T, U>> &vt)\n{\n  const size_t n = vt.size();\n  pair<vc<T>, vc<U>>\
    \ tv;\n  tv.first.resize(n), tv.second.resize(n);\n  for (size_t i = 0; i < n;\
    \ i++)\n    tie(tv.first[i], tv.second[i]) = vt[i];\n  return tv;\n}\ntemplate\
    \ <class T, class U>\nvc<pair<T, U>> zip(const pair<vc<T>, vc<U>> &tv)\n{\n  const\
    \ size_t n = tv.first.size();\n  assert(n == tv.second.size());\n  vc<pair<T,\
    \ U>> vt(n);\n  for (size_t i = 0; i < n; i++)\n    vt[i] = make_pair(tv.first[i],\
    \ tv.second[i]);\n  return vt;\n}\n\nnamespace internal\n{\n\ntemplate <size_t...\
    \ I, class V, class Tp>\nauto vt_to_tv_impl(V &tv, const Tp &t, index_sequence<I...>,\
    \ size_t index)\n{ ((get<I>(tv)[index] = get<I>(t)), ...); }\n\ntemplate <size_t...\
    \ I, class Tp>\nauto tv_to_vt_impl(const Tp &tv, index_sequence<I...>, size_t\
    \ index)\n{ return make_tuple(get<I>(tv)[index]...); }\n\n};\n\ntemplate <class...\
    \ Ts>\nauto unzip(const vc<tuple<Ts...>> &vt)\n{\n  const size_t n = vt.size();\n\
    \  tuple<vc<Ts>...> tv;\n  apply([&](auto &...v)\n        { ((v.resize(n)), ...);\
    \ }, tv);\n  for (size_t i = 0; i < n; i++)\n    internal::vt_to_tv_impl(tv, vt[i],\
    \ make_index_sequence<tuple_size_v<decltype(tv)>>{}, i);\n  return tv;\n}\n\n\
    template <class... Ts>\nauto zip(const tuple<vc<Ts>...> &tv)\n{\n  size_t n =\
    \ get<0>(tv).size();\n  apply([&](auto &...v)\n        { ((assert(v.size() ==\
    \ n)), ...); }, tv);\n  vc<tuple<Ts...>> vt(n);\n  for (size_t i = 0; i < n; i++)\n\
    \    vt[i] = internal::tv_to_vt_impl(tv, index_sequence_for<Ts...>{}, i);\n  return\
    \ vt;\n}\n\n#define UNZIP(vt, ...) auto [__VA_ARGS__] = unzip(vt)\n#define ZIP(vt,\
    \ ...) auto vt = zip(tuple{__VA_ARGS__})\n// ----------\n"
  code: "#pragma once\n\n#include \"template_types.hpp\"\n#include \"template_rep.hpp\"\
    \n#include \"template_dump.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\
    \u30C8\uFF08\u5165\u51FA\u529B\uFF09\n * @docs docs/template/template_inout.md\n\
    \ */\n\n// https://judge.yosupo.jp/submission/170706 (maspy \u3055\u3093)\n//\
    \ https://judge.yosupo.jp/submission/21623  (Nyaan \u3055\u3093)\n#if defined\
    \ FAST_IO and not defined LOCAL\nnamespace fastio {\nstatic constexpr uint32_t\
    \ SIZ = 1 << 17;\nchar ibuf[SIZ];\nchar obuf[SIZ];\nchar out[100];\n// pointer\
    \ of ibuf, obuf\nuint32_t pil = 0, pir = 0, por = 0;\n\nstruct Pre {\n  char num[10000][4];\n\
    \  constexpr Pre() : num() {\n    for (int i = 0; i < 10000; i++) {\n      int\
    \ n = i;\n      for (int j = 3; j >= 0; j--) {\n        num[i][j] = n % 10 | '0';\n\
    \        n /= 10;\n      }\n    }\n  }\n} constexpr pre;\n\ninline void load()\
    \ {\n  memcpy(ibuf, ibuf + pil, pir - pil);\n  pir = pir - pil + fread(ibuf +\
    \ pir - pil, 1, SIZ - pir + pil, stdin);\n  pil = 0;\n  if (pir < SIZ) ibuf[pir++]\
    \ = '\\n';\n}\n\ninline void flush() {\n  fwrite(obuf, 1, por, stdout);\n  por\
    \ = 0;\n}\n\nvoid rd1(char &c) {\n  do {\n    if (pil + 1 > pir) load();\n   \
    \ c = ibuf[pil++];\n  } while (isspace(c));\n}\n\nvoid rd1(string &x) {\n  x.clear();\n\
    \  char c;\n  do {\n    if (pil + 1 > pir) load();\n    c = ibuf[pil++];\n  }\
    \ while (isspace(c));\n  do {\n    x += c;\n    if (pil == pir) load();\n    c\
    \ = ibuf[pil++];\n  } while (!isspace(c));\n}\n\ntemplate <typename T>\nvoid rd1_real(T\
    \ &x) {\n  string s;\n  rd1(s);\n  x = stod(s);\n}\n\ntemplate <typename T>\n\
    void rd1_integer(T &x) {\n  if (pil + 100 > pir) load();\n  char c;\n  do\n  \
    \  c = ibuf[pil++];\n  while (c < '-');\n  bool minus = 0;\n  if constexpr (is_signed<T>::value\
    \ || is_same_v<T, i128>) {\n    if (c == '-') { minus = 1, c = ibuf[pil++]; }\n\
    \  }\n  x = 0;\n  while ('0' <= c) { x = x * 10 + (c & 15), c = ibuf[pil++]; }\n\
    \  if constexpr (is_signed<T>::value || is_same_v<T, i128>) {\n    if (minus)\
    \ x = -x;\n  }\n}\n\nvoid rd1(int &x) { rd1_integer(x); }\nvoid rd1(ll &x) { rd1_integer(x);\
    \ }\nvoid rd1(i128 &x) { rd1_integer(x); }\nvoid rd1(uint &x) { rd1_integer(x);\
    \ }\nvoid rd1(ull &x) { rd1_integer(x); }\nvoid rd1(u128 &x) { rd1_integer(x);\
    \ }\nvoid rd1(double &x) { rd1_real(x); }\nvoid rd1(long double &x) { rd1_real(x);\
    \ }\n// void rd1(f128 &x) { rd1_real(x); }\n\ntemplate <class T, class U>\nvoid\
    \ rd1(pair<T, U> &p) {\n  return rd1(p.first), rd1(p.second);\n}\ntemplate <size_t\
    \ N = 0, typename T>\nvoid rd1_tuple(T &t) {\n  if constexpr (N < std::tuple_size<T>::value)\
    \ {\n    auto &x = std::get<N>(t);\n    rd1(x);\n    rd1_tuple<N + 1>(t);\n  }\n\
    }\ntemplate <class... T>\nvoid rd1(tuple<T...> &tpl) {\n  rd1_tuple(tpl);\n}\n\
    \ntemplate <size_t N = 0, typename T>\nvoid rd1(array<T, N> &x) {\n  for (auto\
    \ &d: x) rd1(d);\n}\ntemplate <class T>\nvoid rd1(vc<T> &x) {\n  for (auto &d:\
    \ x) rd1(d);\n}\n\nvoid read() {}\ntemplate <class H, class... T>\nvoid read(H\
    \ &h, T &... t) {\n  rd1(h), read(t...);\n}\n\nvoid wt1(const char c) {\n  if\
    \ (por == SIZ) flush();\n  obuf[por++] = c;\n}\nvoid wt1(const string s) {\n \
    \ for (char c: s) wt1(c);\n}\nvoid wt1(const char *s) {\n  size_t len = strlen(s);\n\
    \  for (size_t i = 0; i < len; i++) wt1(s[i]);\n}\n\ntemplate <typename T>\nvoid\
    \ wt1_integer(T x) {\n  if (por > SIZ - 100) flush();\n  if (x < 0) { obuf[por++]\
    \ = '-', x = -x; }\n  int outi;\n  for (outi = 96; x >= 10000; outi -= 4) {\n\
    \    memcpy(out + outi, pre.num[x % 10000], 4);\n    x /= 10000;\n  }\n  if (x\
    \ >= 1000) {\n    memcpy(obuf + por, pre.num[x], 4);\n    por += 4;\n  } else\
    \ if (x >= 100) {\n    memcpy(obuf + por, pre.num[x] + 1, 3);\n    por += 3;\n\
    \  } else if (x >= 10) {\n    int q = (x * 103) >> 10;\n    obuf[por] = q | '0';\n\
    \    obuf[por + 1] = (x - q * 10) | '0';\n    por += 2;\n  } else\n    obuf[por++]\
    \ = x | '0';\n  memcpy(obuf + por, out + outi + 4, 96 - outi);\n  por += 96 -\
    \ outi;\n}\n\ntemplate <typename T>\nvoid wt1_real(T x) {\n  ostringstream oss;\n\
    \  oss << fixed << setprecision(15) << double(x);\n  string s = oss.str();\n \
    \ wt1(s);\n}\n\nvoid wt1(int x) { wt1_integer(x); }\ntemplate <class T, enable_if_t<is_integral_v<T>,\
    \ int> = 0>\nvoid wt1(T x) { wt1_integer(x); }\nvoid wt1(i128 x) { wt1_integer(x);\
    \ }\nvoid wt1(u128 x) { wt1_integer(x); }\nvoid wt1(double x) { wt1_real(x); }\n\
    void wt1(long double x) { wt1_real(x); }\n// void wt1(f128 x) { wt1_real(x); }\n\
    \ntemplate <class T, class U>\nvoid wt1(const pair<T, U> &val) {\n  wt1(val.first);\n\
    \  wt1(' ');\n  wt1(val.second);\n}\ntemplate <size_t N = 0, typename T>\nvoid\
    \ wt1_tuple(const T &t) {\n  if constexpr (N < std::tuple_size<T>::value) {\n\
    \    if constexpr (N > 0) { wt1(' '); }\n    const auto x = std::get<N>(t);\n\
    \    wt1(x);\n    wt1_tuple<N + 1>(t);\n  }\n}\ntemplate <class... T>\nvoid wt1(const\
    \ tuple<T...> &tpl) {\n  wt1_tuple(tpl);\n}\ntemplate <class T, size_t S>\nvoid\
    \ wt1(const array<T, S> &val) {\n  auto n = val.size();\n  for (size_t i = 0;\
    \ i < n; i++) {\n    if (i) wt1(' ');\n    wt1(val[i]);\n  }\n}\ntemplate <class\
    \ T>\nvoid wt1(const vector<T> &val) {\n  auto n = val.size();\n  for (size_t\
    \ i = 0; i < n; i++) {\n    if (i) wt1(' ');\n    wt1(val[i]);\n  }\n}\n\nvoid\
    \ write() {}\ntemplate <class Head, class... Tail>\nvoid write(Head &&head, Tail\
    \ &&... tail) {\n  wt1(head);\n  write(std::forward<Tail>(tail)...);\n}\n\nvoid\
    \ print() { wt1('\\n'); }\ntemplate <class Head, class... Tail>\nvoid print(Head\
    \ &&head, Tail &&... tail) {\n  wt1(head);\n  if (sizeof...(Tail)) wt1(' ');\n\
    \  print(std::forward<Tail>(tail)...);\n}\n\n} // namespace fastio\n\n#endif\n\
    \n#if defined FAST_IO and not defined LOCAL\nstruct Dummy {\n  Dummy() { atexit(fastio::flush);\
    \ }\n} dummy;\n#endif\n\n// https://trap.jp/post/1224/\n\n// ---- \u5165\u529B\
    \ ----\n#if defined LOCAL or not defined FAST_IO\ntemplate <class T, class U>\n\
    istream &operator>>(istream &is, pair<T, U> &p)\n{\n  is >> p.first >> p.second;\n\
    \  return is;\n}\ntemplate <class... Ts>\nistream &operator>>(istream &is, tuple<Ts...>\
    \ &t)\n{\n  apply([&](auto &...a)\n        { (is >> ... >> a); }, t);\n  return\
    \ is;\n}\ntemplate <class T, size_t n>\nistream &operator>>(istream &is, array<T,\
    \ n> &a)\n{\n  for (size_t i = 0; i < n; i++)\n    is >> a[i];\n  return is;\n\
    }\ntemplate <class T>\nistream &operator>>(istream &is, vc<T> &a)\n{\n  const\
    \ size_t n = a.size();\n  for (size_t i = 0; i < n; i++)\n    is >> a[i];\n  return\
    \ is;\n}\n#endif\n\nnamespace internal\n{\n\n#if defined LOCAL or not defined\
    \ FAST_IO\ntemplate <class... Ts>\nvoid CIN(Ts &...a) { (cin >> ... >> a); }\n\
    #endif\n\n#if defined FAST_IO and not defined LOCAL\ntemplate <class... Ts>\n\
    void READnodump(Ts &...a) { fastio::read(a...); }\n#else\ntemplate <class... Ts>\n\
    void READnodump(Ts &...a) { CIN(a...); }\n#endif\n\ntemplate <class T>\nvoid READVECnodump(int\
    \ n, vc<T> &v)\n{\n  v.resize(n);\n  READnodump(v);\n}\ntemplate <class T, class...\
    \ Ts>\nvoid READVECnodump(int n, vc<T> &v, vc<Ts> &...vs)\n{ READVECnodump(n,\
    \ v), READVECnodump(n, vs...); }\n\ntemplate <class T>\nvoid READVEC2nodump(int\
    \ n, int m, vvc<T> &v)\n{\n  v.assign(n, vc<T>(m));\n  READnodump(v);\n}\ntemplate\
    \ <class T, class... Ts>\nvoid READVEC2nodump(int n, int m, vvc<T> &v, vvc<Ts>\
    \ &...vs)\n{ READVEC2nodump(n, m, v), READVEC2nodump(n, m, vs...); }\n\ntemplate\
    \ <class T>\nvoid READJAGnodump(int n, vvc<T> &v)\n{\n  v.resize(n);\n  repi(i,\
    \ n)\n  {\n    int k;\n    READnodump(k);\n    READVECnodump(k, v[i]);\n  }\n\
    }\ntemplate <class T, class... Ts>\nvoid READJAGnodump(int n, vvc<T> &v, vvc<Ts>\
    \ &...vs)\n{ READJAGnodump(n, v), READJAGnodump(n, vs...); }\n\n}; // namespace\
    \ internal\n\n#define READ(...) internal::READnodump(__VA_ARGS__); dump(__VA_ARGS__)\n\
    \n#define IN(T, ...) T __VA_ARGS__; READ(__VA_ARGS__)\n\n#define CHAR(...) IN(char,\
    \ __VA_ARGS__)\n#define INT(...) IN(int, __VA_ARGS__)\n#define LL(...) IN(ll,\
    \ __VA_ARGS__)\n#define STR(...) IN(string, __VA_ARGS__)\n#define ARR(T, n, ...)\
    \ array<T, n> __VA_ARGS__; READ(__VA_ARGS__)\n\n#define READVEC(...) internal::READVECnodump(__VA_ARGS__);\
    \ dump(__VA_ARGS__)\n#define READVEC2(...) internal::READVEC2nodump(__VA_ARGS__);\
    \ dump(__VA_ARGS__)\n\n#define VEC(T, n, ...) vc<T> __VA_ARGS__; READVEC(n, __VA_ARGS__)\n\
    #define VEC2(T, n, m, ...) vvc<T> __VA_ARGS__; READVEC2(n, m, __VA_ARGS__)\n\n\
    #define READJAG(...) internal::READJAGnodump(__VA_ARGS__); dump(__VA_ARGS__)\n\
    \n#define JAG(T, n, ...) vvc<T> __VA_ARGS__; READJAG(n, __VA_ARGS__)\n\n// ----------\n\
    \n// ----- \u51FA\u529B -----\n#ifdef INTERACTIVE\n#define ENDL endl\n#else\n\
    #define ENDL '\\n'\n#endif\n\n#if defined LOCAL or not defined FAST_IO\ntemplate\
    \ <class T, class U>\nostream &operator<<(ostream &os, const pair<T, U> &p)\n\
    {\n  os << p.first << ' ' << p.second;\n  return os;\n}\n\nnamespace internal\n\
    {\n\ntemplate <size_t N = 0, typename T>\nvoid cout_tuple(ostream &os, const T\
    \ &t) {\n  if constexpr (N < std::tuple_size<T>::value) {\n    if constexpr (N\
    \ > 0) { os << ' '; }\n    const auto x = std::get<N>(t);\n    os << x;\n    cout_tuple<N\
    \ + 1>(os, t);\n  }\n}\n\n}; // namespace internal\n\ntemplate <class... Ts>\n\
    ostream &operator<<(ostream &os, const tuple<Ts...> &t)\n{\n  internal::cout_tuple(os,\
    \ t);\n  return os;\n}\ntemplate <class T, size_t n>\nostream &operator<<(ostream\
    \ &os, const array<T, n> &a)\n{\n  for (size_t i = 0; i < n; i++)\n  {\n    if\
    \ (i)\n      os << ' ';\n    os << a[i];\n  }\n  return os;\n}\ntemplate <class\
    \ T>\nostream &operator<<(ostream &os, const vc<T> &v)\n{\n  const size_t n =\
    \ v.size();\n  for (size_t i = 0; i < n; i++)\n  {\n    if (i)\n      os << '\
    \ ';\n    os << v[i];\n  }\n  return os;\n}\n\nnamespace internal\n{\n\ntemplate\
    \ <class T>\nvoid COUTW() {}\ntemplate <class... Ts>\nvoid COUTW(const Ts &...a)\
    \ { (cout << ... << a); }\n\ntemplate <class T>\nvoid COUTP() { cout << ENDL;\
    \ }\ntemplate <class T>\nvoid COUTP(const T &a) { cout << a << ENDL; }\ntemplate\
    \ <class T, class... Ts>\nvoid COUTP(const T &a, const Ts &...b)\n{\n  cout <<\
    \ a;\n  (cout << ... << (cout << ' ', b));\n  cout << ENDL;\n}\n\n}; // namespace\
    \ internal\n#endif\n\n#if defined FAST_IO and not defined LOCAL\n#define WRITE\
    \ fastio::write\n#define PRINT fastio::print\n#else\n#define WRITE internal::COUTW\n\
    #define PRINT internal::COUTP\n#endif\n#define PRINTEXIT(...) do { PRINT(__VA_ARGS__);\
    \ exit(0); } while (false)\n#define PRINTRETURN(...) do { PRINT(__VA_ARGS__);\
    \ return; } while (false)\n\ntemplate <class T>\nvoid PRINTV(const vc<T> &v) {\
    \ for (auto &vi : v) PRINT(vi); }\n#define PRINTVEXIT(...) do { PRINTV(__VA_ARGS__);\
    \ exit(0); } while (false)\n#define PRINTVRETURN(...) do { PRINTV(__VA_ARGS__);\
    \ return; } while (false)\n// ----------\n\n// ----- \u57FA\u6E96\u305A\u3089\u3057\
    \ -----\ntemplate <class T, class U, class P>\npair<T, U> operator+=(pair<T, U>\
    \ &a, const P &b)\n{\n  a.first += b.first;\n  a.second += b.second;\n  return\
    \ a;\n}\ntemplate <class T, class U, class P>\npair<T, U> operator+(pair<T, U>\
    \ &a, const P &b) { return a += b; }\n\ntemplate <class T, size_t n, class A>\n\
    array<T, n> operator+=(array<T, n> &a, const A &b)\n{\n  for (size_t i = 0; i\
    \ < n; i++)\n    a[i] += b[i];\n  return a;\n}\ntemplate <class T, size_t n, class\
    \ A>\narray<T, n> operator+(array<T, n> &a, const A &b) { return a += b; }\n\n\
    namespace internal\n{\n\ntemplate <size_t... I, class A, class B>\nauto tuple_add_impl(A\
    \ &a, const B &b, const index_sequence<I...>)\n{\n  ((get<I>(a) += get<I>(b)),\
    \ ...);\n  return a;\n}\n\n}; // namespace internal\n\ntemplate <class... Ts,\
    \ class Tp>\ntuple<Ts...> operator+=(tuple<Ts...> &a, const Tp &b)\n{ return internal::tuple_add_impl(a,\
    \ b, make_index_sequence<tuple_size_v<tuple<Ts...>>>{}); }\ntemplate <class...\
    \ Ts, class Tp>\ntuple<Ts...> operator+(tuple<Ts...> &a, const Tp &b) { return\
    \ a += b; }\n\ntemplate <class T, class Add>\nvoid offset(vc<T> &v, const Add\
    \ &add) { for (auto &vi : v) vi += add; }\ntemplate <class T, class Add>\nvoid\
    \ offset(vvc<T> &v, const Add &add) { for (auto &vi : v) for (auto &vij : vi)\
    \ vij += add; }\n// ----------\n\n// ----- \u8EE2\u7F6E -----\ntemplate <class\
    \ T, const size_t m>\narray<vc<T>, m> unzip(const vc<array<T, m>> &vt)\n{\n  const\
    \ size_t n = vt.size();\n  array<vc<T>, m> tv;\n  tv.fill(vc<T>(n));\n  for (size_t\
    \ i = 0; i < n; i++)\n    for (size_t j = 0; j < m; j++)\n      tv[j][i] = vt[i][j];\n\
    \  return tv;\n}\ntemplate <class T, const size_t m>\nvc<array<T, m>> zip(const\
    \ array<vc<T>, m> &tv)\n{\n  if (tv.empty()) return {};\n  const size_t n = tv[0].size();\n\
    \  vc<array<T, m>> vt(n);\n  for (size_t j = 0; j < m; j++)\n  {\n    assert(tv[j].size()\
    \ == n);\n    for (size_t i = 0; i < n; i++)\n      vt[i][j] = tv[j][i];\n  }\n\
    \  return vt;\n}\n\ntemplate <class T, class U>\npair<vc<T>, vc<U>> unzip(const\
    \ vc<pair<T, U>> &vt)\n{\n  const size_t n = vt.size();\n  pair<vc<T>, vc<U>>\
    \ tv;\n  tv.first.resize(n), tv.second.resize(n);\n  for (size_t i = 0; i < n;\
    \ i++)\n    tie(tv.first[i], tv.second[i]) = vt[i];\n  return tv;\n}\ntemplate\
    \ <class T, class U>\nvc<pair<T, U>> zip(const pair<vc<T>, vc<U>> &tv)\n{\n  const\
    \ size_t n = tv.first.size();\n  assert(n == tv.second.size());\n  vc<pair<T,\
    \ U>> vt(n);\n  for (size_t i = 0; i < n; i++)\n    vt[i] = make_pair(tv.first[i],\
    \ tv.second[i]);\n  return vt;\n}\n\nnamespace internal\n{\n\ntemplate <size_t...\
    \ I, class V, class Tp>\nauto vt_to_tv_impl(V &tv, const Tp &t, index_sequence<I...>,\
    \ size_t index)\n{ ((get<I>(tv)[index] = get<I>(t)), ...); }\n\ntemplate <size_t...\
    \ I, class Tp>\nauto tv_to_vt_impl(const Tp &tv, index_sequence<I...>, size_t\
    \ index)\n{ return make_tuple(get<I>(tv)[index]...); }\n\n};\n\ntemplate <class...\
    \ Ts>\nauto unzip(const vc<tuple<Ts...>> &vt)\n{\n  const size_t n = vt.size();\n\
    \  tuple<vc<Ts>...> tv;\n  apply([&](auto &...v)\n        { ((v.resize(n)), ...);\
    \ }, tv);\n  for (size_t i = 0; i < n; i++)\n    internal::vt_to_tv_impl(tv, vt[i],\
    \ make_index_sequence<tuple_size_v<decltype(tv)>>{}, i);\n  return tv;\n}\n\n\
    template <class... Ts>\nauto zip(const tuple<vc<Ts>...> &tv)\n{\n  size_t n =\
    \ get<0>(tv).size();\n  apply([&](auto &...v)\n        { ((assert(v.size() ==\
    \ n)), ...); }, tv);\n  vc<tuple<Ts...>> vt(n);\n  for (size_t i = 0; i < n; i++)\n\
    \    vt[i] = internal::tv_to_vt_impl(tv, index_sequence_for<Ts...>{}, i);\n  return\
    \ vt;\n}\n\n#define UNZIP(vt, ...) auto [__VA_ARGS__] = unzip(vt)\n#define ZIP(vt,\
    \ ...) auto vt = zip(tuple{__VA_ARGS__})\n// ----------\n"
  dependsOn: []
  isVerificationFile: false
  path: template/template_inout.hpp
  requiredBy: []
  timestamp: '1970-01-01 00:00:00+00:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: template/template_inout.hpp
layout: document
redirect_from:
- /library/template/template_inout.hpp
- /library/template/template_inout.hpp.html
title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\uFF09"
---
## テンプレート（入出力）

### 入出力のタイプ

`FAST_IO`, `FAST_CIO`, `INTERACTIVE` のうちどれか $1$ つまでを `#define` する。$2$ つ以上を define してはならない。

- `FAST_IO`
  - （人から借りた）爆速入出力
    - 参考： https://maspypy.com/library-checker-many-a-b
  - cin, cout と併用してはならない
  - cin でたまにやる、`12+345` に対して `ll x, y; char c; cin >> x >> c >> y;` とするようなやつは多分未対応？（TLE するので注意）
    - 実装してないだけなのか原理的に難しいのかを知らない
- `FAST_CIO`
  - cin, cout を使う
  - `cin.tie(0); ios::sync_with_stdio(false);` をする
  - `ENDL` を `"\n"` にする
- `INTERACTIVE`
  - cin, cout を使う
  - `cin.tie(0); ios::sync_with_stdio(false);` をする
  - `ENDL` を `endl` にする
- 上記のどれも define されていないとき、または `LOCAL` が define されているとき
  - cin, cout を使う
  - `cin.tie(0); ios::sync_with_stdio(false);` をする
  - `ENDL` を `"\n"` にする

どの場合も、`READ`, `WRITE`, `IN`, `PRINT` が使える。これらの使い方はこの後詳しく述べるが、大雑把には次の通り：

- `READ`：定義済み変数に入力（cin でできることに相当）
- `WRITE`：空白や改行を自動で入れずに出力（cout でできることに相当）
- `IN`：変数を定義すると同時に入力
- `PRINT`：空白や改行を自動で入れて出力

ほとんどの場合 `IN` 系（`LL(...)` なども含む）と `PRINT` 系で十分だと思う。

### 入力


#### READ

`READ(...)` で、定義された変数（複数可）に入力する。

対応している型：

- 整数型
  - i128 含む
- modint 系（modint 側に実装）
- pair, tuple, array
- vector
  - 確保された size に応じて入力


他にも

- `READVEC(n, v1, v2, ...)`：すでにある vector `v1`, `v2`, $\dots$ について、サイズ $n$ にしてから入力
- `READVEC2(n, m, v1, v2, ...)`：すでにある vector `v1`, `v2`, $\dots$ について、サイズ $n\times m$ にしてから入力


#### IN

変数を宣言と同時に入力するマクロ。

- `IN(T, ...)`：`T` 型の値（複数可）
  - `READ(...)` が対応している型なら OK、ただし注意点：
    - カンマが入っている型に注意（`pair<int, int>` など。`using pii = pair<int, int> IN(pii, p)` のようにする）
    - vector は宣言後に resize する必要があるため `IN` を使ってもうまくいかない。`VEC` や `VEC2` を使う
　
- `CHAR(...)`
- `INT(...)`
- `LL(...)`
- `STR(...)`

　
- `ARR(T, n, ...)`：`array<T, n>` 型の値
- `VEC(T, n, ...)`：`vc<T>` 型の値、宣言してサイズを $n$ にして入力
- `VEC2(T, n, m, ...)`：`vc<vc<T>>` 型の値、宣言してサイズを $n \times m$ にして入力
- `JAG(T, n, ...)`：次のような入力形式（各行の列数が行の先頭にくる）の際に `vc<vc<T>>` 型の値を入力。$n$ は $1$ 次元目のサイズ。
  ```
  n
  k[1] a[1][1] ... a[1][k[1]]
  :
  k[n] a[n][1] ... a[n][k[n]]
  ```

---

`READ`, `IN` のどちらでも、手元では入力するたびに `dump` するようにしている。木の入力を $N$ 個にして無限ループか？と勘違いするなどはよくあるが、これをすると気づきやすくなると思う。

### 出力

#### WRITE と PRINT

- `WRITE(...)`：出力（引数は複数、区切りなし、改行なし）
- `PRINT(...)`：出力（引数は複数、空白区切り、最後に改行）

対応している型は `READ` と同じ。tuple-like や vector は要素を空白区切り。

#### それ以外

- `PRINTV(...)`：改行区切りで出力
  - `vpll`, `vstr`, `vvc<T>` などに
- `PRINTRETURN(...)`：`PRINT` して `return` する
  - `main` の外に `main2` を作っておけば、複数テストケースの場合も `PRINTRETURN` でよい
- `PRINTVRETURN(...)`：`PRINTV` して `return` する
- `PRINTEXIT(...)`：`PRINT` して `exit(0)` する
  - あまり使わない（そもそも単一テストケースでないと使えない）。再帰関数の中身で使うくらいか？（手元でサンプルを複数試せなくなるが）
- `PRINTVEXIT(...)`：`PRINTV` して `exit(0)` する

### その他

#### 0-indexed に直す

`offset(vc<T> v, T add)` や `offset(vvc<T> v, T add)` で `v` の各要素に `add` が足される。

`T` は tuple-like な型でも可。

副産物として、tuple-like な型どうしが `+` で足し算できる。

#### zip, unzip

`vc<tuple-like<T1, T2, ...>>` (vt) と `tuple-like<vc<T1>, vc<T2>, ...>` (tv) を相互変換する。vt → tv が `unzip` で、tv → vt が `zip`。

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

このような入力は、for 文で手癖で受け取ると入力形式ミスが発生しがちだが、このテンプレを使い始めてからその手のミスが非常に少なくなったと感じている。

また入力以外にも、vector 2 つを pair の vector にしてソートして vector 2 つに戻す、といった場面でも活躍する。

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
auto [A, B] = unzip(AB);
// 上記はマクロを使って UNZIP(AB, A, B) と書ける
```
  
これで `AB` は `vc<pll>` に、`A`, `B` はそれぞれ `vc<ll>` になる。


#### 例 5：vector 2 つを片方の値でソート

```
N
A_1 ... A_N
B_1 ... B_N
```

```cpp
LL(N);
VEC(ll, N, A, B);
auto AB = zip(pair{A, B});
// 上記はマクロを使って ZIP(AB, A, B) と書ける (ただしこのマクロではすべて tuple になる)
ranges::sort(AB);
tie(A, B) = unzip(AB);
```
  
このように、vector 2 つで与えられたものを、pair の vector にして、ソートして、vector 2 つに戻す、ということもできる。
  
#### 例 6：グラフ
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

####  例 7：二次元配列
  
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

#### 例 8：二次元の文字

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

#### 例 9：ジャグ配列 (1)

```
3
2 100 200
3 300 400 500
1 600
```

```cpp
LL(N);
JAG(ll, N, A);
```

各行の列数が行の最初に与えられるときは `JAG` を使えばよい。

#### 例 10：ジャグ配列 (2)

```
N
A[1][1]
A[2][1] A[2][2]
:
A[N][1] A[N][2] ... A[N][N]
```

```cpp
LL(N);
vl A(N);
rep(i, N) A.at(i).resize(i + 1);
READ(A);
```

各行の列数があらかじめわかっているときは、そのサイズの vector を作っておいてから `READ` すればよい。

----

### 設計について

（このファイルに限らないが）関数とマクロで大文字小文字が結構混在しているの、まずい気もするけど（現時点では）競技で使う用として大きな問題はないと判断
