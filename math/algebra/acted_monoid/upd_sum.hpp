#pragma once

#include "../../../template/template_all_but_modint.hpp"

#include "../../../ds/segtree/lazy_segtree.hpp"

/**
 * @brief 作用つきモノイド：更新・和取得
 * @docs docs/math/algebra/acted_monoid/add_sum.md
 */

template <class T>
struct ActedMonoidUpdSum
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
  static constexpr S e() { return {0, T{}}; }
  struct F
  {
    bool is_id;
    T val;
    F() {}
    F(T val) : is_id(false), val(val) {}
    F(bool is_id, T val) : is_id(is_id), val(val) {}
  };
  static constexpr S mapping(F f, S x) { return f.is_id ? x : S{x.len, f.val * x.len}; }
  static constexpr F composition(F f, F g) { return f.is_id ? g : f; }
  static constexpr F id() { return {true, T{}}; }
};

#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(len, val);
#endif

