#pragma once

#include "../template/template_all_but_modint.hpp"

#include "convex_hull_trick.hpp"
#include "../math/rational.hpp"
#include "../utils/is_integral_ext.hpp"
#include "../utils/larger_int.hpp"
#include "../utils/resolved_infty.hpp"

/**
 * @brief Dot Product Convex Hull Trick
 * @docs docs/convex/convex_hull_trick_dot_product.md
 */

template <class T, class U, auto inftyU, template <class, class, class, auto, auto> class CHT>
struct ConvexHullTrickDotProductBase
{
  static_assert(is_integral_ext<T> || is_floating_point_v<T>);
  static_assert(is_signed_ext<T> || is_floating_point_v<T>);

  using LinearForm = ConvexHullTrickLine<T>;

private:
  using R = Rational<T>;
  using V = Rational<U>;
  using RationalLine = ConvexHullTrickLine<R>;
  // 境界の無限大は、空集合で返す U 型の inftyU と独立に持つ。
  using MinCHT = CHT<R, V, less<>, R::infty, inftyU>;
  using MaxCHT = CHT<R, V, greater<>, R::infty, inftyU>;

  MinCHT mn;
  MaxCHT mx;
  LinearForm fst, mna, mxa;
  int add_cnt = 0;
  bool has_line = false;

  static LinearForm form(const RationalLine &f)
  {
    return {T(f.a.num), T(f.b.num), f.id};
  }

  static bool same_point(const LinearForm &x, const LinearForm &y)
  {
    return x.a == y.a && x.b == y.b;
  }

  static U dot(const LinearForm &f, T x, T y)
  {
    return U(f.a) * U(x) + U(f.b) * U(y);
  }

  template <class Hull>
  static void append_chain(const Hull &cht, vc<LinearForm> &res)
  {
    auto segs = cht.segments();
    for (auto it = segs.rbegin(); it != segs.rend(); ++it)
    {
      LinearForm f = form(it->line);
      if (res.empty() || !same_point(res.back(), f)) res.eb(f);
    }
  }

  template <bool minimize>
  pair<U, LinearForm> query_impl(T x, T y) const
  {
    if (!has_line)
    {
      const U &inf = resolved_infty<U, inftyU>();
      return {minimize ? inf : -inf, {0, 0, -1}};
    }
    LinearForm f;
    if (x == 0 && y == 0)
      f = fst;
    else if (y == 0)
    {
      if constexpr (minimize)
        f = x > 0 ? mna : mxa;
      else
        f = x > 0 ? mxa : mna;
    }
    else
    {
      R q{x, y};
      if constexpr (minimize)
        f = form((y > 0 ? mn.query(q) : mx.query(q)).second);
      else
        f = form((y > 0 ? mx.query(q) : mn.query(q)).second);
    }
    return {dot(f, x, y), f};
  }

  template <bool minimize>
  pair<U, LinearForm> query_monotone_impl(T x, T y)
  {
    if (!has_line)
    {
      const U &inf = resolved_infty<U, inftyU>();
      return {minimize ? inf : -inf, {0, 0, -1}};
    }
    assert(y != 0);
    R q{x, y};
    LinearForm f;
    if constexpr (minimize)
      f = form((y > 0 ? mn.query_monotone(q) : mx.query_monotone(q)).second);
    else
      f = form((y > 0 ? mx.query_monotone(q) : mn.query_monotone(q)).second);
    return {dot(f, x, y), f};
  }

public:
  // ax + by
  void add(T a, T b) { add(a, b, add_cnt); }

  // ax + by
  void add(T a, T b, int id)
  {
    add_cnt++;
    LinearForm f{a, b, id};
    mn.add_line(R(a), R(b), id);
    mx.add_line(R(a), R(b), id);
    if (!has_line)
      fst = mna = mxa = f;
    else
    {
      if (a < mna.a) mna = f;
      if (mxa.a < a) mxa = f;
    }
    has_line = true;
  }

  pair<U, LinearForm> min_query(T x, T y) const { return query_impl<true>(x, y); }

  pair<U, LinearForm> max_query(T x, T y) const { return query_impl<false>(x, y); }

  // x/y が単調
  pair<U, LinearForm> min_query_monotone(T x, T y) { return query_monotone_impl<true>(x, y); }

  // x/y が単調
  pair<U, LinearForm> max_query_monotone(T x, T y) { return query_monotone_impl<false>(x, y); }

  void reset_monotone_query()
  {
    mn.reset_monotone_query();
    mx.reset_monotone_query();
  }

  // (a, b) として見る必要のあるものだけ集めた集合
  // 反時計回り
  // (a, b) が最適であるための (x, y) の条件は、両隣 (循環ありで) よりもよいこと
  // (図形的には、凸包の頂点とみなせる)
  vc<LinearForm> convex_hull() const
  {
    vc<LinearForm> res;
    append_chain(mn, res);
    append_chain(mx, res);
    if (res.size() > 1 && same_point(res.front(), res.back())) res.pop_back();
    return res;
  }

  void clear()
  {
    mn.clear();
    mx.clear();
    add_cnt = 0;
    has_line = false;
  }
};

// T: ax + by の a, x, b, y の型
// U: ax + by の型
template <class T, class U = larger_int_t<T>, auto inftyU = nullptr>
struct ConvexHullTrickDotProduct
    : ConvexHullTrickDotProductBase<T, U, inftyU, ConvexHullTrick>
{};

// T: ax + by の a, x, b, y の型
// U: ax + by の型
// 係数 a が単調
template <class T, class U = larger_int_t<T>, auto inftyU = nullptr>
struct ConvexHullTrickDotProductMonotoneSlope
    : ConvexHullTrickDotProductBase<T, U, inftyU, ConvexHullTrickMonotoneSlope>
{};
