#pragma once

#include "internal_convex_polygon.hpp"

/**
 * @brief 凸多角形の Minkowski 和
 * @docs docs/geometry/minkowski_sum.md
 */

template <class T> vector<Point<T>> minkowski_sum(const vector<Point<T>> &p, const vector<Point<T>> &q,
                                                geometry_real_t<T> eps = EPS)
{
  if (p.empty() || q.empty()) return {};
  auto a = geometry_detail::normalize_convex_polygon(p, eps);
  auto b = geometry_detail::normalize_convex_polygon(q, eps);
  if (a.size() == 1)
  {
    for (auto &v : b) v += a[0];
    return b;
  }
  if (b.size() == 1)
  {
    for (auto &v : a) v += b[0];
    return a;
  }
  vector<Point<T>> result;
  result.reserve(a.size() + b.size());
  size_t i = 0, j = 0;
  while (i < a.size() || j < b.size())
  {
    result.push_back(a[i % a.size()] + b[j % b.size()]);
    if (i == a.size()) { ++j; continue; }
    if (j == b.size()) { ++i; continue; }
    using W = geometry_wide_t<T>;
    auto u = a[(i + 1) % a.size()].template cast<W>() - a[i].template cast<W>();
    auto v = b[(j + 1) % b.size()].template cast<W>() - b[j].template cast<W>();
    if (cross(u, v) == W(0) && dot(u, v) > W(0)) { ++i; ++j; }
    else if (polar_less(u, v)) ++i;
    else ++j;
  }
  return geometry_detail::normalize_convex_polygon(result, eps);
}
