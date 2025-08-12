#pragma once

#include "../../../template/template_all_but_modint.hpp"

#include "rectangle_sum.hpp"

/**
 * @brief 矩形加算 → 点取得（オフライン）
 * @docs docs/ds/2d/offline/static_rectangle_add_point_get.md
 */

// G: 可換群
// I: 座標の型
template <class G, class I>
struct StaticRectangleAddPointGet
{
  using S = typename G::S;

private:
  LowerLeftSum<G, I> rs;

public:
  // [lx, rx) × [ly, ry) に重み w の長方形を追加
  void rectangle_add(I lx, I rx, I ly, I ry, const S &w)
  {
    S iw = G::inv(w);
    rs.point_add(lx, ly, w), rs.point_add(rx, ry, w);
    rs.point_add(lx, ry, iw), rs.point_add(rx, ly, iw);
  }
  // (x, y) を含む長方形の重み和を答えるクエリを追加
  void point_get(I x, I y) { rs.rectangle_sum(x + 1, y + 1); }
  // すべてのクエリにまとめて答える
  vc<S> run() { return rs.run(); }
};
