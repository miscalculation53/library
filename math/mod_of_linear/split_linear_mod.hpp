#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 一次式の剰余列の等差数列分割
 * @docs docs/math/mod_of_linear/split_linear_mod.md
 */

struct LinearModArithmetic
{
  ll n, a, b;
  ll l, gap;

  ll operator()(ll i) const
  {
    assert(0 <= i && i < n);
    return a * i + b;
  }
  ll origin_index(ll i) const
  {
    assert(0 <= i && i < n);
    return gap * i + l;
  }
};

// (a*i+b) mod m (0 <= i < n) を分割する。n <= m/gcd(a,m)。
// 各部分列の元の添字は l, l+gap, ...。全体の順序は並べ替わる。
// https://codeforces.com/blog/entry/141889
inline vc<LinearModArithmetic> split_linear_mod(ll n, ll m, ll a, ll b)
{
  assert(n >= 0 && m >= 1);
  a %= m;
  b %= m;
  if (a < 0) a += m;
  if (b < 0) b += m;
  assert(n <= m / gcd(a, m));
  if (n == 0) return {};
  if (n == 1) return {{1, 0, b, 0, 1}};

  // s+1 個の剰余の隣接差に、m/s 未満のものが存在する。
  const ll s = iroot(n, 2);
  // x+a を作る前に折り返す。引数・戻り値は [0,m)。
  auto advance = [&](ll x) { return x >= m - a ? x - (m - a) : x + a; };
  vc<pll> sample;
  for (ll i = 0, value = 0; i <= s; ++i, value = advance(value))
    sample.eb(value, i);
  sort(ALL(sample));
  ll step = m, gap = 0;
  for (ll i = 1; i <= s; ++i)
  {
    const ll diff = sample[i].first - sample[i - 1].first;
    if (diff < abs(step))
    {
      gap = sample[i].second - sample[i - 1].second;
      step = gap > 0 ? diff : -diff;
      gap = abs(gap);
    }
  }

  vc<LinearModArithmetic> res;
  for (ll l = 0, first = b; l < gap; ++l, first = advance(first))
  {
    ll pos = l;
    ll value = first;
    for (;;)
    {
      const ll remain = (n - 1 - pos) / gap + 1;
      const ll len = min(remain, step > 0 ? (m - 1 - value) / step + 1 : value / (-step) + 1);
      res.push_back({len, step, value, pos, gap});
      if (len == remain) break;
      pos += len * gap;
      // 最後の有効な項から 1 回折り返す。各中間値も ll に収まる。
      value += step * (len - 1);
      value += step > 0 ? step - m : step + m;
    }
  }
  return res;
}
