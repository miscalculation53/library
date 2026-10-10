#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/meldable_heap.hpp"

template <class Compare>
void test_random(Compare comp)
{
  MeldableHeapPool<ll, Compare> pool(4, comp);
  vc<MeldableHeap<ll, Compare>> heaps;
  vc<multiset<ll, Compare>> expected;
  repi(i, 12) heaps.eb(pool.make_heap()), expected.eb(comp);
  mt19937 rng(3756);
  repi(iter, 15000)
  {
    int a = rng() % 12, b = rng() % 12, op = rng() % 10;
    ll value = int(rng() % 100) - 50;
    if (op < 4)
    {
      heaps[a].push(value);
      expected[a].insert(value);
    }
    else if (op == 4 && !expected[a].empty())
    {
      assert(heaps[a].top() == *expected[a].begin());
      heaps[a].pop();
      expected[a].erase(expected[a].begin());
    }
    else if (op < 7)
    {
      size_t before = pool.node_count();
      heaps[a].merge(heaps[b]);
      assert(pool.node_count() == before);
      if (a != b)
      {
        expected[a].insert(ALL(expected[b]));
        expected[b].clear();
      }
    }
    else if (op == 7)
    {
      size_t before = pool.node_count();
      heaps[b] = heaps[a].clone();
      assert(pool.node_count() == before + expected[a].size());
      expected[b] = expected[a];
    }
    else if (op == 8)
    {
      heaps[b] = std::move(heaps[a]);
      if (a != b) expected[b] = expected[a], expected[a].clear();
    }
    else
    {
      heaps[a].clear();
      expected[a].clear();
    }
    repi(i, 12)
    {
      assert(heaps[i].size() == int(expected[i].size()));
      assert(heaps[i].empty() == expected[i].empty());
      if (!heaps[i].empty()) assert(heaps[i].top() == *expected[i].begin());
    }
    if (iter % 500 == 499)
    {
      repi(i, 12)
      {
        size_t before = pool.node_count();
        const auto &heap = heaps[i];
        assert(heap.content() == vc<ll>(ALL(expected[i])));
        assert(content(heap) == vc<ll>(ALL(expected[i])));
        assert(pool.node_count() == before);
        auto copy = heaps[i].clone();
        for (ll x : expected[i]) assert(copy.top() == x), copy.pop();
        assert(copy.empty());
      }
      pool.clear();
      for (auto &heap : heaps) heap = pool.make_heap();
      for (auto &values : expected) values.clear();
    }
  }
}

struct Item
{
  unique_ptr<int> value;
  Item() = delete;
  explicit Item(int x) : value(make_unique<int>(x)) {}
  Item(Item &&) noexcept = default;
  Item &operator=(Item &&) noexcept = default;
  Item(const Item &) = delete;
  bool operator<(const Item &other) const { return *value < *other.value; }
};

struct Direction
{
  bool descending;
  Direction() = delete;
  explicit Direction(bool descending) : descending(descending) {}
  bool operator()(ll a, ll b) const { return descending ? a > b : a < b; }
};

struct CopyOnly
{
  int value;
  CopyOnly() = delete;
  explicit CopyOnly(int x) : value(x) {}
  CopyOnly(const CopyOnly &) = default;
  CopyOnly &operator=(const CopyOnly &) = delete;
  bool operator<(const CopyOnly &other) const { return value < other.value; }
};

// Test focus: move-only values, pool transfers, aliasing pushes, and deep left spines.
int main()
{
  static_assert(!is_copy_constructible_v<MeldableHeap<ll>>);
  static_assert(is_nothrow_move_constructible_v<MeldableHeap<ll>>);
  test_random(less<ll>{});
  test_random(greater<ll>{});
  test_random(Direction(true));
  MeldableHeapPool<CopyOnly> copy_pool;
  auto copy_heap = copy_pool.make_heap();
  for (int value : {3, 1, 2, 1}) copy_heap.emplace(value);
  const auto copied_values = copy_heap.content();
  assert(copied_values.size() == 4);
  repi(i, 4) assert(copied_values[i].value == vc<int>({1, 1, 2, 3})[i]);
  assert(copy_pool.node_count() == 4 && copy_heap.size() == 4 && copy_heap.top().value == 1);
  MeldableHeapPool<Item> move_pool;
  auto move_heap = move_pool.make_heap(Item(3));
  move_heap.emplace(1);
  move_heap.push(Item(2));
  for (int value : {1, 2, 3}) assert(*move_heap.top().value == value), move_heap.pop();
  assert(move_heap.empty());
  MeldableHeapPool<ll> p, q;
  const ll five = 5;
  auto a = p.make_heap(five), b = q.make_heap(9LL);
  swap(a, b);
  a.push(1);
  assert(a.top() == 1 && b.top() == 5);
  b = std::move(a);
  assert(a.empty() && b.top() == 1);
  auto moved = std::move(b);
  assert(b.empty() && moved.size() == 2);
  moved.merge(moved);
  auto &alias = moved;
  moved = std::move(alias);
  assert(moved.size() == 2);
  repi(i, 100) moved.push(moved.top());
  assert(moved.size() == 102 && moved.top() == 1);
  MeldableHeapPool<int> deep_pool(200000);
  auto deep = deep_pool.make_heap();
  for (int x = 199999; x >= 0; x--) deep.push(x);
  const auto values = deep.content();
  assert(values.size() == 200000 && deep_pool.node_count() == 200000);
  repi(x, 200000) assert(values[x] == x);
  auto copy = deep.clone();
  repi(x, 200000)
  {
    assert(copy.top() == x && deep.top() == x);
    copy.pop(), deep.pop();
  }
  assert(copy.empty() && deep.empty());
#ifdef LOCAL
  cp::options::es_style = cp::types::es_style_t::no_es;
  cp::options::max_line_width = 1000;
  cp::options::cont_indent_style = cp::types::cont_indent_style_t::minimal;
  MeldableHeapPool<int> dump_pool;
  auto displayed = dump_pool.make_heap(), other = dump_pool.make_heap(3);
  for (int value : {7, 1, 1}) displayed.push(value);
  displayed.merge(other);
  const auto &const_heap = displayed;
  string text = cp::export_var(const_heap);
  assert(text.find("content()= " + cp::export_var(vc<int>{1, 1, 3, 7})) != string::npos);
  assert(displayed.size() == 4 && displayed.top() == 1 && dump_pool.node_count() == 4);
  assert(cp::export_var(other).find("content()= [ ]") != string::npos);
  auto moved_display = std::move(displayed);
  assert(cp::export_var(displayed).find("content()= [ ]") != string::npos);
  dump(moved_display);
  dump(other);
  vc<MeldableHeap<int>> group;
  group.eb(std::move(moved_display));
  group.eb(dump_pool.make_heap());
  assert(cp::export_var(group).find("content()= [ 1, 1, 3, 7 ]") != string::npos);
#endif
  PRINT("Hello World");
}
