#pragma once

#include "../../../template/template_all_but_modint.hpp"

#include "rectangle_sum.hpp"

/**
 * @brief 矩形加算 → 矩形和（オフライン）
 * @docs docs/ds/2d/offline/static_rectangle_add_rectangle_sum.md
 */

// rectangle add をすべてやったあとに rectangle sum をやる
// S: 整数や mint など (加減算および整数倍が可能)
// I: 座標の型
template <class S, class I>
struct StaticRectangleAddRectangleSum
{
private:
  LowerLeftSum<GroupAddSub<S>, I> rs1, rs2, rs3, rs4;
  vc<tuple<int, int, I, I>> qi_coef_rx_ry;
  int qn = 0;
  void lower_left_sum_internal(I rx, I ry, int qi, int coef)
  {
    rs1.rectangle_sum(rx, ry);
    rs2.rectangle_sum(rx, ry);
    rs3.rectangle_sum(rx, ry);
    rs4.rectangle_sum(rx, ry);
    qi_coef_rx_ry.eb(qi, coef, rx, ry);
  }

public:
  // [lx, ∞) × [ly, ∞) に重み w を加算
  void upper_right_add(I lx, I ly, const S &w)
  {
    rs1.point_add(lx, ly, w);
    rs2.point_add(lx, ly, w * ly);
    rs3.point_add(lx, ly, w * lx);
    rs4.point_add(lx, ly, w * lx * ly);
  }
  // [lx, rx) × [ly, ry) に重み w を加算
  void rectangle_add(I lx, I rx, I ly, I ry, const S &w)
  {
    upper_right_add(lx, ly, w);
    upper_right_add(lx, ry, -w);
    upper_right_add(rx, ly, -w);
    upper_right_add(rx, ry, w);
  }
  // (-∞, rx) × (-∞, ry) の重みの総和を答えるクエリを追加
  void lower_left_sum(I rx, I ry)
  {
    lower_left_sum_internal(rx, ry, qn, 1);
    qn++;
  }
  // [lx, rx) × [ly, ry) の重みの総和を答えるクエリを追加
  void rectangle_sum(I lx, I rx, I ly, I ry)
  {
    lower_left_sum_internal(lx, ly, qn, 1);
    lower_left_sum_internal(lx, ry, qn, -1);
    lower_left_sum_internal(rx, ly, qn, -1);
    lower_left_sum_internal(rx, ry, qn, 1);
    qn++;
  }
  // すべてのクエリに答える
  vc<S> run()
  {
    auto res1 = rs1.run(), res2 = rs2.run(), res3 = rs3.run(), res4 = rs4.run();
    vc<S> res(qn, 0);
    repi(i, res1.size())
    {
      auto [qi, coef, rx, ry] = qi_coef_rx_ry[i];
      res[qi] += res1[i] * rx * ry * coef;
      res[qi] -= res2[i] * rx * coef;
      res[qi] -= res3[i] * ry * coef;
      res[qi] += res4[i] * coef;
    }
    return res;
  }
};
