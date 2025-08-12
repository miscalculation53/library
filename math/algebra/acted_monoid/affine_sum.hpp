#pragma once

#include "../../../template/template_all_but_modint.hpp"

#include "../../../ds/segtree/lazy_segtree.hpp"

/**
 * @brief 作用つきモノイド：一次関数作用・和取得
 * @docs docs/math/algebra/acted_monoid/affine_sum.md
 */

template <class T>
struct ActedMonoidAffineSum
{
  struct S
  {
    int len;
    T val;
    S() {}
    S(T val) : len(1), val(val) {}
    S(int len, T val) : len(len), val(val) {}
  };
  static constexpr S op(S a, S b) { return {a.len + b.len, a.val + b.val}; }
  static constexpr S e() { return {0, 0}; }
  struct F
  {
    T b, c;
    F() {}
    F(T b, T c) : b(b), c(c) {}
  };
  static constexpr S mapping(F f, S x) { return {x.len, f.b * x.val + f.c * x.len}; }
  static constexpr F composition(F f, F g) { return {f.b * g.b, f.b * g.c + f.c}; }
  static constexpr F id() { return {1, 0}; }
};

#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(len, val);
#endif
