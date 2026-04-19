---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
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
    #define EPS 1e-11\n#endif\n\n#line 12 \"template/template_math.hpp\"\n\n#line\
    \ 2 \"utils/is_integral_ext.hpp\"\n\n#line 4 \"utils/is_integral_ext.hpp\"\n\n\
    /**\n * @brief $128$ \u30D3\u30C3\u30C8\u6574\u6570\u3092\u542B\u3081\u305F\u6574\
    \u6570\u5224\u5B9A\n * @docs docs/utils/is_integral_ext.md\n */\n\ntemplate <class\
    \ T>\nconstexpr bool is_integral_ext = is_integral_v<T> || is_same_v<T, i128>\
    \ || is_same_v<T, u128>;\n\ntemplate <class T>\nconstexpr bool is_signed_ext =\
    \ is_signed_v<T> || is_same_v<T, i128>;\n\ntemplate <class T>\nconstexpr bool\
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
    \ 'a') + (ai - 10));\n  return s;\n}\n#line 6 \"template/template_vector.hpp\"\
    \n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08vector\uFF09\n *\
    \ @docs docs/template/template_vector.md\n */\n\n#define ALL(a) (a).begin(), (a).end()\n\
    template <class T = ll, class V>\ninline T SZ(const V &x) { return x.size(); }\n\
    #define eb emplace_back\n\n#define LMD(x, fx) ([&](auto x) { return fx; })\ntemplate\
    \ <class F>\nauto gen_vec(int n, const F &f)\n{\n  vc<decltype(f(0))> res(n);\n\
    \  repi(i, n) res[i] = f(i);\n  return res;\n}\n#define GEN_VEC(n, i, fi) (gen_vec(n,\
    \ LMD(i, fi)))\n\n// https://qiita.com/Chippppp/items/13150f5e0ea99f444d97#%E5%A4%9A%E6%AC%A1%E5%85%83vector%E7%94%9F%E6%88%90%E9%96%A2%E6%95%B0\n\
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
    \  repi(x, n) if (!exists[x]) return x;\n  return n;\n}\n\n// (0, 1. ..., n-1)\
    \ \u306E\u9806\u5217\u304B\u5224\u5B9A\ntemplate <class I>\nbool is_permutation(const\
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
    \ 0}, {0, -1}};\nconst vpll DRULplane = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};\n"
  code: "#pragma once\n\n#ifndef INF\n#define INF 4'000'000'000'000'000'037LL\n#endif\n\
    \n#include \"template_types.hpp\"\n#include \"template_rep.hpp\"\n#include \"\
    template_vector.hpp\"\n\n/**\n * @brief \u30C6\u30F3\u30D7\u30EC\u30FC\u30C8\uFF08\
    \u30A2\u30EB\u30B4\u30EA\u30BA\u30E0\uFF09\n * @docs docs/template/template_algo.md\n\
    \ */\n\ntemplate <class V>\nauto SUM(const V &v)\n{\n  typename V::value_type\
    \ s{};\n  fec(vi : v) s += vi;\n  return s;\n}\ntemplate <class T, class V>\n\
    T SUM(const V &v)\n{\n  T s{};\n  fec(vi : v) s += vi;\n  return s;\n}\ntemplate\
    \ <class V>\nauto MAX(const V &v) { return *max_element(ALL(v)); }\ntemplate <class\
    \ V>\nauto MIN(const V &v) { return *min_element(ALL(v)); }\ntemplate <class I\
    \ = ll, class V>\nI ARGMAX(const V &v) { return max_element(ALL(v)) - v.begin();\
    \ }\ntemplate <class I = ll, class V>\nI ARGMIN(const V &v) { return min_element(ALL(v))\
    \ - v.begin(); }\n\ntemplate<class T = ll, class V>\nT mex(const V &a)\n{\n  int\
    \ n = a.size();\n  vector<bool> exists(n, false);\n  repi(i, n) if (0 <= a[i]\
    \ && a[i] < n) exists[a[i]] = true;\n  repi(x, n) if (!exists[x]) return x;\n\
    \  return n;\n}\n\n// (0, 1. ..., n-1) \u306E\u9806\u5217\u304B\u5224\u5B9A\n\
    template <class I>\nbool is_permutation(const vc<I> &p)\n{\n  const int n = p.size();\n\
    \  vc<bool> b(n, false);\n  repi(i, n)\n  {\n    if (!(0 <= p[i] && p[i] < n))\n\
    \      return false;\n    b[p[i]] = true;\n  }\n  return all_of(ALL(b), [](bool\
    \ bi)\n                { return bi; });\n}\n\ntemplate <class T = ll>\nvc<T> permid(const\
    \ int &n, const int &base_index = 0)\n{\n  vc<T> p(n);\n  repi(i, n) p[i] = i\
    \ + base_index;\n  return p;\n}\ntemplate <class T>\nvc<T> perminv(const vc<T>\
    \ &p)\n{\n  if (p.empty())\n    return {};\n  const int n = p.size();\n  vc<T>\
    \ q(MAX(p) + 1);\n  repi(i, n) if (p[i] >= 0) q[p[i]] = i;\n  return q;\n}\n//\
    \ a[p[i]] for all i\ntemplate <class T, class U>\nvc<T> permuted(const vc<T> &a,\
    \ const vc<U> &p)\n{\n  const int n = p.size();\n  vc<T> res(n);\n  repi(i, n)\n\
    \  {\n    assert(0 <= p[i] && p[i] < U(a.size()));\n    res[i] = a[p[i]];\n  }\n\
    \  return res;\n}\n// p[q[r[i]]] for all i \u306A\u3069\ntemplate <class T, class\
    \ U, class... Ts>\nvc<T> permuted(const vc<T> &p, const vc<U> &q, const vc<Ts>\
    \ &...rs)\n{\n  return permuted(permuted(p, q), rs...);\n}\n\ntemplate <class\
    \ V>\nV reversed(const V &v) { return V(v.rbegin(), v.rend()); }\n\n#if __cplusplus\
    \ < 202002L\ntemplate <class V, class... Args>\nV sorted(V v, Args&&... args)\n\
    {\n  sort(ALL(v), forward<Args>(args)...);\n  return v;\n}\n#else\ntemplate <class\
    \ V, class... Args>\nV sorted(V v, Args&&... args)\n{\n  ranges::sort(v, forward<Args>(args)...);\n\
    \  return v;\n}\n#endif\n\ntemplate <class V>\nvoid unique(V &v) { v.erase(std::unique(ALL(v)),\
    \ v.end()); }\ntemplate <class V>\nV uniqued(V v) { unique(v); return v; }\n\n\
    template <class V>\nvoid sortunique(V &v)\n{\n  sort(ALL(v));\n  unique(v);\n\
    }\ntemplate <class V>\nV sortuniqued(V v) { sortunique(v); return v; }\n\n// \u5F15\
    \u6570: vc<pair<T, U>>\n// \u8FD4\u308A\u5024: vc<pair<T, vc<U>>\n// T \u3054\u3068\
    \u306B U \u3092\u307E\u3068\u3081\u305F\u3082\u306E\n// T \u306F\u6BD4\u8F03\u53EF\
    \u80FD\u3067\u3042\u308B\u5FC5\u8981\u304C\u3042\u308B\ntemplate <class T, class\
    \ U>\nvc<pair<T, vc<U>>> sortuniqued_group(vc<pair<T, U>> v)\n{\n  stable_sort(ALL(v),\
    \ [&](cauto &p1, cauto &p2)\n              { return p1.first < p2.first; });\n\
    \  vc<pair<T, vc<U>>> res;\n  fec([x, y] : v)\n  {\n    if (res.empty() || res.back().first\
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
    \ 0}, {0, -1}};\nconst vpll DRULplane = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};\n"
  dependsOn: []
  isVerificationFile: false
  path: template/template_algo.hpp
  requiredBy: []
  timestamp: '1970-01-01 00:00:00+00:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
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
(1) auto SUM(V v)
(2) T SUM<T>(V v)
```

- (1) $v$ の要素の総和を、$v$ の要素の型で返す。
- (2) $v$ の要素の総和を、型 `T` の値として返す。

(1) の引数が `vc<int>` などのときはオーバーフローに注意する。(2) を使い `SUM<ll>(v)` などとする（か、そもそも `int` を使わない）こと。


##### 計算量

- $O(\lvert v \rvert)$

#### MAX, MIN

```cpp
(1) auto MAX(V v)
(2) auto MIN(V v)
```

それぞれ、$v$ の要素の最大値、最小値を（$v$ の要素の型で）返す。

##### 制約

- $\lvert v \rvert > 0$

##### 計算量

- $O(\lvert v \rvert)$

#### ARGMAX, ARGMIN

```cpp
(1) I=ll ARGMAX(V v)
(2) I=ll ARGMIN(V v)
```

それぞれ、$v$ の要素が最大値、最小値をとるインデックス（のうち最小のもの）を返す。$v$ が空のときは $0$ を返す。

##### 計算量

- $O(\lvert v \rvert)$

#### mex

```cpp
U mex(V v)
```

$v$ の要素の mex を（型 `U` の値として）返す。`U` はデフォルトでは `ll`。

$v$ の要素はすべて非負整数であることを仮定。負の数がある場合、それを無視した上で動作する。

##### 計算量

- $O(\lvert v \rvert)$

### 順列

#### is_permutation

```cpp
bool is_permutation(vc<I> p)
```

$p$ が $0, 1, \dots, \lvert p \rvert -1$ の順列かどうか判定する。

##### 計算量

- $O(\lvert p \rvert)$


#### permid

```cpp
vc<T = ll> permid(int n, int base_index = 0)
```

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
(1) vc<T> permuted(vc<T> a, vc<U> p)
(2) vc<T> permuted(vc<T> p, vc<T> q, vc<T> r, ...)
```

- (1)：配列 $a$ を配列 $p$ にしたがって並べ替えた配列を返す。すなわち、$(a_{p_0}, a_{p_1}, \dots, a_{p_{\lvert p \rvert-1}})$ を返す。特に、$p$ が順列の場合や $a, p$ ともに順列の場合の使用を想定している。
- (2)：$s_i = p_{q_{r_{\ddots i}}}$ となる $s$ を返す。（対称群の元の積）

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

#### sorted

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

#### sortuniqued_group

```cpp
vc<pair<T, vc<U>>> sortuniqued_group(vc<pair<T, U>> v)
```

$v$ の要素を `T` ごとにまとめる。

`map` でも似たようなことができるが、静的なときはこちらのほうが速いし、扱いやすくもあると思う。

##### 制約

- `T` は比較可能

##### 計算量

列の長さを $n$ として

- $O(n \log n)$

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
(1) vc<M::S> cuml(vc<M::S> v)
(2) vc<M::S> cumr(vc<M::S> v)
(3) vc<T> cumlsum(vc<T> v)
(4) vc<T> cumrsum(vc<T> v)
(5) vc<T> cumlmax(vc<T> v)
(6) vc<T> cumrmax(vc<T> v)
(7) vc<T> cumlmin(vc<T> v)
(8) vc<T> cumrmin(vc<T> v)
```

$\lvert v \rvert = n$ とする。

- (1)：`M` はモノイド。長さ $n + 1$ の配列を返す。$i \: (0 \leq i \leq n)$ 番目には、$v[0, i)$ に左から順に `M` の積演算を行った結果を返す。
- (2)：`M` はモノイド。長さ $n + 1$ の配列を返す。$i \: (0 \leq i \leq n)$ 番目には、$v[i, n)$ に右から順に `M` の積演算を行った結果を返す。
- (3), (4), (5), (6), (7), (8)：モノイドをそれぞれ `MonoidAdd<T>`, `MonoidMin<T>`, `MonoidMax<T>` としたもの。

例 (cumlsum)：$(3, 1, 4, 1) \to (0, 3, 4, 8, 9)$

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


### その他

#### dij4

```cpp
const vpll DRULgrid = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
const vpll DRULplane = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };
```

グリッドの $4$ 方向の探索で使う。

- `DRULgrid`：$(i, j)$ の $i$ を下方向、$j$ を右方向とする座標系において、`DRUL` の順になっている。
- `DRULplane`：$(x, y)$ の $x$ を右方向、$y$ を上方向とする座標系において、`DRUL` の順になっている。