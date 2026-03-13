---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: math/convolution/convolution.hpp
    title: "\u7573\u307F\u8FBC\u307F"
  - icon: ':question:'
    path: math/crt.hpp
    title: "\u4E2D\u56FD\u5270\u4F59\u5B9A\u7406 (CRT)"
  - icon: ':question:'
    path: math/extgcd.hpp
    title: "\u62E1\u5F35\u30E6\u30FC\u30AF\u30EA\u30C3\u30C9\u4E92\u9664\u6CD5 (extgcd)"
  - icon: ':question:'
    path: math/fps/fps.hpp
    title: "\u5F62\u5F0F\u7684\u51AA\u7D1A\u6570"
  - icon: ':heavy_check_mark:'
    path: math/fps/multipoint_evaluation_geo.hpp
    title: "\u591A\u9805\u5F0F\u306E\u591A\u70B9\u8A55\u4FA1\uFF08\u8A55\u4FA1\u70B9\
      \u304C\u7B49\u6BD4\u6570\u5217\uFF09"
  - icon: ':question:'
    path: math/modint/binomial.hpp
    title: "\u4E8C\u9805\u4FC2\u6570"
  - icon: ':heavy_check_mark:'
    path: math/modint/inv_many.hpp
    title: "\u9006\u5143\u3092\u307E\u3068\u3081\u3066\u6C42\u3081\u308B"
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
    path: math/modint/sqrt_mod.hpp
    title: "mod \u7D20\u6570 \u306E\u5E73\u65B9\u6839"
  - icon: ':question:'
    path: math/prime/large/primality_test.hpp
    title: "\u7D20\u6570\u5224\u5B9A"
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
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/poly_interpolation_geo.test.cpp
    title: verify/yosupo/poly_interpolation_geo.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    _deprecated_at_docs: docs/math/fps/interpolation_geo.md
    document_title: "\u591A\u9805\u5F0F\u88DC\u9593\uFF08\u8A55\u4FA1\u70B9\u304C\u7B49\
      \u6BD4\u6570\u5217\uFF09"
    links:
    - https://noshi91.github.io/algorithm-encyclopedia/polynomial-interpolation-geometric#noredirect
  bundledCode: "#line 2 \"math/fps/interpolation_geo.hpp\"\n\n#line 2 \"template/template_all_but_modint.hpp\"\
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
    \ i128 res = 0;\n  if (s.front() == '-')\n  {\n    for (int i = 1; i < (int)s.size();\
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
    #define fem(...) for (auto &__VA_ARGS__)\n#line 2 \"template/template_math.hpp\"\
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
    \ vt;\n}\n// ----------\n#line 2 \"template/template_random.hpp\"\n\n#line 5 \"\
    template/template_random.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\
    \u30C8\uFF08\u30E9\u30F3\u30C0\u30E0\u751F\u6210\uFF09\n * @docs docs/template/template_random.md\n\
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
    \ 4 \"math/fps/interpolation_geo.hpp\"\n\n#line 2 \"math/fps/fps.hpp\"\n\n#line\
    \ 4 \"math/fps/fps.hpp\"\n\n#line 2 \"math/modint/binomial.hpp\"\n\n#line 4 \"\
    math/modint/binomial.hpp\"\n\n/**\n * @brief \u4E8C\u9805\u4FC2\u6570\n * @docs\
    \ docs/math/modint/binomial.md\n */\n\ntemplate <class T>\nstruct Binomial\n{\n\
    private:\n  static decltype(T::mod()) mod;\n  static vc<T> fac_, finv_, inv_;\n\
    \npublic:\n  static void reserve(int n)\n  {\n    if (mod != T::mod())\n    {\n\
    \      mod = T::mod();\n      fac_ = {1, 1}, finv_ = {1, 1}, inv_ = {0, 1};\n\
    \    }\n    int i = fac_.size();\n    chmin(n, T::mod() - 1);\n    if (n < i)\n\
    \      return;\n    fac_.resize(n + 1), finv_.resize(n + 1), inv_.resize(n + 1);\n\
    \    for (; i <= n; i++)\n    {\n      fac_[i] = fac_[i - 1] * T::raw(i);\n  \
    \    inv_[i] = -inv_[T::mod() % i] * T::raw(T::mod() / i);\n      finv_[i] = finv_[i\
    \ - 1] * inv_[i];\n    }\n  }\n  static T fac(int n)\n  {\n    assert(n >= 0);\n\
    \    if (n >= T::mod())\n      return 0;\n    reserve(n);\n    return fac_[n];\n\
    \  }\n  static T finv(int n)\n  {\n    assert(n < T::mod());\n    if (n < 0)\n\
    \      return 0;\n    reserve(n);\n    return finv_[n];\n  }\n  static T inv(T\
    \ n)\n  {\n    assert(n != 0);\n    reserve(n.val());\n    return inv_[n.val()];\n\
    \  }\n\n  static T P(int n, int k)\n  {\n    if (n < k)\n      return 0;\n   \
    \ if (n < 0 || k < 0)\n      return 0;\n    reserve(n);\n    return fac_[n] *\
    \ finv_[n - k];\n  }\n  static T C(int n, int k)\n  {\n    if (n < k)\n      return\
    \ 0;\n    if (n < 0 || k < 0)\n      return 0;\n    reserve(n);\n    return fac_[n]\
    \ * finv_[k] * finv_[n - k];\n  }\n  static T H(int n, int k)\n  {\n    if (n\
    \ == 0 && k == 0)\n      return 1;\n    return C(n + k - 1, k);\n  }\n};\ntemplate\
    \ <class T> decltype(T::mod()) Binomial<T>::mod{};\ntemplate <class T> vc<T> Binomial<T>::fac_{};\n\
    template <class T> vc<T> Binomial<T>::finv_{};\ntemplate <class T> vc<T> Binomial<T>::inv_{};\n\
    #line 2 \"math/convolution/convolution.hpp\"\n\n#line 4 \"math/convolution/convolution.hpp\"\
    \n\n#line 2 \"math/modint/modint.hpp\"\n\n#line 2 \"math/modint/modint32_internal.hpp\"\
    \n\n#line 4 \"math/modint/modint32_internal.hpp\"\n\nnamespace internal\n{\n\n\
    constexpr ll powmod32_constexpr(ll x, ll n, int m)\n{\n  if (m == 1)\n    return\
    \ 0;\n  uint _m = (uint)m;\n  ull r = 1;\n  ull y = safemod(x, m);\n  while (n)\n\
    \  {\n    if (n & 1)\n      r = (r * y) % _m;\n    y = (y * y) % _m;\n    n >>=\
    \ 1;\n  }\n  return r;\n}\n\nconstexpr bool isprime32_constexpr(int n)\n{\n  if\
    \ (n <= 1)\n    return false;\n  if (n == 2 || n == 7 || n == 61)\n    return\
    \ true;\n  if (n % 2 == 0)\n    return false;\n  ll d = n - 1;\n  while (d % 2\
    \ == 0)\n    d /= 2;\n  constexpr ll bases[3] = {2, 7, 61};\n  for (ll a : bases)\n\
    \  {\n    ll t = d;\n    ll y = powmod32_constexpr(a, t, n);\n    while (t !=\
    \ n - 1 && y != 1 && y != n - 1)\n    {\n      y = y * y % n;\n      t <<= 1;\n\
    \    }\n    if (y != n - 1 && t % 2 == 0)\n      return false;\n  }\n  return\
    \ true;\n}\ntemplate <int n>\nconstexpr bool isprime32 = isprime32_constexpr(n);\n\
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
    \ ll>\nconstexpr tuple<T, T, T> extgcd(T a, T b)\n{\n  if (a == 0 && b == 0)\n\
    \    return {0, 0, 0};\n  \n  // a*x1 + b*y1 == z1  ...(1)\n  // a*x2 + b*y2 ==\
    \ z2  ...(2)\n  T x1 = 1, y1 = 0, z1 = a;\n  T x2 = 0, y2 = 1, z2 = b;\n  while\
    \ (z2 != 0)\n  {\n    // (1)' = (2)\n    // (2)' = (1) - q*(2)\n    T q = z1 /\
    \ z2;\n    tie(x1, x2) = make_pair(x2, x1 - q * x2);\n    tie(y1, y2) = make_pair(y2,\
    \ y1 - q * y2);\n    tie(z1, z2) = make_pair(z2, z1 - q * z2);\n  }\n  if (z1\
    \ < 0)\n    x1 = -x1, y1 = -y1, z1 = -z1;\n  return {z1, x1, y1};\n}\n#line 7\
    \ \"math/modint/modint.hpp\"\n\n/**\n * @brief modint (32 bit)\n * @docs docs/math/modint/modint.md\n\
    \ */\n\ntemplate <int m>\nstruct static_modint : internal::modint_base<static_modint<m>>\n\
    {\n  using mint = static_modint;\nprivate:\n  friend struct internal::modint_base<static_modint<m>>;\n\
    \  uint _v;\n  static constexpr uint umod() { return m; }\n  static constexpr\
    \ bool prime = internal::isprime32<m>;\n\npublic:\n  static constexpr int mod()\
    \ { return m; }\n  static mint raw(int v)\n  {\n    mint x;\n    x._v = v;\n \
    \   return x;\n  }\n\n  static_modint() : _v(0) {}\n  template <class T, typename\
    \ = enable_if_t<is_integral<T>::value>>\n  static_modint(T v)\n  {\n    if constexpr\
    \ (is_signed_v<T>)\n    {\n      ll x = (ll)(v % (ll)(umod()));\n      if (x <\
    \ 0)\n        x += umod();\n      _v = (uint)x;\n    }\n    else\n    {\n    \
    \  _v = (uint)(v % umod());\n    }\n  }\n\n  int val() const { return (int)_v;\
    \ }\n\n  mint& operator*=(const mint &rhs)\n  {\n    ull z = _v;\n    z *= rhs._v;\n\
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
    \  }\n\n  dynamic_modint() : _v(0) {}\n  template <class T, typename = enable_if_t<is_integral<T>::value>>\n\
    \  dynamic_modint(T v)\n  {\n    if constexpr (is_signed_v<T>)\n    {\n      ll\
    \ x = (ll)(v % (ll)(umod()));\n      if (x < 0)\n        x += umod();\n      _v\
    \ = (uint)x;\n    }\n    else\n    {\n      _v = (uint)(v % umod());\n    }\n\
    \  }\n\n  int val() const { return (int)_v; }\n\n  mint& operator*=(const mint\
    \ &rhs)\n  {\n    _v = bt.mul(_v, rhs._v);\n    return *this;\n  }\n\n  mint inv()\
    \ const\n  {\n    auto [g, x, y] = extgcd<int>(_v, mod());\n    assert(g == 1);\n\
    \    return x;\n  }\n};\ntemplate <int id>\ninternal::barrett32 dynamic_modint<id>::bt(998244353);\n\
    \nusing modint998244353 = static_modint<998244353>;\nusing modint1000000007 =\
    \ static_modint<1000000007>;\nusing modint = dynamic_modint<-1>;\n\ntemplate <class\
    \ T>\nstruct is_static_modint : false_type {};\ntemplate <int m>\nstruct is_static_modint<static_modint<m>>\
    \ : true_type {};\ntemplate <class T>\ninline constexpr bool is_static_modint_v\
    \ = is_static_modint<T>::value;\n\ntemplate <class T>\nstruct is_dynamic_modint\
    \ : false_type {};\ntemplate <int id>\nstruct is_dynamic_modint<dynamic_modint<id>>\
    \ : true_type {};\ntemplate <class T>\ninline constexpr bool is_dynamic_modint_v\
    \ = is_dynamic_modint<T>::value;\n\ntemplate <class T>\ninline constexpr bool\
    \ is_modint_v = is_static_modint_v<T> || is_dynamic_modint_v<T>;\n\ntemplate <typename,\
    \ typename = void>\nstruct has_mod : false_type {};\ntemplate <typename T>\nstruct\
    \ has_mod<T, void_t<decltype(declval<T>().mod)>> : true_type {};\n\ntemplate <class\
    \ mint>\nstruct modint_less\n{\n  bool operator()(const mint &a, const mint &b)\
    \ const\n  {\n    if constexpr (has_mod<mint>())\n      return a.val() < b.val();\n\
    \    else\n      return a < b;\n  }\n};\n\ntemplate <class mint>\nstruct modint_hash\n\
    {\n  auto operator()(const mint &x) const\n  {\n    if constexpr (has_mod<mint>())\n\
    \      return hash(x.val());\n    else\n      return hash(x);\n  }\n};\n#line\
    \ 2 \"math/crt.hpp\"\n\n#line 4 \"math/crt.hpp\"\n\n#line 7 \"math/crt.hpp\"\n\
    \n/**\n * @brief \u4E2D\u56FD\u5270\u4F59\u5B9A\u7406 (CRT)\n * @docs docs/math/crt.md\n\
    \ */\n\n// (\u89E3\u304C\u5B58\u5728\u3059\u308B\u304B, r, m)\ntemplate <class\
    \ T = ll, class R0, class R1, class M0, class M1>\nconstexpr tuple<bool, T, T>\
    \ crt2(R0 r0_, R1 r1_, M0 m0_, M1 m1_)\n{\n  T m0 = m0_, m1 = m1_;\n  assert(m0\
    \ >= 1 && m1 >= 1);\n  T r0 = safemod(r0_, m0), r1 = safemod(r1_, m1);\n  if (m0\
    \ < m1)\n    swap(r0, r1), swap(m0, m1);\n  if (m0 % m1 == 0)\n  {\n    if (r0\
    \ % m1 != r1)\n      return {false, 0, 0};\n    return {true, r0, m0};\n  }\n\
    \  auto [g, im, _] = extgcd<T>(m0, m1);\n  T u1 = m1 / g;\n  if ((r1 - r0) % g)\n\
    \    return {false, 0, 0};\n  T x = (r1 - r0) / g % u1 * im % u1;\n  r0 += x *\
    \ m0;\n  m0 *= u1;\n  if (r0 < 0)\n    r0 += m0;\n  return {true, r0, m0};\n}\n\
    \n// (\u89E3\u304C\u5B58\u5728\u3059\u308B\u304B, r, m)\ntemplate <class T = ll,\
    \ class V1, class V2>\nconstexpr tuple<bool, T, T> crt(const V1 &rs, const V2\
    \ &ms)\n{\n  assert(rs.size() == ms.size());\n  const int n = rs.size();\n  T\
    \ r = 0, m = 1;\n  repi(i, n)\n  {\n    auto [ok, nr, nm] = crt2<T>(r, rs[i],\
    \ m, ms[i]);\n    if (!ok)\n      return {false, 0, 0};\n    r = nr, m = nm;\n\
    \  }\n  return {true, r, m};\n}\n\n// (r, m)\n// ms[i] \u305F\u3061\u306F pairwise\
    \ coprime\n// T \u306F ms[i] \u306E\u578B\u306E 2 \u4E57\u304C\u53CE\u307E\u308B\
    \ (\u7B26\u53F7\u3064\u304D)\ntemplate <class mint, class T = ll, class V1, class\
    \ V2>\npair<mint, mint> crt_mod(const V1 &rs, const V2 &ms)\n{\n  assert(rs.size()\
    \ == ms.size());\n  const int n = rs.size();\n  mint r = 0, m = 1;\n  vc<T> rr(n,\
    \ 0), mm(n, 1);\n  repi(i, n)\n  {\n    // r = t[0] + t[1]m[0] + ... + t[i-1]m[0]...m[i-2]\
    \ mod mint::mod\n    // m = m[0]...m[i-1] mod mint::mod\n    // rr[i] = t[0] +\
    \ t[1]m[0] + ... + t[i-1]m[0]...m[i-2] mod m[i]\n    // mm[i] = m[0]...m[i-1]\
    \ mod m[i]\n    assert(ms[i] >= 1);\n    auto [g, im, _] = extgcd<T>(mm[i], ms[i]);\n\
    \    assert(g == 1);\n    T t = safemod((rs[i] % ms[i] - rr[i]) * im, ms[i]);\n\
    \    r += t * m, m *= ms[i];\n    repi(j, i + 1, n)\n    {\n      rr[j] += t *\
    \ mm[j] % ms[j];\n      if (rr[j] >= ms[j])\n        rr[j] -= ms[j];\n      mm[j]\
    \ *= ms[i], mm[j] %= ms[j];\n    }\n  }\n  return {r, m};\n}\n\n// (r, m)\n//\
    \ \u5F15\u6570\u306F array\n// ms[i] \u305F\u3061\u304C\u30B3\u30F3\u30D1\u30A4\
    \u30EB\u6642\u5B9A\u6570\u3067\u3042\u308B\u3053\u3068\u3092\u4EEE\u5B9A\n// ms[i]\
    \ \u305F\u3061\u306F pairwise coprime\n// 0 <= rs[i] < ms[i]\n// T \u306F ms[i]\
    \ \u306E\u578B\u306E 2 \u4E57\u304C\u53CE\u307E\u308B (\u7B26\u53F7\u3064\u304D\
    )\ntemplate <class mint, class T = ll, class U1, class U2, size_t n>\nconstexpr\
    \ pair<mint, mint> crt_mod_constexpr(const array<U1, n> &rs, const array<U2, n>\
    \ &ms)\n{\n  assert(rs.size() == ms.size());\n  mint r = 0, m = 1;\n  array<T,\
    \ n> rr{}, mm;\n  fill(ALL(mm), 1);\n  repi(i, n)\n  {\n    assert(ms[i] >= U2(1));\n\
    \    assert(U1(0) <= rs[i] && U2(rs[i]) < ms[i]);\n    auto [g, im, _] = extgcd<T>(mm[i],\
    \ ms[i]);\n    assert(g == 1);\n    T t = safemod((rs[i] - rr[i]) * im, ms[i]);\n\
    \    r += t * m, m *= ms[i];\n    repi(j, i + 1, n)\n    {\n      rr[j] += t *\
    \ mm[j] % ms[j];\n      if (rr[j] >= ms[j])\n        rr[j] -= ms[j];\n      mm[j]\
    \ *= ms[i], mm[j] %= ms[j];\n    }\n  }\n  return {r, m};\n}\n\n// (r, m)\n//\
    \ ms[i] \u305F\u3061\u306F pairwise coprime\n// ms[i] \u305F\u3061\u304C dynamic\
    \ 32 bit \u306E\u5834\u5408\u306B\u9AD8\u901F\u5316\u3057\u305F\u3082\u306E\n\
    template <class mint, class V1, class V2>\npair<mint, mint> crt_mod_dynamic_32(const\
    \ V1 &rs, const V2 &ms)\n{\n  assert(rs.size() == ms.size());\n  const int n =\
    \ rs.size();\n  mint r = 0, m = 1;\n  vc<internal::barrett32> ba;\n  ba.reserve(n);\n\
    \  repi(i, n) ba.eb(ms[i]);\n  vc<ll> rr(n, 0), mm(n, 1);\n  repi(i, n)\n  {\n\
    \    assert(ms[i] >= 1);\n    auto [g, im, _] = extgcd<ll>(mm[i], ms[i]);\n  \
    \  assert(g == 1);\n    if (im < 0)\n      im += ms[i];\n    ll t = ba[i].mul(safemod(rs[i]\
    \ - rr[i], ms[i]), im);\n    r += t * m, m *= ms[i];\n    repi(j, i + 1, n)\n\
    \    {\n      rr[j] += ba[j].mul(t, mm[j]);\n      if (rr[j] >= ms[j])\n     \
    \   rr[j] -= ms[j];\n      mm[j] = ba[j].mul(mm[j], ms[i]);\n    }\n  }\n  return\
    \ {r, m};\n}\n\n// \u7834\u58CA\u7684\u306B\u5909\u66F4\u3059\u308B\n// \u89E3\
    \u304C\u5B58\u5728\u3057\u306A\u3044\u306A\u3089 false \u3092\u8FD4\u3059\n//\
    \ \u89E3\u304C\u5B58\u5728\u3059\u308B\u306A\u3089 true \u3092\u8FD4\u3057\u3001\
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
    \ i++)\n    {\n      if (powmod32_constexpr(g, (m - 1) / divs[i], m) == 1)\n \
    \     {\n        ok = false;\n        break;\n      }\n    }\n    if (ok)\n  \
    \    return g;\n  }\n}\ntemplate <int m>\nconstexpr int primitive_root_for_convolution\
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
    \  {\n    if constexpr (!internal::isprime32<mint::mod()>)\n      return false;\n\
    \    static constexpr int rank2 = countr_zero(mint::mod() - 1);\n    return n\
    \ <= (1 << rank2);\n  }\n  else\n    return false;\n}\n\ntemplate <int id>\nvoid\
    \ ntt(vc<dynamic_modint<id>> &) { assert(false); }\ntemplate <int id>\nvoid intt(vc<dynamic_modint<id>>\
    \ &) { assert(false); }\n\n// \u7834\u58CA\u7684\u306B\u5909\u66F4\u3059\u308B\
    \ntemplate <int mod>\nvoid ntt(vc<static_modint<mod>> &a)\n{\n  using mint = static_modint<mod>;\n\
    \  int n = int(a.size());\n  int h = countr_zero((unsigned int)n);\n  assert(n\
    \ == (1 << h));\n\n  static const internal::fft_info<mint> info;\n\n  int len\
    \ = 0; // a[i, i+(n>>len), i+2*(n>>len), ..] is transformed\n  while (len < h)\n\
    \  {\n    if (h - len == 1)\n    {\n      int p = 1 << (h - len - 1);\n      mint\
    \ rot = 1;\n      for (int s = 0; s < (1 << len); s++)\n      {\n        int offset\
    \ = s << (h - len);\n        for (int i = 0; i < p; i++)\n        {\n        \
    \  auto l = a[i + offset];\n          auto r = a[i + offset + p] * rot;\n    \
    \      a[i + offset] = l + r;\n          a[i + offset + p] = l - r;\n        }\n\
    \        if (s + 1 != (1 << len))\n          rot *= info.rate2[countr_zero(~(unsigned\
    \ int)(s))];\n      }\n      len++;\n    }\n    else\n    {\n      // 4-base\n\
    \      int p = 1 << (h - len - 2);\n      mint rot = 1, imag = info.root[2];\n\
    \      for (int s = 0; s < (1 << len); s++)\n      {\n        mint rot2 = rot\
    \ * rot;\n        mint rot3 = rot2 * rot;\n        int offset = s << (h - len);\n\
    \        for (int i = 0; i < p; i++)\n        {\n          auto mod2 = 1ULL *\
    \ mint::mod() * mint::mod();\n          auto a0 = 1ULL * a[i + offset].val();\n\
    \          auto a1 = 1ULL * a[i + offset + p].val() * rot.val();\n          auto\
    \ a2 = 1ULL * a[i + offset + 2 * p].val() * rot2.val();\n          auto a3 = 1ULL\
    \ * a[i + offset + 3 * p].val() * rot3.val();\n          auto a1na3imag =\n  \
    \            1ULL * mint(a1 + mod2 - a3).val() * imag.val();\n          auto na2\
    \ = mod2 - a2;\n          a[i + offset] = a0 + a2 + a1 + a3;\n          a[i +\
    \ offset + 1 * p] = a0 + a2 + (2 * mod2 - (a1 + a3));\n          a[i + offset\
    \ + 2 * p] = a0 + na2 + a1na3imag;\n          a[i + offset + 3 * p] = a0 + na2\
    \ + (mod2 - a1na3imag);\n        }\n        if (s + 1 != (1 << len))\n       \
    \   rot *= info.rate3[countr_zero(~(unsigned int)(s))];\n      }\n      len +=\
    \ 2;\n    }\n  }\n}\n\n// \u7834\u58CA\u7684\u306B\u5909\u66F4\u3059\u308B\ntemplate\
    \ <int mod>\nvoid intt(vc<static_modint<mod>> &a)\n{\n  using mint = static_modint<mod>;\n\
    \  int n = int(a.size());\n  int h = countr_zero((unsigned int)n);\n  assert(n\
    \ == (1 << h));\n\n  static const internal::fft_info<mint> info;\n\n  int len\
    \ = h; // a[i, i+(n>>len), i+2*(n>>len), ..] is transformed\n  while (len)\n \
    \ {\n    if (len == 1)\n    {\n      int p = 1 << (h - len);\n      mint irot\
    \ = 1;\n      for (int s = 0; s < (1 << (len - 1)); s++)\n      {\n        int\
    \ offset = s << (h - len + 1);\n        for (int i = 0; i < p; i++)\n        {\n\
    \          auto l = a[i + offset];\n          auto r = a[i + offset + p];\n  \
    \        a[i + offset] = l + r;\n          a[i + offset + p] =\n             \
    \ (unsigned long long)(mint::mod() + l.val() - (uint)r.val()) *\n            \
    \  irot.val();\n          ;\n        }\n        if (s + 1 != (1 << (len - 1)))\n\
    \          irot *= info.irate2[countr_zero(~(unsigned int)(s))];\n      }\n  \
    \    len--;\n    }\n    else\n    {\n      // 4-base\n      int p = 1 << (h -\
    \ len);\n      mint irot = 1, iimag = info.iroot[2];\n      for (int s = 0; s\
    \ < (1 << (len - 2)); s++)\n      {\n        mint irot2 = irot * irot;\n     \
    \   mint irot3 = irot2 * irot;\n        int offset = s << (h - len + 2);\n   \
    \     for (int i = 0; i < p; i++)\n        {\n          auto a0 = 1ULL * a[i +\
    \ offset + 0 * p].val();\n          auto a1 = 1ULL * a[i + offset + 1 * p].val();\n\
    \          auto a2 = 1ULL * a[i + offset + 2 * p].val();\n          auto a3 =\
    \ 1ULL * a[i + offset + 3 * p].val();\n\n          auto a2na3iimag =\n       \
    \       1ULL *\n              mint((mint::mod() + a2 - a3) * iimag.val()).val();\n\
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
    \ vc<T> &a, const vc<T> &b, vc<array<T, k>> &cs)\n{\n  using mint = static_modint<mod>;\n\
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
    \ cntb) <= 300)\n      return internal::convolution_naive(a, b);\n    assert(ntt_ok<static_modint<469762049>>(n\
    \ + m - 1) && \"|a| + |b| - 1 <= 2^26\");\n    vc<ll> a_(n), b_(m);\n    repi(i,\
    \ n) a_[i] = a[i].val();\n    repi(j, m) b_[j] = b[j].val();\n    return internal::convolution_crt_mod<mint,\
    \ 469762049, 1811939329, 2013265921>(a_, b_);\n  }\n}\n\ntemplate <int mod = 998244353,\
    \ class T, typename = enable_if_t<is_integral<T>::value>>\nvc<T> convolution(const\
    \ vc<T> &a, const vc<T> &b)\n{\n  using mint = static_modint<mod>;\n  auto c =\
    \ convolution(vc<mint>(ALL(a)), vc<mint>(ALL(b)));\n  vc<T> c_(c.size());\n  repi(i,\
    \ c.size()) c_[i] = c[i].val();\n  return c_;\n}\n#line 2 \"math/modint/sqrt_mod.hpp\"\
    \n\n#line 2 \"math/modint/modint64.hpp\"\n\n#line 2 \"math/modint/modint64_internal.hpp\"\
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
    \ = dynamic_modint64<-1>;\n\ntemplate <class T>\nstruct is_static_modint64 : false_type\
    \ {};\ntemplate <int m>\nstruct is_static_modint64<static_modint64<m>> : true_type\
    \ {};\ntemplate <class T>\ninline constexpr bool is_static_modint64_v = is_static_modint64<T>::value;\n\
    \ntemplate <class T>\nstruct is_dynamic_modint64 : false_type {};\ntemplate <int\
    \ id>\nstruct is_dynamic_modint64<dynamic_modint64<id>> : true_type {};\ntemplate\
    \ <class T>\ninline constexpr bool is_dynamic_modint64_v = is_dynamic_modint64<T>::value;\n\
    \ntemplate <class T>\ninline constexpr bool is_modint64_v = is_static_modint64_v<T>\
    \ || is_dynamic_modint64_v<T>;\n#line 2 \"math/prime/large/primality_test.hpp\"\
    \n\n#line 6 \"math/prime/large/primality_test.hpp\"\n\n/**\n * @brief \u7D20\u6570\
    \u5224\u5B9A\n * @docs docs/math/prime/large/primality_test.md\n */\n\nnamespace\
    \ internal\n{\n\ntemplate <class mint, class Array>\nbool is_prime_impl(ll n,\
    \ const Array &bases)\n{\n  if (n <= 1)\n    return false;\n  if (n == 2 || n\
    \ == 7 || n == 61)\n    return true;\n  if (n % 2 == 0)\n    return false;\n \
    \ ll d = (n - 1) >> countr_zero(n - 1);\n  mint::set_mod(n);\n  for (ll a : bases)\n\
    \  {\n    ll t = d;\n    mint y = mint(a).pow(t);\n    while (t != n - 1 && y\
    \ != 1 && y != n - 1)\n    {\n      y *= y;\n      t <<= 1;\n    }\n    if (y\
    \ != n - 1 && t % 2 == 0)\n      return false;\n  }\n  return true;\n}\n\n}; //\
    \ namespace internal\n\nbool is_prime(ll n)\n{\n  static constexpr array<ll, 3>\
    \ bases32 = {2, 7, 61};\n  static constexpr array<ll, 7> bases64 = {2, 325, 9375,\
    \ 28178, 450775, 9780504, 1795265022};\n  if (n <= INT_MAX)\n  {\n    using mint\
    \ = dynamic_modint<INT_MIN>;\n    return internal::is_prime_impl<mint>(n, bases32);\n\
    \  }\n  else\n  {\n    using mint = dynamic_modint64_odd<INT_MIN>;\n    return\
    \ internal::is_prime_impl<mint>(n, bases64);\n  }\n}\n#line 7 \"math/modint/sqrt_mod.hpp\"\
    \n\n/**\n * @brief mod \u7D20\u6570 \u306E\u5E73\u65B9\u6839\n * @docs docs/math/modint/sqrt_mod.md\n\
    \ */\n\n// \u5E73\u65B9\u6839\u304C\u5B58\u5728\u3057\u306A\u3051\u308C\u3070\
    \ (false, 0)\n// \u5E73\u65B9\u6839\u304C\u5B58\u5728\u3059\u308C\u3070 (true,\
    \ \u5E73\u65B9\u6839\u306E\u3072\u3068\u3064)\n// \u5E73\u65B9\u6839\u306E\u3082\
    \u3046\u3072\u3068\u3064\u306F -1 \u500D\u3057\u305F\u3082\u306E\n// O(log^2 p)\n\
    template <class mint>\npair<bool, mint> sqrt_mod(mint n)\n{\n  auto p = mint::mod();\n\
    \  assert(is_prime(mint::mod()));\n  if (n == 0)\n    return {true, 0};\n  if\
    \ (p == 2)\n    return {true, n};\n  if (n.pow((p - 1) / 2) == -1)\n    return\
    \ {false, 0};\n  auto q = p - 1;\n  int s = 0;\n  while (q % 2 == 0)\n    q /=\
    \ 2, s++;\n  mint z;\n  do\n  {\n    z = randrange(0, p);\n  } while (z.pow((p\
    \ - 1) / 2) != -1);\n  int m = s;\n  mint c = z.pow(q), t = n.pow(q), r = n.pow((q\
    \ + 1) / 2);\n  while (t != 1)\n  {\n    int m2 = 1;\n    for (mint tmp = t *\
    \ t; tmp != 1; tmp *= tmp, m2++)\n      ;\n    mint b = c.pow(1 << (m - m2 - 1));\n\
    \    m = m2, c = b * b, t *= c, r *= b;\n  }\n  return {true, r};\n}\n#line 8\
    \ \"math/fps/fps.hpp\"\n\n/**\n * @brief \u5F62\u5F0F\u7684\u51AA\u7D1A\u6570\n\
    \ * @docs docs/math/fps/fps.md\n */\n\ntemplate <class mint>\nstruct FormalPowerSeries\
    \ : vc<mint>\n{\n  using F = FormalPowerSeries;\n  using vc<mint>::vc;\n  using\
    \ vc<mint>::operator=;\n  using vc<mint>::size;\n  using vc<mint>::empty;\n  using\
    \ vc<mint>::back;\n  using vc<mint>::pop_back;\n  using vc<mint>::begin;\n  using\
    \ vc<mint>::resize;\n  using vc<mint>::front;\n\n  FormalPowerSeries(const vc<mint>\
    \ &f) : vc<mint>(f) {}\n\n  int sz() const { return size(); }\n  void shrink()\n\
    \  {\n    while (!empty() && back() == 0)\n      pop_back();\n  }\n  mint get(int\
    \ i) const { return 0 <= i && i < sz() ? (*this)[i] : 0; }\n  F pre(int len) const\n\
    \  {\n    assert(len >= 0);\n    return F(begin(), begin() + min(sz(), len));\n\
    \  }\n  F rev(int d = -1) const\n  {\n    F res(*this);\n    if (d >= 0)\n   \
    \   res.resize(d);\n    reverse(ALL(res));\n    return res;\n  }\n  int cnt_nz()\
    \ const { return count_if(ALL(*this), LMD(x, x != 0)); }\n  tuple<bool, int, mint>\
    \ nz_front() const\n  {\n    repi(i, sz()) if ((*this)[i] != 0) return {true,\
    \ i, (*this)[i]};\n    return {false, -1, 0};\n  }\n  vc<pair<int, mint>> nz()\
    \ const\n  {\n    vc<pair<int, mint>> res;\n    repi(i, sz()) if ((*this)[i] !=\
    \ 0) res.eb(i, (*this)[i]);\n    return res;\n  }\n\n  mint eval(const mint &x)\
    \ const\n  {\n    mint res = 0;\n    repi(i, sz() - 1, -1, -1) res = res * x +\
    \ (*this)[i];\n    return res;\n  }\n\n  F operator-() const\n  {\n    F res(*this);\n\
    \    fem(a : res) a = -a;\n    return res;\n  }\n  F &operator*=(const mint &k)\n\
    \  {\n    fem(a : *this) a *= k;\n    return *this;\n  }\n  F operator*(const\
    \ mint &k) const { return F(*this) *= k; }\n  friend F operator*(const mint &k,\
    \ const F &f) { return f * k; }\n  F &operator/=(const mint &k)\n  {\n    *this\
    \ *= k.inv();\n    return *this;\n  }\n  F operator/(const mint &k) const { return\
    \ F(*this) /= k; }\n  F &operator+=(const F &g)\n  {\n    const int n = size(),\
    \ m = g.size();\n    resize(max(n, m));\n    repi(i, m)(*this)[i] += g[i];\n \
    \   return *this;\n  }\n  F operator+(const F &g) const { return F(*this) += g;\
    \ }\n  F &operator-=(const F &g)\n  {\n    const int n = size(), m = g.size();\n\
    \    resize(max(n, m));\n    repi(i, m)(*this)[i] -= g[i];\n    return *this;\n\
    \  }\n  F operator-(const F &g) const { return F(*this) -= g; }\n  F &operator*=(const\
    \ F &g) { return *this = *this * g; }\n  F operator*(const F &g) const { return\
    \ convolution(*this, g); }\n\n  F div_sparse_destructive(const F &g, int d = -1)\n\
    \  {\n    assert(g.get(0) != 0);\n    if (d < 0)\n      d = max(sz(), g.sz());\n\
    \    mint iv = g.front().inv();\n    auto gnz = g.nz();\n    resize(d);\n    repi(i,\
    \ d)\n    {\n      fec([j, b] : gnz)\n      {\n        if (j == 0)\n         \
    \ continue;\n        if (j > i)\n          break;\n        (*this)[i] -= (*this)[i\
    \ - j] * b;\n      }\n      (*this)[i] *= iv;\n    }\n    return pre(d);\n  }\n\
    \  F div_sparse(const F &g, int d = -1) const { return F(*this).div_sparse_destructive(g,\
    \ d); }\n\n  // \u5B9A\u6570\u9805\u304C\u975E\u96F6\n  F inv(int d = -1) const\n\
    \  {\n    assert(get(0) != 0);\n    if (d < 0)\n      d = sz();\n    if (cnt_nz()\
    \ <= 200)\n      return F{1}.div_sparse(*this, d);\n    F f, g2, g{front().inv()};\n\
    \    for (int m = 1; m < d; m *= 2)\n    {\n      if (ntt_ok<mint>(2 * m))\n \
    \     {\n        f = pre(2 * m), g2 = F(g);\n        f.resize(2 * m), ntt(f);\n\
    \        g2.resize(2 * m), ntt(g2);\n        repi(i, 2 * m) f[i] *= g2[i];\n \
    \       intt(f);\n        f >>= m;\n        f.resize(2 * m), ntt(f);\n       \
    \ repi(i, 2 * m) f[i] *= g2[i];\n        intt(f);\n        mint iz = mint(2 *\
    \ m).inv();\n        iz *= -iz;\n        repi(i, m) f[i] *= iz;\n        g.insert(g.end(),\
    \ f.begin(), f.begin() + m);\n      }\n      else\n        g = (g * mint(2) -\
    \ g * g * pre(2 * m)).pre(2 * m);\n    }\n    return g.pre(d);\n  }\n  F &operator/=(const\
    \ F &g)\n  {\n    if (g.cnt_nz() <= 200)\n    {\n      div_sparse_destructive(g);\n\
    \      return *this;\n    }\n    const int d = max(sz(), g.sz());\n    *this =\
    \ (*this * g.inv(d)).pre(d);\n    return *this;\n  }\n  F operator/(const F &g)\
    \ const { return F(*this) /= g; }\n\n  F div_poly(const F &g) const\n  {\n   \
    \ const int k = sz() - g.sz() + 1;\n    if (k <= 0)\n      return {};\n    return\
    \ (rev().pre(k) * g.rev().inv(k)).pre(k).rev();\n  }\n  pair<F, F> divmod(const\
    \ F &g) const\n  {\n    F q = div_poly(g);\n    const int l = sz() - q.sz();\n\
    \    F r = pre(l) - (q.pre(l) * g.pre(l)).pre(l);\n    r.shrink();\n    return\
    \ {q, r};\n  }\n  F operator%(const F &g) const { return divmod(g).second; }\n\
    \  F &operator%=(const F &g) { return *this = *this % g; }\n\n  // mod (x^n -\
    \ 1)\n  F circular_mod(int n) const\n  {\n    F res(n);\n    repi(i, sz()) res[i\
    \ % n] += (*this)[i];\n    return res;\n  }\n\n  F operator<<(int k) const\n \
    \ {\n    F res(sz() + k);\n    repi(i, sz()) res[i + k] = (*this)[i];\n    return\
    \ res;\n  }\n  F operator>>(int k) const\n  {\n    F res(max(0, sz() - k));\n\
    \    repi(i, sz() - k) res[i] = (*this)[i + k];\n    return res;\n  }\n  F &operator<<=(int\
    \ k) { return *this = *this << k; }\n  F &operator>>=(int k) { return *this =\
    \ *this >> k; }\n\n  // \u5FAE\u5206 sum[i=1..n] i*a[i] x^{i-1}\n  F diff() const\n\
    \  {\n    F res(max(0, sz() - 1));\n    repi(i, 1, size()) res[i - 1] = (*this)[i]\
    \ * i;\n    return res;\n  }\n  // \u7A4D\u5206 sum[i=0..n] a[i]/(i+1) * x^{i+1}\n\
    \  F integ() const\n  {\n    F res(sz() + 1);\n    repi(i, size()) res[i + 1]\
    \ = (*this)[i] * Binomial<mint>::inv(i + 1);\n    return res;\n  }\n  // \u5B9A\
    \u6570\u9805\u304C 1\n  F log(int d = -1) const\n  {\n    assert(get(0) == 1);\n\
    \    if (d < 0)\n      d = sz();\n    if (d == 0)\n      return {};\n    F f =\
    \ pre(d);\n    return (f.diff() * f.inv(d - 1)).pre(d - 1).integ();\n  }\n\n \
    \ // \u5FAE\u5206\u65B9\u7A0B\u5F0F a(x)f'(x) + b(x)f(x) = 0, [x^0]f(x) = 1 \u3092\
    \u6E80\u305F\u3059 f \u3092 d \u9805\u307E\u3067\u6C42\u3081\u308B\n  // \u5236\
    \u7D04: [x^0]a(x) = 1\n  // \u8A08\u7B97\u91CF: O( d * (a, b \u306E\u975E\u96F6\
    \u306E\u500B\u6570) )\n  static F diff_eq(const F &a, const F &b, int d)\n  {\n\
    \    assert(a.get(0) == 1);\n    assert(d >= 0);\n    if (d == 0)\n      return\
    \ {};\n    F f(d);\n    f[0] = 1;\n    auto anz = a.nz(), bnz = b.nz();\n    repi(k,\
    \ d - 1)\n    {\n      fec([i, ai] : anz)\n      {\n        if (0 <= k - i + 1)\n\
    \          f[k + 1] -= ai * (k - i + 1) * f[k - i + 1];\n      }\n      fec([j,\
    \ bj] : bnz)\n      {\n        if (0 <= k - j && k - j < k + 1)\n          f[k\
    \ + 1] -= bj * f[k - j];\n      }\n      f[k + 1] *= Binomial<mint>::inv(k + 1);\n\
    \    }\n    return f;\n  }\n  F exp_sparse(int d = -1) const\n  {\n    assert(get(0)\
    \ == 0);\n    if (d < 0)\n      d = sz();\n    return diff_eq(F{1}, -diff(), d);\n\
    \  }\n  // k < 0 \u306E\u3068\u304D\u306F\u5B9A\u6570\u9805\u304C\u975E\u96F6\n\
    \  F pow_sparse(ll k, int d = -1) const\n  {\n    if (d < 0)\n      d = sz();\n\
    \    auto [exi, d0, a0] = nz_front();\n    if (!exi)\n    {\n      assert(k >=\
    \ 0 && \"k < 0 but [x^0]f(x) == 0\");\n      F res(d);\n      if (k == 0 && d\
    \ > 0)\n        res[0] = 1;\n      return res;\n    }\n    mint ia0 = a0.inv();\n\
    \    F f = ((*this) >> d0) * ia0;\n    if (k >= 0)\n    {\n      F g = diff_eq(f,\
    \ -k * f.diff(), d - mul_limited(d0, k, d));\n      F h = (g * a0.pow(k)) << mul_limited(d0,\
    \ k, d);\n      return h.pre(d);\n    }\n    else\n    {\n      assert(d0 == 0\
    \ && \"k < 0 but [x^0]f(x) == 0\");\n      F g = diff_eq(f, -k * f.diff(), d);\n\
    \      F h = (g * ia0.pow(-k));\n      return h.pre(d);\n    }\n  }\n  // (\u5B58\
    \u5728\u3059\u308B\u304B, \u5E73\u65B9\u6839\u306E\u3072\u3068\u3064)\n  pair<bool,\
    \ F> sqrt_sparse(int d = -1) const\n  {\n    if (d < 0)\n      d = sz();\n   \
    \ auto [exi, d0, a0] = nz_front();\n    if (!exi)\n      return {true, F(d)};\n\
    \    if (d0 % 2 != 0)\n      return {false, {}};\n    if (d0 >= d)\n      return\
    \ {true, F(d)};\n    auto [ok, r] = sqrt_mod(a0);\n    if (!ok)\n      return\
    \ {false, {}};\n    mint i2 = Binomial<mint>::inv(2);\n    F f = ((*this) >> d0)\
    \ / a0;\n    F g = diff_eq(f, -i2 * f.diff(), d - d0 / 2);\n    return {true,\
    \ ((g * r) << (d0 / 2)).pre(d)};\n  }\n\n  // \u5B9A\u6570\u9805\u304C 0\n  F\
    \ exp(int d = -1) const\n  {\n    assert(get(0) == 0);\n    if (d < 0)\n     \
    \ d = sz();\n    if (ntt_ok<mint>(2 * d))\n    {\n      if (cnt_nz() <= 320)\n\
    \        return exp_sparse(d);\n      // https://arxiv.org/pdf/1301.5804.pdf\n\
    \      F f{1}, g{1};\n      F f2, g2, f3, q, s, h, u;\n      g2 = {0};\n     \
    \ for (int m = 1; m < d; m *= 2)\n      {\n        mint im = mint(m).inv(), i2m\
    \ = mint(2 * m).inv();\n        f2 = f, f2.resize(2 * m), ntt(f2);\n\n       \
    \ // a\n        f3 = f, ntt(f3);\n        repi(i, m) f3.at(i) *= g2.at(i);\n \
    \       intt(f3);\n        f3 >>= m / 2;\n        f3.resize(m), ntt(f3);\n   \
    \     repi(i, m) f3.at(i) *= g2.at(i);\n        intt(f3);\n        repi(i, m /\
    \ 2) f3.at(i) *= -im * im;\n        g.insert(g.end(), f3.begin(), f3.begin() +\
    \ m / 2);\n        g2 = g, g2.resize(2 * m), ntt(g2);\n\n        // b, c\n   \
    \     q = diff(), q.resize(2 * m), fill(q.begin() + m - 1, q.end(), 0);\n    \
    \    ntt(q);\n        repi(i, 2 * m) q.at(i) *= f2.at(i);\n        intt(q);\n\
    \        q = q.circular_mod(m);\n        repi(i, m) q.at(i) *= i2m;\n\n      \
    \  // d, e\n        q.resize(m + 1);\n        s = ((f.diff() - q) << 1).circular_mod(m);\n\
    \        s.resize(2 * m), ntt(s);\n        repi(i, 2 * m) s.at(i) *= g2.at(i);\n\
    \        intt(s);\n        repi(i, m) s.at(i) *= i2m;\n        s.resize(m);\n\n\
    \        // f, g\n        h = *this, h.resize(2 * m), s.resize(2 * m);\n     \
    \   u = (h - (s << (m - 1)).integ()) >> m;\n        ntt(u);\n        repi(i, 2\
    \ * m) u.at(i) *= f2.at(i);\n        intt(u);\n        repi(i, m) u.at(i) *= i2m;\n\
    \        u.resize(m);\n\n        // h\n        f.insert(f.end(), u.begin(), u.end());\n\
    \      }\n      return f.pre(d);\n    }\n    else\n    {\n      if (cnt_nz() <=\
    \ 3000)\n        return exp_sparse(d);\n      F f{1};\n      for (int m = 1; m\
    \ < d; m *= 2)\n      {\n        f = (f * (pre(2 * m) + F{1} - f.log(2 * m))).pre(2\
    \ * m);\n      }\n      return f.pre(d);\n    }\n  }\n  // k < 0 \u306E\u3068\u304D\
    \u306F\u5B9A\u6570\u9805\u304C\u975E\u96F6\n  F pow(ll k, int d = -1) const\n\
    \  {\n    if (ntt_ok<mint>(2 * d))\n    {\n      if (cnt_nz() <= 100)\n      \
    \  return pow_sparse(k, d);\n    }\n    else\n    {\n      if (cnt_nz() <= 1300)\n\
    \        return pow_sparse(k, d);\n    }\n    if (d < 0)\n      d = sz();\n  \
    \  if (k == 0)\n    {\n      F res(d);\n      res[0] = 1;\n      return res;\n\
    \    }\n    if (k < 0)\n    {\n      assert(get(0) != 0);\n      mint iv = get(0).inv();\n\
    \      F res = ((*this * iv).log(d) * mint(k)).exp(d);\n      return (res * iv.pow(-k)).pre(d);\n\
    \    }\n    repi(i, sz())\n    {\n      if ((*this)[i] != 0)\n      {\n      \
    \  F res = (((*this / (*this)[i]) >> i).log(d) * mint(k)).exp(d);\n        return\
    \ (res * (*this)[i].pow(k) << (i * k)).pre(d);\n      }\n      if (mul_limited(i\
    \ + 1, k, d) >= d)\n        return F(d);\n    }\n    return F(d);\n  }\n  pair<bool,\
    \ F> sqrt(int d = -1) const\n  {\n    if (cnt_nz() <= 200)\n      return sqrt_sparse(d);\n\
    \    if (d < 0)\n      d = sz();\n    auto [exi, d0, a0] = nz_front();\n    if\
    \ (!exi)\n      return {true, F(d)};\n    if (d0 % 2 != 0)\n      return {false,\
    \ {}};\n    if (d0 >= d)\n      return {true, F(d)};\n    auto [ok, r] = sqrt_mod(a0);\n\
    \    if (!ok)\n      return {false, {}};\n    mint i2 = Binomial<mint>::inv(2);\n\
    \    F f = ((*this) >> d0) / a0, g{1};\n    for (int m = 1; m < d; m *= 2)\n \
    \     g = (g + f.pre(2 * m) * g.inv(2 * m)) * i2;\n    return {true, ((g * r)\
    \ << (d0 / 2)).pre(d)};\n  }\n\n  F pow_mod(ll k, const F &g) const\n  {\n   \
    \ assert(k >= 0);\n    if (k == 0)\n      return F{1} % g;\n    if (k & 1)\n \
    \     return (*this) * pow_mod(k - 1, g) % g;\n    F h = pow_mod(k / 2, g);\n\
    \    return h * h % g;\n  }\n\n  // \u5404\u4FC2\u6570 a_n \u3092 n! \u3067\u5272\
    \u3063\u305F\u3082\u306E\n  F egf() const\n  {\n    F res(*this);\n    repi(i,\
    \ sz()) res[i] *= Binomial<mint>::finv(i);\n    return res;\n  }\n  // \u5404\u4FC2\
    \u6570 a_n \u306B n! \u3092\u304B\u3051\u305F\u3082\u306E\n  F ogf() const\n \
    \ {\n    F res(*this);\n    repi(i, sz()) res[i] *= Binomial<mint>::fac(i);\n\
    \    return res;\n  }\n\n  // (1 + cx^d) \u3092\u304B\u3051\u308B\n  F mul_bin_destructive(int\
    \ d, mint c)\n  {\n    resize(sz() + d);\n    rep(i, sz() - 1 - d, -1, -1)(*this)[i\
    \ + d] += (*this)[i] * c;\n    return *this;\n  }\n  // (1 + cx^d) \u3092\u304B\
    \u3051\u305F\u3082\u306E\n  F mul_bin(int d, mint c) const { return F(*this).mul_bin_destructive(d,\
    \ c); }\n  // (1 + cx^d) \u3067\u308F\u308B\n  F div_bin_destructive(int d, mint\
    \ c)\n  {\n    resize(sz() + d);\n    rep(i, sz() - d)(*this)[i + d] -= (*this)[i]\
    \ * c;\n    return *this;\n  }\n  F div_bin(int d, mint c) const { return F(*this).div_bin_destructive(d,\
    \ c); }\n};\n#line 2 \"math/fps/multipoint_evaluation_geo.hpp\"\n\n#line 4 \"\
    math/fps/multipoint_evaluation_geo.hpp\"\n\n#line 6 \"math/fps/multipoint_evaluation_geo.hpp\"\
    \n\n/**\n * @brief \u591A\u9805\u5F0F\u306E\u591A\u70B9\u8A55\u4FA1\uFF08\u8A55\
    \u4FA1\u70B9\u304C\u7B49\u6BD4\u6570\u5217\uFF09\n * @docs docs/math/fps/multipoint_evaluation_geo.md\n\
    \ */\n\n// i = 0, ..., m - 1 \u306B\u5BFE\u3059\u308B f(ar^i)\ntemplate <class\
    \ mint>\nvc<mint> multipoint_evaluation_geo(const FormalPowerSeries<mint> &f,\
    \ int m, const mint &a, const mint &r)\n{\n  using F = FormalPowerSeries<mint>;\n\
    \  assert(m >= 0);\n  if (m == 0)\n    return {};\n  if (r == 0)\n  {\n    vc<mint>\
    \ res(m, f.eval(0));\n    res[0] = f.eval(a);\n    return res;\n  }\n  const int\
    \ n = f.size();\n  auto calc_pw = [&](int k, const mint &x) -> F\n  {\n    F res(k,\
    \ 1);\n    repi(i, 1, k) res[i] = res[i - 1] * x;\n    return res;\n  };\n  auto\
    \ calc_tri = [&](int k, const F &pw) -> F\n  {\n    F res(k, 1);\n    repi(i,\
    \ k - 1) res[i + 1] = res[i] * pw[i];\n    return res;\n  };\n  F pwa = calc_pw(n,\
    \ a), pwr = calc_pw(n + m, r), pwir = calc_pw(max(n, m), r.inv());\n  F trir =\
    \ calc_tri(n + m, pwr), triir = calc_tri(max(n, m), pwir);\n  F s(n);\n  repi(i,\
    \ n) s[n - 1 - i] = f[i] * pwa[i] * triir[i];\n  F u = (s * trir).pre(n + m -\
    \ 1) >> (n - 1);\n  repi(i, m) u[i] *= triir[i];\n  return u;\n}\n#line 2 \"math/modint/inv_many.hpp\"\
    \n\n#line 4 \"math/modint/inv_many.hpp\"\n\n/**\n * @brief \u9006\u5143\u3092\u307E\
    \u3068\u3081\u3066\u6C42\u3081\u308B\n * @docs docs/math/modint/inv_many.md\n\
    \ */\n\ntemplate <class mint>\nvc<mint> inv_many(const vc<mint> &a)\n{\n  const\
    \ int n = a.size();\n  vc<mint> p(n + 1, 1);\n  repi(i, n) p[i + 1] = p[i] * a[i];\n\
    \  mint ip = p.back().inv();\n  vc<mint> res(n);\n  repi(i, n - 1, -1, -1)\n \
    \ {\n    res[i] = ip * p[i];\n    ip *= a[i];\n  }\n  return res;\n}\n#line 8\
    \ \"math/fps/interpolation_geo.hpp\"\n\n/**\n * @brief \u591A\u9805\u5F0F\u88DC\
    \u9593\uFF08\u8A55\u4FA1\u70B9\u304C\u7B49\u6BD4\u6570\u5217\uFF09\n * @docs docs/math/fps/interpolation_geo.md\n\
    \ */\n\n// i = 0, 1, ..., n-1 \u306B\u5BFE\u3057 f(ar^i) = y[i] \u3067\u3042\u308B\
    \u3068\u304D f (n-1 \u6B21) \u3092\u6C42\u3081\u308B\n// \u5236\u7D04: 1 <= i\
    \ < n \u306B\u5BFE\u3057\u3066 r^i != 1\n// https://noshi91.github.io/algorithm-encyclopedia/polynomial-interpolation-geometric#noredirect\n\
    template <class mint>\nFormalPowerSeries<mint> interpolation_geo(const mint &a,\
    \ const mint &r, const vc<mint> &ys)\n{\n  using F = FormalPowerSeries<mint>;\n\
    \  const int n = ys.size();\n  if (n == 0)\n    return {};\n  if (n == 1)\n  \
    \  return {ys[0]};\n  assert(a != 0 && r != 0);\n  vc<mint> pwr(n + 1, 1);\n \
    \ repi(i, 1, n + 1)\n  {\n    pwr[i] = pwr[i - 1] * r;\n    if (i < n)\n     \
    \ assert(pwr[i] != 1);\n  }\n  vc<mint> s(n + 1, 1), t(n + 1, 1), u(n, 1);\n \
    \ repi(i, 1, n + 1) s[i] = s[i - 1] * (1 - pwr[i]);\n  repi(i, n - 1) t[i + 1]\
    \ = t[i] * pwr[n - i - 2];\n  repi(i, n) u[i] = ipow(-1, i) * s[i] * s[n - 1 -\
    \ i] * t[i];\n  vc<mint> iu = inv_many(u);\n  F w(n);\n  repi(i, n) w[i] = ys[i]\
    \ * iu[i];\n  F prod(n + 1);\n  t[0] = 1;\n  repi(i, n) t[i + 1] = t[i] * pwr[i];\n\
    \  if (s[n] == 0)\n  {\n    prod[0] = 1;\n    prod[n] = ipow(-1, n) * t[n];\n\
    \  }\n  else\n  {\n    vc<mint> is = inv_many(s);\n    repi(i, n + 1) prod[i]\
    \ = ipow(-1, i) * t[i] * s[n] * is[i] * is[n - i];\n  }\n  F sum = multipoint_evaluation_geo(w,\
    \ n, mint(1), r);\n  F ans = (prod * sum).pre(n).rev();\n  mint ia = a.inv();\n\
    \  vc<mint> pwia(n, 1);\n  repi(i, 1, n) pwia[i] = pwia[i - 1] * ia;\n  repi(i,\
    \ n) ans[i] *= pwia[i];\n  return ans;\n}\n"
  code: "#pragma once\n\n#include \"../../template/template_all_but_modint.hpp\"\n\
    \n#include \"fps.hpp\"\n#include \"multipoint_evaluation_geo.hpp\"\n#include \"\
    ../modint/inv_many.hpp\"\n\n/**\n * @brief \u591A\u9805\u5F0F\u88DC\u9593\uFF08\
    \u8A55\u4FA1\u70B9\u304C\u7B49\u6BD4\u6570\u5217\uFF09\n * @docs docs/math/fps/interpolation_geo.md\n\
    \ */\n\n// i = 0, 1, ..., n-1 \u306B\u5BFE\u3057 f(ar^i) = y[i] \u3067\u3042\u308B\
    \u3068\u304D f (n-1 \u6B21) \u3092\u6C42\u3081\u308B\n// \u5236\u7D04: 1 <= i\
    \ < n \u306B\u5BFE\u3057\u3066 r^i != 1\n// https://noshi91.github.io/algorithm-encyclopedia/polynomial-interpolation-geometric#noredirect\n\
    template <class mint>\nFormalPowerSeries<mint> interpolation_geo(const mint &a,\
    \ const mint &r, const vc<mint> &ys)\n{\n  using F = FormalPowerSeries<mint>;\n\
    \  const int n = ys.size();\n  if (n == 0)\n    return {};\n  if (n == 1)\n  \
    \  return {ys[0]};\n  assert(a != 0 && r != 0);\n  vc<mint> pwr(n + 1, 1);\n \
    \ repi(i, 1, n + 1)\n  {\n    pwr[i] = pwr[i - 1] * r;\n    if (i < n)\n     \
    \ assert(pwr[i] != 1);\n  }\n  vc<mint> s(n + 1, 1), t(n + 1, 1), u(n, 1);\n \
    \ repi(i, 1, n + 1) s[i] = s[i - 1] * (1 - pwr[i]);\n  repi(i, n - 1) t[i + 1]\
    \ = t[i] * pwr[n - i - 2];\n  repi(i, n) u[i] = ipow(-1, i) * s[i] * s[n - 1 -\
    \ i] * t[i];\n  vc<mint> iu = inv_many(u);\n  F w(n);\n  repi(i, n) w[i] = ys[i]\
    \ * iu[i];\n  F prod(n + 1);\n  t[0] = 1;\n  repi(i, n) t[i + 1] = t[i] * pwr[i];\n\
    \  if (s[n] == 0)\n  {\n    prod[0] = 1;\n    prod[n] = ipow(-1, n) * t[n];\n\
    \  }\n  else\n  {\n    vc<mint> is = inv_many(s);\n    repi(i, n + 1) prod[i]\
    \ = ipow(-1, i) * t[i] * s[n] * is[i] * is[n - i];\n  }\n  F sum = multipoint_evaluation_geo(w,\
    \ n, mint(1), r);\n  F ans = (prod * sum).pre(n).rev();\n  mint ia = a.inv();\n\
    \  vc<mint> pwia(n, 1);\n  repi(i, 1, n) pwia[i] = pwia[i - 1] * ia;\n  repi(i,\
    \ n) ans[i] *= pwia[i];\n  return ans;\n}\n"
  dependsOn:
  - template/template_all_but_modint.hpp
  - template/template_types.hpp
  - template/template_rep.hpp
  - template/template_math.hpp
  - template/template_vector.hpp
  - template/template_algo.hpp
  - template/template_binsearch.hpp
  - template/template_bit.hpp
  - template/template_inout.hpp
  - template/template_dump.hpp
  - template/template_random.hpp
  - math/fps/fps.hpp
  - math/modint/binomial.hpp
  - math/convolution/convolution.hpp
  - math/modint/modint.hpp
  - math/modint/modint32_internal.hpp
  - math/modint/modint_base.hpp
  - math/extgcd.hpp
  - math/crt.hpp
  - math/modint/sqrt_mod.hpp
  - math/modint/modint64.hpp
  - math/modint/modint64_internal.hpp
  - math/prime/large/primality_test.hpp
  - math/fps/multipoint_evaluation_geo.hpp
  - math/modint/inv_many.hpp
  isVerificationFile: false
  path: math/fps/interpolation_geo.hpp
  requiredBy: []
  timestamp: '2026-03-14 04:11:27+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo/poly_interpolation_geo.test.cpp
documentation_of: math/fps/interpolation_geo.hpp
layout: document
redirect_from:
- /library/math/fps/interpolation_geo.hpp
- /library/math/fps/interpolation_geo.hpp.html
title: "\u591A\u9805\u5F0F\u88DC\u9593\uFF08\u8A55\u4FA1\u70B9\u304C\u7B49\u6BD4\u6570\
  \u5217\uFF09"
---
## 多項式補間（評価点が等比数列）

https://noshi91.github.io/algorithm-encyclopedia/polynomial-interpolation-geometric#noredirect

導出は ↑ を参照（結構手間）。

実装上は $r^n \equiv 1$ となる場合にちょっと注意。

#### interpolation_geo

```cpp
FormalPowerSeries<mint> interpolation_geo(mint a, mint r, vc<mint> ys)
```

$f(a r^i) = y_i \ (0 \leq i \leq n-1)$ を満たす、高々 $n-1$ 次の多項式 $f(x)$ を求める。

##### 制約

- $r^i \not\equiv 1 \ (1 \leq i \leq n-1)$

##### 計算量

- $O(n \log n)$
