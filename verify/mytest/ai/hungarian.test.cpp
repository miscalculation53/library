#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "flow/hungarian.hpp"
#include "flow/min_cost_b_flow.hpp"

template <class Cost, class Matrix>
void check_dual(const Matrix &a, const Hungarian<Cost> &r)
{
  const int n = a.size(), m = n ? a[0].size() : int(r.col_to_row.size());
  assert(int(r.row_to_col.size()) == n && int(r.col_to_row.size()) == m);
  i128 total = 0, dual = 0;
  vc<bool> used(m);
  for (int i = 0; i < n; i++)
  {
    const int j = r.row_to_col[i];
    assert(0 <= j && j < m && !used[j] && r.col_to_row[j] == i);
    used[j] = true, total += i128(a[i][j]);
    for (int k = 0; k < m; k++)
      assert(i128(r.row_potential[i]) + i128(r.col_potential[k]) <= i128(a[i][k]));
    assert(i128(r.row_potential[i]) + i128(r.col_potential[j]) == i128(a[i][j]));
    dual += i128(r.row_potential[i]);
  }
  for (int j = 0; j < m; j++)
  {
    if (!used[j]) assert(r.col_to_row[j] == -1 && r.col_potential[j] == 0);
    if (n < m) assert(r.col_potential[j] <= 0);
    dual += i128(r.col_potential[j]);
  }
  assert(total == i128(r.cost) && dual == total);
}

ll brute(const vvc<int> &a, int i = 0, unsigned used = 0)
{
  if (i == int(a.size())) return 0;
  ll best = LLONG_MAX;
  for (int j = 0; j < int(a[0].size()); j++) if (!(used >> j & 1))
    best = min(best, a[i][j] + brute(a, i + 1, used | 1U << j));
  return best;
}

int main()
{
  mt19937_64 rng(20261015);
  for (int trial = 0; trial < 3500; trial++)
  {
    const int n = rng() % 7, m = n + rng() % (9 - n);
    vvc<int> a(n, vc<int>(m));
    const int range = trial % 3 == 0 ? 2 : 100;
    for (auto &row : a) for (auto &x : row) x = int(rng() % (2 * range + 1)) - range;
    Hungarian<ll> r(a);
    assert(r.cost == brute(a));
    check_dual(a, r);
    vc<int> flat(size_t(n) * (m + 3));
    for (int i = 0; i < n; i++) copy(a[i].begin(), a[i].end(), flat.begin() + i * (m + 3));
    Hungarian<ll> raw(n, m, flat.data(), m + 3);
    assert(raw.cost == r.cost);
    check_dual(a, raw);
    const Hungarian deduced(a);
    static_assert(is_same_v<decltype(deduced.cost), int>);
    assert(deduced.cost == r.cost);
    check_dual(a, deduced);
  }
  // Monge・逆 Monge。行・列の定数を加えても条件が保たれる。
  for (int trial = 0; trial < 300; trial++)
  {
    const int n = 1 + rng() % 70;
    vc<ll> x(n), y(n), row(n), col(n);
    for (int i = 0; i < n; i++)
      x[i] = rng() % 100, y[i] = rng() % 100,
      row[i] = ll(rng() % 10000) - 5000, col[i] = ll(rng() % 10000) - 5000;
    sort(x.begin(), x.end()), sort(y.begin(), y.end());
    vvc<ll> a(n, vc<ll>(n));
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++)
      a[i][j] = (x[i] - y[j]) * (x[i] - y[j]) + row[i] + col[j];
    for (int reversed = 0; reversed < 2; reversed++)
    {
      const Hungarian<ll> r(a);
      check_dual(a, r);
      ll expected = 0;
      for (int i = 0; i < n; i++) expected += a[i][reversed ? n - 1 - i : i];
      assert(r.cost == expected);
      for (auto &v : a) reverse(v.begin(), v.end());
    }
  }
  // 中規模は独立した最小費用 b-flow と照合する。
  for (int trial = 0; trial < 60; trial++)
  {
    int n = 1 + rng() % 45, m = n + rng() % 20;
    vvc<ll> a(n, vc<ll>(m));
    MinCostBFlow<ll, ll> g(n + m + 1);
    for (int i = 0; i < n; i++) g.add_supply(i, 1);
    for (int j = 0; j < m; j++) g.add_edge(n + j, n + m, 1, 0);
    g.add_supply(n + m, -n);
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++)
    {
      a[i][j] = trial % 4 == 0 ? ll(i) * j : ll(rng() % 2000000001) - 1000000000;
      g.add_edge(i, n + j, 1, a[i][j]);
    }
    const Hungarian r(a);
    const auto cost = g.flow();
    assert(cost && *cost == r.cost);
    check_dual(a, r);
  }
  {
    // 大きい係数も明示した型のまま扱う。
    const i128 big = i128(1) << 90;
    vvc<i128> a{{big + 1, big + 2, big + 3}, {-big + 1, -big - 2, -big + 3}};
    const Hungarian r(a);
    assert(r.cost == -1);
    check_dual(a, r);
  }
  {
    // Monge 判定の和と総費用の途中だけが、選択した型を越える例。
    const ll big = LLONG_MAX - 10;
    vvc<ll> a{{big, big}, {big, big}, {-big, -big}, {-big, -big}};
    // 同じ行を 4 列に広げ、行定数の相殺を確認する。
    for (auto &row : a) row.resize(4, row[0]);
    const Hungarian r(a);
    assert(r.cost == 0);
    check_dual(a, r);
    vvc<ll> extreme{{LLONG_MIN, LLONG_MIN}, {LLONG_MAX, LLONG_MAX}};
    const Hungarian e(extreme);
    assert(e.cost == -1);
    check_dual(extreme, e);
    // 判定する 2x2 の和が overflow しても、不等号の向きを保つ。
    const ll offset = LLONG_MAX - 100;
    vvc<ll> monge(4, vc<ll>(4));
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++)
      monge[i][j] = (i < 2 ? offset : -offset) - i * j;
    const Hungarian full_width(monge);
    assert(full_width.cost == -14);
    check_dual(monge, full_width);
  }
  {
    const array<array<int, 3>, 2> a{{{{3, -1, 4}}, {{0, 8, 2}}}};
    const Hungarian r(a);
    assert(r.cost == -1);
    check_dual(a, r);
  }
  // 列定数は最適割当を変えない。一般行列の探索で full-width の相殺を確認する。
  for (int trial = 0; trial < 180; trial++)
  {
    const int n = 4 + rng() % 3;
    vvc<int> small(n, vc<int>(n));
    vvc<ll> a(n, vc<ll>(n));
    const ll offset = LLONG_MAX - 10000;
    i128 column_sum = 0;
    for (int j = 0; j < n; j++)
    {
      const ll shift = j < n / 2 ? offset : j < 2 * (n / 2) ? -offset : 0;
      column_sum += shift;
      for (int i = 0; i < n; i++)
      {
        small[i][j] = int(rng() % 201) - 100;
        a[i][j] = shift + small[i][j];
      }
    }
    const Hungarian<ll> r(a);
    assert(i128(r.cost) == i128(brute(small)) + column_sum);
    check_dual(a, r);
  }
  for (int trial = 0; trial < 200; trial++)
  {
    const int n = 1 + rng() % 5, m = n + rng() % 3;
    vvc<int> a(n, vc<int>(m));
    for (auto &row : a) for (auto &value : row) value = int(rng() % 11) - 5;
    const Hungarian<signed char> r(a);
    assert(r.cost == brute(a));
    check_dual(a, r);
  }
  {
    vvc<ll> a{{LLONG_MIN, LLONG_MIN, LLONG_MIN}, {LLONG_MAX, LLONG_MAX, LLONG_MAX}};
    const Hungarian<ll> r(a);
    assert(r.cost == -1);
    check_dual(a, r);
  }
  cout << "Hello World\n";
}
