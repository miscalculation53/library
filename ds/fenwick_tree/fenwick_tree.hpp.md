---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    _deprecated_at_docs: docs/ds/fenwick_tree/fenwick_tree.md
    document_title: Fenwick Tree
    links: []
  bundledCode: "Traceback (most recent call last):\n  File \"/opt/hostedtoolcache/Python/3.12.0/x64/lib/python3.12/site-packages/onlinejudge_verify/documentation/build.py\"\
    , line 71, in _render_source_code_stat\n    bundled_code = language.bundle(stat.path,\
    \ basedir=basedir, options={'include_paths': [basedir]}).decode()\n          \
    \         ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\
    \  File \"/opt/hostedtoolcache/Python/3.12.0/x64/lib/python3.12/site-packages/onlinejudge_verify/languages/cplusplus.py\"\
    , line 187, in bundle\n    bundler.update(path)\n  File \"/opt/hostedtoolcache/Python/3.12.0/x64/lib/python3.12/site-packages/onlinejudge_verify/languages/cplusplus_bundle.py\"\
    , line 401, in update\n    self.update(self._resolve(pathlib.Path(included), included_from=path))\n\
    \                ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n \
    \ File \"/opt/hostedtoolcache/Python/3.12.0/x64/lib/python3.12/site-packages/onlinejudge_verify/languages/cplusplus_bundle.py\"\
    , line 260, in _resolve\n    raise BundleErrorAt(path, -1, \"no such header\"\
    )\nonlinejudge_verify.languages.cplusplus_bundle.BundleErrorAt: ../template/template_all_but_modint.hpp:\
    \ line -1: no such header\n"
  code: "#pragma once\n\n#include \"../template/template_all_but_modint.hpp\"\n\n\
    #include \"../math/algebra/algebra_basic_ops.hpp\"\n\n/**\n * @brief Fenwick Tree\n\
    \ * @docs docs/ds/fenwick_tree/fenwick_tree.md\n */\n\n// G \u306F\u53EF\u63DB\
    \u7FA4 (prefix \u3060\u3051\u306A\u3069\u3067\u3042\u308C\u3070\u53EF\u63DB\u30E2\
    \u30CE\u30A4\u30C9\u3067\u3082 OK)\ntemplate <class G>\nstruct FenwickTree\n{\n\
    \  using S = typename G::S;\n\nprivate:\n  int n;\n  vc<S> dat;\n\npublic:\n \
    \ FenwickTree() {}\n  FenwickTree(int n) : n(n), dat(n + 1, G::e()) {}\n  FenwickTree(const\
    \ vc<S> &v) : FenwickTree(v.size())\n  {\n    repi(i, n) add(i, v[i]);\n  }\n\n\
    \  template <class I = ll>\n  I size() const { return n; }\n\n  // [0, r)\n  //\
    \ \u9006\u5143\u306F\u5FC5\u8981\u306A\u3044\n  S sum(int r) const\n  {\n    assert(0\
    \ <= r && r <= n);\n    S s = G::e();\n    while (r > 0)\n    {\n      s = G::op(s,\
    \ dat[r]);\n      r -= r & -r;\n    }\n    return s;\n  }\n  // [l, r)\n  // \u9006\
    \u5143\u304C\u5FC5\u8981\n  S sum(int l, int r) const\n  {\n    assert(0 <= l\
    \ && l <= r && r <= n);\n    return G::op(G::inv(sum(l)), sum(r));\n  }\n  //\
    \ \u9006\u5143\u304C\u5FC5\u8981\n  S get(int i) const\n  {\n    assert(0 <= i\
    \ && i < n);\n    return sum(i, i + 1);\n  }\n\n  // \u9006\u5143\u306F\u5FC5\u8981\
    \u306A\u3044\n  void add(int i, S x)\n  {\n    assert(0 <= i && i < n);\n    i++;\n\
    \    while (i <= n)\n    {\n      dat[i] = G::op(dat[i], x);\n      i += i & -i;\n\
    \    }\n  }\n  // \u9006\u5143\u304C\u5FC5\u8981\n  void set(int i, S x) { add(i,\
    \ G::op(G::inv(get(i)), x)); }\n\n  // \u6574\u6570\u306E\u666E\u901A\u306E\u8DB3\
    \u3057\u7B97\u3067\u3001\u8981\u7D20\u304C\u975E\u8CA0\u306E\u3068\u304D\n  //\
    \ sum[0, r) >= w \u3068\u306A\u308B\u6700\u5C0F\u306E r (\u306A\u3051\u308C\u3070\
    \ n)\n  template <class I = ll>\n  I geq_min(S w) const\n  {\n    int k = bit_ceil(n);\n\
    \    int x = 0;\n    while (k > 0)\n    {\n      if (x + k - 1 < n && dat[x +\
    \ k] < w)\n      {\n        w = G::op(w, G::inv(dat[x + k]));\n        x += k;\n\
    \      }\n      k >>= 1;\n    }\n    return x;\n  }\n  // \u6574\u6570\u306E\u666E\
    \u901A\u306E\u8DB3\u3057\u7B97\u3067\u3001\u8981\u7D20\u304C\u975E\u8CA0\u306E\
    \u3068\u304D\n  // sum[0, r) < w \u3068\u306A\u308B\u6700\u5927\u306E r (\u306A\
    \u3051\u308C\u3070 -1)\n  template <class I = ll>\n  inline I lt_max(S w) const\
    \ { return geq_min<I>(w) - 1; }\n  // \u6574\u6570\u306E\u666E\u901A\u306E\u8DB3\
    \u3057\u7B97\u3067\u3001\u8981\u7D20\u304C\u975E\u8CA0\u306E\u3068\u304D\n  //\
    \ sum[0, r) > w \u3068\u306A\u308B\u6700\u5C0F\u306E r (\u306A\u3051\u308C\u3070\
    \ n)\n  template <class I = ll>\n  inline I gt_min(S w) const { return geq_min<I>(w\
    \ + 1); }\n  // \u6574\u6570\u306E\u666E\u901A\u306E\u8DB3\u3057\u7B97\u3067\u3001\
    \u8981\u7D20\u304C\u975E\u8CA0\u306E\u3068\u304D\n  // sum[0, r) <= w \u3068\u306A\
    \u308B\u6700\u5927\u306E r (\u306A\u3051\u308C\u3070 -1)\n  template <class I\
    \ = ll>\n  inline I leq_max(S w) const { return gt_min<I>(w) - 1; }\n\n  // \u8981\
    \u7D20\u304C [0, n) \u306E\u591A\u91CD\u96C6\u5408\u3092\u7BA1\u7406\u3059\u308B\
    \u306E\u306B\u4F7F\u3063\u305F\u3068\u304D\u3001k \u756A\u76EE\u306E\u5024 (\u306A\
    \u3051\u308C\u3070 n)\n  template <class I = ll>\n  inline I kth_of_multiset(S\
    \ k) const { return gt_min<I>(k); }\n\n  vc<S> content() const\n  {\n    vc<S>\
    \ res(n);\n    repi(i, n) res[i] = get(i);\n    return res;\n  }\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: ds/fenwick_tree/fenwick_tree.hpp
  requiredBy: []
  timestamp: '1970-01-01 00:00:00+00:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: ds/fenwick_tree/fenwick_tree.hpp
layout: document
redirect_from:
- /library/ds/fenwick_tree/fenwick_tree.hpp
- /library/ds/fenwick_tree/fenwick_tree.hpp.html
title: Fenwick Tree
---
## Fenwick Tree

可換群に対して一点加算・区間取得ができるデータ構造。（取得が prefix だけでよいなら群でなくモノイドでもよい）

できることはセグ木の下位互換だが、定数倍がよく、実装が軽い。

### コンストラクタ

```cpp
(1) FenwickTree<G>(int n)
(2) FenwickTree<G>(vc<G::S> v)
```

- (1) 長さ $n$、全要素単位元で初期化する。
- (2) vector $v$ で初期化する。

#### 制約

- `G` は**可換**
  - 群であることが多いが、モノイドでも一部機能は OK


### メンバ関数

#### size

```cpp
I size<I=ll>()
```

$n$ を返す。

##### 計算量

- $O(1)$


#### sum

```cpp
(1) G::S sum(int r)
(2) G::S sum(int l, int r)
```

- (1)：$[0, r)$ の和を返す。モノイドでよい。
- (2)：$[l, r)$ の和を返す。群であることが必要。

##### 制約

- (1)：$0 \leq r \leq n$
- (2)：$0 \leq l \leq r \leq n$

##### 計算量

- $O(\log n)$


#### get

```cpp
G::S get(int i)
```

$i$ 番目の値を返す。群であることが必要。

##### 制約

- $0 \leq i \lt n$

##### 計算量

- $O(\log n)$


#### add

```cpp
void add(int i, G::S x)
```

$i$ 番目の要素に $x$ を加算する。モノイドでよい。

##### 制約

- $0 \leq i \lt n$

##### 計算量

- $O(\log n)$


#### set

```cpp
void set(int i, G::S x)
```

$i$ 番目の要素を $x$ にする。群であることが必要。

##### 制約

- $0 \leq i \lt n$

##### 計算量

- $O(\log n)$


#### leq_max, lt_max, gt_min, geq_min

```cpp
(1) I leq_max<I=ll>(G::S w)
(2) I lt_max<I=ll>(G::S w)
(3) I gt_max<I=ll>(G::S w)
(4) I geq_max<I=ll>(G::S w)
```

`G` が整数の通常の足し算で、要素が非負であるとする。このとき、$[0, r)$ の要素の和に関して二分探索する：

- (1)：$\mathrm{sum}[0, r) \leq w$ となる $r$ の最大値（なければ $-1$）を返す。
- (2)：$\mathrm{sum}[0, r) \lt w$ となる $r$ の最大値（なければ $-1$）を返す。
- (3)：$\mathrm{sum}[0, r) \gt w$ となる $r$ の最大値（なければ $n$）を返す。
- (4)：$\mathrm{sum}[0, r) \geq w$ となる $r$ の最大値（なければ $n$）を返す。

##### 計算量

- $O(\log n)$


#### kth_of_multiset

```cpp
I kth_of_multiset<I=ll>(G::S k)
```

Fenwick Tree は $[0, n)$ の要素からなる多重集合を管理するのにも使える： `G` は整数の通常の足し算にして、要素 $i$ を $c$ 個追加（削除）するときに $i$ 番目の要素に $c$ を加算（減算）すればよい。（クエリ先読み + 座標圧縮と組み合わせることもしばしば）

このとき、「多重集合の $k$ 番目に小さい要素」を返す（なければ $n$ を返す）。なおこれは `gt_min` と一致する。

##### 計算量

- $O(\log n)$
