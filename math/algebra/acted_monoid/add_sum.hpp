#pragma once

#include "../../../template/template_all_but_modint.hpp"

#include "../../../ds/segtree/lazy_segtree.hpp"

/**
 * @brief 作用つきモノイド：加算・和取得
 * @docs docs/math/algebra/acted_monoid/add_sum.md
 */

template <class T>
struct ActedMonoidAddSum
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
  using F = T;
  static constexpr S mapping(F f, S x) { return {x.len, x.val + f * x.len}; }
  static constexpr F composition(F f, F g) { return f + g; }
  static constexpr F id() { return 0; }
};

#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(len, val);
#endif
