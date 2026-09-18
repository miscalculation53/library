#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../upd.hpp"
#include "../../utils/resolved_infty.hpp"
#include "../../utils/make_unsigned_ext.hpp"

/**
 * @brief 作用つきモノイド：等差数列代入・最小値／最大値取得
 * @docs docs/algebra/acted_monoid/ariupd_min.md
 */

template <class T, auto infty = nullptr>
struct ActedMonoidAriUpdMin
{
  struct S
  {
    int l = 0, r = 0;
    T mn = resolved_infty<T, infty>();
    S() {}
    S(int i, T val) : l(i), r(i + 1), mn(val) {}
    S(int l, int r, T mn) : l(l), r(r), mn(mn) {}
    CPP_DUMP_DEFINE_DATA(l, r, mn);
  };
  using MF = MonoidUpdFlag<array<T, 2>>;
  using F = typename MF::S;
  static constexpr S op(const S &a, const S &b)
  {
    if (a.l == a.r) return b;
    if (b.l == b.r) return a;
    return {min(a.l, b.l), max(a.r, b.r), min(a.mn, b.mn)};
  }
  static constexpr S e() { return {}; }
  static constexpr S mapping(const F &f, S x)
  {
    if (f.is_id || x.l == x.r)
      return x;
    const int i = f.val[1] < T{} ? x.r - 1 : x.l;
    if constexpr (is_integral_ext<T>)
    {
      using U = decltype(0U + make_unsigned_ext_t<T>{});
      x.mn = T(U(f.val[0]) + U(f.val[1]) * U(i));
    }
    else
      x.mn = f.val[0] + f.val[1] * T(i);
    return x;
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};

template <class T, auto infty = nullptr>
struct ActedMonoidAriUpdMax
{
  struct S
  {
    int l = 0, r = 0;
    T mx = -resolved_infty<T, infty>();
    S() {}
    S(int i, T val) : l(i), r(i + 1), mx(val) {}
    S(int l, int r, T mx) : l(l), r(r), mx(mx) {}
    CPP_DUMP_DEFINE_DATA(l, r, mx);
  };
  using MF = MonoidUpdFlag<array<T, 2>>;
  using F = typename MF::S;
  static constexpr S op(const S &a, const S &b)
  {
    if (a.l == a.r) return b;
    if (b.l == b.r) return a;
    return {min(a.l, b.l), max(a.r, b.r), max(a.mx, b.mx)};
  }
  static constexpr S e() { return {}; }
  static constexpr S mapping(const F &f, S x)
  {
    if (f.is_id || x.l == x.r)
      return x;
    const int i = f.val[1] < T{} ? x.l : x.r - 1;
    if constexpr (is_integral_ext<T>)
    {
      using U = decltype(0U + make_unsigned_ext_t<T>{});
      x.mx = T(U(f.val[0]) + U(f.val[1]) * U(i));
    }
    else
      x.mx = f.val[0] + f.val[1] * T(i);
    return x;
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};

template <class T, auto infty = nullptr>
struct ActedMonoidAriUpdMinMax
{
  struct S
  {
    int l = 0, r = 0;
    T mn = resolved_infty<T, infty>(), mx = -resolved_infty<T, infty>();
    S() {}
    S(int i, T val) : l(i), r(i + 1), mn(val), mx(val) {}
    S(int l, int r, T mn, T mx) : l(l), r(r), mn(mn), mx(mx) {}
    CPP_DUMP_DEFINE_DATA(l, r, mn, mx);
  };
  using MF = MonoidUpdFlag<array<T, 2>>;
  using F = typename MF::S;
  static constexpr S op(const S &a, const S &b)
  {
    if (a.l == a.r) return b;
    if (b.l == b.r) return a;
    return {min(a.l, b.l), max(a.r, b.r), min(a.mn, b.mn), max(a.mx, b.mx)};
  }
  static constexpr S e() { return {}; }
  static constexpr S mapping(const F &f, S x)
  {
    if (f.is_id || x.l == x.r)
      return x;
    T a, b;
    if constexpr (is_integral_ext<T>)
    {
      using U = decltype(0U + make_unsigned_ext_t<T>{});
      a = T(U(f.val[0]) + U(f.val[1]) * U(x.l));
      b = T(U(f.val[0]) + U(f.val[1]) * U(x.r - 1));
    }
    else
    {
      a = f.val[0] + f.val[1] * T(x.l);
      b = f.val[0] + f.val[1] * T(x.r - 1);
    }
    x.mn = min(a, b), x.mx = max(a, b);
    return x;
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};
