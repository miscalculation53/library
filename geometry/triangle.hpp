#pragma once

#include "circle.hpp"

/**
 * @brief 三角形の外心・内心・外接円・内接円
 * @docs docs/geometry/triangle.md
 */

template <class T> optional<Point<geometry_field_t<T>>> circumcenter(const Point<T> &a, const Point<T> &b,
                                                                   const Point<T> &c,
                                                                   ld eps = EPS)
{
  using F = geometry_field_t<T>;
  using W = conditional_t<is_integral_ext<T>, i128, F>;
  auto aw = a.template cast<W>(), u = b.template cast<W>() - aw, v = c.template cast<W>() - aw;
  W det = cross(u, v);
  if (geometry_detail::sign(det, eps) == 0) return nullopt;
  return a.template cast<F>() + (rot90(u) * norm2(v) - rot90(v) * norm2(u)) / (W(2) * det);
}
template <class R = ld, class T> optional<Circle<R>> circumcircle(const Point<T> &a, const Point<T> &b,
                                                                   const Point<T> &c,
                                                                   ld eps = EPS)
{
  if constexpr (!is_integral_ext<T> && !is_rational_v<T> && !is_same_v<T, R>)
    return circumcircle<R>(a.template cast<R>(), b.template cast<R>(), c.template cast<R>(), eps);
  auto center = circumcenter(a, b, c, eps);
  if (!center) return nullopt;
  auto p = center->template cast<R>();
  return Circle<R>(p, distance<R>(p, a.template cast<R>()));
}
template <class R = ld, class T> optional<Point<R>> incenter(const Point<T> &a, const Point<T> &b,
                                                              const Point<T> &c,
                                                              ld eps = EPS)
{
  if constexpr (!is_integral_ext<T> && !is_rational_v<T> && !is_same_v<T, R>)
    return incenter<R>(a.template cast<R>(), b.template cast<R>(), c.template cast<R>(), eps);
  if (!orientation(a, b, c, eps)) return nullopt;
  R x = distance<R>(b, c), y = distance<R>(c, a), z = distance<R>(a, b);
  return (a.template cast<R>() * x + b.template cast<R>() * y + c.template cast<R>() * z) / (x + y + z);
}
template <class R = ld, class T> optional<Circle<R>> incircle(const Point<T> &a, const Point<T> &b,
                                                              const Point<T> &c,
                                                              ld eps = EPS)
{
  if constexpr (!is_integral_ext<T> && !is_rational_v<T> && !is_same_v<T, R>)
    return incircle<R>(a.template cast<R>(), b.template cast<R>(), c.template cast<R>(), eps);
  auto center = incenter<R>(a, b, c, eps);
  if (!center) return nullopt;
  R r = geometry_detail::cast<R>(abs(cross(a, b, c))) /
        (distance<R>(a, b) + distance<R>(b, c) + distance<R>(c, a));
  return Circle<R>(*center, r);
}
