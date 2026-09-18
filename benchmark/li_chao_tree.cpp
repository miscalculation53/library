// Comparison prototypes; the public convex/ API is intentionally left for later.
// g++-15 -std=c++20 -O2 -Wall -Wextra benchmark/li_chao_tree.cpp -o /tmp/li-chao-bench
// /tmp/li-chao-bench [repetitions=7] [max_n=1000000] [case=all]
// /tmp/li-chao-bench --check
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;
using i64 = int64_t;
using i128 = __int128_t;
using Clock = chrono::steady_clock;
constexpr i128 INF = i128(1) << 126;

struct Line
{
  i64 a = 0, b = 0;
  int id = -1;
  i128 eval(i64 x) const { return i128(a) * x + b; }
};

// Inclusive integer coordinate domain. A missing line is represented by id=-1.
template <bool Segments>
struct Compressed
{
  vector<i64> xs;
  vector<Line> tree;

  explicit Compressed(vector<i64> coordinates) : xs(std::move(coordinates))
  {
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    tree.resize(4 * xs.size());
  }

  void insert(Line f, int p, int l, int r)
  {
    while (true)
    {
      if (tree[p].id == -1) { tree[p] = f; return; }
      int m = (l + r) / 2;
      if (f.eval(xs[m]) < tree[p].eval(xs[m])) swap(f, tree[p]);
      if (l == r) return;
      if (f.eval(xs[l]) < tree[p].eval(xs[l])) p *= 2, r = m;
      else if (f.eval(xs[r]) < tree[p].eval(xs[r])) p = 2 * p + 1, l = m + 1;
      else return;
    }
  }

  void add_line(Line f) { insert(f, 1, 0, int(xs.size()) - 1); }

  void segment(Line f, int ql, int qr, int p, int l, int r)
  {
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) { insert(f, p, l, r); return; }
    int m = (l + r) / 2;
    segment(f, ql, qr, 2 * p, l, m);
    segment(f, ql, qr, 2 * p + 1, m + 1, r);
  }

  int lower(i64 x) const { return lower_bound(xs.begin(), xs.end(), x) - xs.begin(); }
  int upper(i64 x) const { return upper_bound(xs.begin(), xs.end(), x) - xs.begin(); }
  void add_segment_index(Line f, int l, int r)
  {
    if (l <= r) segment(f, l, r, 1, 0, int(xs.size()) - 1);
  }
  void add_segment(Line f, i64 l, i64 r) { add_segment_index(f, lower(l), upper(r) - 1); }

  // Coordinate routing avoids a separate lower_bound for every query.
  template <bool Indexed>
  i128 query(i64 x, int k = -1) const
  {
    i128 ans = INF;
    int p = 1, l = 0, r = int(xs.size()) - 1;
    while (true)
    {
      if (tree[p].id != -1) ans = min(ans, tree[p].eval(x));
      else if constexpr (!Segments) break;
      if (l == r) break;
      int m = (l + r) / 2;
      bool left;
      if constexpr (Indexed) left = k <= m;
      else left = x <= xs[m];
      if (left) p *= 2, r = m;
      else p = 2 * p + 1, l = m + 1;
    }
    return ans;
  }

  size_t bytes() const { return xs.capacity() * sizeof(i64) + tree.capacity() * sizeof(Line); }
};

struct Dynamic
{
  struct Node
  {
    Line line;
    int child[2] = {-1, -1};
  };
  i64 lo, hi;
  vector<Node> nodes;

  Dynamic(i64 l, i64 r, size_t reserve_count) : lo(l), hi(r)
  {
    nodes.reserve(reserve_count);
    nodes.emplace_back();
  }
  int child(int p, int dir)
  {
    if (nodes[p].child[dir] == -1)
    {
      int k = nodes.size();
      nodes.emplace_back();
      nodes[p].child[dir] = k;
    }
    return nodes[p].child[dir];
  }
  void insert(Line f, int p, i64 l, i64 r)
  {
    while (true)
    {
      if (nodes[p].line.id == -1) { nodes[p].line = f; return; }
      i64 m = midpoint(l, r);
      if (f.eval(m) < nodes[p].line.eval(m)) swap(f, nodes[p].line);
      if (l == r) return;
      if (f.eval(l) < nodes[p].line.eval(l)) p = child(p, 0), r = m;
      else if (f.eval(r) < nodes[p].line.eval(r)) p = child(p, 1), l = m + 1;
      else return;
    }
  }
  void add_line(Line f) { insert(f, 0, lo, hi); }
  void segment(Line f, i64 ql, i64 qr, int p, i64 l, i64 r)
  {
    if (ql <= l && r <= qr) { insert(f, p, l, r); return; }
    i64 m = midpoint(l, r);
    if (ql <= m) segment(f, ql, qr, child(p, 0), l, m);
    if (m < qr) segment(f, ql, qr, child(p, 1), m + 1, r);
  }
  void add_segment(Line f, i64 l, i64 r)
  {
    l = max(l, lo), r = min(r, hi);
    if (l <= r) segment(f, l, r, 0, lo, hi);
  }
  i128 query(i64 x) const
  {
    i128 ans = INF;
    int p = 0;
    i64 l = lo, r = hi;
    while (p != -1)
    {
      if (nodes[p].line.id != -1) ans = min(ans, nodes[p].line.eval(x));
      if (l == r) break;
      i64 m = midpoint(l, r);
      if (x <= m) p = nodes[p].child[0], r = m;
      else p = nodes[p].child[1], l = m + 1;
    }
    return ans;
  }
  size_t bytes() const { return nodes.capacity() * sizeof(Node); }
};

struct Op
{
  // 0 = query, 1 = whole line, 2 = segment on [l,r].
  int type;
  Line line;
  i64 l, r;
};
struct Input
{
  string name;
  int n;
  i64 lo, hi;
  vector<Op> ops;
};
struct Result
{
  double prep_ms, ops_ms;
  size_t bytes, nodes;
  uint64_t checksum;
  vector<i128> answers;
};

uint64_t mix(uint64_t hash, i128 x)
{
  auto u = __uint128_t(x);
  return (hash ^ uint64_t(u) ^ uint64_t(u >> 64)) * 1000000007ULL;
}
double ms(Clock::time_point a, Clock::time_point b)
{
  return chrono::duration<double, milli>(b - a).count();
}

template <bool Segments, bool Indexed>
Result run_compressed(const Input &in, bool capture)
{
  vector<i128> answers;
  if (capture) answers.reserve(in.ops.size());
  auto start = Clock::now();
  vector<i64> coordinates;
  coordinates.reserve(in.ops.size() - in.n);
  for (auto &op : in.ops) if (op.type == 0) coordinates.push_back(op.l);
  Compressed<Segments> tree(std::move(coordinates));
  vector<array<int, 2>> indices;
  if constexpr (Indexed)
  {
    indices.resize(in.ops.size());
    for (size_t i = 0; i < in.ops.size(); ++i)
    {
      auto &op = in.ops[i];
      if (op.type == 0) indices[i][0] = tree.lower(op.l);
      if (op.type == 2) indices[i] = {tree.lower(op.l), tree.upper(op.r) - 1};
    }
  }
  auto prepared = Clock::now();
  uint64_t hash = 0;
  for (size_t i = 0; i < in.ops.size(); ++i)
  {
    auto &op = in.ops[i];
    if (op.type == 1) tree.add_line(op.line);
    else if (op.type == 2)
    {
      if constexpr (Indexed) tree.add_segment_index(op.line, indices[i][0], indices[i][1]);
      else tree.add_segment(op.line, op.l, op.r);
    }
    else
    {
      i128 value;
      if constexpr (Indexed) value = tree.template query<true>(op.l, indices[i][0]);
      else value = tree.template query<false>(op.l);
      hash = mix(hash, value);
      if (capture) answers.push_back(value);
    }
  }
  auto end = Clock::now();
  size_t used = count_if(tree.tree.begin(), tree.tree.end(), [](auto &f) { return f.id != -1; });
  return {ms(start, prepared), ms(prepared, end), tree.bytes() + indices.capacity() * sizeof(indices[0]),
          used, hash, std::move(answers)};
}

Result run_dynamic(const Input &in, bool wide, bool capture)
{
  vector<i128> answers;
  if (capture) answers.reserve(in.ops.size());
  auto start = Clock::now();
  Dynamic tree(wide ? -(i64(1) << 60) : in.lo,
               wide ? (i64(1) << 60) - 1 : in.hi, in.n);
  auto prepared = Clock::now();
  uint64_t hash = 0;
  for (auto &op : in.ops)
  {
    if (op.type == 1) tree.add_line(op.line);
    else if (op.type == 2) tree.add_segment(op.line, op.l, op.r);
    else
    {
      i128 value = tree.query(op.l);
      hash = mix(hash, value);
      if (capture) answers.push_back(value);
    }
  }
  auto end = Clock::now();
  return {ms(start, prepared), ms(prepared, end), tree.bytes(), tree.nodes.size(), hash, std::move(answers)};
}

Result run(const Input &in, int method, bool capture = false)
{
  bool segments = in.name.find("segment") != string::npos;
  if (method >= 2) return run_dynamic(in, method == 3, capture);
  if (segments)
    return method == 0 ? run_compressed<true, false>(in, capture) : run_compressed<true, true>(in, capture);
  return method == 0 ? run_compressed<false, false>(in, capture) : run_compressed<false, true>(in, capture);
}

Input generate(string name, int n, uint64_t seed)
{
  mt19937_64 rng(seed);
  bool random_lines = name == "random";
  bool segments = name.find("segment") != string::npos;
  bool repeated = name == "repeated";
  i64 step = name == "dense" ? 1 : 1000;
  i64 bound = random_lines ? 1'000'000'000 : i64(n) * step / 2;
  Input in{name, n, -bound, bound, {}};
  int queries_per_add = name == "query_heavy" ? 5 : 1;
  in.ops.reserve(n * (queries_per_add + 1));
  vector<int> permutation(n);
  iota(permutation.begin(), permutation.end(), 0);
  shuffle(permutation.begin(), permutation.end(), rng);
  auto coordinate = [&] { return i64(rng() % (2 * bound + 1)) - bound; };
  for (int i = 0; i < n; ++i)
  {
    i64 t = (i64(permutation[i]) - n / 2) * step;
    Line f = random_lines ? Line{coordinate(), coordinate(), i} : Line{-2 * t, t * t, i};
    i64 l = in.lo, r = in.hi;
    if (segments)
    {
      if (name == "segment_narrow") l = t - step, r = t + step;
      else l = coordinate(), r = coordinate();
      if (r < l) swap(l, r);
    }
    in.ops.push_back({segments ? 2 : 1, f, l, r});
    for (int j = 0; j < queries_per_add; ++j)
    {
      i64 x = repeated ? -bound + i64(rng() % 256) * (2 * bound) / 255 : coordinate();
      in.ops.push_back({0, {}, x, x});
    }
  }
  return in;
}

void check_input(const Input &in, bool brute)
{
  auto expected = run(in, 0, true).answers;
  for (int method = 1; method < 4; ++method)
    if (run(in, method, true).answers != expected) throw runtime_error("variant mismatch: " + in.name);
  if (!brute) return;
  vector<Op> lines;
  size_t i = 0;
  for (auto &op : in.ops)
  {
    if (op.type) lines.push_back(op);
    else
    {
      i128 answer = INF;
      for (auto &f : lines)
        if (f.type == 1 || (f.l <= op.l && op.l <= f.r)) answer = min(answer, f.line.eval(op.l));
      if (expected[i++] != answer) throw runtime_error("brute-force mismatch: " + in.name);
    }
  }
}

void correctness()
{
  for (int seed = 0; seed < 20; ++seed)
    for (auto name : {"random", "dense", "sparse", "repeated", "segment_wide", "segment_narrow"})
      check_input(generate(name, 1 + 5 * seed, 12345 + seed), true);
  // Empty queries, equal slopes, ties, single-point and empty segments, signed boundaries.
  Input small{"segment_edges", 7, -3, 3,
              {{0, {}, -3, -3}, {2, {0, 9, 0}, 2, 1}, {2, {1, 0, 1}, -3, -3},
               {0, {}, -3, -3}, {0, {}, -2, -2}, {1, {0, 5, 2}, 0, 0},
               {1, {0, 4, 3}, 0, 0}, {1, {0, 4, 4}, 0, 0}, {2, {-1, 0, 5}, 3, 3},
               {0, {}, 3, 3}, {2, {7, -1, 6}, 0, 0}, {0, {}, 0, 0}, {0, {}, 1, 1}}};
  check_input(small, true);
  for (int seed = 0; seed < 30; ++seed)
  {
    mt19937_64 rng(67890 + seed);
    Input in{"segment_exhaustive", 40, -8, 8, {}};
    for (int i = 0; i < in.n; ++i)
    {
      i64 l = i64(rng() % 25) - 12, r = i64(rng() % 25) - 12;
      Line f{i64(rng() % 21) - 10, i64(rng() % 21) - 10, i};
      in.ops.push_back({i % 7 == 0 ? 1 : 2, f, l, r});
      for (i64 x = in.lo; x <= in.hi; ++x) in.ops.push_back({0, {}, x, x});
    }
    check_input(in, true);
  }
  Dynamic extreme(numeric_limits<i64>::min(), numeric_limits<i64>::max(), 1);
  vector<Line> fs = {{1, 0, 0}, {-1, 0, 1}, {0, numeric_limits<i64>::min(), 2}};
  for (auto f : fs) extreme.add_line(f);
  for (i64 x : {numeric_limits<i64>::min(), i64(-1), i64(0), i64(1), numeric_limits<i64>::max()})
  {
    i128 answer = INF;
    for (auto f : fs) answer = min(answer, f.eval(x));
    if (answer != extreme.query(x)) throw runtime_error("signed boundary mismatch");
  }
  extreme.add_segment({2, 0, 3}, numeric_limits<i64>::min(), numeric_limits<i64>::min());
  extreme.add_segment({-2, 0, 4}, numeric_limits<i64>::max(), numeric_limits<i64>::max());
  if (extreme.query(numeric_limits<i64>::min()) != i128(2) * numeric_limits<i64>::min() ||
      extreme.query(numeric_limits<i64>::max()) != -i128(2) * numeric_limits<i64>::max())
    throw runtime_error("single-point signed boundary mismatch");
  cerr << "Correctness: 150 generated cases + boundary cases passed.\n";
}

int main(int argc, char **argv)
{
  correctness();
  if (argc > 1 && string(argv[1]) == "--check") return 0;
  int reps = argc > 1 ? stoi(argv[1]) : 7;
  int max_n = argc > 2 ? stoi(argv[2]) : 1'000'000;
  string selected = argc > 3 ? argv[3] : "all";
  constexpr array<const char *, 4> names = {"compressed_x", "compressed_index", "dynamic", "dynamic_wide"};
  cout << fixed << setprecision(6);
  cout << "case,n,queries,method,rep,prep_ms,ops_ms,total_ms,allocated_bytes,used_nodes,checksum\n";
  for (string name : {"random", "dense", "sparse", "repeated", "query_heavy", "segment_wide", "segment_narrow"})
  {
    if (selected != "all" && name != selected) continue;
    for (int n : {50'000, 200'000, 1'000'000})
    {
      if (n > max_n || (name.find("segment") != string::npos && n > 200'000)) continue;
      auto in = generate(name, n, 20260917 + n);
      check_input(in, false); // Also warms each method; full output comparison is outside timing.
      cerr << "Measuring " << name << " n=" << n << '\n';
      uint64_t reference = run(in, 0).checksum;
      for (int rep = 0; rep < reps; ++rep)
        for (int offset = 0; offset < 4; ++offset)
        {
          int method = (rep + offset) % 4;
          auto r = run(in, method);
          if (r.checksum != reference) throw runtime_error("timed checksum mismatch");
          cout << name << ',' << n << ',' << in.ops.size() - n << ',' << names[method] << ',' << rep
               << ',' << r.prep_ms << ',' << r.ops_ms << ',' << r.prep_ms + r.ops_ms
               << ',' << r.bytes << ',' << r.nodes << ',' << r.checksum << '\n';
        }
      cout.flush();
    }
  }
}
