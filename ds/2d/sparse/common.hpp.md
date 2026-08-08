---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':x:'
    path: ds/2d/sparse/fenwick_tree.hpp
    title: "$2$ \u6B21\u5143 Fenwick Tree\uFF08\u758E\u3001\u30AF\u30A8\u30EA\u70B9\
      \u5148\u8AAD\u307F\uFF09"
  - icon: ':x:'
    path: ds/2d/sparse/segtree.hpp
    title: "$2$ \u6B21\u5143\u30BB\u30B0\u30E1\u30F3\u30C8\u6728\uFF08\u758E\u3001\
      \u30AF\u30A8\u30EA\u70B9\u5148\u8AAD\u307F\uFF09"
  _extendedVerifiedWith:
  - icon: ':x:'
    path: verify/yosupo/2d_fenwick_tree.test.cpp
    title: verify/yosupo/2d_fenwick_tree.test.cpp
  - icon: ':x:'
    path: verify/yosupo/2d_segtree.test.cpp
    title: verify/yosupo/2d_segtree.test.cpp
  - icon: ':x:'
    path: verify/yosupo/static_range_sum_with_upper_bound_2d_seg.test.cpp
    title: verify/yosupo/static_range_sum_with_upper_bound_2d_seg.test.cpp
  _isVerificationFailed: true
  _pathExtension: hpp
  _verificationStatusIcon: ':x:'
  attributes:
    links: []
  bundledCode: "#line 2 \"ds/2d/sparse/common.hpp\"\n\nnamespace sparse_2d\n{\ntemplate\
    \ <class DS>\nauto range_sum_impl(const DS &ds, int l, int r, int)\n    -> decltype(ds.sum(l,\
    \ r))\n{\n  return ds.sum(l, r);\n}\n\ntemplate <class DS>\nauto range_sum_impl(const\
    \ DS &ds, int l, int r, long)\n    -> decltype(ds.prod(l, r))\n{\n  return ds.prod(l,\
    \ r);\n}\n\ntemplate <class DS>\nauto range_sum(const DS &ds, int l, int r)\n\
    \    -> decltype(range_sum_impl(ds, l, r, 0))\n{\n  return range_sum_impl(ds,\
    \ l, r, 0);\n}\n\ntemplate <class DS>\nauto prefix_sum_impl(const DS &ds, int\
    \ r, int)\n    -> decltype(ds.sum(r))\n{\n  return ds.sum(r);\n}\n\ntemplate <class\
    \ DS>\nauto prefix_sum_impl(const DS &ds, int r, long)\n    -> decltype(range_sum(ds,\
    \ 0, r))\n{\n  return range_sum(ds, 0, r);\n}\n\ntemplate <class DS>\nauto prefix_sum(const\
    \ DS &ds, int r)\n    -> decltype(prefix_sum_impl(ds, r, 0))\n{\n  return prefix_sum_impl(ds,\
    \ r, 0);\n}\n\ntemplate <class M, class DS>\nauto point_add_impl(DS &ds, int p,\
    \ const typename M::S &x, int)\n    -> decltype(ds.add(p, x), void())\n{\n  ds.add(p,\
    \ x);\n}\n\ntemplate <class M, class DS>\nauto point_add_impl(DS &ds, int p, const\
    \ typename M::S &x, long)\n    -> decltype(ds.set(p, M::op(ds.get(p), x)), void())\n\
    {\n  ds.set(p, M::op(ds.get(p), x));\n}\n\ntemplate <class M, class DS>\nvoid\
    \ point_add(DS &ds, int p, const typename M::S &x)\n{\n  point_add_impl<M>(ds,\
    \ p, x, 0);\n}\n} // namespace sparse_2d\n"
  code: "#pragma once\n\nnamespace sparse_2d\n{\ntemplate <class DS>\nauto range_sum_impl(const\
    \ DS &ds, int l, int r, int)\n    -> decltype(ds.sum(l, r))\n{\n  return ds.sum(l,\
    \ r);\n}\n\ntemplate <class DS>\nauto range_sum_impl(const DS &ds, int l, int\
    \ r, long)\n    -> decltype(ds.prod(l, r))\n{\n  return ds.prod(l, r);\n}\n\n\
    template <class DS>\nauto range_sum(const DS &ds, int l, int r)\n    -> decltype(range_sum_impl(ds,\
    \ l, r, 0))\n{\n  return range_sum_impl(ds, l, r, 0);\n}\n\ntemplate <class DS>\n\
    auto prefix_sum_impl(const DS &ds, int r, int)\n    -> decltype(ds.sum(r))\n{\n\
    \  return ds.sum(r);\n}\n\ntemplate <class DS>\nauto prefix_sum_impl(const DS\
    \ &ds, int r, long)\n    -> decltype(range_sum(ds, 0, r))\n{\n  return range_sum(ds,\
    \ 0, r);\n}\n\ntemplate <class DS>\nauto prefix_sum(const DS &ds, int r)\n   \
    \ -> decltype(prefix_sum_impl(ds, r, 0))\n{\n  return prefix_sum_impl(ds, r, 0);\n\
    }\n\ntemplate <class M, class DS>\nauto point_add_impl(DS &ds, int p, const typename\
    \ M::S &x, int)\n    -> decltype(ds.add(p, x), void())\n{\n  ds.add(p, x);\n}\n\
    \ntemplate <class M, class DS>\nauto point_add_impl(DS &ds, int p, const typename\
    \ M::S &x, long)\n    -> decltype(ds.set(p, M::op(ds.get(p), x)), void())\n{\n\
    \  ds.set(p, M::op(ds.get(p), x));\n}\n\ntemplate <class M, class DS>\nvoid point_add(DS\
    \ &ds, int p, const typename M::S &x)\n{\n  point_add_impl<M>(ds, p, x, 0);\n\
    }\n} // namespace sparse_2d\n"
  dependsOn: []
  isVerificationFile: false
  path: ds/2d/sparse/common.hpp
  requiredBy:
  - ds/2d/sparse/fenwick_tree.hpp
  - ds/2d/sparse/segtree.hpp
  timestamp: '2026-08-08 18:27:38+09:00'
  verificationStatus: LIBRARY_ALL_WA
  verifiedWith:
  - verify/yosupo/static_range_sum_with_upper_bound_2d_seg.test.cpp
  - verify/yosupo/2d_segtree.test.cpp
  - verify/yosupo/2d_fenwick_tree.test.cpp
documentation_of: ds/2d/sparse/common.hpp
layout: document
redirect_from:
- /library/ds/2d/sparse/common.hpp
- /library/ds/2d/sparse/common.hpp.html
title: ds/2d/sparse/common.hpp
---
