#pragma once

#include "internal_sparse_interval_tree.hpp"
#include "../internal_dump_content.hpp"

/**
 * @brief 疎双対セグメント木
 * @docs docs/ds/segtree/sparse_dual_segtree.md
 */

template <class A>
struct SparseDualSegmentTree
{
  using S = typename A::S;
  using F = typename A::F;

private:
  internal_sparse_interval_tree::Tree<A, false> tree;

public:
  // 長さ n、各点の初期値 initial_value の列を作る
  SparseDualSegmentTree(ll n, const S &initial_value) : tree(n, initial_value) {}

  // 少なくとも count ノード分の領域をあらかじめ確保する
  void reserve(int count) { tree.reserve(count); }

  // p 番目の値を x にする
  void set(ll p, const S &x) { tree.set(p, x); }

  // p 番目の値を返す
  S get(ll p) const { return tree.get(p); }

  // p 番目に作用 f を適用する
  void apply(ll p, const F &f) { tree.apply(p, f); }

  // [l, r) に作用 f を適用する
  void apply(ll l, ll r, const F &f) { tree.apply(l, r, f); }

  // 保持している区間を (左端, 右端, 一点の値) の順で返す
  vc<tuple<ll, ll, S>> content() const { return tree.content(); }
};

#ifdef LOCAL
namespace cpp_dump::_detail
{
  template <class A>
  inline string export_object_generic(
      const SparseDualSegmentTree<A> &value, const string &indent,
      size_t last_line_length, size_t current_depth, bool fail_on_newline,
      const export_command &command)
  {
    return export_named_content<library_dump_detail::IntervalFields>(
        value, [&]() { return value.content(); },
        indent, last_line_length, current_depth, fail_on_newline, command);
  }
}
#endif
