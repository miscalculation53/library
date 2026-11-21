#pragma once

#include "maxflow.hpp"
#include "internal/capacity_sum.hpp"

/**
 * @brief Project Selection Problem（劣モジュラ二値関数の最小化）
 * @docs docs/flow/project_selection.md
 */

template <class Cost>
class ProjectSelectionProblem
{
  static_assert(is_integral_ext<Cost> && is_signed_ext<Cost>);
  using Unsigned = make_unsigned_ext_t<Cost>;

  template <class T, T> friend class K_ProjectSelectionProblem;

  struct Arc
  {
    int from, to;
    Unsigned cap;
    CPP_DUMP_DEFINE_DATA(from, to, cap);
  };
  static constexpr int source = -1, sink = -2;
  int n = 0, nodes = 0;
  vc<Arc> finite;
  vc<pair<int, int>> hard;
  flow_internal::CapacitySum<Unsigned> bound;
  Unsigned offset = 0;

  void check_vertex(int v) const { assert(0 <= v && v < n); }
  void add_hard_arc(int from, int to)
  {
    if (from != to && from != sink && to != source) hard.emplace_back(from, to);
  }
  void add_arc(int from, int to, Unsigned cap)
  {
    if (from == to || cap == 0) return;
    finite.push_back({from, to, cap});
    bound.add(cap);
  }
  void add_unary(int v, Cost cost0, Cost cost1)
  {
    if (cost0 <= cost1)
      offset += Unsigned(cost0), add_arc(v, sink, Unsigned(cost1) - Unsigned(cost0));
    else offset += Unsigned(cost1), add_arc(source, v, Unsigned(cost0) - Unsigned(cost1));
  }
  void add_unary_difference(int v, Cost f0, Cost f1)
  {
    add_unary(v, f0, f1);
    offset -= Unsigned(f0);
  }
  static bool sum_leq(Cost a, Cost b, Cost c, Cost d)
  {
    flow_internal::SignedCapacitySum<Cost> lhs, rhs;
    lhs.add(a), lhs.add(b);
    rhs.add(c), rhs.add(d);
    return tie(lhs.blocks, lhs.low) <= tie(rhs.blocks, rhs.low);
  }
  void add_pairwise_arc(int from, int to, Cost f00, Cost f01, Cost f10, Cost f11)
  {
    flow_internal::SignedCapacitySum<Cost> slack;
    slack.add(f01), slack.add(f10), slack.subtract(f00), slack.subtract(f11);
    slack.magnitude().for_each_chunk([&](Unsigned cap) { add_arc(from, to, cap); });
  }
  void add_finite_pairwise(int i, int j, Cost f00, Cost f01, Cost f10, Cost f11)
  {
#if defined(LOCAL) || !defined(NDEBUG)
    const bool submodular = sum_leq(f00, f11, f01, f10);
    if (!submodular) dump(i, j, f00, f01, f10, f11);
    assert(submodular && "submodularity requires f00 + f11 <= f01 + f10");
#endif
    if (f00 == f11 && f00 <= f01 && f00 <= f10)
    {
      add_constant(f00);
      add_arc(i, j, Unsigned(f10) - Unsigned(f00));
      add_arc(j, i, Unsigned(f01) - Unsigned(f00));
      return;
    }
    add_unary(i, f00, f10);
    add_unary_difference(j, f10, f11);
    add_pairwise_arc(j, i, f00, f01, f10, f11);
  }
  template <class I>
  void add_all(const vc<I> &vertices, bool value, Cost f_all, Cost f_other)
  {
    static_assert(is_integral_ext<I>);
    assert(f_all <= f_other);
    for ([[maybe_unused]] I v : vertices) assert(u128(v) < unsigned(n));
    if (vertices.empty() || f_all == f_other) { add_constant(f_all); return; }
    const int extra = nodes++;
    add_unary(extra, value ? f_other : f_all, value ? f_all : f_other);
    for (int v : vertices)
      hard.emplace_back(value ? extra : v, value ? v : extra);
  }
  optional<pair<Cost, vc<bool>>> solve_cut() const
  {
    MaxFlow<Unsigned> flow(nodes + 2);
    auto vertex = [&](int v) { return v == source ? nodes : v == sink ? nodes + 1 : v; };
    for (const auto &e : finite) flow.add_edge(vertex(e.from), vertex(e.to), e.cap);
    auto infinity = bound;
    infinity.add(1);
    for (const auto &e : hard)
    {
      const int from = vertex(e.first), to = vertex(e.second);
      infinity.for_each_chunk([&](Unsigned cap) { flow.add_edge(from, to, cap); });
    }
    Unsigned total = offset;
    const Unsigned unlimited = numeric_limits<Unsigned>::max();
    while (true)
    {
      const Unsigned f = flow.flow(nodes, nodes + 1);
      total += f;
      if (f != unlimited) break;
    }
    auto cut = flow.min_cut(nodes);
    for (auto [u, v] : hard) if (cut[vertex(u)] && !cut[vertex(v)]) return nullopt;
    return pair{Cost(total), move(cut)};
  }

public:
  CPP_DUMP_DEFINE_DATA(n, nodes, finite, hard, offset);

  ProjectSelectionProblem() = default;
  explicit ProjectSelectionProblem(int n) : n(n), nodes(n) { assert(n >= 0); }
  template <class I = ll>
  I size() const { return n; }

  void add_constant(Cost value) { offset += Unsigned(value); }

  // 1 変数関数 f(x[i]) を加算、nullopt は +∞ 相当
  void add1(int i, optional<Cost> f0, optional<Cost> f1)
  {
    check_vertex(i);
    if (f0 && f1) { add_unary(i, *f0, *f1); return; }
    if (!f0 && !f1) { add_hard_arc(source, sink); return; }
    if (f0) add_hard_arc(i, sink);
    else add_hard_arc(source, i);
    add_constant(f0 ? *f0 : *f1);
  }

  // 1 変数関数 f(x[i]) を加算、nullopt は +∞ 相当
  template <class F>
  void add1(int i, const F &f)
  {
    check_vertex(i);
    add1(i, f(0), f(1));
  }

  // 2 変数関数 f(x[i], x[j]) を加算、nullopt は +∞ 相当、i != j を想定
  // 劣モジュラ性: f00 + f11 <= f01 + f10
  void add2(int i, int j, optional<Cost> f00, optional<Cost> f01,
            optional<Cost> f10, optional<Cost> f11)
  {
    check_vertex(i), check_vertex(j);
    if (i == j) { add1(i, f00, f11); return; }
    if (f00 && f01 && f10 && f11) { add_finite_pairwise(i, j, *f00, *f01, *f10, *f11); return; }
#if defined(LOCAL) || !defined(NDEBUG)
    const bool closed = !f01 || !f10;
    if (!closed) dump(i, j, f00, f01, f10, f11);
    assert(closed && "allowing (0,1) and (1,0) requires allowing (0,0) and (1,1)");
#endif
    const auto &off_diagonal = f10 ? f10 : f01;
    if (f10) swap(i, j);
    if (!f00 && !off_diagonal && !f11) { add_hard_arc(source, sink); return; }
    add_hard_arc(i, j);
    if (!f00) add_hard_arc(source, j);
    if (!off_diagonal) add_hard_arc(j, i);
    if (!f11) add_hard_arc(i, sink);
    const Cost middle = off_diagonal.value_or(f00.value_or(f11.value_or(Cost(0))));
    add_unary_difference(i, middle, f11.value_or(middle));
    add_unary(j, f00.value_or(middle), middle);
  }

  // 2 変数関数 f(x[i], x[j]) を加算、nullopt は +∞ 相当、i != j を想定
  // 劣モジュラ性: f00 + f11 <= f01 + f10
  template <class F>
  void add2(int i, int j, const F &f)
  {
    check_vertex(i), check_vertex(j);
    add2(i, j, f(0, 0), f(0, 1), f(1, 0), f(1, 1));
  }

  // 指定変数がすべて 0 なら f_all0、それ以外なら f_exists1 を加算
  // 劣モジュラ性: f_all0 <= f_exists1
  template <class I = int>
  void add_all0(const vc<I> &vertices, Cost f_all0, Cost f_exists1)
  { add_all(vertices, false, f_all0, f_exists1); }

  // 指定変数がすべて 1 なら f_all1、それ以外なら f_exists0 を加算
  // 劣モジュラ性: f_all1 <= f_exists0
  template <class I = int>
  void add_all1(const vc<I> &vertices, Cost f_all1, Cost f_exists0)
  { add_all(vertices, true, f_all1, f_exists0); }

  // 関数の最小値とそれを達成する割り当てを返す、nullopt は +∞ 相当
  optional<pair<Cost, vc<int>>> solve() const
  {
    const auto solution = solve_cut();
    if (!solution) return nullopt;
    const auto &[cost, cut] = *solution;
    return pair{cost, vc<int>(cut.begin(), cut.begin() + n)};
  }
};

template <class Cost, Cost = Cost(0)>
class K_ProjectSelectionProblem
{
  using Binary = ProjectSelectionProblem<Cost>;
  vc<int> start{0};
  Binary problem;

  int labels(int i) const
  {
    assert(0 <= i && i < int(start.size()) - 1);
    return start[i + 1] - start[i] + 1;
  }
  int threshold_vertex(int i, int a) const
  {
    const int k = labels(i);
    assert(0 <= a && a <= k);
    if (a == 0) return Binary::source;
    if (a == k) return Binary::sink;
    return start[i] + a - 1;
  }
  void forbid_label(int i, int value)
  {
    problem.add_hard_arc(threshold_vertex(i, value), threshold_vertex(i, value + 1));
  }
  template <class Rows>
  static void check_table([[maybe_unused]] int ki, [[maybe_unused]] int kj, const Rows &f)
  {
    assert(f.size() == size_t(ki));
    for ([[maybe_unused]] const auto &row : f) assert(row.size() == size_t(kj));
  }

  template <class F>
  void add_finite_pairwise(int i, int j, int ki, int kj, const F &f)
  {
    vc<Cost> row(kj);
    for (int y = 0; y < kj; y++) row[y] = f(0, y);
    problem.add_constant(row[0]);
    for (int x = 1; x < ki; x++)
    {
      Cost top_left = row[0], bottom_left = f(x, 0);
      const int row_vertex = start[i] + x - 1;
      problem.add_unary_difference(row_vertex, top_left, bottom_left);
      row[0] = bottom_left;
      for (int y = 1; y < kj; y++)
      {
        const Cost top_right = row[y], bottom_right = f(x, y);
#if defined(LOCAL) || !defined(NDEBUG)
        const bool monge = Binary::sum_leq(top_left, bottom_right, top_right, bottom_left);
        if (!monge) dump(i, j, x - 1, x, y - 1, y, top_left, top_right, bottom_left, bottom_right);
        assert(monge && "Monge requires f(a,b) + f(a+1,b+1) <= f(a,b+1) + f(a+1,b)");
#endif
        problem.add_pairwise_arc(start[j] + y - 1, row_vertex, top_left, top_right, bottom_left, bottom_right);
        row[y] = bottom_right;
        top_left = top_right, bottom_left = bottom_right;
      }
    }
    for (int y = 1; y < kj; y++)
      problem.add_unary_difference(start[j] + y - 1, row[y - 1], row[y]);
  }

  void add_partial_pairwise(int i, int j, const vvc<optional<Cost>> &f)
  {
    const int ki = f.size(), kj = f.front().size();
    vc<int> rows, columns;
    vc<bool> used_column(kj);
    for (int x = 0; x < ki; x++)
    {
      bool used_row = false;
      for (int y = 0; y < kj; y++) if (f[x][y]) used_row = used_column[y] = true;
      if (used_row) rows.emplace_back(x);
      else forbid_label(i, x);
    }
    if (rows.empty()) return;
    for (int y = 0; y < kj; y++)
      if (used_column[y]) columns.emplace_back(y);
      else forbid_label(j, y);

    const int row_count = rows.size(), column_count = columns.size();
    vc<int> left(row_count), right(row_count);
    for (int r = 0; r < row_count; r++)
    {
      const int x = rows[r];
      int lo = 0, hi = column_count - 1;
      while (!f[x][columns[lo]]) lo++;
      while (!f[x][columns[hi]]) hi--;
#if defined(LOCAL) || !defined(NDEBUG)
      if (r > 0)
      {
        const bool ordered = left[r - 1] <= lo && right[r - 1] <= hi;
        if (!ordered) dump(i, j, rows[r - 1], x, columns[left[r - 1]], columns[right[r - 1]], columns[lo], columns[hi]);
        assert(ordered && "Monge requires nondecreasing endpoints of the allowed intervals");
      }
      for (int c = lo + 1; c < hi; c++)
      {
        const auto &value = f[x][columns[c]];
        if (!value) dump(i, j, x, columns[lo], columns[c], columns[hi], f[x][columns[lo]], value, f[x][columns[hi]]);
        assert(value.has_value() && "Monge requires the allowed columns in each row to form an interval");
      }
#endif
      left[r] = lo, right[r] = hi;
      problem.add_hard_arc(threshold_vertex(i, x), threshold_vertex(j, columns[lo]));
      problem.add_hard_arc(threshold_vertex(j, columns[hi] + 1), threshold_vertex(i, x + 1));
    }

    int first_row = 0;
    Cost previous = *f[rows[0]][columns[0]];
    problem.add_constant(previous);
    for (int c = 1; c < column_count; c++)
    {
      while (right[first_row] < c) first_row++;
      const Cost value = *f[rows[first_row]][columns[c]];
      problem.add_unary_difference(threshold_vertex(j, columns[c]), previous, value);
      previous = value;
    }

    for (int r = 1; r < row_count; r++)
    {
      const int lo = left[r], hi = right[r - 1];
      if (lo > hi) continue;
      const int x0 = rows[r - 1], x1 = rows[r];
      const int row_vertex = threshold_vertex(i, x1);
      const Cost before = *f[x0][columns[hi]], after = *f[x1][columns[hi]];
      problem.add_unary_difference(row_vertex, before, after);
      if (hi + 1 < column_count)
        problem.add_unary_difference(threshold_vertex(j, columns[hi + 1]), after, before);
      for (int c = lo + 1; c <= hi; c++)
      {
        const int y0 = columns[c - 1], y1 = columns[c];
        const Cost f00 = *f[x0][y0], f01 = *f[x0][y1], f10 = *f[x1][y0], f11 = *f[x1][y1];
#if defined(LOCAL) || !defined(NDEBUG)
        const bool monge = Binary::sum_leq(f00, f11, f01, f10);
        if (!monge) dump(i, j, x0, x1, y0, y1, f00, f01, f10, f11);
        assert(monge && "Monge requires f(a,b) + f(a',b') <= f(a,b') + f(a',b)");
#endif
        problem.add_pairwise_arc(row_vertex, threshold_vertex(j, y1), f00, f01, f10, f11);
      }
    }
  }

  template <class F>
  void init(int n, const F &count)
  {
    assert(n >= 0);
    start.resize(n + 1);
    for (int i = 0; i < n; i++)
    {
      const auto k = count(i);
      static_assert(is_integral_ext<remove_cv_t<decltype(k)>>);
      assert(k > 0);
      start[i + 1] = start[i] + int(k - 1);
    }
    problem = Binary(start.back());
    for (int i = 0; i < n; i++)
      for (int v = start[i] + 1; v < start[i + 1]; v++) problem.add_hard_arc(v, v - 1);
  }

public:
  CPP_DUMP_DEFINE_DATA(start, problem);

  K_ProjectSelectionProblem() = default;

  // 変数 i の値域を [0, ks[i]) とする
  template <class I = int>
  K_ProjectSelectionProblem(int n, const vc<I> &ks)
  {
    assert(ks.size() == size_t(n));
    init(n, [&](int i) { return ks[i]; });
  }

  // 変数 i の値域を [0, count(i)) とする
  template <class F, enable_if_t<is_invocable_v<const F &, int>, int> = 0>
  K_ProjectSelectionProblem(int n, const F &count) { init(n, count); }

  // 1 変数関数 f(x[i]) を加算、nullopt は +∞ 相当
  template <class T = optional<Cost>>
  void add1(int i, const vc<T> &f)
  {
    assert(f.size() == size_t(labels(i)));
    add1(i, [&](int x) { return f[x]; });
  }

  // 1 変数関数 f(x[i]) を加算、nullopt は +∞ 相当
  template <class T>
  void add1(int i, initializer_list<T> f)
  {
    assert(f.size() == size_t(labels(i)));
    add1(i, [&](int x) { return f.begin()[x]; });
  }

  // 1 変数関数 f(x[i]) を加算、nullopt は +∞ 相当
  template <class F>
  void add1(int i, const F &f)
  {
    const int k = labels(i);
    if constexpr (is_convertible_v<decltype(f(0)), Cost>)
    {
      Cost previous = f(0);
      problem.add_constant(previous);
      for (int x = 1; x < k; x++)
      {
        const Cost current = f(x);
        problem.add_unary_difference(start[i] + x - 1, previous, current);
        previous = current;
      }
    }
    else
    {
      optional<Cost> previous;
      for (int x = 0; x < k; x++)
      {
        const optional<Cost> current = f(x);
        if (!current) { forbid_label(i, x); continue; }
        if (previous) problem.add_unary_difference(start[i] + x - 1, *previous, *current);
        else problem.add_constant(*current);
        previous = current;
      }
    }
  }

  // 2 変数関数 f(x[i], x[j]) を加算、nullopt は +∞ 相当、i != j を想定
  // Monge 性: a < a', b < b' に対し f(a,b) + f(a',b') <= f(a,b') + f(a',b)
  template <class T = optional<Cost>>
  void add2(int i, int j, const vvc<T> &f)
  {
    const int ki = labels(i), kj = labels(j);
    check_table(ki, kj, f);
    if constexpr (is_same_v<T, optional<Cost>>)
    {
      if (i == j) { add1(i, [&](int x) { return f[x][x]; }); return; }
      if (ki == 1) { add1(j, f[0]); return; }
      if (kj == 1) { add1(i, [&](int x) { return f[x][0]; }); return; }
      add_partial_pairwise(i, j, f);
    }
    else add2(i, j, [&](int x, int y) { return f[x][y]; });
  }

  // 2 変数関数 f(x[i], x[j]) を加算、nullopt は +∞ 相当、i != j を想定
  // Monge 性: a < a', b < b' に対し f(a,b) + f(a',b') <= f(a,b') + f(a',b)
  template <class T = Cost>
  void add2(int i, int j, initializer_list<vc<T>> f)
  {
    check_table(labels(i), labels(j), f);
    add2(i, j, [&](int x, int y) { return f.begin()[x][y]; });
  }

  // 2 変数関数 f(x[i], x[j]) を加算、nullopt は +∞ 相当、i != j を想定
  // Monge 性: a < a', b < b' に対し f(a,b) + f(a',b') <= f(a,b') + f(a',b)
  template <class F>
  void add2(int i, int j, const F &f)
  {
    const int ki = labels(i), kj = labels(j);
    if (i == j) { add1(i, [&](int x) { return f(x, x); }); return; }
    if (ki == 1) { add1(j, [&](int y) { return f(0, y); }); return; }
    if (kj == 1) { add1(i, [&](int x) { return f(x, 0); }); return; }

    if constexpr (is_convertible_v<decltype(f(0, 0)), Cost>) add_finite_pairwise(i, j, ki, kj, f);
    else
    {
      vvc<optional<Cost>> table(ki, vc<optional<Cost>>(kj));
      for (int x = 0; x < ki; x++)
        for (int y = 0; y < kj; y++) table[x][y] = f(x, y);
      add_partial_pairwise(i, j, table);
    }
  }

  // 関数の最小値とそれを達成する割り当てを返す、nullopt は +∞ 相当
  optional<pair<Cost, vc<int>>> solve() const
  {
    const auto solution = problem.solve_cut();
    if (!solution) return nullopt;
    const auto &[cost, bits] = *solution;
    vc<int> result(start.size() - 1);
    for (int i = 0; i < int(result.size()); i++)
    {
      const auto first = bits.begin() + start[i], last = bits.begin() + start[i + 1];
      result[i] = partition_point(first, last, [](bool bit) { return bit; }) - first;
    }
    return pair{cost, move(result)};
  }
};
