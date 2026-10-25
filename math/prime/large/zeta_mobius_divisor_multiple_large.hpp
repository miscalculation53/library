#pragma once

#include "template/template_all_but_modint.hpp"
#include "math/prime/large/factorize.hpp"
#include "algebra/algebra_basic_ops.hpp"

/**
 * @brief 約数・倍数 ゼータ・メビウス変換（大きい $m$ の約数）
 * @docs docs/math/prime/large/zeta_mobius_divisor_multiple_large.md
 */

struct ZetaMobiusDivisorMultipleLarge
{
public:
  ll m;
  vc<PrimePower<ll>> fac;
  ll pnum, dnum;
  vc<ll> ds;

private:
  vc<int> f01;  // f01[d] は、d が f[j] == e[j] なら j ビット目が 1

public:
  ZetaMobiusDivisorMultipleLarge() {}
  template <class I>
  ZetaMobiusDivisorMultipleLarge(ll m, const vc<PrimePower<I>> &fac) : m(m), fac(ALL(fac))
  {
    pnum = fac.size();
    dnum = 1;
    fec(pp : fac) dnum *= pp.e + 1;

    ds.resize(dnum), f01.resize(dnum);
    vc<int> f(pnum, 0);
    ll d = 1;
    for (int i = 0;; i++)
    {
      ds[i] = d;
      rep(j, pnum) bset(f01[i], j, f[j] == fac[j].e);
      if (i == dnum - 1)
        break;
      rep(j, pnum - 1, -1, -1)
      {
        if (f[j] == fac[j].e)
        {
          f[j] = 0;
          d /= fac[j].pe;
        }
        else
        {
          f[j]++;
          d *= fac[j].p;
          break;
        }
      }
    }
  }

public:
  // d から ds[i] = d となる i
  int dtoi(ll d) const
  {
    //                f3
    // +       (e3+1) f2
    // + (e2+1)(e3+1) f1
    // = f3+(e3+1)(f2+(e2+1)f1)
    assert(d > 0 && m % d == 0);
    int res = 0;
    fec(pp : fac)
    {
      auto [f, pf, nd] = ord_pow_div(d, pp.p);
      d = nd;
      res *= pp.e + 1;
      res += f;
    }
    return res;
  }
  /*
  // f3 = i % (e3+1)
  // f2 = (i // (e3+1)) % (e2+1)
  ll itod(int i) const
  {
    ll d = 1;
    fec(pp : reversed(fac))
    {
      d *= ipow(pp.p, i % (pp.e + 1));
      i /= pp.e + 1;
    }
    return d;
  }
  //*/

public:
  template <class T>
  struct DivisorMap
  {
  private:
    const ZetaMobiusDivisorMultipleLarge &zm;
    vc<T> v;

    friend struct ZetaMobiusDivisorMultipleLarge;

  public:
    DivisorMap() {}
    DivisorMap(const ZetaMobiusDivisorMultipleLarge &zm)
    : zm(zm), v(zm.dnum) {}
    template <class F>
    DivisorMap(const ZetaMobiusDivisorMultipleLarge &zm, const F &func)
    : zm(zm), v(zm.dnum) { repi(i, zm.dnum) v[i] = func(zm.ds[i]); }

    // m の約数 d に対して値を取得
    // 変更も可能
    // O(loglog d)
    T &get_by_d(ll d) { return v[zm.dtoi(d)]; }
    // m の約数 d に対して値を取得
    // 変更も可能
    // O(loglog d)
    const T &get_by_d(ll d) const { return v[zm.dtoi(d)]; }

    // m の約数 d に対し、i = dtoi(d) から値を取得
    // 変更も可能
    T &get_by_i(ll i) { return v[i]; }
    // m の約数 d に対し、i = dtoi(d) から値を取得
    // 変更も可能
    const T &get_by_i(ll i) const { return v[i]; }

    map<ll, T> to_map()
    {
      map<ll, T> res;
      fec(d : zm.ds) res[d] = get_by_d(d);
      return res;
    }
  };

  template <class T>
  DivisorMap<T> divisor_map() const
  { return DivisorMap<T>(*this); }
  template <class F>
  auto divisor_map(const F &func) -> DivisorMap<decltype(func(0))> const
  { return DivisorMap<decltype(func(0))>(*this, func); }

  // ζa(n) = Σ{d | n} a(d)
  // M は可換モノイド (Σ だと +)
  // O(約数個数 * 素因数個数)
  template <class M>
  DivisorMap<typename M::S> zeta_divisor
  (const DivisorMap<typename M::S> &a) const
  {
    auto b = a;
    for (int j = pnum - 1, k = 1; j >= 0; k *= fac[j].e + 1, j--)
    {
      repi(i, dnum)
      {
        if (!btest(f01[i], j))
          b.v[i + k] = M::op(b.v[i + k], b.v[i]);
      }
    }
    return b;
  }

  // μ は ζ の逆変換
  // μa(n) = Σ{d | n} μ(n/d)a(d)  cf. メビウスの反転公式
  // G は可換群 (Σ だと +, -)
  // O(約数個数 * 素因数個数)
  template <class G>
  DivisorMap<typename G::S> mobius_divisor
  (const DivisorMap<typename G::S> &a) const
  {
    auto b = a;
    for (int j = pnum - 1, k = 1; j >= 0; k *= fac[j].e + 1, j--)
    {
      repi(i, dnum - 1, -1, -1)
      {
        if (!btest(f01[i], j))
          b.v[i + k] = G::op(b.v[i + k], G::inv(b.v[i]));
      }
    }
    return b;
  }

  // μ は ζ の逆変換
  // μa(n) = Σ{d | n} μ(n/d)a(d)  cf. メビウスの反転公式
  // μa(n) の 1 点だけ欲しいときに使う
  // G は可換群 (Σ だと +, -)
  // O(素因数個数 * 2^素因数個数)
  template <class G>
  typename G::S mobius_divisor_point
  (const DivisorMap<typename G::S> &a, ll n) const
  {
    typename G::S res = G::e();
    int si = dtoi(n);
    repi(bit, 1 << pnum)
    {
      int i = si;
      bool ok = true;
      for (int j = pnum - 1, k = 1; j >= 0; k *= fac[j].e + 1, j--)
      {
        if (btest(bit, j))
        {
          if (i - k >= 0 && !btest(f01[i - k], j))
            i -= k;
          else
          {
            ok = false;
            break;
          }
        }
      }
      if (!ok)
        continue;
      if (popcount(bit) % 2 == 0)
        res = G::op(res, a.v[i]);
      else
        res = G::op(res, G::inv(a.v[i]));
    }
    return res;
  }

  // ζ'a(n) = Σ{n | m} a(m)
  // M は可換モノイド (Σ だと +)
  // O(約数個数 * 素因数個数)
  template <class M>
  DivisorMap<typename M::S> zeta_multiple
  (const DivisorMap<typename M::S> &a) const
  {
    auto b = a;
    for (int j = pnum - 1, k = 1; j >= 0; k *= fac[j].e + 1, j--)
    {
      repi(i, dnum - 1, -1, -1)
      {
        if (!btest(f01[i], j))
          b.v[i] = M::op(b.v[i], b.v[i + k]);
      }
    }
    return b;
  }

  // μ' は ζ' の逆変換
  // μ'a(n) = Σ{n | m} μ(m/n)g(m)  cf. メビウスの反転公式
  // G は可換群 (Σ だと +, -)
  // O(約数個数 * 素因数個数)
  template <class G>
  DivisorMap<typename G::S> mobius_multiple
  (const DivisorMap<typename G::S> &a) const
  {
    auto b = a;
    for (int j = pnum - 1, k = 1; j >= 0; k *= fac[j].e + 1, j--)
    {
      repi(i, dnum)
      {
        if (!btest(f01[i], j))
          b.v[i] = G::op(b.v[i], G::inv(b.v[i + k]));
      }
    }
    return b;
  }

  // μ' は ζ' の逆変換
  // μ'a(n) = Σ{n | m} μ(m/n)g(m)  cf. メビウスの反転公式
  // μ'a(n) の 1 点だけ欲しいときに使う
  // G は可換群 (Σ だと +, -)
  // O(素因数個数 * 2^素因数個数)
  template <class G>
  typename G::S mobius_multiple_point
  (const DivisorMap<typename G::S> &a, ll n) const
  {
    typename G::S res = G::e();
    int si = dtoi(n);
    repi(bit, 1 << pnum)
    {
      int i = si;
      bool ok = true;
      for (int j = pnum - 1, k = 1; j >= 0; k *= fac[j].e + 1, j--)
      {
        if (btest(bit, j))
        {
          if (!btest(f01[i], j))
            i += k;
          else
          {
            ok = false;
            break;
          }
        }
      }
      if (!ok)
        continue;
      if (popcount(bit) % 2 == 0)
        res = G::op(res, a.v[i]);
      else
        res = G::op(res, G::inv(a.v[i]));
    }
    return res;
  }
};
