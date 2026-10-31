#pragma once

#include "../math/rational.hpp"

/**
 * @brief 二次元の点・ベクトル
 * @docs docs/geometry/point.md
 */

namespace geometry_detail
{
template <class T, bool = is_integral_ext<T>> struct wide { using type = T; };
template <class T> struct wide<T, true>
{
  static_assert(is_signed_ext<T>, "geometry coordinates use signed integers");
  using type = i128;
};
template <class T> struct wide<Rational<T>, false>
{ using type = Rational<typename wide<T>::type>; };
template <class T> using wide_t = typename wide<T>::type;
template <class T> using field_t = conditional_t<is_integral_ext<T>, Rational<wide_t<T>>, wide_t<T>>;
template <class T> using real_t = conditional_t<is_integral_ext<T> || is_rational_v<T>, ld, T>;

template <class To, class From> constexpr To cast(const From &x)
{
  if constexpr (is_rational_v<From> && !is_rational_v<To>)
    return To(x.num) / To(x.den);
  else return To(x);
}

template <class T> int sign(const T &x, real_t<T> eps = EPS)
{
  if constexpr (is_integral_ext<T> || is_rational_v<T>)
    return int(T(0) < x) - int(x < T(0));
  else return int(eps < x) - int(x < -eps);
}

template <class T> field_t<T> quotient(const T &a, const T &b)
{
  if constexpr (is_integral_ext<T>) return {a, b};
  else return field_t<T>(a) / field_t<T>(b);
}

// 有理数座標の入力形式は整数または num/den。
template <class T> void parse_rational(const string &s, Rational<T> &x)
{
  auto slash = s.find('/');
  auto parse = [](const string &part)
  {
    istringstream in(part);
    T v{};
    in >> v;
    assert(in && in.peek() == char_traits<char>::eof());
    return v;
  };
  x = slash == string::npos ? Rational<T>(parse(s)) :
      Rational<T>(parse(s.substr(0, slash)), parse(s.substr(slash + 1)));
}
template <class T> void read_scalar(istream &in, T &x)
{
  if constexpr (is_rational_v<T>)
  {
    string s;
    if (in >> s) parse_rational(s, x);
  }
  else in >> x;
}
#if defined FAST_IO && !defined LOCAL
template <class T> void read_scalar(T &x)
{
  if constexpr (is_rational_v<T>)
  {
    string s;
    fastio::rd1(s);
    parse_rational(s, x);
  }
  else { using fastio::rd1; rd1(x); }
}
#endif
}

template <class T> using geometry_wide_t = geometry_detail::wide_t<T>;
template <class T> using geometry_field_t = geometry_detail::field_t<T>;
template <class T> using geometry_real_t = geometry_detail::real_t<T>;

enum class Location { Outside = -1, Boundary = 0, Inside = 1 };
#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_ENUM(Location, Location::Outside, Location::Boundary, Location::Inside);
#endif

template <class T = ld>
struct Point
{
  using value_type = T;
  T x{}, y{};
  constexpr Point() = default;
  constexpr Point(const T &x, const T &y) : x(x), y(y) {}
  template <class U, class V,
            enable_if_t<is_convertible_v<const U &, T> && is_convertible_v<const V &, T>, int> = 0>
  constexpr Point(const pair<U, V> &p) : x(p.first), y(p.second) {}
  template <class U> explicit constexpr Point(const Point<U> &p)
      : x(geometry_detail::cast<T>(p.x)), y(geometry_detail::cast<T>(p.y)) {}
  template <class U> constexpr Point<U> cast() const { return Point<U>(*this); }

  constexpr Point operator+() const { return *this; }
  constexpr Point operator-() const { return {-x, -y}; }
  constexpr Point &operator+=(const Point &p) { x += p.x; y += p.y; return *this; }
  constexpr Point &operator-=(const Point &p) { x -= p.x; y -= p.y; return *this; }
  constexpr Point &operator*=(const T &s) { x *= s; y *= s; return *this; }
  template <class U = T, enable_if_t<!is_integral_ext<U>, int> = 0>
  constexpr Point &operator/=(const T &s) { x /= s; y /= s; return *this; }
  friend constexpr Point operator+(Point p, const Point &q) { return p += q; }
  friend constexpr Point operator-(Point p, const Point &q) { return p -= q; }
  friend constexpr Point operator*(Point p, const T &s) { return p *= s; }
  friend constexpr Point operator*(const T &s, Point p) { return p *= s; }
  friend Point<geometry_field_t<T>> operator/(const Point &p, const T &s)
  {
    using W = geometry_wide_t<T>;
    return {geometry_detail::quotient(W(p.x), W(s)), geometry_detail::quotient(W(p.y), W(s))};
  }
  friend constexpr bool operator==(const Point &p, const Point &q) { return p.x == q.x && p.y == q.y; }
  friend constexpr bool operator!=(const Point &p, const Point &q) { return !(p == q); }
  friend constexpr bool operator<(const Point &p, const Point &q)
  { return p.x < q.x || (!(q.x < p.x) && p.y < q.y); }
  friend constexpr bool operator>(const Point &p, const Point &q) { return q < p; }
  friend constexpr bool operator<=(const Point &p, const Point &q) { return !(q < p); }
  friend constexpr bool operator>=(const Point &p, const Point &q) { return !(p < q); }

  friend istream &operator>>(istream &in, Point &p)
  { geometry_detail::read_scalar(in, p.x); geometry_detail::read_scalar(in, p.y); return in; }
  friend ostream &operator<<(ostream &out, const Point &p) { return out << p.x << ' ' << p.y; }
#if defined FAST_IO && !defined LOCAL
  friend void rd1(Point &p) { geometry_detail::read_scalar(p.x); geometry_detail::read_scalar(p.y); }
  friend void wt1(const Point &p) { using fastio::wt1; wt1(p.x); wt1(' '); wt1(p.y); }
#endif
  CPP_DUMP_DEFINE_DATA(x, y);
};

template <class U, class V> Point(const pair<U, V> &) -> Point<common_type_t<U, V>>;

#ifdef LOCAL
namespace cpp_dump::_detail
{
template <class T> inline constexpr bool is_ostream<Point<T>> = false;
}
#endif

template <class T> constexpr geometry_wide_t<T> dot(const Point<T> &p, const Point<T> &q)
{
  using W = geometry_wide_t<T>;
  return W(p.x) * W(q.x) + W(p.y) * W(q.y);
}
template <class T> constexpr geometry_wide_t<T> cross(const Point<T> &p, const Point<T> &q)
{
  using W = geometry_wide_t<T>;
  return W(p.x) * W(q.y) - W(p.y) * W(q.x);
}
template <class T> constexpr geometry_wide_t<T> cross(const Point<T> &a, const Point<T> &b, const Point<T> &c)
{
  using W = geometry_wide_t<T>;
  return cross(b.template cast<W>() - a.template cast<W>(), c.template cast<W>() - a.template cast<W>());
}
template <class T> constexpr geometry_wide_t<T> norm2(const Point<T> &p) { return dot(p, p); }
template <class T> constexpr geometry_wide_t<T> distance2(const Point<T> &p, const Point<T> &q)
{
  using W = geometry_wide_t<T>;
  return norm2(p.template cast<W>() - q.template cast<W>());
}
template <class T> int orientation(const Point<T> &a, const Point<T> &b, const Point<T> &c,
                                  geometry_real_t<T> eps = EPS)
{ return geometry_detail::sign(cross(a, b, c), eps); }
template <class T> bool almost_equal(const Point<T> &p, const Point<T> &q, geometry_real_t<T> eps = EPS)
{
  using W = geometry_wide_t<T>;
  return geometry_detail::sign(W(p.x) - W(q.x), eps) == 0 &&
         geometry_detail::sign(W(p.y) - W(q.y), eps) == 0;
}
template <class T> constexpr Point<T> rot90(const Point<T> &p) { return {-p.y, p.x}; }
template <class T> geometry_real_t<T> norm(const Point<T> &p)
{ return sqrt(geometry_detail::cast<geometry_real_t<T>>(norm2(p))); }
template <class T> geometry_real_t<T> distance(const Point<T> &p, const Point<T> &q)
{ return sqrt(geometry_detail::cast<geometry_real_t<T>>(distance2(p, q))); }
template <class T> geometry_real_t<T> argument(const Point<T> &p)
{
  using R = geometry_real_t<T>;
  assert(p != Point<T>());
  return atan2(geometry_detail::cast<R>(p.y), geometry_detail::cast<R>(p.x));
}
template <class T> geometry_real_t<T> angle(const Point<T> &p, const Point<T> &q)
{
  using R = geometry_real_t<T>;
  assert(p != Point<T>() && q != Point<T>());
  return atan2(geometry_detail::cast<R>(cross(p, q)), geometry_detail::cast<R>(dot(p, q)));
}
template <class T, class Angle> Point<geometry_real_t<T>> rotate(const Point<T> &p, Angle angle)
{
  using R = geometry_real_t<T>;
  auto q = p.template cast<R>();
  R c = cos(R(angle)), s = sin(R(angle));
  return {q.x * c - q.y * s, q.x * s + q.y * c};
}
template <class T, class Angle> Point<geometry_real_t<T>> rotate(Point<T> &p, Angle angle)
{ return rotate(static_cast<const Point<T> &>(p), angle); }
template <class T> optional<Point<geometry_real_t<T>>> unit(const Point<T> &p)
{
  if (p == Point<T>()) return nullopt;
  using R = geometry_real_t<T>;
  return p.template cast<R>() / norm(p);
}

namespace geometry_detail
{
// 零ベクトルを先頭に置き、正の x 軸から反時計回り。同じ半直線は同値。
template <class T> constexpr int polar_compare(const Point<T> &p, const Point<T> &q)
{
  bool p0 = p == Point<T>(), q0 = q == Point<T>();
  if (p0 || q0) return int(q0) - int(p0);
  auto half = [](const Point<T> &v) { return v.y < T(0) || (v.y == T(0) && v.x < T(0)); };
  bool a = half(p), b = half(q);
  if (a != b) return a ? 1 : -1;
  auto c = cross(p, q);
  return int(c < decltype(c)(0)) - int(decltype(c)(0) < c);
}

template <class Relation> struct polar_relation
{
  template <class T> constexpr bool operator()(const Point<T> &p, const Point<T> &q) const
  { return Relation{}(polar_compare(p, q), 0); }
};
}

inline constexpr geometry_detail::polar_relation<less<>> polar_less{};
inline constexpr geometry_detail::polar_relation<less_equal<>> polar_less_equal{};
inline constexpr geometry_detail::polar_relation<greater<>> polar_greater{};
inline constexpr geometry_detail::polar_relation<greater_equal<>> polar_greater_equal{};
inline constexpr geometry_detail::polar_relation<equal_to<>> polar_equal_to{};
inline constexpr geometry_detail::polar_relation<not_equal_to<>> polar_not_equal_to{};
