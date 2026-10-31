#pragma once

#include "line.hpp"

/**
 * @brief 多角形の面積・内外判定・凸包・切断
 * @docs docs/geometry/polygon.md
 */

template <class T = ld> using Polygon = vector<Point<T>>;

template <class T> void reorder_polygon(vector<Point<T>> &p)
{
  auto first = min_element(p.begin(), p.end(), [](const Point<T> &a, const Point<T> &b)
      { return a.y < b.y || (!(b.y < a.y) && a.x < b.x); });
  rotate(p.begin(), first, p.end());
}

template <class T> geometry_wide_t<T> signed_area2(const vector<Point<T>> &p)
{
  geometry_wide_t<T> ans(0);
  for (size_t i = 0; i < p.size(); ++i) ans += cross(p[i], p[(i + 1) % p.size()]);
  return ans;
}
template <class T> geometry_wide_t<T> area2(const vector<Point<T>> &p) { return abs(signed_area2(p)); }
template <class T> geometry_field_t<T> signed_area(const vector<Point<T>> &p)
{ return geometry_detail::quotient(signed_area2(p), geometry_wide_t<T>(2)); }
template <class T> geometry_field_t<T> area(const vector<Point<T>> &p)
{ return geometry_detail::quotient(area2(p), geometry_wide_t<T>(2)); }
template <class T> geometry_real_t<T> perimeter(const vector<Point<T>> &p)
{
  geometry_real_t<T> ans(0);
  for (size_t i = 0; i < p.size(); ++i) ans += distance(p[i], p[(i + 1) % p.size()]);
  return ans;
}
template <class T> Location contains(const vector<Point<T>> &p, const Point<T> &q,
                                     geometry_real_t<T> eps = EPS)
{
  using W = geometry_wide_t<T>;
  int winding = 0;
  for (size_t i = 0; i < p.size(); ++i)
  {
    const auto &a = p[i], &b = p[(i + 1) % p.size()];
    if (contains(Segment<T>(a, b), q, eps)) return Location::Boundary;
    int ay = geometry_detail::sign(W(a.y) - W(q.y), eps);
    int by = geometry_detail::sign(W(b.y) - W(q.y), eps);
    int turn = orientation(a, b, q, eps);
    if (ay <= 0 && by > 0 && turn > 0) ++winding;
    if (by <= 0 && ay > 0 && turn < 0) --winding;
  }
  return winding ? Location::Inside : Location::Outside;
}
template <class T> bool is_convex(const vector<Point<T>> &p, geometry_real_t<T> eps = EPS)
{
  int direction = 0;
  for (size_t i = 0; i < p.size(); ++i)
  {
    int t = orientation(p[i], p[(i + 1) % p.size()], p[(i + 2) % p.size()], eps);
    if (t && direction && t != direction) return false;
    if (t) direction = t;
  }
  return true;
}

// 反時計回りの凸多角形。辺上の中間点を許し、隣接する頂点は異なるものとする。
template <class T> Location contains_convex(const vector<Point<T>> &p, const Point<T> &q,
                                           geometry_real_t<T> eps = EPS)
{
  size_t n = p.size();
  if (n > 1 && p.front() == p.back()) --n;
  if (n == 0) return Location::Outside;
  if (n <= 2)
    return contains(Segment<T>(p[0], p[n - 1]), q, eps) ? Location::Boundary : Location::Outside;
  if (almost_equal(p[0], q, eps)) return Location::Boundary;
  using W = geometry_wide_t<T>;
  auto origin = p[0].template cast<W>();
  auto same_ray = [&](const Point<T> &a, const Point<T> &b)
  {
    return orientation(p[0], a, b, eps) == 0 &&
           dot(a.template cast<W>() - origin, b.template cast<W>() - origin) > W(0);
  };
  int first = orientation(p[0], p[1], q, eps);
  int last = orientation(p[0], p[n - 1], q, eps);
  if (first < 0 || last > 0) return Location::Outside;
  if (first == 0 && same_ray(p[1], q))
  {
    size_t l = 1, r = n;
    while (l + 1 < r)
    {
      size_t m = (l + r) / 2;
      if (same_ray(p[1], p[m])) l = m;
      else r = m;
    }
    const auto &end = distance2(p[0], p[1]) < distance2(p[0], p[l]) ? p[l] : p[1];
    return contains(Segment<T>(p[0], end), q, eps) ? Location::Boundary : Location::Outside;
  }
  if (last == 0 && same_ray(p[n - 1], q))
  {
    size_t l = 0, r = n - 1;
    while (l + 1 < r)
    {
      size_t m = (l + r) / 2;
      if (same_ray(p[n - 1], p[m])) r = m;
      else l = m;
    }
    const auto &end = distance2(p[0], p[r]) < distance2(p[0], p[n - 1]) ? p[n - 1] : p[r];
    return contains(Segment<T>(p[0], end), q, eps) ? Location::Boundary : Location::Outside;
  }
  size_t l = 1, r = n - 1;
  while (l + 1 < r)
  {
    size_t m = (l + r) / 2;
    if (orientation(p[0], p[m], q, eps) >= 0) l = m;
    else r = m;
  }
  if (contains(Segment<T>(p[l], p[r]), q, eps)) return Location::Boundary;
  return orientation(p[0], p[l], p[r], eps) > 0 && orientation(p[l], p[r], q, eps) > 0
      ? Location::Inside : Location::Outside;
}

template <class T> vector<Point<T>> convex_hull(vector<Point<T>> p, bool keep_collinear = false,
                                               geometry_real_t<T> eps = EPS)
{
  sort(p.begin(), p.end());
  p.erase(unique(p.begin(), p.end()), p.end());
  if (p.size() <= 2) return p;
  bool collinear = true;
  for (size_t i = 1; i + 1 < p.size(); ++i)
    if (orientation(p.front(), p.back(), p[i], eps)) { collinear = false; break; }
  if (collinear) return keep_collinear ? p : vector<Point<T>>{p.front(), p.back()};
  vector<Point<T>> lower, upper;
  auto append = [&](vector<Point<T>> &chain, const Point<T> &v)
  {
    while (chain.size() >= 2)
    {
      int turn = orientation(chain[chain.size() - 2], chain.back(), v, eps);
      if (turn > 0 || (keep_collinear && turn == 0)) break;
      chain.pop_back();
    }
    chain.push_back(v);
  };
  for (const auto &v : p) append(lower, v);
  for (auto it = p.rbegin(); it != p.rend(); ++it) append(upper, *it);
  lower.pop_back(); upper.pop_back();
  lower.insert(lower.end(), upper.begin(), upper.end());
  return lower;
}
template <class T> vector<Point<geometry_field_t<T>>> convex_cut(const vector<Point<T>> &p, const Line<T> &l,
                                                               geometry_real_t<T> eps = EPS)
{
  using F = geometry_field_t<T>;
  vector<Point<F>> ans;
  auto append = [&](const Point<F> &q)
  { if (ans.empty() || !almost_equal(ans.back(), q, eps)) ans.push_back(q); };
  for (size_t i = 0; i < p.size(); ++i)
  {
    const auto &a = p[i], &b = p[(i + 1) % p.size()];
    int x = orientation(l.a, l.b, a, eps), y = orientation(l.a, l.b, b, eps);
    if (x >= 0) append(a.template cast<F>());
    if (x * y < 0) append(geometry_detail::line_crossing(l.a, l.b, a, b));
  }
  if (ans.size() > 1 && almost_equal(ans.front(), ans.back(), eps)) ans.pop_back();
  return ans;
}
