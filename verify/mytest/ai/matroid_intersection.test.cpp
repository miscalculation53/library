#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "matroid/matroid_intersection.hpp"
#include "matroid/weighted_matroid_intersection.hpp"
#include "matroid/independence_matroid.hpp"
#include "matroid/matroid_greedy.hpp"
#include "matroid/matroid_union.hpp"

using Predicate = function<bool(const vc<int> &)>;
using Oracle = IndependenceMatroid<Predicate>;

int mask_of(const vc<int> &elements)
{
  int mask = 0;
  for (int e : elements)
  {
    assert(!(mask >> e & 1));
    mask |= 1 << e;
  }
  return mask;
}

vc<int> elements_of(int mask)
{
  vc<int> elements;
  for (int e = 0; mask; e++, mask >>= 1) if (mask & 1) elements.eb(e);
  return elements;
}

Oracle oracle(const vc<bool> &independent)
{
  const int n = __builtin_ctz(unsigned(independent.size()));
  return Oracle(n, [independent](const vc<int> &set) { return independent[mask_of(set)]; });
}

vc<bool> random_matroid(int n, mt19937_64 &rng)
{
  vc<bool> independent(1 << n);
  if (rng() % 2)
  {
    vc<int> group(n), cap(3);
    for (auto &e : group) e = rng() % 3;
    for (auto &c : cap) c = rng() % 4;
    for (int mask = 0; mask < (1 << n); mask++)
    {
      vc<int> count(3);
      for (int e = 0; e < n; e++) if (mask >> e & 1) count[group[e]]++;
      independent[mask] = true;
      for (int c = 0; c < 3; c++) if (count[c] > cap[c]) independent[mask] = false;
    }
  }
  else
  {
    vc<unsigned> vectors(n);
    for (auto &v : vectors) v = rng() % 32;
    for (int mask = 0; mask < (1 << n); mask++)
    {
      unsigned basis[5] = {};
      independent[mask] = true;
      for (int e = 0; e < n; e++) if (mask >> e & 1)
      {
        unsigned x = vectors[e];
        for (int bit = 4; bit >= 0; bit--) if (x >> bit & 1)
        {
          if (basis[bit]) x ^= basis[bit];
          else { basis[bit] = x; break; }
        }
        if (!x) { independent[mask] = false; break; }
      }
    }
  }
  return independent;
}

void check_oracle(const vc<bool> &independent)
{
  auto m = oracle(independent);
  for (int mask = 0; mask < int(independent.size()); mask++) if (independent[mask])
  {
    m.build(elements_of(mask));
    const auto &cm = m;
    for (int e = 0; e < m.size(); e++) if (!(mask >> e & 1))
    {
      auto c = cm.circuit(e);
      if (independent[mask | (1 << e)]) assert(!c);
      else
      {
        assert(c);
        int expected = 0;
        for (int f = 0; f < m.size(); f++) if ((mask >> f & 1) && independent[(mask ^ (1 << f)) | (1 << e)])
          expected |= 1 << f;
        assert(mask_of(*c) == expected);
      }
    }
  }
}

template <class W>
void check_intersection(const vc<bool> &a, const vc<bool> &b, const vc<W> &weight)
{
  int rank = 0;
  vc<optional<W>> optimum(weight.size() + 1);
  for (int mask = 0; mask < int(a.size()); mask++) if (a[mask] && b[mask])
  {
    const int k = __builtin_popcount(unsigned(mask));
    W total{};
    for (int e : elements_of(mask)) total += weight[e];
    rank = max(rank, k);
    if (!optimum[k] || total > *optimum[k]) optimum[k] = total;
  }
  const auto first = oracle(a), second = oracle(b);
  auto result = matroid_intersection(first, second);
  assert(int(result.size()) == rank && a[mask_of(result)] && b[mask_of(result)]);
  auto same = matroid_intersection(first, first);
  int first_rank = 0;
  for (int mask = 0; mask < int(a.size()); mask++) if (a[mask])
    first_rank = max(first_rank, __builtin_popcount(unsigned(mask)));
  assert(int(same.size()) == first_rank);
  // すべての共通独立集合を初期解にして増加路の向き・復元を確認する。
  if (weight.size() <= 6)
    for (int mask = 0; mask < int(a.size()); mask++) if (a[mask] && b[mask])
    {
      auto warm = matroid_intersection(first, second, elements_of(mask));
      assert(int(warm.size()) == rank && a[mask_of(warm)] && b[mask_of(warm)]);
    }
  auto slope = weighted_matroid_intersection_slope(first, second, weight);
  assert(int(slope.size()) == rank + 1);
  for (int k = 0; k <= rank; k++)
  {
    auto [total, set] = slope[k];
    assert(int(set.size()) == k && a[mask_of(set)] && b[mask_of(set)]);
    W actual{};
    for (int e : set) actual += weight[e];
    assert(actual == total && total == *optimum[k]);
    auto exact = weighted_matroid_intersection(first, second, weight, k);
    assert(exact && exact->first == total && exact->second == set);
  }
  auto maximal = weighted_matroid_intersection(first, second, weight);
  assert(maximal && *maximal == slope.back());
  assert(!weighted_matroid_intersection(first, second, weight, rank + 1));
}

void check_greedy(const vc<bool> &a, const vc<ll> &weight)
{
  int rank = 0;
  ll independent_opt = 0, basis_opt = numeric_limits<ll>::lowest();
  for (int mask = 0; mask < int(a.size()); mask++) if (a[mask])
  {
    int k = __builtin_popcount(unsigned(mask));
    ll total = 0;
    for (int e : elements_of(mask)) total += weight[e];
    independent_opt = max(independent_opt, total);
    if (k > rank) rank = k, basis_opt = total;
    else if (k == rank) basis_opt = max(basis_opt, total);
  }
  auto m = oracle(a);
  auto basis = matroid_greedy(m, weight);
  auto set = matroid_greedy(m, weight, true);
  ll sum_basis = 0, sum_set = 0;
  for (int e : basis) sum_basis += weight[e];
  for (int e : set) sum_set += weight[e];
  assert(a[mask_of(basis)] && int(basis.size()) == rank && sum_basis == basis_opt);
  assert(a[mask_of(set)] && sum_set == independent_opt);
}

void check_union(const vvc<bool> &independent)
{
  vc<Oracle> matroids;
  for (const auto &a : independent) matroids.eb(oracle(a));
  auto result = matroid_union(matroids);
  assert(result.size() == independent.size());
  if (independent.empty()) return;
  vc<bool> possible(independent[0].size());
  possible[0] = true;
  for (const auto &a : independent)
  {
    vc<bool> next(possible.size());
    for (int before = 0; before < int(possible.size()); before++) if (possible[before])
      for (int added = 0; added < int(a.size()); added++) if (a[added] && !(before & added))
        next[before | added] = true;
    possible = move(next);
  }
  int expected = 0, selected = 0;
  for (int mask = 0; mask < int(possible.size()); mask++) if (possible[mask])
    expected = max(expected, __builtin_popcount(unsigned(mask)));
  for (int i = 0; i < int(result.size()); i++)
  {
    int mask = mask_of(result[i]);
    assert(independent[i][mask] && !(selected & mask));
    selected |= mask;
  }
  assert(__builtin_popcount(unsigned(selected)) == expected);
}

int main()
{
  mt19937_64 rng(20261101);
  check_union({});
  check_intersection(vc<bool>{true}, vc<bool>{true}, vc<ll>{});
  check_intersection(vc<bool>{true, false}, vc<bool>{true, false}, vc<i128>{i128(1) << 90});
  for (int trial = 0; trial < 450; trial++)
  {
    const int n = rng() % 10;
    const auto a = random_matroid(n, rng), b = random_matroid(n, rng);
    vc<ll> weights(n);
    for (auto &w : weights) w = ll(rng() % 11) - 5;
    check_intersection(a, b, weights);
    check_greedy(a, weights);
    if (trial < 70) check_oracle(a);
    if (n <= 7) check_union({a, b, random_matroid(n, rng)});
    if (trial % 50 == 0)
    {
      vc<i128> wide;
      for (ll w : weights) wide.eb(i128(w) * (i128(1) << 90));
      check_intersection(a, b, wide);
    }
  }
  // 重みの同値が多い場合にも、辺数の短い増加路で更新する。
  for (int trial = 0; trial < 50; trial++)
  {
    auto a = random_matroid(8, rng), b = random_matroid(8, rng);
    check_intersection(a, b, vc<ll>(8, 0));
    check_intersection(a, b, vc<ll>(8, -1));
  }
  cout << "Hello World\n";
}
