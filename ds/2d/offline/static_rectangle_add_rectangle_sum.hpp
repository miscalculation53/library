#pragma once

#include "../../../template/template_all_but_modint.hpp"

/**
 * @brief 矩形加算 → 矩形和（オフライン）
 * @docs docs/ds/2d/offline/static_rectangle_add_rectangle_sum.md
 */

// rectangle add をすべてやったあとに rectangle sum をやる
// G: 可換群
// I: 座標の型
template <class G, class I>
struct StaticRectangleAddRectangleSum
{
  using S = typename G::S;

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
    int qi;
    bool inv;
    bool operator<(const Q &rhs) const { return x < rhs.x; }
  };
  vc<P> ps;
  vc<Q> qs;
  vc<I> ys;
  int qn = 0;

  void lower_left_sum_internal(I rx, I ry, int qi, bool inv)
  {
    qs.push_back({rx, ry, qi, inv});
  }

public:
  // [lx, ∞) × [ly, ∞) の各単位領域に w を作用
  void upper_right_add(I lx, I ly, const S &w)
  {
    const S wx = pow_group<G>(w, lx);
    ps.push_back({lx, ly, {w, pow_group<G>(w, ly), wx, pow_group<G>(wx, ly)}});
    ys.push_back(ly);
  }
  // [lx, rx) × [ly, ry) の各単位領域に w を作用
  void rectangle_add(I lx, I rx, I ly, I ry, const S &w)
  {
    assert(lx <= rx && ly <= ry);
    const S iw = G::inv(w);
    upper_right_add(lx, ly, w);
    upper_right_add(lx, ry, iw);
    upper_right_add(rx, ly, iw);
    upper_right_add(rx, ry, w);
  }
  // (-∞, rx) × (-∞, ry) を群演算で集約するクエリを追加
  void lower_left_sum(I rx, I ry)
  {
    lower_left_sum_internal(rx, ry, qn, false);
    qn++;
  }
  // [lx, rx) × [ly, ry) を群演算で集約するクエリを追加
  void rectangle_sum(I lx, I rx, I ly, I ry)
  {
    assert(lx <= rx && ly <= ry);
    lower_left_sum_internal(lx, ly, qn, false);
    lower_left_sum_internal(lx, ry, qn, true);
    lower_left_sum_internal(rx, ly, qn, true);
    lower_left_sum_internal(rx, ry, qn, false);
    qn++;
  }
  // すべてのクエリに答える
  vc<S> run()
  {
    sort(ALL(ps)), sort(ALL(qs)), sortunique(ys);
    const V e{G::e(), G::e(), G::e(), G::e()};
    vc<V> fw(ys.size() + 1, e);
    vc<S> res(qn, G::e());
    int i = 0;
    fec(q : qs)
    {
      while (i < SZ(ps) && ps[i].x < q.x)
      {
        int j = LB(ys, ps[i].y) + 1;
        while (j < SZ(fw))
        {
          fw[j].a = G::op(fw[j].a, ps[i].v.a);
          fw[j].b = G::op(fw[j].b, ps[i].v.b);
          fw[j].c = G::op(fw[j].c, ps[i].v.c);
          fw[j].d = G::op(fw[j].d, ps[i].v.d);
          j += j & -j;
        }
        i++;
      }
      V s = e;
      int j = LB(ys, q.y);
      while (j > 0)
      {
        s.a = G::op(s.a, fw[j].a);
        s.b = G::op(s.b, fw[j].b);
        s.c = G::op(s.c, fw[j].c);
        s.d = G::op(s.d, fw[j].d);
        j -= j & -j;
      }
      S v = pow_group<G>(pow_group<G>(s.a, q.x), q.y);
      v = G::op(v, G::inv(pow_group<G>(s.b, q.x)));
      v = G::op(v, G::inv(pow_group<G>(s.c, q.y)));
      v = G::op(v, s.d);
      res[q.qi] = G::op(res[q.qi], q.inv ? G::inv(v) : v);
    }
    return res;
  }
};
