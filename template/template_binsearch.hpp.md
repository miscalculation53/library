---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: template/template_math.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09"
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
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_binsearch.test.cpp
    title: verify/mytest/template_binsearch.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    _deprecated_at_docs: docs/template/template_binsearch.md
    document_title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u4E8C\u5206\u63A2\u7D22\
      \uFF09"
    links: []
  bundledCode: "#line 2 \"template/template_binsearch.hpp\"\n\n#line 2 \"template/template_types.hpp\"\
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
    #line 2 \"template/template_vector.hpp\"\n\n#line 2 \"template/template_math.hpp\"\
    \n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n#endif\n#ifndef EPS\n\
    #define EPS 1e-11\n#endif\n\n#line 12 \"template/template_math.hpp\"\n\n/**\n\
    \ * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09\n * @docs\
    \ docs/template/template_math.md\n */\n\ninline bool chmin(auto &a, cauto &b)\
    \ { return a > b ? a = b, true : false; }\ninline bool chmax(auto &a, cauto &b)\
    \ { return a < b ? a = b, true : false; }\n\ntemplate <class T = ll>\ninline T\
    \ divfloor(cauto &a, cauto &b) { return T(a) / T(b) - (T(a) % T(b) && (T(a) ^\
    \ T(b)) < 0); }\ntemplate <class T = ll>\ninline T divceil(cauto &a, cauto &b)\
    \ { return T(a) / T(b) + (T(a) % T(b) && (T(a) ^ T(b)) >= 0); }\ntemplate <class\
    \ T = ll>\ninline T divround(cauto &a, cauto &b) { return divfloor<T>(2 * a +\
    \ b, 2 * b); }\ntemplate <class T = ll>\ninline T safemod(cauto &a, cauto &b)\
    \ { return a - b * divfloor<T>(a, b); }\n\ntemplate <class T = ll>\nconstexpr\
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
    \ int(a > eps) - int(a < -eps); }\n\n// \u4F4D\u53D6\u308A\u8A18\u6570\u6CD5\u3068\
    \u540C\u3058\u9806\u756A\uFF08\u4E0B\u4F4D\u6841\u304C\u5F8C\u308D\uFF09\ntemplate\
    \ <class T = ll>\nvc<T> b_ary(cauto &x, const int &b)\n{\n  vc<T> a;\n  while\
    \ (x > 0)\n  {\n    a.emplace_back(x % b);\n    x /= b;\n  }\n  reverse(a.begin(),\
    \ a.end());\n  return a;\n}\n// \u4F4D\u53D6\u308A\u8A18\u6570\u6CD5\u3068\u540C\
    \u3058\u9806\u756A\uFF08\u4E0B\u4F4D\u6841\u304C\u5F8C\u308D\uFF09\ntemplate <class\
    \ T>\nvc<T> b_ary(cauto &x, const int &b, const int &n)\n{\n  vc<T> a(n);\n  repi(i,\
    \ n)\n  {\n    a[i] = x % b;\n    x /= b;\n  }\n  reverse(a.begin(), a.end());\n\
    \  return a;\n}\nstring b_ary_str(cauto &x, const int &b, bool use_upper = true)\n\
    {\n  auto a = b_ary(x, b);\n  string s = \"\";\n  for (cauto &ai : a)\n    s +=\
    \ (ai < 10 ? '0' + ai : (use_upper ? 'A' : 'a') + (ai - 10));\n  return s;\n}\n\
    string b_ary_str(cauto &x, const int &b, const int &n, bool use_upper = true)\n\
    {\n  auto a = b_ary(x, b, n);\n  string s = \"\";\n  for (cauto &ai : a)\n   \
    \ s += (ai < 10 ? '0' + ai : (use_upper ? 'A' : 'a') + (ai - 10));\n  return s;\n\
    }\n#line 6 \"template/template_vector.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\
    \u30EC\u30FC\u30C8\uFF08vector\uFF09\n * @docs docs/template/template_vector.md\n\
    \ */\n\n#define ALL(a) (a).begin(), (a).end()\n#define SZ(x) (ll)((x).size())\n\
    #define SZI(x) (int)((x).size())\n\ntemplate <class F>\nauto gen_vec(const int\
    \ &n, const F &f)\n{\n  vc<decltype(f(0))> res(n);\n  repi(i, n) res[i] = f(i);\n\
    \  return res;\n}\n\n// https://qiita.com/Chippppp/items/13150f5e0ea99f444d97#%E5%A4%9A%E6%AC%A1%E5%85%83vector%E7%94%9F%E6%88%90%E9%96%A2%E6%95%B0\n\
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
    {\n  (v.insert(v.end(), ALL(vs)), ...);\n  return v;\n}\n\ntemplate <class T>\n\
    T vecget(const vc<T> &v, cauto &i, const T &dflt_negative = -INF, const T &dflt_positive\
    \ = INF)\n{\n  if (i < 0)\n    return dflt_negative;\n  if (i >= SZI(v))\n   \
    \ return dflt_positive;\n  return v[i];\n}\n#line 6 \"template/template_binsearch.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u4E8C\u5206\u63A2\
    \u7D22\uFF09\n * @docs docs/template/template_binsearch.md\n */\n\n#if __cplusplus\
    \ < 202002L\n// val <= v[i] \u3068\u306A\u308B\u6700\u5C0F\u306E i (val \u672A\
    \u6E80\u306E\u5024\u306E\u500B\u6570)\ntemplate <class T = ll, class V, class...\
    \ Args>\ninline T LB(const V &v, Args&&... args)\n{ return lower_bound(ALL(v),\
    \ forward<Args>(args)...) - v.begin(); }\n// val < v[i] \u3068\u306A\u308B\u6700\
    \u5C0F\u306E i (val \u4EE5\u4E0B\u306E\u5024\u306E\u500B\u6570)\ntemplate <class\
    \ T = ll, class V, class... Args>\ninline T UB(const V &v, Args&&... args)\n{\
    \ return upper_bound(ALL(v), forward<Args>(args)...) - v.begin(); }\n#else\n//\
    \ val <= v[i] \u3068\u306A\u308B\u6700\u5C0F\u306E i (val \u672A\u6E80\u306E\u5024\
    \u306E\u500B\u6570)\n// \u5F15\u6570: comp, proj\ntemplate <class T = ll, class\
    \ V, class... Args>\ninline T LB(const V &v, Args&&... args)\n{ return ranges::lower_bound(v,\
    \ forward<Args>(args)...) - v.begin(); }\n// val < v[i] \u3068\u306A\u308B\u6700\
    \u5C0F\u306E i (val \u4EE5\u4E0B\u306E\u5024\u306E\u500B\u6570)\n// \u5F15\u6570\
    : comp, proj\ntemplate <class T = ll, class V, class... Args>\ninline T UB(const\
    \ V &v, Args&&... args)\n{ return ranges::upper_bound(v, forward<Args>(args)...)\
    \ - v.begin(); }\n#endif\n\ntemplate <class T = ll>\npair<T, T> binsearch(cauto\
    \ &judge, cauto &init_ok, cauto &init_ng)\n{\n  T ok(init_ok), ng(init_ng);\n\
    \  assert(judge(ok));\n  assert(!judge(ng));\n  while (ok - ng != 1 && ng - ok\
    \ != 1)\n  {\n    T mid = (ok & ng) + ((ok ^ ng) >> 1);\n    (judge(mid) ? ok\
    \ : ng) = mid;\n  }\n  return make_pair(ok, ng);\n}\ntemplate <class T = ld>\n\
    T binsearch_real(cauto &judge, cauto &init_ok, cauto &init_ng, const int &iteration_count\
    \ = 100)\n{\n  T ok(init_ok), ng(init_ng);\n  assert(judge(ok));\n  assert(!judge(ng));\n\
    \  repi(_, iteration_count)\n  {\n    T mid = (ok + ng) / 2;\n    (judge(mid)\
    \ ? ok : ng) = mid;\n  }\n  return ok;\n}\ntemplate <class T = ll>\npair<T, T>\
    \ expsearch(cauto &judge, cauto &init_val, const bool &positive = true)\n{\n \
    \ T ok, ng;\n  if (judge(init_val))\n  {\n    ok = init_val, ng = init_val + (positive\
    \ ? 1 : -1);\n    for (int i = 1; judge(ng); i++)\n      ok = ng, ng = init_val\
    \ + (positive ? 1 : -1) * (T(1) << i);\n  }\n  else\n  {\n    ng = init_val, ok\
    \ = init_val + (positive ? 1 : -1);\n    for (int i = 1; !judge(ok); i++)\n  \
    \    ng = ok, ok = init_val + (positive ? 1 : -1) * (T(1) << i);\n  }\n  while\
    \ (ok - ng != 1 && ng - ok != 1)\n  {\n    T mid = (ok & ng) + ((ok ^ ng) >> 1);\n\
    \    (judge(mid) ? ok : ng) = mid;\n  }\n  return make_pair(ok, ng);\n}\n"
  code: "#pragma once\n\n#include \"template_types.hpp\"\n#include \"template_rep.hpp\"\
    \n#include \"template_vector.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\
    \u30C8\uFF08\u4E8C\u5206\u63A2\u7D22\uFF09\n * @docs docs/template/template_binsearch.md\n\
    \ */\n\n#if __cplusplus < 202002L\n// val <= v[i] \u3068\u306A\u308B\u6700\u5C0F\
    \u306E i (val \u672A\u6E80\u306E\u5024\u306E\u500B\u6570)\ntemplate <class T =\
    \ ll, class V, class... Args>\ninline T LB(const V &v, Args&&... args)\n{ return\
    \ lower_bound(ALL(v), forward<Args>(args)...) - v.begin(); }\n// val < v[i] \u3068\
    \u306A\u308B\u6700\u5C0F\u306E i (val \u4EE5\u4E0B\u306E\u5024\u306E\u500B\u6570\
    )\ntemplate <class T = ll, class V, class... Args>\ninline T UB(const V &v, Args&&...\
    \ args)\n{ return upper_bound(ALL(v), forward<Args>(args)...) - v.begin(); }\n\
    #else\n// val <= v[i] \u3068\u306A\u308B\u6700\u5C0F\u306E i (val \u672A\u6E80\
    \u306E\u5024\u306E\u500B\u6570)\n// \u5F15\u6570: comp, proj\ntemplate <class\
    \ T = ll, class V, class... Args>\ninline T LB(const V &v, Args&&... args)\n{\
    \ return ranges::lower_bound(v, forward<Args>(args)...) - v.begin(); }\n// val\
    \ < v[i] \u3068\u306A\u308B\u6700\u5C0F\u306E i (val \u4EE5\u4E0B\u306E\u5024\u306E\
    \u500B\u6570)\n// \u5F15\u6570: comp, proj\ntemplate <class T = ll, class V, class...\
    \ Args>\ninline T UB(const V &v, Args&&... args)\n{ return ranges::upper_bound(v,\
    \ forward<Args>(args)...) - v.begin(); }\n#endif\n\ntemplate <class T = ll>\n\
    pair<T, T> binsearch(cauto &judge, cauto &init_ok, cauto &init_ng)\n{\n  T ok(init_ok),\
    \ ng(init_ng);\n  assert(judge(ok));\n  assert(!judge(ng));\n  while (ok - ng\
    \ != 1 && ng - ok != 1)\n  {\n    T mid = (ok & ng) + ((ok ^ ng) >> 1);\n    (judge(mid)\
    \ ? ok : ng) = mid;\n  }\n  return make_pair(ok, ng);\n}\ntemplate <class T =\
    \ ld>\nT binsearch_real(cauto &judge, cauto &init_ok, cauto &init_ng, const int\
    \ &iteration_count = 100)\n{\n  T ok(init_ok), ng(init_ng);\n  assert(judge(ok));\n\
    \  assert(!judge(ng));\n  repi(_, iteration_count)\n  {\n    T mid = (ok + ng)\
    \ / 2;\n    (judge(mid) ? ok : ng) = mid;\n  }\n  return ok;\n}\ntemplate <class\
    \ T = ll>\npair<T, T> expsearch(cauto &judge, cauto &init_val, const bool &positive\
    \ = true)\n{\n  T ok, ng;\n  if (judge(init_val))\n  {\n    ok = init_val, ng\
    \ = init_val + (positive ? 1 : -1);\n    for (int i = 1; judge(ng); i++)\n   \
    \   ok = ng, ng = init_val + (positive ? 1 : -1) * (T(1) << i);\n  }\n  else\n\
    \  {\n    ng = init_val, ok = init_val + (positive ? 1 : -1);\n    for (int i\
    \ = 1; !judge(ok); i++)\n      ng = ok, ok = init_val + (positive ? 1 : -1) *\
    \ (T(1) << i);\n  }\n  while (ok - ng != 1 && ng - ok != 1)\n  {\n    T mid =\
    \ (ok & ng) + ((ok ^ ng) >> 1);\n    (judge(mid) ? ok : ng) = mid;\n  }\n  return\
    \ make_pair(ok, ng);\n}"
  dependsOn:
  - template/template_types.hpp
  - template/template_rep.hpp
  - template/template_vector.hpp
  - template/template_math.hpp
  isVerificationFile: false
  path: template/template_binsearch.hpp
  requiredBy: []
  timestamp: '2024-12-10 18:45:11+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/mytest/template_binsearch.test.cpp
documentation_of: template/template_binsearch.hpp
layout: document
redirect_from:
- /library/template/template_binsearch.hpp
- /library/template/template_binsearch.hpp.html
title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u4E8C\u5206\u63A2\u7D22\uFF09"
---
## テンプレート（二分探索）

### LB, UB

```cpp
(1) U=ll LB(vc<T> v, T val, ...)
(2) U=ll UB(vc<T> v, T val, ...)
```

`ranges::lower_bound` や `ranges::upper_bound` で返るイテレータの `begin` からの距離を整数で返す。

$v$ がソート済みで、後ろの引数がデフォルトの場合、

- (1)：$\mathrm{val} \leq v_i$ となる最小の $i$
  - これは $\mathrm{val}$ 未満の値の個数に一致

- (2)：$\mathrm{val} < v_i$ となる最小の $i$
  - これは $\mathrm{val}$ 以下の値の個数に一致

を返す。

後ろには C++17 以前なら `std::lower_bound`, C++20 以降なら `std::ranges::lower_bound` の引数をとれる。

##### 制約

`std::lower_bound`, `std::upper_bound` に準じる。

##### 計算量

- $O(\log \lvert v \rvert)$

### binsearch

```cpp
(1) T binsearch(F judge, T ok, T ng)
(2) T binsearch_real(F judge, T ok, T ng, int iteration_count)
```

- (1)：整数の二分探索。整数 `T` から `bool` への関数 $\mathrm{judge}$ が $\mathrm{judge}(\mathrm{ok}) = \mathrm{true}, \mathrm{judge}(\mathrm{ng}) = \mathrm{false}$ を満たすとする。このとき $\mathrm{ok}$ と $\mathrm{ng}$ の間にある整数 $\alpha, \beta$ であって $\mathrm{judge}(\alpha) = \mathrm{true}, \mathrm{judge}(\beta) = \mathrm{false}, \lvert \alpha - \beta \rvert = 1$ を満たすものが存在する（特に $\mathrm{judge}$ が単調な場合は唯一である）。そのような $\alpha$ を一つ返す。
- (2)：実数の二分探索。実数 `T` から `bool` への関数 $\mathrm{judge}$ が $\mathrm{judge}(\mathrm{ok}) = \mathrm{true}, \mathrm{judge}(\mathrm{ng}) = \mathrm{false}$ を満たすとする。$1$ 回の反復で、$\mathrm{judge}(\mathrm{ok}) = \mathrm{true}, \mathrm{judge}(\mathrm{ng}) = \mathrm{false}$ を満たしたまま $\lvert \mathrm{ok} - \mathrm{ng} \rvert$ を半分にできる。この反復を $\mathrm{iteration\_count}$ 回繰り返す。

##### 制約

- $\mathrm{judge}(\mathrm{ok}) = \mathrm{true}$
- $\mathrm{judge}(\mathrm{ng}) = \mathrm{false}$
- (1)：型 `T` は整数
- (1)：$\mathrm{ok}, \mathrm{ng}$ は型 `T` の上限・下限付近ではない
- (2)：型 `T` は実数

##### 計算量

$\mathrm{judge}(x)$ の実行にかかる時間を $T$ として

- (1)：$O(T \log \lvert \mathrm{ok} - \mathrm{ng} \rvert)$
- (2)：$O(T \cdot \mathrm{iteration\_count})$

#### expsearch

`T expsearch(F judge, T first = 0, bool positive = true)`

整数の指数探索。整数 `T` から `bool` への関数 $\mathrm{judge}$ は単調であり、$\mathrm{judge}$ の返り値が切り替わる境界（$\mathrm{judge}(\alpha) = \mathrm{true}, \mathrm{judge}(\beta) = \mathrm{false}, \lvert \alpha - \beta \rvert = 1$ を満たす唯一の整数組 $\alpha, \beta$）が

- $\mathrm{positive} = \mathrm{true}$ なら、$[\mathrm{first}, \infty)$ の範囲にある
- $\mathrm{positive} = \mathrm{false}$ なら、$(-\infty, \mathrm{first}]$ の範囲にある

とする。このとき、その $\alpha$ を返す。

最悪計算量のオーダーは二分探索と変わらない。二分探索と比べると、

- $\mathrm{ok}, \mathrm{ng}$ を詳細に見積もって指定する必要がない
- 答えが小さい場合に高速

である点が優れている。

ただし、判定関数内でオーバーフローしないように注意する必要はある。

##### 制約

- 型 `T` は整数
- $\mathrm{judge}$ は単調
- $\mathrm{positive} = \mathrm{true}$ なら、答えは $[\mathrm{first}, \infty)$ の範囲にある
- $\mathrm{positive} = \mathrm{false}$ なら、答えは $(-\infty, \mathrm{first}]$ の範囲にある
- 答えは `T` の範囲に収まる

##### 計算量

$\mathrm{judge}(x)$ の実行にかかる時間を $T$ として

- $O(T \log \lvert \mathrm{first} - \alpha \rvert)$
