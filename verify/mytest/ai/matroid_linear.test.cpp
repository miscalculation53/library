#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "matroid/linear_matroid_intersection.hpp"
#include "matroid/linear_matroid_parity.hpp"
#include "matroid/matroid_intersection.hpp"

namespace
{
template <class F>
int rank_of(const LinearMatroid<F> &m, const vc<int> &ids)
{
  Matrix<F> matrix(m.dimension(), ids.size());
  for (int j = 0; j < int(ids.size()); j++) for (int i = 0; i < m.dimension(); i++)
    matrix[i][j] = m.columns()[ids[j]][i];
  return matrix.template rank<int>();
}

template <class F>
bool independent(const LinearMatroid<F> &m, const vc<int> &ids)
{
  return rank_of(m, ids) == int(ids.size());
}

template <class F>
int brute_intersection(const LinearMatroid<F> &a, const LinearMatroid<F> &b)
{
  int best = 0;
  for (int mask = 0; mask < (1 << a.size()); mask++)
  {
    vc<int> ids;
    for (int i = 0; i < a.size(); i++) if (mask >> i & 1) ids.push_back(i);
    if (int(ids.size()) > best && independent(a, ids) && independent(b, ids))
      best = ids.size();
  }
  return best;
}

template <class F>
int brute_parity(const LinearMatroid<F> &a, const vc<pair<int, int>> &pairs)
{
  int best = 0;
  for (int mask = 0; mask < (1 << pairs.size()); mask++)
  {
    vc<int> ids;
    for (int i = 0; i < int(pairs.size()); i++) if (mask >> i & 1)
      ids.push_back(pairs[i].first), ids.push_back(pairs[i].second);
    if (int(ids.size()) > 2 * best && independent(a, ids)) best = ids.size() / 2;
  }
  return best;
}

template <class F>
void check_circuits(LinearMatroid<F> matroid)
{
  const int n = matroid.size();
  for (int mask = 0; mask < (1 << n); mask++)
  {
    vc<int> chosen;
    for (int e = 0; e < n; e++) if (mask >> e & 1) chosen.push_back(e);
    if (!independent(matroid, chosen)) continue;
    matroid.build(chosen);
    for (int e = 0; e < n; e++) if (!(mask >> e & 1))
    {
      auto circuit = matroid.circuit(e);
      auto added = chosen;
      added.push_back(e);
      assert(bool(circuit) == !independent(matroid, added));
      if (!circuit) continue;
      sort(circuit->begin(), circuit->end());
      assert(unique(circuit->begin(), circuit->end()) == circuit->end());
      for (int out : chosen)
      {
        auto replaced = chosen;
        *find(replaced.begin(), replaced.end(), out) = e;
        assert(independent(matroid, replaced) == binary_search(circuit->begin(), circuit->end(), out));
      }
    }
  }
  auto copied = matroid;
  copied.build({});
  matroid.build({});
  for (int e = 0; e < n; e++) assert(copied.circuit(e) == matroid.circuit(e));
}

template <uint P>
void random_cases(int cases)
{
  using S = static_modint32<P>;
  using F = FieldAddSubMulDiv<S>;
  mt19937_64 rng(20261101000ULL + P);
  auto sample = [&]() { return S(rng() % P); };
  for (int trial = 0; trial < cases; trial++)
  {
    const int n = rng() % 9, da = rng() % 7, db = rng() % 7;
    vvc<S> a(n, vc<S>(da)), b(n, vc<S>(db));
    for (auto &v : a) for (auto &x : v) x = sample();
    for (auto &v : b) for (auto &x : v) x = sample();
    const LinearMatroid<F> first(da, a), second(db, b);
    if (trial < 15) check_circuits(first);
    auto result = linear_matroid_intersection_restore(first, second, sample, P < 100 ? 32 : 2);
    assert(independent(first, result) && independent(second, result));
    int expected_common = brute_intersection(first, second);
    int common_size = linear_matroid_intersection_size(first, second, sample, P < 100 ? 32 : 2);
    assert(0 <= common_size && common_size <= expected_common);
    if constexpr (P > 100) assert(common_size == expected_common);
    assert(int(matroid_intersection(first, second).size()) == expected_common);
    assert(int(result.size()) <= expected_common);
    if constexpr (P > 100) assert(int(result.size()) == expected_common);
    if constexpr (P < 100) if (trial < 5)
    {
      int evaluations = 1, maximum = 0;
      for (int e = 0; e < n; e++) evaluations *= P;
      for (int assignment = 0; assignment < evaluations; assignment++)
      {
        int code = assignment;
        auto exhaustive_sample = [&]() { S x(code % P); code /= P; return x; };
        maximum = max(maximum, internal::linear_matroid_detail::intersection_rank<F>(a, b, 0, exhaustive_sample));
      }
      assert(maximum == expected_common);
    }

    const int m = rng() % 7, d = rng() % 9;
    vvc<S> columns(2 * m + 1, vc<S>(d));
    for (auto &v : columns) for (auto &x : v) x = sample();
    vc<int> order(columns.size());
    iota(order.begin(), order.end(), 0);
    shuffle(order.begin(), order.end(), rng);
    vc<pair<int, int>> pairs;
    for (int i = 0; i < m; i++) pairs.emplace_back(order[2 * i], order[2 * i + 1]);
    const LinearMatroid<F> parity(d, columns);
    auto selected = linear_matroid_parity_restore(parity, pairs, sample, P < 100 ? 32 : 2);
    vc<int> ids;
    for (int e : selected) ids.push_back(pairs[e].first), ids.push_back(pairs[e].second);
    assert(independent(parity, ids));
    int expected_parity = brute_parity(parity, pairs);
    int parity_size = linear_matroid_parity_size(parity, pairs, sample, P < 100 ? 32 : 2);
    assert(0 <= parity_size && parity_size <= expected_parity);
    if constexpr (P > 100) assert(parity_size == expected_parity);
    assert(int(selected.size()) <= expected_parity);
    if constexpr (P > 100) assert(int(selected.size()) == expected_parity);
    if constexpr (P < 100) if (trial < 5)
    {
      int evaluations = 1, maximum = 0;
      for (int e = 0; e < m; e++) evaluations *= P;
      for (int assignment = 0; assignment < evaluations; assignment++)
      {
        int code = assignment;
        auto exhaustive_sample = [&]() { S x(code % P); code /= P; return x; };
        maximum = max(maximum, internal::linear_matroid_detail::parity_rank<F>(columns, pairs, 0, exhaustive_sample));
      }
      assert(maximum == expected_parity);
    }
  }
}

void degenerate_cases()
{
  using S = static_modint32<2>;
  using F = FieldAddSubMulDiv<S>;
  auto one = [] { return S(1); };
  auto zero = [] { return S(0); };
  LinearMatroid<F> three(1, {{1}, {1}, {1}});
  assert(linear_matroid_intersection_restore(three, three, one).size() == 1);
  assert(linear_matroid_intersection_size(three, three, one) == 1);
  assert(linear_matroid_intersection_size(three, three, zero) == 0);
  assert(linear_matroid_intersection_restore(three, three, zero).empty());
  LinearMatroid<F> pairs(2, {{1, 0}, {0, 1}, {1, 0}, {0, 1}, {1, 0}, {0, 1}});
  assert(linear_matroid_parity_restore(pairs, one).size() == 1);
  assert(linear_matroid_parity_size(pairs, one) == 1);
  assert(linear_matroid_parity_size(pairs, zero) == 0);
  assert(linear_matroid_parity_restore(pairs, zero).empty());
  LinearMatroid<F> loops(Matrix<F>(0, 3));
  assert(loops.size() == 3 && loops.dimension() == 0);
  loops.build({});
  assert(loops.circuit(2) && loops.circuit(2)->empty());
  assert(linear_matroid_intersection_restore(loops, loops, one).empty());
  LinearMatroid<F> empty(Matrix<F>(7, 0));
  assert(empty.dimension() == 7 && empty.size() == 0);
  assert(linear_matroid_intersection_restore(empty, empty, one).empty());
  assert(linear_matroid_intersection_size(empty, empty, one) == 0);
  assert(linear_matroid_parity_size(empty, one) == 0);
  assert(linear_matroid_parity_restore(empty, one).empty());
}

void matching_cases()
{
  using S = modint998244353;
  using F = FieldAddSubMulDiv<S>;
  mt19937_64 rng(20261101111ULL);
  auto sample = [&] { return S(rng() % S::mod()); };
  for (int trial = 0; trial < 100; trial++)
  {
    int n = 1 + rng() % 11;
    vc<pair<int, int>> edges;
    for (int i = 0; i < 10; i++)
    {
      int u = rng() % n, v = rng() % n;
      if (u != v) edges.emplace_back(u, v);
    }
    vvc<S> columns;
    for (auto [u, v] : edges)
    {
      columns.emplace_back(n, S(0));
      columns.back()[u] = 1;
      columns.emplace_back(n, S(0));
      columns.back()[v] = 1;
    }
    LinearMatroid<F> matroid(n, columns);
    auto result = linear_matroid_parity_restore(matroid, sample, 2);
    int best = 0;
    for (int mask = 0; mask < (1 << edges.size()); mask++)
    {
      int used = 0, count = 0;
      bool ok = true;
      for (int e = 0; e < int(edges.size()); e++) if (mask >> e & 1)
      {
        auto [u, v] = edges[e];
        if ((used >> u & 1) || (used >> v & 1)) ok = false;
        used |= (1 << u) | (1 << v);
        count++;
      }
      if (ok) best = max(best, count);
    }
    assert(int(result.size()) == best);
  }
}

void large_representation()
{
  using S = modint998244353;
  using F = FieldAddSubMulDiv<S>;
  constexpr int n = 40, d = 48;
  Matrix<F> matrix(d, n);
  for (int e = 0; e < n; e++) matrix[e][e] = 1;
  for (int e = 0; e < n; e++) matrix[45][e] = e + 1;
  LinearMatroid<F> matroid(matrix);
  mt19937_64 rng(20261101);
  auto sample = [&] { return S(rng() % S::mod()); };
  assert(linear_matroid_intersection_size(matroid, matroid, sample, 2) == n);
  assert(linear_matroid_parity_size(matroid, sample, 2) == n / 2);
  auto common = linear_matroid_intersection_restore(matroid, matroid, sample, 2);
  assert(common.size() == n);
  auto parity = linear_matroid_parity_restore(matroid, sample, 2);
  assert(parity.size() == n / 2);
  matroid.build(common);
}
} // namespace

int main()
{
  degenerate_cases();
  random_cases<2>(80);
  random_cases<3>(80);
  random_cases<998244353>(200);
  matching_cases();
  large_representation();
  PRINT("Hello World");
}
