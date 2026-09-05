#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../algebra_base.hpp"
#include "../clamp_add.hpp"

/**
 * @brief 作用つきモノイド：chmin・chmax・加算、最小値・最大値・和取得
 * @docs docs/algebra/acted_monoid/chmin_chmax_add_min_max_sum.md
 */

template <class T, auto infty = INF>
struct MonoidMinMaxSumBeats
{
  struct S
  {
    T mn = resolved_value<T, infty>(), mx = -resolved_value<T, infty>();
    T mn2 = resolved_value<T, infty>(), mx2 = -resolved_value<T, infty>(), sum = T{};
    int len = 0, mn_cnt = 0, mx_cnt = 0;
    bool fail = false;
    S() {}
    S(T x, int len = 1)
        : mn(x), mx(x), sum(x * len), len(len), mn_cnt(len), mx_cnt(len) {}
    CPP_DUMP_DEFINE_DATA(mn, mx, mn2, mx2, sum, len, mn_cnt, mx_cnt, fail);
  };

private:
  static constexpr T second_min(T a, T a2, T b, T b2)
  {
    if (a == b)
      return min(a2, b2);
    if (a2 <= b)
      return a2;
    if (b2 <= a)
      return b2;
    return max(a, b);
  }
  static constexpr T second_max(T a, T a2, T b, T b2)
  {
    if (a == b)
      return max(a2, b2);
    if (a2 >= b)
      return a2;
    if (b2 >= a)
      return b2;
    return min(a, b);
  }

public:
  static constexpr S op(const S &a, const S &b)
  {
    if (a.len == 0)
      return b;
    if (b.len == 0)
      return a;
    S c;
    c.mn = min(a.mn, b.mn), c.mx = max(a.mx, b.mx);
    c.mn2 = second_min(a.mn, a.mn2, b.mn, b.mn2);
    c.mx2 = second_max(a.mx, a.mx2, b.mx, b.mx2);
    c.sum = a.sum + b.sum, c.len = a.len + b.len;
    c.mn_cnt = a.mn_cnt * (a.mn <= b.mn) + b.mn_cnt * (b.mn <= a.mn);
    c.mx_cnt = a.mx_cnt * (a.mx >= b.mx) + b.mx_cnt * (b.mx >= a.mx);
    return c;
  }
  static constexpr S e() { return {}; }
};

template <class T, auto infty = INF>
struct ActedMonoidChminChmaxAddMinMaxSum
{
  using M = MonoidMinMaxSumBeats<T, infty>;
  using MF = MonoidClampAdd<T, infty>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(const F &f, S x)
  {
    if (x.len == 0 || x.fail)
      return x;
    if (x.mn == x.mx || f.lo == f.hi || f.lo >= x.mx || f.hi <= x.mn)
      return S(f(x.mn), x.len);
    if (x.mn2 == x.mx)
    {
      x.mn = x.mx2 = f(x.mn);
      x.mx = x.mn2 = f(x.mx);
      x.sum = x.mn * x.mn_cnt + x.mx * x.mx_cnt;
      return x;
    }
    if (f.lo < x.mn2 && x.mx2 < f.hi)
    {
      T nxt_mn = max(x.mn, f.lo), nxt_mx = min(x.mx, f.hi);
      x.sum += (nxt_mn - x.mn) * x.mn_cnt - (x.mx - nxt_mx) * x.mx_cnt + f.bias * x.len;
      x.mn = nxt_mn + f.bias, x.mx = nxt_mx + f.bias;
      x.mn2 += f.bias, x.mx2 += f.bias;
      return x;
    }
    x.fail = true;
    return x;
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};
