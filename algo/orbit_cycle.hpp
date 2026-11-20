#pragma once

#include "../algebra/internal_power.hpp"
#include "../utils/identity.hpp"

/**
 * @brief 反復列の周期検出・項の取得・先頭区間の集約
 * @docs docs/algo/orbit_cycle.md
 */

namespace internal_orbit_cycle
{
  template <class I>
  constexpr auto nonnegative_index(I k)
  {
    static_assert(is_integral_ext<I>);
    if constexpr (is_signed_ext<I>) assert(k >= 0);
    return common_type_t<I, ull>(k);
  }

  template <class T, class Next, class Equal, class Visit, class I = nullptr_t>
  constexpr optional<ull> find_period(T &state, Next &next, Equal &equal, Visit &&visit,
                                       I *remaining = nullptr)
  {
    constexpr bool bounded = !is_same_v<I, nullptr_t>;
    if constexpr (bounded)
      if (*remaining == 0) return nullopt;

    T checkpoint = state;
    ull block = 1, distance = 0;
    while (true)
    {
      visit(std::as_const(state));
      state = next(std::as_const(state));
      if constexpr (bounded)
        if (--*remaining == 0) return nullopt;
      ++distance;
      if (equal(std::as_const(checkpoint), std::as_const(state))) return distance;
      if (distance == block)
      {
        checkpoint = state;
        block *= 2;
        distance = 0;
      }
    }
  }
}

struct OrbitCycle
{
  ull preperiod, period;
  CPP_DUMP_DEFINE_DATA(preperiod, period);

  // 同じ状態を表す最小の非負添字を返す
  template <class I>
  constexpr I reduce_index(I k) const
  {
    const auto index = internal_orbit_cycle::nonnegative_index(k);
    assert(period > 0);
    if (index < preperiod) return k;
    return I(preperiod + (index - preperiod) % period);
  }
};

// 周期に入る反復列の前周期長と最小周期を返す
// next は T から T への関数
template <class T, class Next, class Equal = equal_to<>>
constexpr OrbitCycle orbit_cycle(const T &initial, Next &&next, Equal &&equal = {})
{
  T fast = initial;
  const ull period = *internal_orbit_cycle::find_period(fast, next, equal, [](const T &) {});
  fast = initial;
  for (ull i = 0; i < period; ++i) fast = next(std::as_const(fast));
  T slow = initial;
  ull preperiod = 0;
  while (!equal(std::as_const(slow), std::as_const(fast)))
  {
    slow = next(std::as_const(slow));
    fast = next(std::as_const(fast));
    ++preperiod;
  }
  return {preperiod, period};
}

// 初期値を第 0 項とする反復列の第 k 項を返す
// next は T から T への関数
template <class T, class I, class Next, class Equal = equal_to<>>
constexpr T orbit_at(T initial, I k, Next &&next, Equal &&equal = {})
{
  auto remaining = internal_orbit_cycle::nonnegative_index(k);
  const auto period = internal_orbit_cycle::find_period(
      initial, next, equal, [](const T &) {}, &remaining);
  if (period) remaining %= *period;
  for (; remaining != 0; --remaining) initial = next(std::as_const(initial));
  return initial;
}

// 反復列の先頭 k 項を project で写し、列の順にモノイド M で結合する
template <class M, class T, class I, class Next, class Project = Identity, class Equal = equal_to<>>
constexpr typename M::S orbit_prefix_prod(T initial, I k, Next &&next,
                                          Project &&project = {}, Equal &&equal = {})
{
  auto remaining = internal_orbit_cycle::nonnegative_index(k);
  typename M::S result = M::e();
  if (remaining == 0) return result;
  --remaining;

  auto append = [&](const T &state) { result = M::op(std::move(result), project(state)); };
  const auto period = internal_orbit_cycle::find_period(initial, next, equal, append, &remaining);
  if (period && remaining + 1 > *period)
  {
    const auto count = remaining + 1;
    const ull rest = count % *period;
    auto collect = [&](ull length, typename M::S product)
    {
      for (ull i = 0; i < length; ++i)
      {
        product = M::op(std::move(product), project(std::as_const(initial)));
        initial = next(std::as_const(initial));
      }
      return product;
    };
    auto prefix = collect(rest, M::e());
    const auto cycle = collect(*period - rest, prefix);
    auto repeated = internal_monoid_power::power<M>(cycle, I(count / *period));
    return M::op(M::op(std::move(result), std::move(repeated)), std::move(prefix));
  }

  for (; remaining != 0; --remaining)
  {
    append(std::as_const(initial));
    initial = next(std::as_const(initial));
  }
  append(std::as_const(initial));
  return result;
}

template <class T>
class Orbit
{
  OrbitCycle cycle_;
  vc<T> states_;

public:
  using const_reference = typename vc<T>::const_reference;
  CPP_DUMP_DEFINE_DATA(cycle_, states_);

  // 周期に入る反復列の前周期と周期一周の状態を保持する
  template <class Next, class Equal = equal_to<>>
  Orbit(const T &initial, Next &&next, Equal &&equal = {})
      : cycle_(orbit_cycle(initial, next, equal))
  {
    const ull length = cycle_.preperiod + cycle_.period;
    states_.reserve(length);
    T state = initial;
    for (ull i = 0; i < length; ++i)
    {
      if (i != 0) state = next(std::as_const(state));
      states_.push_back(state);
    }
  }

  OrbitCycle cycle() const { return cycle_; }
  const vc<T> &states() const { return states_; }

  // 第 k 項と等しい状態を返す
  template <class I>
  const_reference operator[](I k) const
  {
    return states_[cycle_.reduce_index(k)];
  }
};

template <class M>
class OrbitPrefixProd
{
public:
  using S = typename M::S;

private:
  OrbitCycle cycle_;
  vc<S> preperiod_prod_, period_prod_;

  template <class Extend>
  void build(Extend &&extend)
  {
    auto append_prefixes = [&](vc<S> &products, ull first, ull last)
    {
      products.reserve(last - first + 1);
      products.push_back(M::e());
      for (ull i = first; i < last; ++i)
        products.push_back(extend(products.back(), i));
    };
    append_prefixes(preperiod_prod_, 0, cycle_.preperiod);
    append_prefixes(period_prod_, cycle_.preperiod, cycle_.preperiod + cycle_.period);
  }

public:
  CPP_DUMP_DEFINE_DATA(cycle_, preperiod_prod_, period_prod_);

  // 周期に入る反復列を射影し、先頭区間の集約を前計算する
  template <class T, class Next, class Project = Identity, class Equal = equal_to<>>
  OrbitPrefixProd(const T &initial, Next &&next, Project &&project = {}, Equal &&equal = {})
      : cycle_(orbit_cycle(initial, next, equal))
  {
    T state = initial;
    build([&](const S &prefix, ull i)
    {
      if (i != 0) state = next(std::as_const(state));
      return M::op(prefix, project(std::as_const(state)));
    });
  }

  // 保存済みの反復列から先頭区間の集約を前計算する
  template <class T, class Project = Identity>
  explicit OrbitPrefixProd(const Orbit<T> &orbit, Project &&project = {})
      : cycle_(orbit.cycle())
  {
    const auto &states = orbit.states();
    build([&](const S &prefix, ull i)
    {
      const T &state = states[i];
      return M::op(prefix, project(state));
    });
  }

  OrbitCycle cycle() const { return cycle_; }

  // 先頭 k 項の射影を列の順にモノイド M で結合した値を返す
  template <class I>
  S prod(I k) const
  {
    const auto count = internal_orbit_cycle::nonnegative_index(k);
    if (count <= cycle_.preperiod) return preperiod_prod_[count];

    const auto remaining = count - cycle_.preperiod;
    const S &prefix = preperiod_prod_.back();
    if (remaining <= cycle_.period) return M::op(prefix, period_prod_[remaining]);

    const auto repetitions = I(remaining / cycle_.period);
    const ull rest = remaining % cycle_.period;
    auto repeated = internal_monoid_power::power<M>(period_prod_.back(), repetitions);
    return M::op(M::op(prefix, std::move(repeated)), period_prod_[rest]);
  }
};
