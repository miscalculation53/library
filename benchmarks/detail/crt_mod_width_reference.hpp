#pragma once
// 値による切り替えを入れる前の実装。
#include "math/crt.hpp"
template <class mint, class V1, class V2>
pair<mint, mint> crt_mod_before_width(const V1 &rs, const V2 &ms)
{
  using T = decay_t<decltype(ms[0])>;
  assert(rs.size() == ms.size());
  const int n = rs.size();
  mint r = 0, m = 1;

  if constexpr (sizeof(T) <= 4)
  {
    vc<internal::barrett32> ba;
    ba.reserve(n);
    repi(i, n) ba.eb(ms[i]);
    vc<uint> rr(n, 0), mm(n, 1);
    repi(i, n)
    {
      assert(ms[i] >= 1);
      auto [g, im, _] = extgcd<ll>(mm[i], ms[i]);
      assert(g == 1);
      if (im < 0)
        im += ms[i];
      ll diff = safemod(ll(rs[i]) - rr[i], ms[i]);
      uint t = ba[i].mul(diff, im);
      r += t * m, m *= ms[i];
      repi(j, i + 1, n)
      {
        rr[j] += ba[j].mul(t, mm[j]);
        if (rr[j] >= (uint)ms[j])
          rr[j] -= ms[j];
        mm[j] = ba[j].mul(mm[j], ms[i]);
      }
    }
  }
  else
  {
    vc<ull> rr(n, 0), mm(n, 1);
    repi(i, n)
    {
      assert(ms[i] >= 1);
      auto [g, im, _] = extgcd<ll>(mm[i], ms[i]);
      assert(g == 1);
      if (im < 0)
        im += ms[i];
      i128 diff = safemod((i128)rs[i] - rr[i], ms[i]);
      ull t = (ull)((u128)diff * im % ms[i]);
      r += t * m, m *= ms[i];
      repi(j, i + 1, n)
      {
        rr[j] += (ull)((u128)t * mm[j] % ms[j]);
        if (rr[j] >= (ull)ms[j]) rr[j] -= ms[j];
        mm[j] = (ull)((u128)mm[j] * ms[i] % ms[j]);
      }
    }
  }
  return {r, m};
}

