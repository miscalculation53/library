#pragma once

#include "../../algo/stable_argsort.hpp"
#include "../../algebra/algebra_basic_ops.hpp"
#include "../internal_dump_content.hpp"

/**
 * @brief 疎な区間更新の imos 法
 * @docs docs/ds/cumulative_sum/sparse_imos.md
 */

template <class G, class I = ll>
struct SparseImos
{
  using S = typename G::S;

private:
  I n;
  S initial_value;
  vc<pair<I, S>> changes;

public:
  SparseImos() : SparseImos(I(0)) {}

  // 長さ n、各点の初期値 initial_value の列を作る。G は可換群
  explicit SparseImos(I n, const S &initial_value = G::e())
      : n(n), initial_value(initial_value)
  {
    assert(0 <= n);
  }

  // [l, r) の各点に v を作用させる
  void add(I l, I r, const S &v)
  {
    assert(0 <= l && l <= r && r <= n);
    if (l == r) return;
    changes.eb(l, v);
    if (r != n) changes.eb(r, G::inv(v));
  }

  // 各区間を (左端, 右端, 一点の値) の順で返す
  vc<tuple<I, I, S>> content() const
  {
    auto order = stable_argsort(changes, [](const auto &change) { return change.first; });
    vc<tuple<I, I, S>> res;
    I left = 0;
    S value = initial_value;
    for (int i : order)
    {
      const auto &[right, delta] = changes[i];
      if (left < right)
      {
        res.eb(left, right, value);
        left = right;
      }
      value = G::op(value, delta);
    }
    if (left < n) res.eb(left, n, value);
    return res;
  }
};

#ifdef LOCAL
namespace cpp_dump::_detail
{
  template <class G, class I>
  inline string export_object_generic(
      const SparseImos<G, I> &value, const string &indent,
      size_t last_line_length, size_t current_depth, bool fail_on_newline,
      const export_command &command)
  {
    return export_named_content<library_dump_detail::IntervalFields>(
        value, [&]() { return value.content(); },
        indent, last_line_length, current_depth, fail_on_newline, command);
  }
}
#endif
