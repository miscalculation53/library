---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: math/algebra/algebra_base.hpp
    title: "\u4EE3\u6570\u7684\u69CB\u9020\u306E struct\uFF08\u57FA\u672C\uFF09"
  - icon: ':question:'
    path: math/algebra/algebra_basic_ops.hpp
    title: "\u4EE3\u6570\u7684\u69CB\u9020\uFF08\u56DB\u5247\u6F14\u7B97\u3068 min,\
      \ max\uFF09"
  - icon: ':question:'
    path: math/extgcd.hpp
    title: "\u62E1\u5F35\u30E6\u30FC\u30AF\u30EA\u30C3\u30C9\u4E92\u9664\u6CD5 (extgcd)"
  - icon: ':question:'
    path: math/modint/modint.hpp
    title: modint (32 bit)
  - icon: ':question:'
    path: math/modint/modint32_internal.hpp
    title: math/modint/modint32_internal.hpp
  - icon: ':question:'
    path: math/modint/modint64.hpp
    title: modint (64 bit)
  - icon: ':question:'
    path: math/modint/modint64_internal.hpp
    title: math/modint/modint64_internal.hpp
  - icon: ':question:'
    path: math/modint/modint_base.hpp
    title: math/modint/modint_base.hpp
  - icon: ':question:'
    path: math/prime/factorize.hpp
    title: "\u7D20\u56E0\u6570\u5206\u89E3"
  - icon: ':question:'
    path: math/prime/primality_test.hpp
    title: "\u7D20\u6570\u5224\u5B9A"
  - icon: ':question:'
    path: math/prime/prime_power.hpp
    title: "\u7D20\u3079\u304D\u69CB\u9020\u4F53"
  - icon: ':question:'
    path: math/prime/zeta_mobius_divisor_multiple_large.hpp
    title: "\u7D04\u6570\u30FB\u500D\u6570 \u30BC\u30FC\u30BF\u30FB\u30E1\u30D3\u30A6\
      \u30B9\u5909\u63DB\uFF08\u5927\u304D\u3044 $m$ \u306E\u7D04\u6570\uFF09"
  - icon: ':question:'
    path: template/template_algo.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30A2\u30EB\u30B4\u30EA\u30BA\
      \u30E0\uFF09"
  - icon: ':question:'
    path: template/template_all.hpp
    title: template/template_all.hpp
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
  _isVerificationFailed: true
  _pathExtension: cpp
  _verificationStatusIcon: ':x:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://yukicoder.me/problems/no/2578
    links:
    - https://yukicoder.me/problems/no/2578
  bundledCode: "#line 1 \"verify/yukicoder/zeta_mobius_multiple_large.test.cpp\"\n\
    #define PROBLEM \"https://yukicoder.me/problems/no/2578\"\n\n#define SINGLE_TESTCASE\n\
    // #define MULTI_TESTCASE\n// #define AOJ_TESTCASE\n\n#define FAST_IO\n// #define\
    \ FAST_CIO\n// #define INTERACTIVE\n\n#define INF 4'000'000'000'000'000'037LL\n\
    #define EPS 1e-11\n\n#line 2 \"template/template_all.hpp\"\n\n#line 2 \"template/template_types.hpp\"\
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
    \ M m = INF)\n{\n  assert(a >= 0 && b >= 0 && m >= 0);\n  if (b == 0)\n    return\
    \ 0;\n  return T(a) > T(m) / T(b) ? T(m) : T(a) * T(b);\n}\ntemplate <class T\
    \ = ll, class A, class B, class M>\nT pow_limited(A a, B b, M m = INF)\n{\n  assert(a\
    \ >= 0 && b >= 0 && m >= 0);\n  if (a <= 1 || b == 0)\n    return min(ipow<T>(a,\
    \ b), T(m));\n  \n  T res = 1, tmp = a;\n  while (true)\n  {\n    if (b & 1)\n\
    \    {\n      if (res > T(m) / tmp)\n        return m;\n      res *= tmp;\n  \
    \  }\n    b >>= 1;\n    if (b == 0)\n      break;\n    if (tmp > T(m) / tmp)\n\
    \      return m;\n    tmp *= tmp;\n  }\n  return res;\n}\n\ntemplate <class T\
    \ = ll, class A, class K>\nconstexpr T iroot(A a, K k)\n{\n  assert(a >= 0 &&\
    \ k >= 1);\n  if (a <= 1 || k == 1)\n    return a;\n  if (k == 2)\n  {\n    if\
    \ constexpr (sizeof(T) > sizeof(ull))\n    {\n      if ((u128)a < ((u128)1 <<\
    \ 120))\n        return sqrtl(a);\n    }\n    else\n      return sqrtl(a);\n \
    \ }\n\n  auto isok = [&](T x) -> bool\n  {\n    if (x == 0)\n      return true;\n\
    \    T res = 1, k2 = k;\n    while (true)\n    {\n      if (k2 & 1)\n      {\n\
    \        if (res > T(a) / x)\n          return false;\n        res *= x;\n   \
    \   }\n      k2 >>= 1;\n      if (k2 == 0)\n        break;\n      if (x > T(a)\
    \ / x)\n        return false;\n      x *= x;\n    }\n    return res <= T(a);\n\
    \  };\n\n  T x = pow(a, 1.0 / k);\n  bool up = true;\n  while (!isok(x))\n   \
    \ up = false, x--;\n  if (up)\n  {\n    while (x < numeric_limits<T>::max() &&\
    \ isok(x + 1))\n      x++;\n  }\n  return x;\n}\n\n// https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html\n\
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
    \ 'a') + (ai - 10));\n  return s;\n}\n#line 2 \"template/template_vector.hpp\"\
    \n\n#line 6 \"template/template_vector.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\
    \u30EC\u30FC\u30C8\uFF08vector\uFF09\n * @docs docs/template/template_vector.md\n\
    \ */\n\n#define ALL(a) (a).begin(), (a).end()\ntemplate <class T = ll, class V>\n\
    inline T SZ(const V &x) { return x.size(); }\n#define eb emplace_back\n\ntemplate\
    \ <class F>\nauto gen_vec(const int &n, const F &f)\n{\n  vc<decltype(f(0))> res(n);\n\
    \  repi(i, n) res[i] = f(i);\n  return res;\n}\n\n// https://qiita.com/Chippppp/items/13150f5e0ea99f444d97#%E5%A4%9A%E6%AC%A1%E5%85%83vector%E7%94%9F%E6%88%90%E9%96%A2%E6%95%B0\n\
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
    \  return v[i];\n}\n#line 2 \"template/template_algo.hpp\"\n\n#ifndef INF\n#define\
    \ INF 4'000'000'000'000'000'037LL\n#endif\n\n#line 10 \"template/template_algo.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30A2\u30EB\u30B4\
    \u30EA\u30BA\u30E0\uFF09\n * @docs docs/template/template_algo.md\n */\n\ntemplate\
    \ <class V>\nauto SUM(const V &v) { return accumulate(ALL(v), (decltype(v[0]))0);\
    \ }\ntemplate <class T, class V>\nT SUM(const V &v) { return accumulate(ALL(v),\
    \ T(0)); }\ntemplate <class V>\nauto MAX(const V &v) { return *max_element(ALL(v));\
    \ }\ntemplate <class V>\nauto MIN(const V &v) { return *min_element(ALL(v)); }\n\
    template <class I = ll, class V>\nI ARGMAX(const V &v) { return max_element(ALL(v))\
    \ - v.begin(); }\ntemplate <class I = ll, class V>\nI ARGMIN(const V &v) { return\
    \ min_element(ALL(v)) - v.begin(); }\n\ntemplate<class T = ll, class V>\nT mex(const\
    \ V &a)\n{\n  int n = a.size();\n  vector<bool> exists(n, false);\n  repi(i, n)\
    \ if (0 <= a[i] && a[i] < n) exists[a[i]] = true;\n  repi(x, n) if (!exists[x])\
    \ return x;\n  return n;\n}\n\ntemplate <class T = ll>\nvc<T> permid(const int\
    \ &n, const int &base_index = 0)\n{\n  vc<T> p(n);\n  repi(i, n) p[i] = i + base_index;\n\
    \  return p;\n}\ntemplate <class T>\nvc<T> perminv(const vc<T> &p)\n{\n  if (p.empty())\n\
    \    return {};\n  const int n = p.size();\n  vc<T> q(MAX(p) + 1);\n  repi(i,\
    \ n) if (p[i] >= 0) q[p[i]] = i;\n  return q;\n}\n// a[p[i]] for all i\ntemplate\
    \ <class T, class U>\nvc<T> permuted(const vc<T> &a, const vc<U> &p)\n{\n  const\
    \ int n = p.size();\n  vc<T> res(n);\n  repi(i, n)\n  {\n    assert(0 <= p[i]\
    \ && p[i] < U(a.size()));\n    res[i] = a[p[i]];\n  }\n  return res;\n}\n\ntemplate\
    \ <class V>\nV reversed(const V &v) { return V(v.rbegin(), v.rend()); }\n\n#if\
    \ __cplusplus < 202002L\ntemplate <class V, class... Args>\nV sorted(V v, Args&&...\
    \ args)\n{\n  sort(ALL(v), forward<Args>(args)...);\n  return v;\n}\n#else\ntemplate\
    \ <class V, class... Args>\nV sorted(V v, Args&&... args)\n{\n  ranges::sort(v,\
    \ forward<Args>(args)...);\n  return v;\n}\n#endif\n\ntemplate <class V>\nvoid\
    \ unique(V &v) { v.erase(std::unique(ALL(v)), v.end()); }\ntemplate <class V>\n\
    V uniqued(V v) { unique(v); return v; }\n\ntemplate <class V>\nvoid sortunique(V\
    \ &v)\n{\n  sort(ALL(v));\n  unique(v);\n}\ntemplate <class V>\nV sortuniqued(V\
    \ v) { sortunique(v); return v; }\n\n// 01234 -> 12340\ntemplate <class V, class\
    \ U>\nvoid rotate(V &v, U k)\n{ \n  const U n = v.size();\n  k = (k % n + n) %\
    \ n;\n  std::rotate(v.begin(), v.begin() + k, v.end());\n}\n// 01234 -> 12340\n\
    template <class V, class U>\nV rotated(V v, U k) { rotate(v, k); return v; }\n\
    \ntemplate <class T>\nvvc<T> top(const vvc<T> &a)\n{\n  if (a.empty())\n    return\
    \ {};\n  const int n = a.size(), m = a[0].size();\n  vvc<T> b(m, vc<T>(n));\n\
    \  repi(i, n)\n  {\n    assert(SZ<int>(a[i]) == m);\n    repi(j, m) b[j][i] =\
    \ a[i][j];\n  }\n  return b;\n}\nvstr top(const vstr &a)\n{\n  vvc<char> a_(a.size());\n\
    \  repi(i, SZ<int>(a)) a_[i] = {ALL(a[i])};\n  vvc<char> b_ = top(a_);\n  vstr\
    \ b(b_.size());\n  repi(i, SZ<int>(b)) b[i] = {ALL(b_[i])};\n  return b;\n}\n\n\
    // 12\n// 34 -> 246\n// 56    135\n// (\u53CD\u6642\u8A08\u56DE\u308A)\ntemplate\
    \ <class VV, class U = ll>\nVV rot90(const VV &a, U k = 1)\n{\n  if (a.empty())\n\
    \    return {};\n  const int n = a.size(), m = a[0].size();\n  k = (k % 4 + 4)\
    \ % 4;\n  if (k == 0)\n    return a;\n  else if (k == 1)\n  {\n    VV b(m);\n\
    \    repi(j, m) b[j].resize(n);\n    repi(i, n)\n    {\n      assert(SZ<int>(a[i])\
    \ == m);\n      repi(j, m) b[m - 1 - j][i] = a[i][j];\n    }\n    return b;\n\
    \  }\n  else if (k == 2)\n  {\n    VV b(n);\n    repi(i, n) b[i].resize(m);\n\
    \    repi(i, n)\n    {\n      assert(SZ<int>(a[i]) == m);\n      repi(j, m) b[n\
    \ - 1 - i][m - 1 - j] = a[i][j];\n    }\n    return b;\n  }\n  else\n  {\n   \
    \ VV b(m);\n    repi(j, m) b[j].resize(n);\n    repi(i, n)\n    {\n      assert(SZ<int>(a[i])\
    \ == m);\n      repi(j, m) b[j][n - 1 - i] = a[i][j];\n    }\n    return b;\n\
    \  }\n}\n\ntemplate <class T, class F = decltype(plus<>())>\nvc<T> cuml(const\
    \ vc<T> &v, F op = plus<>(), const T &e = 0)\n{\n  const int n = v.size();\n \
    \ vc<T> res(n + 1, e);\n  repi(i, n) res[i + 1] = op(res[i], v[i]);\n  return\
    \ res;\n}\ntemplate <class T, class F = decltype(plus<>())>\nvc<T> cumr(const\
    \ vc<T> &v, const F &op = plus<>(), const T &e = 0)\n{ return reversed(cuml<T,\
    \ F>(reversed(v), op, e)); }\ntemplate <class T, const T infty = INF>\nvc<T> cumlmax(const\
    \ vc<T> &v)\n{ return cuml(v, [](T a, T b) { return max(a, b); }, -infty); }\n\
    template <class T, const T infty = INF>\nvc<T> cumrmax(const vc<T> &v)\n{ return\
    \ cumr(v, [](T a, T b) { return max(a, b); }, -infty); }\ntemplate <class T, const\
    \ T infty = INF>\nvc<T> cumlmin(const vc<T> &v)\n{ return cuml(v, [](T a, T b)\
    \ { return min(a, b); }, infty); }\ntemplate <class T, const T infty = INF>\n\
    vc<T> cumrmin(const vc<T> &v)\n{ return cumr(v, [](T a, T b) { return min(a, b);\
    \ }, infty); }\n\ntemplate <class T>\nvc<T> adjd(const vc<T> &v)\n{\n  int n =\
    \ v.size();\n  vc<T> res(n + 1);\n  res[0] = v[0];\n  repi(i, 1, n) res[i] = v[i]\
    \ - v[i - 1];\n  res[n] = -v[n - 1];\n  return res;\n}\n\nconst vpll DRULgrid\
    \ = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};\nconst vpll DRULplane = {{0, -1}, {1,\
    \ 0}, {0, 1}, {-1, 0}};\n#line 2 \"template/template_binsearch.hpp\"\n\n#line\
    \ 6 \"template/template_binsearch.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\
    \u30FC\u30C8\uFF08\u4E8C\u5206\u63A2\u7D22\uFF09\n * @docs docs/template/template_binsearch.md\n\
    \ */\n\n#if __cplusplus < 202002L\n// val <= v[i] \u3068\u306A\u308B\u6700\u5C0F\
    \u306E i (val \u672A\u6E80\u306E\u5024\u306E\u500B\u6570)\n// \u5F15\u6570: comp\n\
    template <class T = ll, class V, class... Args>\ninline T LB(const V &v, Args&&...\
    \ args)\n{ return lower_bound(ALL(v), forward<Args>(args)...) - v.begin(); }\n\
    // val < v[i] \u3068\u306A\u308B\u6700\u5C0F\u306E i (val \u4EE5\u4E0B\u306E\u5024\
    \u306E\u500B\u6570)\n// \u5F15\u6570: comp\ntemplate <class T = ll, class V, class...\
    \ Args>\ninline T UB(const V &v, Args&&... args)\n{ return upper_bound(ALL(v),\
    \ forward<Args>(args)...) - v.begin(); }\n#else\n// val <= v[i] \u3068\u306A\u308B\
    \u6700\u5C0F\u306E i (val \u672A\u6E80\u306E\u5024\u306E\u500B\u6570)\n// \u5F15\
    \u6570: comp, proj\ntemplate <class T = ll, class V, class... Args>\ninline T\
    \ LB(const V &v, Args&&... args)\n{ return ranges::lower_bound(v, forward<Args>(args)...)\
    \ - v.begin(); }\n// val < v[i] \u3068\u306A\u308B\u6700\u5C0F\u306E i (val \u4EE5\
    \u4E0B\u306E\u5024\u306E\u500B\u6570)\n// \u5F15\u6570: comp, proj\ntemplate <class\
    \ T = ll, class V, class... Args>\ninline T UB(const V &v, Args&&... args)\n{\
    \ return ranges::upper_bound(v, forward<Args>(args)...) - v.begin(); }\n#endif\n\
    \ntemplate <class T>\nstruct is_random_access_iterator\n{\n  static constexpr\
    \ bool value = is_same_v<\n    typename iterator_traits<T>::iterator_category,\n\
    \    random_access_iterator_tag\n  >;\n};\ntemplate <class T>\nconstexpr bool\
    \ is_random_access_iterator_v = is_random_access_iterator<T>::value;\n\ntemplate\
    \ <class T = ll, class V, class... Args>\ninline auto lt_max(const V &v, Args&&...\
    \ args)\n{\n  if constexpr (is_random_access_iterator_v<typename V::iterator>)\n\
    \    return LB<T>(v, forward<Args>(args)...) - 1;\n  else\n  {\n    auto it =\
    \ v.lower_bound(forward<Args>(args)...);\n    if (it == v.begin())\n      return\
    \ v.end();\n    else\n      return prev(it);\n  }\n}\ntemplate <class T = ll,\
    \ class V, class... Args>\ninline auto leq_max(const V &v, Args&&... args)\n{\n\
    \  if constexpr (is_random_access_iterator_v<typename V::iterator>)\n    return\
    \ UB<T>(v, forward<Args>(args)...) - 1;\n  else\n  {\n    auto it = v.upper_bound(forward<Args>(args)...);\n\
    \    if (it == v.begin())\n      return v.end();\n    else\n      return prev(it);\n\
    \  }\n}\ntemplate <class T = ll, class V, class... Args>\ninline auto gt_min(const\
    \ V &v, Args&&... args)\n{\n  if constexpr (is_random_access_iterator_v<typename\
    \ V::iterator>)\n    return UB<T>(v, forward<Args>(args)...);\n  else\n    return\
    \ v.upper_bound(forward<Args>(args)...);\n}\ntemplate <class T = ll, class V,\
    \ class... Args>\ninline auto geq_min(const V &v, Args&&... args)\n{\n  if constexpr\
    \ (is_random_access_iterator_v<typename V::iterator>)\n    return LB<T>(v, forward<Args>(args)...);\n\
    \  else\n    return v.lower_bound(forward<Args>(args)...);\n}\n\ntemplate <class\
    \ T = ll, class V, class... Args>\ninline T lt_cnt(const V &v, Args&&... args)\n\
    { return LB<T>(v, forward<Args>(args)...); }\ntemplate <class T = ll, class V,\
    \ class... Args>\ninline T leq_cnt(const V &v, Args&&... args)\n{ return UB<T>(v,\
    \ forward<Args>(args)...); }\ntemplate <class T = ll, class V, class... Args>\n\
    inline T gt_cnt(const V &v, Args&&... args)\n{ return SZ<T>(v) - UB<T>(v, forward<Args>(args)...);\
    \ }\ntemplate <class T = ll, class V, class... Args>\ninline T geq_cnt(const V\
    \ &v, Args&&... args)\n{ return SZ<T>(v) - LB<T>(v, forward<Args>(args)...); }\n\
    \ntemplate <class T = ll, class V, class L, class R, class... Args>\ninline T\
    \ in_cnt(const V &v, L l, R r, Args&&... args)\n{\n  if (l > r)\n    return 0;\n\
    \  return lt_cnt<T>(v, r, forward<Args>(args)...) - lt_cnt<T>(v, l, forward<Args>(args)...);\n\
    }\n\ntemplate <class T = ll, class Judge, class InitOk, class InitNg>\npair<T,\
    \ T> binsearch(const Judge &judge, const InitOk &init_ok, const InitNg &init_ng)\n\
    {\n  T ok(init_ok), ng(init_ng);\n  assert(judge(ok));\n  assert(!judge(ng));\n\
    \  while (ok - ng != 1 && ng - ok != 1)\n  {\n    T mid = (ok & ng) + ((ok ^ ng)\
    \ >> 1);\n    (judge(mid) ? ok : ng) = mid;\n  }\n  return make_pair(ok, ng);\n\
    }\ntemplate <class T = ld, class Judge, class InitOk, class InitNg>\nT binsearch_real(const\
    \ Judge &judge, const InitOk &init_ok, const InitNg &init_ng, int iteration_count\
    \ = 100)\n{\n  T ok(init_ok), ng(init_ng);\n  assert(judge(ok));\n  assert(!judge(ng));\n\
    \  repi(_, iteration_count)\n  {\n    T mid = (ok + ng) / 2;\n    (judge(mid)\
    \ ? ok : ng) = mid;\n  }\n  return ok;\n}\ntemplate <class T = ll, class Judge,\
    \ class InitVal>\npair<T, T> expsearch(const Judge &judge, const InitVal &init_val,\
    \ const bool &positive = true)\n{\n  T ok, ng;\n  if (judge(init_val))\n  {\n\
    \    ok = init_val, ng = init_val + (positive ? 1 : -1);\n    for (int i = 1;\
    \ judge(ng); i++)\n      ok = ng, ng = init_val + (positive ? 1 : -1) * (T(1)\
    \ << i);\n  }\n  else\n  {\n    ng = init_val, ok = init_val + (positive ? 1 :\
    \ -1);\n    for (int i = 1; !judge(ok); i++)\n      ng = ok, ok = init_val + (positive\
    \ ? 1 : -1) * (T(1) << i);\n  }\n  while (ok - ng != 1 && ng - ok != 1)\n  {\n\
    \    T mid = (ok & ng) + ((ok ^ ng) >> 1);\n    (judge(mid) ? ok : ng) = mid;\n\
    \  }\n  return make_pair(ok, ng);\n}\n#line 2 \"template/template_bit.hpp\"\n\n\
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
    CPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 10000);\n#define local(...) __VA_ARGS__\n\
    #else\n#define dump(...)\n#define local(...)\n#endif\n#line 6 \"template/template_inout.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u5165\u51FA\u529B\
    \uFF09\n * @docs docs/template/template_inout.md\n */\n\n// https://judge.yosupo.jp/submission/170706\
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
    \  string s = oss.str();\n  wt1(s);\n}\n\ntemplate <class T, enable_if_t<is_integral_v<T>,\
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
    \ &&... tail) {\n  wt1(head);\n  write(forward<Tail>(tail)...);\n}\n\nvoid print()\
    \ { wt1('\\n'); }\ntemplate <class Head, class... Tail>\nvoid print(Head &&head,\
    \ Tail &&... tail) {\n  wt1(head);\n  if (sizeof...(Tail)) wt1(' ');\n  print(forward<Tail>(tail)...);\n\
    }\n\n} // namespace fastio\n\n#endif\n\n#if defined FAST_IO and not defined LOCAL\n\
    struct Dummy {\n  Dummy() { atexit(fastio::flush); }\n} dummy;\n#endif\n\n// https://trap.jp/post/1224/\n\
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
    \ T, const size_t m>\narray<vc<T>, m> top(const vc<array<T, m>> &vt)\n{\n  const\
    \ size_t n = vt.size();\n  array<vc<T>, m> tv;\n  tv.fill(vc<T>(n));\n  for (size_t\
    \ i = 0; i < n; i++)\n    for (size_t j = 0; j < m; j++)\n      tv[j][i] = vt[i][j];\n\
    \  return tv;\n}\ntemplate <class T, const size_t m>\nvc<array<T, m>> top(const\
    \ array<vc<T>, m> &tv)\n{\n  if (tv.empty()) return {};\n  const size_t n = tv[0].size();\n\
    \  vc<array<T, m>> vt(n);\n  for (size_t j = 0; j < m; j++)\n  {\n    assert(tv[j].size()\
    \ == n);\n    for (size_t i = 0; i < n; i++)\n      vt[i][j] = tv[j][i];\n  }\n\
    \  return vt;\n}\n\ntemplate <class T, class U>\npair<vc<T>, vc<U>> top(const\
    \ vc<pair<T, U>> &vt)\n{\n  const size_t n = vt.size();\n  pair<vc<T>, vc<U>>\
    \ tv;\n  tv.first.resize(n), tv.second.resize(n);\n  for (size_t i = 0; i < n;\
    \ i++)\n    tie(tv.first[i], tv.second[i]) = vt[i];\n  return tv;\n}\ntemplate\
    \ <class T, class U>\nvc<pair<T, U>> top(const pair<vc<T>, vc<U>> &tv)\n{\n  const\
    \ size_t n = tv.first.size();\n  assert(n == tv.second.size());\n  vc<pair<T,\
    \ U>> vt(n);\n  for (size_t i = 0; i < n; i++)\n    vt[i] = make_pair(tv.first[i],\
    \ tv.second[i]);\n  return vt;\n}\n\nnamespace internal\n{\n\ntemplate <size_t...\
    \ I, class V, class Tp>\nauto vt_to_tv_impl(V &tv, const Tp &t, index_sequence<I...>,\
    \ size_t index)\n{ ((get<I>(tv)[index] = get<I>(t)), ...); }\n\ntemplate <size_t...\
    \ I, class Tp>\nauto tv_to_vt_impl(const Tp &tv, index_sequence<I...>, size_t\
    \ index)\n{ return make_tuple(get<I>(tv)[index]...); }\n\n};\n\ntemplate <class...\
    \ Ts>\nauto top(const vc<tuple<Ts...>> &vt)\n{\n  const size_t n = vt.size();\n\
    \  tuple<vc<Ts>...> tv;\n  apply([&](auto &...v)\n        { ((v.resize(n)), ...);\
    \ }, tv);\n  for (size_t i = 0; i < n; i++)\n    internal::vt_to_tv_impl(tv, vt[i],\
    \ make_index_sequence<tuple_size_v<decltype(tv)>>{}, i);\n  return tv;\n}\n\n\
    template <class... Ts>\nauto top(const tuple<vc<Ts>...> &tv)\n{\n  size_t n =\
    \ get<0>(tv).size();\n  apply([&](auto &...v)\n        { ((assert(v.size() ==\
    \ n)), ...); }, tv);\n  vc<tuple<Ts...>> vt(n);\n  for (size_t i = 0; i < n; i++)\n\
    \    vt[i] = internal::tv_to_vt_impl(tv, index_sequence_for<Ts...>{}, i);\n  return\
    \ vt;\n}\n// ----------\n#line 15 \"verify/yukicoder/zeta_mobius_multiple_large.test.cpp\"\
    \n\n#line 2 \"math/prime/zeta_mobius_divisor_multiple_large.hpp\"\n\n#line 2 \"\
    math/prime/factorize.hpp\"\n\n#line 2 \"math/modint/modint.hpp\"\n\n#line 2 \"\
    math/modint/modint32_internal.hpp\"\n\n#line 4 \"math/modint/modint32_internal.hpp\"\
    \n\nnamespace internal\n{\n\nconstexpr ll powmod32_constexpr(ll x, ll n, int m)\n\
    {\n  if (m == 1)\n    return 0;\n  uint _m = (uint)m;\n  ull r = 1;\n  ull y =\
    \ safemod(x, m);\n  while (n)\n  {\n    if (n & 1)\n      r = (r * y) % _m;\n\
    \    y = (y * y) % _m;\n    n >>= 1;\n  }\n  return r;\n}\n\nconstexpr bool isprime32_constexpr(int\
    \ n)\n{\n  if (n <= 1)\n    return false;\n  if (n == 2 || n == 7 || n == 61)\n\
    \    return true;\n  if (n % 2 == 0)\n    return false;\n  ll d = n - 1;\n  while\
    \ (d % 2 == 0)\n    d /= 2;\n  constexpr ll bases[3] = {2, 7, 61};\n  for (ll\
    \ a : bases)\n  {\n    ll t = d;\n    ll y = powmod32_constexpr(a, t, n);\n  \
    \  while (t != n - 1 && y != 1 && y != n - 1)\n    {\n      y = y * y % n;\n \
    \     t <<= 1;\n    }\n    if (y != n - 1 && t % 2 == 0)\n      return false;\n\
    \  }\n  return true;\n}\ntemplate <int n>\nconstexpr bool isprime32 = isprime32_constexpr(n);\n\
    \nstruct barrett32\n{\n  uint m;\n  ull im;\n\n  explicit barrett32(uint m) :\
    \ m(m), im((ull)(-1) / m + 1) {}\n  uint umod() const { return m; }\n  uint mul(uint\
    \ a, uint b) const\n  {\n    ull z = a;\n    z *= b;\n    ull x = (ull)((u128(z)*im)\
    \ >> 64);\n    ull y = x * m;\n    return (uint)(z - y + (z < y ? m : 0));\n \
    \ }\n};\n\n}\n#line 2 \"math/modint/modint_base.hpp\"\n\n#line 4 \"math/modint/modint_base.hpp\"\
    \n\nnamespace internal\n{\n\n#define REF static_cast<mint &>(*this)\n#define CREF\
    \ static_cast<const mint &>(*this)\n#define VAL *static_cast<const mint *>(this)\n\
    \ntemplate <class mint>\nstruct modint_base\n{\n  mint &operator+=(const mint\
    \ &rhs)\n  {\n    mint &self = REF;\n    self._v += rhs._v;\n    if (self._v >=\
    \ self.umod())\n      self._v -= self.umod();\n    return self;\n  }\n  mint &operator-=(const\
    \ mint &rhs)\n  {\n    mint &self = REF;\n    self._v -= rhs._v;\n    if (self._v\
    \ >= self.umod())\n      self._v += self.umod();\n    return self;\n  }\n  mint\
    \ &operator/=(const mint &rhs)\n  {\n    mint &self = REF;\n    return self =\
    \ self * rhs.inv();\n  }\n\n  mint &operator++()\n  {\n    mint &self = REF;\n\
    \    self._v++;\n    if (self._v == self.umod())\n      self._v = 0;\n    return\
    \ self;\n  }\n  mint &operator--()\n  {\n    mint &self = REF;\n    if (self._v\
    \ == 0)\n      self._v = self.umod();\n    self._v--;\n    return self;\n  }\n\
    \  mint operator++(int)\n  {\n    mint res = VAL;\n    ++REF;\n    return res;\n\
    \  }\n  mint operator--(int)\n  {\n    mint res = VAL;\n    --REF;\n    return\
    \ res;\n  }\n\n  mint operator+() const { return VAL; }\n  mint operator-() const\
    \ { return mint() - VAL; }\n\n  mint pow(ll n) const\n  {\n    assert(n >= 0);\n\
    \    mint x = VAL, r = 1;\n    while (n)\n    {\n      if (n & 1)\n        r *=\
    \ x;\n      x *= x;\n      n >>= 1;\n    }\n    return r;\n  }\n\n  friend mint\
    \ operator+(const mint &lhs, const mint &rhs)\n  { return mint(lhs) += rhs; }\n\
    \  friend mint operator-(const mint &lhs, const mint &rhs)\n  { return mint(lhs)\
    \ -= rhs; }\n  friend mint operator*(const mint &lhs, const mint &rhs)\n  { return\
    \ mint(lhs) *= rhs; }\n  friend mint operator/(const mint &lhs, const mint &rhs)\n\
    \  { return mint(lhs) /= rhs; }\n  friend bool operator==(const mint &lhs, const\
    \ mint &rhs)\n  { return mint(lhs).eq(rhs); }\n  friend bool operator!=(const\
    \ mint &lhs, const mint &rhs)\n  { return mint(lhs).neq(rhs); }\nprivate:\n  bool\
    \ eq(const mint &rhs) { return REF._v == rhs._v; }\n  bool neq(const mint &rhs)\
    \ { return REF._v != rhs._v; }\n};\n\n}\n\n#if defined LOCAL or not defined FAST_IO\n\
    template <typename T, std::enable_if_t<std::is_base_of_v<internal::modint_base<T>,\
    \ T>, int> = 0>\nistream &operator>>(istream &is, T &x)\n{\n  ll a;\n  is >> a;\n\
    \  x = a;\n  return is;\n}\ntemplate <typename T, std::enable_if_t<std::is_base_of_v<internal::modint_base<T>,\
    \ T>, int> = 0>\nostream &operator<<(ostream &os, const T &x)\n{\n  os << x.val();\n\
    \  return os;\n}\n#else\ntemplate <typename T, std::enable_if_t<std::is_base_of_v<internal::modint_base<T>,\
    \ T>, int> = 0>\nvoid rd1(T &x)\n{\n  ll a;\n  fastio::rd1(a);\n  x = a;\n}\n\
    template <typename T, std::enable_if_t<std::is_base_of_v<internal::modint_base<T>,\
    \ T>, int> = 0>\nvoid wt1(const T &x) { fastio::wt1(x.val()); }\n#endif\n#line\
    \ 2 \"math/extgcd.hpp\"\n\n#line 4 \"math/extgcd.hpp\"\n\n/**\n * @brief \u62E1\
    \u5F35\u30E6\u30FC\u30AF\u30EA\u30C3\u30C9\u4E92\u9664\u6CD5 (extgcd)\n * @docs\
    \ docs/math/extgcd.md\n */\n\n// g == gcd(x, y) >= 0, ax + by == g \u3092\u6E80\
    \u305F\u3059 (g, x, y)\n// max(|x|, |y|) <= max(|a|, |b|)\ntemplate <class T =\
    \ ll>\nconstexpr tuple<T, T, T> extgcd(const T &a, const T &b)\n{\n  if (a ==\
    \ 0 && b == 0)\n    return {0, 0, 0};\n  \n  // a*x1 + b*y1 == z1  ...(1)\n  //\
    \ a*x2 + b*y2 == z2  ...(2)\n  T x1 = 1, y1 = 0, z1 = a;\n  T x2 = 0, y2 = 1,\
    \ z2 = b;\n  while (z2 != 0)\n  {\n    // (1)' = (2)\n    // (2)' = (1) - q*(2)\n\
    \    T q = z1 / z2;\n    tie(x1, x2) = make_pair(x2, x1 - q * x2);\n    tie(y1,\
    \ y2) = make_pair(y2, y1 - q * y2);\n    tie(z1, z2) = make_pair(z2, z1 - q *\
    \ z2);\n  }\n  if (z1 < 0)\n    x1 = -x1, y1 = -y1, z1 = -z1;\n  return {z1, x1,\
    \ y1};\n}\n#line 7 \"math/modint/modint.hpp\"\n\n/**\n * @brief modint (32 bit)\n\
    \ * @docs docs/math/modint/modint.md\n */\n\ntemplate <int m>\nstruct static_modint\
    \ : internal::modint_base<static_modint<m>>\n{\n  using mint = static_modint;\n\
    private:\n  friend struct internal::modint_base<static_modint<m>>;\n  uint _v;\n\
    \  static constexpr uint umod() { return m; }\n  static constexpr bool prime =\
    \ internal::isprime32<m>;\n\npublic:\n  static constexpr int mod() { return m;\
    \ }\n  static mint raw(int v)\n  {\n    mint x;\n    x._v = v;\n    return x;\n\
    \  }\n\n  static_modint() : _v(0) {}\n  template <class T>\n  static_modint(T\
    \ v)\n  {\n    if constexpr (is_signed_v<T>)\n    {\n      ll x = (ll)(v % (ll)(umod()));\n\
    \      if (x < 0)\n        x += umod();\n      _v = (uint)x;\n    }\n    else\
    \ if constexpr (is_unsigned_v<T>)\n    {\n      _v = (uint)(v % umod());\n   \
    \ }\n    else\n    {\n      static_assert(is_signed_v<T> || is_unsigned_v<T>,\
    \ \"Unsupported Type\");\n    }\n  }\n\n  int val() const { return (int)_v; }\n\
    \n  mint& operator*=(const mint &rhs)\n  {\n    ull z = _v;\n    z *= rhs._v;\n\
    \    _v = (uint)(z % umod());\n    return *this;\n  }\n\n  mint inv() const\n\
    \  {\n    if (prime)\n    {\n      assert(_v != 0);\n      return CREF.pow(umod()\
    \ - 2);\n    }\n    else\n    {\n      auto [g, x, y] = extgcd<int>(_v, m);\n\
    \      assert(g == 1);\n      return x;\n    }\n  }\n};\n\ntemplate <int id>\n\
    struct dynamic_modint : internal::modint_base<dynamic_modint<id>>\n{\n  using\
    \ mint = dynamic_modint;\nprivate:\n  friend struct internal::modint_base<dynamic_modint<id>>;\n\
    \  uint _v;\n  static internal::barrett32 bt;\n  static uint umod() { return bt.umod();\
    \ }\n\npublic:\n  static int mod() { return (int)(bt.umod()); }\n  static void\
    \ set_mod(int m)\n  {\n    assert(m >= 1);\n    bt = internal::barrett32(m);\n\
    \  }\n  static mint raw(int v)\n  {\n    mint x;\n    x._v = v;\n    return x;\n\
    \  }\n\n  dynamic_modint() : _v(0) {}\n  template <class T>\n  dynamic_modint(T\
    \ v)\n  {\n    if constexpr (is_signed_v<T>)\n    {\n      ll x = (ll)(v % (ll)(umod()));\n\
    \      if (x < 0)\n        x += umod();\n      _v = (uint)x;\n    }\n    else\
    \ if constexpr (is_unsigned_v<T>)\n    {\n      _v = (uint)(v % umod());\n   \
    \ }\n    else\n    {\n      static_assert(is_signed_v<T> || is_unsigned_v<T>,\
    \ \"Unsupported Type\");\n    }\n  }\n\n  int val() const { return (int)_v; }\n\
    \n  mint& operator*=(const mint &rhs)\n  {\n    _v = bt.mul(_v, rhs._v);\n   \
    \ return *this;\n  }\n\n  mint inv() const\n  {\n    auto [g, x, y] = extgcd<int>(_v,\
    \ mod());\n    assert(g == 1);\n    return x;\n  }\n};\ntemplate <int id>\ninternal::barrett32\
    \ dynamic_modint<id>::bt(998244353);\n\nusing modint998244353 = static_modint<998244353>;\n\
    using modint1000000007 = static_modint<1000000007>;\nusing modint = dynamic_modint<-1>;\n\
    #line 2 \"math/modint/modint64.hpp\"\n\n#line 2 \"math/modint/modint64_internal.hpp\"\
    \n\n#line 5 \"math/modint/modint64_internal.hpp\"\n\nnamespace internal\n{\n\n\
    constexpr ll powmod64_constexpr(ll x, ll n, ll m)\n{\n  if (m == 1)\n    return\
    \ 0;\n  ull _m = (ull)m;\n  ull r = 1;\n  ull y = safemod(x, m);\n  while (n)\n\
    \  {\n    u128 y128(y);\n    if (n & 1)\n      r = (y128 * r) % _m;\n    y = (y128\
    \ * y) % _m;\n    n >>= 1;\n  }\n  return r;\n}\n\nconstexpr bool isprime64_constexpr(ll\
    \ n)\n{\n  if (n <= INT_MAX)\n    return isprime32_constexpr(n);\n  if (n % 2\
    \ == 0)\n    return false;\n  ll d = n - 1;\n  while (d % 2 == 0)\n    d /= 2;\n\
    \  constexpr ll bases[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};\n\
    \  for (ll a : bases)\n  {\n    ll t = d;\n    ll y = powmod64_constexpr(a, t,\
    \ n);\n    while (t != n - 1 && y != 1 && y != n - 1)\n    {\n      y = (u128(y)\
    \ * y) % n;\n      t <<= 1;\n    }\n    if (y != n - 1 && t % 2 == 0)\n      return\
    \ false;\n  }\n  return true;\n}\ntemplate <ll n>\nconstexpr bool isprime64 =\
    \ isprime64_constexpr(n);\n\ninline constexpr ull inv64(ull a)\n{\n  ull x = a;\n\
    \  while (a * x != 1) x *= 2 - a * x;\n  return x;\n}\n\nstruct montgomery64odd\n\
    {\n  ull m, im, sq;\n  // sq = (2^64)^2 % m = (2^128 - m) % m = (-m % 2^128) %\
    \ m\n  explicit montgomery64odd(ull m) : m(m), im(inv64(m)), sq(-u128(m) % m)\
    \ {}\n  ull umod() const { return m; }\n  ull reduce(u128 x) const\n  {\n    auto\
    \ t = (x + u128(m) * (-im * ull(x))) >> 64;\n    if (t >= m)\n      t -= m;\n\
    \    return (ull)t;\n  }\n  ull inv_reduce(i128 v) const\n  { return reduce(u128(v\
    \ % m + m) * sq); }\n};\n\n// https://www.mathenachia.blog/even-mod-montgomery-impl/\n\
    struct montgomery64\n{\n  ull m, mx, imx, d, q;\n  uint b;\n\n  explicit montgomery64(ull\
    \ m) : m(m)\n  {\n    b = countr_zero(m), mx = m >> b;  // m == 2^b * mx, mx is\
    \ odd\n    imx = inv64(mx);\n    d = powmod64_constexpr((mx + 1) / 2, b, mx);\
    \  // 2^{-b} mod mx\n    u128 sq = -u128(mx) % mx;  // 2^128 mod mx\n    q = (1\
    \ + (((sq - 1) * d) << b)) % m;\n  }\n  ull umod() const { return m; }\n  ull\
    \ reduce(u128 x) const\n  {\n    ull p = x & MASK(b);  // x mod 2^b\n    x = (x\
    \ >> b) + p * d;\n    ull y = p << (64 - b);\n    auto t = (x + u128(mx) * (imx\
    \ * (y - ull(x)))) >> (64 - b);\n    if (t >= m)\n    {\n      t -= m;\n     \
    \ if (t >= m)\n        t -= m;\n    }\n    return (ull)t;\n  }\n  ull inv_reduce(i128\
    \ v) const\n  { return reduce(u128(v % m + m) * q); }\n};\n\n}\n#line 7 \"math/modint/modint64.hpp\"\
    \n\n/**\n * @brief modint (64 bit)\n * @docs docs/math/modint/modint64.md\n */\n\
    \ntemplate <ll m>\nstruct static_modint64 : internal::modint_base<static_modint64<m>>\n\
    {\n  using mint = static_modint64;\nprivate:\n  friend struct internal::modint_base<static_modint64<m>>;\n\
    \  ull _v;\n  static constexpr ull umod() { return m; }\n  static constexpr bool\
    \ prime = internal::isprime64<m>;\n\npublic:\n  static constexpr ll mod() { return\
    \ m; }\n  static mint raw(ll v)\n  {\n    mint x;\n    x._v = v;\n    return x;\n\
    \  }\n\n  static_modint64() : _v(0) {}\n  template <class T>\n  static_modint64(T\
    \ v)\n  {\n    if constexpr (is_unsigned_v<T>)\n    {\n      _v = (ull)(v % umod());\n\
    \    }\n    else\n    {\n      ll x = (ll)(v % (ll)(umod()));\n      if (x < 0)\n\
    \        x += umod();\n      _v = (ull)x;\n    }\n  }\n\n  ll val() const { return\
    \ (ll)_v; }\n\n  mint& operator*=(const mint &rhs)\n  {\n    u128 z = _v;\n  \
    \  z *= rhs._v;\n    _v = (ull)(z % umod());\n    return *this;\n  }\n\n  mint\
    \ inv() const\n  {\n    if (prime)\n    {\n      assert(_v != 0);\n      return\
    \ CREF.pow(umod() - 2);\n    }\n    else\n    {\n      auto [g, x, y] = extgcd<ll>(_v,\
    \ m);\n      assert(g == 1);\n      return x;\n    }\n  }\n};\n\ntemplate <int\
    \ id>\nstruct dynamic_modint64_odd : internal::modint_base<dynamic_modint64_odd<id>>\n\
    {\n  using mint = dynamic_modint64_odd;\nprivate:\n  friend struct internal::modint_base<dynamic_modint64_odd<id>>;\n\
    \  ull _v; // montgomery expression\n  static internal::montgomery64odd mg;\n\
    \  static ull umod() { return mg.umod(); }\n\npublic:\n  static ll mod() { return\
    \ (ll)(mg.umod()); }\n  static void set_mod(ll m)\n  {\n    assert(m >= 1 && m\
    \ % 2 == 1);\n    mg = internal::montgomery64odd(m);\n  }\n\n  dynamic_modint64_odd()\
    \ : _v(0) {}\n  dynamic_modint64_odd(i128 v)\n  { _v = mg.inv_reduce(v); }\n\n\
    \  ll val() const { return (ll)mg.reduce(_v); }\n\n  mint& operator*=(const mint\
    \ &rhs)\n  {\n    _v = mg.reduce(u128(_v) * rhs._v);\n    return *this;\n  }\n\
    \n  mint inv() const\n  {\n    auto [g, x, y] = extgcd<ll>(val(), mod());\n  \
    \  assert(g == 1);\n    return x;\n  }\n};\ntemplate <int id>\ninternal::montgomery64odd\
    \ dynamic_modint64_odd<id>::mg((1LL << 61) - 1);\n\ntemplate <int id>\nstruct\
    \ dynamic_modint64 : internal::modint_base<dynamic_modint64<id>>\n{\n  using mint\
    \ = dynamic_modint64;\nprivate:\n  friend struct internal::modint_base<dynamic_modint64<id>>;\n\
    \  ull _v; // montgomery expression\n  static internal::montgomery64 mg;\n  static\
    \ ull umod() { return mg.umod(); }\n\npublic:\n  static ll mod() { return (ll)(mg.umod());\
    \ }\n  static void set_mod(ll m)\n  {\n    assert(m >= 1);\n    mg = internal::montgomery64(m);\n\
    \  }\n\n  dynamic_modint64() : _v(0) {}\n  dynamic_modint64(i128 v)\n  { _v =\
    \ mg.inv_reduce(v); }\n\n  ll val() const { return (ll)mg.reduce(_v); }\n\n  mint&\
    \ operator*=(const mint &rhs)\n  {\n    _v = mg.reduce(u128(_v) * rhs._v);\n \
    \   return *this;\n  }\n\n  mint inv() const\n  {\n    auto [g, x, y] = extgcd<ll>(val(),\
    \ mod());\n    assert(g == 1);\n    return x;\n  }\n};\ntemplate <int id>\ninternal::montgomery64\
    \ dynamic_modint64<id>::mg((1LL << 61) - 1);\n\nusing modint61 = static_modint64<(1LL\
    \ << 61) - 1>;\nusing modint64_odd = dynamic_modint64_odd<-1>;\nusing modint64\
    \ = dynamic_modint64<-1>;\n#line 2 \"math/prime/prime_power.hpp\"\n\n#line 4 \"\
    math/prime/prime_power.hpp\"\n\n/**\n * @brief \u7D20\u3079\u304D\u69CB\u9020\u4F53\
    \n * @docs docs/math/prime/prime_power.md\n */\n\ntemplate <class P>\nstruct PrimePower\n\
    {\n  P p;\n  int e;\n  P pe;\n\n  PrimePower() : p(-1), e(-1), pe(-1) {}\n  PrimePower(P\
    \ p, int e = 1) : p(p), e(e), pe(ipow(p, e)) {}\n  PrimePower(P p, int e, P pe)\
    \ : p(p), e(e), pe(pe) {}\n  template <class P2>\n  PrimePower(const PrimePower<P2>\
    \ &pp) : p(pp.p), e(pp.e), pe(pp.pe) {}\n\n  template <class P2>\n  bool operator==(const\
    \ PrimePower<P2> &rhs) const\n  { return p == rhs.p && e == rhs.e && pe == rhs.pe;\
    \ }\n  template <class P2>\n  bool operator!=(const PrimePower<P2> &rhs) const\
    \ { return *this != rhs; }\n\n  void mul_p() { e++, pe = ull(pe) * ull(p); }\n\
    \  void div_p() { e--, pe /= p; }\n};\n#ifdef LOCAL\nCPP_DUMP_DEFINE_EXPORT_OBJECT(PrimePower<int>,\
    \ p, e, pe);\nCPP_DUMP_DEFINE_EXPORT_OBJECT(PrimePower<ll>, p, e, pe);\n#endif\n\
    \n// n \u304C m \u3067\u5272\u308A\u5207\u308C\u308B\u56DE\u6570 e \u306B\u3064\
    \u3044\u3066\u3001(e, m^e, n/m^e)\ntuple<int, ll, ll> ord_pow_div(ll n, ll m)\n\
    {\n  assert(m >= 2);\n  if (m == 2)\n  {\n    int e = countr_zero(n);\n    return\
    \ {e, 1LL << e, n >> e};\n  }\n  if (n % m != 0)\n    return {0, 1, n};\n  n /=\
    \ m;\n  if (n % m != 0)\n    return {1, m, n};\n  n /= m;\n  ll m2 = m * m;\n\
    \  auto [f, m2f, nn] = ord_pow_div(n, m2);\n  int e = 2 + 2 * f;\n  ll me = m2f\
    \ * m2;\n  if (nn % m == 0)\n    e++, me *= m, nn /= m;\n  return {e, me, nn};\n\
    }\n\n// \u76F8\u7570\u306A\u308B\u7D20\u56E0\u6570\ntemplate <class P>\nvc<P>\
    \ factors(const vc<PrimePower<P>> &fac)\n{\n  vc<P> res(fac.size());\n  repi(i,\
    \ fac.size()) res[i] = fac[i].p;\n  return res;\n}\n\n// \u5F15\u6570 fac \u306F\
    \u7D20\u56E0\u6570\u5206\u89E3\u5F62\ntemplate <class P>\nvc<ll> divisors(const\
    \ vc<PrimePower<P>> &fac)\n{\n  vc<ll> res;\n  auto dfs = [&](auto dfs, ll d,\
    \ int i) -> void\n  {\n    if (i == SZ<int>(fac))\n    {\n      res.emplace_back(d);\n\
    \      return;\n    }\n    auto &pp = fac[i];\n    ull nd = d;\n    repi(j, pp.e\
    \ + 1)\n    {\n      dfs(dfs, nd, i + 1);\n      nd *= pp.p;\n    }\n  };\n  dfs(dfs,\
    \ 1, 0);\n  sort(ALL(res));\n  return res;\n}\n\ntemplate <class P>\nvc<PrimePower<P>>\
    \ factorized_mul\n(const vc<PrimePower<P>> &fac1, const vc<PrimePower<P>> &fac2)\n\
    {\n  const int n = fac1.size(), m = fac2.size();\n  vc<PrimePower<P>> fac;\n \
    \ fac.reserve(n + m);\n  int i = 0, j = 0;\n  while (i < n && j < m)\n  {\n  \
    \  if (fac1[i].p < fac2[j].p)\n      fac.emplace_back(fac1[i++]);\n    else if\
    \ (fac1[i].p > fac2[j].p)\n      fac.emplace_back(fac2[j++]);\n    else\n    {\n\
    \      fac.emplace_back(fac1[i].p, fac1[i].e + fac2[j].e, ull(fac1[i].pe) * ull(fac2[j].pe));\n\
    \      i++, j++;\n    }\n  }\n  fac.insert(fac.end(), fac1.begin() + i, fac1.end());\n\
    \  fac.insert(fac.end(), fac2.begin() + j, fac2.end());\n  return fac;\n}\n#line\
    \ 2 \"math/prime/primality_test.hpp\"\n\n#line 6 \"math/prime/primality_test.hpp\"\
    \n\n/**\n * @brief \u7D20\u6570\u5224\u5B9A\n * @docs docs/math/prime/primality_test.md\n\
    \ */\n\nnamespace internal\n{\n\ntemplate <class mint, class Array>\nbool is_prime_impl(ll\
    \ n, const Array &bases)\n{\n  if (n <= 1)\n    return false;\n  if (n == 2 ||\
    \ n == 7 || n == 61)\n    return true;\n  if (n % 2 == 0)\n    return false;\n\
    \  ll d = (n - 1) >> countr_zero(n - 1);\n  mint::set_mod(n);\n  for (ll a : bases)\n\
    \  {\n    ll t = d;\n    mint y = mint(a).pow(t);\n    while (t != n - 1 && y\
    \ != 1 && y != n - 1)\n    {\n      y *= y;\n      t <<= 1;\n    }\n    if (y\
    \ != n - 1 && t % 2 == 0)\n      return false;\n  }\n  return true;\n}\n\n}; //\
    \ namespace internal\n\nbool is_prime(ll n)\n{\n  static constexpr array<ll, 3>\
    \ bases32 = {2, 7, 61};\n  static constexpr array<ll, 7> bases64 = {2, 325, 9375,\
    \ 28178, 450775, 9780504, 1795265022};\n  if (n <= INT_MAX)\n  {\n    using mint\
    \ = dynamic_modint<INT_MIN>;\n    return internal::is_prime_impl<mint>(n, bases32);\n\
    \  }\n  else\n  {\n    using mint = dynamic_modint64_odd<INT_MIN>;\n    return\
    \ internal::is_prime_impl<mint>(n, bases64);\n  }\n}\n#line 8 \"math/prime/factorize.hpp\"\
    \n\n/**\n * @brief \u7D20\u56E0\u6570\u5206\u89E3\n * @docs docs/math/prime/factorize.md\n\
    \ */\n\nnamespace internal\n{\n\ntemplate <class mint>\nll get_prime_factor_impl(ll\
    \ n)\n{\n  mint::set_mod(n);\n  int m = pow(n, .125);\n  mt19937 _mt;\n  while\
    \ (true)\n  {\n    int c = 1 + _mt() % 100;\n    mint x = 2, y = 2, prod = 1;\n\
    \    ll g = 1;\n    while (g == 1)\n    {\n      repi(i, m)\n      {\n       \
    \ x = x * x + c;\n        y = y * y + c, y = y * y + c;\n        prod *= x - y;\n\
    \      }\n      g = gcd(prod.val(), n);\n    }\n    if (g == n)\n      continue;\n\
    \    if (is_prime(g))\n      return g;\n    else if (is_prime(n / g))\n      return\
    \ n / g;\n    else\n      return get_prime_factor_impl<mint>(g);\n  }\n}\n\nll\
    \ get_prime_factor(ll n)\n{\n  if (n <= INT_MAX)\n  {\n    using mint = dynamic_modint<INT_MIN>;\n\
    \    return get_prime_factor_impl<mint>(n);\n  }\n  else\n  {\n    using mint\
    \ = dynamic_modint64_odd<INT_MIN>;\n    return get_prime_factor_impl<mint>(n);\n\
    \  }\n}\n\n}; // namespace internal\n\nvc<PrimePower<ll>> factorize(ll n)\n{\n\
    \  vc<PrimePower<ll>> res;\n  repi(p, 2, 100)\n  {\n    if (n % p == 0)\n    {\n\
    \      auto [e, pe, nn] = ord_pow_div(n, p);\n      res.emplace_back(PrimePower<ll>(p,\
    \ e, pe));\n      n = nn;\n    }\n  }\n  while (n > 1)\n  {\n    if (is_prime(n))\n\
    \    {\n      res.emplace_back(n);\n      break;\n    }\n    ll p = internal::get_prime_factor(n);\n\
    \    auto [e, pe, nn] = ord_pow_div(n, p);\n    res.emplace_back(PrimePower<ll>(p,\
    \ e, pe));\n    n = nn;\n  }\n  sort(ALL(res), [&](const PrimePower<ll> &pp1,\
    \ const PrimePower<ll> &pp2)\n       { return pp1.p < pp2.p; });\n  return res;\n\
    }\n#line 2 \"math/algebra/algebra_basic_ops.hpp\"\n\n#line 2 \"math/algebra/algebra_base.hpp\"\
    \n\n#line 4 \"math/algebra/algebra_base.hpp\"\n\n/**\n * @brief \u4EE3\u6570\u7684\
    \u69CB\u9020\u306E struct\uFF08\u57FA\u672C\uFF09\n * @docs docs/math/algebra/algebra_base.md\n\
    \ */\n\ntemplate <class S_, auto op_, auto e_>\nstruct Monoid\n{\n  using S =\
    \ S_;\n  static constexpr auto op = op_;\n  static constexpr auto e = e_;\n};\n\
    \ntemplate <class S_, auto op_, auto e_, auto inv_>\nstruct Group\n{\n  using\
    \ S = S_;\n  static constexpr auto op = op_;\n  static constexpr auto e = e_;\n\
    \  static constexpr auto inv = inv_;\n};\n\ntemplate <class S_, auto add_, auto\
    \ e0_, auto mul_, auto e1_>\nstruct SemiRing\n{\n  using S = S_;\n  static constexpr\
    \ auto add = add_;\n  static constexpr auto e0 = e0_;\n  static constexpr auto\
    \ mul = mul_;\n  static constexpr auto e1 = e1_;\n};\n\ntemplate <class S_, auto\
    \ add_, auto e0_, auto minus_, auto mul_, auto e1_>\nstruct Ring\n{\n  using S\
    \ = S_;\n  static constexpr auto add = add_;\n  static constexpr auto e0 = e0_;\n\
    \  static constexpr auto minus = minus_;\n  static constexpr auto mul = mul_;\n\
    \  static constexpr auto e1 = e1_;\n};\n\ntemplate <class SR>\nusing MonoidOfSemiRingAdd\
    \ = Monoid<typename SR::S, SR::add, SR::e0>;\ntemplate <class SR>\nusing MonoidOfSemiRingMul\
    \ = Monoid<typename SR::S, SR::mul, SR::e1>;\ntemplate <class R>\nusing GroupOfRingAdd\
    \ = Group<typename R::S, R::add, R::e0, R::minus>;\n#line 5 \"math/algebra/algebra_basic_ops.hpp\"\
    \n\n/**\n * @brief \u4EE3\u6570\u7684\u69CB\u9020\uFF08\u56DB\u5247\u6F14\u7B97\
    \u3068 min, max\uFF09\n * @docs docs/math/algebra/algebra_basic_ops.md\n */\n\n\
    template <class T>\nstruct MonoidAdd\n{\n  using S = T;\n  static constexpr S\
    \ op(S a, S b) { return a + b; }\n  static constexpr S e() { return 0; }\n};\n\
    template <class T>\nstruct MonoidMul\n{\n  using S = T;\n  static constexpr S\
    \ op(S a, S b) { return a * b; }\n  static constexpr S e() { return 1; }\n};\n\
    template <class T, const T infty = INF>\nstruct MonoidMin\n{\n  using S = T;\n\
    \  static constexpr S op(S a, S b) { return min(a, b); }\n  static constexpr S\
    \ e() { return infty; }\n};\ntemplate <class T, const T infty = INF>\nstruct MonoidMax\n\
    {\n  using S = T;\n  static constexpr S op(S a, S b) { return max(a, b); }\n \
    \ static constexpr S e() { return -infty; }\n};\n\ntemplate <class T>\nstruct\
    \ GroupAddSub\n{\n  using S = T;\n  static constexpr S op(S a, S b) { return a\
    \ + b; }\n  static constexpr S e() { return 0; }\n  static constexpr S inv(S a)\
    \ { return -a; }\n};\ntemplate <class T>\nstruct GroupMulDiv\n{\n  using S = T;\n\
    \  static constexpr S op(S a, S b) { return a * b; }\n  static constexpr S e()\
    \ { return 1; }\n  static constexpr S inv(S a) { return 1 / a; }\n};\n\ntemplate\
    \ <class T, const T infty = INF>\nstruct SemiRingMinPlus\n{\n  using S = T;\n\
    \  static constexpr S add(S a, S b) { return min(a, b); }\n  static constexpr\
    \ S e0() { return infty; }\n  static constexpr S mul(S a, S b) { return a + b;\
    \ }\n  static constexpr S e1() { return 0; }\n};\ntemplate <class T, const T infty\
    \ = INF>\nstruct SemiRingMaxPlus\n{\n  using S = T;\n  static constexpr S add(S\
    \ a, S b) { return max(a, b); }\n  static constexpr S e0() { return -infty; }\n\
    \  static constexpr S mul(S a, S b) { return a + b; }\n  static constexpr S e1()\
    \ { return 0; }\n};\n\ntemplate <class T>\nstruct RingAddSubMul\n{\n  using S\
    \ = T;\n  static constexpr S add(S a, S b) { return a + b; }\n  static constexpr\
    \ S minus(S a) { return -a; }\n  static constexpr S e0() { return 0; }\n  static\
    \ constexpr S mul(S a, S b) { return a * b; }\n  static constexpr S e1() { return\
    \ 1; }\n};\n#line 6 \"math/prime/zeta_mobius_divisor_multiple_large.hpp\"\n\n\
    /**\n * @brief \u7D04\u6570\u30FB\u500D\u6570 \u30BC\u30FC\u30BF\u30FB\u30E1\u30D3\
    \u30A6\u30B9\u5909\u63DB\uFF08\u5927\u304D\u3044 $m$ \u306E\u7D04\u6570\uFF09\n\
    \ * @docs docs/math/prime/zeta_mobius_divisor_multiple_large.md\n */\n\nstruct\
    \ ZetaMobiusDivisorMultipleLarge\n{\npublic:\n  ll m;\n  vc<PrimePower<ll>> fac;\n\
    \  ll pnum, dnum;\n  vc<ll> ds;\n\nprivate:\n  vc<int> f01;  // f01[d] \u306F\u3001\
    d \u304C f[j] == e[j] \u306A\u3089 j \u30D3\u30C3\u30C8\u76EE\u304C 1\n\npublic:\n\
    \  ZetaMobiusDivisorMultipleLarge() {}\n  ZetaMobiusDivisorMultipleLarge(ll m)\
    \ : m(m)\n  {\n    fac = factorize(m);\n    pnum = fac.size();\n    dnum = 1;\n\
    \    fec(pp : fac) dnum *= pp.e + 1;\n\n    ds.resize(dnum), f01.resize(dnum);\n\
    \    vc<int> f(pnum, 0);\n    ll d = 1;\n    for (int i = 0;; i++)\n    {\n  \
    \    ds[i] = d;\n      rep(j, pnum) bset(f01[i], j, f[j] == fac[j].e);\n     \
    \ if (i == dnum - 1)\n        break;\n      rep(j, pnum - 1, -1, -1)\n      {\n\
    \        if (f[j] == fac[j].e)\n        {\n          f[j] = 0;\n          d /=\
    \ fac[j].pe;\n        }\n        else\n        {\n          f[j]++;\n        \
    \  d *= fac[j].p;\n          break;\n        }\n      }\n    }\n  }\n\npublic:\n\
    \  // d \u304B\u3089 ds[i] = d \u3068\u306A\u308B i\n  int dtoi(ll d) const\n\
    \  {\n    //                f3\n    // +       (e3+1) f2\n    // + (e2+1)(e3+1)\
    \ f1\n    // = f3+(e3+1)(f2+(e2+1)f1)\n    assert(d > 0 && m % d == 0);\n    int\
    \ res = 0;\n    fec(pp : fac)\n    {\n      auto [f, pf, nd] = ord_pow_div(d,\
    \ pp.p);\n      d = nd;\n      res *= pp.e + 1;\n      res += f;\n    }\n    return\
    \ res;\n  }\n  /*\n  // f3 = i % (e3+1)\n  // f2 = (i // (e3+1)) % (e2+1)\n  ll\
    \ itod(int i) const\n  {\n    ll d = 1;\n    fec(pp : reversed(fac))\n    {\n\
    \      d *= ipow(pp.p, i % (pp.e + 1));\n      i /= pp.e + 1;\n    }\n    return\
    \ d;\n  }\n  //*/\n\npublic:\n  template <class T>\n  struct DivisorMap\n  {\n\
    \  private:\n    const ZetaMobiusDivisorMultipleLarge &zm;\n    vc<T> v;\n\n \
    \   friend struct ZetaMobiusDivisorMultipleLarge;\n\n  public:\n    DivisorMap()\
    \ {}\n    DivisorMap(const ZetaMobiusDivisorMultipleLarge &zm)\n    : zm(zm),\
    \ v(zm.dnum) {}\n    template <class F>\n    DivisorMap(const ZetaMobiusDivisorMultipleLarge\
    \ &zm, const F &func)\n    : zm(zm), v(zm.dnum) { repi(i, zm.dnum) v[i] = func(zm.ds[i]);\
    \ }\n\n    // m \u306E\u7D04\u6570 d \u306B\u5BFE\u3057\u3066\u5024\u3092\u53D6\
    \u5F97\n    // \u5909\u66F4\u3082\u53EF\u80FD\n    // O(loglog d)\n    T &get_by_d(ll\
    \ d) { return v[zm.dtoi(d)]; }\n    // m \u306E\u7D04\u6570 d \u306B\u5BFE\u3057\
    \u3066\u5024\u3092\u53D6\u5F97\n    // \u5909\u66F4\u3082\u53EF\u80FD\n    //\
    \ O(loglog d)\n    const T &get_by_d(ll d) const { return v[zm.dtoi(d)]; }\n\n\
    \    // m \u306E\u7D04\u6570 d \u306B\u5BFE\u3057\u3001i = dtoi(d) \u304B\u3089\
    \u5024\u3092\u53D6\u5F97\n    // \u5909\u66F4\u3082\u53EF\u80FD\n    T &get_by_i(ll\
    \ i) { return v[i]; }\n    // m \u306E\u7D04\u6570 d \u306B\u5BFE\u3057\u3001\
    i = dtoi(d) \u304B\u3089\u5024\u3092\u53D6\u5F97\n    // \u5909\u66F4\u3082\u53EF\
    \u80FD\n    const T &get_by_i(ll i) const { return v[i]; }\n\n    map<ll, T> to_map()\n\
    \    {\n      map<ll, T> res;\n      fec(d : zm.ds) res[d] = get_by_d(d);\n  \
    \    return res;\n    }\n  };\n\n  template <class T>\n  DivisorMap<T> divisor_map()\
    \ const\n  { return DivisorMap<T>(*this); }\n  template <class T, class F>\n \
    \ DivisorMap<T> divisor_map(const F &func) const\n  { return DivisorMap<T>(*this,\
    \ func); }\n\n  // \u03B6a(n) = \u03A3{d | n} a(d)\n  // M \u306F\u53EF\u63DB\u30E2\
    \u30CE\u30A4\u30C9 (\u03A3 \u3060\u3068 +)\n  // O(\u7D04\u6570\u500B\u6570 *\
    \ \u7D20\u56E0\u6570\u500B\u6570)\n  template <class M>\n  DivisorMap<typename\
    \ M::S> zeta_divisor\n  (const DivisorMap<typename M::S> &a) const\n  {\n    auto\
    \ b = a;\n    for (int j = pnum - 1, k = 1; j >= 0; k *= fac[j].e + 1, j--)\n\
    \    {\n      repi(i, dnum)\n      {\n        if (!btest(f01[i], j))\n       \
    \   b.v[i + k] = M::op(b.v[i + k], b.v[i]);\n      }\n    }\n    return b;\n \
    \ }\n\n  // \u03BC \u306F \u03B6 \u306E\u9006\u5909\u63DB\n  // \u03BCa(n) = \u03A3\
    {d | n} \u03BC(n/d)a(d)  cf. \u30E1\u30D3\u30A6\u30B9\u306E\u53CD\u8EE2\u516C\u5F0F\
    \n  // G \u306F\u53EF\u63DB\u7FA4 (\u03A3 \u3060\u3068 +, -)\n  // O(\u7D04\u6570\
    \u500B\u6570 * \u7D20\u56E0\u6570\u500B\u6570)\n  template <class G>\n  DivisorMap<typename\
    \ G::S> mobius_divisor\n  (const DivisorMap<typename G::S> &a) const\n  {\n  \
    \  auto b = a;\n    for (int j = pnum - 1, k = 1; j >= 0; k *= fac[j].e + 1, j--)\n\
    \    {\n      repi(i, dnum - 1, -1, -1)\n      {\n        if (!btest(f01[i], j))\n\
    \          b.v[i + k] = G::op(b.v[i + k], G::inv(b.v[i]));\n      }\n    }\n \
    \   return b;\n  }\n\n  // \u03BC \u306F \u03B6 \u306E\u9006\u5909\u63DB\n  //\
    \ \u03BCa(n) = \u03A3{d | n} \u03BC(n/d)a(d)  cf. \u30E1\u30D3\u30A6\u30B9\u306E\
    \u53CD\u8EE2\u516C\u5F0F\n  // \u03BCa(n) \u306E 1 \u70B9\u3060\u3051\u6B32\u3057\
    \u3044\u3068\u304D\u306B\u4F7F\u3046\n  // G \u306F\u53EF\u63DB\u7FA4 (\u03A3\
    \ \u3060\u3068 +, -)\n  // O(\u7D20\u56E0\u6570\u500B\u6570 * 2^\u7D20\u56E0\u6570\
    \u500B\u6570)\n  template <class G>\n  typename G::S mobius_divisor_point\n  (const\
    \ DivisorMap<typename G::S> &a, ll n) const\n  {\n    typename G::S res = G::e();\n\
    \    int si = dtoi(n);\n    repi(bit, 1 << pnum)\n    {\n      int i = si;\n \
    \     for (int j = pnum - 1, k = 1; j >= 0; k *= fac[j].e + 1, j--)\n      {\n\
    \        if (btest(bit, j) && i - k >= 0 && !btest(f01[i - k], j))\n         \
    \ i -= k;\n      }\n      if (popcount(bit) % 2 == 0)\n        res = G::op(res,\
    \ a.v[i]);\n      else\n        res = G::op(res, G::inv(a.v[i]));\n    }\n   \
    \ return res;\n  }\n\n  // \u03B6'a(n) = \u03A3{n | m} a(m)\n  // M \u306F\u53EF\
    \u63DB\u30E2\u30CE\u30A4\u30C9 (\u03A3 \u3060\u3068 +)\n  // O(\u7D04\u6570\u500B\
    \u6570 * \u7D20\u56E0\u6570\u500B\u6570)\n  template <class M>\n  DivisorMap<typename\
    \ M::S> zeta_multiple\n  (const DivisorMap<typename M::S> &a) const\n  {\n   \
    \ auto b = a;\n    for (int j = pnum - 1, k = 1; j >= 0; k *= fac[j].e + 1, j--)\n\
    \    {\n      repi(i, dnum - 1, -1, -1)\n      {\n        if (!btest(f01[i], j))\n\
    \          b.v[i] = M::op(b.v[i], b.v[i + k]);\n      }\n    }\n    return b;\n\
    \  }\n\n  // \u03BC' \u306F \u03B6' \u306E\u9006\u5909\u63DB\n  // \u03BC'a(n)\
    \ = \u03A3{n | m} \u03BC(m/n)g(m)  cf. \u30E1\u30D3\u30A6\u30B9\u306E\u53CD\u8EE2\
    \u516C\u5F0F\n  // G \u306F\u53EF\u63DB\u7FA4 (\u03A3 \u3060\u3068 +, -)\n  //\
    \ O(\u7D04\u6570\u500B\u6570 * \u7D20\u56E0\u6570\u500B\u6570)\n  template <class\
    \ G>\n  DivisorMap<typename G::S> mobius_multiple\n  (const DivisorMap<typename\
    \ G::S> &a) const\n  {\n    auto b = a;\n    for (int j = pnum - 1, k = 1; j >=\
    \ 0; k *= fac[j].e + 1, j--)\n    {\n      repi(i, dnum)\n      {\n        if\
    \ (!btest(f01[i], j))\n          b.v[i] = G::op(b.v[i], G::inv(b.v[i + k]));\n\
    \      }\n    }\n    return b;\n  }\n\n  // \u03BC' \u306F \u03B6' \u306E\u9006\
    \u5909\u63DB\n  // \u03BC'a(n) = \u03A3{n | m} \u03BC(m/n)g(m)  cf. \u30E1\u30D3\
    \u30A6\u30B9\u306E\u53CD\u8EE2\u516C\u5F0F\n  // \u03BC'a(n) \u306E 1 \u70B9\u3060\
    \u3051\u6B32\u3057\u3044\u3068\u304D\u306B\u4F7F\u3046\n  // G \u306F\u53EF\u63DB\
    \u7FA4 (\u03A3 \u3060\u3068 +, -)\n  // O(\u7D20\u56E0\u6570\u500B\u6570 * 2^\u7D20\
    \u56E0\u6570\u500B\u6570)\n  template <class G>\n  typename G::S mobius_multiple_point\n\
    \  (const DivisorMap<typename G::S> &a, ll n) const\n  {\n    typename G::S res\
    \ = G::e();\n    int si = dtoi(n);\n    repi(bit, 1 << pnum)\n    {\n      int\
    \ i = si;\n      for (int j = pnum - 1, k = 1; j >= 0; k *= fac[j].e + 1, j--)\n\
    \      {\n        if (btest(bit, j) && !btest(f01[i], j))\n          i += k;\n\
    \      }\n      if (popcount(bit) % 2 == 0)\n        res = G::op(res, a.v[i]);\n\
    \      else\n        res = G::op(res, G::inv(a.v[i]));\n    }\n    return res;\n\
    \  }\n};\n#line 18 \"verify/yukicoder/zeta_mobius_multiple_large.test.cpp\"\n\n\
    #line 20 \"verify/yukicoder/zeta_mobius_multiple_large.test.cpp\"\nusing mint\
    \ = modint998244353;\n\nvoid init() {}\n\nvoid main2()\n{\n  LL(T, M);\n  ZetaMobiusDivisorMultipleLarge\
    \ zm(M);\n  rep(_, T)\n  {\n    LL(N, B, C, D);\n    VEC(ll, N, A);\n    vc<mint>\
    \ W(N);\n    W.at(0) = B;\n    rep(i, 1, N) W.at(i) = C * W.at(i - 1) + D;\n\n\
    \    auto h = zm.divisor_map<mint>([&](ll)\n                                 \
    \ { return 1; });\n    rep(i, N)\n    {\n      if (M % A.at(i) != 0)\n       \
    \ continue;\n      h.get_by_d(M / A.at(i)) *= 1 + W.at(i);\n    }\n    dump(h.to_map());\n\
    \    auto g = zm.zeta_multiple<MonoidMul<mint>>(h);\n    dump(g.to_map());\n\n\
    \    auto f = zm.mobius_multiple<GroupAddSub<mint>>(g);\n    dump(f.to_map());\n\
    \    mint ans = f.get_by_d(1);\n    if (M == 1)\n      ans--;\n    PRINT(ans);\n\
    \n    mint ans2 = zm.mobius_multiple_point<GroupAddSub<mint>>(g, 1);\n    if (M\
    \ == 1)\n      ans2--;\n    dump(ans2);\n    assert(ans == ans2);\n  }\n}\n\n\
    void test() {}\n\nint main()\n{\n  cauto CERR = [](string val, string color)\n\
    \  {\n    string s = \"\\033[\" + color + \"m\" + val + \"\\033[m\";\n    #ifdef\
    \ LOCAL\n    cerr << s;\n    #endif\n    /* \u30B3\u30FC\u30C9\u30C6\u30B9\u30C8\
    \u3067\u78BA\u8A8D\u3059\u308B\u969B\u306B\u30B3\u30E1\u30F3\u30C8\u30A2\u30A6\
    \u30C8\u3092\u5916\u3059\n    cerr << val;\n    //*/\n  };\n\n  #if defined FAST_IO\
    \ and not defined LOCAL\n  CERR(\"\\n[FAST_IO]\\n\\n\", \"32\");\n  #endif\n \
    \ #if defined FAST_CIO and not defined LOCAL\n  CERR(\"\\n[FAST_CIO]\\n\\n\",\
    \ \"32\");\n  cin.tie(0);\n  ios::sync_with_stdio(false);\n  #endif\n  cout <<\
    \ fixed << setprecision(20);\n\n  test();\n  init();\n\n  #if defined AOJ_TESTCASE\
    \ or (defined LOCAL and defined SINGLE_TESTCASE)\n  CERR(\"\\n[AOJ_TESTCASE]\\\
    n\\n\", \"35\");\n  while (true)\n  {\n    dump(\"new testcase\");\n    main2();\n\
    \  }\n  #elif defined SINGLE_TESTCASE\n  CERR(\"\\n[SINGLE_TESTCASE]\\n\\n\",\
    \ \"36\");\n  main2();\n  #elif defined MULTI_TESTCASE\n  CERR(\"\\n[MULTI_TESTCASE]\\\
    n\\n\", \"33\");\n  dump(\"T\");\n  IN(uint, T);\n  while (T--)\n  {\n    dump(\"\
    new testcase\");\n    main2();\n  }\n  #endif\n}\n"
  code: "#define PROBLEM \"https://yukicoder.me/problems/no/2578\"\n\n#define SINGLE_TESTCASE\n\
    // #define MULTI_TESTCASE\n// #define AOJ_TESTCASE\n\n#define FAST_IO\n// #define\
    \ FAST_CIO\n// #define INTERACTIVE\n\n#define INF 4'000'000'000'000'000'037LL\n\
    #define EPS 1e-11\n\n#include \"template/template_all.hpp\"\n\n#include \"math/prime/zeta_mobius_divisor_multiple_large.hpp\"\
    \n#include \"math/algebra/algebra_basic_ops.hpp\"\n\n#include \"math/modint/modint.hpp\"\
    \nusing mint = modint998244353;\n\nvoid init() {}\n\nvoid main2()\n{\n  LL(T,\
    \ M);\n  ZetaMobiusDivisorMultipleLarge zm(M);\n  rep(_, T)\n  {\n    LL(N, B,\
    \ C, D);\n    VEC(ll, N, A);\n    vc<mint> W(N);\n    W.at(0) = B;\n    rep(i,\
    \ 1, N) W.at(i) = C * W.at(i - 1) + D;\n\n    auto h = zm.divisor_map<mint>([&](ll)\n\
    \                                  { return 1; });\n    rep(i, N)\n    {\n   \
    \   if (M % A.at(i) != 0)\n        continue;\n      h.get_by_d(M / A.at(i)) *=\
    \ 1 + W.at(i);\n    }\n    dump(h.to_map());\n    auto g = zm.zeta_multiple<MonoidMul<mint>>(h);\n\
    \    dump(g.to_map());\n\n    auto f = zm.mobius_multiple<GroupAddSub<mint>>(g);\n\
    \    dump(f.to_map());\n    mint ans = f.get_by_d(1);\n    if (M == 1)\n     \
    \ ans--;\n    PRINT(ans);\n\n    mint ans2 = zm.mobius_multiple_point<GroupAddSub<mint>>(g,\
    \ 1);\n    if (M == 1)\n      ans2--;\n    dump(ans2);\n    assert(ans == ans2);\n\
    \  }\n}\n\nvoid test() {}\n\nint main()\n{\n  cauto CERR = [](string val, string\
    \ color)\n  {\n    string s = \"\\033[\" + color + \"m\" + val + \"\\033[m\";\n\
    \    #ifdef LOCAL\n    cerr << s;\n    #endif\n    /* \u30B3\u30FC\u30C9\u30C6\
    \u30B9\u30C8\u3067\u78BA\u8A8D\u3059\u308B\u969B\u306B\u30B3\u30E1\u30F3\u30C8\
    \u30A2\u30A6\u30C8\u3092\u5916\u3059\n    cerr << val;\n    //*/\n  };\n\n  #if\
    \ defined FAST_IO and not defined LOCAL\n  CERR(\"\\n[FAST_IO]\\n\\n\", \"32\"\
    );\n  #endif\n  #if defined FAST_CIO and not defined LOCAL\n  CERR(\"\\n[FAST_CIO]\\\
    n\\n\", \"32\");\n  cin.tie(0);\n  ios::sync_with_stdio(false);\n  #endif\n  cout\
    \ << fixed << setprecision(20);\n\n  test();\n  init();\n\n  #if defined AOJ_TESTCASE\
    \ or (defined LOCAL and defined SINGLE_TESTCASE)\n  CERR(\"\\n[AOJ_TESTCASE]\\\
    n\\n\", \"35\");\n  while (true)\n  {\n    dump(\"new testcase\");\n    main2();\n\
    \  }\n  #elif defined SINGLE_TESTCASE\n  CERR(\"\\n[SINGLE_TESTCASE]\\n\\n\",\
    \ \"36\");\n  main2();\n  #elif defined MULTI_TESTCASE\n  CERR(\"\\n[MULTI_TESTCASE]\\\
    n\\n\", \"33\");\n  dump(\"T\");\n  IN(uint, T);\n  while (T--)\n  {\n    dump(\"\
    new testcase\");\n    main2();\n  }\n  #endif\n}"
  dependsOn:
  - template/template_all.hpp
  - template/template_types.hpp
  - template/template_rep.hpp
  - template/template_math.hpp
  - template/template_vector.hpp
  - template/template_algo.hpp
  - template/template_binsearch.hpp
  - template/template_bit.hpp
  - template/template_inout.hpp
  - template/template_dump.hpp
  - math/prime/zeta_mobius_divisor_multiple_large.hpp
  - math/prime/factorize.hpp
  - math/modint/modint.hpp
  - math/modint/modint32_internal.hpp
  - math/modint/modint_base.hpp
  - math/extgcd.hpp
  - math/modint/modint64.hpp
  - math/modint/modint64_internal.hpp
  - math/prime/prime_power.hpp
  - math/prime/primality_test.hpp
  - math/algebra/algebra_basic_ops.hpp
  - math/algebra/algebra_base.hpp
  isVerificationFile: true
  path: verify/yukicoder/zeta_mobius_multiple_large.test.cpp
  requiredBy: []
  timestamp: '2025-03-16 23:12:25+09:00'
  verificationStatus: TEST_WRONG_ANSWER
  verifiedWith: []
documentation_of: verify/yukicoder/zeta_mobius_multiple_large.test.cpp
layout: document
redirect_from:
- /verify/verify/yukicoder/zeta_mobius_multiple_large.test.cpp
- /verify/verify/yukicoder/zeta_mobius_multiple_large.test.cpp.html
title: verify/yukicoder/zeta_mobius_multiple_large.test.cpp
---
