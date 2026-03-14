#pragma once

#include "../../../template/template_all_but_modint.hpp"

#include "../../segtree/lazy_segtree.hpp"
#include "../../../math/algebra/acted_monoid/add_min_count.hpp"

/**
 * @brief 長方形の和集合の面積
 * @docs docs/ds/2d/offline/area_of_union_of_rectangles.md
 */

// T: 答えの型
template <class T = ll>
struct AreaOfUnionOfRectangles
{
private:
  vc<T> ys;
  vc<tuple<T, T, T, T>> qs;

public:
  void rectangle_add(T lx, T rx, T ly, T ry)
  {
    ys.eb(ly), ys.eb(ry);
    qs.eb(lx, ly, ry, 1), qs.eb(rx, ly, ry, -1);
  }
  T run()
  {
    sortunique(ys), sort(ALL(qs));
    using AM = ActedMonoidAddMinCount<T, T>;
    vc<typename AM::S> vec(ys.size(), {0, 0});
    repi(i, SZ(ys) - 1) vec[i] = {0, ys[i + 1] - ys[i]};
    LazySegmentTree<AM> seg(vec);
    T px = 0, ans = 0;
    fec([x, ly, ry, c] : qs)
    {
      auto res = seg.all_prod();
      T tmp = (ys.back() - ys.front()) - (res.mn == 0 ? res.cnt : 0);
      ans += (x - px) * tmp;
      int l = LB(ys, ly), r = LB(ys, ry);
      seg.apply(l, r, c);
      px = x;
    }
    return ans;
  }
};
