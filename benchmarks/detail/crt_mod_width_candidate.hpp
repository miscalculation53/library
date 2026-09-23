#pragma once
// 調査用。少数の式では従来法より遅くなったため、本体への適用は見送った。
#include "math/crt.hpp"
template <class mint, class V1, class V2>
pair<mint, mint> crt_mod_width_candidate(const V1 &rs, const V2 &ms)
{
  using T = decay_t<decltype(ms[0])>;
  assert(rs.size() == ms.size());
  const int n = rs.size();
  mint r = 0, m = 1;

  // 既存の 32 bit 経路では rr の加算も uint で行う。
  // ll に格納された小さい法も同じ経路で処理する。
  bool small = true;
  if constexpr (sizeof(T) > 4 || (sizeof(T) == 4 && is_unsigned_v<T>))
    for (auto mod : ms)
      if (mod > INT_MAX) { small = false; break; }
  if (small && (sizeof(T) <= 4 || n >= 16))
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
      ll diff = rs[i] % ll(ms[i]);
      if (diff < 0) diff += ms[i];
      diff -= rr[i];
      if (diff < 0) diff += ms[i];
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
    auto solve = [&](auto word) -> pair<mint, mint>
    {
      using W = decltype(word);
      vc<ull> rr(n, 0), mm(n, 1);
      repi(i, n)
      {
        assert(ms[i] >= 1);
        auto [g, im, _] = extgcd<ll>(mm[i], ms[i]);
        assert(g == 1);
        if (im < 0) im += ms[i];
        ll diff = rs[i] % ll(ms[i]);
        if (diff < 0) diff += ms[i];
        diff -= rr[i];
        if (diff < 0) diff += ms[i];
        ull t = W(diff) * im % ms[i];
        r += t * m, m *= ms[i];
        repi(j, i + 1, n)
        {
          rr[j] += ull(W(t) * mm[j] % ms[j]);
          if (rr[j] >= ull(ms[j])) rr[j] -= ms[j];
          mm[j] = W(mm[j]) * ms[i] % ms[j];
        }
      }
      return {r, m};
    };
    // 小さい法では、少数の式を通常の 64 bit 除算で処理する。
    if (small) return solve(ull(0));
    return solve(u128(0));
  }
  return {r, m};
}

