#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

//! SPDX-License-Identifier: CC0-1.0

#include "algebra/min_max_sum.hpp"
#include "algebra/prefix_min_max.hpp"
#include "algebra/power_sum.hpp"
#include "algebra/index_power_sum.hpp"
#include "algebra/multivariate_power_sum.hpp"
#include "algebra/majority_vote.hpp"
#include "algebra/upd.hpp"
#include "algebra/clamp_add.hpp"
#include "algebra/clamp_add_cost.hpp"
#include "algebra/acted_monoid/add_min_count.hpp"
#include "algebra/acted_monoid/upd_min.hpp"
#include "algebra/acted_monoid/clamp_min_max.hpp"
#include "algebra/acted_monoid/ariupd_min.hpp"
#include "algebra/acted_monoid/chmin_chmax_add_min_max_sum.hpp"

// Compare public pow against the definition using only op and e.
template <class M, class I> auto reference_power(typename M::S x, I k)
{
  auto res = M::e();
  while (k)
  {
    if (k & 1)
      res = M::op(res, x);
    k >>= 1;
    if (k)
      x = M::op(x, x);
  }
  return res;
}

void test_numeric_formulas()
{
  using MM = MonoidMinMax<ll>;
  using Min = MonoidMinSum<ll>;
  using Max = MonoidMaxSum<ll>;
  using Sum = MonoidMinMaxSum<ll>;
  using Pre = MonoidPrefixMinMax<ll, true>;
  using PS = MonoidPowerSum<ll, 4>;
  using IS = MonoidIndexPowerSum<ll, 4>;
  using MS = MonoidMultivariatePowerSum<ll, 2, 3>;
  using B = MonoidMinMaxSumBeats<ll>;
  mt19937_64 rng(20261028);
  int checked = 0;
  for (int iter = 0; iter < 3000; ++iter)
  {
    auto mm = MM::e();
    auto mn = Min::e();
    auto mx = Max::e();
    auto s = Sum::e();
    auto pre = Pre::e();
    auto ps = PS::e();
    auto is = IS::e();
    auto ms = MS::e();
    auto b = B::e();
    int n = rng() % 9;
    for (int i = 0; i < n; ++i)
    {
      ll v = ll(rng() % 7) - 3;
      mm = MM::op(mm, MM::S(v));
      mn = Min::op(mn, Min::S(v));
      mx = Max::op(mx, Max::S(v));
      s = Sum::op(s, Sum::S(v));
      pre = Pre::op(pre, Pre::S(v));
      ps = PS::op(ps, PS::S(v));
      is = IS::op(is, IS::S(i, v));
      ms = MS::op(ms, MS::S({v, 2 - v}));
      b = B::op(b, B::S(v));
    }
    for (ll k : {0, 1, 2, 3, 8, 63, 1000})
    {
      auto want_mm = reference_power<MM>(mm, k);
      auto got_mm = MM::pow(mm, k);
      assert(want_mm.mn == got_mm.mn && want_mm.mx == got_mm.mx);
      auto want_min = reference_power<Min>(mn, k), got_min = Min::pow(mn, k);
      auto want_max = reference_power<Max>(mx, k), got_max = Max::pow(mx, k);
      auto want_sum = reference_power<Sum>(s, k), got_sum = Sum::pow(s, k);
      assert(tie(want_min.mn, want_min.sum, want_min.len) == tie(got_min.mn, got_min.sum, got_min.len));
      assert(tie(want_max.mx, want_max.sum, want_max.len) == tie(got_max.mx, got_max.sum, got_max.len));
      assert(tie(want_sum.mn, want_sum.mx, want_sum.sum, want_sum.len) ==
             tie(got_sum.mn, got_sum.mx, got_sum.sum, got_sum.len));
      auto want_pre = reference_power<Pre>(pre, k), got_pre = Pre::pow(pre, k);
      assert(tie(want_pre.sum, want_pre.mn, want_pre.mx) == tie(got_pre.sum, got_pre.mn, got_pre.mx));
      assert(reference_power<PS>(ps, k).power_sum == PS::pow(ps, k).power_sum);
      auto want_is = reference_power<IS>(is, k), got_is = IS::pow(is, k);
      assert(want_is.val == got_is.val && want_is.power_sum == got_is.power_sum);
      assert(reference_power<MS>(ms, k).power_sum == MS::pow(ms, k).power_sum);
      auto want_b = reference_power<B>(b, k), got_b = B::pow(b, k);
      assert(tie(want_b.mn, want_b.mx, want_b.mn2, want_b.mx2, want_b.sum, want_b.len, want_b.mn_cnt, want_b.mx_cnt,
                 want_b.fail) == tie(got_b.mn, got_b.mx, got_b.mn2, got_b.mx2, got_b.sum, got_b.len, got_b.mn_cnt,
                                     got_b.mx_cnt, got_b.fail));
      ++checked;
    }
  }
  using Cost = MonoidClampAddCost<ll, 2, -3, 1'000'000'000>;
  using Clamp = MonoidClampAdd<ll, 1'000'000'000>;
  for (int iter = 0; iter < 10000; ++iter)
  {
    ll lo = ll(rng() % 41) - 20, hi = ll(rng() % 41) - 20;
    if (lo > hi)
      swap(lo, hi);
    if (rng() % 5 == 0)
      lo = -1'000'000'000;
    if (rng() % 5 == 0)
      hi = 1'000'000'000;
    ll d = ll(rng() % 25) - 12, base = ll(rng() % 51) - 25;
    Cost::S x(lo, hi, d, base);
    for (ll k : {0, 1, 2, 3, 8, 63, 1000})
    {
      auto want = reference_power<Cost>(x, k), got = Cost::pow(x, k);
      assert(tie(want.lo, want.hi, want.bias, want.base) == tie(got.lo, got.hi, got.bias, got.base));
      auto position = Clamp::pow({lo, hi, d}, k);
      assert(tie(position.lo, position.hi, position.bias) == tie(got.lo, got.hi, got.bias));
      ++checked;
    }
  }
  using Vote = MajorityVote<int>;
  for (int cand = 0; cand < 5; ++cand)
    for (ll cnt : {0, 1, 10})
      for (ll k : {0, 1, 2, 63})
      {
        auto want = reference_power<Vote>({cand, cnt}, k);
        auto got = Vote::pow({cand, cnt}, k);
        assert(want.cand == got.cand && want.cnt == got.cnt);
      }
  static_assert(internal::HasMonoidPow<MonoidMinCount<ll>, ll>::value);
  static_assert(internal::HasMonoidPow<ActedMonoidAddMinCount<ll>, ll>::value);
  static_assert(internal::HasMonoidPow<ActedMonoidUpdMin<ll>, ll>::value);
  static_assert(internal::HasMonoidPow<ActedMonoidClampMin<ll>, ll>::value);
  static_assert(internal::HasMonoidPow<OppositeMonoid<MonoidMin<ll>>, ll>::value);
  static_assert(internal::HasMonoidPow<NormalAndOppositeMonoid<MonoidMin<ll>>, ll>::value);
  assert(checked == 91000);
}

#include "math/modint/modint.hpp"
#include "algebra/trivial.hpp"
#include "algebra/bit.hpp"
#include "algebra/algebra_basic_ops.hpp"
#include "algebra/affine_function.hpp"
#include "algebra/acted_monoid/add_sum.hpp"
#include "algebra/acted_monoid/affine_sum.hpp"
#include "algebra/acted_monoid/upd_sum.hpp"
#include "algebra/acted_monoid/add_min_sum.hpp"
#include "algebra/acted_monoid/upd_min_sum.hpp"
#include "algebra/acted_monoid/add_min.hpp"
#include "algebra/acted_monoid/add_power_sum.hpp"
#include "algebra/acted_monoid/upd_power_sum.hpp"
#include "algebra/acted_monoid/add_multivariate_power_sum.hpp"
#include "algebra/acted_monoid/upd_multivariate_power_sum.hpp"
#include "algebra/acted_monoid/polyadd_sum.hpp"
#include "algebra/acted_monoid/polyupd_sum.hpp"

struct AddOnlyValue
{
  ll val{};
  constexpr AddOnlyValue() = default;
  constexpr explicit AddOnlyValue(ll val) : val(val) {}
  friend constexpr AddOnlyValue operator+(const AddOnlyValue &a, const AddOnlyValue &b)
  {
    return AddOnlyValue(a.val + b.val);
  }
  friend constexpr bool operator<(const AddOnlyValue &a, const AddOnlyValue &b) { return a.val < b.val; }
};

struct NarrowMemberProduct
{
  i128 val{};
  NarrowMemberProduct() = default;
  explicit NarrowMemberProduct(i128 val) : val(val) {}
  NarrowMemberProduct operator*(ll k) const { return NarrowMemberProduct(val * k); }
  friend NarrowMemberProduct operator+(NarrowMemberProduct a, NarrowMemberProduct b)
  {
    return NarrowMemberProduct(a.val + b.val);
  }
};
struct NarrowFreeProduct
{
  i128 val{};
  NarrowFreeProduct() = default;
  explicit NarrowFreeProduct(i128 val) : val(val) {}
  friend NarrowFreeProduct operator*(NarrowFreeProduct x, ll k) { return NarrowFreeProduct(x.val * k); }
  friend NarrowFreeProduct operator+(NarrowFreeProduct a, NarrowFreeProduct b)
  {
    return NarrowFreeProduct(a.val + b.val);
  }
};
struct NarrowConstructor
{
  i128 val{};
  NarrowConstructor() = default;
  NarrowConstructor(ll val) : val(val) {}
  friend NarrowConstructor operator*(NarrowConstructor a, NarrowConstructor b)
  {
    NarrowConstructor res;
    res.val = a.val * b.val;
    return res;
  }
  friend NarrowConstructor operator+(NarrowConstructor a, NarrowConstructor b)
  {
    NarrowConstructor res;
    res.val = a.val + b.val;
    return res;
  }
};

template <class T>
void check_narrow_scalar()
{
  using M = MonoidPowerSum<T, 0>;
  typename M::S x;
  x.power_sum[0] = T(1);
  const u128 k = u128(1) << 80;
  assert(M::pow(x, k).power_sum[0].val == i128(k));
  assert(M::pow(x, 123LL).power_sum[0].val == 123);
  static_assert(!internal_monoid_power::HasScalarProduct<T, u128>::value);
  static_assert(internal_monoid_power::HasScalarProduct<T, ll>::value);
}

void test_types_and_wrappers()
{
  using P = MonoidPrefixMinMax<AddOnlyValue, true>;
  constexpr P::S x(AddOnlyValue(-2), AddOnlyValue(-5), AddOnlyValue(3));
  constexpr auto p = P::pow(x, 10);
  static_assert(p.sum.val == -20 && p.mn.val == -23 && p.mx.val == 3);
  using PM = MonoidPrefixMinMax<ll, true>;
  constexpr auto r = PM::pow({2, -3, 5}, 10);
  static_assert(r.sum == 20 && r.mn == -3 && r.mx == 23);
  static_assert(OppositeMonoid<MonoidMin<int>>::pow(7, 20) == 7);
  assert(pow_group<OppositeGroup<GroupAddSub<double>>>(2.5, -3.0) == -7.5);
  auto both = pow_group<NormalAndOppositeGroup<GroupAddSub<ll>>>({3, -2}, -4);
  assert(both.normal == -12 && both.opposite == 8);
  using Aff = MonoidAffineFunction<modint998244353>;
  using Dual = NormalAndOppositeMonoid<Aff>;
  Dual::S pair(Aff::S(2, 3), Aff::S(5, 7));
  auto got = Dual::pow(pair, 31), want = reference_power<Dual>(pair, 31);
  assert(got.normal.b == want.normal.b && got.normal.c == want.normal.c);
  assert(got.opposite.b == want.opposite.b && got.opposite.c == want.opposite.c);

  // The original function-address and acted-monoid forwarding interfaces remain usable.
  using Len = MonoidLenSum<ll>;
  constexpr auto pointer = Len::pow;
  constexpr auto acted_pointer = ActedMonoidAddSum<ll>::pow;
  assert(pointer({1, 3}, 5).val == 15 && acted_pointer({1, 3}, 5).len == 5);
  using AddOnlySum = MonoidLenSum<AddOnlyValue>;
  auto sum = AddOnlySum::pow({1, AddOnlyValue(7)}, 123);
  assert(sum.len == 123 && sum.val.val == 861);
  using PS = MonoidPowerSum<AddOnlyValue, 3>;
  PS::S component;
  component.power_sum = {AddOnlyValue(2), AddOnlyValue(3), AddOnlyValue(-4), AddOnlyValue(5)};
  auto scaled = PS::pow(component, 100);
  assert(scaled.power_sum[0].val == 200 && scaled.power_sum[2].val == -400);
  using Mod = MonoidPowerSum<modint998244353, 3>;
  auto m = Mod::op(Mod::S(3), Mod::S(5));
  assert(Mod::pow(m, 1000000).power_sum == reference_power<Mod>(m, 1000000).power_sum);

  const u128 k = (u128(1) << 80) + 7;
  check_narrow_scalar<NarrowMemberProduct>();
  check_narrow_scalar<NarrowFreeProduct>();
  check_narrow_scalar<NarrowConstructor>();
  static_assert(internal_monoid_power::HasScalarProduct<modint998244353, u128>::value);
  assert(Mod::pow(m, k).power_sum == reference_power<Mod>(m, k).power_sum);
  using Wide = MonoidMinMaxSum<i128, nullptr, i128>;
  auto wide = Wide::pow(Wide::S(-3), k);
  assert(wide.len == i128(k) && wide.sum == -3 * i128(k) && wide.mn == -3 && wide.mx == -3);
  using WideLen = MonoidLenSum<i128, i128>;
  auto reversed = OppositeMonoid<WideLen>::pow(WideLen::S(-3), k);
  assert(reversed.len == i128(k) && reversed.val == -3 * i128(k));
  auto periodic = MonoidPrefixMinMax<i128, true>::pow({0, -7, 9}, k);
  assert(periodic.sum == 0 && periodic.mn == -7 && periodic.mx == 9);
  using U = MonoidPowerSum<unsigned short, 0>;
  U::S short_value;
  short_value.power_sum[0] = 60000;
  assert(U::pow(short_value, 60000).power_sum == reference_power<U>(short_value, 60000).power_sum);
  using U32 = MonoidPowerSum<unsigned, 1>;
  U32::S wrap;
  wrap.power_sum = {3, 7};
  assert(U32::pow(wrap, k).power_sum == reference_power<U32>(wrap, k).power_sum);

  using Vote = MajorityVote<int>;
  for (int candidate : {0, 3})
    for (ll count : {-2, 0, 5})
      for (ll exponent : {0, 1, 2, 7})
      {
        auto a = Vote::pow({candidate, count}, exponent), b = reference_power<Vote>({candidate, count}, exponent);
        assert(a.cand == b.cand && a.cnt == b.cnt);
      }
  using Beats = MonoidMinMaxSumBeats<ll>;
  for (ll len : {0, 1, 7})
    for (bool fail : {false, true})
      for (ll exponent : {0, 1, 2, 7})
      {
        Beats::S state(3, len);
        state.fail = fail;
        auto a = Beats::pow(state, exponent), b = reference_power<Beats>(state, exponent);
        assert(tie(a.mn, a.mx, a.mn2, a.mx2, a.sum, a.len, a.mn_cnt, a.mx_cnt, a.fail) ==
               tie(b.mn, b.mx, b.mn2, b.mx2, b.sum, b.len, b.mn_cnt, b.mx_cnt, b.fail));
      }
}

template <class AM, class Project> void check_forward(typename AM::S x, Project project)
{
  static_assert(internal::HasMonoidPow<AM, ll>::value);
  for (ll k : {0, 1, 2, 63, 1000})
    assert(project(AM::pow(x, k)) == project(reference_power<AM>(x, k)));
}

void test_forwarding()
{
  auto scalar = [](const auto &x) { return x; };
  check_forward<ActedMonoidUpdMin<ll>>(3, scalar);
  check_forward<ActedMonoidUpdMax<ll>>(3, scalar);
  check_forward<ActedMonoidClampMin<ll>>(3, scalar);
  check_forward<ActedMonoidClampMax<ll>>(3, scalar);
  check_forward<ActedMonoidAddMin<ll>>(3, scalar);
  check_forward<ActedMonoidAddMax<ll>>(3, scalar);
  auto extremes = [](const auto &x) { return pair{x.mn, x.mx}; };
  check_forward<ActedMonoidUpdMinMax<ll>>({-3, 7}, extremes);
  check_forward<ActedMonoidClampMinMax<ll>>({-3, 7}, extremes);
  check_forward<ActedMonoidAddMinMax<ll>>({-3, 7}, extremes);
  check_forward<ActedMonoidAddMinCount<ll>>({3, 2}, [](auto x) { return pair{x.mn, x.cnt}; });
  check_forward<ActedMonoidAddMaxCount<ll>>({3, 2}, [](auto x) { return pair{x.mx, x.cnt}; });
  auto sum = [](const auto &x) { return pair{x.sum, x.len}; };
  check_forward<ActedMonoidAddMinSum<ll>>(3, sum);
  check_forward<ActedMonoidAddMaxSum<ll>>(3, sum);
  check_forward<ActedMonoidAddMinMaxSum<ll>>(3, sum);
  check_forward<ActedMonoidUpdMinSum<ll>>(3, sum);
  check_forward<ActedMonoidUpdMaxSum<ll>>(3, sum);
  check_forward<ActedMonoidUpdMinMaxSum<ll>>(3, sum);
  check_forward<ActedMonoidChminChmaxAddMinMaxSum<ll>>(3, sum);
  auto powers = [](const auto &x) { return x.power_sum; };
  check_forward<ActedMonoidAffinePowerSum<ll, 3>>(3, powers);
  check_forward<ActedMonoidAddPowerSum<ll, 3>>(3, powers);
  check_forward<ActedMonoidUpdPowerSum<ll, 3>>(3, powers);
  using Mix = MonoidMultivariatePowerSum<ll, 2, 3>;
  check_forward<ActedMonoidAffineMultivariatePowerSum<ll, 2, 3>>(Mix::S({2, 3}), powers);
  check_forward<ActedMonoidAddMultivariatePowerSum<ll, 2, 3>>(Mix::S({2, 3}), powers);
  check_forward<ActedMonoidUpdMultivariatePowerSum<ll, 2, 3>>(Mix::S({2, 3}), powers);
  auto indexed = [](const auto &x) { return pair{x.val, x.power_sum}; };
  check_forward<ActedMonoidPolyAddSum<ll, 3>>({7, 5}, indexed);
  check_forward<ActedMonoidPolyUpdSum<ll, 3>>({7, 5}, indexed);
  check_forward<ActedMonoidAriUpdMin<ll>>({2, 7, -3}, [](auto x) { return tuple{x.l, x.r, x.mn}; });
  check_forward<ActedMonoidAriUpdMax<ll>>({2, 7, 9}, [](auto x) { return tuple{x.l, x.r, x.mx}; });
  check_forward<ActedMonoidAriUpdMinMax<ll>>({2, 7, -3, 9}, [](auto x) { return tuple{x.l, x.r, x.mn, x.mx}; });
  check_forward<MonoidUpd<ll, 999>>(7, scalar);
  check_forward<MonoidUpd<ll, 999>>(999, scalar);
  check_forward<MonoidUpdFlag<ll>>({false, 7}, [](auto x) { return pair{x.is_id, x.val}; });
  check_forward<MonoidUpdFlag<ll>>({true, 7}, [](auto x) { return pair{x.is_id, x.val}; });
  check_forward<GroupTrivial>({}, scalar);
}

void test_clamp_wide_and_evaluation()
{
  using M = MonoidClampAddCost<ll, 2, -3, 1'000'000'000>;
  for (ll lo : {-10, 0, 5})
    for (ll width : {0, 1, 10})
      for (ll d : {-11, -1, 0, 1, 11})
      {
        M::S f(lo, lo + width, d, 7);
        for (ll k = 0; k < 25; ++k)
          for (ll initial : {-30, -5, 0, 5, 30})
          {
            ll p = initial, cost = 0;
            for (ll j = 0; j < k; ++j)
            {
              auto next = f(p);
              p = next.first;
              cost += next.second;
            }
            assert((M::pow(f, k)(initial) == pair<ll, ll>{p, cost}));
          }
      }
  // Huge exponents with bounded positions avoid forming k*bias in a fixed-width integer.
  const u128 huge = u128(1) << 120;
  using C = MonoidClampAdd<i128>;
  const i128 step = i128(1) << 60;
  auto positive = C::pow({-5, 7, step}, huge);
  auto negative = C::pow({-5, 7, -step}, huge);
  assert(positive(0) == step + 7 && negative(0) == -step - 5);
  using Cost = MonoidClampAddCost<i128, 1, 1>;
  auto balanced = Cost::pow({0, 10, 3, -3}, huge);
  assert(balanced(5).first == 13 && balanced(5).second == -8);
  using Free = MonoidClampAddCost<i128, 0, 0>;
  assert(Free::pow({-5, 7, step, 0}, huge)(0).second == 0);
  using Float = MonoidClampAddCost<double, 2, -3>;
  auto a = Float::pow({-1.5, 4.25, .375, .125}, 23);
  auto b = reference_power<Float>({-1.5, 4.25, .375, .125}, 23);
  assert(abs(a(-3).first - b(-3).first) < 1e-10 && abs(a(-3).second - b(-3).second) < 1e-10);
}

int main()
{
  test_numeric_formulas();
  test_types_and_wrappers();
  test_forwarding();
  test_clamp_wide_and_evaluation();
  cout << "Hello World\n";
}
