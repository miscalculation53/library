#pragma once

#include "point.hpp"

/**
 * @brief 直線・線分と交差・距離・射影
 * @docs docs/geometry/line.md
 */

template <class T = ld> struct Line
{
  Point<T> a{}, b{T(1), T(0)};
  Line() = default;
  Line(const Point<T> &a, const Point<T> &b) : a(a), b(b) { assert(a != b); }
  template <class U> explicit Line(const Line<U> &l) : Line(Point<T>(l.a), Point<T>(l.b)) {}
  template <class U> Line<U> cast() const { return Line<U>(*this); }
  Point<geometry_wide_t<T>> vec() const
  { using W = geometry_wide_t<T>; return b.template cast<W>() - a.template cast<W>(); }
  friend istream &operator>>(istream &in, Line &l) { in >> l.a >> l.b; assert(!in || l.a != l.b); return in; }
  friend ostream &operator<<(ostream &out, const Line &l) { return out << l.a << ' ' << l.b; }
#if defined FAST_IO && !defined LOCAL
  friend void rd1(Line &l) { rd1(l.a); rd1(l.b); assert(l.a != l.b); }
  friend void wt1(const Line &l) { using fastio::wt1; wt1(l.a); wt1(' '); wt1(l.b); }
#endif
  CPP_DUMP_DEFINE_DATA(a, b);
};

template <class T = ld> struct Segment
{
  Point<T> a{}, b{};
  Segment() = default;
  Segment(const Point<T> &a, const Point<T> &b) : a(a), b(b) {}
  template <class U> explicit Segment(const Segment<U> &s) : a(s.a), b(s.b) {}
  template <class U> Segment<U> cast() const { return Segment<U>(*this); }
  Point<geometry_wide_t<T>> vec() const
  { using W = geometry_wide_t<T>; return b.template cast<W>() - a.template cast<W>(); }
  friend istream &operator>>(istream &in, Segment &s) { return in >> s.a >> s.b; }
  friend ostream &operator<<(ostream &out, const Segment &s) { return out << s.a << ' ' << s.b; }
#if defined FAST_IO && !defined LOCAL
  friend void rd1(Segment &s) { rd1(s.a); rd1(s.b); }
  friend void wt1(const Segment &s) { using fastio::wt1; wt1(s.a); wt1(' '); wt1(s.b); }
#endif
  CPP_DUMP_DEFINE_DATA(a, b);
};

#ifdef LOCAL
namespace cpp_dump::_detail
{
template <class T> inline constexpr bool is_ostream<Line<T>> = false;
template <class T> inline constexpr bool is_ostream<Segment<T>> = false;
}
#endif

enum class IntersectionKind { Empty, Points, Segment, Line, Circle };
#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_ENUM(IntersectionKind, IntersectionKind::Empty, IntersectionKind::Points,
                          IntersectionKind::Segment, IntersectionKind::Line, IntersectionKind::Circle);
#endif

template <class T> struct Intersection
{
  IntersectionKind kind = IntersectionKind::Empty;
  vector<Point<T>> points;
  bool empty() const { return kind == IntersectionKind::Empty; }
  bool infinite() const
  { return kind == IntersectionKind::Segment || kind == IntersectionKind::Line || kind == IntersectionKind::Circle; }
  CPP_DUMP_DEFINE_DATA(kind, points);
};

template <class T> bool contains(const Line<T> &l, const Point<T> &p, geometry_real_t<T> eps = EPS)
{ assert(l.a != l.b); return orientation(l.a, l.b, p, eps) == 0; }
template <class T> bool contains(const Segment<T> &s, const Point<T> &p, geometry_real_t<T> eps = EPS)
{
  if (s.a == s.b) return almost_equal(s.a, p, eps);
  if (orientation(s.a, s.b, p, eps)) return false;
  using W = geometry_wide_t<T>;
  return geometry_detail::sign(dot(p.template cast<W>() - s.a.template cast<W>(),
                                   p.template cast<W>() - s.b.template cast<W>()), eps) <= 0;
}
template <class T> bool parallel(const Line<T> &l, const Line<T> &m, geometry_real_t<T> eps = EPS)
{ return geometry_detail::sign(cross(l.vec(), m.vec()), eps) == 0; }
template <class T> bool perpendicular(const Line<T> &l, const Line<T> &m, geometry_real_t<T> eps = EPS)
{ return geometry_detail::sign(dot(l.vec(), m.vec()), eps) == 0; }
template <class T> bool parallel(const Line<T> &l, const Segment<T> &s, geometry_real_t<T> eps = EPS)
{ return parallel(l, Line<T>(s.a, s.b), eps); }
template <class T> bool parallel(const Segment<T> &s, const Line<T> &l, geometry_real_t<T> eps = EPS)
{ return parallel(l, s, eps); }
template <class T> bool parallel(const Segment<T> &s, const Segment<T> &t, geometry_real_t<T> eps = EPS)
{ return parallel(Line<T>(s.a, s.b), Line<T>(t.a, t.b), eps); }
template <class T> bool perpendicular(const Line<T> &l, const Segment<T> &s, geometry_real_t<T> eps = EPS)
{ return perpendicular(l, Line<T>(s.a, s.b), eps); }
template <class T> bool perpendicular(const Segment<T> &s, const Line<T> &l, geometry_real_t<T> eps = EPS)
{ return perpendicular(l, s, eps); }
template <class T> bool perpendicular(const Segment<T> &s, const Segment<T> &t, geometry_real_t<T> eps = EPS)
{ return perpendicular(Line<T>(s.a, s.b), Line<T>(t.a, t.b), eps); }
template <class T> bool intersects(const Line<T> &l, const Line<T> &m, geometry_real_t<T> eps = EPS)
{ return !parallel(l, m, eps) || contains(l, m.a, eps); }
template <class T> bool intersects(const Line<T> &l, const Segment<T> &s, geometry_real_t<T> eps = EPS)
{ return orientation(l.a, l.b, s.a, eps) * orientation(l.a, l.b, s.b, eps) <= 0; }
template <class T> bool intersects(const Segment<T> &s, const Line<T> &l, geometry_real_t<T> eps = EPS)
{ return intersects(l, s, eps); }
template <class T> bool intersects(const Segment<T> &s, const Segment<T> &t, geometry_real_t<T> eps = EPS)
{
  if (contains(s, t.a, eps) || contains(s, t.b, eps) || contains(t, s.a, eps) || contains(t, s.b, eps)) return true;
  return orientation(s.a, s.b, t.a, eps) * orientation(s.a, s.b, t.b, eps) < 0 &&
         orientation(t.a, t.b, s.a, eps) * orientation(t.a, t.b, s.b, eps) < 0;
}

namespace geometry_detail
{
template <class T> Point<field_t<T>> affine(const Point<T> &a, const Point<T> &v,
                                            const T &num, const T &den)
{
  if constexpr (is_integral_ext<T>)
    return {{a.x * den + v.x * num, den}, {a.y * den + v.y * num, den}};
  else return a.template cast<field_t<T>>() + v.template cast<field_t<T>>() * quotient(num, den);
}
template <class T> Point<field_t<T>> line_crossing(const Point<T> &a, const Point<T> &b,
                                                 const Point<T> &c, const Point<T> &d)
{
  using W = wide_t<T>;
  auto aw = a.template cast<W>(), v = b.template cast<W>() - aw;
  auto cw = c.template cast<W>(), w = d.template cast<W>() - cw;
  return affine(aw, v, cross(cw - aw, w), cross(v, w));
}
}

template <class T> Intersection<geometry_field_t<T>> intersection(const Line<T> &l, const Line<T> &m,
                                                                geometry_real_t<T> eps = EPS)
{
  using F = geometry_field_t<T>;
  if (parallel(l, m, eps))
  {
    if (contains(l, m.a, eps)) return {IntersectionKind::Line, {l.a.template cast<F>(), l.b.template cast<F>()}};
    return {};
  }
  return {IntersectionKind::Points, {geometry_detail::line_crossing(l.a, l.b, m.a, m.b)}};
}
template <class T> Intersection<geometry_field_t<T>> intersection(const Line<T> &l, const Segment<T> &s,
                                                                geometry_real_t<T> eps = EPS)
{
  using F = geometry_field_t<T>;
  if (!intersects(l, s, eps)) return {};
  if (s.a == s.b) return {IntersectionKind::Points, {s.a.template cast<F>()}};
  if (contains(l, s.a, eps) && contains(l, s.b, eps))
    return {IntersectionKind::Segment, {s.a.template cast<F>(), s.b.template cast<F>()}};
  if (contains(l, s.a, eps)) return {IntersectionKind::Points, {s.a.template cast<F>()}};
  if (contains(l, s.b, eps)) return {IntersectionKind::Points, {s.b.template cast<F>()}};
  return {IntersectionKind::Points, {geometry_detail::line_crossing(l.a, l.b, s.a, s.b)}};
}
template <class T> Intersection<geometry_field_t<T>> intersection(const Segment<T> &s, const Line<T> &l,
                                                                geometry_real_t<T> eps = EPS)
{ return intersection(l, s, eps); }
template <class T> Intersection<geometry_field_t<T>> intersection(const Segment<T> &s, const Segment<T> &t,
                                                                geometry_real_t<T> eps = EPS)
{
  using F = geometry_field_t<T>;
  if (!intersects(s, t, eps)) return {};
  vector<Point<T>> ends;
  for (const auto &p : {s.a, s.b}) if (contains(t, p, eps)) ends.push_back(p);
  for (const auto &p : {t.a, t.b}) if (contains(s, p, eps)) ends.push_back(p);
  if (!ends.empty())
  {
    sort(ends.begin(), ends.end());
    if (almost_equal(ends.front(), ends.back(), eps))
      return {IntersectionKind::Points, {ends.front().template cast<F>()}};
    return {IntersectionKind::Segment, {ends.front().template cast<F>(), ends.back().template cast<F>()}};
  }
  return {IntersectionKind::Points, {geometry_detail::line_crossing(s.a, s.b, t.a, t.b)}};
}

template <class T> Point<geometry_field_t<T>> projection(const Line<T> &l, const Point<T> &p)
{
  using W = geometry_wide_t<T>;
  auto a = l.a.template cast<W>(), v = l.vec();
  return geometry_detail::affine(a, v, dot(p.template cast<W>() - a, v), norm2(v));
}
template <class T> Point<geometry_field_t<T>> reflection(const Line<T> &l, const Point<T> &p)
{
  using F = geometry_field_t<T>;
  return projection(l, p) * F(2) - p.template cast<F>();
}
template <class T> Point<geometry_field_t<T>> closest_point(const Segment<T> &s, const Point<T> &p)
{
  using W = geometry_wide_t<T>; using F = geometry_field_t<T>;
  auto a = s.a.template cast<W>(), v = s.vec();
  W n = norm2(v), t = dot(p.template cast<W>() - a, v);
  if (t <= W(0)) return s.a.template cast<F>();
  if (n <= t) return s.b.template cast<F>();
  return geometry_detail::affine(a, v, t, n);
}

namespace geometry_detail
{
template <class T> pair<wide_t<T>, wide_t<T>> distance_fraction(const Line<T> &l, const Point<T> &p)
{
  auto c = cross(l.a, l.b, p);
  return {c * c, norm2(l.vec())};
}
template <class T> pair<wide_t<T>, wide_t<T>> distance_fraction(const Segment<T> &s, const Point<T> &p)
{
  using W = wide_t<T>;
  auto a = s.a.template cast<W>(), v = s.vec();
  W n = norm2(v), t = dot(p.template cast<W>() - a, v);
  if (t <= W(0)) return {distance2(s.a, p), W(1)};
  if (n <= t) return {distance2(s.b, p), W(1)};
  W c = cross(s.a, s.b, p);
  return {c * c, n};
}
}

template <class T> geometry_field_t<T> distance2(const Line<T> &l, const Point<T> &p)
{ auto f = geometry_detail::distance_fraction(l, p); return geometry_detail::quotient(f.first, f.second); }
template <class T> geometry_field_t<T> distance2(const Point<T> &p, const Line<T> &l) { return distance2(l, p); }
template <class T> geometry_field_t<T> distance2(const Segment<T> &s, const Point<T> &p)
{ auto f = geometry_detail::distance_fraction(s, p); return geometry_detail::quotient(f.first, f.second); }
template <class T> geometry_field_t<T> distance2(const Point<T> &p, const Segment<T> &s) { return distance2(s, p); }
template <class T> geometry_field_t<T> distance2(const Line<T> &l, const Line<T> &m)
{ return intersects(l, m) ? geometry_field_t<T>(0) : distance2(l, m.a); }
template <class T> geometry_field_t<T> distance2(const Line<T> &l, const Segment<T> &s)
{ return intersects(l, s) ? geometry_field_t<T>(0) : min(distance2(l, s.a), distance2(l, s.b)); }
template <class T> geometry_field_t<T> distance2(const Segment<T> &s, const Line<T> &l) { return distance2(l, s); }
template <class T> geometry_field_t<T> distance2(const Segment<T> &s, const Segment<T> &t)
{
  if (intersects(s, t)) return geometry_field_t<T>(0);
  return min({distance2(s, t.a), distance2(s, t.b), distance2(t, s.a), distance2(t, s.b)});
}
template <class T, template <class> class A, template <class> class B>
auto distance(const A<T> &a, const B<T> &b)
    -> enable_if_t<(is_same_v<A<T>, Line<T>> || is_same_v<A<T>, Segment<T>> || is_same_v<A<T>, Point<T>>) &&
                   (is_same_v<B<T>, Line<T>> || is_same_v<B<T>, Segment<T>> || is_same_v<B<T>, Point<T>>) &&
                   !(is_same_v<A<T>, Point<T>> && is_same_v<B<T>, Point<T>>), geometry_real_t<T>>
{ return sqrt(geometry_detail::cast<geometry_real_t<T>>(distance2(a, b))); }
