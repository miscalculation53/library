#pragma once

#include "../template/template_all_but_modint.hpp"

#include "../math/rational.hpp"
#include "../utils/is_integral_ext.hpp"
#include "../utils/larger_int.hpp"

/**
 * @brief Half-Plane Intersection
 * @docs docs/convex/halfplane_intersection.md
 */

template <class T = ll, class Calc = larger_int_t<larger_int_t<T>>>
struct HalfPlaneIntersection
{
  static_assert(is_integral_ext<T> && is_signed_ext<T>);
  static_assert(is_integral_ext<Calc> && is_signed_ext<Calc>);

  using Fraction = Rational<Calc>;

  struct HalfPlane
  {
    T a, b, c;
    int id = -1;
  };

  struct Point
  {
    Fraction x, y;
  };

  enum class Status
  {
    Empty,
    Unbounded,
    Bounded,
  };

  struct Result
  {
    Status status;
    vc<HalfPlane> boundaries;
    vc<Point> vertices;
  };

private:
  struct Line
  {
    T a, b, c;
    int id;
    bool box;
  };

  struct FractionPoint
  {
    Calc x, y, den;
  };

  vc<HalfPlane> hps;
  T infty;
  int add_cnt = 0;

  static Calc det(const Line &p, const Line &q)
  {
    return Calc(p.a) * Calc(q.b) - Calc(p.b) * Calc(q.a);
  }

  static Calc dot_normal(const Line &p, const Line &q)
  {
    return Calc(p.a) * Calc(q.a) + Calc(p.b) * Calc(q.b);
  }

  static int half(const Line &p)
  {
    Calc x = -Calc(p.b), y = Calc(p.a);
    return y > 0 || (y == 0 && x >= 0) ? 0 : 1;
  }

  static bool angle_less(const Line &p, const Line &q)
  {
    int ph = half(p), qh = half(q);
    if (ph != qh) return ph < qh;
    Calc cr = det(p, q);
    return cr != 0 && cr > 0;
  }

  static bool parallel(const Line &p, const Line &q) { return det(p, q) == 0; }

  static bool same_direction(const Line &p, const Line &q)
  {
    return parallel(p, q) && dot_normal(p, q) > 0;
  }

  static bool stricter(const Line &p, const Line &q)
  {
    Calc z;
    if (q.a != 0)
    {
      z = Calc(p.c) * Calc(q.a) - Calc(p.a) * Calc(q.c);
      if (q.a < 0) z = -z;
    }
    else
    {
      z = Calc(p.c) * Calc(q.b) - Calc(p.b) * Calc(q.c);
      if (q.b < 0) z = -z;
    }
    return z > 0;
  }

  static FractionPoint intersection(const Line &p, const Line &q)
  {
    FractionPoint res{
        Calc(p.b) * Calc(q.c) - Calc(q.b) * Calc(p.c),
        Calc(p.c) * Calc(q.a) - Calc(q.c) * Calc(p.a), det(p, q)};
    if (res.den < 0) res.x = -res.x, res.y = -res.y, res.den = -res.den;
    return res;
  }

  static Calc side(const Line &h, const FractionPoint &p)
  {
    return Calc(h.a) * p.x + Calc(h.b) * p.y + Calc(h.c) * p.den;
  }

  static bool outside(const Line &h, const FractionPoint &p) { return side(h, p) > 0; }

  static Point point(const FractionPoint &p)
  {
    return {Fraction(p.x, p.den), Fraction(p.y, p.den)};
  }

  static HalfPlane halfplane(const Line &h) { return {h.a, h.b, h.c, h.id}; }

public:
  // 空の半平面集合を作る。infty は非有界判定用の内部の枠に使う。
  explicit HalfPlaneIntersection(T infty = numeric_limits<T>::max() / T(2)) : infty(infty)
  {
    assert(infty > 0);
  }

  // ax + by + c <= 0 を追加する。id は 0 始まりの追加順になる。
  void add(T a, T b, T c) { add(a, b, c, add_cnt); }

  // ax + by + c <= 0 を指定した id で追加する。
  void add(T a, T b, T c, int id)
  {
    add_cnt++;
    hps.eb(HalfPlane{a, b, c, id});
  }

  // 共通部分の状態、境界に残る半平面、有限頂点を返す。
  Result intersection() const
  {
    vc<Line> ls;
    ls.reserve(hps.size() + 4);
    for (const HalfPlane &h : hps)
    {
      if (h.a == 0 && h.b == 0)
      {
        if (h.c > 0) return {Status::Empty, {}, {}};
        continue;
      }
      ls.eb(Line{h.a, h.b, h.c, h.id, false});
    }
    ls.eb(Line{1, 0, -infty, -1, true});
    ls.eb(Line{-1, 0, -infty, -1, true});
    ls.eb(Line{0, 1, -infty, -1, true});
    ls.eb(Line{0, -1, -infty, -1, true});

    sort(ls.begin(), ls.end(), angle_less);
    vc<Line> ord;
    ord.reserve(ls.size());
    for (const Line &p : ls)
    {
      if (!ord.empty() && same_direction(ord.back(), p))
      {
        if (stricter(p, ord.back())) ord.back() = p;
      }
      else
        ord.eb(p);
    }
    if (ord.size() >= 2 && same_direction(ord.front(), ord.back()))
    {
      if (stricter(ord.back(), ord.front())) ord.front() = ord.back();
      ord.pop_back();
    }

    deque<Line> dq;
    for (const Line &p : ord)
    {
      while (dq.size() >= 2)
      {
        if (parallel(dq[dq.size() - 2], dq.back())) return {Status::Empty, {}, {}};
        if (!outside(p, intersection(dq[dq.size() - 2], dq.back()))) break;
        dq.pop_back();
      }
      while (dq.size() >= 2)
      {
        if (parallel(dq[0], dq[1])) return {Status::Empty, {}, {}};
        if (!outside(p, intersection(dq[0], dq[1]))) break;
        dq.pop_front();
      }
      if (!dq.empty() && parallel(dq.back(), p)) return {Status::Empty, {}, {}};
      dq.eb(p);
    }

    while (dq.size() >= 3)
    {
      if (parallel(dq[dq.size() - 2], dq.back())) return {Status::Empty, {}, {}};
      if (!outside(dq.front(), intersection(dq[dq.size() - 2], dq.back()))) break;
      dq.pop_back();
    }
    while (dq.size() >= 3)
    {
      if (parallel(dq[0], dq[1])) return {Status::Empty, {}, {}};
      if (!outside(dq.back(), intersection(dq[0], dq[1]))) break;
      dq.pop_front();
    }
    if (dq.size() < 3 || parallel(dq.front(), dq.back()))
      return {Status::Empty, {}, {}};

    int n = dq.size();
    vc<FractionPoint> ps(n);
    repi(i, n)
    {
      const Line &p = dq[i], &q = dq[(i + 1) % n];
      if (parallel(p, q)) return {Status::Empty, {}, {}};
      ps[i] = intersection(p, q);
    }
    bool has_interior = false;
    optional<Line> edge;
    repi(i, n) if (side(dq[(i + n - 1) % n], ps[i]) < 0)
    {
      if (edge && !parallel(*edge, dq[i])) has_interior = true;
      if (!edge) edge = dq[i];
    }
    if (!has_interior) return {Status::Empty, {}, {}};

    bool unbounded = false;
    vc<HalfPlane> boundaries;
    for (const Line &p : dq)
    {
      unbounded |= p.box;
      if (!p.box) boundaries.eb(halfplane(p));
    }

    vc<Point> vertices;
    if (!unbounded)
      for (const FractionPoint &p : ps) vertices.eb(point(p));
    else
      repi(i, n) if (!dq[i].box && !dq[(i + 1) % n].box) vertices.eb(point(ps[i]));
    return {unbounded ? Status::Unbounded : Status::Bounded,
            std::move(boundaries), std::move(vertices)};
  }

  // すべての半平面と自動採番を消去する。
  void clear()
  {
    hps.clear();
    add_cnt = 0;
  }
};
