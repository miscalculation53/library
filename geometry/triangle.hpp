#pragma once

#include "circle.hpp"

/**
 * @brief 三角形の外心・内心・外接円・内接円
 * @docs docs/geometry/triangle.md
 */

template <class T> optional<Point<geometry_field_t<T>>> circumcenter(const Point<T> &a, const Point<T> &b,
                                                                   const Point<T> &c,
                                                                   geometry_real_t<T> eps = EPS)
{
  using W = geometry_wide_t<T>; using F = geometry_field_t<T>;
  auto aw = a.template cast<W>(), u = b.template cast<W>() - aw, v = c.template cast<W>() - aw;
  W det = cross(u, v);
  if (geometry_detail::sign(det, eps) == 0) return nullopt;
  return a.template cast<F>() + (rot90(u) * norm2(v) - rot90(v) * norm2(u)) / (W(2) * det);
}
template <class T> optional<Circle<geometry_real_t<T>>> circumcircle(const Point<T> &a, const Point<T> &b,
                                                                   const Point<T> &c,
                                                                   geometry_real_t<T> eps = EPS)
{
  auto center = circumcenter(a, b, c, eps);
  if (!center) return nullopt;
  using R = geometry_real_t<T>;
  auto p = center->template cast<R>();
  return Circle<R>(p, distance(p, a.template cast<R>()));
}
template <class T> optional<Point<geometry_real_t<T>>> incenter(const Point<T> &a, const Point<T> &b,
                                                              const Point<T> &c,
                                                              geometry_real_t<T> eps = EPS)
{
  if (!orientation(a, b, c, eps)) return nullopt;
  using R = geometry_real_t<T>;
  R x = distance(b, c), y = distance(c, a), z = distance(a, b);
  return (a.template cast<R>() * x + b.template cast<R>() * y + c.template cast<R>() * z) / (x + y + z);
}
template <class T> optional<Circle<geometry_real_t<T>>> incircle(const Point<T> &a, const Point<T> &b,
                                                              const Point<T> &c,
                                                              geometry_real_t<T> eps = EPS)
{
  auto center = incenter(a, b, c, eps);
  if (!center) return nullopt;
  using R = geometry_real_t<T>;
  R r = geometry_detail::cast<R>(abs(cross(a, b, c))) / (distance(a, b) + distance(b, c) + distance(c, a));
  return Circle<R>(*center, r);
}
