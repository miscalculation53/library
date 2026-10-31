#pragma once

#include "internal_convex_polygon.hpp"

/**
 * @brief 凸領域を保持して半平面で繰り返し切断
 * @docs docs/geometry/convex_cut.md
 */

template <class T = ld> class ConvexCut
{
public:
  using value_type = geometry_field_t<T>;
  using P = Point<value_type>;

private:
  struct Vertex
  {
    P point;
    Line<T> incoming;
  };
  vector<Vertex> vertices;

  static P reduced(P p)
  {
    if constexpr (is_rational_v<value_type>)
    {
      auto x = p.x.reduced(), y = p.y.reduced();
      p.x = value_type(x.first, x.second);
      p.y = value_type(y.first, y.second);
    }
    return p;
  }

public:
  explicit ConvexCut(const T &sp = T(100000), geometry_real_t<T> eps = EPS)
  {
    assert(T(0) <= sp);
    *this = ConvexCut(vector<Point<T>>{{-sp, -sp}, {sp, -sp}, {sp, sp}, {-sp, sp}}, eps);
  }
  explicit ConvexCut(const vector<Point<T>> &p, geometry_real_t<T> eps = EPS)
  {
    auto q = geometry_detail::normalize_convex_polygon(p, eps);
    vertices.reserve(q.size());
    for (size_t i = 0; i < q.size(); ++i)
    {
      // 一点の領域では incoming は使わない。
      Line<T> edge = q.size() == 1 ? Line<T>() : Line<T>(q[(i + q.size() - 1) % q.size()], q[i]);
      vertices.push_back({reduced(q[i].template cast<value_type>()), edge});
    }
  }

  void cut(const Line<T> &line, geometry_real_t<T> eps = EPS)
  {
    if (vertices.empty()) return;
    auto l = line.template cast<value_type>();
    vector<Vertex> next;
    next.reserve(vertices.size() + 1);
    auto append = [&](const P &p, const Line<T> &incoming)
    {
      if (next.empty() || !almost_equal(next.back().point, p, eps)) next.push_back({p, incoming});
    };
    int a_side = orientation(l.a, l.b, vertices.back().point, eps);
    for (size_t i = 0; i < vertices.size(); ++i)
    {
      const auto &b = vertices[i];
      int b_side = orientation(l.a, l.b, b.point, eps);
      if (a_side * b_side < 0)
      {
        // 新しい頂点は、元の境界直線と今回の切断直線から直接作る。
        P p = reduced(geometry_detail::line_crossing(b.incoming.a, b.incoming.b, line.a, line.b));
        append(p, a_side < 0 ? line : b.incoming);
      }
      if (b_side >= 0)
      {
        // 外側から境界上の頂点に入る辺は、切断直線上に置き換わる。
        append(b.point, a_side < 0 && b_side == 0 ? line : b.incoming);
      }
      a_side = b_side;
    }
    if (next.size() > 1 && almost_equal(next.front().point, next.back().point, eps))
    {
      next.front().incoming = next.back().incoming;
      next.pop_back();
    }
    vertices.swap(next);
  }
  void cut(const Point<T> &a, const Point<T> &b, geometry_real_t<T> eps = EPS)
  { cut(Line<T>(a, b), eps); }
  bool empty() const { return vertices.empty(); }
  size_t size() const { return vertices.size(); }
  vector<P> nw_poly() const
  {
    vector<P> p;
    p.reserve(vertices.size());
    for (const auto &v : vertices) p.push_back(v.point);
    reorder_polygon(p);
    return p;
  }
  vector<P> content() const { return nw_poly(); }
};
