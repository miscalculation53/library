---
data:
  _extendedDependsOn:
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
    path: template/template_algo.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30A2\u30EB\u30B4\u30EA\u30BA\
      \u30E0\uFF09"
  - icon: ':heavy_check_mark:'
    path: template/template_all.hpp
    title: template/template_all.hpp
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
    path: template/template_func.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u95A2\u6570\u30AA\u30D6\u30B8\
      \u30A7\u30AF\u30C8\uFF09"
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
  - icon: ':heavy_check_mark:'
    path: template/template_vector.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08vector\uFF09"
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://yukicoder.me/problems/no/888
    links:
    - https://yukicoder.me/problems/no/888
  bundledCode: "#line 1 \"verify/yukicoder/yuki888.test.cpp\"\n#define PROBLEM \"\
    https://yukicoder.me/problems/no/888\"\n\n#define SINGLE_TESTCASE\n// #define\
    \ MULTI_TESTCASE\n// #define AOJ_TESTCASE\n\n#define FAST_IO\n\n#define INF 4'000'000'000'000'000'037LL\n\
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
    #define fem(...) for (auto &__VA_ARGS__)\n#line 2 \"template/template_math.hpp\"\
    \n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n#endif\n#ifndef EPS\n\
    #define EPS 1e-11\n#endif\n\n#line 12 \"template/template_math.hpp\"\n\n/**\n\
    \ * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09\n * @docs\
    \ docs/template/template_math.md\n */\n\ninline bool chmin(auto &a, cauto &b)\
    \ { return a > b ? a = b, true : false; }\ninline bool chmax(auto &a, cauto &b)\
    \ { return a < b ? a = b, true : false; }\n\ntemplate <class T = ll>\ninline constexpr\
    \ T divfloor(cauto &a, cauto &b) { return T(a) / T(b) - (T(a) % T(b) && (T(a)\
    \ ^ T(b)) < 0); }\ntemplate <class T = ll>\ninline constexpr T divceil(cauto &a,\
    \ cauto &b) { return T(a) / T(b) + (T(a) % T(b) && (T(a) ^ T(b)) >= 0); }\ntemplate\
    \ <class T = ll>\ninline constexpr T divround(cauto &a, cauto &b) { return divfloor<T>(2\
    \ * T(a) + T(b), 2 * T(b)); }\ntemplate <class T = ll>\ninline constexpr T safemod(cauto\
    \ &a, cauto &b) { return T(a) - T(b) * divfloor<T>(a, b); }\n\ntemplate <class\
    \ T = ll>\nconstexpr T ipow(auto a, auto b)\n{\n  assert(b >= 0);\n  if (b ==\
    \ 0) return 1;\n  if (a == 0 || a == 1) return a;\n  if (a == -1) return b & 1\
    \ ? -1 : 1;\n\n  T res = 1, tmp = a;\n  while (true)\n  {\n    if (b & 1)\n  \
    \    res *= tmp;\n    b >>= 1;\n    if (b == 0)\n      break;\n    tmp *= tmp;\n\
    \  }\n  return res;\n}\ntemplate <class T = ll>\nT mul_limited(cauto &a, cauto\
    \ &b, cauto &m = INF)\n{\n  assert(a >= 0 && b >= 0 && m >= 0);\n  if (b == 0)\n\
    \    return 0;\n  return T(a) > T(m) / T(b) ? T(m) : T(a) * T(b);\n}\ntemplate\
    \ <class T = ll>\nT pow_limited(cauto &a, cauto &b, cauto &m = INF)\n{\n  assert(a\
    \ >= 0 && b >= 0 && m >= 0);\n  if (a <= 1 || b == 0)\n    return min(ipow<T>(a,\
    \ b), T(m));\n  \n  T res = 1;\n  repi(_, b)\n  {\n    if (res > T(m) / T(a))\n\
    \      return T(m);\n    res *= T(a);\n  }\n  return res;\n}\n\ntemplate <class\
    \ T = ll>\nconstexpr T iroot(cauto &a, cauto &k)\n{\n  assert(a >= 0 && k >= 1);\n\
    \  if (a <= 1 || k == 1)\n    return a;\n\n  auto isok = [&](const T &x) -> bool\n\
    \  {\n    if (x == 0)\n      return true;\n    T tmp = 1;\n    repi(_, k)\n  \
    \  {\n      if (tmp > T(a) / x)\n        return false;\n      tmp *= x;\n    }\n\
    \    return tmp <= T(a);\n  };\n\n  T ok = 0, ng = 1;\n  while (isok(ng))\n  \
    \  ok = ng, ng <<= 1;\n  while (ng - ok > 1)\n  {\n    T mid = ((ng - ok) >> 1)\
    \ + ok;\n    if (isok(mid))\n      ok = mid;\n    else\n      ng = mid;\n  }\n\
    \  return ok;\n}\n\n// https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html\n\
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
    }\n#line 2 \"template/template_vector.hpp\"\n\n#line 6 \"template/template_vector.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08vector\uFF09\n *\
    \ @docs docs/template/template_vector.md\n */\n\n#define ALL(a) (a).begin(), (a).end()\n\
    template <class T = ll>\ninline T SZ(cauto &x) { return x.size(); }\n\ntemplate\
    \ <class F>\nauto gen_vec(const int &n, const F &f)\n{\n  vc<decltype(f(0))> res(n);\n\
    \  repi(i, n) res[i] = f(i);\n  return res;\n}\n\n// https://qiita.com/Chippppp/items/13150f5e0ea99f444d97#%E5%A4%9A%E6%AC%A1%E5%85%83vector%E7%94%9F%E6%88%90%E9%96%A2%E6%95%B0\n\
    template <class T, size_t d, size_t i = 0>\nauto dvec(cauto (&sz)[d], const T\
    \ &init)\n{\n  if constexpr (i < d)\n    return vc(sz[i], dvec<T, d, i + 1>(sz,\
    \ init));\n  else\n    return init;\n}\n\ntemplate <class T = ll>\nT ctol(const\
    \ char &c, const string &s)\n{\n  repi(i, SZ<int>(s)) if (s[i] == c) return i;\n\
    \  return -1;\n}\ntemplate <class T = ll>\nvc<T> stov(const string &s, const char\
    \ &first)\n{\n  return gen_vec(SZ<int>(s), [&](int i) -> T\n                 {\
    \ return s[i] - first; });\n}\ntemplate <class T = ll>\nvc<T> stov(const string\
    \ &s, const string &t)\n{\n  return gen_vec(SZ<int>(s), [&](int i) -> T\n    \
    \             { return ctol(s[i], t); });\n}\n\ntemplate <class T>\nvc<T> concat(const\
    \ vvc<T> &vs)\n{\n  vc<T> res;\n  for (cauto &v : vs)\n    res.insert(res.end(),\
    \ ALL(v));\n  return res;\n}\ntemplate <class T>\nvc<T> concat(const vc<T> &v)\
    \ { return v; }\ntemplate <class T, class... Ts>\nvc<T> concat(vc<T> v, const\
    \ vc<Ts> &...vs)\n{\n  (v.insert(v.end(), ALL(vs)), ...);\n  return v;\n}\n\n\
    template <class T>\nT vecget(const vc<T> &v, cauto &i, const T &dflt_negative\
    \ = -INF, const T &dflt_positive = INF)\n{\n  if (i < 0)\n    return dflt_negative;\n\
    \  if (i >= SZ<int>(v))\n    return dflt_positive;\n  return v[i];\n}\n#line 2\
    \ \"template/template_func.hpp\"\n\n#line 5 \"template/template_func.hpp\"\n\n\
    /**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u95A2\u6570\u30AA\u30D6\
    \u30B8\u30A7\u30AF\u30C8\uFF09\n * @docs docs/template/template_func.md\n */\n\
    \n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n#endif\n\ntemplate <class\
    \ T = ll>\nstruct max_op\n{\n  T operator()(const T &a, const T &b) const { return\
    \ max(a, b); }\n};\ntemplate <class T = ll>\nstruct min_op\n{\n  T operator()(const\
    \ T &a, const T &b) const { return min(a, b); }\n};\n\ntemplate <class T, const\
    \ T val>\nstruct const_fn\n{\n  T operator()() const { return val; }\n};\nusing\
    \ max_e = const_fn<ll, -INF>;\nusing min_e = const_fn<ll, INF>;\nusing zero_fn\
    \ = const_fn<ll, 0LL>;\n#line 2 \"template/template_algo.hpp\"\n\n#ifndef INF\n\
    #define INF 4'000'000'000'000'000'037LL\n#endif\n\n#line 11 \"template/template_algo.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30A2\u30EB\u30B4\
    \u30EA\u30BA\u30E0\uFF09\n * @docs docs/template/template_algo.md\n */\n\ntemplate\
    \ <class T, class U = ll>\nU SUM(const vc<T> &v) { return accumulate(ALL(v), U(0));\
    \ }\ntemplate <class T>\nT MAX(const vc<T> &v) { return *max_element(ALL(v));\
    \ }\ntemplate <class T>\nT MIN(const vc<T> &v) { return *min_element(ALL(v));\
    \ }\ntemplate <class T, class U = ll>\nU ARGMAX(const vc<T> &v) { return max_element(ALL(v))\
    \ - v.begin(); }\ntemplate <class T, class U = ll>\nU ARGMIN(const vc<T> &v) {\
    \ return min_element(ALL(v)) - v.begin(); }\n\ntemplate<class T, class U = ll>\n\
    U mex(const vector<T> &a)\n{\n  int n = a.size();\n  vector<bool> exists(n, false);\n\
    \  repi(i, n) if (0 <= a[i] && a[i] < n) exists[a[i]] = true;\n  repi(x, n) if\
    \ (!exists[x]) return x;\n  return n;\n}\n\ntemplate <class T = ll>\nvc<T> permid(const\
    \ int &n, const int &base_index = 0)\n{\n  vc<T> p(n);\n  repi(i, n) p[i] = i\
    \ + base_index;\n  return p;\n}\ntemplate <class T>\nvc<T> perminv(const vc<T>\
    \ &p)\n{\n  if (p.empty())\n    return {};\n  const int n = p.size();\n  vc<T>\
    \ q(MAX(p) + 1);\n  repi(i, n) if (p[i] >= 0) q[p[i]] = i;\n  return q;\n}\n//\
    \ a[p[i]] for all i\ntemplate <class T, class U>\nvc<T> permuted(const vc<T> &a,\
    \ const vc<U> &p)\n{\n  const int n = p.size();\n  vc<T> res(n);\n  repi(i, n)\n\
    \  {\n    assert(0 <= p[i] && p[i] < U(a.size()));\n    res[i] = a[p[i]];\n  }\n\
    \  return res;\n}\n\ntemplate <class V>\nV reversed(const V &v) { return {v.rbegin(),\
    \ v.rend()}; }\n\n#if __cplusplus < 202002L\ntemplate <class V, class... Args>\n\
    V sorted(V v, Args&&... args)\n{\n  sort(ALL(v), forward<Args>(args)...);\n  return\
    \ v;\n}\n#else\ntemplate <class V, class... Args>\nV sorted(V v, Args&&... args)\n\
    {\n  ranges::sort(v, forward<Args>(args)...);\n  return v;\n}\n#endif\n\ntemplate\
    \ <class V>\nvoid unique(V &v) { v.erase(unique(ALL(v)), v.end()); }\ntemplate\
    \ <class V>\nV uniqued(V v) { unique(v); return v; }\n\ntemplate <class V>\nvoid\
    \ sortunique(V &v)\n{\n  sort(ALL(v));\n  unique(v);\n}\ntemplate <class V>\n\
    V sortuniqued(V v) { sortunique(v); return v; }\n\n// 01234 -> 12340\ntemplate\
    \ <class V, class U>\nvoid rotate(V &v, U k)\n{ \n  const U n = v.size();\n  k\
    \ = (k % n + n) % n;\n  rotate(v.begin(), v.begin() + k, v.end());\n}\n// 01234\
    \ -> 12340\ntemplate <class V, class U>\nV rotated(V v, U k) { rotate(v, k); return\
    \ v; }\n\ntemplate <class T>\nvvc<T> top(const vvc<T> &a)\n{\n  if (a.empty())\n\
    \    return {};\n  const int n = a.size(), m = a[0].size();\n  vvc<T> b(m, vc<T>(n));\n\
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
    \ vc<T> &v, const F &op = plus<>(), const T &e = 0)\n{\n  const int n = v.size();\n\
    \  vc<T> res(n + 1, e);\n  repi(i, n) res[i + 1] = op(res[i], v[i]);\n  return\
    \ res;\n}\ntemplate <class T, class F = decltype(plus<>())>\nvc<T> cumr(const\
    \ vc<T> &v, const F &op = plus<>(), const T &e = 0)\n{ return reversed(cuml<T,\
    \ F>(reversed(v), op, e)); }\ntemplate <class T>\nvc<T> cumlmax(const vc<T> &v)\
    \ { return cuml(v, max_op<T>(), max_e()()); }\ntemplate <class T>\nvc<T> cumrmax(const\
    \ vc<T> &v) { return cumr(v, max_op<T>(), max_e()()); }\ntemplate <class T>\n\
    vc<T> cumlmin(const vc<T> &v) { return cuml(v, min_op<T>(), min_e()()); }\ntemplate\
    \ <class T>\nvc<T> cumrmin(const vc<T> &v) { return cumr(v, min_op<T>(), min_e()());\
    \ }\n\ntemplate <class T>\nvc<T> adjd(const vc<T> &v)\n{\n  int n = v.size();\n\
    \  vc<T> res(n + 1);\n  res[0] = v[0];\n  repi(i, 1, n) res[i] = v[i] - v[i -\
    \ 1];\n  res[n] = -v[n - 1];\n  return res;\n}\n\ntemplate <class T = ll>\nstruct\
    \ direct_product\n{\nprivate:\n  vc<T> a;\npublic:\n  direct_product(const vc<T>\
    \ &a) : a(a)\n  {\n    assert(!a.empty());\n    fec(ai : a) assert(ai >= 1);\n\
    \  }\n  struct Iterator\n  {\n  private:\n    vc<T> b;\n    const direct_product\
    \ &prod;\n\n  public:\n    Iterator(const vc<T> &b, const direct_product &prod)\
    \ : b(b), prod(prod) {}\n    vc<T> operator*() const { return b; }\n    Iterator&\
    \ operator++()\n    {\n      b.back()++;\n      repi(i, SZ<int>(prod.a) - 1, 0,\
    \ -1)\n      {\n        if (b[i] == prod.a[i])\n        {\n          b[i] = 0;\n\
    \          b[i - 1]++;\n        }\n        else\n          break;\n      }\n \
    \     return *this;\n    }\n    bool operator!=(const Iterator &other) const {\
    \ return b != other.b; }\n  };\n  Iterator begin() const { return Iterator(vc<T>(a.size(),\
    \ 0), *this); }\n  Iterator end() const\n  {\n    vc<T> c(a.size(), 0);\n    c[0]\
    \ = a[0];\n    return Iterator(c, *this);\n  }\n};\n\nconst vpll DRULgrid = {{1,\
    \ 0}, {0, 1}, {-1, 0}, {0, -1}};\nconst vpll DRULplane = {{0, -1}, {1, 0}, {0,\
    \ 1}, {-1, 0}};\n#line 2 \"template/template_binsearch.hpp\"\n\n#line 6 \"template/template_binsearch.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u4E8C\u5206\u63A2\
    \u7D22\uFF09\n * @docs docs/template/template_binsearch.md\n */\n\n#if __cplusplus\
    \ < 202002L\n// val <= v[i] \u3068\u306A\u308B\u6700\u5C0F\u306E i (val \u672A\
    \u6E80\u306E\u5024\u306E\u500B\u6570)\n// \u5F15\u6570: comp\ntemplate <class\
    \ T = ll, class V, class... Args>\ninline T LB(const V &v, Args&&... args)\n{\
    \ return lower_bound(ALL(v), forward<Args>(args)...) - v.begin(); }\n// val <\
    \ v[i] \u3068\u306A\u308B\u6700\u5C0F\u306E i (val \u4EE5\u4E0B\u306E\u5024\u306E\
    \u500B\u6570)\n// \u5F15\u6570: comp\ntemplate <class T = ll, class V, class...\
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
    \ntemplate <class T = ll>\npair<T, T> binsearch(cauto &judge, cauto &init_ok,\
    \ cauto &init_ng)\n{\n  T ok(init_ok), ng(init_ng);\n  assert(judge(ok));\n  assert(!judge(ng));\n\
    \  while (ok - ng != 1 && ng - ok != 1)\n  {\n    T mid = (ok & ng) + ((ok ^ ng)\
    \ >> 1);\n    (judge(mid) ? ok : ng) = mid;\n  }\n  return make_pair(ok, ng);\n\
    }\ntemplate <class T = ld>\nT binsearch_real(cauto &judge, cauto &init_ok, cauto\
    \ &init_ng, const int &iteration_count = 100)\n{\n  T ok(init_ok), ng(init_ng);\n\
    \  assert(judge(ok));\n  assert(!judge(ng));\n  repi(_, iteration_count)\n  {\n\
    \    T mid = (ok + ng) / 2;\n    (judge(mid) ? ok : ng) = mid;\n  }\n  return\
    \ ok;\n}\ntemplate <class T = ll>\npair<T, T> expsearch(cauto &judge, cauto &init_val,\
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
    \ */\n\ninline constexpr ull pow2(auto k) { return 1ULL << k; }\ninline constexpr\
    \ ull MASK(auto k) { return (1ULL << k) - 1ULL; }\n\n#if __cplusplus < 202002L\n\
    // x == 0 \u306A\u3089\u3070 0\u3001\u305D\u3046\u3067\u306A\u3051\u308C\u3070\
    \ 1 + floor(log2(x))\n// 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, ... \ninline constexpr\
    \ ull bit_width(ull x) { return x == 0 ? 0 : 64 - __builtin_clzll(x); }\n// 0,\
    \ 1, 2, 2, 4, 4, 4, 4, 8, 8, ...\ninline constexpr ull bit_floor(ull x) { return\
    \ x == 0 ? 0ULL : 1ULL << (bit_width(x) - 1); }\n// 1, 1, 2, 4, 4, 8, 8, 8, 8,\
    \ 16, ...\ninline constexpr ull bit_ceil(ull x) { return x == 0 ? 1ULL : 1ULL\
    \ << bit_width(x - 1); }\ninline constexpr ull countr_zero(ull x) { assert(x !=\
    \ 0); return __builtin_ctzll(x); }\ninline constexpr ull popcount(ull x) { return\
    \ __builtin_popcountll(x); }\ninline constexpr bool has_single_bit(ull x) { return\
    \ popcount(x) == 1; }\n#else\n// 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, ... \ninline constexpr\
    \ ll bit_width(ll x) { return bit_width((ull)x); }\n// 0, 1, 2, 2, 4, 4, 4, 4,\
    \ 8, 8, ...\ninline constexpr ll bit_floor(ll x) { return bit_floor((ull)x); }\n\
    // 1, 1, 2, 4, 4, 8, 8, 8, 8, 16, ...\ninline constexpr ll bit_ceil(ll x) { return\
    \ bit_ceil((ull)x); }\ninline constexpr ll countr_zero(ll x) { assert(x != 0);\
    \ return countr_zero((ull)x); }\ninline constexpr ll popcount(ll x) { return popcount((ull)x);\
    \ }\ninline constexpr bool has_single_bit(ll x) { return has_single_bit((ull)x);\
    \ }\n#endif\n\ninline constexpr ull lsb_pos(ull x) { assert(x != 0); return countr_zero(x);\
    \ }\ninline constexpr ull msb_pos(ull x) { assert(x != 0); return bit_width(x)\
    \ - 1; }\ninline constexpr ull lsb_mask(ull x) { assert(x != 0); return x & -x;\
    \ }\ninline constexpr ull msb_mask(ull x) { assert(x != 0); return bit_floor(x);\
    \ }\n\ninline constexpr bool btest(ull x, uint k) { return (x >> k) & 1; }\ntemplate\
    \ <class T>\ninline void bset(T &x, uint k, bool b = 1) { b ? x |= (1ULL << k)\
    \ : x &= ~(1ULL << k); }\ntemplate <class T>\ninline void bflip(T &x, uint k)\
    \ { x ^= (1ULL << k); }\ninline constexpr bool bsubset(ull x, ull y) { return\
    \ (x & y) == x; }\ninline constexpr bool bsupset(ull x, ull y) { return (x & y)\
    \ == y; }\ninline constexpr ull bsetminus(ull x, ull y) { return x & ~y; }\n\n\
    template <class T>\nstruct bsubsets\n{\nprivate:\n  T x;\npublic:\n  bsubsets(T\
    \ x) : x(x) {}\n  struct Iterator\n  {\n  private:\n    T y;\n    bool is_end;\n\
    \    const bsubsets &bs;\n  public:\n    Iterator(T y, bool is_end, const bsubsets\
    \ &bs) : y(y), is_end(is_end), bs(bs) {}\n    T operator*() const { return y;\
    \ }\n    Iterator& operator++()\n    {\n      if (y == 0)\n        is_end = true;\n\
    \      y = (y - 1) & bs.x;\n      return *this;\n    }\n    bool operator!=(const\
    \ Iterator &other) const { return y != other.y || is_end != other.is_end; }\n\
    \  };\n  Iterator begin() const { return Iterator(x, false, *this); }\n  Iterator\
    \ end() const { return Iterator(x, true, *this); }\n};\ntemplate <class T>\nstruct\
    \ bsupsets\n{\nprivate:\n  int n;\n  T x;\npublic:\n  bsupsets(int n, T x) : n(n),\
    \ x(x) {}\n  struct Iterator\n  {\n  private:\n    T y;\n    const bsupsets &bs;\n\
    \  public:\n    Iterator(T y, const bsupsets &bs) : y(y), bs(bs) {}\n    T operator*()\
    \ const { return y; }\n    Iterator& operator++()\n    {\n      y = (y + 1) |\
    \ bs.x;\n      return *this;\n    }\n    bool operator!=(const Iterator &other)\
    \ const { return y != other.y; }\n  };\n  Iterator begin() const { return Iterator(x,\
    \ *this); }\n  Iterator end() const { return Iterator((T(1) << n) | x, *this);\
    \ }\n};\n#line 2 \"template/template_inout.hpp\"\n\n#line 5 \"template/template_inout.hpp\"\
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
    #endif\n#line 13 \"verify/yukicoder/yuki888.test.cpp\"\n\n#line 2 \"math/prime/factorize.hpp\"\
    \n\n#line 2 \"math/modint/modint.hpp\"\n\n#line 2 \"math/modint/modint_internal.hpp\"\
    \n\n#line 4 \"math/modint/modint_internal.hpp\"\n\nnamespace internal\n{\n\nconstexpr\
    \ ll powmod32_constexpr(ll x, ll n, int m)\n{\n  if (m == 1)\n    return 0;\n\
    \  uint _m = (uint)m;\n  ull r = 1;\n  ull y = safemod(x, m);\n  while (n)\n \
    \ {\n    if (n & 1)\n      r = (r * y) % _m;\n    y = (y * y) % _m;\n    n >>=\
    \ 1;\n  }\n  return r;\n}\nconstexpr ll powmod64_constexpr(ll x, ll n, ll m)\n\
    {\n  if (m == 1)\n    return 0;\n  ull _m = (ull)m;\n  ull r = 1;\n  ull y = safemod(x,\
    \ m);\n  while (n)\n  {\n    u128 y128(y);\n    if (n & 1)\n      r = (y128 *\
    \ r) % _m;\n    y = (y128 * y) % _m;\n    n >>= 1;\n  }\n  return r;\n}\n\nconstexpr\
    \ bool isprime32_constexpr(int n)\n{\n  if (n <= 1)\n    return false;\n  if (n\
    \ == 2 || n == 7 || n == 61)\n    return true;\n  if (n % 2 == 0)\n    return\
    \ false;\n  ll d = n - 1;\n  while (d % 2 == 0)\n    d /= 2;\n  constexpr ll bases[3]\
    \ = {2, 7, 61};\n  for (ll a : bases)\n  {\n    ll t = d;\n    ll y = powmod32_constexpr(a,\
    \ t, n);\n    while (t != n - 1 && y != 1 && y != n - 1)\n    {\n      y = y *\
    \ y % n;\n      t <<= 1;\n    }\n    if (y != n - 1 && t % 2 == 0)\n      return\
    \ false;\n  }\n  return true;\n}\nconstexpr bool isprime64_constexpr(ll n)\n{\n\
    \  if (n <= INT_MAX)\n    return isprime32_constexpr(n);\n  if (n % 2 == 0)\n\
    \    return false;\n  ll d = n - 1;\n  while (d % 2 == 0)\n    d /= 2;\n  constexpr\
    \ ll bases[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};\n  for (ll\
    \ a : bases)\n  {\n    ll t = d;\n    ll y = powmod64_constexpr(a, t, n);\n  \
    \  while (t != n - 1 && y != 1 && y != n - 1)\n    {\n      y = (u128(y) * y)\
    \ % n;\n      t <<= 1;\n    }\n    if (y != n - 1 && t % 2 == 0)\n      return\
    \ false;\n  }\n  return true;\n}\ntemplate <int n>\nconstexpr bool isprime32 =\
    \ isprime32_constexpr(n);\ntemplate <ll n>\nconstexpr bool isprime64 = isprime64_constexpr(n);\n\
    \nstruct barrett32\n{\n  uint m;\n  ull im;\n\n  explicit barrett32(uint m) :\
    \ m(m), im((ull)(-1) / m + 1) {}\n  uint umod() const { return m; }\n  uint mul(uint\
    \ a, uint b) const\n  {\n    ull z = a;\n    z *= b;\n    ull x = (ull)((u128(z)*im)\
    \ >> 64);\n    ull y = x * m;\n    return (uint)(z - y + (z < y ? m : 0));\n \
    \ }\n};\n\ninline constexpr ull inv64(ull a)\n{\n  ull x = a;\n  while (a * x\
    \ != 1) x *= 2 - a * x;\n  return x;\n}\n\nstruct montgomery64odd\n{\n  ull m,\
    \ im, sq;\n  explicit montgomery64odd(ull m) : m(m), im(inv64(m)), sq(-u128(m)\
    \ % m) {}\n  ull umod() const { return m; }\n  ull reduce(u128 x) const\n  {\n\
    \    auto t = (x + u128(m) * (-im * ull(x))) >> 64;\n    if (t >= m)\n      t\
    \ -= m;\n    return (ull)t;\n  }\n  ull inv_reduce(i128 v) const\n  { return reduce(u128(v\
    \ % m + m) * sq); }\n};\n\n// https://www.mathenachia.blog/even-mod-montgomery-impl/\n\
    struct montgomery64\n{\n  ull m, mx, imx, d, sq;\n  uint b;\n\n  explicit montgomery64(ull\
    \ m) : m(m), sq(-u128(m) % m)\n  {\n    b = countr_zero(m), mx = m >> b;  // m\
    \ == 2^b * mx, mx is odd\n    imx = inv64(mx);\n    d = powmod64_constexpr((mx\
    \ + 1) / 2, b, mx);  // 2^{-b} mod mx\n  }\n  ull umod() const { return m; }\n\
    \  ull reduce(u128 x) const\n  {\n    ull p = x & MASK(b);  // x mod 2^b\n   \
    \ x = (x >> b) + p * d;\n    ull y = p << (64 - b);\n    auto t = (x + u128(mx)\
    \ * (imx * (y - ull(x)))) >> (64 - b);\n    if (t >= m)\n    {\n      t -= m;\n\
    \      if (t >= m)\n        t -= m;\n    }\n    return (ull)t;\n  }\n  ull inv_reduce(i128\
    \ v) const\n  { return reduce(u128(v % m + m) * sq); }\n};\n\n}\n#line 2 \"math/extgcd.hpp\"\
    \n\n#line 4 \"math/extgcd.hpp\"\n\n/**\n * @brief \u62E1\u5F35\u30E6\u30FC\u30AF\
    \u30EA\u30C3\u30C9\u4E92\u9664\u6CD5 (extgcd)\n * @docs docs/math/extgcd.md\n\
    \ */\n\n// g == gcd(x, y) >= 0, ax + by == g \u3092\u6E80\u305F\u3059 (g, x, y)\n\
    // max(|x|, |y|) <= max(|a|, |b|)\ntemplate <class T = ll>\nconstexpr tuple<T,\
    \ T, T> extgcd(const T &a, const T &b)\n{\n  if (a == 0 && b == 0)\n    return\
    \ {0, 0, 0};\n  \n  // a*x1 + b*y1 == z1  ...(1)\n  // a*x2 + b*y2 == z2  ...(2)\n\
    \  T x1 = 1, y1 = 0, z1 = a;\n  T x2 = 0, y2 = 1, z2 = b;\n  while (z2 != 0)\n\
    \  {\n    // (1)' = (2)\n    // (2)' = (1) - q*(2)\n    T q = z1 / z2;\n    tie(x1,\
    \ x2) = make_pair(x2, x1 - q * x2);\n    tie(y1, y2) = make_pair(y2, y1 - q *\
    \ y2);\n    tie(z1, z2) = make_pair(z2, z1 - q * z2);\n  }\n  if (z1 < 0)\n  \
    \  x1 = -x1, y1 = -y1, z1 = -z1;\n  return {z1, x1, y1};\n}\n#line 6 \"math/modint/modint.hpp\"\
    \n\n/**\n * @brief modint (32 bit)\n * @docs docs/math/modint/modint.md\n */\n\
    \ntemplate <int m>\nstruct static_modint\n{\n  using mint = static_modint;\nprivate:\n\
    \  uint _v;\n  static constexpr uint umod() { return m; }\n  static constexpr\
    \ bool prime = internal::isprime32<m>;\n\npublic:\n  static constexpr int mod()\
    \ { return m; }\n  static mint raw(int v)\n  {\n    mint x;\n    x._v = v;\n \
    \   return x;\n  }\n\n  static_modint() : _v(0) {}\n  template <class T>\n  static_modint(T\
    \ v)\n  {\n    if constexpr (is_signed_v<T>)\n    {\n      ll x = (ll)(v % (ll)(umod()));\n\
    \      if (x < 0)\n        x += umod();\n      _v = (uint)x;\n    }\n    else\
    \ if constexpr (is_unsigned_v<T>)\n    {\n      _v = (uint)(v % umod());\n   \
    \ }\n    else\n    {\n      static_assert(is_signed_v<T> || is_unsigned_v<T>,\
    \ \"Unsupported Type\");\n    }\n  }\n\n  int val() const { return (int)_v; }\n\
    \n  mint& operator++()\n  {\n    _v++;\n    if (_v == umod())\n      _v = 0;\n\
    \    return *this;\n  }\n  mint& operator--()\n  {\n    if (_v == 0)\n      _v\
    \ = umod();\n    _v--;\n    return *this;\n  }\n  mint operator++(int)\n  {\n\
    \    mint res = *this;\n    ++*this;\n    return res;\n  }\n  mint operator--(int)\n\
    \  {\n    mint res = *this;\n    --*this;\n    return res;\n  }\n\n  mint& operator+=(const\
    \ mint& rhs)\n  {\n    _v += rhs._v;\n    if (_v >= umod())\n      _v -= umod();\n\
    \    return *this;\n  }\n  mint& operator-=(const mint &rhs)\n  {\n    _v -= rhs._v;\n\
    \    if (_v >= umod())\n      _v += umod();\n    return *this;\n  }\n  mint& operator*=(const\
    \ mint &rhs)\n  {\n    ull z = _v;\n    z *= rhs._v;\n    _v = (uint)(z % umod());\n\
    \    return *this;\n  }\n  mint& operator/=(const mint &rhs) { return *this =\
    \ *this * rhs.inv(); }\n\n  mint operator+() const { return *this; }\n  mint operator-()\
    \ const { return mint() - *this; }\n\n  mint pow(ll n) const\n  {\n    assert(n\
    \ >= 0);\n    mint x = *this, r = 1;\n    while (n)\n    {\n      if (n & 1)\n\
    \        r *= x;\n      x *= x;\n      n >>= 1;\n    }\n    return r;\n  }\n \
    \ mint inv() const\n  {\n    if (prime)\n    {\n      assert(_v != 0);\n     \
    \ return pow(umod() - 2);\n    }\n    else\n    {\n      auto [g, x, y] = extgcd<int>(_v,\
    \ m);\n      assert(g == 1);\n      return x;\n    }\n  }\n\n  friend mint operator+(const\
    \ mint &lhs, const mint &rhs)\n  { return mint(lhs) += rhs; }\n  friend mint operator-(const\
    \ mint &lhs, const mint &rhs)\n  { return mint(lhs) -= rhs; }\n  friend mint operator*(const\
    \ mint &lhs, const mint &rhs)\n  { return mint(lhs) *= rhs; }\n  friend mint operator/(const\
    \ mint &lhs, const mint &rhs)\n  { return mint(lhs) /= rhs; }\n  friend bool operator==(const\
    \ mint &lhs, const mint &rhs)\n  { return lhs._v == rhs._v; }\n  friend bool operator!=(const\
    \ mint &lhs, const mint &rhs)\n  { return lhs._v != rhs._v; }\n\n  friend istream\
    \ &operator>>(istream &is, mint &x)\n  {\n    ll a;\n    is >> a;\n    x = a;\n\
    \    return is;\n  }\n  friend ostream &operator<<(ostream &os, const mint &x)\n\
    \  {\n    os << x.val();\n    return os;\n  }\n};\n\ntemplate <int id>\nstruct\
    \ dynamic_modint\n{\n  using mint = dynamic_modint;\nprivate:\n  uint _v;\n  static\
    \ internal::barrett32 bt;\n  static uint umod() { return bt.umod(); }\n\npublic:\n\
    \  static int mod() { return (int)(bt.umod()); }\n  static void set_mod(int m)\n\
    \  {\n    assert(m >= 1);\n    bt = internal::barrett32(m);\n  }\n  static mint\
    \ raw(int v)\n  {\n    mint x;\n    x._v = v;\n    return x;\n  }\n\n  dynamic_modint()\
    \ : _v(0) {}\n  template <class T>\n  dynamic_modint(T v)\n  {\n    if constexpr\
    \ (is_signed_v<T>)\n    {\n      ll x = (ll)(v % (ll)(umod()));\n      if (x <\
    \ 0)\n        x += umod();\n      _v = (uint)x;\n    }\n    else if constexpr\
    \ (is_unsigned_v<T>)\n    {\n      _v = (uint)(v % umod());\n    }\n    else\n\
    \    {\n      static_assert(is_signed_v<T> || is_unsigned_v<T>, \"Unsupported\
    \ Type\");\n    }\n  }\n\n  int val() const { return (int)_v; }\n\n  mint& operator++()\n\
    \  {\n    _v++;\n    if (_v == umod())\n      _v = 0;\n    return *this;\n  }\n\
    \  mint& operator--()\n  {\n    if (_v == 0)\n      _v = umod();\n    _v--;\n\
    \    return *this;\n  }\n  mint operator++(int)\n  {\n    mint res = *this;\n\
    \    ++*this;\n    return res;\n  }\n  mint operator--(int)\n  {\n    mint res\
    \ = *this;\n    --*this;\n    return res;\n  }\n\n  mint& operator+=(const mint&\
    \ rhs)\n  {\n    _v += rhs._v;\n    if (_v >= umod())\n      _v -= umod();\n \
    \   return *this;\n  }\n  mint& operator-=(const mint &rhs)\n  {\n    _v -= rhs._v;\n\
    \    if (_v >= umod())\n      _v += umod();\n    return *this;\n  }\n  mint& operator*=(const\
    \ mint &rhs)\n  {\n    _v = bt.mul(_v, rhs._v);\n    return *this;\n  }\n  mint&\
    \ operator/=(const mint &rhs) { return *this = *this * rhs.inv(); }\n\n  mint\
    \ operator+() const { return *this; }\n  mint operator-() const { return mint()\
    \ - *this; }\n\n  mint pow(ll n) const\n  {\n    assert(n >= 0);\n    mint x =\
    \ *this, r = 1;\n    while (n)\n    {\n      if (n & 1)\n        r *= x;\n   \
    \   x *= x;\n      n >>= 1;\n    }\n    return r;\n  }\n  mint inv() const\n \
    \ {\n    auto [g, x, y] = extgcd<int>(_v, mod());\n    assert(g == 1);\n    return\
    \ x;\n  }\n\n  friend mint operator+(const mint &lhs, const mint &rhs)\n  { return\
    \ mint(lhs) += rhs; }\n  friend mint operator-(const mint &lhs, const mint &rhs)\n\
    \  { return mint(lhs) -= rhs; }\n  friend mint operator*(const mint &lhs, const\
    \ mint &rhs)\n  { return mint(lhs) *= rhs; }\n  friend mint operator/(const mint\
    \ &lhs, const mint &rhs)\n  { return mint(lhs) /= rhs; }\n  friend bool operator==(const\
    \ mint &lhs, const mint &rhs)\n  { return lhs._v == rhs._v; }\n  friend bool operator!=(const\
    \ mint &lhs, const mint &rhs)\n  { return lhs._v != rhs._v; }\n\n  friend istream\
    \ &operator>>(istream &is, mint &x)\n  {\n    ll a;\n    is >> a;\n    x = a;\n\
    \    return is;\n  }\n  friend ostream &operator<<(ostream &os, const mint &x)\n\
    \  {\n    os << x.val();\n    return os;\n  }\n};\ntemplate <int id>\ninternal::barrett32\
    \ dynamic_modint<id>::bt(998244353);\n\nusing modint998244353 = static_modint<998244353>;\n\
    using modint1000000007 = static_modint<1000000007>;\nusing modint = dynamic_modint<-1>;\n\
    #line 2 \"math/modint/modint64.hpp\"\n\n#line 6 \"math/modint/modint64.hpp\"\n\
    \n/**\n * @brief modint (64 bit)\n * @docs docs/math/modint/modint64.md\n */\n\
    \ntemplate <ll m>\nstruct static_modint64\n{\n  using mint = static_modint64;\n\
    private:\n  ull _v;\n  static constexpr ull umod() { return m; }\n  static constexpr\
    \ bool prime = internal::isprime64<m>;\n\npublic:\n  static constexpr ll mod()\
    \ { return m; }\n  static mint raw(ll v)\n  {\n    mint x;\n    x._v = v;\n  \
    \  return x;\n  }\n\n  static_modint64() : _v(0) {}\n  template <class T>\n  static_modint64(T\
    \ v)\n  {\n    if constexpr (is_unsigned_v<T>)\n    {\n      _v = (ull)(v % umod());\n\
    \    }\n    else\n    {\n      ll x = (ll)(v % (ll)(umod()));\n      if (x < 0)\n\
    \        x += umod();\n      _v = (ull)x;\n    }\n  }\n\n  ll val() const { return\
    \ (ll)_v; }\n\n  mint& operator++()\n  {\n    _v++;\n    if (_v == umod())\n \
    \     _v = 0;\n    return *this;\n  }\n  mint& operator--()\n  {\n    if (_v ==\
    \ 0)\n      _v = umod();\n    _v--;\n    return *this;\n  }\n  mint operator++(int)\n\
    \  {\n    mint res = *this;\n    ++*this;\n    return res;\n  }\n  mint operator--(int)\n\
    \  {\n    mint res = *this;\n    --*this;\n    return res;\n  }\n\n  mint& operator+=(const\
    \ mint& rhs)\n  {\n    _v += rhs._v;\n    if (_v >= umod())\n      _v -= umod();\n\
    \    return *this;\n  }\n  mint& operator-=(const mint &rhs)\n  {\n    _v -= rhs._v;\n\
    \    if (_v >= umod())\n      _v += umod();\n    return *this;\n  }\n  mint& operator*=(const\
    \ mint &rhs)\n  {\n    u128 z = _v;\n    z *= rhs._v;\n    _v = (ull)(z % umod());\n\
    \    return *this;\n  }\n  mint& operator/=(const mint &rhs) { return *this =\
    \ *this * rhs.inv(); }\n\n  mint operator+() const { return *this; }\n  mint operator-()\
    \ const { return mint() - *this; }\n\n  mint pow(ll n) const\n  {\n    assert(n\
    \ >= 0);\n    mint x = *this, r = 1;\n    while (n)\n    {\n      if (n & 1)\n\
    \        r *= x;\n      x *= x;\n      n >>= 1;\n    }\n    return r;\n  }\n \
    \ mint inv() const\n  {\n    if (prime)\n    {\n      assert(_v != 0);\n     \
    \ return pow(umod() - 2);\n    }\n    else\n    {\n      auto [g, x, y] = extgcd<ll>(_v,\
    \ m);\n      assert(g == 1);\n      return x;\n    }\n  }\n\n  friend mint operator+(const\
    \ mint &lhs, const mint &rhs)\n  { return mint(lhs) += rhs; }\n  friend mint operator-(const\
    \ mint &lhs, const mint &rhs)\n  { return mint(lhs) -= rhs; }\n  friend mint operator*(const\
    \ mint &lhs, const mint &rhs)\n  { return mint(lhs) *= rhs; }\n  friend mint operator/(const\
    \ mint &lhs, const mint &rhs)\n  { return mint(lhs) /= rhs; }\n  friend bool operator==(const\
    \ mint &lhs, const mint &rhs)\n  { return lhs._v == rhs._v; }\n  friend bool operator!=(const\
    \ mint &lhs, const mint &rhs)\n  { return lhs._v != rhs._v; }\n\n  friend istream\
    \ &operator>>(istream &is, mint &x)\n  {\n    ll a;\n    is >> a;\n    x = a;\n\
    \    return is;\n  }\n  friend ostream &operator<<(ostream &os, const mint &x)\n\
    \  {\n    os << x.val();\n    return os;\n  }\n};\n\ntemplate <int id>\nstruct\
    \ dynamic_modint64_odd\n{\n  using mint = dynamic_modint64_odd;\nprivate:\n  ull\
    \ _v;  // montgomery expression\n  static internal::montgomery64odd mg;\n  static\
    \ ull umod() { return mg.umod(); }\n\npublic:\n  static ll mod() { return (ll)(mg.umod());\
    \ }\n  static void set_mod(ll m)\n  {\n    assert(m >= 1 && m % 2 == 1);\n   \
    \ mg = internal::montgomery64odd(m);\n  }\n\n  dynamic_modint64_odd() : _v(0)\
    \ {}\n  dynamic_modint64_odd(i128 v)\n  { _v = mg.inv_reduce(v); }\n\n  ll val()\
    \ const { return (ll)mg.reduce(_v); }\n\n  mint& operator++()\n  {\n    _v++;\n\
    \    if (_v == umod())\n      _v = 0;\n    return *this;\n  }\n  mint& operator--()\n\
    \  {\n    if (_v == 0)\n      _v = umod();\n    _v--;\n    return *this;\n  }\n\
    \  mint operator++(int)\n  {\n    mint res = *this;\n    ++*this;\n    return\
    \ res;\n  }\n  mint operator--(int)\n  {\n    mint res = *this;\n    --*this;\n\
    \    return res;\n  }\n\n  mint& operator+=(const mint& rhs)\n  {\n    _v += rhs._v;\n\
    \    if (_v >= umod())\n      _v -= umod();\n    return *this;\n  }\n  mint& operator-=(const\
    \ mint &rhs)\n  {\n    _v -= rhs._v;\n    if (_v >= umod())\n      _v += umod();\n\
    \    return *this;\n  }\n  mint& operator*=(const mint &rhs)\n  {\n    _v = mg.reduce(u128(_v)\
    \ * rhs._v);\n    return *this;\n  }\n  mint& operator/=(const mint &rhs) { return\
    \ *this = *this * rhs.inv(); }\n\n  mint operator+() const { return *this; }\n\
    \  mint operator-() const { return mint() - *this; }\n\n  mint pow(ll n) const\n\
    \  {\n    assert(n >= 0);\n    mint x = *this, r = 1;\n    while (n)\n    {\n\
    \      if (n & 1)\n        r *= x;\n      x *= x;\n      n >>= 1;\n    }\n   \
    \ return r;\n  }\n  mint inv() const\n  {\n    auto [g, x, y] = extgcd<ll>(val(),\
    \ mod());\n    assert(g == 1);\n    return x;\n  }\n\n  friend mint operator+(const\
    \ mint &lhs, const mint &rhs)\n  { return mint(lhs) += rhs; }\n  friend mint operator-(const\
    \ mint &lhs, const mint &rhs)\n  { return mint(lhs) -= rhs; }\n  friend mint operator*(const\
    \ mint &lhs, const mint &rhs)\n  { return mint(lhs) *= rhs; }\n  friend mint operator/(const\
    \ mint &lhs, const mint &rhs)\n  { return mint(lhs) /= rhs; }\n  friend bool operator==(const\
    \ mint &lhs, const mint &rhs)\n  { return lhs._v == rhs._v; }\n  friend bool operator!=(const\
    \ mint &lhs, const mint &rhs)\n  { return lhs._v != rhs._v; }\n\n  friend istream\
    \ &operator>>(istream &is, mint &x)\n  {\n    ll a;\n    is >> a;\n    x = a;\n\
    \    return is;\n  }\n  friend ostream &operator<<(ostream &os, const mint &x)\n\
    \  {\n    os << x.val();\n    return os;\n  }\n};\ntemplate <int id>\ninternal::montgomery64odd\
    \ dynamic_modint64_odd<id>::mg((1LL << 61) - 1);\n\nusing modint61 = static_modint64<(1LL\
    \ << 61) - 1>;\n#line 2 \"math/prime/prime_power.hpp\"\n\n#line 4 \"math/prime/prime_power.hpp\"\
    \n\n/**\n * @brief \u7D20\u3079\u304D\u69CB\u9020\u4F53\n * @docs docs/math/prime/prime_power.md\n\
    \ */\n\ntemplate <class P>\nstruct PrimePower\n{\n  P p;\n  int e;\n  P pe;\n\n\
    \  PrimePower() {}\n  PrimePower(P p, int e = 1) : p(p), e(e), pe(ipow(p, e))\
    \ {}\n  PrimePower(P p, int e, P pe) : p(p), e(e), pe(pe) {}\n  template <class\
    \ P2>\n  PrimePower(const PrimePower<P2> &pp) : p(pp.p), e(pp.e), pe(pp.pe) {}\n\
    \n  void mul_p() { e++, pe *= p; }\n  void div_p() { e--, pe /= p; }\n};\n#ifdef\
    \ LOCAL\nCPP_DUMP_DEFINE_EXPORT_OBJECT(PrimePower<int>, p, e, pe);\nCPP_DUMP_DEFINE_EXPORT_OBJECT(PrimePower<ll>,\
    \ p, e, pe);\n#endif\n#line 2 \"math/prime/primality_test.hpp\"\n\n#line 6 \"\
    math/prime/primality_test.hpp\"\n\n/**\n * @brief \u7D20\u6570\u5224\u5B9A\n *\
    \ @docs docs/math/prime/primality_test.md\n */\n\nbool is_prime_int(int n)\n{\n\
    \  if (n <= 1)\n    return false;\n  if (n == 2 || n == 7 || n == 61)\n    return\
    \ true;\n  if (n % 2 == 0)\n    return false;\n  ll d = (n - 1) >> countr_zero(n\
    \ - 1);\n  static const ll bases[3] = {2, 7, 61};\n  using mint = dynamic_modint<-2>;\n\
    \  mint::set_mod(n);\n  for (ll a : bases)\n  {\n    ll t = d;\n    mint y = mint(a).pow(t);\n\
    \    while (t != n - 1 && y != 1 && y != n - 1)\n    {\n      y *= y;\n      t\
    \ <<= 1;\n    }\n    if (y != n - 1 && t % 2 == 0)\n      return false;\n  }\n\
    \  return true;\n}\n\nbool is_prime(ll n)\n{\n  if (n <= INT_MAX)\n    return\
    \ is_prime_int(n);\n  if (n % 2 == 0)\n    return false;\n  ll d = (n - 1) >>\
    \ countr_zero(n - 1);\n  static const ll bases[7] = {2, 325, 9375, 28178, 450775,\
    \ 9780504, 1795265022};\n  using mint = dynamic_modint64_odd<-2>;\n  mint::set_mod(n);\n\
    \  for (ll a : bases)\n  {\n    ll t = d;\n    mint y = mint(a).pow(t);\n    while\
    \ (t != n - 1 && y != 1 && y != n - 1)\n    {\n      y *= y;\n      t <<= 1;\n\
    \    }\n    if (y != n - 1 && t % 2 == 0)\n      return false;\n  }\n  return\
    \ true;\n}\n#line 8 \"math/prime/factorize.hpp\"\n\n/**\n * @brief \u7D20\u56E0\
    \u6570\u5206\u89E3\n * @docs docs/math/prime/factorize.md\n */\n\nnamespace internal\n\
    {\n\nint get_prime_factor_int(int n)\n{\n  int m = pow(n, .125);\n  using mint\
    \ = dynamic_modint<-2>;\n  mint::set_mod(n);\n  for (int c = 1;; c++)\n  {\n \
    \   mint x = 2, y = 2, prod = 1;\n    int g = 1;\n    for (int t = 1; g == 1;\
    \ t = min(2 * t, m))\n    {\n      repi(i, t)\n      {\n        x = x * x + c;\n\
    \        y = y * y + c, y = y * y + c;\n        prod *= x - y;\n      }\n    \
    \  g = gcd(prod.val(), n);\n    }\n    if (g == n)\n      continue;\n    if (is_prime(g))\n\
    \      return g;\n    else if (is_prime(n / g))\n      return n / g;\n    else\n\
    \      return get_prime_factor_int(g);\n  }\n}\n\nll get_prime_factor(ll n)\n\
    {\n  if (n <= INT_MAX)\n    return get_prime_factor_int(n);\n  int m = pow(n,\
    \ .125);\n  using mint = dynamic_modint64_odd<-2>;\n  mint::set_mod(n);\n  for\
    \ (int c = 1;; c++)\n  {\n    mint x = 2, y = 2, prod = 1;\n    ll g = 1;\n  \
    \  for (int t = 1; g == 1; t = min(2 * t, m))\n    {\n      repi(i, t)\n     \
    \ {\n        x = x * x + c;\n        y = y * y + c, y = y * y + c;\n        prod\
    \ *= x - y;\n      }\n      g = gcd(prod.val(), n);\n    }\n    if (g == n)\n\
    \      continue;\n    if (is_prime(g))\n      return g;\n    else if (is_prime(n\
    \ / g))\n      return n / g;\n    else\n      return get_prime_factor(g);\n  }\n\
    }\n\n};\n\nvc<PrimePower<ll>> factorize(ll n)\n{\n  vc<PrimePower<ll>> res;\n\
    \  repi(p, 2, 100)\n  {\n    PrimePower<ll> pp(p, 0);\n    while (n % p == 0)\n\
    \      n /= p, pp.mul_p();\n    if (pp.e > 0)\n      res.emplace_back(pp);\n \
    \ }\n  while (n > 1)\n  {\n    if (is_prime(n))\n    {\n      res.emplace_back(n);\n\
    \      break;\n    }\n    ll p = internal::get_prime_factor(n);\n    PrimePower<ll>\
    \ pp(p, 0);\n    while (n % p == 0)\n      n /= p, pp.mul_p();\n    res.emplace_back(pp);\n\
    \  }\n  sort(ALL(res), [&](cauto &pp1, cauto &pp2)\n       { return pp1.p < pp2.p;\
    \ });\n  return res;\n}\n\nvc<ll> divisors(const vc<PrimePower<ll>> &pps)\n{\n\
    \  vc<ll> res;\n  auto dfs = [&](auto dfs, ll d, int i) -> void\n  {\n    if (i\
    \ == SZ<int>(pps))\n    {\n      res.emplace_back(d);\n      return;\n    }\n\
    \    auto &pp = pps[i];\n    ull nd = d;\n    repi(j, pp.e + 1)\n    {\n     \
    \ dfs(dfs, nd, i + 1);\n      nd *= pp.p;\n    }\n  };\n  dfs(dfs, 1, 0);\n  sort(ALL(res));\n\
    \  return res;\n}\n#line 15 \"verify/yukicoder/yuki888.test.cpp\"\n\nvoid init()\
    \ {}\n\nvoid main2()\n{\n  LL(N);\n  auto ds = divisors(factorize(N));\n  PRINT(SUM(ds));\n\
    }\n\nvoid test() {}\n\nint main()\n{\n  cauto CERR = [](cauto &val)\n  {\n   \
    \ #ifndef BOJ\n      cerr << val;\n    #endif\n  };\n\n  #if defined FAST_IO and\
    \ not defined LOCAL\n  CERR(\"[FAST_IO]\\n\\n\");\n  cin.tie(0);\n  ios::sync_with_stdio(false);\n\
    \  #endif\n  cout << fixed << setprecision(20);\n\n  test();\n  init();\n\n  #if\
    \ defined AOJ_TESTCASE or (not defined NOT_AOJ and defined LOCAL and defined SINGLE_TESTCASE)\n\
    \  CERR(\"[AOJ_TESTCASE]\\n\\n\");\n  while (true)\n  {\n    dump(\"new testcase\"\
    );\n    main2();\n  }\n  #elif defined SINGLE_TESTCASE\n  CERR(\"[SINGLE_TESTCASE]\\\
    n\\n\");\n  main2();\n  #elif defined MULTI_TESTCASE\n  CERR(\"[MULTI_TESTCASE]\\\
    n\\n\");\n  int T;\n  cin >> T;\n  while (T--)\n  {\n    dump(\"new testcase\"\
    );\n    main2();\n  }\n  #endif\n}\n"
  code: "#define PROBLEM \"https://yukicoder.me/problems/no/888\"\n\n#define SINGLE_TESTCASE\n\
    // #define MULTI_TESTCASE\n// #define AOJ_TESTCASE\n\n#define FAST_IO\n\n#define\
    \ INF 4'000'000'000'000'000'037LL\n#define EPS 1e-11\n\n#include \"../../template/template_all.hpp\"\
    \n\n#include \"../../math/prime/factorize.hpp\"\n\nvoid init() {}\n\nvoid main2()\n\
    {\n  LL(N);\n  auto ds = divisors(factorize(N));\n  PRINT(SUM(ds));\n}\n\nvoid\
    \ test() {}\n\nint main()\n{\n  cauto CERR = [](cauto &val)\n  {\n    #ifndef\
    \ BOJ\n      cerr << val;\n    #endif\n  };\n\n  #if defined FAST_IO and not defined\
    \ LOCAL\n  CERR(\"[FAST_IO]\\n\\n\");\n  cin.tie(0);\n  ios::sync_with_stdio(false);\n\
    \  #endif\n  cout << fixed << setprecision(20);\n\n  test();\n  init();\n\n  #if\
    \ defined AOJ_TESTCASE or (not defined NOT_AOJ and defined LOCAL and defined SINGLE_TESTCASE)\n\
    \  CERR(\"[AOJ_TESTCASE]\\n\\n\");\n  while (true)\n  {\n    dump(\"new testcase\"\
    );\n    main2();\n  }\n  #elif defined SINGLE_TESTCASE\n  CERR(\"[SINGLE_TESTCASE]\\\
    n\\n\");\n  main2();\n  #elif defined MULTI_TESTCASE\n  CERR(\"[MULTI_TESTCASE]\\\
    n\\n\");\n  int T;\n  cin >> T;\n  while (T--)\n  {\n    dump(\"new testcase\"\
    );\n    main2();\n  }\n  #endif\n}"
  dependsOn:
  - template/template_all.hpp
  - template/template_types.hpp
  - template/template_rep.hpp
  - template/template_math.hpp
  - template/template_vector.hpp
  - template/template_func.hpp
  - template/template_algo.hpp
  - template/template_binsearch.hpp
  - template/template_bit.hpp
  - template/template_inout.hpp
  - template/template_dump.hpp
  - math/prime/factorize.hpp
  - math/modint/modint.hpp
  - math/modint/modint_internal.hpp
  - math/extgcd.hpp
  - math/modint/modint64.hpp
  - math/prime/prime_power.hpp
  - math/prime/primality_test.hpp
  isVerificationFile: true
  path: verify/yukicoder/yuki888.test.cpp
  requiredBy: []
  timestamp: '2024-12-27 23:57:11+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/yukicoder/yuki888.test.cpp
layout: document
redirect_from:
- /verify/verify/yukicoder/yuki888.test.cpp
- /verify/verify/yukicoder/yuki888.test.cpp.html
title: verify/yukicoder/yuki888.test.cpp
---
