#pragma once

#include "../template/template_all_but_modint.hpp"

#include "modint/modint.hpp"
#include "prime/large/factorize.hpp"
#include "svp2d.hpp"

// 素数 p に対し、p = a^2 + b^2 となる (a, b)
// のうち 0 <= a <= b であるもの (1 通り) を求める
// (存在するか, ペア)
// 表し方が存在しない <=> p が 4n+3 型素数
template <class T>
pair<bool, pair<T, T>> two_square_sum_prime(T p)
{
  assert(is_prime(p));
  if (p == 2)
    return {true, {1, 1}};
  if (p % 4 == 3)
    return {false, {}};
  ll i;
  if (p <= INT_MAX)
  {
    using mint = dynamic_modint32<INT_MIN>;
    mint::set_mod(p);
    mint ii = 1;
    rep(v, 2, p)
    {
      ii = mint(v).pow((p - 1) / 4);
      if (ii * ii == p - 1)
        break;
    }
    i = ii.val();
  }
  else
  {
    using mint = dynamic_modint64_odd<INT_MIN>;
    mint::set_mod(p);
    mint ii = 1;
    rep(v, 2, p)
    {
      ii = mint(v).pow((p - 1) / 4);
      if (ii * ii == p - 1)
        break;
    }
    i = ii.val();
  }
  auto [a, b] = svp2d<T>({1, i}, {0, p});
  return {true, minmax({abs(a), abs(b)})};
}

// 整数 n に対し n = a^2 + b^2 となる正整数 a, b の組 (0 <= a <= b) をすべて求める
vc<pair<ll, ll>> two_square_sum(ll n)
{
  if (n < 0)
    return {};
  if (n == 0)
    return {{0, 0}};
  ll sb = 1;
  vc<pair<ll, ll>> cds;
  fec([ p, e, pe ] : factorize(n))
  {
    if (p % 4 == 3)
    {
      if (e % 2 != 0)
        return {};
      sb *= ipow(p, e / 2);
      continue;
    }
    auto [ok, cd] = two_square_sum_prime(p);
    repi(_, e) cds.eb(cd);
  }
  vc<pair<ll, ll>> ans = {{0, sb}}, nans;
  fec([ c, d ] : cds)
  {
    nans.clear();
    fec([ a, b ] : ans)
    {
      nans.eb(minmax({abs(a * d - b * c), a * c + b * d}));
      nans.eb(minmax({abs(a * c - b * d), a * d + b * c}));
    }
    swap(ans, nans);
    sortunique(ans);
  }
  return ans;
}
