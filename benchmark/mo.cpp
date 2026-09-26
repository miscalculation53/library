// g++-15 -std=c++20 -O2 -DNDEBUG -I . benchmark/mo.cpp -o /tmp/mo-bench
// /tmp/mo-bench [n=200000] [q=200000] [repetitions=5] [filter=all]
// filter: a dataset name, tree_walk (fixed-order microbenchmark), or check.
#include "ds/mo/mo.hpp"
#include "graph/tree/mo_on_tree.hpp"
#include "mo_reference.hpp"

using Clock = chrono::steady_clock;
using Queries = vc<pair<ll, ll>>;
volatile ull benchmark_sink = 0;

ll distance(const Queries &qs, int a, int b)
{
  auto [x, y] = a == -1 ? pair<ll, ll>{0, 0} : qs[a];
  return abs(x - qs[b].first) + abs(y - qs[b].second);
}

ll cost(const Queries &qs, const vc<int> &ord, bool origin = true)
{
  ll res = 0;
  repi(i, ord.size())
    if (i || origin) res += distance(qs, i ? ord[i - 1] : -1, ord[i]);
  return res;
}

// Key comparison has exactly the same equivalence classes as mo_order_params.
vc<int> key_order(const Queries &qs, int b, int t, bool radix = false)
{
  struct Item { ull key; int id; };
  int q = qs.size();
  vc<ull> keys(q);
  ull max_key = 0;
  repi(i, q)
  {
    uint block = (qs[i].first + t * b / 2) / b;
    uint right = qs[i].second;
    keys[i] = (ull(block) << 32) | ((block & 1) ? ~right : right);
    max_key = max(max_key, keys[i]);
  }
  vc<int> ord = permid<int>(q);
  if (!radix)
  {
    sort(ALL(ord), [&](int i, int j) { return keys[i] < keys[j]; });
    return ord;
  }
  vc<Item> a(q), tmp(q);
  repi(i, q) a[i] = {keys[i], i};
  vc<int> cnt(1 << 16);
  for (int shift = 0; shift < 64; shift += 16)
  {
    fill(ALL(cnt), 0);
    for (auto item : a) cnt[(item.key >> shift) & 65535]++;
    int sum = 0;
    for (int &c : cnt) { int old = c; c = sum; sum += old; }
    for (auto item : a) tmp[cnt[(item.key >> shift) & 65535]++] = item;
    a.swap(tmp);
    if (shift == 48 || (max_key >> (shift + 16)) == 0) break;
  }
  repi(i, q) ord[i] = a[i].id;
  return ord;
}

void local_improve(const Queries &qs, vc<int> &ord)
{
  repi(i, int(ord.size()) - 1)
  {
    int p = i ? ord[i - 1] : -1, a = ord[i], b = ord[i + 1];
    ll before = distance(qs, p, a), after = distance(qs, p, b);
    if (i + 2 < int(ord.size()))
    {
      before += distance(qs, b, ord[i + 2]);
      after += distance(qs, a, ord[i + 2]);
    }
    if (after < before) swap(ord[i], ord[i + 1]);
  }
}

vc<int> bucket_order(const Queries &qs, const vc<int> &by_right, int n, int b, int t)
{
  int q = qs.size(), blocks = (n + t * b / 2) / b + 1;
  vc<int> block(q), cursor(blocks), ord(q);
  repi(i, q) cursor[block[i] = (qs[i].first + t * b / 2) / b]++;
  int sum = 0;
  repi(k, blocks)
  {
    int count = cursor[k];
    cursor[k] = sum + ((k & 1) ? count : 0);
    sum += count;
  }
  for (int i : by_right)
  {
    int k = block[i];
    ord[(k & 1) ? --cursor[k] : cursor[k]++] = i;
  }
  return ord;
}

ull hilbert(uint x, uint y, int bits, int rotate = 0)
{
  if (bits == 0) return 0;
  uint half = 1u << (bits - 1);
  int segment = x < half ? (y < half ? 0 : 3) : (y < half ? 1 : 2);
  segment = (segment + rotate) & 3;
  static constexpr int delta[4] = {3, 0, 0, 1};
  ull size = 1ULL << (2 * bits - 2);
  ull sub = hilbert(x & (half - 1), y & (half - 1), bits - 1, (rotate + delta[segment]) & 3);
  return segment * size + ((segment == 1 || segment == 2) ? sub : size - sub - 1);
}

vc<int> order(const Queries &qs, const string &method)
{
  if (qs.empty()) return {};
  if (method == "current6") return mo_reference::mo_order(qs);
  if (method == "library") return internal::mo_order(qs);
  int n = 0, q = qs.size();
  for (auto [l, r] : qs) n = max<ll>(n, max(l, r));
  if (method == "hilbert")
  {
    int bits = 0;
    while ((1ULL << bits) <= ull(n)) bits++;
    vc<ull> keys(q);
    repi(i, q) keys[i] = hilbert(qs[i].first, qs[i].second, bits);
    vc<int> ord = permid<int>(q);
    sort(ALL(ord), [&](int i, int j) { return keys[i] < keys[j]; });
    return ord;
  }
  int b1 = max(1, int(n / sqrt(q + 1)));
  int b2 = max(1, int(sqrt(3) * n / sqrt(2 * q + 1)));
  int b3 = max(1, int(sqrt(2) * n / sqrt(q + 1)));
  if (method == "key1") return key_order(qs, b2, 0);
  vc<int> by_right;
  if (method == "bucket6" || method == "radix_bucket6")
  {
    if (method == "radix_bucket6") by_right = key_order(qs, n + 1, 0, true);
    else
    {
      by_right = permid<int>(q);
      sort(ALL(by_right), [&](int i, int j) { return qs[i].second < qs[j].second; });
    }
  }
  vc<int> best;
  ll best_cost = LLONG_MAX;
  for (int b : {b1, b2, b3})
  {
    for (int t : {0, 1})
    {
      auto ord = by_right.empty() ? key_order(qs, b, t, method == "radix6") : bucket_order(qs, by_right, n, b, t);
      ll c = cost(qs, ord, method == "origin6");
      if (c < best_cost) best_cost = c, best = move(ord);
    }
    if (method == "key2") break;
  }
  if (method == "key6_local") local_improve(qs, best);
  return best;
}

vc<int> walk(const Queries &qs, const vc<int> &ord, const vc<int> &values)
{
  vc<int> cnt(4096), ans(qs.size());
  int l = 0, r = 0, distinct = 0;
  auto add = [&](int i) { distinct += (cnt[values[i]]++ == 0); };
  auto del = [&](int i) { distinct -= (--cnt[values[i]] == 0); };
  for (int id : ord)
  {
    auto [nl, nr] = qs[id];
    while (nl < l) add(--l);
    while (r < nr) add(r++);
    while (l < nl) del(l++);
    while (nr < r) del(--r);
    ans[id] = distinct;
  }
  return ans;
}

struct Sample { double setup, run; ll moves; };
double elapsed(Clock::time_point a, Clock::time_point b)
{
  return chrono::duration<double, milli>(b - a).count();
}
double median(vc<double> x) { sort(ALL(x)); return x[x.size() / 2]; }

void report(const string &dataset, const string &method, int n, int q, const vc<Sample> &samples)
{
  vc<double> setup, run, total;
  for (auto s : samples) setup.push_back(s.setup), run.push_back(s.run), total.push_back(s.setup + s.run);
  cout << dataset << ',' << method << ',' << n << ',' << q << ',' << samples.size() << ','
       << median(setup) << ',' << median(run) << ',' << median(total) << ','
       << *min_element(ALL(total)) << ',' << *max_element(ALL(total)) << ',' << samples.front().moves << endl;
}

void bench_array(const string &dataset, int n, int q, int repeats)
{
  mt19937 rng(20260926);
  Queries qs;
  vc<int> values(n);
  for (int &v : values) v = rng() % 4096;
  vc<pair<int, int>> pool;
  repi(i, 64)
  {
    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r) swap(l, r);
    pool.emplace_back(l, r);
  }
  repi(i, q)
  {
    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r) swap(l, r);
    if (dataset == "short") r = min(n, l + int(rng() % 101));
    if (dataset == "clustered")
    {
      int center = (rng() % 8) * (n / 8);
      l = min(n, center + int(rng() % max(1, n / 200)));
      r = min(n, l + int(rng() % max(1, n / 200)));
    }
    if (dataset == "duplicates") tie(l, r) = pool[rng() % pool.size()];
    if (dataset == "band_edges")
    {
      int b = max(1, int(sqrt(3) * n / sqrt(2 * q + 1)));
      int block = rng() % max(1, n / b);
      l = min(n, block * b + (rng() % 2 ? 0 : b - 1));
      r = l + rng() % (n - l + 1);
      if (i == 0) r = n;
    }
    qs.emplace_back(l, r);
  }
  const vc<string> methods{"current6", "key6", "radix6", "bucket6", "radix_bucket6", "library", "key2", "key1", "key6_local", "hilbert", "origin6"};
  vvc<Sample> samples(methods.size());
  auto expected = walk(qs, order(qs, "current6"), values);
  // Small cases are also checked directly, independently of Mo's ordering/walk.
  if (n <= 300)
    repi(i, q)
    {
      set<int> colors;
      for (int j = qs[i].first; j < qs[i].second; j++) colors.insert(values[j]);
      if (expected[i] != int(colors.size())) abort();
    }
  auto reference_order = order(qs, "current6");
  if (order(qs, "key6") != reference_order) abort();
  if (order(qs, "library") != order(qs, "radix_bucket6")) abort();
  for (int repeat = -1; repeat < repeats; repeat++)
    repi(j, methods.size())
    {
      int m = (j + repeat + 1) % methods.size();
      auto begin = Clock::now();
      auto ord = order(qs, methods[m]);
      auto ordered = Clock::now();
      auto answers = walk(qs, ord, values);
      auto end = Clock::now();
      if (answers != expected) abort();
      auto sorted = ord;
      sort(ALL(sorted));
      if (sorted != permid<int>(q)) abort();
      ll moves = cost(qs, ord);
      if (methods[m] == "key6_local" && moves > cost(qs, reference_order)) abort();
      benchmark_sink = accumulate(ALL(answers), 0ULL);
      if (repeat >= 0) samples[m].push_back({elapsed(begin, ordered), elapsed(ordered, end), moves});
    }
  repi(i, methods.size()) report(dataset, methods[i], n, q, samples[i]);
}

struct TreeQueries { Queries ranges; vc<int> tour, extra; };
TreeQueries convert_tree(const RootedTree &g, const Queries &uvs, bool edge, bool skip_lca)
{
  int n = g.size(), q = uvs.size();
  TreeQueries result{Queries(q), vc<int>(2 * n), vc<int>(q, -1)};
  vc<int> in(n), out(n);
  repi(v, n)
  {
    in[v] = 2 * g.preorder(v) - g.depth(v);
    out[v] = 2 * g.postorder(v) - g.depth(v) - 1;
    result.tour[in[v]] = result.tour[out[v]] = v;
  }
  repi(i, q)
  {
    auto [u, v] = uvs[i];
    if (in[u] > in[v]) swap(u, v);
    int w = skip_lca ? -1 : g.lca(u, v);
    bool ancestor = skip_lca ? g.is_ancestor(u, v) : u == w;
    if (ancestor) result.ranges[i] = {in[u] + edge, in[v] + 1};
    else
    {
      result.ranges[i] = {out[u], in[v] + 1};
      if (!edge) result.extra[i] = skip_lca ? g.lca(u, v) : w;
    }
  }
  return result;
}

template <class Active, bool branchless>
vc<int> walk_tree(const TreeQueries &t, const vc<int> &ord, const vc<int> &values, bool edge, int root)
{
  Active active(values.size(), false);
  vc<int> cnt(4096), ans(ord.size());
  int distinct = 0, l = 0, r = 0;
  auto add = [&](int v) { distinct += (cnt[values[v]]++ == 0); };
  auto del = [&](int v) { distinct -= (--cnt[values[v]] == 0); };
  auto flip = [&](int i)
  {
    int v = t.tour[i];
    if (edge && v == root) return;
    bool before = active[v];
    if constexpr (branchless)
    {
      int &c = cnt[values[v]];
      distinct -= (c != 0);
      c += before ? -1 : 1;
      distinct += (c != 0);
    }
    else if (before) del(v);
    else add(v);
    active[v] = !before;
  };
  for (int id : ord)
  {
    auto [nl, nr] = t.ranges[id];
    while (nl < l) flip(--l);
    while (r < nr) flip(r++);
    while (l < nl) flip(l++);
    while (nr < r) flip(--r);
    if (t.extra[id] != -1) add(t.extra[id]);
    ans[id] = distinct;
    if (t.extra[id] != -1) del(t.extra[id]);
  }
  return ans;
}

void bench_tree(const string &dataset, int n, int q, int repeats)
{
  mt19937 rng(20260926);
  vc<pair<int, int>> es;
  repi(v, 1, n)
  {
    int p = dataset == "tree_chain" ? v - 1 : dataset == "tree_star" ? 0 : rng() % v;
    es.emplace_back(v, p);
  }
  RootedTree g(n, es, n / 2);
  vc<int> values(n);
  for (int &v : values) v = rng() % 4096;
  Queries uvs(q);
  for (auto &[u, v] : uvs) u = rng() % n, v = rng() % n;
  const vc<string> methods{"bits_current6", "bytes_current6", "bytes_key6", "bytes_local", "bytes_branchless", "skip_lca"};
  for (bool edge : {false, true})
  {
    vvc<Sample> samples(methods.size());
    vc<int> expected(q), cnt(4096);
    int distinct = 0;
    mo_on_tree(g, uvs, [&](int v) { distinct += (cnt[values[v]]++ == 0); },
      [&](int v) { distinct -= (--cnt[values[v]] == 0); },
      [&](int id) { expected[id] = distinct; }, edge);
    if (n <= 300)
      repi(i, q)
      {
        auto [u, v] = uvs[i];
        int w = g.lca(u, v);
        set<int> colors;
        for (int x : g.path(u, v)) if (!edge || x != w) colors.insert(values[x]);
        if (expected[i] != int(colors.size())) abort();
      }
    for (int repeat = -1; repeat < repeats; repeat++)
      repi(j, methods.size())
      {
        int m = (j + repeat + 1) % methods.size();
        auto begin = Clock::now();
        auto t = convert_tree(g, uvs, edge, m == 5);
        auto ord = order(t.ranges, m < 2 ? "current6" : m == 3 ? "key6_local" : "key6");
        auto ordered = Clock::now();
        auto answers = m == 0 ? walk_tree<vb, false>(t, ord, values, edge, g.root()) :
                       m == 4 ? walk_tree<vc<unsigned char>, true>(t, ord, values, edge, g.root()) :
                                walk_tree<vc<unsigned char>, false>(t, ord, values, edge, g.root());
        auto end = Clock::now();
        if (answers != expected) abort();
        benchmark_sink = accumulate(ALL(answers), 0ULL);
        if (repeat >= 0) samples[m].push_back({elapsed(begin, ordered), elapsed(ordered, end), cost(t.ranges, ord)});
      }
    repi(i, methods.size()) report(dataset + (edge ? "_edge" : "_vertex"), methods[i], n, q, samples[i]);
  }
}

void bench_tree_walk(int n, int q, int repeats)
{
  cout << "dataset,method,n,q,repeat,walk_ms,cpu_ms\n";
  for (string shape : {"random", "chain", "star"})
  {
    mt19937 rng(20260926);
    vc<pair<int, int>> es;
    repi(v, 1, n) es.emplace_back(v, shape == "chain" ? v - 1 : shape == "star" ? 0 : rng() % v);
    RootedTree g(n, es, n / 2);
    vc<int> values(n);
    for (int &v : values) v = rng() % 4096;
    Queries uvs(q);
    for (auto &[u, v] : uvs) u = rng() % n, v = rng() % n;
    for (bool edge : {false, true})
    {
      auto t = convert_tree(g, uvs, edge, false);
      auto ord = order(t.ranges, "current6");
      auto expected = walk_tree<vb, false>(t, ord, values, edge, g.root());
      for (int repeat = -1; repeat < repeats; repeat++)
        repi(j, 3)
        {
          int m = (j + repeat + 1) % 3;
          auto cpu_begin = std::clock();
          auto begin = Clock::now();
          auto answers = m == 0 ? walk_tree<vb, false>(t, ord, values, edge, g.root()) :
                         m == 1 ? walk_tree<vc<unsigned char>, false>(t, ord, values, edge, g.root()) :
                                  walk_tree<vc<unsigned char>, true>(t, ord, values, edge, g.root());
          auto end = Clock::now();
          auto cpu_end = std::clock();
          if (answers != expected) abort();
          benchmark_sink = accumulate(ALL(answers), 0ULL);
          if (repeat >= 0)
            cout << shape << (edge ? "_edge," : "_vertex,") << (m == 0 ? "bits" : m == 1 ? "bytes" : "branchless")
                 << ',' << n << ',' << q << ',' << repeat << ',' << elapsed(begin, end) << ','
                 << 1000.0 * (cpu_end - cpu_begin) / CLOCKS_PER_SEC << endl;
        }
    }
  }
}

void validate_orders()
{
  mt19937 rng(5331);
  for (int q : {0, 1, 2, 10, 100})
  {
    Queries qs(q);
    for (auto &[l, r] : qs) l = rng() % 101, r = rng() % 101;
    auto by_right = permid<int>(q);
    sort(ALL(by_right), [&](int i, int j) { return qs[i].second < qs[j].second; });
    for (int b : {1, 2, 7, 20, 100, 101}) for (int t : {0, 1})
    {
      if (key_order(qs, b, t) != mo_reference::mo_order_params(qs, b, t)) abort();
      auto key = [&](int i)
      {
        int block = (qs[i].first + t * b / 2) / b;
        return pair<ll, ll>{block, (block & 1) ? -qs[i].second : qs[i].second};
      };
      for (auto ord : {key_order(qs, b, t, true), bucket_order(qs, by_right, 100, b, t)})
      {
        if (!is_sorted(ALL(ord), [&](int i, int j) { return key(i) < key(j); })) abort();
        sort(ALL(ord));
        if (ord != permid<int>(q)) abort();
      }
    }
  }
  for (int bits = 1; bits <= 6; bits++)
  {
    int n = 1 << bits;
    vc<pair<int, int>> xy(n * n, {-1, -1});
    repi(x, n) repi(y, n)
    {
      ull h = hilbert(x, y, bits);
      if (h >= xy.size() || xy[h].first != -1) abort();
      xy[h] = {x, y};
    }
    repi(i, 1, n * n)
      if (abs(xy[i].first - xy[i - 1].first) + abs(xy[i].second - xy[i - 1].second) != 1) abort();
  }
}

int main(int argc, char **argv)
{
  int n = argc > 1 ? stoi(argv[1]) : 200000;
  int q = argc > 2 ? stoi(argv[2]) : n;
  int repeats = argc > 3 ? stoi(argv[3]) : 5;
  string filter = argc > 4 ? argv[4] : "all";
  if (n < 1 || q < 0 || repeats < 1) return 1;
  validate_orders();
  if (filter == "check") return 0;
  cout << fixed << setprecision(3);
  if (filter == "tree_walk")
  {
    bench_tree_walk(n, q, repeats);
    return 0;
  }
  cout << "dataset,method,n,q,repeats,setup_ms,walk_ms,total_ms,min_total_ms,max_total_ms,moves\n";
  for (string dataset : {"random", "short", "clustered", "duplicates", "band_edges", "tree_random", "tree_chain", "tree_star"})
  {
    if (filter != "all" && filter != dataset) continue;
    if (dataset.rfind("tree_", 0) == 0) bench_tree(dataset, n, q, repeats);
    else bench_array(dataset, n, q, repeats);
  }
  return 0;
}
