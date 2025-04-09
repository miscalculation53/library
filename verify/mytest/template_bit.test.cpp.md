---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: template/template_algo.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30A2\u30EB\u30B4\u30EA\u30BA\
      \u30E0\uFF09"
  - icon: ':question:'
    path: template/template_bit.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30D3\u30C3\u30C8\u6F14\u7B97\
      \uFF09"
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
  - icon: ':question:'
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
  bundledCode: "#line 1 \"verify/mytest/template_bit.test.cpp\"\n#define PROBLEM \"\
    https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A\"\n\n#line\
    \ 2 \"template/template_bit.hpp\"\n\n#line 2 \"template/template_types.hpp\"\n\
    \n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09\n * @docs\
    \ docs/template/template_types.md\n */\n\n#include <bits/stdc++.h>\nusing namespace\
    \ std;\n\n#ifndef EPS\n#define EPS 1e-11\n#endif\nusing ld = decltype(EPS);\n\n\
    using ll = long long;\nusing uint = unsigned int;\nusing ull = unsigned long long;\n\
    using pll = pair<ll, ll>;\nusing tlll = tuple<ll, ll, ll>;\nusing tllll = tuple<ll,\
    \ ll, ll, ll>;\n\n#define vc vector\ntemplate <class T>\nusing vvc = vc<vc<T>>;\n\
    template <class T>\nusing vvvc = vc<vc<vc<T>>>;\n\nusing vb = vc<bool>;\nusing\
    \ vl = vc<ll>;\nusing vpll = vc<pll>;\nusing vtlll = vc<tlll>;\nusing vtllll =\
    \ vc<tllll>;\nusing vstr = vc<string>;\nusing vvb = vvc<bool>;\nusing vvl = vvc<ll>;\n\
    \ntemplate <class T>\nusing pql = priority_queue<T, vc<T>, greater<T>>;\ntemplate\
    \ <class T>\nusing pqg = priority_queue<T>;\n\n#ifdef __SIZEOF_INT128__\nusing\
    \ i128 = __int128_t;\nusing u128 = __uint128_t;\ni128 stoi128(const string &s)\n\
    {\n  i128 res = 0;\n  if (s.front() == '-')\n  {\n    for (int i = 1; i < (int)s.size();\
    \ i++)\n      res = 10 * res + s[i] - '0';\n    res = -res;\n  }\n  else\n  {\n\
    \    for (auto &&c : s)\n      res = 10 * res + c - '0';\n  }\n  return res;\n\
    }\nstring i128tos(i128 x)\n{\n  if (x == 0) return \"0\";\n  string sign = \"\"\
    , res = \"\";\n  if (x < 0)\n    x = -x, sign = \"-\";\n  while (x > 0)\n  {\n\
    \    res += '0' + x % 10;\n    x /= 10;\n  }\n  reverse(res.begin(), res.end());\n\
    \  return sign + res;\n}\nistream &operator>>(istream &is, i128 &a)\n{\n  string\
    \ s;\n  is >> s;\n  a = stoi128(s);\n  return is;\n}\nostream &operator<<(ostream\
    \ &os, const i128 &a)\n{\n  os << i128tos(a);\n  return os;\n}\n#endif\n\n#define\
    \ cauto const auto\n#line 2 \"template/template_rep.hpp\"\n\n#line 4 \"template/template_rep.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09\n * @docs\
    \ docs/template/template_rep.md\n */\n\n// https://trap.jp/post/1224/\n\n#define\
    \ overload4(_1, _2, _3, _4, name, ...) name\n#define rep1(i, n) for (ll i = 0,\
    \ nnnnn = ll(n); i < nnnnn; i++)\n#define rep2(i, l, r) for (ll i = ll(l), rrrrr\
    \ = ll(r); i < rrrrr; i++)\n#define rep3(i, l, r, d) for (ll i = ll(l), rrrrr\
    \ = ll(r), ddddd = ll(d); ddddd > 0 ? i < rrrrr : i > rrrrr; i += d)\n#define\
    \ rep(...) overload4(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)\n#define repi1(i,\
    \ n) for (int i = 0, nnnnn = int(n); i < nnnnn; i++)\n#define repi2(i, l, r) for\
    \ (int i = int(l), rrrrr = int(r); i < rrrrr; i++)\n#define repi3(i, l, r, d)\
    \ for (int i = int(l), rrrrr = int(r), ddddd = int(d); ddddd > 0 ? i < rrrrr :\
    \ i > rrrrr; i += d)\n#define repi(...) overload4(__VA_ARGS__, repi3, repi2, repi1)(__VA_ARGS__)\n\
    \n#define fe(...) for (auto __VA_ARGS__)\n#define fec(...) for (cauto &__VA_ARGS__)\n\
    #define fem(...) for (auto &__VA_ARGS__)\n#line 5 \"template/template_bit.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30D3\u30C3\u30C8\
    \u6F14\u7B97\uFF09\n * @docs docs/template/template_bit.md\n */\n\ntemplate <class\
    \ T>\ninline constexpr ull pow2(T k) { return 1ULL << k; }\ntemplate <class T>\n\
    inline constexpr ull MASK(T k) { return (1ULL << k) - 1ULL; }\n\n#if __cplusplus\
    \ < 202002L\n// x == 0 \u306A\u3089\u3070 0\u3001\u305D\u3046\u3067\u306A\u3051\
    \u308C\u3070 1 + floor(log2(x))\n// 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, ... \ninline\
    \ constexpr ull bit_width(ull x) { return x == 0 ? 0 : 64 - __builtin_clzll(x);\
    \ }\n// 0, 1, 2, 2, 4, 4, 4, 4, 8, 8, ...\ninline constexpr ull bit_floor(ull\
    \ x) { return x == 0 ? 0ULL : 1ULL << (bit_width(x) - 1); }\n// 1, 1, 2, 4, 4,\
    \ 8, 8, 8, 8, 16, ...\ninline constexpr ull bit_ceil(ull x) { return x == 0 ?\
    \ 1ULL : 1ULL << bit_width(x - 1); }\ninline constexpr ull countr_zero(ull x)\
    \ { assert(x != 0); return __builtin_ctzll(x); }\ninline constexpr ull popcount(ull\
    \ x) { return __builtin_popcountll(x); }\ninline constexpr bool has_single_bit(ull\
    \ x) { return popcount(x) == 1; }\n#else\n// 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, ...\
    \ \ninline constexpr ll bit_width(ll x) { return std::bit_width((ull)x); }\n//\
    \ 0, 1, 2, 2, 4, 4, 4, 4, 8, 8, ...\ninline constexpr ll bit_floor(ll x) { return\
    \ std::bit_floor((ull)x); }\n// 1, 1, 2, 4, 4, 8, 8, 8, 8, 16, ...\ninline constexpr\
    \ ll bit_ceil(ll x) { return std::bit_ceil((ull)x); }\ninline constexpr ll countr_zero(ll\
    \ x) { assert(x != 0); return std::countr_zero((ull)x); }\ninline constexpr ll\
    \ popcount(ll x) { return std::popcount((ull)x); }\ninline constexpr bool has_single_bit(ll\
    \ x) { return std::has_single_bit((ull)x); }\n#endif\n\ninline constexpr ull lsb_pos(ull\
    \ x) { assert(x != 0); return countr_zero(x); }\ninline constexpr ull msb_pos(ull\
    \ x) { assert(x != 0); return bit_width(x) - 1; }\ninline constexpr ull lsb_mask(ull\
    \ x) { assert(x != 0); return x & -x; }\ninline constexpr ull msb_mask(ull x)\
    \ { assert(x != 0); return bit_floor(x); }\n\ninline constexpr bool btest(ull\
    \ x, uint k) { return (x >> k) & 1; }\ntemplate <class T>\ninline void bset(T\
    \ &x, uint k, bool b = 1) { b ? x |= (1ULL << k) : x &= ~(1ULL << k); }\ntemplate\
    \ <class T>\ninline void bflip(T &x, uint k) { x ^= (1ULL << k); }\ninline constexpr\
    \ bool bsubset(ull x, ull y) { return (x & y) == x; }\ninline constexpr bool bsupset(ull\
    \ x, ull y) { return (x & y) == y; }\ninline constexpr ull bsetminus(ull x, ull\
    \ y) { return x & ~y; }\n#line 2 \"template/template_dump.hpp\"\n\n#line 4 \"\
    template/template_dump.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\
    \uFF08dump\uFF09\n * @docs docs/template/template_dump.md\n */\n\n#ifdef LOCAL\n\
    #include <cpp-dump.hpp> // https://github.com/philip82148/cpp-dump\nnamespace\
    \ cpp_dump::_detail\n{\n  inline string export_var(\n      const i128 &x, const\
    \ string &indent, size_t last_line_length,\n      size_t current_depth, bool fail_on_newline,\
    \ const export_command &command\n  ) {\n    return export_var(i128tos(x), indent,\
    \ last_line_length, current_depth, fail_on_newline, command);\n  }\n} // namespace\
    \ cpp_dump::_detail\n#define dump(...) cpp_dump(__VA_ARGS__)\nnamespace cp = cpp_dump;\n\
    CPP_DUMP_SET_OPTION_GLOBAL(log_label_func, cp::log_label::line());\nCPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count,\
    \ 1000);\n#define local(...) __VA_ARGS__\n#define oj(...)\n#define local_oj(a,\
    \ b) (a)\n#else\n#define dump(...)\n#define local(...)\n#define oj(...) __VA_ARGS__\n\
    #define local_oj(a, b) (b)\n#endif\n#line 2 \"template/template_algo.hpp\"\n\n\
    #ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n#endif\n\n#line 2 \"template/template_vector.hpp\"\
    \n\n#line 2 \"template/template_math.hpp\"\n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n\
    #endif\n#ifndef EPS\n#define EPS 1e-11\n#endif\n\n#line 12 \"template/template_math.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09\
    \n * @docs docs/template/template_math.md\n */\n\ntemplate <class T, class U>\n\
    inline bool chmin(T &a, U b) { return a > b ? a = b, true : false; }\ntemplate\
    \ <class T, class U>\ninline bool chmax(T &a, U b) { return a < b ? a = b, true\
    \ : false; }\n\ntemplate <class T = ll, class U, class V>\ninline constexpr T\
    \ divfloor(U a, V b) { return T(a) / T(b) - (T(a) % T(b) && (T(a) ^ T(b)) < 0);\
    \ }\ntemplate <class T = ll, class U, class V>\ninline constexpr T divceil(U a,\
    \ V b) { return T(a) / T(b) + (T(a) % T(b) && (T(a) ^ T(b)) >= 0); }\ntemplate\
    \ <class T = ll, class U, class V>\ninline constexpr T divround(U a, V b) { return\
    \ divfloor<T>(2 * T(a) + T(b), 2 * T(b)); }\ntemplate <class T = ll, class U,\
    \ class V>\ninline constexpr T safemod(U a, V b) { return T(a) - T(b) * divfloor<T>(a,\
    \ b); }\n\ntemplate <class T = ll, class U, class V>\nconstexpr T ipow(U a, V\
    \ b)\n{\n  assert(b >= 0);\n  if (b == 0)\n    return 1;\n  if (a == 0 || a ==\
    \ 1)\n    return a;\n  if (a < 0 && a == -1)\n    return b & 1 ? -1 : 1;\n\n \
    \ T res = 1, tmp = a;\n  while (true)\n  {\n    if (b & 1)\n      res *= tmp;\n\
    \    b >>= 1;\n    if (b == 0)\n      break;\n    tmp *= tmp;\n  }\n  return res;\n\
    }\ntemplate <class T = ll, class A, class B, class M>\nT mul_limited(A a, B b,\
    \ M m)\n{\n  assert(a >= 0 && b >= 0 && m >= 0);\n  if (b == 0)\n    return 0;\n\
    \  return T(a) > T(m) / T(b) ? T(m) : T(a) * T(b);\n}\ntemplate <class T = ll,\
    \ class A, class B>\nT mul_limited(A a, B b) { return mul_limited<T>(a, b, INF);\
    \ }\ntemplate <class T = ll, class A, class B, class M>\nT pow_limited(A a, B\
    \ b, M m)\n{\n  assert(a >= 0 && b >= 0 && m >= 0);\n  if (a <= 1 || b == 0)\n\
    \    return min(ipow<T>(a, b), T(m));\n  \n  T res = 1, tmp = a;\n  while (true)\n\
    \  {\n    if (b & 1)\n    {\n      if (res > T(m) / tmp)\n        return m;\n\
    \      res *= tmp;\n    }\n    b >>= 1;\n    if (b == 0)\n      break;\n    if\
    \ (tmp > T(m) / tmp)\n      return m;\n    tmp *= tmp;\n  }\n  return res;\n}\n\
    template <class T = ll, class A, class B>\nT pow_limited(A a, B b) { return pow_limited<T>(a,\
    \ b, INF); }\n\ntemplate <class T = ll, class A, class K>\nconstexpr T iroot(A\
    \ a, K k)\n{\n  assert(a >= 0 && k >= 1);\n  if (a <= 1 || k == 1)\n    return\
    \ a;\n  if (k == 2)\n  {\n    if constexpr (sizeof(T) > sizeof(ull))\n    {\n\
    \      if ((u128)a < ((u128)1 << 120))\n        return sqrtl(a);\n    }\n    else\n\
    \      return sqrtl(a);\n  }\n\n  auto isok = [&](T x) -> bool\n  {\n    if (x\
    \ == 0)\n      return true;\n    T res = 1, k2 = k;\n    while (true)\n    {\n\
    \      if (k2 & 1)\n      {\n        if (res > T(a) / x)\n          return false;\n\
    \        res *= x;\n      }\n      k2 >>= 1;\n      if (k2 == 0)\n        break;\n\
    \      if (x > T(a) / x)\n        return false;\n      x *= x;\n    }\n    return\
    \ res <= T(a);\n  };\n\n  T x = pow(a, 1.0 / k);\n  bool up = true;\n  while (!isok(x))\n\
    \    up = false, x--;\n  if (up)\n  {\n    while (x < numeric_limits<T>::max()\
    \ && isok(x + 1))\n      x++;\n  }\n  return x;\n}\n\n// https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html\n\
    template <class D = decltype(EPS), class A>\nint SGN(A a, const D &eps = EPS)\
    \ { return int(a > eps) - int(a < -eps); }\n\n// \u4F4D\u53D6\u308A\u8A18\u6570\
    \u6CD5\u3068\u540C\u3058\u9806\u756A\uFF08\u4E0B\u4F4D\u6841\u304C\u5F8C\u308D\
    \uFF09\n// 0 \u306B\u5BFE\u3057\u3066\u306F {0} \u304C\u8FD4\u308B\ntemplate <class\
    \ T = ll, class U, class V>\nvc<T> base_repr(U val, V base)\n{\n  assert(val >=\
    \ 0);\n  assert(base >= 2);\n  if (val == 0)\n    return {0};\n  vc<T> a;\n  while\
    \ (val > 0)\n  {\n    a.emplace_back(val % base);\n    val /= base;\n  }\n  reverse(a.begin(),\
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
    \ 'a') + (ai - 10));\n  return s;\n}\n#line 6 \"template/template_vector.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08vector\uFF09\n *\
    \ @docs docs/template/template_vector.md\n */\n\n#define ALL(a) (a).begin(), (a).end()\n\
    template <class T = ll, class V>\ninline T SZ(const V &x) { return x.size(); }\n\
    #define eb emplace_back\n\ntemplate <class F>\nauto gen_vec(const int &n, const\
    \ F &f)\n{\n  vc<decltype(f(0))> res(n);\n  repi(i, n) res[i] = f(i);\n  return\
    \ res;\n}\n\n// https://qiita.com/Chippppp/items/13150f5e0ea99f444d97#%E5%A4%9A%E6%AC%A1%E5%85%83vector%E7%94%9F%E6%88%90%E9%96%A2%E6%95%B0\n\
    template <class T, size_t d, size_t i = 0, class V>\nauto dvec(const V (&sz)[d],\
    \ const T &init)\n{\n  if constexpr (i < d)\n    return vc(sz[i], dvec<T, d, i\
    \ + 1>(sz, init));\n  else\n    return init;\n}\n\ntemplate <class T = ll>\nT\
    \ ctol(const char &c, const string &s)\n{\n  repi(i, SZ<int>(s)) if (s[i] == c)\
    \ return i;\n  return -1;\n}\ntemplate <class T = ll>\nvc<T> stov(const string\
    \ &s, const char &first)\n{\n  return gen_vec(SZ<int>(s), [&](int i) -> T\n  \
    \               { return s[i] - first; });\n}\ntemplate <class T = ll>\nvc<T>\
    \ stov(const string &s, const string &t)\n{\n  return gen_vec(SZ<int>(s), [&](int\
    \ i) -> T\n                 { return ctol(s[i], t); });\n}\n\ntemplate <class\
    \ T>\nvc<T> concat(const vvc<T> &vs)\n{\n  vc<T> res;\n  for (cauto &v : vs)\n\
    \    res.insert(res.end(), ALL(v));\n  return res;\n}\ntemplate <class T>\nvc<T>\
    \ concat(const vc<T> &v) { return v; }\ntemplate <class T, class... Ts>\nvc<T>\
    \ concat(vc<T> v, const vc<Ts> &...vs)\n{\n  (v.insert(v.end(), ALL(vs)), ...);\n\
    \  return v;\n}\n\ntemplate <class T, class I>\nT vecget(const vc<T> &v, I i,\
    \ const T &dflt_negative = -INF, const T &dflt_positive = INF)\n{\n  if (i < 0)\n\
    \    return dflt_negative;\n  if (i >= SZ<int>(v))\n    return dflt_positive;\n\
    \  return v[i];\n}\n#line 10 \"template/template_algo.hpp\"\n\n/**\n * @brief\
    \ \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30A2\u30EB\u30B4\u30EA\u30BA\u30E0\
    \uFF09\n * @docs docs/template/template_algo.md\n */\n\ntemplate <class V>\nauto\
    \ SUM(const V &v)\n{\n  typename V::value_type s{};\n  fec(vi : v) s += vi;\n\
    \  return s;\n}\ntemplate <class T, class V>\nT SUM(const V &v)\n{\n  T s{};\n\
    \  fec(vi : v) s += vi;\n  return s;\n}\ntemplate <class V>\nauto MAX(const V\
    \ &v) { return *max_element(ALL(v)); }\ntemplate <class V>\nauto MIN(const V &v)\
    \ { return *min_element(ALL(v)); }\ntemplate <class I = ll, class V>\nI ARGMAX(const\
    \ V &v) { return max_element(ALL(v)) - v.begin(); }\ntemplate <class I = ll, class\
    \ V>\nI ARGMIN(const V &v) { return min_element(ALL(v)) - v.begin(); }\n\ntemplate<class\
    \ T = ll, class V>\nT mex(const V &a)\n{\n  int n = a.size();\n  vector<bool>\
    \ exists(n, false);\n  repi(i, n) if (0 <= a[i] && a[i] < n) exists[a[i]] = true;\n\
    \  repi(x, n) if (!exists[x]) return x;\n  return n;\n}\n\ntemplate <class T =\
    \ ll>\nvc<T> permid(const int &n, const int &base_index = 0)\n{\n  vc<T> p(n);\n\
    \  repi(i, n) p[i] = i + base_index;\n  return p;\n}\ntemplate <class T>\nvc<T>\
    \ perminv(const vc<T> &p)\n{\n  if (p.empty())\n    return {};\n  const int n\
    \ = p.size();\n  vc<T> q(MAX(p) + 1);\n  repi(i, n) if (p[i] >= 0) q[p[i]] = i;\n\
    \  return q;\n}\n// a[p[i]] for all i\ntemplate <class T, class U>\nvc<T> permuted(const\
    \ vc<T> &a, const vc<U> &p)\n{\n  const int n = p.size();\n  vc<T> res(n);\n \
    \ repi(i, n)\n  {\n    assert(0 <= p[i] && p[i] < U(a.size()));\n    res[i] =\
    \ a[p[i]];\n  }\n  return res;\n}\n\ntemplate <class V>\nV reversed(const V &v)\
    \ { return V(v.rbegin(), v.rend()); }\n\n#if __cplusplus < 202002L\ntemplate <class\
    \ V, class... Args>\nV sorted(V v, Args&&... args)\n{\n  sort(ALL(v), forward<Args>(args)...);\n\
    \  return v;\n}\n#else\ntemplate <class V, class... Args>\nV sorted(V v, Args&&...\
    \ args)\n{\n  ranges::sort(v, forward<Args>(args)...);\n  return v;\n}\n#endif\n\
    \ntemplate <class V>\nvoid unique(V &v) { v.erase(std::unique(ALL(v)), v.end());\
    \ }\ntemplate <class V>\nV uniqued(V v) { unique(v); return v; }\n\ntemplate <class\
    \ V>\nvoid sortunique(V &v)\n{\n  sort(ALL(v));\n  unique(v);\n}\ntemplate <class\
    \ V>\nV sortuniqued(V v) { sortunique(v); return v; }\n\n// 01234 -> 12340\ntemplate\
    \ <class V, class U>\nvoid rotate(V &v, U k)\n{ \n  const U n = v.size();\n  k\
    \ = (k % n + n) % n;\n  std::rotate(v.begin(), v.begin() + k, v.end());\n}\n//\
    \ 01234 -> 12340\ntemplate <class V, class U>\nV rotated(V v, U k) { rotate(v,\
    \ k); return v; }\n\ntemplate <class T>\nvvc<T> top(const vvc<T> &a)\n{\n  if\
    \ (a.empty())\n    return {};\n  const int n = a.size(), m = a[0].size();\n  vvc<T>\
    \ b(m, vc<T>(n));\n  repi(i, n)\n  {\n    assert(SZ<int>(a[i]) == m);\n    repi(j,\
    \ m) b[j][i] = a[i][j];\n  }\n  return b;\n}\nvstr top(const vstr &a)\n{\n  vvc<char>\
    \ a_(a.size());\n  repi(i, SZ<int>(a)) a_[i] = {ALL(a[i])};\n  vvc<char> b_ =\
    \ top(a_);\n  vstr b(b_.size());\n  repi(i, SZ<int>(b)) b[i] = {ALL(b_[i])};\n\
    \  return b;\n}\n\n// 12\n// 34 -> 246\n// 56    135\n// (\u53CD\u6642\u8A08\u56DE\
    \u308A)\ntemplate <class VV, class U = ll>\nVV rot90(const VV &a, U k = 1)\n{\n\
    \  if (a.empty())\n    return {};\n  const int n = a.size(), m = a[0].size();\n\
    \  k = (k % 4 + 4) % 4;\n  if (k == 0)\n    return a;\n  else if (k == 1)\n  {\n\
    \    VV b(m);\n    repi(j, m) b[j].resize(n);\n    repi(i, n)\n    {\n      assert(SZ<int>(a[i])\
    \ == m);\n      repi(j, m) b[m - 1 - j][i] = a[i][j];\n    }\n    return b;\n\
    \  }\n  else if (k == 2)\n  {\n    VV b(n);\n    repi(i, n) b[i].resize(m);\n\
    \    repi(i, n)\n    {\n      assert(SZ<int>(a[i]) == m);\n      repi(j, m) b[n\
    \ - 1 - i][m - 1 - j] = a[i][j];\n    }\n    return b;\n  }\n  else\n  {\n   \
    \ VV b(m);\n    repi(j, m) b[j].resize(n);\n    repi(i, n)\n    {\n      assert(SZ<int>(a[i])\
    \ == m);\n      repi(j, m) b[j][n - 1 - i] = a[i][j];\n    }\n    return b;\n\
    \  }\n}\n\ntemplate <class T>\nstruct MonoidAdd\n{\n  using S = T;\n  static constexpr\
    \ S op(S a, S b) { return a + b; }\n  static constexpr S e() { return 0; }\n};\n\
    template <class T, const T infty = INF>\nstruct MonoidMin\n{\n  using S = T;\n\
    \  static constexpr S op(S a, S b) { return min(a, b); }\n  static constexpr S\
    \ e() { return infty; }\n};\ntemplate <class T, const T infty = INF>\nstruct MonoidMax\n\
    {\n  using S = T;\n  static constexpr S op(S a, S b) { return max(a, b); }\n \
    \ static constexpr S e() { return -infty; }\n};\n\n// left_index \u304C 0 \u306A\
    \u3089\u3001\u9577\u3055 n+1 \u3067 a.front() \u304C e()\n// left_index \u304C\
    \ 1 \u306A\u3089\u3001\u9577\u3055 n \u3067 e() \u304C\u306A\u3044\ntemplate <class\
    \ M>\nvc<typename M::S> cuml(const vc<typename M::S> &v, int left_index = 0)\n\
    {\n  const int n = v.size();\n  vc<typename M::S> res(n + 1);\n  res[0] = M::e();\n\
    \  repi(i, n) res[i + 1] = M::op(res[i], v[i]);\n  res.erase(res.begin(), res.begin()\
    \ + left_index);\n  return res;\n}\n// right_index \u304C 0 \u306A\u3089\u3001\
    \u9577\u3055 n+1 \u3067 a.back() \u304C e()\n// right_index \u304C 1 \u306A\u3089\
    \u3001\u9577\u3055 n \u3067 e() \u304C\u306A\u3044\ntemplate <class M>\nvc<typename\
    \ M::S> cumr(const vc<typename M::S> &v, int right_index = 0)\n{ return reversed(cuml<M>(reversed(v),\
    \ right_index)); }\ntemplate <class T>\nvc<T> cumlsum(const vc<T> &v, int left_index\
    \ = 0)\n{ return cuml<MonoidAdd<T>>(v, left_index); }\ntemplate <class T>\nvc<T>\
    \ cumrsum(const vc<T> &v, int right_index = 0)\n{ return cumr<MonoidAdd<T>>(v,\
    \ right_index); }\ntemplate <class T>\nvc<T> cumlmin(const vc<T> &v, int left_index\
    \ = 0)\n{ return cuml<MonoidMin<T>>(v, left_index); }\ntemplate <class T>\nvc<T>\
    \ cumrmin(const vc<T> &v, int right_index = 0)\n{ return cumr<MonoidMin<T>>(v,\
    \ right_index); }\ntemplate <class T>\nvc<T> cumlmax(const vc<T> &v, int left_index\
    \ = 0)\n{ return cuml<MonoidMax<T>>(v, left_index); }\ntemplate <class T>\nvc<T>\
    \ cumrmax(const vc<T> &v, int right_index = 0)\n{ return cumr<MonoidMax<T>>(v,\
    \ right_index); }\n\n// \u30C7\u30D5\u30A9\u30EB\u30C8\u3067\u306F\u9577\u3055\
    \ n+1\n// left_index, right_index \u3092\u305D\u308C\u305E\u308C 1 \u306B\u3059\
    \u308B\u3068\u3001\u5DE6\u53F3\u304C\u524A\u9664\u3055\u308C\u308B\ntemplate <class\
    \ T>\nvc<T> adjd(const vc<T> &v, int left_index = 0, int right_index = 0)\n{\n\
    \  int n = v.size();\n  vc<T> res(n + 1);\n  res[0] = v[0];\n  repi(i, 1, n) res[i]\
    \ = v[i] - v[i - 1];\n  res[n] = -v[n - 1];\n  res.erase(res.end() - right_index,\
    \ res.end());\n  res.erase(res.begin(), res.begin() + left_index);\n  return res;\n\
    }\n\nconst vpll DRULgrid = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};\nconst vpll DRULplane\
    \ = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};\n#line 6 \"verify/mytest/template_bit.test.cpp\"\
    \n\nvoid test1()\n{\n  vc<ll> width_model = {0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 40};\n\
    \  vc<ll> floor_model = {0, 1, 2, 2, 4, 4, 4, 4, 8, 8, 1LL << 39};\n  vc<ll> ceil_model\
    \ = {1, 1, 2, 4, 4, 8, 8, 8, 8, 16, 1LL << 40};\n  vc<ll> pp_model = {0, 1, 1,\
    \ 2, 1, 2, 2, 3, 1, 2, 20};\n  vc<ll> single_model = {0, 1, 1, 0, 1, 0, 0, 0,\
    \ 1, 0, 0};\n  vc<ll> width, floor, ceil, pp, single;\n  fec(k : vl{0, 1, 2, 3,\
    \ 4, 5, 6, 7, 8, 9, 0b1010101010101010101010101010101010101010LL})\n  {\n    width.push_back(bit_width(k));\n\
    \    floor.push_back(bit_floor(k));\n    ceil.push_back(bit_ceil(k));\n    pp.push_back(popcount(k));\n\
    \    single.push_back(has_single_bit(k));\n  }\n  assert(width == width_model);\n\
    \  assert(floor == floor_model);\n  assert(ceil == ceil_model);\n  assert(pp ==\
    \ pp_model);\n  assert(single == single_model);\n}\n\nvoid test2()\n{\n  vc<ll>\
    \ crz_model = {0, 1, 0, 2, 0, 1, 0, 3, 0, 1};\n  vc<ll> lp_model = {0, 1, 0, 2,\
    \ 0, 1, 0, 3, 0, 1};\n  vc<ll> lm_model = {1, 2, 1, 4, 1, 2, 1, 8, 1, 2};\n  vc<ll>\
    \ mp_model = {0, 1, 1, 2, 2, 2, 2, 3, 3, 39};\n  vc<ll> mm_model = {1, 2, 2, 4,\
    \ 4, 4, 4, 8, 8, 1LL << 39};\n  vc<ll> crz, lp, lm, mp, mm;\n  fec(k : vl{1, 2,\
    \ 3, 4, 5, 6, 7, 8, 9, 0b1010101010101010101010101010101010101010LL})\n  {\n \
    \   crz.push_back(countr_zero(k));\n    lp.push_back(lsb_pos(k));\n    mp.push_back(msb_pos(k));\n\
    \    lm.push_back(lsb_mask(k));\n    mm.push_back(msb_mask(k));\n  }\n  assert(crz\
    \ == crz_model);\n  assert(lp == lp_model);\n  assert(lm == lm_model);\n  assert(mp\
    \ == mp_model);\n  assert(mm == mm_model);\n}\n\nvoid test3()\n{\n  ll x = 0b1001;\n\
    \  assert(btest(x, 0));\n  assert(!btest(x, 1));\n  bset(x, 1);\n  assert(btest(x,\
    \ 1));\n  bset(x, 0, 0);\n  assert(!btest(x, 0));\n  ll y(x);\n  bflip(y, 2);\n\
    \  bflip(y, 3);\n  assert(btest(x, 2) != btest(y, 2));\n  assert(btest(x, 3) !=\
    \ btest(y, 3));\n}\n\nint main()\n{\n  test1();\n  test2();\n  test3();\n\n  cout\
    \ << \"Hello World\" << endl;\n}\n"
  code: "#define PROBLEM \"https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A\"\
    \n\n#include \"template/template_bit.hpp\"\n#include \"template/template_dump.hpp\"\
    \n#include \"template/template_algo.hpp\"\n\nvoid test1()\n{\n  vc<ll> width_model\
    \ = {0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 40};\n  vc<ll> floor_model = {0, 1, 2, 2, 4,\
    \ 4, 4, 4, 8, 8, 1LL << 39};\n  vc<ll> ceil_model = {1, 1, 2, 4, 4, 8, 8, 8, 8,\
    \ 16, 1LL << 40};\n  vc<ll> pp_model = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 20};\n \
    \ vc<ll> single_model = {0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0};\n  vc<ll> width, floor,\
    \ ceil, pp, single;\n  fec(k : vl{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0b1010101010101010101010101010101010101010LL})\n\
    \  {\n    width.push_back(bit_width(k));\n    floor.push_back(bit_floor(k));\n\
    \    ceil.push_back(bit_ceil(k));\n    pp.push_back(popcount(k));\n    single.push_back(has_single_bit(k));\n\
    \  }\n  assert(width == width_model);\n  assert(floor == floor_model);\n  assert(ceil\
    \ == ceil_model);\n  assert(pp == pp_model);\n  assert(single == single_model);\n\
    }\n\nvoid test2()\n{\n  vc<ll> crz_model = {0, 1, 0, 2, 0, 1, 0, 3, 0, 1};\n \
    \ vc<ll> lp_model = {0, 1, 0, 2, 0, 1, 0, 3, 0, 1};\n  vc<ll> lm_model = {1, 2,\
    \ 1, 4, 1, 2, 1, 8, 1, 2};\n  vc<ll> mp_model = {0, 1, 1, 2, 2, 2, 2, 3, 3, 39};\n\
    \  vc<ll> mm_model = {1, 2, 2, 4, 4, 4, 4, 8, 8, 1LL << 39};\n  vc<ll> crz, lp,\
    \ lm, mp, mm;\n  fec(k : vl{1, 2, 3, 4, 5, 6, 7, 8, 9, 0b1010101010101010101010101010101010101010LL})\n\
    \  {\n    crz.push_back(countr_zero(k));\n    lp.push_back(lsb_pos(k));\n    mp.push_back(msb_pos(k));\n\
    \    lm.push_back(lsb_mask(k));\n    mm.push_back(msb_mask(k));\n  }\n  assert(crz\
    \ == crz_model);\n  assert(lp == lp_model);\n  assert(lm == lm_model);\n  assert(mp\
    \ == mp_model);\n  assert(mm == mm_model);\n}\n\nvoid test3()\n{\n  ll x = 0b1001;\n\
    \  assert(btest(x, 0));\n  assert(!btest(x, 1));\n  bset(x, 1);\n  assert(btest(x,\
    \ 1));\n  bset(x, 0, 0);\n  assert(!btest(x, 0));\n  ll y(x);\n  bflip(y, 2);\n\
    \  bflip(y, 3);\n  assert(btest(x, 2) != btest(y, 2));\n  assert(btest(x, 3) !=\
    \ btest(y, 3));\n}\n\nint main()\n{\n  test1();\n  test2();\n  test3();\n\n  cout\
    \ << \"Hello World\" << endl;\n}"
  dependsOn:
  - template/template_bit.hpp
  - template/template_types.hpp
  - template/template_rep.hpp
  - template/template_dump.hpp
  - template/template_algo.hpp
  - template/template_vector.hpp
  - template/template_math.hpp
  isVerificationFile: true
  path: verify/mytest/template_bit.test.cpp
  requiredBy: []
  timestamp: '2025-04-10 02:46:07+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/mytest/template_bit.test.cpp
layout: document
redirect_from:
- /verify/verify/mytest/template_bit.test.cpp
- /verify/verify/mytest/template_bit.test.cpp.html
title: verify/mytest/template_bit.test.cpp
---
