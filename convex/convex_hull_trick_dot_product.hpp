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

template <class T, class Calc, Calc infty, bool monotone_coefficient>
struct ConvexHullTrickDotProductBase
{
  static_assert(is_integral_ext<T> || is_floating_point_v<T>);
  static_assert(is_signed_ext<T> || is_floating_point_v<T>);

  struct LinearForm
  {
    T a, b;
    int id = -1;

    friend bool operator==(const LinearForm &x, const LinearForm &y)
    {
      return x.a == y.a && x.b == y.b && x.id == y.id;
    }
  };

private:
  using R = Rational<T>;
  using RationalLine = ConvexHullTrickLine<R>;

  template <class Objective>
  using Hull = conditional_t<monotone_coefficient,
                             ConvexHullTrickMonotoneSlope<R, Objective>,
                             ConvexHullTrick<R, Objective>>;

  Hull<less<>> lo;
  Hull<greater<>> hi;
  optional<LinearForm> fst, mna, mxa;
  mutable vc<LinearForm> ch;
  mutable bool built = false;
  int add_cnt = 0;
  array<int, 2> qpos{-1, -1}, qdir{0, 0};
  array<optional<pair<T, T>>, 2> last_q;

  static LinearForm form(const RationalLine &f)
  {
    return {f.a.num, f.b.num, f.id};
  }

  static Calc dot(const LinearForm &f, T x, T y)
  {
    return Calc(f.a) * Calc(x) + Calc(f.b) * Calc(y);
  }

  static Calc cross(const LinearForm &o, const LinearForm &a, const LinearForm &b)
  {
    return (Calc(a.a) - Calc(o.a)) * (Calc(b.b) - Calc(o.b)) -
           (Calc(a.b) - Calc(o.b)) * (Calc(b.a) - Calc(o.a));
  }

  template <bool minimize>
  static bool better(Calc x, Calc y)
  {
    if constexpr (minimize)
      return x < y;
    else
      return y < x;
  }

  static int half(T x, T y) { return y > 0 || (y == 0 && x >= 0) ? 0 : 1; }

  static bool arg_less(T ax, T ay, T bx, T by)
  {
    int ah = half(ax, ay), bh = half(bx, by);
    if (ah != bh) return ah < bh;
    return Calc(ax) * Calc(by) - Calc(ay) * Calc(bx) > 0;
  }

  template <class H>
  static void append_forms(const H &h, vc<LinearForm> &forms)
  {
    for (const auto &seg : h.segments()) forms.eb(form(seg.line));
  }

  void build() const
  {
    if (built) return;
    vc<LinearForm> pts;
    append_forms(lo, pts);
    append_forms(hi, pts);
    sort(pts.begin(), pts.end(), [](const LinearForm &x, const LinearForm &y)
         { return tie(x.a, x.b, x.id) < tie(y.a, y.b, y.id); });
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    if (pts.size() <= 1)
      ch = pts;
    else
    {
      vc<LinearForm> low, high;
      for (const LinearForm &p : pts)
      {
        while (low.size() >= 2 && cross(low[low.size() - 2], low.back(), p) <= 0)
          low.pop_back();
        low.eb(p);
      }
      for (auto it = pts.rbegin(); it != pts.rend(); ++it)
      {
        while (high.size() >= 2 && cross(high[high.size() - 2], high.back(), *it) <= 0)
          high.pop_back();
        high.eb(*it);
      }
      low.pop_back();
      high.pop_back();
      ch = std::move(low);
      ch.insert(ch.end(), high.begin(), high.end());
    }
    built = true;
  }

  template <bool minimize>
  LinearForm best(T x, T y) const
  {
    if (x == 0 && y == 0) return *fst;
    if (y == 0)
    {
      if constexpr (minimize)
        return x > 0 ? *mna : *mxa;
      else
        return x > 0 ? *mxa : *mna;
    }
    R q(x, y);
    if constexpr (minimize)
      return form(y > 0 ? lo.query_line(q) : hi.query_line(q));
    else
      return form(y > 0 ? hi.query_line(q) : lo.query_line(q));
  }

  template <bool minimize>
  LinearForm best_monotone(T x, T y)
  {
    assert(x != 0 || y != 0);
    build();
    constexpr int k = minimize ? 0 : 1;
    int &pos = qpos[k], &dir = qdir[k];
    auto &last = last_q[k];
    if (pos == -1)
    {
      pos = 0;
      repi(i, 1, ch.size()) if (better<minimize>(dot(ch[i], x, y), dot(ch[pos], x, y))) pos = i;
    }
    else
    {
      auto [lx, ly] = *last;
      int d = arg_less(lx, ly, x, y) - arg_less(x, y, lx, ly);
      if (d)
      {
        assert((!dir || d == dir) && "query angle is not monotone");
        dir = d;
      }
      int step = dir < 0 ? -1 : 1, n = SZ(ch);
      repi(_, ch.size() - 1)
      {
        int nxt = (pos + step + n) % n;
        if (!better<minimize>(dot(ch[nxt], x, y), dot(ch[pos], x, y))) break;
        pos = nxt;
      }
    }
    last = pair{x, y};
    return ch[pos];
  }

  template <bool minimize>
  pair<Calc, LinearForm> query_with_form_impl(T x, T y) const
  {
    if (!fst) return {minimize ? infty : -infty, {0, 0, -1}};
    LinearForm f = best<minimize>(x, y);
    return {dot(f, x, y), f};
  }

  template <bool minimize>
  Calc query_impl(T x, T y) const
  {
    if (!fst) return minimize ? infty : -infty;
    return dot(best<minimize>(x, y), x, y);
  }

  template <bool minimize>
  pair<Calc, LinearForm> query_monotone_with_form_impl(T x, T y)
  {
    if (!fst) return {minimize ? infty : -infty, {0, 0, -1}};
    LinearForm f = best_monotone<minimize>(x, y);
    return {dot(f, x, y), f};
  }

  template <bool minimize>
  Calc query_monotone_impl(T x, T y)
  {
    if (!fst) return minimize ? infty : -infty;
    return dot(best_monotone<minimize>(x, y), x, y);
  }

public:
  // ax + by を追加する。id は 0 始まりの追加順になる。
  void add(T a, T b) { add(a, b, add_cnt); }

  // ax + by を指定した id で追加する。単調係数版では a の増減方向を自動で判定する。
  void add(T a, T b, int id)
  {
    add_cnt++;
    reset_monotone_query();
    LinearForm f{a, b, id};
    lo.add_line(R(a), R(b), id);
    hi.add_line(R(a), R(b), id);
    if (!fst) fst = f;
    if (!mna || a < mna->a) mna = f;
    if (!mxa || mxa->a < a) mxa = f;
    built = false;
    ch.clear();
  }

  // (x, y) における最小値を返す。空なら infty を返す。
  Calc min_query(T x, T y) const { return query_impl<true>(x, y); }

  // (x, y) における最大値を返す。空なら -infty を返す。
  Calc max_query(T x, T y) const { return query_impl<false>(x, y); }

  // (x, y) における最小値と一次式を返す。空なら infty とダミー一次式を返す。
  pair<Calc, LinearForm> min_query_with_form(T x, T y) const
  {
    return query_with_form_impl<true>(x, y);
  }

  // (x, y) における最大値と一次式を返す。空なら -infty とダミー一次式を返す。
  pair<Calc, LinearForm> max_query_with_form(T x, T y) const
  {
    return query_with_form_impl<false>(x, y);
  }

  // 偏角が単調な (x, y) における最小値を返す。方向は自動で判定する。
  Calc min_query_monotone(T x, T y) { return query_monotone_impl<true>(x, y); }

  // 偏角が単調な (x, y) における最大値を返す。方向は自動で判定する。
  Calc max_query_monotone(T x, T y) { return query_monotone_impl<false>(x, y); }

  // 偏角が単調な (x, y) における最小値と一次式を返す。方向は自動で判定する。
  pair<Calc, LinearForm> min_query_monotone_with_form(T x, T y)
  {
    return query_monotone_with_form_impl<true>(x, y);
  }

  // 偏角が単調な (x, y) における最大値と一次式を返す。方向は自動で判定する。
  pair<Calc, LinearForm> max_query_monotone_with_form(T x, T y)
  {
    return query_monotone_with_form_impl<false>(x, y);
  }

  // 単調クエリの現在位置と方向を消去する。
  void reset_monotone_query()
  {
    qpos = {-1, -1};
    qdir = {0, 0};
    last_q = {};
  }

  // 係数点の凸包頂点を反時計回りに返す。
  vc<LinearForm> convex_hull() const
  {
    build();
    return ch;
  }

  // すべての一次式、単調性の状態、自動採番を消去する。
  void clear()
  {
    lo.clear();
    hi.clear();
    fst.reset();
    mna.reset();
    mxa.reset();
    add_cnt = 0;
    ch.clear();
    built = false;
    reset_monotone_query();
  }
};

// 係数を任意順に追加できる Dot Product Convex Hull Trick
template <class T = ll, class Calc = larger_int_t<T>, Calc infty = Calc(INF)>
struct ConvexHullTrickDotProduct
    : ConvexHullTrickDotProductBase<T, Calc, infty, false>
{};

// 係数 a を単調に追加する Dot Product Convex Hull Trick
template <class T = ll, class Calc = larger_int_t<T>, Calc infty = Calc(INF)>
struct ConvexHullTrickDotProductMonotoneCoefficient
    : ConvexHullTrickDotProductBase<T, Calc, infty, true>
{};
