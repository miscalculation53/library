---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: math/bigint.hpp
    title: "\u591A\u500D\u9577\u6574\u6570"
  - icon: ':heavy_check_mark:'
    path: math/convolution/convolution.hpp
    title: "\u7573\u307F\u8FBC\u307F"
  - icon: ':heavy_check_mark:'
    path: math/convolution/convolution_ll.hpp
    title: "\u7573\u307F\u8FBC\u307F\uFF08long long\uFF09"
  - icon: ':heavy_check_mark:'
    path: math/crt.hpp
    title: "\u4E2D\u56FD\u5270\u4F59\u5B9A\u7406 (CRT)"
  - icon: ':heavy_check_mark:'
    path: math/extgcd.hpp
    title: "\u62E1\u5F35\u30E6\u30FC\u30AF\u30EA\u30C3\u30C9\u4E92\u9664\u6CD5 (extgcd)"
  - icon: ':heavy_check_mark:'
    path: math/modint/modint.hpp
    title: math/modint/modint.hpp
  - icon: ':heavy_check_mark:'
    path: math/modint/modint_internal_barrett32.hpp
    title: math/modint/modint_internal_barrett32.hpp
  - icon: ':heavy_check_mark:'
    path: math/modint/modint_internal_isprime.hpp
    title: math/modint/modint_internal_isprime.hpp
  - icon: ':heavy_check_mark:'
    path: math/modint/modint_internal_montgomery64.hpp
    title: math/modint/modint_internal_montgomery64.hpp
  - icon: ':heavy_check_mark:'
    path: math/modint/modint_internal_static.hpp
    title: math/modint/modint_internal_static.hpp
  - icon: ':heavy_check_mark:'
    path: template/template_algo.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30A2\u30EB\u30B4\u30EA\u30BA\
      \u30E0\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_all_but_modint.hpp
    title: template/template_all_but_modint.hpp
  - icon: ':heavy_check_mark:'
    path: template/template_binsearch.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u4E8C\u5206\u63A2\u7D22\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_bit.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30D3\u30C3\u30C8\u6F14\u7B97\
      \uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_dump.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_inout.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_main.hpp
    title: template/template_main.hpp
  - icon: ':heavy_check_mark:'
    path: template/template_math.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_random.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30E9\u30F3\u30C0\u30E0\u751F\
      \u6210\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_rep.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_types.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_vector.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08vector\uFF09"
  - icon: ':heavy_check_mark:'
    path: utils/is_integral_ext.hpp
    title: "$128$ \u30D3\u30C3\u30C8\u6574\u6570\u3092\u542B\u3081\u305F\u6574\u6570\
      \u5224\u5B9A"
  - icon: ':heavy_check_mark:'
    path: utils/larger_int.hpp
    title: "$2$ \u500D\u306E\u30D3\u30C3\u30C8\u6570\u306E\u6574\u6570\u578B"
  - icon: ':heavy_check_mark:'
    path: utils/make_unsigned_ext.hpp
    title: "$128$ \u30D3\u30C3\u30C8\u6574\u6570\u306B\u5BFE\u5FDC\u3057\u305F make_singed\
      \ \u3068 make_unsigned"
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/division_of_hex_big_integers
    links:
    - https://judge.yosupo.jp/problem/division_of_hex_big_integers
  bundledCode: "#line 1 \"verify/yosupo/big_integer_division_hex.test.cpp\"\n#define\
    \ PROBLEM \"https://judge.yosupo.jp/problem/division_of_hex_big_integers\"\n\n\
    // #define SINGLE_TESTCASE\n#define MULTI_TESTCASE\n// #define AOJ_TESTCASE\n\n\
    #ifndef LOCAL\n#define FAST_IO\n// #define FAST_CIO\n// #define INTERACTIVE\n\
    #endif\n\n#define INF 4'000'000'000'000'000'037LL\n#define EPS 1e-11\n\n#line\
    \ 2 \"template/template_all_but_modint.hpp\"\n\n#line 2 \"template/template_types.hpp\"\
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
    #line 2 \"template/template_math.hpp\"\n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n\
    #endif\n#ifndef EPS\n#define EPS 1e-11\n#endif\n\n#line 12 \"template/template_math.hpp\"\
    \n\n#line 2 \"utils/is_integral_ext.hpp\"\n\n#line 4 \"utils/is_integral_ext.hpp\"\
    \n\n/**\n * @brief $128$ \u30D3\u30C3\u30C8\u6574\u6570\u3092\u542B\u3081\u305F\
    \u6574\u6570\u5224\u5B9A\n * @docs docs/utils/is_integral_ext.md\n */\n\ntemplate\
    \ <class T>\nconstexpr bool is_integral_ext = is_integral_v<T> || is_same_v<T,\
    \ i128> || is_same_v<T, u128>;\n\ntemplate <class T>\nconstexpr bool is_signed_ext\
    \ = is_signed_v<T> || is_same_v<T, i128>;\n\ntemplate <class T>\nconstexpr bool\
    \ is_unsigned_ext = is_signed_v<T> || is_same_v<T, u128>;\n#line 14 \"template/template_math.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09\
    \n * @docs docs/template/template_math.md\n */\n\ntemplate <class T, class U>\n\
    inline bool chmin(T &a, U b) { return a > b ? a = b, true : false; }\ntemplate\
    \ <class T, class U>\ninline bool chmax(T &a, U b) { return a < b ? a = b, true\
    \ : false; }\n\ntemplate <class T = ll, class U, class V, typename = enable_if_t<is_integral_ext<U>\
    \ && is_integral_ext<V>>>\ninline constexpr T divfloor(U a, V b) { return T(a)\
    \ / T(b) - (T(a) % T(b) && (T(a) ^ T(b)) < 0); }\ntemplate <class T = ll, class\
    \ U, class V, typename = enable_if_t<is_integral_ext<U> && is_integral_ext<V>>>\n\
    inline constexpr T divceil(U a, V b) { return T(a) / T(b) + (T(a) % T(b) && (T(a)\
    \ ^ T(b)) >= 0); }\ntemplate <class T = ll, class U, class V, typename = enable_if_t<is_integral_ext<U>\
    \ && is_integral_ext<V>>>\ninline constexpr T divround(U a, V b) { return divfloor<T>(2\
    \ * T(a) + T(b), 2 * T(b)); }\ntemplate <class T = ll, class U, class V, typename\
    \ = enable_if_t<is_integral_ext<U> && is_integral_ext<V>>>\ninline constexpr T\
    \ safemod(U a, V b) { return T(a) - T(b) * divfloor<T>(a, b); }\n\ntemplate <class\
    \ T = ll, class U, class V>\nconstexpr T ipow(U a, V b)\n{\n  assert(b >= 0);\n\
    \  if (b == 0)\n    return 1;\n  if (a == 0 || a == 1)\n    return a;\n  if (a\
    \ < 0 && a == -1)\n    return b & 1 ? -1 : 1;\n\n  T res = 1, tmp = a;\n  while\
    \ (true)\n  {\n    if (b & 1)\n      res *= tmp;\n    b >>= 1;\n    if (b == 0)\n\
    \      break;\n    tmp *= tmp;\n  }\n  return res;\n}\ntemplate <class T = ll,\
    \ class A, class B, class M>\nT mul_limited(A a, B b, M m)\n{\n  assert(a >= 0\
    \ && b >= 0 && m >= 0);\n  if (b == 0)\n    return 0;\n  return T(a) > T(m) /\
    \ T(b) ? T(m) : T(a) * T(b);\n}\ntemplate <class T = ll, class A, class B>\nT\
    \ mul_limited(A a, B b) { return mul_limited<T>(a, b, INF); }\ntemplate <class\
    \ T = ll, class A, class B, class M>\nT pow_limited(A a, B b, M m)\n{\n  assert(a\
    \ >= 0 && b >= 0 && m >= 0);\n  if (a <= 1 || b == 0)\n    return min(ipow<T>(a,\
    \ b), T(m));\n  \n  T res = 1, tmp = a;\n  while (true)\n  {\n    if (b & 1)\n\
    \    {\n      if (res > T(m) / tmp)\n        return m;\n      res *= tmp;\n  \
    \  }\n    b >>= 1;\n    if (b == 0)\n      break;\n    if (tmp > T(m) / tmp)\n\
    \      return m;\n    tmp *= tmp;\n  }\n  return res;\n}\ntemplate <class T =\
    \ ll, class A, class B>\nT pow_limited(A a, B b) { return pow_limited<T>(a, b,\
    \ INF); }\n\ntemplate <class T = ll, class A, class K>\nconstexpr T iroot(A a,\
    \ K k)\n{\n  assert(a >= 0 && k >= 1);\n  if (a <= 1 || k == 1)\n    return a;\n\
    \  if (k == 2)\n  {\n    if constexpr (sizeof(T) > sizeof(ull))\n    {\n     \
    \ if ((u128)a < ((u128)1 << 120))\n        return sqrtl(a);\n    }\n    else\n\
    \      return sqrtl(a);\n  }\n\n  auto isok = [&](T x) -> bool\n  {\n    if (x\
    \ == 0)\n      return true;\n    T res = 1, k2 = k;\n    while (true)\n    {\n\
    \      if (k2 & 1)\n      {\n        if (res > T(a) / x)\n          return false;\n\
    \        res *= x;\n      }\n      k2 >>= 1;\n      if (k2 == 0)\n        break;\n\
    \      if (x > T(a) / x)\n        return false;\n      x *= x;\n    }\n    return\
    \ res <= T(a);\n  };\n\n  T x = pow(a, 1.0 / k);\n  bool up = true;\n  while (!isok(x))\n\
    \    up = false, x--;\n  if (up)\n  {\n    while (x < numeric_limits<T>::max()\
    \ && isok(x + 1))\n      x++;\n  }\n  return x;\n}\ntemplate <class T = ll, class\
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
    \ 'a') + (ai - 10));\n  return s;\n}\n#line 2 \"template/template_vector.hpp\"\
    \n\n#line 6 \"template/template_vector.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\
    \u30EC\u30FC\u30C8\uFF08vector\uFF09\n * @docs docs/template/template_vector.md\n\
    \ */\n\n#define ALL(a) (a).begin(), (a).end()\ntemplate <class T = ll, class V>\n\
    inline T SZ(const V &x) { return x.size(); }\n#define eb emplace_back\n\n#define\
    \ LMD(x, fx) ([&](auto x) { return fx; })\ntemplate <class F>\nauto gen_vec(int\
    \ n, const F &f)\n{\n  vc<decltype(f(0))> res(n);\n  repi(i, n) res[i] = f(i);\n\
    \  return res;\n}\n#define GEN_VEC(n, i, fi) (gen_vec(n, LMD(i, fi)))\n\n// https://qiita.com/Chippppp/items/13150f5e0ea99f444d97#%E5%A4%9A%E6%AC%A1%E5%85%83vector%E7%94%9F%E6%88%90%E9%96%A2%E6%95%B0\n\
    template <class T, size_t d, size_t i = 0, class V>\nauto dvec(const V (&sz)[d],\
    \ const T &init)\n{\n  if constexpr (i < d)\n    return vc(sz[i], dvec<T, d, i\
    \ + 1>(sz, init));\n  else\n    return init;\n}\n\ntemplate <class T = ll>\nT\
    \ ctol(const char &c, const string &s)\n{\n  repi(i, SZ<int>(s)) if (s[i] == c)\
    \ return i;\n  return -1;\n}\ntemplate <class T = ll>\nvc<T> stov(const string\
    \ &s, char first)\n{\n  return gen_vec(SZ<int>(s), [&](int i) -> T\n         \
    \        { return s[i] - first; });\n}\ntemplate <class T = ll>\nvc<T> stov(const\
    \ string &s, const string &t)\n{\n  return gen_vec(SZ<int>(s), [&](int i) -> T\n\
    \                 { return ctol(s[i], t); });\n}\ntemplate <class T>\nstring vtos(const\
    \ vc<T> &v, char first)\n{\n  string res = \"\";\n  fe(vi : v) res += vi + first;\n\
    \  return res;\n}\ntemplate <class T>\nstring vtos(const vc<T> &v, const string\
    \ &t)\n{\n  string res = \"\";\n  fe(vi : v) res += t[vi];\n  return res;\n}\n\
    \ntemplate <class T>\nvc<T> concat(const vvc<T> &vs)\n{\n  vc<T> res;\n  for (cauto\
    \ &v : vs)\n    res.insert(res.end(), ALL(v));\n  return res;\n}\ntemplate <class\
    \ T>\nvc<T> concat(const vc<T> &v) { return v; }\ntemplate <class T, class...\
    \ Ts>\nvc<T> concat(vc<T> v, const vc<Ts> &...vs)\n{\n  (v.insert(v.end(), ALL(vs)),\
    \ ...);\n  return v;\n}\n\ntemplate <class T>\nvc<T> merged(const vc<T> &a, const\
    \ vc<T> &b)\n{\n  vc<T> res;\n  merge(ALL(a), ALL(b), back_inserter(res));\n \
    \ return res;\n}\n\ntemplate <class T, class I>\nT vecget(const vc<T> &v, I i,\
    \ const T &dflt_negative = -INF, const T &dflt_positive = INF)\n{\n  if (i < 0)\n\
    \    return dflt_negative;\n  if (i >= SZ<int>(v))\n    return dflt_positive;\n\
    \  return v[i];\n}\n#line 2 \"template/template_algo.hpp\"\n\n#ifndef INF\n#define\
    \ INF 4'000'000'000'000'000'037LL\n#endif\n\n#line 10 \"template/template_algo.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30A2\u30EB\u30B4\
    \u30EA\u30BA\u30E0\uFF09\n * @docs docs/template/template_algo.md\n */\n\ntemplate\
    \ <class V>\nauto SUM(const V &v)\n{\n  typename V::value_type s{};\n  fec(vi\
    \ : v) s += vi;\n  return s;\n}\ntemplate <class T, class V>\nT SUM(const V &v)\n\
    {\n  T s{};\n  fec(vi : v) s += vi;\n  return s;\n}\ntemplate <class V>\nauto\
    \ MAX(const V &v) { return *max_element(ALL(v)); }\ntemplate <class V>\nauto MIN(const\
    \ V &v) { return *min_element(ALL(v)); }\ntemplate <class I = ll, class V>\nI\
    \ ARGMAX(const V &v) { return max_element(ALL(v)) - v.begin(); }\ntemplate <class\
    \ I = ll, class V>\nI ARGMIN(const V &v) { return min_element(ALL(v)) - v.begin();\
    \ }\n\ntemplate<class T = ll, class V>\nT mex(const V &a)\n{\n  int n = a.size();\n\
    \  vector<bool> exists(n, false);\n  repi(i, n) if (0 <= a[i] && a[i] < n) exists[a[i]]\
    \ = true;\n  repi(x, n) if (!exists[x]) return x;\n  return n;\n}\n\n// (0, 1.\
    \ ..., n-1) \u306E\u9806\u5217\u304B\u5224\u5B9A\ntemplate <class I>\nbool is_permutation(const\
    \ vc<I> &p)\n{\n  const int n = p.size();\n  vc<bool> b(n, false);\n  repi(i,\
    \ n)\n  {\n    if (!(0 <= p[i] && p[i] < n))\n      return false;\n    b[p[i]]\
    \ = true;\n  }\n  return all_of(ALL(b), [](bool bi)\n                { return\
    \ bi; });\n}\n\ntemplate <class T = ll>\nvc<T> permid(const int &n, const int\
    \ &base_index = 0)\n{\n  vc<T> p(n);\n  repi(i, n) p[i] = i + base_index;\n  return\
    \ p;\n}\ntemplate <class T>\nvc<T> perminv(const vc<T> &p)\n{\n  if (p.empty())\n\
    \    return {};\n  const int n = p.size();\n  vc<T> q(MAX(p) + 1);\n  repi(i,\
    \ n) if (p[i] >= 0) q[p[i]] = i;\n  return q;\n}\n// a[p[i]] for all i\ntemplate\
    \ <class T, class U>\nvc<T> permuted(const vc<T> &a, const vc<U> &p)\n{\n  const\
    \ int n = p.size();\n  vc<T> res(n);\n  repi(i, n)\n  {\n    assert(0 <= p[i]\
    \ && p[i] < U(a.size()));\n    res[i] = a[p[i]];\n  }\n  return res;\n}\n// p[q[r[i]]]\
    \ for all i \u306A\u3069\ntemplate <class T, class U, class... Ts>\nvc<T> permuted(const\
    \ vc<T> &p, const vc<U> &q, const vc<Ts> &...rs)\n{\n  return permuted(permuted(p,\
    \ q), rs...);\n}\n\ntemplate <class V>\nV reversed(const V &v) { return V(v.rbegin(),\
    \ v.rend()); }\n\n#if __cplusplus < 202002L\ntemplate <class V, class... Args>\n\
    V sorted(V v, Args&&... args)\n{\n  sort(ALL(v), forward<Args>(args)...);\n  return\
    \ v;\n}\n#else\ntemplate <class V, class... Args>\nV sorted(V v, Args&&... args)\n\
    {\n  ranges::sort(v, forward<Args>(args)...);\n  return v;\n}\n#endif\n\ntemplate\
    \ <class V>\nvoid unique(V &v) { v.erase(std::unique(ALL(v)), v.end()); }\ntemplate\
    \ <class V>\nV uniqued(V v) { unique(v); return v; }\n\ntemplate <class V>\nvoid\
    \ sortunique(V &v)\n{\n  sort(ALL(v));\n  unique(v);\n}\ntemplate <class V>\n\
    V sortuniqued(V v) { sortunique(v); return v; }\n\n// \u5F15\u6570: vc<pair<T,\
    \ U>>\n// \u8FD4\u308A\u5024: vc<pair<T, vc<U>>\n// T \u3054\u3068\u306B U \u3092\
    \u307E\u3068\u3081\u305F\u3082\u306E\n// T \u306F\u6BD4\u8F03\u53EF\u80FD\u3067\
    \u3042\u308B\u5FC5\u8981\u304C\u3042\u308B\ntemplate <class T, class U>\nvc<pair<T,\
    \ vc<U>>> sortuniqued_group(vc<pair<T, U>> v)\n{\n  stable_sort(ALL(v), [&](cauto\
    \ &p1, cauto &p2)\n              { return p1.first < p2.first; });\n  vc<pair<T,\
    \ vc<U>>> res;\n  fec([x, y] : v)\n  {\n    if (res.empty() || res.back().first\
    \ != x)\n      res.eb(x, vc{y});\n    else\n      res.back().second.eb(y);\n \
    \ }\n  return res;\n}\n\n// 01234 -> 12340\ntemplate <class V, class U>\nvoid\
    \ rotate(V &v, U k)\n{ \n  const U n = v.size();\n  k = (k % n + n) % n;\n  std::rotate(v.begin(),\
    \ v.begin() + k, v.end());\n}\n// 01234 -> 12340\ntemplate <class V, class U>\n\
    V rotated(V v, U k) { rotate(v, k); return v; }\n\ntemplate <class T>\nvvc<T>\
    \ top(const vvc<T> &a)\n{\n  if (a.empty())\n    return {};\n  const int n = a.size(),\
    \ m = a[0].size();\n  vvc<T> b(m, vc<T>(n));\n  repi(i, n)\n  {\n    assert(SZ<int>(a[i])\
    \ == m);\n    repi(j, m) b[j][i] = a[i][j];\n  }\n  return b;\n}\nvstr top(const\
    \ vstr &a)\n{\n  vvc<char> a_(a.size());\n  repi(i, SZ<int>(a)) a_[i] = {ALL(a[i])};\n\
    \  vvc<char> b_ = top(a_);\n  vstr b(b_.size());\n  repi(i, SZ<int>(b)) b[i] =\
    \ {ALL(b_[i])};\n  return b;\n}\n\n// 12\n// 34 -> 246\n// 56    135\n// (\u53CD\
    \u6642\u8A08\u56DE\u308A)\ntemplate <class VV, class U = ll>\nVV rot90(const VV\
    \ &a, U k = 1)\n{\n  if (a.empty())\n    return {};\n  const int n = a.size(),\
    \ m = a[0].size();\n  k = (k % 4 + 4) % 4;\n  if (k == 0)\n    return a;\n  else\
    \ if (k == 1)\n  {\n    VV b(m);\n    repi(j, m) b[j].resize(n);\n    repi(i,\
    \ n)\n    {\n      assert(SZ<int>(a[i]) == m);\n      repi(j, m) b[m - 1 - j][i]\
    \ = a[i][j];\n    }\n    return b;\n  }\n  else if (k == 2)\n  {\n    VV b(n);\n\
    \    repi(i, n) b[i].resize(m);\n    repi(i, n)\n    {\n      assert(SZ<int>(a[i])\
    \ == m);\n      repi(j, m) b[n - 1 - i][m - 1 - j] = a[i][j];\n    }\n    return\
    \ b;\n  }\n  else\n  {\n    VV b(m);\n    repi(j, m) b[j].resize(n);\n    repi(i,\
    \ n)\n    {\n      assert(SZ<int>(a[i]) == m);\n      repi(j, m) b[j][n - 1 -\
    \ i] = a[i][j];\n    }\n    return b;\n  }\n}\n\ntemplate <class T>\nstruct MonoidAdd\n\
    {\n  using S = T;\n  static constexpr S op(S a, S b) { return a + b; }\n  static\
    \ constexpr S e() { return 0; }\n};\ntemplate <class T, const T infty = INF>\n\
    struct MonoidMin\n{\n  using S = T;\n  static constexpr S op(S a, S b) { return\
    \ min(a, b); }\n  static constexpr S e() { return infty; }\n};\ntemplate <class\
    \ T, const T infty = INF>\nstruct MonoidMax\n{\n  using S = T;\n  static constexpr\
    \ S op(S a, S b) { return max(a, b); }\n  static constexpr S e() { return -infty;\
    \ }\n};\n\n// left_index \u304C 0 \u306A\u3089\u3001\u9577\u3055 n+1 \u3067 a.front()\
    \ \u304C e()\n// left_index \u304C 1 \u306A\u3089\u3001\u9577\u3055 n \u3067 e()\
    \ \u304C\u306A\u3044\ntemplate <class M>\nvc<typename M::S> cuml(const vc<typename\
    \ M::S> &v, int left_index = 0)\n{\n  const int n = v.size();\n  vc<typename M::S>\
    \ res(n + 1);\n  res[0] = M::e();\n  repi(i, n) res[i + 1] = M::op(res[i], v[i]);\n\
    \  res.erase(res.begin(), res.begin() + left_index);\n  return res;\n}\n// right_index\
    \ \u304C 0 \u306A\u3089\u3001\u9577\u3055 n+1 \u3067 a.back() \u304C e()\n// right_index\
    \ \u304C 1 \u306A\u3089\u3001\u9577\u3055 n \u3067 e() \u304C\u306A\u3044\ntemplate\
    \ <class M>\nvc<typename M::S> cumr(const vc<typename M::S> &v, int right_index\
    \ = 0)\n{ return reversed(cuml<M>(reversed(v), right_index)); }\ntemplate <class\
    \ T>\nvc<T> cumlsum(const vc<T> &v, int left_index = 0)\n{ return cuml<MonoidAdd<T>>(v,\
    \ left_index); }\ntemplate <class T>\nvc<T> cumrsum(const vc<T> &v, int right_index\
    \ = 0)\n{ return cumr<MonoidAdd<T>>(v, right_index); }\ntemplate <class T>\nvc<T>\
    \ cumlmin(const vc<T> &v, int left_index = 0)\n{ return cuml<MonoidMin<T>>(v,\
    \ left_index); }\ntemplate <class T>\nvc<T> cumrmin(const vc<T> &v, int right_index\
    \ = 0)\n{ return cumr<MonoidMin<T>>(v, right_index); }\ntemplate <class T>\nvc<T>\
    \ cumlmax(const vc<T> &v, int left_index = 0)\n{ return cuml<MonoidMax<T>>(v,\
    \ left_index); }\ntemplate <class T>\nvc<T> cumrmax(const vc<T> &v, int right_index\
    \ = 0)\n{ return cumr<MonoidMax<T>>(v, right_index); }\n\n// \u30C7\u30D5\u30A9\
    \u30EB\u30C8\u3067\u306F\u9577\u3055 n+1\n// left_index, right_index \u3092\u305D\
    \u308C\u305E\u308C 1 \u306B\u3059\u308B\u3068\u3001\u5DE6\u53F3\u304C\u524A\u9664\
    \u3055\u308C\u308B\ntemplate <class T>\nvc<T> adjd(const vc<T> &v, int left_index\
    \ = 0, int right_index = 0)\n{\n  int n = v.size();\n  vc<T> res(n + 1);\n  res[0]\
    \ = v[0];\n  repi(i, 1, n) res[i] = v[i] - v[i - 1];\n  res[n] = -v[n - 1];\n\
    \  res.erase(res.end() - right_index, res.end());\n  res.erase(res.begin(), res.begin()\
    \ + left_index);\n  return res;\n}\n\nconst vpll DRULgrid = {{1, 0}, {0, 1}, {-1,\
    \ 0}, {0, -1}};\nconst vpll DRULplane = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};\n\
    #line 2 \"template/template_binsearch.hpp\"\n\n#line 6 \"template/template_binsearch.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u4E8C\u5206\u63A2\
    \u7D22\uFF09\n * @docs docs/template/template_binsearch.md\n */\n\ntemplate <class\
    \ T>\nstruct is_random_access_iterator\n{\n  static constexpr bool value = is_same_v<\n\
    \    typename iterator_traits<T>::iterator_category,\n    random_access_iterator_tag\n\
    \  >;\n};\ntemplate <class T>\nconstexpr bool is_random_access_iterator_v = is_random_access_iterator<T>::value;\n\
    \n// --- LB, UB ---\n\n#if __cplusplus < 202002L\nstruct identity\n{\n  template\
    \ <class T>\n  constexpr T &&operator()(T &&t) const noexcept\n  { return forward<T>(t);\
    \ }\n};\nnamespace internal\n{\n  template <class T = ll, class V, class Judge>\n\
    \  inline T bound_helper(const V &v, Judge judge)\n  {\n    int l = -1, r = v.size();\n\
    \    while (r - l > 1)\n    {\n      int m = (l + r) / 2;\n      if (judge(m))\n\
    \        l = m;\n      else\n        r = m;\n    }\n    return r;\n  }\n};\n//\
    \ val <= v[i] \u3068\u306A\u308B\u6700\u5C0F\u306E i (val \u672A\u6E80\u306E\u5024\
    \u306E\u500B\u6570)\ntemplate <class T = ll, class V, class Value, class Comp\
    \ = less<>, class Proj = identity>\ninline T LB(const V &v, const Value &val,\
    \ Comp comp = {}, Proj proj = {})\n{\n  return internal::bound_helper(v, [&](int\
    \ i) -> bool\n                                { return comp(proj(*(v.begin() +\
    \ i)), val); });\n}\n// val < v[i] \u3068\u306A\u308B\u6700\u5C0F\u306E i (val\
    \ \u4EE5\u4E0B\u306E\u5024\u306E\u500B\u6570)\ntemplate <class T = ll, class V,\
    \ class Value, class Comp = less<>, class Proj = identity>\ninline T UB(const\
    \ V &v, const Value &val, Comp comp = {}, Proj proj = {})\n{\n  return internal::bound_helper(v,\
    \ [&](int i) -> bool\n                                { return !comp(val, proj(*(v.begin()\
    \ + i))); });\n}\n#define DEFAULT_COMP less<>\n#else\n// val <= v[i] \u3068\u306A\
    \u308B\u6700\u5C0F\u306E i (val \u672A\u6E80\u306E\u5024\u306E\u500B\u6570)\n\
    template <class T = ll, class V, class Value, class Comp = ranges::less, class\
    \ Proj = identity>\ninline T LB(const V &v, const Value &val, Comp comp = {},\
    \ Proj proj = {})\n{ return ranges::lower_bound(v, val, comp, proj) - v.begin();\
    \ }\n// val < v[i] \u3068\u306A\u308B\u6700\u5C0F\u306E i (val \u4EE5\u4E0B\u306E\
    \u5024\u306E\u500B\u6570)\ntemplate <class T = ll, class V, class Value, class\
    \ Comp = ranges::less, class Proj = identity>\ninline T UB(const V &v, const Value\
    \ &val, Comp comp = {}, Proj proj = {})\n{ return ranges::upper_bound(v, val,\
    \ comp, proj) - v.begin(); }\n#define DEFAULT_COMP ranges::less\n#endif\n\n//\
    \ --- vector \u7B49\u306E lt, leq, gt, geq ---\n\n// v[i] < val \u3068\u306A\u308B\
    \u6700\u5927\u306E i (\u306A\u3051\u308C\u3070 -1)\ntemplate <class T = ll, class\
    \ V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>\ninline auto\
    \ lt_max(const V &v, const Value &val, Comp comp = {}, Proj proj = {})\n-> enable_if_t<is_random_access_iterator_v<typename\
    \ V::iterator>, T>\n{ return LB<T>(v, val, comp, proj) - 1; }\n// v[i] <= val\
    \ \u3068\u306A\u308B\u6700\u5927\u306E i (\u306A\u3051\u308C\u3070 -1)\ntemplate\
    \ <class T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj =\
    \ identity>\ninline auto leq_max(const V &v, const Value &val, Comp comp = {},\
    \ Proj proj = {})\n-> enable_if_t<is_random_access_iterator_v<typename V::iterator>,\
    \ T>\n{ return UB<T>(v, val, comp, proj) - 1; }\n// val < v[i] \u3068\u306A\u308B\
    \u6700\u5C0F\u306E i (\u306A\u3051\u308C\u3070 n)\ntemplate <class T = ll, class\
    \ V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>\ninline auto\
    \ gt_min(const V &v, const Value &val, Comp comp = {}, Proj proj = {})\n-> enable_if_t<is_random_access_iterator_v<typename\
    \ V::iterator>, T>\n{ return UB<T>(v, val, comp, proj); }\n// val <= v[i] \u3068\
    \u306A\u308B\u6700\u5C0F\u306E i (\u306A\u3051\u308C\u3070 n)\ntemplate <class\
    \ T = ll, class V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>\n\
    inline auto geq_min(const V &v, const Value &val, Comp comp = {}, Proj proj =\
    \ {})\n-> enable_if_t<is_random_access_iterator_v<typename V::iterator>, T>\n\
    { return LB<T>(v, val, comp, proj); }\n// v[i] < val \u3068\u306A\u308B i \u306E\
    \u500B\u6570\ntemplate <class T = ll, class V, class Value, class Comp = DEFAULT_COMP,\
    \ class Proj = identity>\ninline auto lt_cnt(const V &v, const Value &val, Comp\
    \ comp = {}, Proj proj = {})\n-> enable_if_t<is_random_access_iterator_v<typename\
    \ V::iterator>, T>\n{ return LB<T>(v, val, comp, proj); }\n// v[i] <= val \u3068\
    \u306A\u308B i \u306E\u500B\u6570\ntemplate <class T = ll, class V, class Value,\
    \ class Comp = DEFAULT_COMP, class Proj = identity>\ninline auto leq_cnt(const\
    \ V &v, const Value &val, Comp comp = {}, Proj proj = {})\n-> enable_if_t<is_random_access_iterator_v<typename\
    \ V::iterator>, T>\n{ return UB<T>(v, val, comp, proj); }\n// val < v[i] \u3068\
    \u306A\u308B i \u306E\u500B\u6570\ntemplate <class T = ll, class V, class Value,\
    \ class Comp = DEFAULT_COMP, class Proj = identity>\ninline auto gt_cnt(const\
    \ V &v, const Value &val, Comp comp = {}, Proj proj = {})\n-> enable_if_t<is_random_access_iterator_v<typename\
    \ V::iterator>, T>\n{ return SZ<T>(v) - UB<T>(v, val, comp, proj); }\n// val <=\
    \ v[i] \u3068\u306A\u308B i \u306E\u500B\u6570\ntemplate <class T = ll, class\
    \ V, class Value, class Comp = DEFAULT_COMP, class Proj = identity>\ninline auto\
    \ geq_cnt(const V &v, const Value &val, Comp comp = {}, Proj proj = {})\n-> enable_if_t<is_random_access_iterator_v<typename\
    \ V::iterator>, T>\n{ return SZ<T>(v) - LB<T>(v, val, comp, proj); }\n// l <=\
    \ v[i] < r \u3068\u306A\u308B i \u306E\u500B\u6570\ntemplate <class T = ll, class\
    \ V, class L, class R, class Comp = DEFAULT_COMP, class Proj = identity>\ninline\
    \ auto in_cnt(const V &v, L l, R r, Comp comp = {}, Proj proj = {})\n-> enable_if_t<is_random_access_iterator_v<typename\
    \ V::iterator>, T>\n{\n  if (l > r)\n    return 0;\n  return lt_cnt<T>(v, r, comp,\
    \ proj) - lt_cnt<T>(v, l, comp, proj);\n}\n\n// --- set \u7B49\u306E lt, leq,\
    \ gt, geq ---\n\n// *it < val \u3068\u306A\u308B\u6700\u5927\u306E it (\u306A\u3051\
    \u308C\u3070 end())\ntemplate <class V, class Value>\ninline auto lt_max(const\
    \ V &v, const Value &val)\n-> enable_if_t<!is_random_access_iterator_v<typename\
    \ V::iterator>, typename V::const_iterator>\n{\n  auto it = v.lower_bound(val);\n\
    \  return it == v.begin() ? v.end() : prev(it);\n}\n// *it <= val \u3068\u306A\
    \u308B\u6700\u5927\u306E it (\u306A\u3051\u308C\u3070 end())\ntemplate <class\
    \ V, class Value>\ninline auto leq_max(const V &v, const Value &val)\n-> enable_if_t<!is_random_access_iterator_v<typename\
    \ V::iterator>, typename V::const_iterator>\n{\n  auto it = v.upper_bound(val);\n\
    \  return it == v.begin() ? v.end() : prev(it);\n}\n// val < *it \u3068\u306A\u308B\
    \u6700\u5C0F\u306E it (\u306A\u3051\u308C\u3070 end())\ntemplate <class V, class\
    \ Value>\ninline auto gt_min(const V &v, const Value &val)\n-> enable_if_t<!is_random_access_iterator_v<typename\
    \ V::iterator>, typename V::const_iterator>\n{ return v.upper_bound(val); }\n\
    // val <= *it \u3068\u306A\u308B\u6700\u5C0F\u306E it (\u306A\u3051\u308C\u3070\
    \ end())\ntemplate <class V, class Value>\ninline auto geq_min(const V &v, const\
    \ Value &val)\n-> enable_if_t<!is_random_access_iterator_v<typename V::iterator>,\
    \ typename V::const_iterator>\n{ return v.lower_bound(val); }\n\n// --- \u81EA\
    \u4F5C\u4E8C\u5206\u63A2\u7D22 ---\n\n// (ok, ng)\ntemplate <class T = ll, class\
    \ Judge, class InitOk, class InitNg>\npair<T, T> binsearch(const Judge &judge,\
    \ InitOk init_ok, InitNg init_ng, bool check_ok = true, bool check_ng = true)\n\
    {\n  T ok(init_ok), ng(init_ng);\n  if (check_ok)\n    assert(judge(ok));\n  if\
    \ (check_ng)\n    assert(!judge(ng));\n  while (ok - ng != 1 && ng - ok != 1)\n\
    \  {\n    T mid = (ok & ng) + ((ok ^ ng) >> 1);\n    (judge(mid) ? ok : ng) =\
    \ mid;\n  }\n  return {ok, ng};\n}\ntemplate <class T = ld, class Judge, class\
    \ InitOk, class InitNg>\nT binsearch_real(const Judge &judge, InitOk init_ok,\
    \ InitNg init_ng, int iteration_count = 100, bool check_ok = true, bool check_ng\
    \ = true)\n{\n  T ok(init_ok), ng(init_ng);\n  if (check_ok)\n    assert(judge(ok));\n\
    \  if (check_ng)\n    assert(!judge(ng));\n  repi(_, iteration_count)\n  {\n \
    \   T mid = (ok + ng) / 2;\n    (judge(mid) ? ok : ng) = mid;\n  }\n  return ok;\n\
    }\n// (ok, ng)\ntemplate <class T = ll, class Judge, class InitVal>\npair<T, T>\
    \ expsearch(const Judge &judge, InitVal init_val, bool positive = true)\n{\n \
    \ T ok, ng;\n  if (judge(init_val))\n  {\n    ok = init_val, ng = init_val + (positive\
    \ ? 1 : -1);\n    for (int i = 1; judge(ng); i++)\n      ok = ng, ng = init_val\
    \ + (positive ? 1 : -1) * (T(1) << i);\n  }\n  else\n  {\n    ng = init_val, ok\
    \ = init_val + (positive ? 1 : -1);\n    for (int i = 1; !judge(ok); i++)\n  \
    \    ng = ok, ok = init_val + (positive ? 1 : -1) * (T(1) << i);\n  }\n  while\
    \ (ok - ng != 1 && ng - ok != 1)\n  {\n    T mid = (ok & ng) + ((ok ^ ng) >> 1);\n\
    \    (judge(mid) ? ok : ng) = mid;\n  }\n  return {ok, ng};\n}\n#line 2 \"template/template_bit.hpp\"\
    \n\n#line 5 \"template/template_bit.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\
    \u30EC\u30FC\u30C8\uFF08\u30D3\u30C3\u30C8\u6F14\u7B97\uFF09\n * @docs docs/template/template_bit.md\n\
    \ */\n\ntemplate <class T>\ninline constexpr ull pow2(T k) { return 1ULL << k;\
    \ }\ntemplate <class T>\ninline constexpr ull MASK(T k) { return (1ULL << k) -\
    \ 1ULL; }\n\n#if __cplusplus < 202002L\n// x == 0 \u306A\u3089\u3070 0\u3001\u305D\
    \u3046\u3067\u306A\u3051\u308C\u3070 1 + floor(log2(x))\n// 0, 1, 2, 2, 3, 3,\
    \ 3, 3, 4, 4, ... \ninline constexpr ull bit_width(ull x) { return x == 0 ? 0\
    \ : 64 - __builtin_clzll(x); }\n// 0, 1, 2, 2, 4, 4, 4, 4, 8, 8, ...\ninline constexpr\
    \ ull bit_floor(ull x) { return x == 0 ? 0ULL : 1ULL << (bit_width(x) - 1); }\n\
    // 1, 1, 2, 4, 4, 8, 8, 8, 8, 16, ...\ninline constexpr ull bit_ceil(ull x) {\
    \ return x == 0 ? 1ULL : 1ULL << bit_width(x - 1); }\ninline constexpr ull countr_zero(ull\
    \ x) { assert(x != 0); return __builtin_ctzll(x); }\ninline constexpr ull popcount(ull\
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
    \ y) { return x & ~y; }\n#line 2 \"template/template_inout.hpp\"\n\n#line 2 \"\
    template/template_dump.hpp\"\n\n#line 4 \"template/template_dump.hpp\"\n\n/**\n\
    \ * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09\n * @docs docs/template/template_dump.md\n\
    \ */\n\n#ifdef LOCAL\n#include <cpp-dump.hpp> // https://github.com/philip82148/cpp-dump\n\
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
    \ res;\n}\n#line 6 \"template/template_inout.hpp\"\n\n/**\n * @brief \u30C6\u30F3\
    \u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\uFF09\n * @docs docs/template/template_inout.md\n\
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
    \ ...) auto vt = zip(tuple{__VA_ARGS__})\n// ----------\n#line 2 \"template/template_random.hpp\"\
    \n\n#line 5 \"template/template_random.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\
    \u30EC\u30FC\u30C8\uFF08\u30E9\u30F3\u30C0\u30E0\u751F\u6210\uFF09\n * @docs docs/template/template_random.md\n\
    \ */\n\nmt19937_64 mt;\n\n// [l, r] \u304B\u3089\u7B49\u78BA\u7387\ntemplate <class\
    \ T = ll, class U1, class U2>\nT randint(U1 l, U2 r)\n{\n  assert(T(l) <= T(r));\n\
    \  return T(l) + mt() % (T(r) - T(l) + 1);\n}\n// [l, r) \u304B\u3089\u7B49\u78BA\
    \u7387\ntemplate <class T = ll, class U1, class U2>\nT randrange(U1 l, U2 r)\n\
    {\n  assert(T(l) < T(r));\n  return T(l) + mt() % (T(r) - T(l));\n}\n\n// [l,\
    \ r) \u304B\u3089\u76F8\u7570\u306A\u308B k \u500B\u3092\u9078\u3076\n// does_sort:\
    \ \u30BD\u30FC\u30C8\u3059\u308B\u304B\u3069\u3046\u304B\ntemplate <int k, bool\
    \ does_sort, class T = ll, class U1, class U2>\narray<T, k> random_sample_range_array(U1\
    \ l, U2 r)\n{\n  assert(T(r) - T(l) >= T(k));\n  array<T, k> res;\n  repi(i, k)\
    \ res[i] = randint<T>(T(l), T(r) - T(k));\n  sort(ALL(res));\n  repi(i, k) res[i]\
    \ += i;\n  if (!does_sort)\n    shuffle(ALL(res), mt);\n  return res;\n}\n// [l,\
    \ r) \u304B\u3089\u76F8\u7570\u306A\u308B k \u500B\u3092\u9078\u3076\n// does_sort:\
    \ \u30BD\u30FC\u30C8\u3059\u308B\u304B\u3069\u3046\u304B\ntemplate <bool does_sort,\
    \ class T = ll, class U1, class U2>\nvc<T> random_sample_range_vector(U1 l, U2\
    \ r, int k)\n{\n  assert(T(r) - T(l) >= T(k));\n  vc<T> res(k);\n  repi(i, k)\
    \ res[i] = randint<T>(T(l), T(r) - T(k));\n  sort(ALL(res));\n  repi(i, k) res[i]\
    \ += i;\n  if (!does_sort)\n    shuffle(ALL(res), mt);\n  return res;\n}\n#line\
    \ 17 \"verify/yosupo/big_integer_division_hex.test.cpp\"\n\n#line 2 \"math/bigint.hpp\"\
    \n\n#line 4 \"math/bigint.hpp\"\n\n#line 2 \"math/convolution/convolution_ll.hpp\"\
    \n\n#line 4 \"math/convolution/convolution_ll.hpp\"\n\n#line 2 \"math/convolution/convolution.hpp\"\
    \n\n#line 4 \"math/convolution/convolution.hpp\"\n\n#line 2 \"math/modint/modint.hpp\"\
    \n\n#line 4 \"math/modint/modint.hpp\"\n\n#line 2 \"math/modint/modint_internal_static.hpp\"\
    \n\n#line 4 \"math/modint/modint_internal_static.hpp\"\n\n#line 2 \"utils/larger_int.hpp\"\
    \n\n#line 4 \"utils/larger_int.hpp\"\n\n/**\n * @brief $2$ \u500D\u306E\u30D3\u30C3\
    \u30C8\u6570\u306E\u6574\u6570\u578B\n * @docs docs/utils/larger_int.md\n */\n\
    \ntemplate <class T>\nstruct larger_int;\n\n#define LARGER_INT(T, U) \\\n  template\
    \ <>            \\\n  struct larger_int<T>   \\\n  {                      \\\n\
    \    using type = U;      \\\n  };\n\nLARGER_INT(signed char, short)\nLARGER_INT(short,\
    \ int)\nLARGER_INT(int, long long)\nLARGER_INT(long, __int128_t)\nLARGER_INT(long\
    \ long, __int128_t)\n\nLARGER_INT(unsigned char, unsigned short)\nLARGER_INT(unsigned\
    \ short, unsigned int)\nLARGER_INT(unsigned int, unsigned long long)\nLARGER_INT(unsigned\
    \ long, __uint128_t)\nLARGER_INT(unsigned long long, __uint128_t)\n\n#undef LARGER_INT\n\
    \ntemplate <class T>\nusing larger_int_t = typename larger_int<T>::type;\n#line\
    \ 2 \"math/modint/modint_internal_isprime.hpp\"\n\n#line 4 \"math/modint/modint_internal_isprime.hpp\"\
    \n\n#line 6 \"math/modint/modint_internal_isprime.hpp\"\n\nnamespace internal\n\
    {\n\ntemplate <class T>\nconstexpr ll powmod_constexpr(ll x, ll n, T m)\n{\n \
    \ if (m == 1)\n    return 0;\n  using U = make_unsigned_t<T>;\n  using L = larger_int_t<U>;\n\
    \n  U r = 1, y = safemod(x, m);\n  while (n)\n  {\n    if (n & 1)\n      r = L(r)\
    \ * y % m;\n    y = L(y) * y % m;\n    n >>= 1;\n  }\n  return r;\n}\n\ntemplate\
    \ <class T>\nconstexpr bool isprime_constexpr(T n)\n{\n  if constexpr (sizeof(T)\
    \ > 4)\n  {\n    if (n <= INT_MAX)\n      return isprime_constexpr<int>(n);\n\
    \  }\n\n  if (n <= 1)\n    return false;\n  if (n == 2 || n == 7 || n == 61)\n\
    \    return true;\n  if (n % 2 == 0)\n    return false;\n\n  ll d = n - 1;\n \
    \ while (d % 2 == 0)\n    d /= 2;\n\n  using U = make_unsigned_t<T>;\n  using\
    \ L = larger_int_t<U>;\n\n  auto miller_rabin = [&](const auto &bases) constexpr\n\
    \  {\n    for (ll a : bases)\n    {\n      ll t = d, y = powmod_constexpr(a, t,\
    \ n);\n      while (t != n - 1 && y != 1 && y != n - 1)\n      {\n        y =\
    \ L(y) * y % n;\n        t <<= 1;\n      }\n      if (y != n - 1 && t % 2 == 0)\n\
    \        return false;\n    }\n    return true;\n  };\n\n  if constexpr(sizeof(T)\
    \ <= 4)\n  {\n    constexpr ll bases[3] = {2, 7, 61};\n    return miller_rabin(bases);\n\
    \  }\n  else\n  {\n    constexpr ll bases[7] = {2, 325, 9375, 28178, 450775, 9780504,\
    \ 1795265022};\n    return miller_rabin(bases);\n  }\n}\n\ntemplate <auto n>\n\
    constexpr bool isprime = isprime_constexpr(n);\n\n};\n#line 7 \"math/modint/modint_internal_static.hpp\"\
    \n\nnamespace internal\n{\n\ntemplate <auto M>\nstruct policy_static\n{\n  using\
    \ mod_type = decltype(M);\n  using value_type = make_unsigned_t<mod_type>;\n \
    \ using calc_type = larger_int_t<value_type>;\n\n  static constexpr bool is_prime\
    \ = isprime_constexpr(M);\n\n  static constexpr mod_type mod() { return M; }\n\
    \  static constexpr value_type umod() { return M; }\n\n  static constexpr value_type\
    \ init(value_type v) { return v; }\n  static constexpr mod_type val(value_type\
    \ v) { return v; }\n  static constexpr value_type mul(value_type a, value_type\
    \ b)\n  {\n    return (value_type)((calc_type(a) * b) % M);\n  }\n};\n\n};\n#line\
    \ 2 \"math/modint/modint_internal_barrett32.hpp\"\n\n#line 4 \"math/modint/modint_internal_barrett32.hpp\"\
    \n\nnamespace internal\n{\n\nstruct barrett32\n{\n  uint m;\n  ull im;\n  explicit\
    \ barrett32(uint m) : m(m), im((ull)(-1) / m + 1) {}\n  uint umod() const { return\
    \ m; }\n  uint mul(uint a, uint b) const\n  {\n    ull z = a;\n    z *= b;\n \
    \   ull x = ull((u128(z) * im) >> 64);\n    ull y = x * m;\n    return uint(z\
    \ - y + (z < y ? m : 0));\n  }\n};\n\ntemplate <int id>\nstruct policy_barrett32\n\
    {\n  using value_type = uint;\n  using calc_type = ull;\n  using mod_type = int;\n\
    \  \n  static constexpr bool is_prime = false;\n  static inline barrett32 reducer{998244353};\n\
    \  static void set_mod(mod_type m) { reducer = barrett32(m); }\n  static mod_type\
    \ mod() { return reducer.umod(); }\n  static value_type umod() { return reducer.umod();\
    \ }\n  static value_type init(value_type v) { return v; }\n  static mod_type val(value_type\
    \ v) { return v; }\n  static value_type mul(value_type a, value_type b) { return\
    \ reducer.mul(a, b); }\n};\n\n};\n#line 2 \"math/modint/modint_internal_montgomery64.hpp\"\
    \n\n#line 4 \"math/modint/modint_internal_montgomery64.hpp\"\n\n#line 6 \"math/modint/modint_internal_montgomery64.hpp\"\
    \n\nnamespace internal\n{\n\ninline constexpr ull inv64(ull a)\n{\n  ull x = a;\n\
    \  while (a * x != 1) x *= 2 - a * x;\n  return x;\n}\n\nstruct montgomery64odd\n\
    {\n  ull m, im, sq;\n  explicit montgomery64odd(ull m) : m(m), im(inv64(m)), sq(-u128(m)\
    \ % m) {}\n  ull umod() const { return m; }\n  ull reduce(u128 x) const\n  {\n\
    \    auto t = (x + u128(m) * (-im * ull(x))) >> 64;\n    if (t >= m) t -= m;\n\
    \    return (ull)t;\n  }\n  ull inv_reduce(i128 v) const { return reduce(u128(v\
    \ % m + m) * sq); }\n};\n\n// https://www.mathenachia.blog/even-mod-montgomery-impl/\n\
    struct montgomery64\n{\n  ull m, mx, imx, d, q;\n  uint b;\n  explicit montgomery64(ull\
    \ m) : m(m)\n  {\n    b = countr_zero(m), mx = m >> b; \n    imx = inv64(mx);\n\
    \    d = powmod_constexpr((mx + 1) / 2, b, mx); \n    u128 sq = -u128(mx) % mx;\
    \ \n    q = (1 + (((sq - 1) * d) << b)) % m;\n  }\n  ull umod() const { return\
    \ m; }\n  ull reduce(u128 x) const\n  {\n    ull p = x & MASK(b); \n    x = (x\
    \ >> b) + p * d;\n    ull y = p << (64 - b);\n    auto t = (x + u128(mx) * (imx\
    \ * (y - ull(x)))) >> (64 - b);\n    if (t >= m) { t -= m; if (t >= m) t -= m;\
    \ }\n    return (ull)t;\n  }\n  ull inv_reduce(i128 v) const { return reduce(u128(v\
    \ % m + m) * q); }\n};\n\ntemplate <int id>\nstruct policy_montgomery64_odd\n\
    {\n  using value_type = ull;\n  using calc_type = u128;\n  using mod_type = ll;\n\
    \n  static constexpr bool is_prime = false;\n  static inline montgomery64odd reducer{(1LL\
    \ << 61) - 1};\n  static void set_mod(mod_type m) { reducer = montgomery64odd(m);\
    \ }\n  static mod_type mod() { return reducer.umod(); }\n  static value_type umod()\
    \ { return reducer.umod(); }\n  static value_type init(value_type v) { return\
    \ reducer.inv_reduce(v); }\n  static mod_type val(value_type v) { return reducer.reduce(v);\
    \ }\n  static value_type mul(value_type a, value_type b) { return reducer.reduce((calc_type)a\
    \ * b); }\n};\n\ntemplate <int id>\nstruct policy_montgomery64\n{\n  using value_type\
    \ = ull;\n  using calc_type = u128;\n  using mod_type = ll;\n\n  static constexpr\
    \ bool is_prime = false;\n  static inline montgomery64 reducer{(1LL << 61) - 1};\n\
    \  static void set_mod(mod_type m) { reducer = montgomery64(m); }\n  static mod_type\
    \ mod() { return reducer.umod(); }\n  static value_type umod() { return reducer.umod();\
    \ }\n  static value_type init(value_type v) { return reducer.inv_reduce(v); }\n\
    \  static mod_type val(value_type v) { return reducer.reduce(v); }\n  static value_type\
    \ mul(value_type a, value_type b) { return reducer.reduce((calc_type)a * b); }\n\
    };\n\n};\n#line 2 \"math/extgcd.hpp\"\n\n#line 4 \"math/extgcd.hpp\"\n\n/**\n\
    \ * @brief \u62E1\u5F35\u30E6\u30FC\u30AF\u30EA\u30C3\u30C9\u4E92\u9664\u6CD5\
    \ (extgcd)\n * @docs docs/math/extgcd.md\n */\n\n// g == gcd(x, y) >= 0, ax +\
    \ by == g \u3092\u6E80\u305F\u3059 (g, x, y)\n// max(|x|, |y|) <= max(|a|, |b|)\n\
    template <class T = ll>\nconstexpr tuple<T, T, T> extgcd(T a, T b)\n{\n  if (a\
    \ == 0 && b == 0)\n    return {0, 0, 0};\n  \n  // a*x1 + b*y1 == z1  ...(1)\n\
    \  // a*x2 + b*y2 == z2  ...(2)\n  T x1 = 1, y1 = 0, z1 = a;\n  T x2 = 0, y2 =\
    \ 1, z2 = b;\n  while (z2 != 0)\n  {\n    // (1)' = (2)\n    // (2)' = (1) - q*(2)\n\
    \    T q = z1 / z2;\n    tie(x1, x2) = make_pair(x2, x1 - q * x2);\n    tie(y1,\
    \ y2) = make_pair(y2, y1 - q * y2);\n    tie(z1, z2) = make_pair(z2, z1 - q *\
    \ z2);\n  }\n  if (z1 < 0)\n    x1 = -x1, y1 = -y1, z1 = -z1;\n  return {z1, x1,\
    \ y1};\n}\n#line 10 \"math/modint/modint.hpp\"\n\nnamespace internal\n{\n\n  template\
    \ <class Policy>\n  struct modint_impl\n  {\n    using V = typename Policy::value_type;\n\
    \    using M = typename Policy::mod_type;\n    using mint = modint_impl;\n\n \
    \ private:\n    V _v;\n\n  public:\n    static constexpr M mod() { return Policy::mod();\
    \ }\n\n    template <class T = Policy>\n    static auto set_mod(M m) -> decltype(T::set_mod(m))\
    \ { return T::set_mod(m); }\n\n    static mint raw(V v)\n    {\n      mint x;\n\
    \      x._v = v;\n      return x;\n    }\n\n    modint_impl() : _v(0) {}\n\n \
    \   template <class T, typename = enable_if_t<is_integral_ext<T>>>\n    modint_impl(T\
    \ v)\n    {\n      V rem;\n      if constexpr (is_signed_ext<T>)\n      {\n  \
    \      using S = make_signed_t<V>;\n        S x = v % S(Policy::umod());\n   \
    \     if (x < 0)\n          x += Policy::umod();\n        rem = x;\n      }\n\
    \      else\n        rem = V(v % Policy::umod());\n      _v = Policy::init(rem);\n\
    \    };\n\n    M val() const { return Policy::val(_v); }\n\n    mint &operator+=(const\
    \ mint &rhs)\n    {\n      _v += rhs._v;\n      if (_v >= Policy::umod())\n  \
    \      _v -= Policy::umod();\n      return *this;\n    }\n    mint &operator-=(const\
    \ mint &rhs)\n    {\n      _v -= rhs._v;\n      if (_v >= Policy::umod())\n  \
    \      _v += Policy::umod();\n      return *this;\n    }\n    mint &operator*=(const\
    \ mint &rhs)\n    {\n      _v = Policy::mul(_v, rhs._v);\n      return *this;\n\
    \    }\n    mint &operator/=(const mint &rhs)\n    {\n      return *this *= rhs.inv();\n\
    \    }\n\n    mint &operator++()\n    {\n      _v++;\n      if (_v == Policy::umod())\n\
    \        _v = 0;\n      return *this;\n    }\n    mint &operator--()\n    {\n\
    \      if (_v == 0)\n        _v = Policy::umod();\n      _v--;\n      return *this;\n\
    \    }\n    mint operator++(int)\n    {\n      mint res = *this;\n      ++(*this);\n\
    \      return res;\n    }\n    mint operator--(int)\n    {\n      mint res = *this;\n\
    \      --(*this);\n      return res;\n    }\n    mint operator+() const { return\
    \ *this; }\n    mint operator-() const { return mint() - *this; }\n\n    template\
    \ <class T>\n    mint pow(T n) const\n    {\n      assert(n >= 0);\n      mint\
    \ x = *this, r = 1;\n      while (n)\n      {\n        if (n & 1)\n          r\
    \ *= x;\n        x *= x;\n        n >>= 1;\n      }\n      return r;\n    }\n\
    \    mint inv() const\n    {\n      if constexpr (Policy::is_prime)\n      {\n\
    \        return pow(mod() - 2);\n      }\n      else\n      {\n        auto [g,\
    \ x, y] = extgcd<M>(val(), mod());\n        assert(g == 1);\n        return mint(x);\n\
    \      }\n    }\n\n    friend mint operator+(const mint &lhs, const mint &rhs)\
    \ { return mint(lhs) += rhs; }\n    friend mint operator-(const mint &lhs, const\
    \ mint &rhs) { return mint(lhs) -= rhs; }\n    friend mint operator*(const mint\
    \ &lhs, const mint &rhs) { return mint(lhs) *= rhs; }\n    friend mint operator/(const\
    \ mint &lhs, const mint &rhs) { return mint(lhs) /= rhs; }\n    friend bool operator==(const\
    \ mint &lhs, const mint &rhs) { return lhs._v == rhs._v; }\n    friend bool operator!=(const\
    \ mint &lhs, const mint &rhs) { return lhs._v != rhs._v; }\n  \n#if defined LOCAL\
    \ or not defined FAST_IO\n    friend std::istream &operator>>(std::istream &is,\
    \ mint &x)\n    {\n      long long a;\n      is >> a;\n      x = a;\n      return\
    \ is;\n    }\n    friend std::ostream &operator<<(std::ostream &os, const mint\
    \ &x)\n    {\n      os << x.val();\n      return os;\n    }\n#else\n    friend\
    \ void rd1(mint &x)\n    {\n      long long a;\n      fastio::rd1(a);\n      x\
    \ = a;\n    }\n    friend void wt1(const mint &x)\n    {\n      fastio::wt1(x.val());\n\
    \    }\n#endif\n  };\n\n};\n\ntemplate <int mod>\nusing static_modint32 = internal::modint_impl<internal::policy_static<mod>>;\n\
    template <int id>\nusing dynamic_modint32 = internal::modint_impl<internal::policy_barrett32<id>>;\n\
    template <ll mod>\nusing static_modint64 = internal::modint_impl<internal::policy_static<mod>>;\n\
    template <int id>\nusing dynamic_modint64_odd = internal::modint_impl<internal::policy_montgomery64_odd<id>>;\n\
    template <int id>\nusing dynamic_modint64 = internal::modint_impl<internal::policy_montgomery64<id>>;\n\
    \nusing modint998244353 = static_modint32<998244353>;\nusing modint1000000007\
    \ = static_modint32<1000000007>;\nusing modint = dynamic_modint32<-1>;\nusing\
    \ modint61 = static_modint64<(1LL << 61) - 1>;\nusing modint64 = dynamic_modint64<-1>;\n\
    \ntemplate <class T>\nstruct is_modint : std::false_type\n{\n};\ntemplate <class\
    \ Policy>\nstruct is_modint<internal::modint_impl<Policy>> : std::true_type\n\
    {\n};\ntemplate <class T>\ninline constexpr bool is_modint_v = is_modint<T>::value;\n\
    \ntemplate <class T>\nstruct is_static_modint : false_type {};\ntemplate <int\
    \ m>\nstruct is_static_modint<static_modint32<m>> : true_type {};\ntemplate <ll\
    \ m>\nstruct is_static_modint<static_modint64<m>> : true_type {};\ntemplate <class\
    \ T>\ninline constexpr bool is_static_modint_v = is_static_modint<T>::value;\n\
    \ntemplate <class T>\nstruct is_dynamic_modint : false_type {};\ntemplate <int\
    \ id>\nstruct is_dynamic_modint<dynamic_modint32<id>> : true_type {};\ntemplate\
    \ <int id>\nstruct is_dynamic_modint<dynamic_modint64_odd<id>> : true_type {};\n\
    template <int id>\nstruct is_dynamic_modint<dynamic_modint64<id>> : true_type\
    \ {};\ntemplate <class T>\ninline constexpr bool is_dynamic_modint_v = is_dynamic_modint<T>::value;\n\
    \ntemplate <typename, typename = void>\nstruct has_mod : std::false_type\n{\n\
    };\ntemplate <typename T>\nstruct has_mod<T, std::void_t<decltype(T::mod())>>\
    \ : std::true_type\n{\n};\n\ntemplate <class mint>\nstruct modint_less\n{\n  bool\
    \ operator()(const mint &a, const mint &b) const\n  {\n    if constexpr (is_modint_v<mint>)\n\
    \      return a.val() < b.val();\n    else\n      return a < b;\n  }\n};\n\ntemplate\
    \ <class mint>\nstruct modint_hash\n{\n  auto operator()(const mint &x) const\n\
    \  {\n    if constexpr (is_modint_v<mint>)\n      return std::hash<decltype(x.val())>{}(x.val());\n\
    \    else\n      return std::hash<mint>{}(x);\n  }\n};\n#line 2 \"math/crt.hpp\"\
    \n\n#line 4 \"math/crt.hpp\"\n\n#line 8 \"math/crt.hpp\"\n\n/**\n * @brief \u4E2D\
    \u56FD\u5270\u4F59\u5B9A\u7406 (CRT)\n * @docs docs/math/crt.md\n */\n\n// (\u89E3\
    \u304C\u5B58\u5728\u3059\u308B\u304B, r, m)\ntemplate <class T = ll, class R0,\
    \ class R1, class M0, class M1>\nconstexpr tuple<bool, T, T> crt2(R0 r0_, R1 r1_,\
    \ M0 m0_, M1 m1_)\n{\n  T m0 = m0_, m1 = m1_;\n  assert(m0 >= 1 && m1 >= 1);\n\
    \  T r0 = safemod(r0_, m0), r1 = safemod(r1_, m1);\n  if (m0 < m1)\n    swap(r0,\
    \ r1), swap(m0, m1);\n  if (m0 % m1 == 0)\n  {\n    if (r0 % m1 != r1)\n     \
    \ return {false, 0, 0};\n    return {true, r0, m0};\n  }\n  auto [g, im, _] =\
    \ extgcd<T>(m0, m1);\n  T u1 = m1 / g;\n  if ((r1 - r0) % g)\n    return {false,\
    \ 0, 0};\n  T x = (r1 - r0) / g % u1 * im % u1;\n  r0 += x * m0;\n  m0 *= u1;\n\
    \  if (r0 < 0)\n    r0 += m0;\n  return {true, r0, m0};\n}\n\n// (\u89E3\u304C\
    \u5B58\u5728\u3059\u308B\u304B, r, m)\ntemplate <class T = ll, class V1, class\
    \ V2>\nconstexpr tuple<bool, T, T> crt(const V1 &rs, const V2 &ms)\n{\n  assert(rs.size()\
    \ == ms.size());\n  const int n = rs.size();\n  T r = 0, m = 1;\n  repi(i, n)\n\
    \  {\n    auto [ok, nr, nm] = crt2<T>(r, rs[i], m, ms[i]);\n    if (!ok)\n   \
    \   return {false, 0, 0};\n    r = nr, m = nm;\n  }\n  return {true, r, m};\n\
    }\n\n// (r, m)\n// ms[i] \u305F\u3061\u306F pairwise coprime\ntemplate <class\
    \ mint, class V1, class V2>\npair<mint, mint> crt_mod(const V1 &rs, const V2 &ms)\n\
    {\n  using T = decay_t<decltype(ms[0])>;\n  assert(rs.size() == ms.size());\n\
    \  const int n = rs.size();\n  mint r = 0, m = 1;\n\n  if constexpr (sizeof(T)\
    \ <= 4)\n  {\n    vc<internal::barrett32> ba;\n    ba.reserve(n);\n    repi(i,\
    \ n) ba.eb(ms[i]);\n    vc<uint> rr(n, 0), mm(n, 1);\n    repi(i, n)\n    {\n\
    \      assert(ms[i] >= 1);\n      auto [g, im, _] = extgcd<ll>(mm[i], ms[i]);\n\
    \      assert(g == 1);\n      if (im < 0)\n        im += ms[i];\n      ll diff\
    \ = safemod(ll(rs[i]) - rr[i], ms[i]);\n      uint t = ba[i].mul(diff, im);\n\
    \      r += t * m, m *= ms[i];\n      repi(j, i + 1, n)\n      {\n        rr[j]\
    \ += ba[j].mul(t, mm[j]);\n        if (rr[j] >= (uint)ms[j])\n          rr[j]\
    \ -= ms[j];\n        mm[j] = ba[j].mul(mm[j], ms[i]);\n      }\n    }\n  }\n \
    \ else\n  {\n    vc<ull> rr(n, 0), mm(n, 1);\n    repi(i, n)\n    {\n      assert(ms[i]\
    \ >= 1);\n      auto [g, im, _] = extgcd<ll>(mm[i], ms[i]);\n      assert(g ==\
    \ 1);\n      if (im < 0)\n        im += ms[i];\n      i128 diff = safemod((i128)rs[i]\
    \ - rr[i], ms[i]);\n      ull t = (ull)((u128)diff * im % ms[i]);\n      r +=\
    \ t * m, m *= ms[i];\n      repi(j, i + 1, n)\n      {\n        rr[j] += (ull)((u128)t\
    \ * mm[j] % ms[j]);\n        if (rr[j] >= (ull)ms[j]) rr[j] -= ms[j];\n      \
    \  mm[j] = (ull)((u128)mm[j] * ms[i] % ms[j]);\n      }\n    }\n  }\n  return\
    \ {r, m};\n}\n\n// (r, m)\n// \u5F15\u6570\u306F array\n// ms[i] \u305F\u3061\u304C\
    \u30B3\u30F3\u30D1\u30A4\u30EB\u6642\u5B9A\u6570\u3067\u3042\u308B\u3053\u3068\
    \u3092\u4EEE\u5B9A\n// ms[i] \u305F\u3061\u306F pairwise coprime\n// 0 <= rs[i]\
    \ < ms[i]\ntemplate <class mint, class U1, class U2, size_t n>\nconstexpr pair<mint,\
    \ mint> crt_mod_constexpr(const array<U1, n> &rs, const array<U2, n> &ms)\n{\n\
    \  using T = larger_int_t<U2>;\n  assert(rs.size() == ms.size());\n  mint r =\
    \ 0, m = 1;\n  array<T, n> rr{}, mm;\n  fill(ALL(mm), 1);\n  repi(i, n)\n  {\n\
    \    assert(ms[i] >= U2(1));\n    assert(U1(0) <= rs[i] && U2(rs[i]) < ms[i]);\n\
    \    auto [g, im, _] = extgcd<T>(mm[i], ms[i]);\n    assert(g == 1);\n    T t\
    \ = safemod((rs[i] - rr[i]) * im, ms[i]);\n    r += t * m, m *= ms[i];\n    repi(j,\
    \ i + 1, n)\n    {\n      rr[j] += t * mm[j] % ms[j];\n      if (rr[j] >= ms[j])\n\
    \        rr[j] -= ms[j];\n      mm[j] *= ms[i], mm[j] %= ms[j];\n    }\n  }\n\
    \  return {r, m};\n}\n\n// \u7834\u58CA\u7684\u306B\u5909\u66F4\u3059\u308B\n\
    // \u89E3\u304C\u5B58\u5728\u3057\u306A\u3044\u306A\u3089 false \u3092\u8FD4\u3059\
    \n// \u89E3\u304C\u5B58\u5728\u3059\u308B\u306A\u3089 true \u3092\u8FD4\u3057\u3001\
    ms[i] \u305F\u3061\u304C pairwise coprime \u3067\u3042\u308B\u3088\u3046\u306A\
    \u7B49\u4FA1\u306A\u65B9\u7A0B\u5F0F\u306B\u5909\u63DB\u3059\u308B\ntemplate <class\
    \ V1, class V2>\nbool pre_crt(const V1 &rs, V2 &ms)\n{\n  using T = typename V2::value_type;\n\
    \  assert(rs.size() == ms.size());\n  const int n = rs.size();\n  repi(i, n) repi(j,\
    \ i + 1, n)\n  {\n    T g = gcd(ms[i], ms[j]);\n    if ((rs[i] - rs[j]) % g)\n\
    \      return false;\n    ms[i] /= g, ms[j] /= g;\n    T gi = gcd(ms[i], g), gj\
    \ = g / gi;\n    do\n    {\n      g = gcd(gi, gj);\n      gi *= g, gj /= g;\n\
    \    } while (g > 1);\n    ms[i] *= gi, ms[j] *= gj;\n  }\n  return true;\n}\n\
    #line 7 \"math/convolution/convolution.hpp\"\n\n/**\n * @brief \u7573\u307F\u8FBC\
    \u307F\n * @docs docs/math/convolution/convolution.md\n */\n\ntemplate <class\
    \ T>\nT convolution_point_get(const vc<T> &a, const vc<T> &b, int p)\n{\n  const\
    \ int n = a.size(), m = b.size();\n  T res = 0;\n  repi(i, max(0, p - m + 1),\
    \ min(n, p + 1)) res += a[i] * b[p - i];\n  return res;\n}\n\nnamespace internal\n\
    {\n\nconstexpr int primitive_root_constexpr(int m)\n{\n  if (m == 2)\n    return\
    \ 1;\n  if (m == 167772161)\n    return 3;\n  if (m == 469762049)\n    return\
    \ 3;\n  if (m == 754974721)\n    return 11;\n  if (m == 998244353)\n    return\
    \ 3;\n  if (m == 1107296257)\n    return 10;\n  if (m == 1711276033)\n    return\
    \ 29;\n  if (m == 1811939329)\n    return 13;\n  if (m == 2013265921)\n    return\
    \ 31;\n  if (m == 2113929217)\n    return 5;\n  int divs[20] = {};\n  divs[0]\
    \ = 2;\n  int cnt = 1;\n  int x = (m - 1) / 2;\n  while (x % 2 == 0)\n    x /=\
    \ 2;\n  for (int i = 3; (long long)(i)*i <= x; i += 2)\n  {\n    if (x % i ==\
    \ 0)\n    {\n      divs[cnt++] = i;\n      while (x % i == 0)\n      {\n     \
    \   x /= i;\n      }\n    }\n  }\n  if (x > 1)\n  {\n    divs[cnt++] = x;\n  }\n\
    \  for (int g = 2;; g++)\n  {\n    bool ok = true;\n    for (int i = 0; i < cnt;\
    \ i++)\n    {\n      if (powmod_constexpr(g, (m - 1) / divs[i], m) == 1)\n   \
    \   {\n        ok = false;\n        break;\n      }\n    }\n    if (ok)\n    \
    \  return g;\n  }\n}\ntemplate <int m>\nconstexpr int primitive_root_for_convolution\
    \ = primitive_root_constexpr(m);\n\ntemplate <class mint, int g = internal::primitive_root_for_convolution<mint::mod()>>\n\
    struct fft_info\n{\n  static constexpr int rank2 = countr_zero(mint::mod() - 1);\n\
    \  std::array<mint, rank2 + 1> root;  // root[i]^(2^i) == 1\n  std::array<mint,\
    \ rank2 + 1> iroot; // root[i] * iroot[i] == 1\n\n  std::array<mint, std::max(0,\
    \ rank2 - 2 + 1)> rate2;\n  std::array<mint, std::max(0, rank2 - 2 + 1)> irate2;\n\
    \n  std::array<mint, std::max(0, rank2 - 3 + 1)> rate3;\n  std::array<mint, std::max(0,\
    \ rank2 - 3 + 1)> irate3;\n\n  fft_info()\n  {\n    root[rank2] = mint(g).pow((mint::mod()\
    \ - 1) >> rank2);\n    iroot[rank2] = root[rank2].inv();\n    for (int i = rank2\
    \ - 1; i >= 0; i--)\n    {\n      root[i] = root[i + 1] * root[i + 1];\n     \
    \ iroot[i] = iroot[i + 1] * iroot[i + 1];\n    }\n\n    {\n      mint prod = 1,\
    \ iprod = 1;\n      for (int i = 0; i <= rank2 - 2; i++)\n      {\n        rate2[i]\
    \ = root[i + 2] * prod;\n        irate2[i] = iroot[i + 2] * iprod;\n        prod\
    \ *= iroot[i + 2];\n        iprod *= root[i + 2];\n      }\n    }\n    {\n   \
    \   mint prod = 1, iprod = 1;\n      for (int i = 0; i <= rank2 - 3; i++)\n  \
    \    {\n        rate3[i] = root[i + 3] * prod;\n        irate3[i] = iroot[i +\
    \ 3] * iprod;\n        prod *= iroot[i + 3];\n        iprod *= root[i + 3];\n\
    \      }\n    }\n  }\n};\n\n}  // namespace internal\n\n// mint \u3067\u9577\u3055\
    \ n \u306E\u5217\u306E ntt \u304C\u3067\u304D\u308B\u304B\u5224\u5B9A\ntemplate\
    \ <class mint>\nbool ntt_ok(int n)\n{\n  if constexpr (is_static_modint_v<mint>)\n\
    \  {\n    if constexpr (!internal::isprime<mint::mod()>)\n      return false;\n\
    \    static constexpr int rank2 = countr_zero(mint::mod() - 1);\n    return n\
    \ <= (1 << rank2);\n  }\n  else\n    return false;\n}\n\ntemplate <int id>\nvoid\
    \ ntt(vc<dynamic_modint32<id>> &) { assert(false); }\ntemplate <int id>\nvoid\
    \ intt(vc<dynamic_modint32<id>> &) { assert(false); }\n\n// \u7834\u58CA\u7684\
    \u306B\u5909\u66F4\u3059\u308B\ntemplate <int mod>\nvoid ntt(vc<static_modint32<mod>>\
    \ &a)\n{\n  using mint = static_modint32<mod>;\n  int n = int(a.size());\n  int\
    \ h = countr_zero((unsigned int)n);\n  assert(n == (1 << h));\n\n  static const\
    \ internal::fft_info<mint> info;\n\n  int len = 0; // a[i, i+(n>>len), i+2*(n>>len),\
    \ ..] is transformed\n  while (len < h)\n  {\n    if (h - len == 1)\n    {\n \
    \     int p = 1 << (h - len - 1);\n      mint rot = 1;\n      for (int s = 0;\
    \ s < (1 << len); s++)\n      {\n        int offset = s << (h - len);\n      \
    \  for (int i = 0; i < p; i++)\n        {\n          auto l = a[i + offset];\n\
    \          auto r = a[i + offset + p] * rot;\n          a[i + offset] = l + r;\n\
    \          a[i + offset + p] = l - r;\n        }\n        if (s + 1 != (1 << len))\n\
    \          rot *= info.rate2[countr_zero(~(unsigned int)(s))];\n      }\n    \
    \  len++;\n    }\n    else\n    {\n      // 4-base\n      int p = 1 << (h - len\
    \ - 2);\n      mint rot = 1, imag = info.root[2];\n      for (int s = 0; s < (1\
    \ << len); s++)\n      {\n        mint rot2 = rot * rot;\n        mint rot3 =\
    \ rot2 * rot;\n        int offset = s << (h - len);\n        for (int i = 0; i\
    \ < p; i++)\n        {\n          auto mod2 = 1ULL * mint::mod() * mint::mod();\n\
    \          auto a0 = 1ULL * a[i + offset].val();\n          auto a1 = 1ULL * a[i\
    \ + offset + p].val() * rot.val();\n          auto a2 = 1ULL * a[i + offset +\
    \ 2 * p].val() * rot2.val();\n          auto a3 = 1ULL * a[i + offset + 3 * p].val()\
    \ * rot3.val();\n          auto a1na3imag =\n              1ULL * mint(a1 + mod2\
    \ - a3).val() * imag.val();\n          auto na2 = mod2 - a2;\n          a[i +\
    \ offset] = a0 + a2 + a1 + a3;\n          a[i + offset + 1 * p] = a0 + a2 + (2\
    \ * mod2 - (a1 + a3));\n          a[i + offset + 2 * p] = a0 + na2 + a1na3imag;\n\
    \          a[i + offset + 3 * p] = a0 + na2 + (mod2 - a1na3imag);\n        }\n\
    \        if (s + 1 != (1 << len))\n          rot *= info.rate3[countr_zero(~(unsigned\
    \ int)(s))];\n      }\n      len += 2;\n    }\n  }\n}\n\n// \u7834\u58CA\u7684\
    \u306B\u5909\u66F4\u3059\u308B\ntemplate <int mod>\nvoid intt(vc<static_modint32<mod>>\
    \ &a)\n{\n  using mint = static_modint32<mod>;\n  int n = int(a.size());\n  int\
    \ h = countr_zero((unsigned int)n);\n  assert(n == (1 << h));\n\n  static const\
    \ internal::fft_info<mint> info;\n\n  int len = h; // a[i, i+(n>>len), i+2*(n>>len),\
    \ ..] is transformed\n  while (len)\n  {\n    if (len == 1)\n    {\n      int\
    \ p = 1 << (h - len);\n      mint irot = 1;\n      for (int s = 0; s < (1 << (len\
    \ - 1)); s++)\n      {\n        int offset = s << (h - len + 1);\n        for\
    \ (int i = 0; i < p; i++)\n        {\n          auto l = a[i + offset];\n    \
    \      auto r = a[i + offset + p];\n          a[i + offset] = l + r;\n       \
    \   a[i + offset + p] =\n              (unsigned long long)(mint::mod() + l.val()\
    \ - (uint)r.val()) *\n              irot.val();\n          ;\n        }\n    \
    \    if (s + 1 != (1 << (len - 1)))\n          irot *= info.irate2[countr_zero(~(unsigned\
    \ int)(s))];\n      }\n      len--;\n    }\n    else\n    {\n      // 4-base\n\
    \      int p = 1 << (h - len);\n      mint irot = 1, iimag = info.iroot[2];\n\
    \      for (int s = 0; s < (1 << (len - 2)); s++)\n      {\n        mint irot2\
    \ = irot * irot;\n        mint irot3 = irot2 * irot;\n        int offset = s <<\
    \ (h - len + 2);\n        for (int i = 0; i < p; i++)\n        {\n          auto\
    \ a0 = 1ULL * a[i + offset + 0 * p].val();\n          auto a1 = 1ULL * a[i + offset\
    \ + 1 * p].val();\n          auto a2 = 1ULL * a[i + offset + 2 * p].val();\n \
    \         auto a3 = 1ULL * a[i + offset + 3 * p].val();\n\n          auto a2na3iimag\
    \ =\n              1ULL *\n              mint((mint::mod() + a2 - a3) * iimag.val()).val();\n\
    \n          a[i + offset] = a0 + a1 + a2 + a3;\n          a[i + offset + 1 * p]\
    \ =\n              (a0 + (mint::mod() - a1) + a2na3iimag) * irot.val();\n    \
    \      a[i + offset + 2 * p] =\n              (a0 + a1 + (mint::mod() - a2) +\
    \ (mint::mod() - a3)) *\n              irot2.val();\n          a[i + offset +\
    \ 3 * p] =\n              (a0 + (mint::mod() - a1) + (mint::mod() - a2na3iimag))\
    \ *\n              irot3.val();\n        }\n        if (s + 1 != (1 << (len -\
    \ 2)))\n          irot *= info.irate3[countr_zero(~(unsigned int)(s))];\n    \
    \  }\n      len -= 2;\n    }\n  }\n}\n\nnamespace internal\n{\n\ntemplate <class\
    \ mint>\nvc<mint> convolution_naive(const vc<mint> &a, const vc<mint> &b)\n{\n\
    \  const int n = a.size(), m = b.size();\n  const int cnta = n - count(ALL(a),\
    \ 0), cntb = m - count(ALL(b), 0);\n  vc<mint> c(n + m - 1);\n  if ((ll)m * cnta\
    \ > (ll)n * cntb)\n  {\n    repi(j, m)\n    {\n      if (b[j] == 0)\n        continue;\n\
    \      repi(i, n) c[i + j] += a[i] * b[j];\n    }\n  }\n  else\n  {\n    repi(i,\
    \ n)\n    {\n      if (a[i] == 0)\n        continue;\n      repi(j, m) c[i + j]\
    \ += a[i] * b[j];\n    }\n  }\n  return c;\n}\n\n// n == 0 or m == 0 \u306F\u30D0\
    \u30B0\u308B\u306E\u3067\u6CE8\u610F\ntemplate <class mint>\nvc<mint> convolution_ntt(vc<mint>\
    \ a, vc<mint> b)\n{\n  const int n = a.size(), m = b.size();\n  const int z =\
    \ bit_ceil(n + m - 1);\n  if (a == b)\n  {\n    a.resize(z);\n    ntt(a);\n  \
    \  repi(i, z) a[i] *= a[i];\n  }\n  else\n  {\n    a.resize(z), b.resize(z);\n\
    \    ntt(a), ntt(b);\n    repi(i, z) a[i] *= b[i];\n  }\n  intt(a);\n  mint iz\
    \ = mint(z).inv();\n  fem(ai : a) ai *= iz;\n  a.resize(n + m - 1);\n  return\
    \ a;\n}\n\ntemplate <size_t j, int mod, class T, size_t k>\nvoid convolution_crt_helper(const\
    \ vc<T> &a, const vc<T> &b, vc<array<T, k>> &cs)\n{\n  using mint = static_modint32<mod>;\n\
    \  const int n = a.size(), m = b.size();\n  auto c = convolution_ntt(vc<mint>(ALL(a)),\
    \ vc<mint>(ALL(b)));\n  repi(i, n + m - 1) cs[i][j] = c[i].val();\n}\n\n// n ==\
    \ 0 or m == 0 \u306F\u30D0\u30B0\u308B\u306E\u3067\u6CE8\u610F\ntemplate <int\
    \ ...ms, class T>\nvc<T> convolution_crt(const vc<T> &a, const vc<T> &b)\n{\n\
    \  const int n = a.size(), m = b.size();\n\n  constexpr size_t k = sizeof...(ms);\n\
    \  vc<array<T, k>> cs(n + m - 1);\n  constexpr array<int, k> ms_arr = {ms...};\n\
    \  [&]<size_t... Is>(index_sequence<Is...>)\n  {\n    (convolution_crt_helper<Is,\
    \ ms_arr[Is], T, k>(a, b, cs), ...);\n  }(make_index_sequence<k>{});\n\n  vc<T>\
    \ c(n + m - 1);\n  repi(i, n + m - 1) c[i] = get<1>(crt(cs[i], ms_arr));\n  return\
    \ c;\n}\n\n// n == 0 or m == 0 \u306F\u30D0\u30B0\u308B\u306E\u3067\u6CE8\u610F\
    \ntemplate <class mint, int... ms, class T>\nvc<mint> convolution_crt_mod(const\
    \ vc<T> &a, const vc<T> &b)\n{\n  const int n = a.size(), m = b.size();\n\n  constexpr\
    \ size_t k = sizeof...(ms);\n  vc<array<T, k>> cs(n + m - 1);\n  constexpr array<int,\
    \ k> ms_arr = {ms...};\n  [&]<size_t... Is>(index_sequence<Is...>)\n  {\n    (convolution_crt_helper<Is,\
    \ ms_arr[Is], T, k>(a, b, cs), ...);\n  }(make_index_sequence<k>{});\n\n  vc<mint>\
    \ c(n + m - 1);\n  repi(i, n + m - 1) c[i] = crt_mod_constexpr<mint>(cs[i], ms_arr).first;\n\
    \  return c;\n}\n\n}  // namespace internal\n\n// ntt-friendly \u306A\u3089\u666E\
    \u901A\u3001\u305D\u3046\u3067\u306A\u3051\u308C\u3070 3 \u3064\u306E mod\n//\
    \ mod \u306F 10^9 \u7A0B\u5EA6\u3092\u60F3\u5B9A\u3001\u5217\u306E\u9577\u3055\
    \u306F\u5408\u8A08 2^26 \u7A0B\u5EA6\ntemplate <class mint, typename = std::enable_if_t<!std::is_integral<mint>::value>>\n\
    vc<mint> convolution(const vc<mint> &a, const vc<mint> &b)\n{\n  const int n =\
    \ a.size(), m = b.size();\n  const int cnta = n - count(ALL(a), 0), cntb = m -\
    \ count(ALL(b), 0);\n  if (n == 0 || m == 0)\n    return {};\n  if (ntt_ok<mint>(n\
    \ + m - 1))\n  {\n    if (min(cnta, cntb) <= 60)\n      return internal::convolution_naive(a,\
    \ b);\n    return internal::convolution_ntt(a, b);\n  }\n  else\n  {\n    if (min(cnta,\
    \ cntb) <= 300)\n      return internal::convolution_naive(a, b);\n    assert(ntt_ok<static_modint32<469762049>>(n\
    \ + m - 1) && \"|a| + |b| - 1 <= 2^26\");\n    vc<ll> a_(n), b_(m);\n    repi(i,\
    \ n) a_[i] = a[i].val();\n    repi(j, m) b_[j] = b[j].val();\n    return internal::convolution_crt_mod<mint,\
    \ 469762049, 1811939329, 2013265921>(a_, b_);\n  }\n}\n\ntemplate <int mod = 998244353,\
    \ class T, typename = enable_if_t<is_integral<T>::value>>\nvc<T> convolution(const\
    \ vc<T> &a, const vc<T> &b)\n{\n  using mint = static_modint32<mod>;\n  auto c\
    \ = convolution(vc<mint>(ALL(a)), vc<mint>(ALL(b)));\n  vc<T> c_(c.size());\n\
    \  repi(i, c.size()) c_[i] = c[i].val();\n  return c_;\n}\n#line 6 \"math/convolution/convolution_ll.hpp\"\
    \n\n/**\n * @brief \u7573\u307F\u8FBC\u307F\uFF08long long\uFF09\n * @docs docs/math/convolution/convolution_ll.md\n\
    \ */\n\n// \u6700\u7D42\u7684\u306A\u8981\u7D20\u304C 4.2 \xD7 10^18 \u7A0B\u5EA6\
    \u306B\u53CE\u307E\u308B\u5834\u5408\n// mod 2 \u3064\u3067\u8A08\u7B97\n// \u5217\
    \u306E\u9577\u3055\u306F\u5408\u8A08 2^25 \u7A0B\u5EA6\nvc<ll> convolution_4e18(const\
    \ vc<ll> &a, const vc<ll> &b)\n{\n  const int n = a.size(), m = b.size();\n  const\
    \ int cnta = n - count(ALL(a), 0), cntb = m - count(ALL(b), 0);\n  if (min(cnta,\
    \ cntb) <= 150)\n    return internal::convolution_naive(a, b);\n  return internal::convolution_crt<2013265921,\
    \ 2113929217>(a, b);\n}\n#line 2 \"utils/make_unsigned_ext.hpp\"\n\n#line 4 \"\
    utils/make_unsigned_ext.hpp\"\n\n/**\n * @brief $128$ \u30D3\u30C3\u30C8\u6574\
    \u6570\u306B\u5BFE\u5FDC\u3057\u305F make_singed \u3068 make_unsigned\n * @docs\
    \ docs/utils/make_unsigned_ext.md\n */\n\ntemplate <class T>\nstruct make_unsigned_ext\n\
    {\n  using type = make_unsigned_t<T>;\n};\ntemplate <>\nstruct make_unsigned_ext<i128>\n\
    {\n  using type = u128;\n};\ntemplate <class T>\nusing make_unsigned_ext_t = typename\
    \ make_unsigned_ext<T>::type;\n\ntemplate <class T>\nstruct make_signed_ext\n\
    {\n  using type = make_signed_t<T>;\n};\ntemplate <>\nstruct make_signed_ext<u128>\n\
    {\n  using type = i128;\n};\ntemplate <class T>\nusing make_signed_ext_t = typename\
    \ make_signed_ext<T>::type;\n#line 8 \"math/bigint.hpp\"\n\n/**\n * @brief \u591A\
    \u500D\u9577\u6574\u6570\n * @docs docs/math/bigint.md\n */\n\n// \u6841\u6570\
    \u306E\u5236\u9650\n// FFT \u304C\u3067\u304D\u308B: 2^24 \u2252 1.67 * 10^7 \u6841\
    \n// CRT \u304C\u58CA\u308C\u306A\u3044: (3 * 10^18) * d / b^{2d} \u6841\n// (b,\
    \ d) = (10, 6) \u3067 1.80 * 10^7\n// (b, d) = (16, 5) \u3067 1.36 * 10^7\n//\
    \ (b, d) = (2, 21) \u3067 1.43 * 10^7\ntemplate <int base = 10, int digit = 6>\n\
    struct BigInteger\n{\nprivate:\n  static constexpr int BASE = ipow(base, digit);\n\
    \  vl vec;\n  bool is_nega = false;\n\n  void zero_suppress()\n  {\n    while\
    \ (!vec.empty() && vec.back() == 0)\n      vec.pop_back();\n    if (vec.empty())\n\
    \      is_nega = false;\n  }\n  void carry(int d = -1)\n  {\n    const int n =\
    \ vec.size();\n    if (n == 0)\n      return;\n    if (d < 0)\n      d = n - 1;\n\
    \    repi(i, d)\n    {\n      vec[i + 1] += vec[i] / BASE;\n      vec[i] = vec[i]\
    \ % BASE;\n    }\n    for (int i = d; !(0 <= vec[i] && vec[i] < BASE); i++)\n\
    \    {\n      vec.eb(vec[i] / BASE);\n      vec[i] = vec[i] % BASE;\n    }\n \
    \ }\n  void borrow(int si, int mx)\n  {\n    ll bor = 0;\n    repi(i, si, vec.size())\n\
    \    {\n      ll val = vec[i] - bor;\n      if (val < 0)\n      {\n        bor\
    \ = (-val + BASE - 1) / BASE;\n        vec[i] = val + bor * BASE;\n      }\n \
    \     else\n      {\n        vec[i] = val;\n        bor = 0;\n        if (i >=\
    \ mx)\n          break;\n      }\n    }\n    if (bor > 0)\n    {\n      is_nega\
    \ ^= 1;\n      ll car = 1;\n      repi(i, vec.size())\n      {\n        ll val\
    \ = (BASE - 1) - vec[i] + car;\n        if (val >= BASE)\n          vec[i] = val\
    \ - BASE, car = 1;\n        else\n          vec[i] = val, car = 0;\n      }\n\
    \    }\n    zero_suppress();\n    if (vec.empty())\n      is_nega = false;\n \
    \ }\n\n  BigInteger &operator<<=(size_t k)\n  {\n    vec.insert(vec.begin(), k,\
    \ 0);\n    return *this;\n  }\n  BigInteger &operator>>=(size_t k)\n  {\n    vec.erase(vec.begin(),\
    \ vec.begin() + min(vec.size(), k));\n    return *this;\n  }\n  BigInteger operator<<(size_t\
    \ k) const { return BigInteger(*this) <<= k; }\n  BigInteger operator>>(size_t\
    \ k) const { return BigInteger(*this) >>= k; }\n\n  int cmp(const BigInteger &b)\
    \ const\n  {\n    if (is_nega ^ b.is_nega)\n      return is_nega ? -1 : 1;\n \
    \   if (vec.size() != b.vec.size())\n      return ((vec.size() < b.vec.size())\
    \ ^ is_nega) ? -1 : 1;\n    repi(i, SZ(vec) - 1, -1, -1)\n    {\n      if (vec[i]\
    \ != b.vec[i])\n        return ((vec[i] < b.vec[i]) ^ is_nega) ? -1 : 1;\n   \
    \ }\n    return 0;\n  }\n\n  template <class T>\n  void add_abs(int q, T val)\n\
    \  {\n    int i = q;\n    while (val > 0)\n    {\n      if (i >= SZ(vec))\n  \
    \      vec.eb(0);\n      val += vec[i];\n      vec[i] = ll(val % BASE);\n    \
    \  val /= BASE;\n      i++;\n    }\n  }\n  template <class T>\n  void sub_abs(int\
    \ q, T val)\n  {\n    int i = q;\n    while (val > 0)\n    {\n      if (i >= SZ(vec))\n\
    \        vec.eb(0);\n      vec[i] -= ll(val % BASE);\n      val /= BASE;\n   \
    \   i++;\n    }\n    borrow(q, i - 1);\n  }\n\npublic:\n  BigInteger() {}\n  BigInteger(const\
    \ string &s)\n  {\n    int len = s.size(), offs = 0;\n    if (!s.empty() && s[0]\
    \ == '-')\n    {\n      is_nega = true;\n      len--;\n      offs++;\n    }\n\
    \    else\n      is_nega = false;\n    vec.resize(divceil(len, digit));\n    repi(i,\
    \ vec.size())\n    {\n      int r = len - i * digit;\n      int l = max(0, r -\
    \ digit);\n      string t = s.substr(offs + l, r - l);\n      if (!t.empty())\n\
    \        vec[i] = stoi(t, 0, base);\n    }\n    zero_suppress();\n  }\n  template\
    \ <class T, typename = enable_if_t<is_integral_ext<T>>>\n  BigInteger(T x)\n \
    \ {\n    using U = make_unsigned_ext_t<T>;\n    U ux = x;\n    if constexpr (is_signed_ext<T>)\n\
    \    {\n      if (x < 0)\n        is_nega = true, ux = -ux;\n      else\n    \
    \    is_nega = false;\n    }\n    else\n      is_nega = false;\n    \n    if (ux\
    \ == 0)\n      return;\n    while (ux > 0)\n    {\n      vec.eb((ll)(ux % BASE));\n\
    \      ux /= BASE;\n    }\n  }\n\n  string to_string() const\n  {\n    const int\
    \ n = vec.size();\n    if (n == 0)\n      return \"0\";\n    string s = is_nega\
    \ ? \"-\" : \"\";\n    s += base_repr_str(vec.back(), base);\n    repi(i, SZ(vec)\
    \ - 2, -1, -1)\n    {\n      s += base_repr_str(vec[i], base, digit);\n    }\n\
    \    return s;\n  }\n\n  bool operator<(const BigInteger &b) const { return cmp(b)\
    \ < 0; }\n  bool operator>(const BigInteger &b) const { return cmp(b) > 0; }\n\
    \  bool operator<=(const BigInteger &b) const { return cmp(b) <= 0; }\n  bool\
    \ operator>=(const BigInteger &b) const { return cmp(b) >= 0; }\n  bool operator==(const\
    \ BigInteger &b) const { return is_nega == b.is_nega && vec == b.vec; }\n  bool\
    \ operator!=(const BigInteger &b) const { return !(*this == b); }\n\n  BigInteger\
    \ operator-() const\n  {\n    BigInteger res(*this);\n    res.is_nega ^= 1;\n\
    \    return res;\n  }\n  BigInteger abs() const\n  {\n    BigInteger res(*this);\n\
    \    res.is_nega = false;\n    return res;\n  }\n  BigInteger &operator+=(const\
    \ BigInteger &b)\n  {\n    if (is_nega ^ b.is_nega)\n      return *this -= -b;\n\
    \    if (vec.size() < b.vec.size())\n      vec.resize(b.vec.size());\n    repi(i,\
    \ b.vec.size()) vec[i] += b.vec[i];\n    carry();\n    return *this;\n  }\n  BigInteger\
    \ &operator-=(const BigInteger &b)\n  {\n    if (is_nega ^ b.is_nega)\n      return\
    \ *this += -b;\n    if (vec.size() < b.vec.size())\n      vec.resize(b.vec.size());\n\
    \    repi(i, b.vec.size()) vec[i] -= b.vec[i];\n    borrow(0, SZ(b.vec) - 1);\n\
    \    return *this;\n  }\n  BigInteger &operator*=(const BigInteger &b)\n  {\n\
    \    if (this->vec.empty() || b.vec.empty())\n    {\n      this->vec.clear();\n\
    \      this->is_nega = false;\n      return *this;\n    }\n    vec = convolution_4e18(this->vec,\
    \ b.vec);\n    carry();\n    zero_suppress();\n    is_nega ^= b.is_nega;\n   \
    \ return *this;\n  }\n  BigInteger operator+(const BigInteger &b) const { return\
    \ BigInteger(*this) += b; }\n  BigInteger operator-(const BigInteger &b) const\
    \ { return BigInteger(*this) -= b; }\n  BigInteger operator*(const BigInteger\
    \ &b) const { return BigInteger(*this) *= b; }\n  BigInteger inv(int d) const\n\
    \  {\n    assert(!vec.empty());\n    BigInteger a(abs()), c, c2;\n    BigInteger\
    \ b = binsearch(LMD(m, ((a * m).vec.size() <= a.vec.size())), 0, BASE + 1, false,\
    \ false).first;\n    const BigInteger ONE(1), TWO(2);\n    for (int k = 1;; k\
    \ = min(2 * k, d))\n    {\n      c = a * b;\n      if (SZ(b.vec) >= d + 1 && SZ(c.vec)\
    \ == SZ(a.vec) + SZ(b.vec) - 1)\n      {\n        c2 = c + a;\n        if (c2.vec.size()\
    \ == a.vec.size() + b.vec.size())\n        {\n          if (c2 == ONE << (SZ(c2.vec)\
    \ - 1))\n            b += 1;\n          break;\n        }\n      }\n      b *=\
    \ (TWO << (SZ(a.vec) + SZ(b.vec) - 1)) - c;\n      if (SZ(b.vec) >= k + 1)\n \
    \       b >>= SZ(b.vec) - k - 1;\n    }\n    b >>= 1;\n    if (is_nega)\n    \
    \  b = -b;\n    return b;\n  }\n  BigInteger operator/(const BigInteger &b) const\n\
    \  {\n    assert(!b.vec.empty());\n    BigInteger a = abs();\n    if (a < b.abs())\n\
    \      return 0;\n    BigInteger ib = b.abs().inv(a.vec.size());\n    BigInteger\
    \ c = (a * ib) >> (SZ(a.vec) + SZ(b.vec) - 1);\n    if (a >= b.abs() * (c + 1))\n\
    \      c += 1;\n    c.is_nega = is_nega ^ b.is_nega;\n    return c;\n  }\n  pair<BigInteger,\
    \ BigInteger> divmod(const BigInteger &b) const\n  {\n    BigInteger q = *this\
    \ / b;\n    BigInteger r = *this - q * b;\n    return {q, r};\n  }\n  BigInteger\
    \ operator%(const BigInteger &b) const { return divmod(b).second; }\n  BigInteger\
    \ &operator/=(const BigInteger &b) { return *this = *this / b; }\n  BigInteger\
    \ &operator%=(const BigInteger &b) { return *this = *this % b; }\n\n  template\
    \ <class T, typename = enable_if_t<is_integral_ext<T>>>\n  BigInteger &operator+=(T\
    \ v)\n  {\n    if (v == 0)\n      return *this;\n    bool v_nega = false;\n  \
    \  using U = make_unsigned_ext_t<T>;\n    U uv = v;\n    if constexpr (is_signed_ext<T>)\n\
    \    {\n      if (v < 0)\n        v_nega = true, uv = -uv;\n    }\n    using V\
    \ = larger_int_t<U>;\n    if (is_nega == v_nega)\n      add_abs(0, (V)uv);\n \
    \   else\n      sub_abs(0, (V)uv);\n    return *this;\n  }\n  template <class\
    \ T, typename = enable_if_t<is_integral_ext<T>>>\n  BigInteger &operator-=(T v)\n\
    \  {\n    if (v == 0)\n      return *this;\n    bool v_nega = false;\n    using\
    \ U = make_unsigned_ext_t<T>;\n    U uv = v;\n    if constexpr (is_signed_ext<T>)\n\
    \    {\n      if (v < 0)\n        v_nega = true, uv = -uv;\n    }\n    using V\
    \ = larger_int_t<U>;\n    if (is_nega != v_nega)\n      add_abs(0, (V)uv);\n \
    \   else\n      sub_abs(0, (V)uv);\n    return *this;\n  }\n  template <class\
    \ T, typename = enable_if_t<is_integral_ext<T>>>\n  BigInteger &operator*=(T v)\n\
    \  {\n    if (vec.empty() || v == 0)\n    {\n      vec.clear();\n      is_nega\
    \ = false;\n      return *this;\n    }\n    bool v_nega = false;\n    using U\
    \ = make_unsigned_ext_t<T>;\n    U uv = v;\n    if constexpr (is_signed_ext<T>)\n\
    \    {\n      if (v < 0)\n        v_nega = true, uv = -uv;\n    }\n    is_nega\
    \ ^= v_nega;\n    using V = larger_int_t<U>;\n    V car = 0;\n    repi(i, SZ(vec))\n\
    \    {\n      car += (V)vec[i] * uv;\n      vec[i] = (ll)(car % BASE);\n     \
    \ car /= BASE;\n    }\n    while (car > 0)\n    {\n      vec.eb((ll)(car % BASE));\n\
    \      car /= BASE;\n    }\n    return *this;\n  }\n  template <class T, typename\
    \ = enable_if_t<is_integral_ext<T>>>\n  BigInteger &operator/=(T v)\n  {\n   \
    \ assert(v != 0);\n    if (vec.empty())\n      return *this;\n    bool v_nega\
    \ = false;\n    using U = make_unsigned_ext_t<T>;\n    U uv = v;\n    if constexpr\
    \ (is_signed_ext<T>)\n    {\n      if (v < 0)\n        v_nega = true, uv = -uv;\n\
    \    }\n    is_nega ^= v_nega;\n    using V = larger_int_t<U>;\n    V rem = 0;\n\
    \    repi(i, SZ(vec) - 1, -1, -1)\n    {\n      V cur = vec[i] + rem * BASE;\n\
    \      vec[i] = (ll)(cur / uv);\n      rem = cur % uv;\n    }\n    zero_suppress();\n\
    \    return *this;\n  }\n  template <class T, typename = enable_if_t<is_integral_ext<T>>>\n\
    \  BigInteger &operator%=(T v)\n  {\n    assert(v != 0);\n    if (vec.empty())\n\
    \      return *this;\n    using U = make_unsigned_ext_t<T>;\n    U uv = v;\n \
    \   if constexpr (is_signed_ext<T>)\n    {\n      if (v < 0)\n        uv = -uv;\n\
    \    }\n    using V = larger_int_t<U>;\n    V rem = 0;\n    repi(i, SZ(vec) -\
    \ 1, -1, -1)\n    {\n      V cur = vec[i] + rem * BASE;\n      rem = cur % uv;\n\
    \    }\n    vec.clear();\n    if (rem > 0)\n    {\n      while (rem > 0)\n   \
    \   {\n        vec.eb((ll)(rem % BASE));\n        rem /= BASE;\n      }\n    }\n\
    \    else\n    {\n      is_nega = false;\n    }\n    return *this;\n  }\n\n  //\
    \ base**i * coef \u3092\u52A0\u7B97\u3001\u306A\u3089\u3057 O(1) \u6642\u9593\n\
    \  template <class T, typename = enable_if_t<is_integral_ext<T>>>\n  BigInteger\
    \ &add_term(long long i, T coef)\n  {\n    if (coef == 0)\n      return *this;\n\
    \    bool c_nega = false;\n    using U = make_unsigned_ext_t<T>;\n    U ucoef\
    \ = coef;\n    if constexpr (is_signed_ext<T>)\n    {\n      if (coef < 0)\n \
    \       c_nega = true, ucoef = -ucoef;\n    }\n    int q = i / digit, r = i %\
    \ digit;\n    using V = larger_int_t<U>;\n    V val = (V)ucoef * ipow(base, r);\n\
    \    if (is_nega == c_nega)\n      add_abs(q, val);\n    else\n      sub_abs(q,\
    \ val);\n    return *this;\n  }\n  // base**i * coef \u3092\u6E1B\u7B97\u3001\u6570\
    \u304C\u5E38\u306B\u975E\u8CA0\u306E\u5834\u5408\u306F\u306A\u3089\u3057 O(1)\
    \ \u6642\u9593\n  template <class T, typename = enable_if_t<is_integral_ext<T>>>\n\
    \  BigInteger &sub_term(long long i, T coef)\n  {\n    if (coef == 0)\n      return\
    \ *this;\n    bool c_nega = false;\n    using U = make_unsigned_ext_t<T>;\n  \
    \  U ucoef = coef;\n    if constexpr (is_signed_ext<T>)\n    {\n      if (coef\
    \ < 0)\n        c_nega = true, ucoef = -ucoef;\n    }\n    int q = i / digit,\
    \ r = i % digit;\n    using V = larger_int_t<U>;\n    V val = (V)ucoef * ipow(base,\
    \ r);\n    if (is_nega != c_nega)\n      add_abs(q, val);\n    else\n      sub_abs(q,\
    \ val);\n    return *this;\n  }\n\n  template <class T, typename = enable_if_t<is_integral_ext<T>>>\n\
    \  BigInteger operator+(T v) const { return BigInteger(*this) += v; }\n  template\
    \ <class T, typename = enable_if_t<is_integral_ext<T>>>\n  BigInteger operator-(T\
    \ v) const { return BigInteger(*this) -= v; }\n  template <class T, typename =\
    \ enable_if_t<is_integral_ext<T>>>\n  BigInteger operator*(T v) const { return\
    \ BigInteger(*this) *= v; }\n  template <class T, typename = enable_if_t<is_integral_ext<T>>>\n\
    \  BigInteger operator/(T v) const { return BigInteger(*this) /= v; }\n  template\
    \ <class T, typename = enable_if_t<is_integral_ext<T>>>\n  BigInteger operator%(T\
    \ v) const { return BigInteger(*this) %= v; }\n};\n\ntemplate <class T, int base,\
    \ int digit, typename = enable_if_t<is_integral_ext<T>>>\nBigInteger<base, digit>\
    \ operator+(T a, const BigInteger<base, digit> &b)\n{\n  return b + a;\n}\ntemplate\
    \ <class T, int base, int digit, typename = enable_if_t<is_integral_ext<T>>>\n\
    BigInteger<base, digit> operator*(T a, const BigInteger<base, digit> &b)\n{\n\
    \  return b * a;\n}\ntemplate <class T, int base, int digit, typename = enable_if_t<is_integral_ext<T>>>\n\
    BigInteger<base, digit> operator-(T a, const BigInteger<base, digit> &b)\n{\n\
    \  return BigInteger<base, digit>(a) - b;\n}\n\n#if defined LOCAL or not defined\
    \ FAST_IO\ntemplate <int base, int digit>\nistream &operator>>(istream &is, BigInteger<base,\
    \ digit> &a)\n{\n  string s;\n  is >> s;\n  a = s;\n  return is;\n}\ntemplate\
    \ <int base, int digit>\nostream &operator<<(ostream &os, const BigInteger<base,\
    \ digit> &a)\n{\n  os << a.to_string();\n  return os;\n}\n#else\ntemplate <int\
    \ base, int digit>\nvoid rd1(BigInteger<base, digit> &a)\n{\n  string s;\n  fastio::rd1(s);\n\
    \  a = s;\n}\ntemplate <int base, int digit>\nvoid wt1(const BigInteger<base,\
    \ digit> &a)\n{\n  fastio::wt1(a.to_string());\n}\n#endif\n\ntemplate <int base,\
    \ int digit>\ninline BigInteger<base, digit> divfloor(const BigInteger<base, digit>\
    \ &a, const BigInteger<base, digit> &b)\n{\n  auto [q, r] = a.divmod(b);\n  if\
    \ (r != 0 && (a < 0) != (b < 0))\n    q -= 1;\n  return q;\n}\ntemplate <int base,\
    \ int digit>\ninline BigInteger<base, digit> divceil(const BigInteger<base, digit>\
    \ &a, const BigInteger<base, digit> &b)\n{\n  auto [q, r] = a.divmod(b);\n  if\
    \ (r != 0 && (a < 0) == (b < 0))\n    q += 1;\n  return q;\n}\ntemplate <int base,\
    \ int digit>\ninline BigInteger<base, digit> divround(const BigInteger<base, digit>\
    \ &a, const BigInteger<base, digit> &b)\n{\n  return divfloor(a * 2 + b, b * 2);\n\
    }\ntemplate <int base, int digit>\ninline BigInteger<base, digit> safemod(const\
    \ BigInteger<base, digit> &a, const BigInteger<base, digit> &b)\n{\n  auto [q,\
    \ r] = a.divmod(b);\n  if (r != 0 && (a < 0) != (b < 0))\n    r += b;\n  return\
    \ r;\n}\n\ntemplate <int base, int digit, class T, typename = enable_if_t<is_integral_ext<T>>>\n\
    inline BigInteger<base, digit> divfloor(const BigInteger<base, digit> &a, T b)\n\
    {\n  BigInteger<base, digit> q = a / b;\n  BigInteger<base, digit> r = a % b;\n\
    \  if (r != 0 && (a < 0) != (b < 0))\n    q -= 1;\n  return q;\n}\ntemplate <int\
    \ base, int digit, class T, typename = enable_if_t<is_integral_ext<T>>>\ninline\
    \ BigInteger<base, digit> divceil(const BigInteger<base, digit> &a, T b)\n{\n\
    \  BigInteger<base, digit> q = a / b;\n  BigInteger<base, digit> r = a % b;\n\
    \  if (r != 0 && (a < 0) == (b < 0))\n    q += 1;\n  return q;\n}\ntemplate <int\
    \ base, int digit, class T, typename = enable_if_t<is_integral_ext<T>>>\ninline\
    \ BigInteger<base, digit> divround(const BigInteger<base, digit> &a, T b)\n{\n\
    \  return divfloor(a * 2 + b, b * 2);\n}\ntemplate <int base, int digit, class\
    \ T, typename = enable_if_t<is_integral_ext<T>>>\ninline BigInteger<base, digit>\
    \ safemod(const BigInteger<base, digit> &a, T b)\n{\n  BigInteger<base, digit>\
    \ r = a % b;\n  if (r != 0 && (a < 0) != (b < 0))\n    r += b;\n  return r;\n\
    }\n\ntemplate <class T, int base, int digit, typename = enable_if_t<is_integral_ext<T>>>\n\
    inline BigInteger<base, digit> divfloor(T a, const BigInteger<base, digit> &b)\n\
    {\n  return divfloor(BigInteger<base, digit>(a), b);\n}\ntemplate <class T, int\
    \ base, int digit, typename = enable_if_t<is_integral_ext<T>>>\ninline BigInteger<base,\
    \ digit> divceil(T a, const BigInteger<base, digit> &b)\n{\n  return divceil(BigInteger<base,\
    \ digit>(a), b);\n}\ntemplate <class T, int base, int digit, typename = enable_if_t<is_integral_ext<T>>>\n\
    inline BigInteger<base, digit> divround(T a, const BigInteger<base, digit> &b)\n\
    {\n  return divround(BigInteger<base, digit>(a), b);\n}\ntemplate <class T, int\
    \ base, int digit, typename = enable_if_t<is_integral_ext<T>>>\ninline BigInteger<base,\
    \ digit> safemod(T a, const BigInteger<base, digit> &b)\n{\n  return safemod(BigInteger<base,\
    \ digit>(a), b);\n}\n\ntemplate <int base, int digit>\ninline pair<BigInteger<base,\
    \ digit>, BigInteger<base, digit>> safedivmod(const BigInteger<base, digit> &a,\
    \ const BigInteger<base, digit> &b)\n{\n  auto [q, r] = a.divmod(b);\n  if (r\
    \ != 0 && (a < 0) != (b < 0))\n  {\n    q -= 1;\n    r += b;\n  }\n  return {q,\
    \ r};\n}\ntemplate <int base, int digit, class T, typename = enable_if_t<is_integral_ext<T>>>\n\
    inline pair<BigInteger<base, digit>, BigInteger<base, digit>> safedivmod(const\
    \ BigInteger<base, digit> &a, T b)\n{\n  BigInteger<base, digit> q = a / b;\n\
    \  BigInteger<base, digit> r = a % b;\n  if (r != 0 && (a < 0) != (b < 0))\n \
    \ {\n    q -= 1;\n    r += b;\n  }\n  return {q, r};\n}\ntemplate <class T, int\
    \ base, int digit, typename = enable_if_t<is_integral_ext<T>>>\ninline pair<BigInteger<base,\
    \ digit>, BigInteger<base, digit>> safedivmod(T a, const BigInteger<base, digit>\
    \ &b)\n{\n  return safedivmod(BigInteger<base, digit>(a), b);\n}\n#line 19 \"\
    verify/yosupo/big_integer_division_hex.test.cpp\"\nusing bigint = BigInteger<16,\
    \ 5>;\n\nvoid init() {}\n\nvoid main2()\n{\n  IN(bigint, A, B);\n  PRINT(safedivmod(A,\
    \ B));\n}\n\nvoid test() {}\n\n#line 2 \"template/template_main.hpp\"\n\n#line\
    \ 4 \"template/template_main.hpp\"\n\ntemplate <auto init, auto main2, auto test>\n\
    struct Main\n{\n  Main()\n  {\n    cauto CERR = [](string val, string color)\n\
    \    {\n      string s = \"\\033[\" + color + \"m\" + val + \"\\033[m\";\n   \
    \   #ifdef LOCAL\n      cerr << s;\n      #endif\n      /* \u30B3\u30FC\u30C9\u30C6\
    \u30B9\u30C8\u3067\u78BA\u8A8D\u3059\u308B\u969B\u306B\u30B3\u30E1\u30F3\u30C8\
    \u30A2\u30A6\u30C8\u3092\u5916\u3059\n      cerr << val;\n      //*/\n    };\n\
    \  \n    #if defined FAST_IO and not defined LOCAL\n    CERR(\"\\n[FAST_IO]\\\
    n\\n\", \"32\");\n    #endif\n    #if defined FAST_CIO and not defined LOCAL\n\
    \    CERR(\"\\n[FAST_CIO]\\n\\n\", \"32\");\n    cin.tie(0);\n    ios::sync_with_stdio(false);\n\
    \    #endif\n    cout << fixed << setprecision(20);\n  \n    init();\n    #ifdef\
    \ LOCAL\n    test();\n    #endif\n  \n    #if defined AOJ_TESTCASE or (defined\
    \ LOCAL and defined SINGLE_TESTCASE)\n    CERR(\"\\n[AOJ_TESTCASE]\\n\\n\", \"\
    35\");\n    while (true)\n    {\n      dump(\"new testcase\");\n      main2();\n\
    \    }\n    #elif defined SINGLE_TESTCASE\n    CERR(\"\\n[SINGLE_TESTCASE]\\n\\\
    n\", \"36\");\n    main2();\n    #elif defined MULTI_TESTCASE\n    CERR(\"\\n[MULTI_TESTCASE]\\\
    n\\n\", \"33\");\n    local(while (true))\n    {\n      dump(\"T\");\n      IN(uint,\
    \ T);\n      while (T--)\n      {\n        dump(\"new testcase\");\n        main2();\n\
    \      }\n    }\n    #endif\n  }\n};\n#line 32 \"verify/yosupo/big_integer_division_hex.test.cpp\"\
    \nMain<init, main2, test> main_dummy;\nint main() {}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/division_of_hex_big_integers\"\
    \n\n// #define SINGLE_TESTCASE\n#define MULTI_TESTCASE\n// #define AOJ_TESTCASE\n\
    \n#ifndef LOCAL\n#define FAST_IO\n// #define FAST_CIO\n// #define INTERACTIVE\n\
    #endif\n\n#define INF 4'000'000'000'000'000'037LL\n#define EPS 1e-11\n\n#include\
    \ \"template/template_all_but_modint.hpp\"\n\n#include \"math/bigint.hpp\"\nusing\
    \ bigint = BigInteger<16, 5>;\n\nvoid init() {}\n\nvoid main2()\n{\n  IN(bigint,\
    \ A, B);\n  PRINT(safedivmod(A, B));\n}\n\nvoid test() {}\n\n#include \"template/template_main.hpp\"\
    \nMain<init, main2, test> main_dummy;\nint main() {}\n"
  dependsOn:
  - template/template_all_but_modint.hpp
  - template/template_types.hpp
  - template/template_rep.hpp
  - template/template_math.hpp
  - utils/is_integral_ext.hpp
  - template/template_vector.hpp
  - template/template_algo.hpp
  - template/template_binsearch.hpp
  - template/template_bit.hpp
  - template/template_inout.hpp
  - template/template_dump.hpp
  - template/template_random.hpp
  - math/bigint.hpp
  - math/convolution/convolution_ll.hpp
  - math/convolution/convolution.hpp
  - math/modint/modint.hpp
  - math/modint/modint_internal_static.hpp
  - utils/larger_int.hpp
  - math/modint/modint_internal_isprime.hpp
  - math/modint/modint_internal_barrett32.hpp
  - math/modint/modint_internal_montgomery64.hpp
  - math/extgcd.hpp
  - math/crt.hpp
  - utils/make_unsigned_ext.hpp
  - template/template_main.hpp
  isVerificationFile: true
  path: verify/yosupo/big_integer_division_hex.test.cpp
  requiredBy: []
  timestamp: '2026-04-20 06:20:24+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/yosupo/big_integer_division_hex.test.cpp
layout: document
redirect_from:
- /verify/verify/yosupo/big_integer_division_hex.test.cpp
- /verify/verify/yosupo/big_integer_division_hex.test.cpp.html
title: verify/yosupo/big_integer_division_hex.test.cpp
---
