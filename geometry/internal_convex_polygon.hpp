#pragma once

#include "polygon.hpp"

namespace geometry_detail
{
// 巡回する凸境界を、共線の中間点を省いた反時計回りの点列にそろえる。
template <class T> vector<Point<T>> normalize_convex_polygon(const vector<Point<T>> &input,
                                                           real_t<T> eps)
{
  vector<Point<T>> p;
  p.reserve(input.size());
  for (const auto &q : input) if (p.empty() || p.back() != q) p.push_back(q);
  if (p.size() > 1 && p.front() == p.back()) p.pop_back();
  if (p.size() <= 1) return p;
  int direction = 0;
  for (size_t i = 0; i < p.size(); ++i)
  {
    direction = orientation(p[i], p[(i + 1) % p.size()], p[(i + 2) % p.size()], eps);
    if (direction) break;
  }
  if (direction == 0)
  {
    auto mm = minmax_element(p.begin(), p.end());
    if (*mm.first == *mm.second) return {*mm.first};
    vector<Point<T>> ends{*mm.first, *mm.second};
    reorder_polygon(ends);
    return ends;
  }
  if (direction < 0) reverse(p.begin(), p.end());
  vector<Point<T>> corners;
  corners.reserve(p.size());
  for (size_t i = 0; i < p.size(); ++i)
    if (orientation(p[(i + p.size() - 1) % p.size()], p[i], p[(i + 1) % p.size()], eps))
      corners.push_back(p[i]);
  reorder_polygon(corners);
  return corners;
}
}
