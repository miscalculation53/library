#pragma once

#include "line.hpp"

/**
 * @brief 円の位置関係・交点・接線
 * @docs docs/geometry/circle.md
 */

enum class CircleRelation { Separate, ExternallyTangent, Intersecting, InternallyTangent, Contained, Coincident };
#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_ENUM(CircleRelation, CircleRelation::Separate, CircleRelation::ExternallyTangent,
                          CircleRelation::Intersecting, CircleRelation::InternallyTangent,
                          CircleRelation::Contained, CircleRelation::Coincident);
#endif

template <class T = ld> struct Circle
{
  Point<T> c{};
  T r{};
  Circle() = default;
  Circle(const Point<T> &c, const T &r) : c(c), r(r) { assert(T(0) <= r); }
  template <class U> explicit Circle(const Circle<U> &o) : Circle(Point<T>(o.c), geometry_detail::cast<T>(o.r)) {}
  template <class U> Circle<U> cast() const { return Circle<U>(*this); }
  friend istream &operator>>(istream &in, Circle &o)
  { in >> o.c; geometry_detail::read_scalar(in, o.r); assert(!in || T(0) <= o.r); return in; }
  friend ostream &operator<<(ostream &out, const Circle &o) { return out << o.c << ' ' << o.r; }
#if defined FAST_IO && !defined LOCAL
  friend void rd1(Circle &o) { rd1(o.c); geometry_detail::read_scalar(o.r); assert(T(0) <= o.r); }
  friend void wt1(const Circle &o) { using fastio::wt1; wt1(o.c); wt1(' '); wt1(o.r); }
#endif
  CPP_DUMP_DEFINE_DATA(c, r);
};

#ifdef LOCAL
namespace cpp_dump::_detail
{
template <class T> inline constexpr bool is_ostream<Circle<T>> = false;
}
#endif

template <class T> Location contains(const Circle<T> &c, const Point<T> &p, geometry_real_t<T> eps = EPS)
{
  using W = geometry_wide_t<T>;
  int s = geometry_detail::sign(distance2(c.c, p) - W(c.r) * W(c.r), eps);
  return s < 0 ? Location::Inside : s == 0 ? Location::Boundary : Location::Outside;
}
template <class T> CircleRelation circle_relation(const Circle<T> &c, const Circle<T> &d,
                                          geometry_real_t<T> eps = EPS)
{
  using W = geometry_wide_t<T>;
  W dd = distance2(c.c, d.c), sum = W(c.r) + W(d.r), diff = W(c.r) - W(d.r);
  if (almost_equal(c.c, d.c, eps) && geometry_detail::sign(diff, eps) == 0) return CircleRelation::Coincident;
  int outer = geometry_detail::sign(dd - sum * sum, eps);
  int inner = geometry_detail::sign(dd - diff * diff, eps);
  if (outer > 0) return CircleRelation::Separate;
  if (outer == 0) return CircleRelation::ExternallyTangent;
  if (inner > 0) return CircleRelation::Intersecting;
  if (inner == 0) return CircleRelation::InternallyTangent;
  return CircleRelation::Contained;
}
template <class T> bool intersects(const Circle<T> &c, const Circle<T> &d, geometry_real_t<T> eps = EPS)
{
  auto k = circle_relation(c, d, eps);
  return k != CircleRelation::Separate && k != CircleRelation::Contained;
}
template <class T> bool intersects(const Circle<T> &c, const Line<T> &l, geometry_real_t<T> eps = EPS)
{
  using W = geometry_wide_t<T>;
  auto f = geometry_detail::distance_fraction(l, c.c);
  return geometry_detail::sign(f.first - W(c.r) * W(c.r) * f.second, eps) <= 0;
}
template <class T> bool intersects(const Line<T> &l, const Circle<T> &c, geometry_real_t<T> eps = EPS)
{ return intersects(c, l, eps); }
template <class T> bool intersects(const Circle<T> &c, const Segment<T> &s, geometry_real_t<T> eps = EPS)
{
  using W = geometry_wide_t<T>;
  auto f = geometry_detail::distance_fraction(s, c.c);
  W rr = W(c.r) * W(c.r);
  return geometry_detail::sign(f.first - rr * f.second, eps) <= 0 &&
         geometry_detail::sign(max(distance2(c.c, s.a), distance2(c.c, s.b)) - rr, eps) >= 0;
}
template <class T> bool intersects(const Segment<T> &s, const Circle<T> &c, geometry_real_t<T> eps = EPS)
{ return intersects(c, s, eps); }

template <class T> Intersection<geometry_real_t<T>> intersection(const Circle<T> &c, const Line<T> &l,
                                                               geometry_real_t<T> eps = EPS)
{
  using R = geometry_real_t<T>; using W = geometry_wide_t<T>;
  auto f = geometry_detail::distance_fraction(l, c.c);
  W delta = W(c.r) * W(c.r) * f.second - f.first;
  int s = geometry_detail::sign(delta, eps);
  if (s < 0) return {};
  auto cr = c.template cast<R>();
  auto lr = l.template cast<R>();
  auto foot = projection(lr, cr.c);
  if (s == 0) return {IntersectionKind::Points, {foot}};
  R height2 = geometry_detail::cast<R>(delta) / geometry_detail::cast<R>(f.second);
  auto offset = lr.vec() * (sqrt(max(R(0), height2)) / norm(lr.vec()));
  return {IntersectionKind::Points, {foot - offset, foot + offset}};
}
template <class T> Intersection<geometry_real_t<T>> intersection(const Line<T> &l, const Circle<T> &c,
                                                               geometry_real_t<T> eps = EPS)
{ return intersection(c, l, eps); }
template <class T> Intersection<geometry_real_t<T>> intersection(const Circle<T> &c, const Segment<T> &s,
                                                               geometry_real_t<T> eps = EPS)
{
  using R = geometry_real_t<T>;
  if (!intersects(c, s, eps)) return {};
  if (s.a == s.b) return {IntersectionKind::Points, {s.a.template cast<R>()}};
  auto ans = intersection(c, Line<T>(s.a, s.b), eps);
  auto sr = s.template cast<R>();
  ans.points.erase(remove_if(ans.points.begin(), ans.points.end(), [&](const auto &p)
      { return !contains(sr, p, eps); }), ans.points.end());
  if (ans.points.empty()) ans.kind = IntersectionKind::Empty;
  return ans;
}
template <class T> Intersection<geometry_real_t<T>> intersection(const Segment<T> &s, const Circle<T> &c,
                                                               geometry_real_t<T> eps = EPS)
{ return intersection(c, s, eps); }
template <class T> Intersection<geometry_real_t<T>> intersection(const Circle<T> &c, const Circle<T> &d,
                                                               geometry_real_t<T> eps = EPS)
{
  using R = geometry_real_t<T>; using W = geometry_wide_t<T>;
  auto k = circle_relation(c, d, eps);
  if (k == CircleRelation::Separate || k == CircleRelation::Contained) return {};
  if (k == CircleRelation::Coincident)
  {
    if (c.r == T(0)) return {IntersectionKind::Points, {c.c.template cast<R>()}};
    return {IntersectionKind::Circle, {}};
  }
  auto cr = c.template cast<R>(), dr = d.template cast<R>();
  auto v = dr.c - cr.c;
  W dd = distance2(c.c, d.c);
  W numerator = dd + W(c.r) * W(c.r) - W(d.r) * W(d.r);
  R t = geometry_detail::cast<R>(numerator) / (R(2) * geometry_detail::cast<R>(dd));
  auto foot = cr.c + v * t;
  if (k == CircleRelation::ExternallyTangent || k == CircleRelation::InternallyTangent)
    return {IntersectionKind::Points, {foot}};
  // 4 d^2 h^2 = ((r+s)^2-d^2)(d^2-(r-s)^2)。整数で接触を分類してから平方根を取る。
  W sum = W(c.r) + W(d.r), diff = W(c.r) - W(d.r);
  R h2 = geometry_detail::cast<R>(sum * sum - dd) * geometry_detail::cast<R>(dd - diff * diff) /
         (R(4) * geometry_detail::cast<R>(dd));
  auto offset = rot90(v) * (sqrt(max(R(0), h2)) / norm(v));
  return {IntersectionKind::Points, {foot - offset, foot + offset}};
}

template <class T> struct Tangents
{
  bool infinite = false;
  vector<Line<T>> lines;
  CPP_DUMP_DEFINE_DATA(infinite, lines);
};

template <class T> Tangents<geometry_real_t<T>> tangents(const Circle<T> &c, const Point<T> &p,
                                                       geometry_real_t<T> eps = EPS)
{
  using R = geometry_real_t<T>; using W = geometry_wide_t<T>;
  Tangents<R> ans;
  auto cr = c.template cast<R>();
  auto pr = p.template cast<R>();
  if (c.r == T(0))
  {
    if (almost_equal(c.c, p, eps)) ans.infinite = true;
    else ans.lines.emplace_back(cr.c, pr);
    return ans;
  }
  W dd = distance2(c.c, p), rr = W(c.r) * W(c.r);
  int s = geometry_detail::sign(dd - rr, eps);
  if (s < 0) return ans;
  auto v = pr - cr.c;
  if (s == 0) { ans.lines.emplace_back(pr, pr + rot90(v)); return ans; }
  R d2 = geometry_detail::cast<R>(dd);
  auto foot = cr.c + v * (cr.r * cr.r / d2);
  auto off = rot90(v) * (cr.r * sqrt(geometry_detail::cast<R>(dd - rr)) / d2);
  ans.lines.emplace_back(pr, foot - off);
  ans.lines.emplace_back(pr, foot + off);
  return ans;
}
template <class T> Tangents<geometry_real_t<T>> tangents(const Point<T> &p, const Circle<T> &c,
                                                       geometry_real_t<T> eps = EPS)
{ return tangents(c, p, eps); }
template <class T> Tangents<geometry_real_t<T>> common_tangents(const Circle<T> &c, const Circle<T> &d,
                                                              geometry_real_t<T> eps = EPS)
{
  using R = geometry_real_t<T>; using W = geometry_wide_t<T>;
  Tangents<R> ans;
  if (almost_equal(c.c, d.c, eps))
  {
    ans.infinite = geometry_detail::sign(W(c.r) - W(d.r), eps) == 0;
    return ans;
  }
  auto cr = c.template cast<R>(), dr = d.template cast<R>();
  auto v = dr.c - cr.c;
  W dd = distance2(c.c, d.c);
  R d2 = geometry_detail::cast<R>(dd);
  auto append = [&](const Line<R> &l)
  {
    for (const auto &m : ans.lines) if (parallel(l, m, eps) && contains(l, m.a, eps)) return;
    ans.lines.push_back(l);
  };
  for (int side : {-1, 1})
  {
    W delta = W(c.r) - W(side) * W(d.r), rem = dd - delta * delta;
    int s = geometry_detail::sign(rem, eps);
    if (s < 0) continue;
    R h = s == 0 ? R(0) : sqrt(max(R(0), geometry_detail::cast<R>(rem)));
    for (int turn : {-1, 1})
    {
      auto normal = (v * geometry_detail::cast<R>(delta) + rot90(v) * (R(turn) * h)) / d2;
      auto p = cr.c + normal * cr.r;
      // 法線から方向を作ると、接点が一致する場合や半径ゼロでも直線を表せる。
      append(Line<R>(p, p + rot90(normal)));
      if (s == 0) break;
    }
  }
  return ans;
}

template <class T> geometry_real_t<T> distance(const Circle<T> &c, const Point<T> &p)
{
  using R = geometry_real_t<T>;
  return abs(distance(c.c, p) - geometry_detail::cast<R>(c.r));
}
template <class T> geometry_real_t<T> distance(const Point<T> &p, const Circle<T> &c) { return distance(c, p); }
template <class T> geometry_real_t<T> distance(const Circle<T> &c, const Line<T> &l)
{
  using R = geometry_real_t<T>;
  return max(R(0), distance(l, c.c) - geometry_detail::cast<R>(c.r));
}
template <class T> geometry_real_t<T> distance(const Line<T> &l, const Circle<T> &c) { return distance(c, l); }
template <class T> geometry_real_t<T> distance(const Circle<T> &c, const Segment<T> &s)
{
  using R = geometry_real_t<T>;
  R r = geometry_detail::cast<R>(c.r);
  return max({R(0), distance(s, c.c) - r, r - max(distance(c.c, s.a), distance(c.c, s.b))});
}
template <class T> geometry_real_t<T> distance(const Segment<T> &s, const Circle<T> &c) { return distance(c, s); }
template <class T> geometry_real_t<T> distance(const Circle<T> &c, const Circle<T> &d)
{
  using R = geometry_real_t<T>;
  R v = distance(c.c, d.c), r = geometry_detail::cast<R>(c.r), s = geometry_detail::cast<R>(d.r);
  return max({R(0), v - r - s, abs(r - s) - v});
}
