---
data:
  _extendedDependsOn:
  - icon: ':x:'
    path: ds/segtree/segtree.hpp
    title: "\u30BB\u30B0\u30E1\u30F3\u30C8\u6728"
  - icon: ':question:'
    path: graph/tree/rooted_tree.hpp
    title: "\u6839\u3064\u304D\u6728\u30AF\u30E9\u30B9\uFF08HLD \u3082\u3059\u308B\
      \uFF09"
  - icon: ':x:'
    path: math/algebra/affine_function.hpp
    title: "\u4EE3\u6570\u7684\u69CB\u9020\uFF08\u4E00\u6B21\u95A2\u6570\uFF09"
  - icon: ':question:'
    path: math/algebra/algebra_base.hpp
    title: "\u4EE3\u6570\u7684\u69CB\u9020\u306E struct\uFF08\u57FA\u672C\uFF09"
  - icon: ':question:'
    path: math/extgcd.hpp
    title: "\u62E1\u5F35\u30E6\u30FC\u30AF\u30EA\u30C3\u30C9\u4E92\u9664\u6CD5 (extgcd)"
  - icon: ':question:'
    path: math/modint/modint.hpp
    title: math/modint/modint.hpp
  - icon: ':question:'
    path: math/modint/modint_internal_barrett32.hpp
    title: math/modint/modint_internal_barrett32.hpp
  - icon: ':question:'
    path: math/modint/modint_internal_isprime.hpp
    title: math/modint/modint_internal_isprime.hpp
  - icon: ':question:'
    path: math/modint/modint_internal_montgomery64.hpp
    title: math/modint/modint_internal_montgomery64.hpp
  - icon: ':question:'
    path: math/modint/modint_internal_static.hpp
    title: math/modint/modint_internal_static.hpp
  - icon: ':question:'
    path: template/template_algo.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30A2\u30EB\u30B4\u30EA\u30BA\
      \u30E0\uFF09"
  - icon: ':question:'
    path: template/template_all_but_modint.hpp
    title: template/template_all_but_modint.hpp
  - icon: ':question:'
    path: template/template_binsearch.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u4E8C\u5206\u63A2\u7D22\uFF09"
  - icon: ':question:'
    path: template/template_bit.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30D3\u30C3\u30C8\u6F14\u7B97\
      \uFF09"
  - icon: ':question:'
    path: template/template_dump.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09"
  - icon: ':question:'
    path: template/template_inout.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\uFF09"
  - icon: ':question:'
    path: template/template_main.hpp
    title: template/template_main.hpp
  - icon: ':question:'
    path: template/template_math.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09"
  - icon: ':question:'
    path: template/template_random.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30E9\u30F3\u30C0\u30E0\u751F\
      \u6210\uFF09"
  - icon: ':question:'
    path: template/template_rep.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08rep\uFF09"
  - icon: ':question:'
    path: template/template_types.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u578B\uFF09"
  - icon: ':question:'
    path: template/template_vector.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08vector\uFF09"
  - icon: ':question:'
    path: utils/is_integral_ext.hpp
    title: "$128$ \u30D3\u30C3\u30C8\u6574\u6570\u3092\u542B\u3081\u305F\u6574\u6570\
      \u5224\u5B9A"
  - icon: ':question:'
    path: utils/larger_int.hpp
    title: "\u7A4D\u306E\u8A08\u7B97\u306B\u4F7F\u3046\u62E1\u5F35\u578B"
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: true
  _pathExtension: cpp
  _verificationStatusIcon: ':x:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/vertex_set_path_composite
    links:
    - https://judge.yosupo.jp/problem/vertex_set_path_composite
  bundledCode: "#line 1 \"verify/yosupo/vertex_set_path_composite.test.cpp\"\n#define\
    \ PROBLEM \"https://judge.yosupo.jp/problem/vertex_set_path_composite\"\n\n#define\
    \ SINGLE_TESTCASE\n// #define MULTI_TESTCASE\n// #define AOJ_TESTCASE\n\n#ifndef\
    \ LOCAL\n#define FAST_IO\n// #define FAST_CIO\n// #define INTERACTIVE\n#endif\n\
    \n#define INF 4'000'000'000'000'000'037LL\n#define EPS 1e-11\n\n#line 2 \"template/template_all_but_modint.hpp\"\
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
    \ T>\nusing pqg = priority_queue<T>;\n\n#ifdef __SIZEOF_INT128__\nusing i128 =\
    \ __int128_t;\nusing u128 = __uint128_t;\ni128 stoi128(const string &s)\n{\n \
    \ const bool neg = s.front() == '-';\n  u128 res = 0;\n  for (int i = neg; i <\
    \ (int)s.size(); i++)\n    res = 10 * res + s[i] - '0';\n  if (neg)\n    return\
    \ -i128(res - 1) - 1;\n  return i128(res);\n}\nstring i128tos(i128 x)\n{\n  if\
    \ (x == 0) return \"0\";\n  string sign = \"\", res = \"\";\n  u128 ux;\n  if\
    \ (x < 0)\n    ux = u128(-(x + 1)) + 1, sign = \"-\";\n  else\n    ux = x;\n \
    \ while (ux > 0)\n  {\n    res += '0' + ux % 10;\n    ux /= 10;\n  }\n  reverse(res.begin(),\
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
    \  if (k == 2)\n  {\n    const T aa = T(a);\n    T x = T(sqrtl((long double)a));\n\
    \    while (x > aa / x)\n      x--;\n    while (x < numeric_limits<T>::max())\n\
    \    {\n      const T y = x + 1;\n      if (y > aa / y)\n        break;\n    \
    \  x = y;\n    }\n    return x;\n  }\n\n  auto isok = [&](T x) -> bool\n  {\n\
    \    if (x == 0)\n      return true;\n    T res = 1, k2 = k;\n    while (true)\n\
    \    {\n      if (k2 & 1)\n      {\n        if (res > T(a) / x)\n          return\
    \ false;\n        res *= x;\n      }\n      k2 >>= 1;\n      if (k2 == 0)\n  \
    \      break;\n      if (x > T(a) / x)\n        return false;\n      x *= x;\n\
    \    }\n    return res <= T(a);\n  };\n\n  T x = pow(a, 1.0 / k);\n  bool up =\
    \ true;\n  while (!isok(x))\n    up = false, x--;\n  if (up)\n  {\n    while (x\
    \ < numeric_limits<T>::max() && isok(x + 1))\n      x++;\n  }\n  return x;\n}\n\
    template <class T = ll, class A, class K>\nconstexpr T iroot_ceil(A a, K k)\n\
    {\n  T x = iroot<T>(a, k);\n  return ipow<T>(x, k) == a ? x : x + 1;\n}\n\n//\
    \ https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html\n\
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
    \ INF 4'000'000'000'000'000'037LL\n#endif\n\n#line 11 \"template/template_algo.hpp\"\
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
    V sortuniqued(V v) { sortunique(v); return v; }\n\n// 01234 -> 12340\ntemplate\
    \ <class V, class U>\nvoid rotate(V &v, U k)\n{ \n  const U n = v.size();\n  if\
    \ (n == 0)\n    return;\n  k = (k % n + n) % n;\n  std::rotate(v.begin(), v.begin()\
    \ + k, v.end());\n}\n// 01234 -> 12340\ntemplate <class V, class U>\nV rotated(V\
    \ v, U k) { rotate(v, k); return v; }\n\ntemplate <class T>\nvvc<T> top(const\
    \ vvc<T> &a)\n{\n  if (a.empty())\n    return {};\n  const int n = a.size(), m\
    \ = a[0].size();\n  vvc<T> b(m, vc<T>(n));\n  repi(i, n)\n  {\n    assert(SZ<int>(a[i])\
    \ == m);\n    repi(j, m) b[j][i] = a[i][j];\n  }\n  return b;\n}\nvstr top(const\
    \ vstr &a)\n{\n  vvc<char> a_(a.size());\n  repi(i, SZ<int>(a)) a_[i] = {ALL(a[i])};\n\
    \  vvc<char> b_ = top(a_);\n  vstr b(b_.size());\n  repi(i, SZ<int>(b)) b[i] =\
    \ {ALL(b_[i])};\n  return b;\n}\n\ntemplate <class T, class = void>\nstruct has_e0\
    \ : false_type {};\ntemplate <class T>\nstruct has_e0<T, void_t<decltype(T::e0())>>\
    \ : true_type {};\ntemplate <class T>\ninline constexpr bool has_e0_v = has_e0<T>::value;\n\
    \ntemplate <class T>\nstruct MonoidAdd\n{\n  using S = T;\n  static constexpr\
    \ S op(S a, S b) { return a + b; }\n  static constexpr S e()\n  {\n    if constexpr\
    \ (has_e0_v<S>)\n      return S::e0();\n    else\n      return {};\n  }\n  template\
    \ <class I, class = decltype(declval<S>() * declval<I>())>\n  static constexpr\
    \ S pow(const S &a, I k) { return a * k; }\n};\ntemplate <class T, const T infty\
    \ = INF>\nstruct MonoidMin\n{\n  using S = T;\n  static constexpr S op(S a, S\
    \ b) { return min(a, b); }\n  static constexpr S e() { return infty; }\n  template\
    \ <class I>\n  static constexpr S pow(const S &a, I k) { return k == 0 ? e() :\
    \ a; }\n};\ntemplate <class T, const T infty = INF>\nstruct MonoidMax\n{\n  using\
    \ S = T;\n  static constexpr S op(S a, S b) { return max(a, b); }\n  static constexpr\
    \ S e() { return -infty; }\n  template <class I>\n  static constexpr S pow(const\
    \ S &a, I k) { return k == 0 ? e() : a; }\n};\n\nnamespace internal\n{\n  template\
    \ <class M, class I, class = void>\n  struct HasMonoidPow : false_type\n  {\n\
    \  };\n  template <class M, class I>\n  struct HasMonoidPow<M, I, void_t<decltype(M::pow(declval<const\
    \ typename M::S &>(), declval<I>()))>> : true_type\n  {\n  };\n}\n\ntemplate <class\
    \ M, class I>\ntypename M::S pow_monoid(typename M::S a, I k)\n{\n  if constexpr\
    \ (is_signed_ext<I>)\n    assert(k >= 0);\n  if constexpr (internal::HasMonoidPow<M,\
    \ I>::value)\n    return M::pow(a, k);\n  else\n  {\n    typename M::S c = M::e();\n\
    \    for (; k; k >>= 1)\n    {\n      if (k & 1)\n        c = M::op(c, a);\n \
    \     a = M::op(a, a);\n    }\n    return c;\n  }\n}\n\ntemplate <class G, class\
    \ I>\ntypename G::S pow_group(typename G::S a, I k)\n{\n  if constexpr (is_signed_ext<I>)\n\
    \  {\n    if (k < 0)\n    {\n      a = G::inv(a);\n      return G::op(pow_monoid<G>(a,\
    \ -(k + 1)), a);\n    }\n  }\n  return pow_monoid<G>(a, k);\n}\n\n// left_index\
    \ \u304C 0 \u306A\u3089\u3001\u9577\u3055 n+1 \u3067 a.front() \u304C e()\n//\
    \ left_index \u304C 1 \u306A\u3089\u3001\u9577\u3055 n \u3067 e() \u304C\u306A\
    \u3044\ntemplate <class M>\nvc<typename M::S> cuml(const vc<typename M::S> &v,\
    \ int left_index = 0)\n{\n  const int n = v.size();\n  vc<typename M::S> res(n\
    \ + 1);\n  res[0] = M::e();\n  repi(i, n) res[i + 1] = M::op(res[i], v[i]);\n\
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
    \ = 0, int right_index = 0)\n{\n  int n = v.size();\n  assert(0 <= left_index\
    \ && 0 <= right_index && left_index + right_index <= n + 1);\n  vc<T> res(n +\
    \ 1);\n  if (n == 0)\n  {\n    res[0] = T{};\n    res.erase(res.end() - right_index,\
    \ res.end());\n    res.erase(res.begin(), res.begin() + left_index);\n    return\
    \ res;\n  }\n  res[0] = v[0];\n  repi(i, 1, n) res[i] = v[i] - v[i - 1];\n  res[n]\
    \ = -v[n - 1];\n  res.erase(res.end() - right_index, res.end());\n  res.erase(res.begin(),\
    \ res.begin() + left_index);\n  return res;\n}\n\nconstexpr array<pll, 4> DRULgrid\
    \ = {{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};\nconstexpr array<pll, 4> DRULplane =\
    \ {{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}};\n#line 2 \"template/template_binsearch.hpp\"\
    \n\n#line 6 \"template/template_binsearch.hpp\"\n\n/**\n * @brief \u30C6\u30F3\
    \u30D7\u30EC\u30FC\u30C8\uFF08\u4E8C\u5206\u63A2\u7D22\uFF09\n * @docs docs/template/template_binsearch.md\n\
    \ */\n\ntemplate <class T>\nstruct is_random_access_iterator\n{\n  static constexpr\
    \ bool value = is_same_v<\n    typename iterator_traits<T>::iterator_category,\n\
    \    random_access_iterator_tag\n  >;\n};\ntemplate <class T>\nconstexpr bool\
    \ is_random_access_iterator_v = is_random_access_iterator<T>::value;\n\n// ---\
    \ LB, UB ---\n\n#if __cplusplus < 202002L\nstruct identity\n{\n  template <class\
    \ T>\n  constexpr T &&operator()(T &&t) const noexcept\n  { return forward<T>(t);\
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
    \u4F5C\u4E8C\u5206\u63A2\u7D22 ---\n\nnamespace internal\n{\ntemplate <class T>\n\
    bool binsearch_adjacent(T a, T b)\n{\n  if (a < b)\n    return a + 1 == b;\n \
    \ if (b < a)\n    return b + 1 == a;\n  return false;\n}\n};\n\n// (ok, ng)\n\
    template <class T = ll, class Judge, class InitOk, class InitNg>\npair<T, T> binsearch(const\
    \ Judge &judge, InitOk init_ok, InitNg init_ng, bool check_ok = true, bool check_ng\
    \ = true)\n{\n  T ok(init_ok), ng(init_ng);\n  if (check_ok)\n    assert(judge(ok));\n\
    \  if (check_ng)\n    assert(!judge(ng));\n  while (!internal::binsearch_adjacent(ok,\
    \ ng))\n  {\n    T mid = (ok & ng) + ((ok ^ ng) >> 1);\n    (judge(mid) ? ok :\
    \ ng) = mid;\n  }\n  return {ok, ng};\n}\ntemplate <class T = ld, class Judge,\
    \ class InitOk, class InitNg>\nT binsearch_real(const Judge &judge, InitOk init_ok,\
    \ InitNg init_ng, int iteration_count = 100, bool check_ok = true, bool check_ng\
    \ = true)\n{\n  T ok(init_ok), ng(init_ng);\n  if (check_ok)\n    assert(judge(ok));\n\
    \  if (check_ng)\n    assert(!judge(ng));\n  repi(_, iteration_count)\n  {\n \
    \   T mid = (ok + ng) / 2;\n    (judge(mid) ? ok : ng) = mid;\n  }\n  return ok;\n\
    }\n// (ok, ng)\ntemplate <class T = ll, class Judge, class InitVal>\npair<T, T>\
    \ expsearch(const Judge &judge, InitVal init_val, bool positive = true)\n{\n \
    \ T cur(init_val), step = 1;\n  const bool cur_ok = judge(cur);\n  auto advance\
    \ = [&](T x, T d, bool pos) -> T\n  {\n    if (pos)\n      return x > numeric_limits<T>::max()\
    \ - d ? numeric_limits<T>::max() : x + d;\n    else\n      return x < numeric_limits<T>::lowest()\
    \ + d ? numeric_limits<T>::lowest() : x - d;\n  };\n  T prv = advance(cur, 1,\
    \ !positive);\n  if (prv != cur && judge(prv) != cur_ok)\n  {\n    if (cur_ok)\n\
    \      return {cur, prv};\n    else\n      return {prv, cur};\n  }\n  while (true)\n\
    \  {\n    T nxt = advance(cur, step, positive);\n    assert(nxt != cur && \"the\
    \ boundary must exist in the searched direction\");\n    if (nxt == cur || judge(nxt)\
    \ != cur_ok)\n    {\n      T ok = cur_ok ? cur : nxt;\n      T ng = cur_ok ? nxt\
    \ : cur;\n      return binsearch<T>(judge, ok, ng, false, false);\n    }\n   \
    \ cur = nxt;\n    if (step > numeric_limits<T>::max() / 2)\n      step = numeric_limits<T>::max();\n\
    \    else\n      step *= 2;\n  }\n}\n#line 2 \"template/template_bit.hpp\"\n\n\
    #line 5 \"template/template_bit.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\
    \u30FC\u30C8\uFF08\u30D3\u30C3\u30C8\u6F14\u7B97\uFF09\n * @docs docs/template/template_bit.md\n\
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
    CPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 100);\n#define local(...) __VA_ARGS__\n\
    #define oj(...)\n#define local_oj(a, b) (a)\nCPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(content());\n\
    #else\n#define dump(...)\n#define local(...)\n#define oj(...) __VA_ARGS__\n#define\
    \ local_oj(a, b) (b)\n#endif\n\ntemplate <class T, class Sequence>\nvc<T> content(queue<T,\
    \ Sequence> que)\n{\n  vc<T> res;\n  while (!que.empty())\n  {\n    res.eb(que.front());\n\
    \    que.pop();\n  }\n  return res;\n}\ntemplate <class T, class Sequence, class\
    \ Compare>\nvc<T> content(priority_queue<T, Sequence, Compare> pque)\n{\n  vc<T>\
    \ res;\n  while (!pque.empty())\n  {\n    res.eb(pque.top());\n    pque.pop();\n\
    \  }\n  return res;\n}\ntemplate <class T>\nauto content(const T &obj) { return\
    \ obj.content(); }\n#line 6 \"template/template_inout.hpp\"\n\n/**\n * @brief\
    \ \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\uFF09\n * @docs\
    \ docs/template/template_inout.md\n */\n\n// https://judge.yosupo.jp/submission/170706\
    \ (maspy \u3055\u3093)\n// https://judge.yosupo.jp/submission/21623  (Nyaan \u3055\
    \u3093)\n#if defined FAST_IO and not defined LOCAL\nnamespace fastio {\ntemplate\
    \ <class T>\nstruct unsigned_integer\n{\n  using type = make_unsigned_t<T>;\n\
    };\ntemplate <>\nstruct unsigned_integer<i128>\n{\n  using type = u128;\n};\n\
    template <>\nstruct unsigned_integer<u128>\n{\n  using type = u128;\n};\ntemplate\
    \ <class T>\nusing unsigned_integer_t = typename unsigned_integer<T>::type;\n\n\
    static constexpr uint32_t SIZ = 1 << 17;\nchar ibuf[SIZ];\nchar obuf[SIZ];\nchar\
    \ out[100];\n// pointer of ibuf, obuf\nuint32_t pil = 0, pir = 0, por = 0;\n\n\
    struct Pre {\n  char num[10000][4];\n  constexpr Pre() : num() {\n    for (int\
    \ i = 0; i < 10000; i++) {\n      int n = i;\n      for (int j = 3; j >= 0; j--)\
    \ {\n        num[i][j] = n % 10 | '0';\n        n /= 10;\n      }\n    }\n  }\n\
    } constexpr pre;\n\ninline void load() {\n  memcpy(ibuf, ibuf + pil, pir - pil);\n\
    \  pir = pir - pil + fread(ibuf + pir - pil, 1, SIZ - pir + pil, stdin);\n  pil\
    \ = 0;\n  if (pir < SIZ) ibuf[pir++] = '\\n';\n}\n\ninline void flush() {\n  fwrite(obuf,\
    \ 1, por, stdout);\n  por = 0;\n}\n\nvoid rd1(char &c) {\n  do {\n    if (pil\
    \ + 1 > pir) load();\n    c = ibuf[pil++];\n  } while (isspace(c));\n}\n\nvoid\
    \ rd1(string &x) {\n  x.clear();\n  char c;\n  do {\n    if (pil + 1 > pir) load();\n\
    \    c = ibuf[pil++];\n  } while (isspace(c));\n  do {\n    x += c;\n    if (pil\
    \ == pir) load();\n    c = ibuf[pil++];\n  } while (!isspace(c));\n}\n\ntemplate\
    \ <typename T>\nvoid rd1_real(T &x) {\n  string s;\n  rd1(s);\n  if constexpr\
    \ (is_same_v<T, long double>)\n    x = stold(s);\n  else\n    x = stod(s);\n}\n\
    \ntemplate <typename T>\nvoid rd1_integer(T &x) {\n  if (pil + 100 > pir) load();\n\
    \  char c;\n  do\n    c = ibuf[pil++];\n  while (c < '-');\n  bool minus = 0;\n\
    \  if constexpr (is_signed<T>::value || is_same_v<T, i128>) {\n    if (c == '-')\
    \ { minus = 1, c = ibuf[pil++]; }\n  }\n  using U = unsigned_integer_t<T>;\n \
    \ U val = 0;\n  while ('0' <= c) { val = val * 10 + (c & 15), c = ibuf[pil++];\
    \ }\n  pil--;\n  if constexpr (is_signed<T>::value || is_same_v<T, i128>)\n  {\n\
    \    if (minus)\n    {\n      const U min_abs = U(numeric_limits<T>::max()) +\
    \ 1;\n      assert(val <= min_abs);\n      x = val == min_abs ? numeric_limits<T>::lowest()\
    \ : -T(val);\n    }\n    else\n    {\n      assert(val <= U(numeric_limits<T>::max()));\n\
    \      x = T(val);\n    }\n  }\n  else\n    x = T(val);\n}\n\nvoid rd1(int &x)\
    \ { rd1_integer(x); }\nvoid rd1(ll &x) { rd1_integer(x); }\nvoid rd1(i128 &x)\
    \ { rd1_integer(x); }\nvoid rd1(uint &x) { rd1_integer(x); }\nvoid rd1(ull &x)\
    \ { rd1_integer(x); }\nvoid rd1(u128 &x) { rd1_integer(x); }\nvoid rd1(double\
    \ &x) { rd1_real(x); }\nvoid rd1(long double &x) { rd1_real(x); }\n// void rd1(f128\
    \ &x) { rd1_real(x); }\n\ntemplate <class T, class U>\nvoid rd1(pair<T, U> &p)\
    \ {\n  return rd1(p.first), rd1(p.second);\n}\ntemplate <class... T>\nvoid rd1(tuple<T...>\
    \ &tpl) {\n  apply([](auto &...x) { (rd1(x), ...); }, tpl);\n}\n\ntemplate <size_t\
    \ N = 0, typename T>\nvoid rd1(array<T, N> &x) {\n  for (auto &d: x) rd1(d);\n\
    }\ntemplate <class T>\nvoid rd1(vc<T> &x) {\n  for (auto &d: x) rd1(d);\n}\n\n\
    template <class... T>\nvoid read(T &...x) {\n  (rd1(x), ...);\n}\n\nvoid wt1(const\
    \ char c) {\n  if (por == SIZ) flush();\n  obuf[por++] = c;\n}\nvoid wt1(const\
    \ string s) {\n  for (char c: s) wt1(c);\n}\nvoid wt1(const char *s) {\n  size_t\
    \ len = strlen(s);\n  for (size_t i = 0; i < len; i++) wt1(s[i]);\n}\n\ntemplate\
    \ <typename T>\nvoid wt1_integer(T x) {\n  if (por > SIZ - 100) flush();\n  using\
    \ U = unsigned_integer_t<T>;\n  U ux;\n  if constexpr (is_signed<T>::value ||\
    \ is_same_v<T, i128>)\n  {\n    if (x < 0)\n      obuf[por++] = '-', ux = U(0)\
    \ - U(x);\n    else\n      ux = U(x);\n  }\n  else\n    ux = x;\n  int outi;\n\
    \  for (outi = 96; ux >= 10000; outi -= 4) {\n    memcpy(out + outi, pre.num[ux\
    \ % 10000], 4);\n    ux /= 10000;\n  }\n  if (ux >= 1000) {\n    memcpy(obuf +\
    \ por, pre.num[ux], 4);\n    por += 4;\n  } else if (ux >= 100) {\n    memcpy(obuf\
    \ + por, pre.num[ux] + 1, 3);\n    por += 3;\n  } else if (ux >= 10) {\n    int\
    \ q = (ux * 103) >> 10;\n    obuf[por] = q | '0';\n    obuf[por + 1] = (ux - q\
    \ * 10) | '0';\n    por += 2;\n  } else\n    obuf[por++] = ux | '0';\n  memcpy(obuf\
    \ + por, out + outi + 4, 96 - outi);\n  por += 96 - outi;\n}\n\ntemplate <typename\
    \ T>\nvoid wt1_real(T x) {\n  ostringstream oss;\n  oss << fixed << setprecision(15)\
    \ << x;\n  string s = oss.str();\n  wt1(s);\n}\n\nvoid wt1(int x) { wt1_integer(x);\
    \ }\ntemplate <class T, enable_if_t<is_integral_v<T>, int> = 0>\nvoid wt1(T x)\
    \ { wt1_integer(x); }\nvoid wt1(i128 x) { wt1_integer(x); }\nvoid wt1(u128 x)\
    \ { wt1_integer(x); }\nvoid wt1(double x) { wt1_real(x); }\nvoid wt1(long double\
    \ x) { wt1_real(x); }\n// void wt1(f128 x) { wt1_real(x); }\n\ntemplate <class\
    \ T, class U>\nvoid wt1(const pair<T, U> &val) {\n  wt1(val.first);\n  wt1(' ');\n\
    \  wt1(val.second);\n}\ntemplate <class... T>\nvoid wt1(const tuple<T...> &tpl)\
    \ {\n  if constexpr (sizeof...(T))\n  {\n    int i = 0;\n    apply([&](const auto\
    \ &...x)\n          { ((i++ ? wt1(' ') : void(), wt1(x)), ...); }, tpl);\n  }\n\
    }\ntemplate <class T, size_t S>\nvoid wt1(const array<T, S> &val) {\n  auto n\
    \ = val.size();\n  for (size_t i = 0; i < n; i++) {\n    if (i) wt1(' ');\n  \
    \  wt1(val[i]);\n  }\n}\ntemplate <class T>\nvoid wt1(const vector<T> &val) {\n\
    \  auto n = val.size();\n  for (size_t i = 0; i < n; i++) {\n    if (i) wt1('\
    \ ');\n    wt1(val[i]);\n  }\n}\n\ntemplate <class... T>\nvoid write(T &&...x)\
    \ {\n  (wt1(std::forward<T>(x)), ...);\n}\n\ntemplate <class... T>\nvoid print(T\
    \ &&...x) {\n  if constexpr (sizeof...(T))\n  {\n    int i = 0;\n    ((i++ ? wt1('\
    \ ') : void(), wt1(std::forward<T>(x))), ...);\n  }\n  wt1('\\n');\n}\n\n} //\
    \ namespace fastio\n\n#endif\n\n#if defined FAST_IO and not defined LOCAL\nstruct\
    \ Dummy {\n  Dummy() { atexit(fastio::flush); }\n} dummy;\n#endif\n\n// https://trap.jp/post/1224/\n\
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
    \ Ts>\nvoid READnodump(Ts &...a) { CIN(a...); }\n#endif\n\ntemplate <class...\
    \ T>\nvoid READVECnodump(int n, vc<T> &...v)\n{\n  (v.resize(n), ...);\n  READnodump(v...);\n\
    }\n\ntemplate <class... T>\nvoid READVEC2nodump(int n, int m, vvc<T> &...v)\n\
    {\n  (v.assign(n, vc<T>(m)), ...);\n  READnodump(v...);\n}\n\ntemplate <class...\
    \ T>\nvoid READJAGnodump(int n, vvc<T> &...vs)\n{\n  auto read_one = [&](auto\
    \ &v)\n  {\n    v.resize(n);\n    for (auto &row : v)\n    {\n      int k;\n \
    \     READnodump(k);\n      row.resize(k);\n      READnodump(row);\n    }\n  };\n\
    \  (read_one(vs), ...);\n}\n\n}; // namespace internal\n\n#define READ(...) internal::READnodump(__VA_ARGS__);\
    \ dump(__VA_ARGS__)\n\n#define IN(T, ...) T __VA_ARGS__; READ(__VA_ARGS__)\n\n\
    #define CHAR(...) IN(char, __VA_ARGS__)\n#define INT(...) IN(int, __VA_ARGS__)\n\
    #define LL(...) IN(ll, __VA_ARGS__)\n#define STR(...) IN(string, __VA_ARGS__)\n\
    #define ARR(T, n, ...) array<T, n> __VA_ARGS__; READ(__VA_ARGS__)\n\n#define READVEC(...)\
    \ internal::READVECnodump(__VA_ARGS__); dump(__VA_ARGS__)\n#define READVEC2(...)\
    \ internal::READVEC2nodump(__VA_ARGS__); dump(__VA_ARGS__)\n\n#define VEC(T, n,\
    \ ...) vc<T> __VA_ARGS__; READVEC(n, __VA_ARGS__)\n#define VEC2(T, n, m, ...)\
    \ vvc<T> __VA_ARGS__; READVEC2(n, m, __VA_ARGS__)\n\n#define READJAG(...) internal::READJAGnodump(__VA_ARGS__);\
    \ dump(__VA_ARGS__)\n\n#define JAG(T, n, ...) vvc<T> __VA_ARGS__; READJAG(n, __VA_ARGS__)\n\
    \n// ----------\n\n// ----- \u51FA\u529B -----\n#ifdef INTERACTIVE\n#define ENDL\
    \ endl\n#else\n#define ENDL '\\n'\n#endif\n\n#if defined LOCAL or not defined\
    \ FAST_IO\ntemplate <class T, class U>\nostream &operator<<(ostream &os, const\
    \ pair<T, U> &p)\n{\n  os << p.first << ' ' << p.second;\n  return os;\n}\n\n\
    template <class... Ts>\nostream &operator<<(ostream &os, const tuple<Ts...> &t)\n\
    {\n  if constexpr (sizeof...(Ts))\n  {\n    apply([&](const auto &...x)\n    \
    \      {\n            int i = 0;\n            ((os << (i++ ? \" \" : \"\") <<\
    \ x), ...);\n          }, t);\n  }\n  return os;\n}\ntemplate <class T, size_t\
    \ n>\nostream &operator<<(ostream &os, const array<T, n> &a)\n{\n  for (size_t\
    \ i = 0; i < n; i++)\n  {\n    if (i)\n      os << ' ';\n    os << a[i];\n  }\n\
    \  return os;\n}\ntemplate <class T>\nostream &operator<<(ostream &os, const vc<T>\
    \ &v)\n{\n  const size_t n = v.size();\n  for (size_t i = 0; i < n; i++)\n  {\n\
    \    if (i)\n      os << ' ';\n    os << v[i];\n  }\n  return os;\n}\n\nnamespace\
    \ internal\n{\n\ntemplate <class... Ts>\nvoid COUTW(const Ts &...a)\n{\n  if constexpr\
    \ (sizeof...(Ts))\n    (cout << ... << a);\n}\n\ntemplate <class... Ts>\nvoid\
    \ COUTP(const Ts &...a)\n{\n  if constexpr (sizeof...(Ts))\n  {\n    int i = 0;\n\
    \    ((cout << (i++ ? \" \" : \"\") << a), ...);\n  }\n  cout << ENDL;\n}\n\n\
    }; // namespace internal\n#endif\n\n#if defined FAST_IO and not defined LOCAL\n\
    #define WRITE fastio::write\n#define PRINT fastio::print\n#else\n#define WRITE\
    \ internal::COUTW\n#define PRINT internal::COUTP\n#endif\n#define PRINTEXIT(...)\
    \ do { PRINT(__VA_ARGS__); exit(0); } while (false)\n#define PRINTRETURN(...)\
    \ do { PRINT(__VA_ARGS__); return; } while (false)\n\ntemplate <class T>\nvoid\
    \ PRINTV(const vc<T> &v) { for (auto &vi : v) PRINT(vi); }\n#define PRINTVEXIT(...)\
    \ do { PRINTV(__VA_ARGS__); exit(0); } while (false)\n#define PRINTVRETURN(...)\
    \ do { PRINTV(__VA_ARGS__); return; } while (false)\n// ----------\n\n// -----\
    \ \u57FA\u6E96\u305A\u3089\u3057 -----\ntemplate <class T, class U, class P>\n\
    pair<T, U> &operator+=(pair<T, U> &a, const P &b)\n{\n  a.first += b.first;\n\
    \  a.second += b.second;\n  return a;\n}\ntemplate <class T, class U, class P>\n\
    pair<T, U> operator+(pair<T, U> a, const P &b) { return a += b; }\n\ntemplate\
    \ <class T, size_t n, class A>\narray<T, n> &operator+=(array<T, n> &a, const\
    \ A &b)\n{\n  for (size_t i = 0; i < n; i++)\n    a[i] += b[i];\n  return a;\n\
    }\ntemplate <class T, size_t n, class A>\narray<T, n> operator+(array<T, n> a,\
    \ const A &b) { return a += b; }\n\nnamespace internal\n{\n\ntemplate <size_t...\
    \ I, class A, class B>\nauto &tuple_add_impl(A &a, const B &b, const index_sequence<I...>)\n\
    {\n  ((get<I>(a) += get<I>(b)), ...);\n  return a;\n}\n\n}; // namespace internal\n\
    \ntemplate <class... Ts, class Tp>\ntuple<Ts...> &operator+=(tuple<Ts...> &a,\
    \ const Tp &b)\n{ return internal::tuple_add_impl(a, b, make_index_sequence<tuple_size_v<tuple<Ts...>>>{});\
    \ }\ntemplate <class... Ts, class Tp>\ntuple<Ts...> operator+(tuple<Ts...> a,\
    \ const Tp &b) { return a += b; }\n\ntemplate <class T, class Add>\nvoid offset(vc<T>\
    \ &v, const Add &add) { for (auto &vi : v) vi += add; }\ntemplate <class T, class\
    \ Add>\nvoid offset(vvc<T> &v, const Add &add) { for (auto &vi : v) for (auto\
    \ &vij : vi) vij += add; }\n// ----------\n\n// ----- \u8EE2\u7F6E -----\ntemplate\
    \ <class T, const size_t m>\narray<vc<T>, m> unzip(const vc<array<T, m>> &vt)\n\
    {\n  const size_t n = vt.size();\n  array<vc<T>, m> tv;\n  tv.fill(vc<T>(n));\n\
    \  for (size_t i = 0; i < n; i++)\n    for (size_t j = 0; j < m; j++)\n      tv[j][i]\
    \ = vt[i][j];\n  return tv;\n}\ntemplate <class T, const size_t m>\nvc<array<T,\
    \ m>> zip(const array<vc<T>, m> &tv)\n{\n  if (tv.empty()) return {};\n  const\
    \ size_t n = tv[0].size();\n  vc<array<T, m>> vt(n);\n  for (size_t j = 0; j <\
    \ m; j++)\n  {\n    assert(tv[j].size() == n);\n    for (size_t i = 0; i < n;\
    \ i++)\n      vt[i][j] = tv[j][i];\n  }\n  return vt;\n}\n\ntemplate <class T,\
    \ class U>\npair<vc<T>, vc<U>> unzip(const vc<pair<T, U>> &vt)\n{\n  const size_t\
    \ n = vt.size();\n  pair<vc<T>, vc<U>> tv;\n  tv.first.resize(n), tv.second.resize(n);\n\
    \  for (size_t i = 0; i < n; i++)\n    tie(tv.first[i], tv.second[i]) = vt[i];\n\
    \  return tv;\n}\ntemplate <class T, class U>\nvc<pair<T, U>> zip(const pair<vc<T>,\
    \ vc<U>> &tv)\n{\n  const size_t n = tv.first.size();\n  assert(n == tv.second.size());\n\
    \  vc<pair<T, U>> vt(n);\n  for (size_t i = 0; i < n; i++)\n    vt[i] = make_pair(tv.first[i],\
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
    \ get<0>(tv).size();\n  apply([&](auto &...v)\n        { ((void(v), assert(v.size()\
    \ == n)), ...); }, tv);\n  vc<tuple<Ts...>> vt(n);\n  for (size_t i = 0; i < n;\
    \ i++)\n    vt[i] = internal::tv_to_vt_impl(tv, index_sequence_for<Ts...>{}, i);\n\
    \  return vt;\n}\n\n#define UNZIP(vt, ...) auto [__VA_ARGS__] = unzip(vt)\n#define\
    \ ZIP(vt, ...) auto vt = zip(tuple{__VA_ARGS__})\n// ----------\n#line 2 \"template/template_random.hpp\"\
    \n\n#line 5 \"template/template_random.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\
    \u30EC\u30FC\u30C8\uFF08\u30E9\u30F3\u30C0\u30E0\u751F\u6210\uFF09\n * @docs docs/template/template_random.md\n\
    \ */\n\nmt19937_64 mt;\n\n// [l, r] \u304B\u3089\u7B49\u78BA\u7387\ntemplate <class\
    \ T = ll, class U1, class U2>\nT randint(U1 l, U2 r)\n{\n  assert(T(l) <= T(r));\n\
    \  return uniform_int_distribution<T>(T(l), T(r))(mt);\n}\n// [l, r) \u304B\u3089\
    \u7B49\u78BA\u7387\ntemplate <class T = ll, class U1, class U2>\nT randrange(U1\
    \ l, U2 r)\n{\n  assert(T(l) < T(r));\n  return uniform_int_distribution<T>(T(l),\
    \ T(r) - 1)(mt);\n}\n\n// [l, r) \u304B\u3089\u4E00\u69D8\u30E9\u30F3\u30C0\u30E0\
    \u306A\u5B9F\u6570\u3092\u8FD4\u3059\ntemplate <class T = double, class U1, class\
    \ U2>\nT randreal(U1 l, U2 r)\n{\n  assert(T(l) < T(r));\n  return uniform_real_distribution<T>(T(l),\
    \ T(r))(mt);\n}\n\n// \u78BA\u7387 p \u3067 true \u3092\u8FD4\u3059\nbool randbool(double\
    \ p)\n{\n  assert(0 <= p && p <= 1);\n  return bernoulli_distribution(p)(mt);\n\
    }\n\nnamespace internal\n{\ntemplate <bool does_sort, class V, class T>\nvoid\
    \ random_sample_range(V &res, T l, T r)\n{\n  int k = res.size();\n  T n = r -\
    \ l;\n  if (k <= 256)\n  {\n    repi(i, k)\n    {\n      T j = n - T(k) + T(i),\
    \ x = randint<T>(0, j);\n      if (find(res.begin(), res.begin() + i, x) != res.begin()\
    \ + i)\n        x = j;\n      res[i] = x;\n    }\n  }\n  else\n  {\n    unordered_set<T>\
    \ used;\n    used.reserve(2 * size_t(k));\n    repi(i, k)\n    {\n      T j =\
    \ n - T(k) + T(i), x = randint<T>(0, j);\n      if (!used.insert(x).second)\n\
    \        x = j, used.insert(x);\n      res[i] = x;\n    }\n  }\n  for (T &x :\
    \ res) x += l;\n  if constexpr (does_sort)\n    sort(res.begin(), res.end());\n\
    \  else\n    shuffle(res.begin(), res.end(), mt);\n}\n}; // namespace internal\n\
    \n// [l, r) \u304B\u3089\u76F8\u7570\u306A\u308B k \u500B\u3092\u9078\u3076\n\
    // does_sort: \u30BD\u30FC\u30C8\u3059\u308B\u304B\u3069\u3046\u304B\ntemplate\
    \ <int k, bool does_sort, class T = ll, class U1, class U2>\narray<T, k> random_sample_range_array(U1\
    \ l, U2 r)\n{\n  assert(T(r) - T(l) >= T(k));\n  array<T, k> res;\n  internal::random_sample_range<does_sort>(res,\
    \ T(l), T(r));\n  return res;\n}\n// [l, r) \u304B\u3089\u76F8\u7570\u306A\u308B\
    \ k \u500B\u3092\u9078\u3076\n// does_sort: \u30BD\u30FC\u30C8\u3059\u308B\u304B\
    \u3069\u3046\u304B\ntemplate <bool does_sort, class T = ll, class U1, class U2>\n\
    vc<T> random_sample_range_vector(U1 l, U2 r, int k)\n{\n  assert(k >= 0);\n  assert(T(r)\
    \ - T(l) >= T(k));\n  vc<T> res(k);\n  internal::random_sample_range<does_sort>(res,\
    \ T(l), T(r));\n  return res;\n}\n#line 17 \"verify/yosupo/vertex_set_path_composite.test.cpp\"\
    \n\n#line 2 \"math/modint/modint.hpp\"\n\n#line 4 \"math/modint/modint.hpp\"\n\
    \n#line 2 \"math/modint/modint_internal_static.hpp\"\n\n#line 4 \"math/modint/modint_internal_static.hpp\"\
    \n\n#line 2 \"utils/larger_int.hpp\"\n\n#line 4 \"utils/larger_int.hpp\"\n\n/**\n\
    \ * @brief \u7A4D\u306E\u8A08\u7B97\u306B\u4F7F\u3046\u62E1\u5F35\u578B\n * @docs\
    \ docs/utils/larger_int.md\n */\n\ntemplate <class T>\nstruct larger_int\n{\n\
    \  using type = T;\n};\n\n#define LARGER_INT(T, U) \\\n  template <>         \
    \   \\\n  struct larger_int<T>   \\\n  {                      \\\n    using type\
    \ = U;      \\\n  };\n\nLARGER_INT(signed char, short)\nLARGER_INT(short, int)\n\
    LARGER_INT(int, long long)\nLARGER_INT(long, __int128_t)\nLARGER_INT(long long,\
    \ __int128_t)\n\nLARGER_INT(unsigned char, unsigned short)\nLARGER_INT(unsigned\
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
    \ m; }\n  ull reduce(u128 x) const\n  {\n    if (b == 0)\n    {\n      auto t\
    \ = (x + u128(mx) * (-imx * ull(x))) >> 64;\n      if (t >= m) t -= m;\n     \
    \ return (ull)t;\n    }\n    ull p = x & MASK(b); \n    x = (x >> b) + p * d;\n\
    \    ull y = p << (64 - b);\n    auto t = (x + u128(mx) * (imx * (y - ull(x))))\
    \ >> (64 - b);\n    if (t >= m) { t -= m; if (t >= m) t -= m; }\n    return (ull)t;\n\
    \  }\n  ull inv_reduce(i128 v) const { return reduce(u128(v % m + m) * q); }\n\
    };\n\ntemplate <int id>\nstruct policy_montgomery64_odd\n{\n  using value_type\
    \ = ull;\n  using calc_type = u128;\n  using mod_type = ll;\n\n  static constexpr\
    \ bool is_prime = false;\n  static inline montgomery64odd reducer{(1LL << 61)\
    \ - 1};\n  static void set_mod(mod_type m) { reducer = montgomery64odd(m); }\n\
    \  static mod_type mod() { return reducer.umod(); }\n  static value_type umod()\
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
    \    else\n      return std::hash<mint>{}(x);\n  }\n};\n#line 19 \"verify/yosupo/vertex_set_path_composite.test.cpp\"\
    \nusing mint = modint998244353;\n\n#line 2 \"graph/tree/rooted_tree.hpp\"\n\n\
    #line 4 \"graph/tree/rooted_tree.hpp\"\n\n/**\n * @brief \u6839\u3064\u304D\u6728\
    \u30AF\u30E9\u30B9\uFF08HLD \u3082\u3059\u308B\uFF09\n * @docs docs/graph/tree/rooted_tree.md\n\
    \ */\n\n// \u53C2\u8003: https://judge.yosupo.jp/submission/349844\n\n// \u30B3\
    \u30F3\u30B9\u30C8\u30E9\u30AF\u30BF\u306F\n// - n, es, root (**es \u306B\u30B3\
    \u30B9\u30C8\u306E\u60C5\u5831\u306F\u5165\u308C\u306A\u3044**)\n// - n, p\n//\
    \ \u306E\u3044\u305A\u308C\u304B\nstruct RootedTree\n{\nprotected:\n  static const\
    \ int MSK = 1 << 30;\n  int n;\n  vc<int> p_head, dep, pre, post, preinv;\n  //\
    \ \u69CB\u7BC9\u5F8C\u306E p_head[v] \u306E\u5024:\n  // - MSK \u304C\u7ACB\u3063\
    \u3066\u3044\u308B\u5834\u5408\u3001v \u306F heavy path \u306E\u5148\u982D\u3067\
    \u3001MSK \u3092\u53D6\u3063\u305F\u3082\u306E\u304C\u89AA\n  // - MSK \u304C\u7ACB\
    \u3063\u3066\u3044\u306A\u3044\u5834\u5408\u3001v \u304C\u5C5E\u3059\u308B heavy\
    \ path \u306E\u5148\u982D\n\n  // v \u304C\u5C5E\u3059\u308B heavy path \u306E\
    \u5148\u982D\n  inline int internal_head(int v) const { return (p_head[v] & MSK)\
    \ ? v : p_head[v]; }\n  // hv \u304C heavy path \u306E\u5148\u982D\u3067\u3042\
    \u308B\u3068\u3057\u3066\u3001hv \u306E\u89AA\u3002hv \u304C\u6839\u306E\u3068\
    \u304D\u306F\u6839\u3092\u8FD4\u3059\n  inline int parent_of_head(int hv) const\
    \ { return p_head[hv] & ~MSK; }\n\n  // p_head \u306B\u89AA\u914D\u5217\u304C\u683C\
    \u7D0D\u3055\u308C\u3066\u3044\u308B\u72B6\u614B\u304B\u3089\u30B9\u30BF\u30FC\
    \u30C8\n  void build(int rt, const vc<int> &siz, vc<int> &max_chi_siz)\n  {\n\
    \    // \u90E8\u5206\u6728\u30B5\u30A4\u30BA\u9806\u306B\u4E26\u3079\u308B\n \
    \   vc<int> ord(n);\n    {\n      vc<int> cnt(n + 1);\n      repi(i, n) cnt[siz[i]]++;\n\
    \      cnt = cumlsum(cnt);\n      repi(i, n) ord[n - 1 - (cnt[siz[i]]++)] = i;\n\
    \    }\n    // \u5FC5\u8981\u306A\u914D\u5217\u306E\u8A08\u7B97\n    {\n     \
    \ p_head[rt] = rt | MSK, post[rt] = n;\n      vc<int> add(n, 1);\n      repi(i,\
    \ 1, n)\n      {\n        int v = ord[i];\n        int p = p_head[v];\n      \
    \  dep[v] = dep[p] + 1;\n        pre[v] = pre[p] + add[p];\n        add[p] +=\
    \ siz[v];\n        post[v] = pre[v] + siz[v];\n        if (max_chi_siz[p] == siz[v])\n\
    \        {\n          max_chi_siz[p] = 0;\n          p_head[v] = internal_head(p_head[v]);\n\
    \        }\n        else\n          p_head[v] |= MSK;\n      }\n      preinv =\
    \ perminv(pre);\n    }\n  }\n\npublic:\n  RootedTree() {}\n  template <class I>\n\
    \  RootedTree(int n, const vc<I> &par)\n  : n(n), p_head(n), dep(n), pre(n), post(n),\
    \ preinv(n)\n  {\n    assert(n >= 1);\n    assert(SZ(par) == n);\n    int rt =\
    \ -1;\n    vc<int> deg(n), siz(n, 1), max_chi_siz(n);\n    repi(i, n)\n    {\n\
    \      if (par[i] < 0 || par[i] == i)\n      {\n        assert(rt == -1 && \"\
    There are more than two roots\");\n        rt = i;\n      }\n      else\n    \
    \  {\n        p_head[i] = par[i];\n        deg[par[i]]++;\n      }\n    }\n  \
    \  assert(rt != -1 && \"There is no root\");\n    repi(i, n)\n    {\n      int\
    \ v = i;\n      while (v != rt && deg[v] == 0)\n      {\n        int p = p_head[v];\n\
    \        deg[p]--, deg[v]--;\n        siz[p] += siz[v];\n        chmax(max_chi_siz[p],\
    \ siz[v]);\n        v = p;\n      }\n    }\n    build(rt, siz, max_chi_siz);\n\
    \  }\n  template <class P>\n  RootedTree(int n, const vc<P> &es, int rt)\n  :\
    \ n(n), p_head(n), dep(n), pre(n), post(n), preinv(n)\n  {\n    assert(n >= 1);\n\
    \    assert(SZ(es) == n - 1);\n    assert(0 <= rt && rt < n);\n\n    vc<int> deg(n),\
    \ siz(n, 1), max_chi_siz(n);\n    fec([ u, v ] : es)\n    {\n      deg[u]++, deg[v]++;\n\
    \      p_head[u] ^= v, p_head[v] ^= u;\n    }\n    deg[rt] = 0;\n    repi(i, n)\n\
    \    {\n      int v = i;\n      while (deg[v] == 1)\n      {\n        int p =\
    \ p_head[v];\n        deg[p]--, deg[v]--, p_head[p] ^= v;\n        siz[p] += siz[v];\n\
    \        chmax(max_chi_siz[p], siz[v]);\n        v = p;\n      }\n    }\n    build(rt,\
    \ siz, max_chi_siz);\n  }\n\n  // \u9802\u70B9\u6570\u3092\u8FD4\u3059\n  template\
    \ <class I = ll>\n  I size() const { return n; }\n  // \u6839\u3092\u8FD4\u3059\
    \n  int root() const { return preinv[0]; }\n\n  // v \u306E\u6DF1\u3055\n  template\
    \ <class I = ll>\n  I depth(int v) const\n  {\n    assert(0 <= v && v < n);\n\
    \    return dep[v];\n  }\n\n  // v \u3092\u6839\u3068\u3059\u308B\u90E8\u5206\u6728\
    \u306E\u30B5\u30A4\u30BA (v \u3082\u542B\u3080)\n  template <class I = ll>\n \
    \ I subtree_size(int v) const\n  {\n    assert(0 <= v && v < n);\n    return post[v]\
    \ - pre[v];\n  }\n  // \u8FBA (u, v) \u304C\u5B58\u5728\u3059\u308B\u3068\u3057\
    \u3066\u3001\u3053\u308C\u3092\u5207\u3063\u305F\u3068\u304D\u306E\n  // u \u5074\
    \u3001v \u5074\u305D\u308C\u305E\u308C\u306E\u9023\u7D50\u6210\u5206\u30B5\u30A4\
    \u30BA\n  template <class I = ll>\n  pair<I, I> cut_and_subtree_size(int u, int\
    \ v) const\n  {\n    assert(0 <= u && u < n);\n    assert(0 <= v && v < n);\n\
    \    if (dep[u] < dep[v])\n      return {n - subtree_size(v), subtree_size(v)};\n\
    \    else\n      return {subtree_size(u), n - subtree_size(u)};\n  }\n\n  // v\
    \ \u306E\u89AA\u3092\u8FD4\u3059\u3002v \u306F\u6839\u3067\u3042\u3063\u3066\u306F\
    \u3044\u3051\u306A\u3044\n  int parent(int v) const\n  {\n    assert(0 <= v &&\
    \ v < n);\n    assert(v != root());\n    int ph = p_head[v];\n    if (ph & MSK)\n\
    \      return ph & ~MSK;\n    else\n      return preinv[pre[v] - 1];\n  }\n\n\
    \  // v \u304C\u5C5E\u3059\u308B heavy path \u306E\u5148\u982D\u3092\u8FD4\u3059\
    \n  int head(int v) const\n  {\n    assert(0 <= v && v < n);\n    return internal_head(v);\n\
    \  }\n\n  // v \u306E preorder\n  int preorder(int v) const\n  {\n    assert(0\
    \ <= v && v < n);\n    return pre[v];\n  }\n  // v \u306E postorder\n  int postorder(int\
    \ v) const\n  {\n    assert(0 <= v && v < n);\n    return post[v];\n  }\n  //\
    \ preorder \u304C i \u306E\u9802\u70B9\n  int preorder_select(int i) const\n \
    \ {\n    assert(0 <= i && i < n);\n    return preinv[i];\n  }\n\n  // v \u304B\
    \u3089\u6839\u307E\u3067\u306E\u30D1\u30B9\u3067\u901A\u308B\u9802\u70B9\u306E\
    \u5217\n  vc<int> path_to_root(int v) const\n  {\n    assert(0 <= v && v < n);\n\
    \    vc<int> res = {v};\n    res.reserve(dep[v] + 1);\n    int rt = root();\n\
    \    while (v != rt)\n    {\n      int p = parent(v);\n      res.eb(p);\n    \
    \  v = p;\n    }\n    return res;\n  }\n  // u \u304B\u3089 v \u307E\u3067\u306E\
    \u30D1\u30B9\u3067\u901A\u308B\u9802\u70B9\u306E\u5217\n  vc<int> path(int u,\
    \ int v) const\n  {\n    assert(0 <= u && u < n);\n    assert(0 <= v && v < n);\n\
    \    vc<int> pu = {u}, pv = {v};\n    while (u != v)\n    {\n      if (dep[u]\
    \ > dep[v])\n      {\n        int p = parent(u);\n        pu.eb(p);\n        u\
    \ = p;\n      }\n      else\n      {\n        int p = parent(v);\n        pv.eb(p);\n\
    \        v = p;\n      }\n    }\n    pv.pop_back();\n    return concat(pu, reversed(pv));\n\
    \  }\n\n  // \u6839\u306B\u8FD1\u3044\u9802\u70B9\u307B\u3069\u5148\u306B\u6765\
    \u308B\u9806\u5E8F\u3067\u4E26\u3079\u305F\u9802\u70B9\u306E\u5217\u3092\u3072\
    \u3068\u3064\u8FD4\u3059\n  const vc<int> &top_down_vertices() const { return\
    \ preinv; }\n  // \u6DF1\u3044\u9802\u70B9\u307B\u3069\u5148\u306B\u6765\u308B\
    \u9806\u5E8F\u3067\u4E26\u3079\u305F\u9802\u70B9\u306E\u5217\u3092\u8FD4\u3059\
    \n  vc<int> bottom_up_vertices() const { return reversed(top_down_vertices());\
    \ }\n\n  // bp: \u9577\u3055 2(n-1) \u306E\u62EC\u5F27\u5217\n  // vs: \u9577\u3055\
    \ 2(n-1)+1 \u306E\u9802\u70B9\u5217\n  // 0 <= i < 2(n-1) \u306B\u5BFE\u3057\u3001\
    i \u56DE\u76EE\u306E\u79FB\u52D5\u306F bp[i] \u306E\u5411\u304D\u3067 vs[i] \u304B\
    \u3089 vs[i+1] \u3078\n  pair<string, vc<int>> dfs_ordered_vertices() const\n\
    \  {\n    string bp(2 * (n - 1), '?');\n    vc<int> vs(2 * (n - 1) + 1);\n   \
    \ int rt = root();\n    vs[0] = rt;\n    repi(v, n)\n    {\n      if (v == rt)\n\
    \        continue;\n      int p = parent(v);\n      int in_idx = 2 * pre[v] -\
    \ dep[v] - 1;\n      int out_idx = 2 * post[v] - dep[v] - 2;\n      bp[in_idx]\
    \ = '(', vs[in_idx + 1] = v;\n      bp[out_idx] = ')', vs[out_idx + 1] = p;\n\
    \    }\n    return {bp, vs};\n  }\n\n  class ChildIterator;\n  class ChildRange;\n\
    \n  class ChildIterator\n  {\n  private:\n    const RootedTree* t;\n    int cur;\n\
    \    friend class RootedTree;\n    friend class ChildRange;\n    ChildIterator(const\
    \ RootedTree* t, int cur) : t(t), cur(cur) {}\n\n  public:\n    using iterator_category\
    \ = std::input_iterator_tag;\n    using value_type        = int;\n    using difference_type\
    \   = std::ptrdiff_t;\n    using pointer           = int*;\n    using reference\
    \         = int;\n    int operator*() const { return t->preinv[cur]; }\n    void\
    \ operator++() { cur = t->post[t->preinv[cur]]; }\n    bool operator!=(const ChildIterator&\
    \ r) const { return cur != r.cur; }\n    bool operator==(const ChildIterator&\
    \ r) const { return cur == r.cur; }\n  };\n\n  class ChildRange\n  {\n  private:\n\
    \    const RootedTree* t;\n    int cur, end_pos;\n    friend class RootedTree;\n\
    \    ChildRange(const RootedTree* t, int cur, int end_pos) : t(t), cur(cur), end_pos(end_pos)\
    \ {}\n\n  public:\n    ChildIterator begin() const { return ChildIterator(t, cur);\
    \ }\n    ChildIterator end() const { return ChildIterator(t, end_pos); }\n   \
    \ bool empty() const { return cur == end_pos; }\n    vc<int> to_v() const { return\
    \ vc<int>(begin(), end()); }\n  };\n\n  // v \u306E heavy child (\u306A\u3051\u308C\
    \u3070 -1)\n  int heavy_child(int v) const\n  {\n    assert(0 <= v && v < n);\n\
    \    if (post[v] - pre[v] == 1)\n      return -1;\n    return preinv[pre[v] +\
    \ 1];\n  }\n  ChildRange children(int v) const\n  {\n    assert(0 <= v && v <\
    \ n);\n    return {this, pre[v] + 1, post[v]};\n  }\n  ChildRange light_children(int\
    \ v) const\n  {\n    assert(0 <= v && v < n);\n    int cur = pre[v] + 1;\n   \
    \ if (cur < post[v])\n    {\n      cur = post[preinv[cur]];\n    }\n    return\
    \ {this, cur, post[v]};\n  }\n\n  // \u8FBA (u, v) \u304C\u5B58\u5728\u3059\u308B\
    \u3068\u3057\u3066\u3001(\u89AA\u5074, \u5B50\u5074)\n  pair<int, int> parent_child(int\
    \ u, int v) const\n  {\n    assert(0 <= u && u < n);\n    assert(0 <= v && v <\
    \ n);\n    return dep[u] < dep[v] ? pair{u, v} : pair{v, u};\n  }\n  // \u9577\
    \u3055 n \u306E\u914D\u5217\u3002v \u756A\u76EE\u306B\u306F v \u304C\u5B50\u5074\
    \u306B\u306A\u308B\u8FBA\u306E\u60C5\u5831\u304C\u683C\u7D0D\u3055\u308C\u308B\
    \u3002\u6839\u306F\u672A\u5B9A\u7FA9\u3002\n  template <class P, class EdgeInfo>\n\
    \  vc<EdgeInfo> reordered_edge_info(const vc<P> &es, const vc<EdgeInfo> &edge_info)\
    \ const\n  {\n    assert(SZ(es) == n - 1 && SZ(edge_info) >= n - 1);\n    vc<EdgeInfo>\
    \ res(n);\n    repi(i, n - 1)\n    {\n      auto [u, v] = es[i];\n      int eid\
    \ = dep[u] < dep[v] ? v : u;\n      res[eid] = edge_info[i];\n    }\n    return\
    \ res;\n  }\n\n  int lca(int u, int v) const\n  {\n    assert(0 <= u && u < n);\n\
    \    assert(0 <= v && v < n);\n    while (true)\n    {\n      int hu = internal_head(u),\
    \ hv = internal_head(v);\n      if (hu == hv)\n        break;\n      if (dep[hu]\
    \ > dep[hv])\n        swap(u, v), swap(hu, hv);\n      v = parent_of_head(hv);\n\
    \    }\n    return dep[u] < dep[v] ? u : v;\n  }\n\n  template <class I = ll>\n\
    \  I dist(int u, int v) const\n  {\n    assert(0 <= u && u < n);\n    assert(0\
    \ <= v && v < n);\n    return dep[u] + dep[v] - 2 * dep[lca(u, v)];\n  }\n\n \
    \ // u \u304C v \u306E\u7956\u5148\u304B\n  bool is_ancestor(int u, int v) const\n\
    \  {\n    assert(0 <= u && u < n);\n    assert(0 <= v && v < n);\n    return pre[u]\
    \ <= pre[v] && pre[v] < post[u];\n  }\n\n  // v \u306E k \u500B\u4E0A\u3002\u306A\
    \u3051\u308C\u3070 -1\n  int la(int v, int k) const\n  {\n    assert(0 <= v &&\
    \ v < n);\n    assert(0 <= k);\n    if (k > dep[v])\n      return -1;\n    while\
    \ (true)\n    {\n      int hv = internal_head(v);\n      if (k <= dep[v] - dep[hv])\n\
    \        break;\n      k -= dep[v] - dep[hv] + 1;\n      v = parent_of_head(hv);\n\
    \    }\n    return preinv[pre[v] - k];\n  }\n\n  // u \u304B\u3089 v \u306B k\
    \ \u500B\u9032\u3080\u3002\u306A\u3051\u308C\u3070 -1\n  int jump(int u, int v,\
    \ int k) const\n  {\n    assert(0 <= u && u < n);\n    assert(0 <= v && v < n);\n\
    \    assert(0 <= k);\n    int l = lca(u, v);\n    int d_ul = dep[u] - dep[l];\n\
    \    int d_vl = dep[v] - dep[l];\n    if (k > d_ul + d_vl)\n      return -1;\n\
    \    if (k <= d_ul)\n      return la(u, k);\n    return la(v, d_ul + d_vl - k);\n\
    \  }\n\n  // costs[v] := (parent(v), v) \u306E\u30B3\u30B9\u30C8 \u3068\u3057\u305F\
    \u3068\u304D\u306E\u3001\u5404\u9802\u70B9\u306E\u6DF1\u3055\n  // costs[root()]\
    \ \u306F\u53C2\u7167\u3055\u308C\u306A\u3044\n  // \u4F7F\u3046\u969B\u306F reordered_edge_info\
    \ \u95A2\u6570\u3082\u9069\u5207\u306B\u6D3B\u7528\u3059\u308B\u3068\u3088\u3044\
    \n  template <class T>\n  vc<T> weighted_depths(vc<T> costs) const\n  {\n    vc<T>\
    \ res(n, 0);\n    auto vs = top_down_vertices();\n    costs[root()] = 0;\n   \
    \ rep(i, 1, n)\n    {\n      int v = vs[i], p = parent(v);\n      costs[v] +=\
    \ costs[p];\n    }\n    return costs;\n  }\n\n  // \u9802\u70B9\u30C7\u30FC\u30BF\
    \u3092\u884C\u304D\u304C\u3051\u9806\u306B\u4E26\u3079\u66FF\u3048\u308B\n  template\
    \ <class T>\n  vc<T> reordered_vertex_info(const vc<T> &vertex_info) const\n \
    \ {\n    return permuted(vertex_info, preinv);\n  }\n  // v \u306E\u90E8\u5206\
    \u6728\u304C\u884C\u304D\u304C\u3051\u9806 [l, r) \u306E\u9802\u70B9\u3067\u3042\
    \u308B\u3088\u3046\u306A (l, r)\n  // edge \u306E\u3068\u304D\u306F\u8FBA\u5C5E\
    \u6027 (v \u3092\u9664\u304F)\n  pair<int, int> subtree_interval(int v, bool edge\
    \ = false) const\n  {\n    return {pre[v] + edge, post[v]};\n  }\n  // u \u304B\
    \u3089 v \u3078\u306E\u30D1\u30B9\u304C\n  // \u884C\u304D\u304C\u3051\u9806 [l_1,\
    \ r_1), ..., [l_k, r_k) \u306E\u9802\u70B9 (\u3053\u306E\u9806) \u3067\u3042\u308B\
    \u3068\u304D\n  // f(l_1, r_1, isrev), ..., f(l_k, r_k, isrev) \u3092\u9806\u306B\
    \u5B9F\u884C\u3059\u308B\n  // isrev \u306F\u9006\u5411\u304D\u306E\u3068\u304D\
    \n  // edge \u306E\u3068\u304D\u306F\u8FBA\u5C5E\u6027 (lca(u, v) \u3092\u9664\
    \u304F)\n  template <class F>\n  void path_query(int u, int v, F f, bool edge\
    \ = false) const\n  {\n    assert(0 <= u && u < n);\n    assert(0 <= v && v <\
    \ n);\n    static pair<int, int> down_path[30];\n    int down_cnt = 0;\n    while\
    \ (true)\n    {\n      int hu = internal_head(u), hv = internal_head(v);\n   \
    \   if (hu == hv)\n        break;\n      if (dep[hu] > dep[hv])\n      {\n   \
    \     f(pre[hu], pre[u] + 1, true);\n        u = parent_of_head(hu);\n      }\n\
    \      else\n      {\n        down_path[down_cnt++] = {pre[hv], pre[v] + 1};\n\
    \        v = parent_of_head(hv);\n      }\n    }\n\n    if (dep[u] > dep[v])\n\
    \    {\n      int l = pre[v] + edge;\n      int r = pre[u] + 1;\n      if (l <\
    \ r)\n        f(l, r, true);\n    }\n    else\n    {\n      int l = pre[u] + edge;\n\
    \      int r = pre[v] + 1;\n      if (l < r)\n        f(l, r, false);\n    }\n\
    \n    repi(i, down_cnt - 1, -1, -1)\n    {\n      auto [l, r] = down_path[i];\n\
    \      f(l, r, false);\n    }\n  }\n};\n#line 2 \"ds/segtree/segtree.hpp\"\n\n\
    #line 4 \"ds/segtree/segtree.hpp\"\n\n/**\n * @brief \u30BB\u30B0\u30E1\u30F3\u30C8\
    \u6728\n * @docs docs/ds/segtree/segtree.md\n */\n\ntemplate <class M>\nstruct\
    \ SegmentTree\n{\n  using S = typename M::S;\n\nprivate:\n  int n, siz;\n  vc<S>\
    \ dat;\n  void update(int i) { dat[i] = M::op(dat[2 * i], dat[2 * i + 1]); }\n\
    \npublic:\n  SegmentTree() {}\n  SegmentTree(int n) : SegmentTree(vc<S>(n, M::e()))\
    \ {}\n  template <class Iter>\n  SegmentTree(const Iter &bg, const Iter &ed) :\
    \ SegmentTree(vc<S>(bg, ed)) {}\n  template <class T>\n  SegmentTree(const vc<T>\
    \ &vec) : n(vec.size()), siz(bit_ceil(vec.size())), dat(2 * siz, M::e())\n  {\n\
    \    repi(i, vec.size()) dat[siz + i] = vec[i];\n    repi(i, siz - 1, 0, -1) update(i);\n\
    \  }\n\n  void set(int p, const S &x)\n  {\n    assert(0 <= p && p < n);\n   \
    \ p += siz;\n    dat[p] = x;\n    while (p)\n      p >>= 1, update(p);\n  }\n\n\
    \  S get(int p) const\n  {\n    assert(0 <= p && p < n);\n    return dat[siz +\
    \ p];\n  }\n  S prod(int l, int r) const\n  {\n    assert(0 <= l && l <= r &&\
    \ r <= n);\n    l += siz, r += siz;\n    S sml = M::e(), smr = M::e();\n    while\
    \ (l < r)\n    {\n      if (l & 1)\n        sml = M::op(sml, dat[l++]);\n    \
    \  if (r & 1)\n        smr = M::op(dat[--r], smr);\n      l >>= 1, r >>= 1;\n\
    \    }\n    return M::op(sml, smr);\n  }\n  S all_prod() const { return dat[1];\
    \ }\n\n  // g(prod(l, r)) \u304C true \u3068\u306A\u308B\u6700\u5927\u306E r \u3092\
    \u8FD4\u3059\n  template <class G>\n  int max_right_ok(int l, const G &g) const\n\
    \  {\n    assert(0 <= l && l <= n);\n    assert(g(M::e()));\n    if (l == n)\n\
    \      return n;\n    l += siz;\n    S sm = M::e();\n    do\n    {\n      while\
    \ (l % 2 == 0)\n        l >>= 1;\n      if (!g(M::op(sm, dat[l])))\n      {\n\
    \        while (l < siz)\n        {\n          l *= 2;\n          if (g(M::op(sm,\
    \ dat[l])))\n          {\n            sm = M::op(sm, dat[l]);\n            l++;\n\
    \          }\n        }\n        return l - siz;\n      }\n      sm = M::op(sm,\
    \ dat[l]);\n      l++;\n    } while ((l & -l) != l);\n    return n;\n  }\n\n \
    \ // g(prod(l, r)) \u304C true \u3068\u306A\u308B\u6700\u5C0F\u306E l \u3092\u8FD4\
    \u3059\n  template <class G>\n  int min_left_ok(int r, const G &g) const\n  {\n\
    \    assert(0 <= r && r <= n);\n    assert(g(M::e()));\n    if (r == 0)\n    \
    \  return 0;\n    r += siz;\n    S sm = M::e();\n    do\n    {\n      r--;\n \
    \     while (r > 1 && (r % 2))\n        r >>= 1;\n      if (!g(M::op(dat[r], sm)))\n\
    \      {\n        while (r < siz)\n        {\n          r = 2 * r + 1;\n     \
    \     if (g(M::op(dat[r], sm)))\n          {\n            sm = M::op(dat[r], sm);\n\
    \            r--;\n          }\n        }\n        return r + 1 - siz;\n     \
    \ }\n      sm = M::op(dat[r], sm);\n    } while ((r & -r) != r);\n    return 0;\n\
    \  }\n\n  vc<S> content() const\n  {\n    vc<S> res(n);\n    repi(i, n) res[i]\
    \ = get(i);\n    return res;\n  }\n};\n#line 2 \"math/algebra/affine_function.hpp\"\
    \n\n#line 2 \"math/algebra/algebra_base.hpp\"\n\n#line 4 \"math/algebra/algebra_base.hpp\"\
    \n\n/**\n * @brief \u4EE3\u6570\u7684\u69CB\u9020\u306E struct\uFF08\u57FA\u672C\
    \uFF09\n * @docs docs/math/algebra/algebra_base.md\n */\n\ntemplate <class S_,\
    \ auto op_, auto e_>\nstruct Monoid\n{\n  using S = S_;\n  static constexpr auto\
    \ op = op_;\n  static constexpr auto e = e_;\n};\n\ntemplate <class S_, auto op_,\
    \ auto e_, auto inv_>\nstruct Group\n{\n  using S = S_;\n  static constexpr auto\
    \ op = op_;\n  static constexpr auto e = e_;\n  static constexpr auto inv = inv_;\n\
    };\n\ntemplate <class S_, auto add_, auto e0_, auto mul_, auto e1_>\nstruct SemiRing\n\
    {\n  using S = S_;\n  static constexpr auto add = add_;\n  static constexpr auto\
    \ e0 = e0_;\n  static constexpr auto mul = mul_;\n  static constexpr auto e1 =\
    \ e1_;\n};\n\ntemplate <class S_, auto add_, auto e0_, auto minus_, auto mul_,\
    \ auto e1_>\nstruct Ring\n{\n  using S = S_;\n  static constexpr auto add = add_;\n\
    \  static constexpr auto e0 = e0_;\n  static constexpr auto minus = minus_;\n\
    \  static constexpr auto mul = mul_;\n  static constexpr auto e1 = e1_;\n};\n\n\
    template <class S_, auto add_, auto e0_, auto minus_, auto mul_, auto e1_, auto\
    \ inv_>\nstruct Field\n{\n  using S = S_;\n  static constexpr auto add = add_;\n\
    \  static constexpr auto e0 = e0_;\n  static constexpr auto minus = minus_;\n\
    \  static constexpr auto mul = mul_;\n  static constexpr auto e1 = e1_;\n  static\
    \ constexpr auto inv = inv_;\n};\n\ntemplate <class M>\nstruct OppositeMonoid\n\
    {\n  using S = typename M::S;\n  static constexpr S op(const S &a, const S &b)\
    \ { return M::op(b, a); }\n  static constexpr auto e = M::e;\n};\ntemplate <class\
    \ G>\nstruct OppositeGroup\n{\n  using S = typename G::S;\n  static constexpr\
    \ S op(const S &a, const S &b) { return G::op(b, a); }\n  static constexpr auto\
    \ e = G::e;\n  static constexpr auto inv = G::inv;\n};\ntemplate <class M>\nstruct\
    \ NormalAndOppositeMonoid\n{\n  struct S\n  {\n    typename M::S normal;\n   \
    \ typename M::S opposite;\n    S() {}\n    template <class... Args,\n        \
    \      std::enable_if_t<std::is_constructible_v<typename M::S, Args...>, std::nullptr_t>\
    \ = nullptr>\n    S(Args &&...args)\n        : normal(std::forward<Args>(args)...),\
    \ opposite(normal) {}\n    S rev() { return {opposite, normal}; }\n    S(const\
    \ typename M::S &normal, const typename M::S &opposite) : normal(normal), opposite(opposite)\
    \ {}\n  };\n  static constexpr S op(const S &a, const S &b) { return {M::op(a.normal,\
    \ b.normal), M::op(b.opposite, a.opposite)}; }\n  static constexpr S e() { return\
    \ {M::e(), M::e()}; }\n};\ntemplate <class G>\nstruct NormalAndOppositeGroup\n\
    {\n  struct S\n  {\n    typename G::S normal;\n    typename G::S opposite;\n \
    \   S() {}\n    template <class... Args,\n              std::enable_if_t<std::is_constructible_v<typename\
    \ G::S, Args...>, std::nullptr_t> = nullptr>\n    S(Args &&...args)\n        :\
    \ normal(std::forward<Args>(args)...), opposite(normal) {}\n    S rev() { return\
    \ {opposite, normal}; }\n    S(const typename G::S &normal, const typename G::S\
    \ &opposite) : normal(normal), opposite(opposite) {}\n  };\n  static constexpr\
    \ S op(const S &a, const S &b) { return {G::op(a.normal, b.normal), G::op(b.opposite,\
    \ a.opposite)}; }\n  static constexpr S e() { return {G::e(), G::e()}; }\n  static\
    \ constexpr S inv(const S &a) { return {G::inv(a.normal), G::inv(a.opposite)};\
    \ }\n};\n\ntemplate <class SR>\nusing MonoidOfSemiRingAdd = Monoid<typename SR::S,\
    \ SR::add, SR::e0>;\ntemplate <class SR>\nusing MonoidOfSemiRingMul = Monoid<typename\
    \ SR::S, SR::mul, SR::e1>;\ntemplate <class R>\nusing GroupOfRingAdd = Group<typename\
    \ R::S, R::add, R::e0, R::minus>;\ntemplate <class K>\nusing GroupOfFieldMul =\
    \ Group<typename K::S, K::mul, K::e1, K::inv>;\n\n// Madd \u306F\u53EF\u63DB\n\
    template <class Madd, class Mmul>\nstruct SemiRingFromMonoidMonoid\n{\n  static_assert(is_same_v<typename\
    \ Madd::S, typename Mmul::S>, \"Madd::S and Mmul::S must be identical\");\n  using\
    \ S = typename Madd::S;\n  static constexpr auto add = Madd::op;\n  static constexpr\
    \ auto e0 = Madd::e;\n  static constexpr auto mul = Mmul::op;\n  static constexpr\
    \ auto e1 = Mmul::e;\n};\n\n// Gadd \u306F\u53EF\u63DB\ntemplate <class Gadd,\
    \ class Mmul>\nstruct RingFromGroupMonoid\n{\n  static_assert(is_same_v<typename\
    \ Gadd::S, typename Mmul::S>, \"Gadd::S and Mmul::S must be identical\");\n  using\
    \ S = typename Gadd::S;\n  static constexpr auto add = Gadd::op;\n  static constexpr\
    \ auto e0 = Gadd::e;\n  static constexpr auto minus = Gadd::inv;\n  static constexpr\
    \ auto mul = Mmul::op;\n  static constexpr auto e1 = Mmul::e;\n};\n\n// Gadd,\
    \ Gmul \u306F\u53EF\u63DB\ntemplate <class Gadd, class Gmul>\nstruct FieldFromGroupGroup\n\
    {\n  static_assert(is_same_v<typename Gadd::S, typename Gmul::S>, \"Gadd::S and\
    \ Gmul::S must be identical\");\n  using S = typename Gadd::S;\n  static constexpr\
    \ auto add = Gadd::op;\n  static constexpr auto e0 = Gadd::e;\n  static constexpr\
    \ auto minus = Gadd::inv;\n  static constexpr auto mul = Gmul::op;\n  static constexpr\
    \ auto e1 = Gmul::e;\n  static constexpr auto inv = Gmul::inv;\n};\n#line 5 \"\
    math/algebra/affine_function.hpp\"\n\n/**\n * @brief \u4EE3\u6570\u7684\u69CB\u9020\
    \uFF08\u4E00\u6B21\u95A2\u6570\uFF09\n * @docs docs/math/algebra/affine_function.md\n\
    \ */\n\n// op(f, g) = f \u2218 g\ntemplate <class mint>\nstruct GroupAffineFunction\n\
    {\n  struct S\n  {\n    mint a, b;\n    S() {}\n    S(mint a, mint b) : a(a),\
    \ b(b) {}\n    template <class T, class U>\n    S(const pair<T, U> &ab) : a(ab.first),\
    \ b(ab.second) {}\n  };\n  static constexpr S op(const S &f, const S &g)\n  {\n\
    \    // f.a (g.a x + g.b) + f.b\n    return {f.a * g.a, f.a * g.b + f.b};\n  }\n\
    \  static constexpr S e() { return {1, 0}; }\n  static constexpr S inv(const S\
    \ &f)\n  {\n    // y = ax + b <=> x = (y-b)/a\n    mint ainv = 1 / f.a;\n    return\
    \ {ainv, -f.b * ainv};\n  }\n};\n#ifdef LOCAL\nCPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(a,\
    \ b);\n#endif\n#line 24 \"verify/yosupo/vertex_set_path_composite.test.cpp\"\n\
    \nvoid init() {}\n\nvoid main2()\n{\n  LL(N, Q);\n  VEC(pll, N, AB);\n  VEC(pll,\
    \ N - 1, UV);\n  RootedTree G(N, UV, randrange(0, N));\n  using M = NormalAndOppositeGroup<OppositeGroup<GroupAffineFunction<mint>>>;\n\
    \  SegmentTree<M> seg(G.reordered_vertex_info(AB));\n  rep(_, Q)\n  {\n    LL(t);\n\
    \    if (t == 0)\n    {\n      LL(p, c, d);\n      seg.set(G.preorder(p), pair{c,\
    \ d});\n    }\n    else if (t == 1)\n    {\n      LL(u, v, x);\n      auto res\
    \ = M::e();\n      G.path_query(u, v, [&](ll l, ll r, bool isrev)\n          \
    \         {\n      auto tmp = seg.prod(l, r);\n      res = M::op(res, isrev ?\
    \ tmp.rev() : tmp);});\n      auto [c, d] = res.normal;\n      PRINT(c * x + d);\n\
    \    }\n  }\n}\n\nvoid test() {}\n\n#line 2 \"template/template_main.hpp\"\n\n\
    #line 4 \"template/template_main.hpp\"\n\ntemplate <auto init, auto main2, auto\
    \ test>\nstruct Main\n{\n  Main()\n  {\n    cauto CERR = [](string val, string\
    \ color)\n    {\n      string s = \"\\033[\" + color + \"m\" + val + \"\\033[m\"\
    ;\n      #ifdef LOCAL\n      cerr << s;\n      #endif\n      /* \u30B3\u30FC\u30C9\
    \u30C6\u30B9\u30C8\u3067\u78BA\u8A8D\u3059\u308B\u969B\u306B\u30B3\u30E1\u30F3\
    \u30C8\u30A2\u30A6\u30C8\u3092\u5916\u3059\n      cerr << val;\n      //*/\n \
    \   };\n  \n    #if defined FAST_IO and not defined LOCAL\n    CERR(\"\\n[FAST_IO]\\\
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
    \      }\n    }\n    #endif\n  }\n};\n#line 60 \"verify/yosupo/vertex_set_path_composite.test.cpp\"\
    \nMain<init, main2, test> main_dummy;\nint main() {}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/vertex_set_path_composite\"\
    \n\n#define SINGLE_TESTCASE\n// #define MULTI_TESTCASE\n// #define AOJ_TESTCASE\n\
    \n#ifndef LOCAL\n#define FAST_IO\n// #define FAST_CIO\n// #define INTERACTIVE\n\
    #endif\n\n#define INF 4'000'000'000'000'000'037LL\n#define EPS 1e-11\n\n#include\
    \ \"template/template_all_but_modint.hpp\"\n\n#include \"math/modint/modint.hpp\"\
    \nusing mint = modint998244353;\n\n#include \"graph/tree/rooted_tree.hpp\"\n#include\
    \ \"ds/segtree/segtree.hpp\"\n#include \"math/algebra/affine_function.hpp\"\n\n\
    void init() {}\n\nvoid main2()\n{\n  LL(N, Q);\n  VEC(pll, N, AB);\n  VEC(pll,\
    \ N - 1, UV);\n  RootedTree G(N, UV, randrange(0, N));\n  using M = NormalAndOppositeGroup<OppositeGroup<GroupAffineFunction<mint>>>;\n\
    \  SegmentTree<M> seg(G.reordered_vertex_info(AB));\n  rep(_, Q)\n  {\n    LL(t);\n\
    \    if (t == 0)\n    {\n      LL(p, c, d);\n      seg.set(G.preorder(p), pair{c,\
    \ d});\n    }\n    else if (t == 1)\n    {\n      LL(u, v, x);\n      auto res\
    \ = M::e();\n      G.path_query(u, v, [&](ll l, ll r, bool isrev)\n          \
    \         {\n      auto tmp = seg.prod(l, r);\n      res = M::op(res, isrev ?\
    \ tmp.rev() : tmp);});\n      auto [c, d] = res.normal;\n      PRINT(c * x + d);\n\
    \    }\n  }\n}\n\nvoid test() {}\n\n#include \"template/template_main.hpp\"\n\
    Main<init, main2, test> main_dummy;\nint main() {}\n"
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
  - math/modint/modint.hpp
  - math/modint/modint_internal_static.hpp
  - utils/larger_int.hpp
  - math/modint/modint_internal_isprime.hpp
  - math/modint/modint_internal_barrett32.hpp
  - math/modint/modint_internal_montgomery64.hpp
  - math/extgcd.hpp
  - graph/tree/rooted_tree.hpp
  - ds/segtree/segtree.hpp
  - math/algebra/affine_function.hpp
  - math/algebra/algebra_base.hpp
  - template/template_main.hpp
  isVerificationFile: true
  path: verify/yosupo/vertex_set_path_composite.test.cpp
  requiredBy: []
  timestamp: '2026-08-08 20:51:19+09:00'
  verificationStatus: TEST_WRONG_ANSWER
  verifiedWith: []
documentation_of: verify/yosupo/vertex_set_path_composite.test.cpp
layout: document
redirect_from:
- /verify/verify/yosupo/vertex_set_path_composite.test.cpp
- /verify/verify/yosupo/vertex_set_path_composite.test.cpp.html
title: verify/yosupo/vertex_set_path_composite.test.cpp
---
