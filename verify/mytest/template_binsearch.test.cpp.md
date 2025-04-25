---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: template/template_binsearch.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u4E8C\u5206\u63A2\u7D22\uFF09"
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
    - https://atcoder.jp/contests/kupc2013/tasks/kupc2013_a
    - https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A
  bundledCode: "#line 1 \"verify/mytest/template_binsearch.test.cpp\"\n#define PROBLEM\
    \ \"https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A\"\n\n#line\
    \ 2 \"template/template_binsearch.hpp\"\n\n#line 2 \"template/template_types.hpp\"\
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
    #line 2 \"template/template_vector.hpp\"\n\n#line 2 \"template/template_math.hpp\"\
    \n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n#endif\n#ifndef EPS\n\
    #define EPS 1e-11\n#endif\n\n#line 12 \"template/template_math.hpp\"\n\n/**\n\
    \ * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09\n * @docs\
    \ docs/template/template_math.md\n */\n\ntemplate <class T, class U>\ninline bool\
    \ chmin(T &a, U b) { return a > b ? a = b, true : false; }\ntemplate <class T,\
    \ class U>\ninline bool chmax(T &a, U b) { return a < b ? a = b, true : false;\
    \ }\n\ntemplate <class T = ll, class U, class V>\ninline constexpr T divfloor(U\
    \ a, V b) { return T(a) / T(b) - (T(a) % T(b) && (T(a) ^ T(b)) < 0); }\ntemplate\
    \ <class T = ll, class U, class V>\ninline constexpr T divceil(U a, V b) { return\
    \ T(a) / T(b) + (T(a) % T(b) && (T(a) ^ T(b)) >= 0); }\ntemplate <class T = ll,\
    \ class U, class V>\ninline constexpr T divround(U a, V b) { return divfloor<T>(2\
    \ * T(a) + T(b), 2 * T(b)); }\ntemplate <class T = ll, class U, class V>\ninline\
    \ constexpr T safemod(U a, V b) { return T(a) - T(b) * divfloor<T>(a, b); }\n\n\
    template <class T = ll, class U, class V>\nconstexpr T ipow(U a, V b)\n{\n  assert(b\
    \ >= 0);\n  if (b == 0)\n    return 1;\n  if (a == 0 || a == 1)\n    return a;\n\
    \  if (a < 0 && a == -1)\n    return b & 1 ? -1 : 1;\n\n  T res = 1, tmp = a;\n\
    \  while (true)\n  {\n    if (b & 1)\n      res *= tmp;\n    b >>= 1;\n    if\
    \ (b == 0)\n      break;\n    tmp *= tmp;\n  }\n  return res;\n}\ntemplate <class\
    \ T = ll, class A, class B, class M>\nT mul_limited(A a, B b, M m)\n{\n  assert(a\
    \ >= 0 && b >= 0 && m >= 0);\n  if (b == 0)\n    return 0;\n  return T(a) > T(m)\
    \ / T(b) ? T(m) : T(a) * T(b);\n}\ntemplate <class T = ll, class A, class B>\n\
    T mul_limited(A a, B b) { return mul_limited<T>(a, b, INF); }\ntemplate <class\
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
    \  return v[i];\n}\n#line 6 \"template/template_binsearch.hpp\"\n\n/**\n * @brief\
    \ \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u4E8C\u5206\u63A2\u7D22\uFF09\n *\
    \ @docs docs/template/template_binsearch.md\n */\n\ntemplate <class T>\nstruct\
    \ is_random_access_iterator\n{\n  static constexpr bool value = is_same_v<\n \
    \   typename iterator_traits<T>::iterator_category,\n    random_access_iterator_tag\n\
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
    \ const InitOk &init_ok, const InitNg &init_ng)\n{\n  T ok(init_ok), ng(init_ng);\n\
    \  assert(judge(ok));\n  assert(!judge(ng));\n  while (ok - ng != 1 && ng - ok\
    \ != 1)\n  {\n    T mid = (ok & ng) + ((ok ^ ng) >> 1);\n    (judge(mid) ? ok\
    \ : ng) = mid;\n  }\n  return {ok, ng};\n}\ntemplate <class T = ld, class Judge,\
    \ class InitOk, class InitNg>\nT binsearch_real(const Judge &judge, const InitOk\
    \ &init_ok, const InitNg &init_ng, int iteration_count = 100)\n{\n  T ok(init_ok),\
    \ ng(init_ng);\n  assert(judge(ok));\n  assert(!judge(ng));\n  repi(_, iteration_count)\n\
    \  {\n    T mid = (ok + ng) / 2;\n    (judge(mid) ? ok : ng) = mid;\n  }\n  return\
    \ ok;\n}\n// (ok, ng)\ntemplate <class T = ll, class Judge, class InitVal>\npair<T,\
    \ T> expsearch(const Judge &judge, const InitVal &init_val, bool positive = true)\n\
    {\n  T ok, ng;\n  if (judge(init_val))\n  {\n    ok = init_val, ng = init_val\
    \ + (positive ? 1 : -1);\n    for (int i = 1; judge(ng); i++)\n      ok = ng,\
    \ ng = init_val + (positive ? 1 : -1) * (T(1) << i);\n  }\n  else\n  {\n    ng\
    \ = init_val, ok = init_val + (positive ? 1 : -1);\n    for (int i = 1; !judge(ok);\
    \ i++)\n      ng = ok, ok = init_val + (positive ? 1 : -1) * (T(1) << i);\n  }\n\
    \  while (ok - ng != 1 && ng - ok != 1)\n  {\n    T mid = (ok & ng) + ((ok ^ ng)\
    \ >> 1);\n    (judge(mid) ? ok : ng) = mid;\n  }\n  return {ok, ng};\n}\n#line\
    \ 2 \"template/template_dump.hpp\"\n\n#line 4 \"template/template_dump.hpp\"\n\
    \n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08dump\uFF09\n * @docs\
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
    \ local_oj(a, b) (b)\n#endif\n#line 5 \"verify/mytest/template_binsearch.test.cpp\"\
    \n\nmt19937 mt;\nvoid test1()\n{\n  ll n = 1 + mt() % 10;\n  vl a(n);\n  rep(i,\
    \ n) a[i] = 1 + mt() % 10;\n  sort(ALL(a));\n  ll k = -1 + mt() % 12;\n\n  ll\
    \ i1, i2, i3;\n\n  i1 = LB(a, k);\n  i2 = binsearch([&](ll i)\n              \
    \    { return k <= vecget(a, i); }, SZ<int>(a), -1)\n            .first;\n  i3\
    \ = expsearch([&](ll i)\n                  { return k <= vecget(a, i); }, 0, true)\n\
    \            .first;\n  assert(i1 == i2 && i2 == i3);\n\n  i1 = UB(a, k);\n  i2\
    \ = binsearch([&](ll i)\n                  { return k < vecget(a, i); }, SZ<int>(a),\
    \ -1)\n            .first;\n  i3 = expsearch([&](ll i)\n                  { return\
    \ k < vecget(a, i); }, 0, true)\n            .first;\n  assert(i1 == i2 && i2\
    \ == i3);\n}\n\nvoid test2()\n{\n  ll n = 1 + mt() % 10;\n  vl a(n);\n  rep(i,\
    \ n) a[i] = 1 + mt() % 10;\n  sort(ALL(a));\n  ll k = -1 + mt() % 12;\n\n  ll\
    \ i1, i2;\n  i1 = lt_max(a, k);\n  i2 = expsearch([&](ll i)\n                \
    \ { return vecget(a, i) < k; }, 0, true)\n           .first;\n  assert(i1 == i2);\n\
    \  i1 = leq_max(a, k);\n  i2 = expsearch([&](ll i)\n                 { return\
    \ vecget(a, i) <= k; }, 0, true)\n           .first;\n  assert(i1 == i2);\n  i1\
    \ = gt_min(a, k);\n  i2 = expsearch([&](ll i)\n                 { return k < vecget(a,\
    \ i); }, 0, true)\n           .first;\n  assert(i1 == i2);\n  i1 = geq_min(a,\
    \ k);\n  i2 = expsearch([&](ll i)\n                 { return k <= vecget(a, i);\
    \ }, 0, true)\n           .first;\n  assert(i1 == i2);\n\n  i1 = lt_cnt(a, k);\n\
    \  i2 = count_if(ALL(a), [&](ll ai)\n                { return ai < k; });\n  assert(i1\
    \ == i2);\n  i1 = leq_cnt(a, k);\n  i2 = count_if(ALL(a), [&](ll ai)\n       \
    \         { return ai <= k; });\n  assert(i1 == i2);\n  i1 = gt_cnt(a, k);\n \
    \ i2 = count_if(ALL(a), [&](ll ai)\n                { return k < ai; });\n  assert(i1\
    \ == i2);\n  i1 = geq_cnt(a, k);\n  i2 = count_if(ALL(a), [&](ll ai)\n       \
    \         { return k <= ai; });\n  assert(i1 == i2);\n}\n\nvoid test3()\n{\n \
    \ vc<int> v = {1, 3, 5, 7, 9};\n  int i = leq_max(v, 6);\n  assert(v[i] == 5);\n\
    \  set<int> s = {1, 3, 5, 7, 9};\n  auto it = leq_max(s, 6);\n  assert(*it ==\
    \ 5);\n}\n\n// https://atcoder.jp/contests/kupc2013/tasks/kupc2013_a\nvoid test4()\n\
    {\n  using P = pair<int, string>;\n  auto solve = [](int q, vc<P> v) -> string\n\
    \  {\n    v.insert(v.begin(), {1, \"kogakubu10gokan\"});\n    int i = leq_max(v,\
    \ q, {}, [](const P &p)\n                    { return p.first; });\n    return\
    \ v[i].second;\n  };\n\n  assert(solve(12, {\n    {5, \"sogo5gokan\"},\n    {10,\
    \ \"sogo10gokan\"},\n    {15, \"sogo15gokan\"}\n  }) == \"sogo10gokan\");\n  assert(solve(10,\
    \ {\n    {5, \"kogakubu11gokan\"},\n    {10, \"sogo10gokan\"},\n    {15, \"KyotoUniversityResearchPark\"\
    }\n  }) == \"sogo10gokan\");\n  assert(solve(3, {\n    {5, \"kogakubu11gokan\"\
    },\n    {10, \"sogo10gokan\"},\n    {15, \"KyotoUniversityResearchPark\"}\n  })\
    \ == \"kogakubu10gokan\");\n}\n\nint main()\n{\n  rep(_, 10000) test1();\n  rep(_,\
    \ 10000) test2();\n  test3();\n  test4();\n\n  cout << \"Hello World\" << endl;\n\
    }\n"
  code: "#define PROBLEM \"https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A\"\
    \n\n#include \"template/template_binsearch.hpp\"\n#include \"template/template_dump.hpp\"\
    \n\nmt19937 mt;\nvoid test1()\n{\n  ll n = 1 + mt() % 10;\n  vl a(n);\n  rep(i,\
    \ n) a[i] = 1 + mt() % 10;\n  sort(ALL(a));\n  ll k = -1 + mt() % 12;\n\n  ll\
    \ i1, i2, i3;\n\n  i1 = LB(a, k);\n  i2 = binsearch([&](ll i)\n              \
    \    { return k <= vecget(a, i); }, SZ<int>(a), -1)\n            .first;\n  i3\
    \ = expsearch([&](ll i)\n                  { return k <= vecget(a, i); }, 0, true)\n\
    \            .first;\n  assert(i1 == i2 && i2 == i3);\n\n  i1 = UB(a, k);\n  i2\
    \ = binsearch([&](ll i)\n                  { return k < vecget(a, i); }, SZ<int>(a),\
    \ -1)\n            .first;\n  i3 = expsearch([&](ll i)\n                  { return\
    \ k < vecget(a, i); }, 0, true)\n            .first;\n  assert(i1 == i2 && i2\
    \ == i3);\n}\n\nvoid test2()\n{\n  ll n = 1 + mt() % 10;\n  vl a(n);\n  rep(i,\
    \ n) a[i] = 1 + mt() % 10;\n  sort(ALL(a));\n  ll k = -1 + mt() % 12;\n\n  ll\
    \ i1, i2;\n  i1 = lt_max(a, k);\n  i2 = expsearch([&](ll i)\n                \
    \ { return vecget(a, i) < k; }, 0, true)\n           .first;\n  assert(i1 == i2);\n\
    \  i1 = leq_max(a, k);\n  i2 = expsearch([&](ll i)\n                 { return\
    \ vecget(a, i) <= k; }, 0, true)\n           .first;\n  assert(i1 == i2);\n  i1\
    \ = gt_min(a, k);\n  i2 = expsearch([&](ll i)\n                 { return k < vecget(a,\
    \ i); }, 0, true)\n           .first;\n  assert(i1 == i2);\n  i1 = geq_min(a,\
    \ k);\n  i2 = expsearch([&](ll i)\n                 { return k <= vecget(a, i);\
    \ }, 0, true)\n           .first;\n  assert(i1 == i2);\n\n  i1 = lt_cnt(a, k);\n\
    \  i2 = count_if(ALL(a), [&](ll ai)\n                { return ai < k; });\n  assert(i1\
    \ == i2);\n  i1 = leq_cnt(a, k);\n  i2 = count_if(ALL(a), [&](ll ai)\n       \
    \         { return ai <= k; });\n  assert(i1 == i2);\n  i1 = gt_cnt(a, k);\n \
    \ i2 = count_if(ALL(a), [&](ll ai)\n                { return k < ai; });\n  assert(i1\
    \ == i2);\n  i1 = geq_cnt(a, k);\n  i2 = count_if(ALL(a), [&](ll ai)\n       \
    \         { return k <= ai; });\n  assert(i1 == i2);\n}\n\nvoid test3()\n{\n \
    \ vc<int> v = {1, 3, 5, 7, 9};\n  int i = leq_max(v, 6);\n  assert(v[i] == 5);\n\
    \  set<int> s = {1, 3, 5, 7, 9};\n  auto it = leq_max(s, 6);\n  assert(*it ==\
    \ 5);\n}\n\n// https://atcoder.jp/contests/kupc2013/tasks/kupc2013_a\nvoid test4()\n\
    {\n  using P = pair<int, string>;\n  auto solve = [](int q, vc<P> v) -> string\n\
    \  {\n    v.insert(v.begin(), {1, \"kogakubu10gokan\"});\n    int i = leq_max(v,\
    \ q, {}, [](const P &p)\n                    { return p.first; });\n    return\
    \ v[i].second;\n  };\n\n  assert(solve(12, {\n    {5, \"sogo5gokan\"},\n    {10,\
    \ \"sogo10gokan\"},\n    {15, \"sogo15gokan\"}\n  }) == \"sogo10gokan\");\n  assert(solve(10,\
    \ {\n    {5, \"kogakubu11gokan\"},\n    {10, \"sogo10gokan\"},\n    {15, \"KyotoUniversityResearchPark\"\
    }\n  }) == \"sogo10gokan\");\n  assert(solve(3, {\n    {5, \"kogakubu11gokan\"\
    },\n    {10, \"sogo10gokan\"},\n    {15, \"KyotoUniversityResearchPark\"}\n  })\
    \ == \"kogakubu10gokan\");\n}\n\nint main()\n{\n  rep(_, 10000) test1();\n  rep(_,\
    \ 10000) test2();\n  test3();\n  test4();\n\n  cout << \"Hello World\" << endl;\n\
    }"
  dependsOn:
  - template/template_binsearch.hpp
  - template/template_types.hpp
  - template/template_rep.hpp
  - template/template_vector.hpp
  - template/template_math.hpp
  - template/template_dump.hpp
  isVerificationFile: true
  path: verify/mytest/template_binsearch.test.cpp
  requiredBy: []
  timestamp: '2025-04-26 00:43:27+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/mytest/template_binsearch.test.cpp
layout: document
redirect_from:
- /verify/verify/mytest/template_binsearch.test.cpp
- /verify/verify/mytest/template_binsearch.test.cpp.html
title: verify/mytest/template_binsearch.test.cpp
---
