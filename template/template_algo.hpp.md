---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: template/template_func.hpp
    title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u95A2\u6570\u30AA\u30D6\u30B8\
      \u30A7\u30AF\u30C8\uFF09"
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
  _extendedRequiredBy:
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
    path: verify/mytest/template_algo.test.cpp
    title: verify/mytest/template_algo.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/mytest/template_bit.test.cpp
    title: verify/mytest/template_bit.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/aplusb.test.cpp
    title: verify/yosupo/aplusb.test.cpp
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
    path: verify/yosupo/primality_test.test.cpp
    title: verify/yosupo/primality_test.test.cpp
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
    _deprecated_at_docs: docs/template/template_algo.md
    document_title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30A2\u30EB\u30B4\u30EA\
      \u30BA\u30E0\uFF09"
    links: []
  bundledCode: "#line 2 \"template/template_algo.hpp\"\n\n#ifndef INF\n#define INF\
    \ 4'000'000'000'000'000'037LL\n#endif\n\n#line 2 \"template/template_types.hpp\"\
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
    #define fem(...) for (auto &__VA_ARGS__)\n#line 2 \"template/template_vector.hpp\"\
    \n\n#line 2 \"template/template_math.hpp\"\n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n\
    #endif\n#ifndef EPS\n#define EPS 1e-11\n#endif\n\n#line 12 \"template/template_math.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u6F14\u7B97\uFF09\
    \n * @docs docs/template/template_math.md\n */\n\ninline bool chmin(auto &a, cauto\
    \ &b) { return a > b ? a = b, true : false; }\ninline bool chmax(auto &a, cauto\
    \ &b) { return a < b ? a = b, true : false; }\n\ntemplate <class T = ll>\ninline\
    \ constexpr T divfloor(cauto &a, cauto &b) { return T(a) / T(b) - (T(a) % T(b)\
    \ && (T(a) ^ T(b)) < 0); }\ntemplate <class T = ll>\ninline constexpr T divceil(cauto\
    \ &a, cauto &b) { return T(a) / T(b) + (T(a) % T(b) && (T(a) ^ T(b)) >= 0); }\n\
    template <class T = ll>\ninline constexpr T divround(cauto &a, cauto &b) { return\
    \ divfloor<T>(2 * T(a) + T(b), 2 * T(b)); }\ntemplate <class T = ll>\ninline constexpr\
    \ T safemod(cauto &a, cauto &b) { return T(a) - T(b) * divfloor<T>(a, b); }\n\n\
    template <class T = ll>\nconstexpr T ipow(auto a, auto b)\n{\n  assert(b >= 0);\n\
    \  if (b == 0) return 1;\n  if (a == 0 || a == 1) return a;\n  if (a == -1) return\
    \ b & 1 ? -1 : 1;\n\n  T res = 1, tmp = a;\n  while (true)\n  {\n    if (b & 1)\n\
    \      res *= tmp;\n    b >>= 1;\n    if (b == 0)\n      break;\n    tmp *= tmp;\n\
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
    \ return dflt_positive;\n  return v[i];\n}\n#line 2 \"template/template_func.hpp\"\
    \n\n#line 5 \"template/template_func.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\
    \u30EC\u30FC\u30C8\uFF08\u95A2\u6570\u30AA\u30D6\u30B8\u30A7\u30AF\u30C8\uFF09\
    \n * @docs docs/template/template_func.md\n */\n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n\
    #endif\n\ntemplate <class T = ll>\nstruct max_op\n{\n  T operator()(const T &a,\
    \ const T &b) const { return max(a, b); }\n};\ntemplate <class T = ll>\nstruct\
    \ min_op\n{\n  T operator()(const T &a, const T &b) const { return min(a, b);\
    \ }\n};\n\ntemplate <class T, const T val>\nstruct const_fn\n{\n  T operator()()\
    \ const { return val; }\n};\nusing max_e = const_fn<ll, -INF>;\nusing min_e =\
    \ const_fn<ll, INF>;\nusing zero_fn = const_fn<ll, 0LL>;\n#line 11 \"template/template_algo.hpp\"\
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
    \  repi(i, n)\n  {\n    assert(SZI(a[i]) == m);\n    repi(j, m) b[j][i] = a[i][j];\n\
    \  }\n  return b;\n}\nvstr top(const vstr &a)\n{\n  vvc<char> a_(a.size());\n\
    \  repi(i, SZI(a)) a_[i] = {ALL(a[i])};\n  vvc<char> b_ = top(a_);\n  vstr b(b_.size());\n\
    \  repi(i, SZI(b)) b[i] = {ALL(b_[i])};\n  return b;\n}\n\n// 12\n// 34 -> 246\n\
    // 56    135\n// (\u53CD\u6642\u8A08\u56DE\u308A)\ntemplate <class VV, class U\
    \ = ll>\nVV rot90(const VV &a, U k = 1)\n{\n  if (a.empty())\n    return {};\n\
    \  const int n = a.size(), m = a[0].size();\n  k = (k % 4 + 4) % 4;\n  if (k ==\
    \ 0)\n    return a;\n  else if (k == 1)\n  {\n    VV b(m);\n    repi(j, m) b[j].resize(n);\n\
    \    repi(i, n)\n    {\n      assert(SZI(a[i]) == m);\n      repi(j, m) b[m -\
    \ 1 - j][i] = a[i][j];\n    }\n    return b;\n  }\n  else if (k == 2)\n  {\n \
    \   VV b(n);\n    repi(i, n) b[i].resize(m);\n    repi(i, n)\n    {\n      assert(SZI(a[i])\
    \ == m);\n      repi(j, m) b[n - 1 - i][m - 1 - j] = a[i][j];\n    }\n    return\
    \ b;\n  }\n  else\n  {\n    VV b(m);\n    repi(j, m) b[j].resize(n);\n    repi(i,\
    \ n)\n    {\n      assert(SZI(a[i]) == m);\n      repi(j, m) b[j][n - 1 - i] =\
    \ a[i][j];\n    }\n    return b;\n  }\n}\n\ntemplate <class T, class F = decltype(plus<>())>\n\
    vc<T> cuml(const vc<T> &v, const F &op = plus<>(), const T &e = 0)\n{\n  const\
    \ int n = v.size();\n  vc<T> res(n + 1, e);\n  repi(i, n) res[i + 1] = op(res[i],\
    \ v[i]);\n  return res;\n}\ntemplate <class T, class F = decltype(plus<>())>\n\
    vc<T> cumr(const vc<T> &v, const F &op = plus<>(), const T &e = 0)\n{ return reversed(cuml<T,\
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
    \ operator++()\n    {\n      b.back()++;\n      repi(i, SZI(prod.a) - 1, 0, -1)\n\
    \      {\n        if (b[i] == prod.a[i])\n        {\n          b[i] = 0;\n   \
    \       b[i - 1]++;\n        }\n        else\n          break;\n      }\n    \
    \  return *this;\n    }\n    bool operator!=(const Iterator &other) const { return\
    \ b != other.b; }\n  };\n  Iterator begin() const { return Iterator(vc<T>(a.size(),\
    \ 0), *this); }\n  Iterator end() const\n  {\n    vc<T> c(a.size(), 0);\n    c[0]\
    \ = a[0];\n    return Iterator(c, *this);\n  }\n};\n\n// DRUL\nconst vpll dij4\
    \ = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};\n"
  code: "#pragma once\n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n#endif\n\
    \n#include \"template_types.hpp\"\n#include \"template_rep.hpp\"\n#include \"\
    template_vector.hpp\"\n#include \"template_func.hpp\"\n\n/**\n * @brief \u30C6\
    \u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30A2\u30EB\u30B4\u30EA\u30BA\u30E0\uFF09\
    \n * @docs docs/template/template_algo.md\n */\n\ntemplate <class T, class U =\
    \ ll>\nU SUM(const vc<T> &v) { return accumulate(ALL(v), U(0)); }\ntemplate <class\
    \ T>\nT MAX(const vc<T> &v) { return *max_element(ALL(v)); }\ntemplate <class\
    \ T>\nT MIN(const vc<T> &v) { return *min_element(ALL(v)); }\ntemplate <class\
    \ T, class U = ll>\nU ARGMAX(const vc<T> &v) { return max_element(ALL(v)) - v.begin();\
    \ }\ntemplate <class T, class U = ll>\nU ARGMIN(const vc<T> &v) { return min_element(ALL(v))\
    \ - v.begin(); }\n\ntemplate<class T, class U = ll>\nU mex(const vector<T> &a)\n\
    {\n  int n = a.size();\n  vector<bool> exists(n, false);\n  repi(i, n) if (0 <=\
    \ a[i] && a[i] < n) exists[a[i]] = true;\n  repi(x, n) if (!exists[x]) return\
    \ x;\n  return n;\n}\n\ntemplate <class T = ll>\nvc<T> permid(const int &n, const\
    \ int &base_index = 0)\n{\n  vc<T> p(n);\n  repi(i, n) p[i] = i + base_index;\n\
    \  return p;\n}\ntemplate <class T>\nvc<T> perminv(const vc<T> &p)\n{\n  if (p.empty())\n\
    \    return {};\n  const int n = p.size();\n  vc<T> q(MAX(p) + 1);\n  repi(i,\
    \ n) if (p[i] >= 0) q[p[i]] = i;\n  return q;\n}\n// a[p[i]] for all i\ntemplate\
    \ <class T, class U>\nvc<T> permuted(const vc<T> &a, const vc<U> &p)\n{\n  const\
    \ int n = p.size();\n  vc<T> res(n);\n  repi(i, n)\n  {\n    assert(0 <= p[i]\
    \ && p[i] < U(a.size()));\n    res[i] = a[p[i]];\n  }\n  return res;\n}\n\ntemplate\
    \ <class V>\nV reversed(const V &v) { return {v.rbegin(), v.rend()}; }\n\n#if\
    \ __cplusplus < 202002L\ntemplate <class V, class... Args>\nV sorted(V v, Args&&...\
    \ args)\n{\n  sort(ALL(v), forward<Args>(args)...);\n  return v;\n}\n#else\ntemplate\
    \ <class V, class... Args>\nV sorted(V v, Args&&... args)\n{\n  ranges::sort(v,\
    \ forward<Args>(args)...);\n  return v;\n}\n#endif\n\ntemplate <class V>\nvoid\
    \ unique(V &v) { v.erase(unique(ALL(v)), v.end()); }\ntemplate <class V>\nV uniqued(V\
    \ v) { unique(v); return v; }\n\ntemplate <class V>\nvoid sortunique(V &v)\n{\n\
    \  sort(ALL(v));\n  unique(v);\n}\ntemplate <class V>\nV sortuniqued(V v) { sortunique(v);\
    \ return v; }\n\n// 01234 -> 12340\ntemplate <class V, class U>\nvoid rotate(V\
    \ &v, U k)\n{ \n  const U n = v.size();\n  k = (k % n + n) % n;\n  rotate(v.begin(),\
    \ v.begin() + k, v.end());\n}\n// 01234 -> 12340\ntemplate <class V, class U>\n\
    V rotated(V v, U k) { rotate(v, k); return v; }\n\ntemplate <class T>\nvvc<T>\
    \ top(const vvc<T> &a)\n{\n  if (a.empty())\n    return {};\n  const int n = a.size(),\
    \ m = a[0].size();\n  vvc<T> b(m, vc<T>(n));\n  repi(i, n)\n  {\n    assert(SZI(a[i])\
    \ == m);\n    repi(j, m) b[j][i] = a[i][j];\n  }\n  return b;\n}\nvstr top(const\
    \ vstr &a)\n{\n  vvc<char> a_(a.size());\n  repi(i, SZI(a)) a_[i] = {ALL(a[i])};\n\
    \  vvc<char> b_ = top(a_);\n  vstr b(b_.size());\n  repi(i, SZI(b)) b[i] = {ALL(b_[i])};\n\
    \  return b;\n}\n\n// 12\n// 34 -> 246\n// 56    135\n// (\u53CD\u6642\u8A08\u56DE\
    \u308A)\ntemplate <class VV, class U = ll>\nVV rot90(const VV &a, U k = 1)\n{\n\
    \  if (a.empty())\n    return {};\n  const int n = a.size(), m = a[0].size();\n\
    \  k = (k % 4 + 4) % 4;\n  if (k == 0)\n    return a;\n  else if (k == 1)\n  {\n\
    \    VV b(m);\n    repi(j, m) b[j].resize(n);\n    repi(i, n)\n    {\n      assert(SZI(a[i])\
    \ == m);\n      repi(j, m) b[m - 1 - j][i] = a[i][j];\n    }\n    return b;\n\
    \  }\n  else if (k == 2)\n  {\n    VV b(n);\n    repi(i, n) b[i].resize(m);\n\
    \    repi(i, n)\n    {\n      assert(SZI(a[i]) == m);\n      repi(j, m) b[n -\
    \ 1 - i][m - 1 - j] = a[i][j];\n    }\n    return b;\n  }\n  else\n  {\n    VV\
    \ b(m);\n    repi(j, m) b[j].resize(n);\n    repi(i, n)\n    {\n      assert(SZI(a[i])\
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
    \ operator++()\n    {\n      b.back()++;\n      repi(i, SZI(prod.a) - 1, 0, -1)\n\
    \      {\n        if (b[i] == prod.a[i])\n        {\n          b[i] = 0;\n   \
    \       b[i - 1]++;\n        }\n        else\n          break;\n      }\n    \
    \  return *this;\n    }\n    bool operator!=(const Iterator &other) const { return\
    \ b != other.b; }\n  };\n  Iterator begin() const { return Iterator(vc<T>(a.size(),\
    \ 0), *this); }\n  Iterator end() const\n  {\n    vc<T> c(a.size(), 0);\n    c[0]\
    \ = a[0];\n    return Iterator(c, *this);\n  }\n};\n\n// DRUL\nconst vpll dij4\
    \ = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};"
  dependsOn:
  - template/template_types.hpp
  - template/template_rep.hpp
  - template/template_vector.hpp
  - template/template_math.hpp
  - template/template_func.hpp
  isVerificationFile: false
  path: template/template_algo.hpp
  requiredBy:
  - math/modint/modint_internal.hpp
  - math/modint/modint.hpp
  - math/modint/modint64.hpp
  - math/prime/factorize.hpp
  - math/prime/primality_test.hpp
  - math/prime/prime_power.hpp
  - math/extgcd.hpp
  - template/template.cpp
  - template/template_all.hpp
  timestamp: '2024-12-21 17:37:41+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo/many_aplusb_128bit.test.cpp
  - verify/yosupo/kth_root_integer.test.cpp
  - verify/yosupo/factorize.test.cpp
  - verify/yosupo/many_aplusb_tuple.test.cpp
  - verify/yosupo/primality_test.test.cpp
  - verify/yosupo/aplusb.test.cpp
  - verify/yosupo/many_aplusb.test.cpp
  - verify/mytest/modint.test.cpp
  - verify/mytest/template_algo.test.cpp
  - verify/mytest/template_bit.test.cpp
  - verify/mytest/modint64.test.cpp
  - verify/aoj/extgcd.test.cpp
  - verify/yukicoder/yuki1550_static_modint.test.cpp
  - verify/yukicoder/yuki1550_dynamic_modint.test.cpp
  - verify/yukicoder/yuki888.test.cpp
documentation_of: template/template_algo.hpp
layout: document
redirect_from:
- /library/template/template_algo.hpp
- /library/template/template_algo.hpp.html
title: "\u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\u30A2\u30EB\u30B4\u30EA\u30BA\u30E0\
  \uFF09"
---
## テンプレート（アルゴリズム）

### 集約

#### SUM

```cpp
U SUM(vc<T> v)
```

$v$ の要素の総和を（型 `U` の値として）返す。`U` はデフォルトでは `ll`。

##### 計算量

- $O(\lvert v \rvert)$

#### MAX, MIN

```cpp
(1) T MAX(vc<T> v, T dflt = INF)
(2) T MIN(vc<T> v, T dflt = -INF)
```

それぞれ、$v$ の要素の最大値、最小値を返す。

##### 制約

- $\lvert v \rvert > 0$

##### 計算量

- $O(\lvert v \rvert)$

#### ARGMAX, ARGMIN

```cpp
(1) T ARGMAX(vc<T> v)
(2) T ARGMIN(vc<T> v)
```

それぞれ、$v$ の要素が最大値、最小値をとるインデックス（のうち最小のもの）を返す。$v$ が空のときは $0$ を返す。

##### 計算量

- $O(\lvert v \rvert)$

#### mex

```cpp
U mex(vc<T> v)
```

$v$ の要素の mex を（型 `U` の値として）返す。`U` はデフォルトでは `ll`。

$v$ の要素はすべて非負整数であることを仮定。負の数がある場合、それを無視した上で動作する。

##### 計算量

- $O(\lvert v \rvert)$

### 順列

#### permid

`vc<T = ll> permid(int n, int base_index = 0)`

長さ $n$ の恒等順列 $(0, 1, \dots, n-1)$ を返す。

##### 計算量

- $O(n)$

#### perminv

```cpp
vc<T> perminv(vc<T> p)
```

順列 $p$ の逆順列を返す。すなわち、$q_{p_i} = i$ を満たす順列 $q$ を返す。

$p$ が $(0, 1, \dots, \lvert p \rvert-1)$ の順列でない場合は、$p_i \geq 0$ なるすべての $i$ に対し $q_{p_i} = i$ を満たす配列 $q$ をどれか一つ返す。

##### 計算量

- $O(\lvert p \rvert + \max(p))$

#### permuted

```cpp
vc<T> permuted(vc<T> a, vc<U> p)
```

配列 $a$ を配列 $p$ にしたがって並べ替えた配列を返す。すなわち、$(a_{p_0}, a_{p_1}, \dots, a_{p_{\lvert p \rvert-1}})$ を返す。特に、$p$ が順列の場合や $a, p$ ともに順列の場合の使用を想定している。

##### 制約

- $0 \leq p_i < \lvert a \rvert$

##### 計算量

- $O(\lvert p \rvert)$


### string や vector の操作

#### reversed

```cpp
(1) string reversed(string s)
(2) vc<T> reversed(vc<T> v)
```

列を逆順にしたものを返す（破壊的変更は行わない）。

使用例：

- 回文判定が `s == reversed(s)` と書ける
- 左右から見る系の処理で `reversed(f(reversed(a)))` などと書ける

##### 計算量

列の長さを $n$ として

- $O(n)$

#### SORT, sorted

```cpp
(1) string sorted(string s, ...)
(2) vc<T> sorted(vc<T> v, ...)
```

列をソートしたものを返す（破壊的変更は行わない）。

C++17 以前ならば `std::sort`, C++20 以降ならば `std::ranges::sort` の引数を後ろにとれる。

##### 制約

`std::sort` に準じる。

##### 計算量

`std::sort` に準じる。具体的には、列の長さを $n$ として

- $O(n \log n)$

#### unique, uniqued, sortunique, sortuniqued

`std::unique` をもとにしている。

```cpp
(1) void unique(string &s)
(2) void unique(vc<T> &v)
(3) string uniqued(string s)
(4) vc<T> uniqued(vc<T> v)
(5) void sortunique(string &s)
(6) void sortunique(vc<T> &v)
(7) string sortunique(string s)
(8) vc<T> sortunique(vc<T> v)
```

- (1), (2)：重複する隣接要素を削除する。
- (3), (4)：重複する隣接要素を削除した列を返す。
- (5), (6)：ソートした後、重複する隣接要素を削除する。
- (7), (8)：「ソートした後、重複する隣接要素を削除する」操作を行った結果の列を返す。

##### 計算量

列の長さを $n$ として

- (1), (2), (3), (4)：$O(n)$
- (5), (6), (7), (8)：$O(n \log n)$

#### rotate, rotated

`std::rotate` をもとにしている。

```cpp
(1) void rotate(string &s, U k)
(2) void rotate(vc<T> &v, U k)
(3) string rotated(string s, U k)
(4) vc<T> rotated(vc<T> v, U k)
```

- (1), (2)：列を**右に** $k$ 個 cyclic-shift する。
- (3), (4)：列を**右に** $k$ 個 cyclic-shift してできる列を返す。

右シフトなので、たとえば $(0, 1, 2, 3, 4)$ は $1$ 個シフトすると $(1, 2, 3, 4, 0)$ になる。

##### 制約

- `U` は整数
- $k$ は型 `U` の上限・下限付近ではない

##### 計算量

列の長さを $n$ として

- $O(n)$

### 二次元配列の操作

#### top

```cpp
(1) vvc<T> top(vvc<T> a)
(2) vstr top(vstr a)
```

二次元配列 $a$ を転置した二次元配列を返す。すなわち、$a$ のサイズが $n \times m$ であるとき、サイズが $m \times n$ で、$a_{i,j} = b_{j,i} \: (0 \leq i < n, 0 \leq j < m)$ を満たす二次元配列 $b$ を返す。

ただし、サイズが $0 \times m$ の二次元配列は表現できない（$0 \times 0$ とみなされる）ことに注意。

##### 計算量

$a$ のサイズを $n \times m$ として

- $O(nm)$

#### rot90

```cpp
(1) vvc<T> rot90(vvc<T> a, U k = 1)
(2) vstr rot90(vstr a, U k = 1)
```

二次元配列を**反時計回りに** $90$ 度回転することを $k$ 回行った結果の二次元配列を返す。

```
12
34 -> 246
56    135
```

同じく $0 \times m$ には注意。

##### 制約

- `U` は整数
- $k$ は型 `U` の上限・下限付近ではない

##### 計算量

$a$ のサイズを $n \times m$ として

- $O(nm)$

### 累積和・差分

#### cuml, cumr

```cpp
(1) vc<T> cuml(vc<T> v, F op = plus<>(), T e = 0)
(2) vc<T> cumr(vc<T> v, F op = plus<>(), T e = 0)
(3) vc<T> cumlmax(vc<T> v)
(4) vc<T> cumrmax(vc<T> v)
(5) vc<T> cumlmin(vc<T> v)
(6) vc<T> cumrmin(vc<T> v)
```

$\lvert v \rvert = n$ とする。

- (1)：長さ $n + 1$ の配列を返す。$i \: (0 \leq i \leq n)$ 番目には、$v[0, i)$ 全体に二項演算 $\mathrm{op}$ を行った結果を格納する。単位元は $\mathrm{e}$ 。デフォルトでは、$\mathrm{op}$ は $+$ 、$\mathrm{e}$ は $0$ 。
- (2)：長さ $n + 1$ の配列を返す。$i \: (0 \leq i \leq n)$ 番目には、$v[i, n)$ 全体に二項演算 $\mathrm{op}$ を行った結果を格納する。単位元は $\mathrm{e}$ 。デフォルトでは、$\mathrm{op}$ は $+$ 、$\mathrm{e}$ は $0$ 。
- (3), (4), (5), (6)：(1) や (2) で、$(\mathrm{op}, \mathrm{e})$ を $(\max, -\mathrm{INF})$ や $(\min, \mathrm{INF})$ としたもの。

例 (cuml)：$(3, 1, 4, 1) \to (0, 3, 4, 8, 9)$

##### 制約

- $\mathrm{op}$ は可換

##### 計算量

- $O(n)$

#### adjd

```cpp
vc<T> adjd(vc<T> v)
```

$\lvert v \rvert = n$ とする。

長さ $n+1$ の配列を返す。$i \: (0 \leq i \leq n)$ 番目には、$v_i - v_{i-1}$ が格納される（ただし、$v_{-1}$ や $v_n$ は $0$ とみなす）。

例：$(3, 1, 4, 1) \to (3, -2, 3, -3, -1)$ 

##### 計算量

- $O(n)$

### 全探索

#### direct_product

```cpp
direct_product(vc<T> a)
```

長さ $\lvert a \rvert$ で、$i \: (0 \leq i < \lvert a \rvert)$ 番目が $[0, a_i)$ であるような vector をすべて（辞書順で）列挙する。
  - 例：$a = (2, 1, 3)$ のとき
    - $(0, 0, 0)$
    - $(0, 0, 1)$
    - $(0, 0, 2)$
    - $(1, 0, 0)$
    - $(1, 0, 1)$
    - $(1, 0, 2)$

イテレータを実装している形なので、たとえば範囲 for の中で
```
fec(v : direct_product({2, 1, 3}))
```
のように使う。

##### 計算量

- $1$ 回のイテレーションが償却 $O(1)$

### その他

#### dij4

```cpp
const vpll dij4 = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} }
```

グリッドの $4$ 方向の探索で使う。

$(i, j)$ の $i$ を下方向、$j$ を右方向とする座標系において、`DRUL` の順になっている。