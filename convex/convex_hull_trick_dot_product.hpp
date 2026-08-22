#pragma once

#include "../template/template_all_but_modint.hpp"

#include "convex_hull_trick.hpp"
#include "../math/rational.hpp"
#include "../utils/is_integral_ext.hpp"
#include "../utils/larger_int.hpp"

/**
 * @brief Dot Product Convex Hull Trick
 * @docs docs/convex/convex_hull_trick_dot_product.md
 */

template <class T, class Calc, Calc infty, template <class, class, auto> class CHT>
struct ConvexHullTrickDotProductBase
{
  static_assert(is_integral_ext<T> || is_floating_point_v<T>);
  static_assert(is_signed_ext<T> || is_floating_point_v<T>);

  using LinearForm = ConvexHullTrickLine<T>;

private:
  using R = Rational<Calc>;
  using RationalLine = ConvexHullTrickLine<R>;
  using MinCHT = CHT<R, less<>, infty>;
  using MaxCHT = CHT<R, greater<>, infty>;

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

  static Calc dot(const LinearForm &f, T x, T y)
  {
    return Calc(f.a) * Calc(x) + Calc(f.b) * Calc(y);
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
  pair<Calc, LinearForm> query_impl(T x, T y) const
  {
    if (!has_line) return {minimize ? infty : -infty, {0, 0, -1}};
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
      R q{Calc(x), Calc(y)};
      if constexpr (minimize)
        f = form((y > 0 ? mn.query(q) : mx.query(q)).second);
      else
        f = form((y > 0 ? mx.query(q) : mn.query(q)).second);
    }
    return {dot(f, x, y), f};
  }

  template <bool minimize>
  pair<Calc, LinearForm> query_monotone_impl(T x, T y)
  {
    if (!has_line) return {minimize ? infty : -infty, {0, 0, -1}};
    assert(y != 0);
    R q{Calc(x), Calc(y)};
    LinearForm f;
    if constexpr (minimize)
      f = form((y > 0 ? mn.query_monotone(q) : mx.query_monotone(q)).second);
    else
      f = form((y > 0 ? mx.query_monotone(q) : mn.query_monotone(q)).second);
    return {dot(f, x, y), f};
  }

public:
  // ax + by を追加する。id は 0 始まりの追加順になる。
  void add(T a, T b) { add(a, b, add_cnt); }

  // ax + by を指定した id で追加する。
  void add(T a, T b, int id)
  {
    add_cnt++;
    LinearForm f{a, b, id};
    mn.add_line(R(Calc(a)), R(Calc(b)), id);
    mx.add_line(R(Calc(a)), R(Calc(b)), id);
    if (!has_line)
      fst = mna = mxa = f;
    else
    {
      if (a < mna.a) mna = f;
      if (mxa.a < a) mxa = f;
    }
    has_line = true;
  }

  // (x, y) における最小値と一次式を返す。空なら infty とダミー一次式を返す。
  pair<Calc, LinearForm> min_query(T x, T y) const { return query_impl<true>(x, y); }

  // (x, y) における最大値と一次式を返す。空なら -infty とダミー一次式を返す。
  pair<Calc, LinearForm> max_query(T x, T y) const { return query_impl<false>(x, y); }

  // x/y が単調なクエリ列に対する最小値と一次式を返す。
  pair<Calc, LinearForm> min_query_monotone(T x, T y) { return query_monotone_impl<true>(x, y); }

  // x/y が単調なクエリ列に対する最大値と一次式を返す。
  pair<Calc, LinearForm> max_query_monotone(T x, T y) { return query_monotone_impl<false>(x, y); }

  // 単調クエリの現在位置と方向を消去する。
  void reset_monotone_query()
  {
    mn.reset_monotone_query();
    mx.reset_monotone_query();
  }

  // 係数点の凸包頂点を反時計回りに返す。
  vc<LinearForm> convex_hull() const
  {
    vc<LinearForm> res;
    append_chain(mn, res);
    append_chain(mx, res);
    if (res.size() > 1 && same_point(res.front(), res.back())) res.pop_back();
    return res;
  }

  // すべての一次式、単調性の状態、自動採番を消去する。
  void clear()
  {
    mn.clear();
    mx.clear();
    add_cnt = 0;
    has_line = false;
  }
};

// 係数を任意順に追加できる Dot Product Convex Hull Trick
template <class T = ll, class Calc = larger_int_t<T>, Calc infty = Calc(INF)>
struct ConvexHullTrickDotProduct
    : ConvexHullTrickDotProductBase<T, Calc, infty, ConvexHullTrick>
{};

// 係数 a を単調に追加する Dot Product Convex Hull Trick
template <class T = ll, class Calc = larger_int_t<T>, Calc infty = Calc(INF)>
struct ConvexHullTrickDotProductMonotoneSlope
    : ConvexHullTrickDotProductBase<T, Calc, infty, ConvexHullTrickMonotoneSlope>
{};
