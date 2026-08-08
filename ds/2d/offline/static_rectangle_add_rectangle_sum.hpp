#pragma once

#include "../../../template/template_all_but_modint.hpp"

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
  struct V
  {
    S a, b, c, d;
  };
  struct P
  {
    I x, y;
    V v;
    bool operator<(const P &rhs) const { return x < rhs.x; }
  };
  struct Q
  {
    I x, y;
    int qi, coef;
    bool operator<(const Q &rhs) const { return x < rhs.x; }
  };
  vc<P> ps;
  vc<Q> qs;
  vc<I> ys;
  int qn = 0;

  void lower_left_sum_internal(I rx, I ry, int qi, int coef)
  {
    qs.push_back({rx, ry, qi, coef});
  }

public:
  // [lx, ∞) × [ly, ∞) に重み w を加算
  void upper_right_add(I lx, I ly, const S &w)
  {
    ps.push_back({lx, ly, {w, w * ly, w * lx, w * lx * ly}});
    ys.push_back(ly);
  }
  // [lx, rx) × [ly, ry) に重み w を加算
  void rectangle_add(I lx, I rx, I ly, I ry, const S &w)
  {
    assert(lx <= rx && ly <= ry);
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
    assert(lx <= rx && ly <= ry);
    lower_left_sum_internal(lx, ly, qn, 1);
    lower_left_sum_internal(lx, ry, qn, -1);
    lower_left_sum_internal(rx, ly, qn, -1);
    lower_left_sum_internal(rx, ry, qn, 1);
    qn++;
  }
  // すべてのクエリに答える
  vc<S> run()
  {
    sort(ALL(ps)), sort(ALL(qs)), sortunique(ys);
    const V zero{S(0), S(0), S(0), S(0)};
    vc<V> fw(ys.size() + 1, zero);
    vc<S> res(qn, S(0));
    int i = 0;
    fec(q : qs)
    {
      while (i < SZ(ps) && ps[i].x < q.x)
      {
        int j = LB(ys, ps[i].y) + 1;
        while (j < SZ(fw))
        {
          fw[j].a += ps[i].v.a;
          fw[j].b += ps[i].v.b;
          fw[j].c += ps[i].v.c;
          fw[j].d += ps[i].v.d;
          j += j & -j;
        }
        i++;
      }
      V s = zero;
      int j = LB(ys, q.y);
      while (j > 0)
      {
        s.a += fw[j].a;
        s.b += fw[j].b;
        s.c += fw[j].c;
        s.d += fw[j].d;
        j -= j & -j;
      }
      res[q.qi] += (s.a * q.x * q.y - s.b * q.x - s.c * q.y + s.d) * q.coef;
    }
    return res;
  }
};
