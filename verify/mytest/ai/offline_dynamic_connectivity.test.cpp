#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/offline_dynamic_connectivity.hpp"

// 各時刻の多重集合を素朴な逐次処理と比較し、undo の対応も確認する。
void check_states(const OfflineDynamicConnectivity<int> &dc, const vvc<int> &expected)
{
  repi(_, 2)
  {
    vc<int> counts(expected.empty() ? 1 : expected[0].size());
    vc<int> history;
    int next_time = 0;
    dc.run(
        [&](int obj)
        {
          counts[obj]++;
          history.eb(obj);
        },
        [&]
        {
          assert(!history.empty());
          counts[history.back()]--;
          history.pop_back();
        },
        [&](int t)
        {
          assert(t == next_time++);
          assert(counts == expected[t]);
        });
    assert(next_time == SZ<int>(expected));
    assert(history.empty());
    assert(counts == vc<int>(counts.size()));
  }

  vvc<int> from_spans(expected.size(), vc<int>(expected.empty() ? 1 : expected[0].size()));
  fec([l, r, obj] : dc.spans())
  {
    assert(0 <= l && l < r && r <= SZ<int>(expected));
    repi(t, l, r) from_spans[t][obj]++;
  }
  assert(from_spans == expected);
}

void test_boundaries()
{
  OfflineDynamicConnectivity<int> empty(0);
  empty.add(0, 0);
  empty.erase_if(0, 0);
  empty.add(0, 0);
  empty.add_span(0, 0, 0);
  check_states(empty, {});

  check_states(OfflineDynamicConnectivity<int>(1), {{0}});
  check_states(OfflineDynamicConnectivity<int>(7), vvc<int>(7, vc<int>(1)));

  OfflineDynamicConnectivity<int> dc(5);
  dc.add(0, 0);
  dc.add(0, 0);
  dc.add(0, 1);
  dc.erase_if(0, 1);
  dc.erase_if(1, 0);
  dc.add(2, 0);
  dc.erase_if(2, 0);
  dc.add(2, 2);
  dc.erase_if(3, 0);
  // 明示した区間は登録順と独立で、同じ対象の区間を重ねられる。
  dc.add_span(3, 5, 1);
  dc.add_span(0, 3, 1);
  dc.add_span(1, 4, 1);
  dc.add_span(2, 2, 1);
  vvc<int> expected{{2, 1, 0}, {1, 2, 0}, {1, 2, 1}, {0, 2, 1}, {0, 1, 1}};
  check_states(dc, expected);

  // spans/run の呼び出し後も登録できる。右端の操作は以前の時刻に影響しない。
  dc.erase_if(5, 2);
  dc.add(5, 0);
  dc.add_span(0, 1, 2);
  expected[0][2]++;
  check_states(dc, expected);
}

void test_random_states()
{
  mt19937 rng(928731);
  repi(_, 300)
  {
    const int q = 1 + rng() % 40, objects = 1 + rng() % 8;
    OfflineDynamicConnectivity<int> dc(q);
    vc<int> counts(objects);
    repi(i, 12)
    {
      int obj = rng() % objects;
      dc.add(0, obj);
      counts[obj]++;
    }
    vvc<int> expected(q);
    repi(t, q)
    {
      int operations = rng() % 6;
      repi(i, operations)
      {
        int obj = rng() % objects;
        int type = rng() % 3;
        if (type == 0)
        {
          dc.add(t, obj);
          counts[obj]++;
        }
        else if (type == 1 && counts[obj])
        {
          dc.erase_if(t, obj);
          counts[obj]--;
        }
        else
        {
          dc.erase_if(t, obj);
          if (counts[obj]) counts[obj]--;
        }
      }
      expected[t] = counts;
    }
    repi(i, 15)
    {
      int l = rng() % (q + 1), r = rng() % (q + 1), obj = rng() % objects;
      if (l > r) swap(l, r);
      dc.add_span(l, r, obj);
      repi(t, l, r) expected[t][obj]++;
    }
    check_states(dc, expected);
  }
}

void test_erase_if()
{
  OfflineDynamicConnectivity<int> empty(0);
  empty.erase_if(0, 0);
  check_states(empty, {});

  OfflineDynamicConnectivity<int> dc(5);
  dc.erase_if(0, 0);
  dc.add(0, 0);
  dc.add(0, 0);
  dc.erase_if(1, 1);
  dc.erase_if(1, 0);
  dc.erase_if(2, 0);
  dc.erase_if(2, 0);
  dc.add(2, 0);
  dc.erase_if(3, 0);
  dc.erase_if(3, 0);
  // 明示した区間は erase_if で管理する対象と独立。
  dc.add_span(0, 5, 1);
  dc.erase_if(4, 1);
  check_states(dc, {{2, 1}, {1, 1}, {1, 1}, {0, 1}, {0, 1}});
  dc.erase_if(5, 0);
}

vvc<bool> reachability(int n, const map<pair<int, int>, int> &edges)
{
  vvc<bool> res(n, vc<bool>(n));
  repi(v, n) res[v][v] = true;
  fec([edge, count] : edges)
    if (count) res[edge.first][edge.second] = res[edge.second][edge.first] = true;
  repi(k, n) repi(u, n) repi(v, n) res[u][v] = res[u][v] || (res[u][k] && res[k][v]);
  return res;
}

void test_random_connectivity()
{
  mt19937 rng(712408);
  repi(_, 100)
  {
    const int n = 1 + rng() % 8, q = 1 + rng() % 60;
    OfflineDynamicConnectivity<pair<int, int>> dc(q);
    map<pair<int, int>, int> edges;
    vc<vvc<bool>> expected;
    repi(t, q)
    {
      int u = rng() % n, v = rng() % n;
      if (u > v) swap(u, v);
      pair<int, int> edge{u, v};
      if (edges[edge] && rng() % 2)
      {
        dc.erase_if(t, edge);
        edges[edge]--;
      }
      else
      {
        dc.add(t, edge);
        edges[edge]++;
      }
      expected.eb(reachability(n, edges));
    }
    UnionFindUndo<UFDataEmpty<>> uf(n);
    dc.run(
        [&](auto edge) { uf.merge(edge.first, edge.second); },
        [&] { uf.undo(); },
        [&](int t)
        {
          repi(u, n) repi(v, n) assert(uf.same(u, v) == expected[t][u][v]);
        });
    repi(v, n) assert(uf.size(v) == 1);
  }
}

void test_legacy_functions()
{
  vc<pair<ll, int>> events{{4, 1}, {1, 0}, {0, 1}, {3, 0}, {2, 2}};
  auto spans = add_del_to_span<int>(5, events);
  vc<tuple<int, int, int>> expected_spans{{1, 3, 0}, {0, 4, 1}, {2, 5, 2}};
  assert(spans == expected_spans);
  vvc<int> expected{{0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1}, {0, 0, 1}};
  vc<int> counts(3), history;
  int next_time = 0;
  OfflineDynamicConnectivity<int> dc(5, std::move(spans));
  dc.run(
      [&](int obj) { counts[obj]++; history.eb(obj); },
      [&] { counts[history.back()]--; history.pop_back(); },
      [&](int t) { assert(t == next_time++); assert(counts == expected[t]); });
  assert(next_time == 5 && history.empty() && counts == vc<int>(3));
  assert(add_del_to_span(0, vc<pair<int, int>>{}).empty());

  // 区間端の型が小さくても、セグメント木の添字は int で計算する。
  vc<tuple<int8_t, int8_t, int>> small_spans{{60, 99, 0}, {0, 99, 1}};
  counts.assign(2, 0);
  next_time = 0;
  OfflineDynamicConnectivity<int> small(99, std::move(small_spans));
  small.run(
      [&](int obj) { counts[obj]++; history.eb(obj); },
      [&] { counts[history.back()]--; history.pop_back(); },
      [&](int t)
      {
        assert(t == next_time++);
        assert(counts == vc<int>({int(t >= 60), 1}));
      });
  assert(next_time == 99 && history.empty() && counts == vc<int>(2));
}

void test_grid_updates()
{
  mt19937 rng(827031);
  repi(_, 100)
  {
    const int n = 1 + rng() % 4, w = 1 + rng() % 4, q = 1 + rng() % 20;
    OfflineDynamicConnectivity<pair<int, int>> dc(q + 1);
    vc<string> rows(n, string(w, 'a'));
    auto randomize = [&](string &row)
    { for (char &c : row) c = 'a' + rng() % 3; };
    for (auto &row : rows) randomize(row);

    auto grid_edges = [&]
    {
      map<pair<int, int>, int> edges;
      repi(i, n) repi(j, w)
      {
        int u = i * w + j;
        if (i + 1 < n && rows[i][j] == rows[i + 1][j]) edges[{u, u + w}] = 1;
        if (j + 1 < w && rows[i][j] == rows[i][j + 1]) edges[{u, u + 1}] = 1;
      }
      return edges;
    };
    auto initial = grid_edges();
    fec([edge, count] : initial) dc.add(0, edge);
    vc<vvc<bool>> expected{reachability(n * w, initial)};
    repi(t, 1, q + 1)
    {
      int i = rng() % n;
      randomize(rows[i]);
      auto update = [&](int u, int v, bool present)
      {
        dc.erase_if(t, {u, v});
        if (present) dc.add(t, {u, v});
      };
      repi(j, w)
      {
        int u = i * w + j;
        if (j + 1 < w) update(u, u + 1, rows[i][j] == rows[i][j + 1]);
        if (i > 0) update(u - w, u, rows[i - 1][j] == rows[i][j]);
        if (i + 1 < n) update(u, u + w, rows[i][j] == rows[i + 1][j]);
      }
      expected.eb(reachability(n * w, grid_edges()));
    }
    UnionFindUndo<UFDataEmpty<>> uf(n * w);
    dc.run(
        [&](auto edge) { uf.merge(edge.first, edge.second); },
        [&] { uf.undo(); },
        [&](int t)
        {
          repi(u, n * w) repi(v, n * w) assert(uf.same(u, v) == expected[t][u][v]);
        });
    repi(v, n * w) assert(uf.size(v) == 1);
  }
}

// 比較だけを持ち、既定構築・代入を持たない型も操作の登録に使える。
struct CompareOnlyObject
{
  int key, label;
  CompareOnlyObject() = delete;
  CompareOnlyObject(int key, int label) : key(key), label(label) {}
  CompareOnlyObject(const CompareOnlyObject &) = default;
  CompareOnlyObject &operator=(const CompareOnlyObject &) = delete;
  bool operator<(const CompareOnlyObject &other) const { return key < other.key; }
};

void test_compare_only_object()
{
  OfflineDynamicConnectivity<CompareOnlyObject> dc(6);
  dc.erase_if(0, {2, 99});
  dc.add(0, {2, 20});
  dc.add(1, {1, 10});
  dc.add(1, {2, 21});
  dc.erase_if(2, {2, 99});
  dc.erase_if(3, {2, 99});
  dc.add(3, {2, 30});
  dc.erase_if(4, {2, 99});
  dc.add(4, {2, 40});
  dc.add_span(0, 6, {3, 300});
  auto labels = [](const auto &dc)
  {
    vc<tuple<int, int, int, int>> res;
    for (const auto &[l, r, obj] : dc.spans()) res.eb(l, r, obj.key, obj.label);
    sort(ALL(res));
    return res;
  };
  vc<tuple<int, int, int, int>> expected{
      {0, 3, 2, 20}, {0, 6, 3, 300}, {1, 2, 2, 20},
      {1, 6, 1, 10}, {3, 4, 2, 30}, {4, 6, 2, 40}};
  assert(labels(dc) == expected);
  auto copy = dc;
  copy.erase_if(5, {1, 0});
  assert(labels(dc) == expected && labels(copy) != expected);
  auto moved = std::move(copy);
  assert(labels(moved) != expected);
  copy.add(5, {4, 400});
  assert((labels(copy) == vc<tuple<int, int, int, int>>({{5, 6, 4, 400}})));

  vc<int> counts(4), history;
  vvc<int> states{{0, 0, 1, 1}, {0, 1, 2, 1}, {0, 1, 1, 1},
                 {0, 1, 1, 1}, {0, 1, 1, 1}, {0, 1, 1, 1}};
  dc.run(
      [&](const auto &obj) { counts[obj.key]++; history.eb(obj.key); },
      [&] { counts[history.back()]--; history.pop_back(); },
      [&](int t) { assert(counts == states[t]); });
  assert(history.empty() && counts == vc<int>(4));
}

void test_full_lifetime()
{
  int added = 0, undone = 0, state = 0, next_time = 0;
  OfflineDynamicConnectivity<int> dc(50001, vc<tuple<int, int, int>>{{0, 50001, 1}, {50001, 50001, 100}});
  dc.run(
      [&](int x) { added++; state += x; },
      [&] { undone++; state--; },
      [&](int t) { assert(t == next_time++); assert(state == 1); });
  assert(added == 1 && undone == 1 && next_time == 50001 && state == 0);
}

struct UnorderedObject
{
  inline static int copies = 0;
  int value;
  UnorderedObject() = delete;
  explicit UnorderedObject(int value) : value(value) {}
  UnorderedObject(const UnorderedObject &other) : value(other.value) { copies++; }
  UnorderedObject(UnorderedObject &&other) noexcept : value(other.value) {}
  UnorderedObject &operator=(const UnorderedObject &) = delete;
};

void test_span_only_interface()
{
  using Obj = UnorderedObject;
  vc<tuple<int, int, Obj>> source;
  source.eb(0, 5, Obj(3));
  source.eb(0, 5, Obj(7));
  source.eb(5, 5, Obj(100));
  Obj::copies = 0;
  OfflineDynamicConnectivity dc(5, std::move(source));
  assert(Obj::copies == 0);
  repi(_, 2)
  {
    int state = 0, next_time = 0;
    vc<int> history;
    Obj::copies = 0;
    dc.run(
        [&](const Obj &obj) { state += obj.value; history.eb(obj.value); },
        [&] { state -= history.back(); history.pop_back(); },
        [&](int t) { assert(t == next_time++); assert(state == 10); });
    // 実行用の各ノードへのコピーだけが発生する。
    assert(Obj::copies == 2);
    assert(state == 0 && next_time == 5 && history.empty());
  }
  assert(dc.spans().size() == 2);
  dc.add_span(1, 4, Obj(11));
  assert(dc.spans().size() == 3);

  OfflineDynamicConnectivity<Obj> built(1);
  built.add_span(0, 1, Obj(17));
  int state = 0;
  built.run([&](const Obj &obj) { state += obj.value; }, [&] { state -= 17; },
            [&](int t) { assert(t == 0 && state == 17); });
  assert(state == 0);

  vc<tuple<ll, ll, Obj>> wide;
  wide.eb(0, 1, Obj(23));
  Obj::copies = 0;
  OfflineDynamicConnectivity<Obj> converted(1, std::move(wide));
  assert(Obj::copies == 0);
  converted.run([&](const Obj &obj) { state += obj.value; }, [&] { state -= 23; },
                [&](int t) { assert(t == 0 && state == 23); });
  assert(state == 0);

  // 初期区間と追加・削除操作も併用できる。
  OfflineDynamicConnectivity<int> mixed(3, vc<tuple<int, int, int>>{{0, 3, 0}});
  mixed.add(1, 0);
  mixed.erase_if(2, 0);
  check_states(mixed, {{1}, {2}, {1}});
}

int main()
{
  test_boundaries();
  test_erase_if();
  test_random_states();
  test_random_connectivity();
  test_grid_updates();
  test_legacy_functions();
  test_compare_only_object();
  test_full_lifetime();
  test_span_only_interface();
  PRINT("Hello World");
}
