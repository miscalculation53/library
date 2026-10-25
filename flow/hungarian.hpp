#pragma once

#include "../utils/make_unsigned_ext.hpp"

/*!
 * The assignment core is adapted from gatagat/lap, _lapjv_cpp/lapjv.cpp.
 * https://github.com/gatagat/lap/blob/e29341d9e0ec8c3a1f11f239eb65a31973f4a0ab/_lapjv_cpp/lapjv.cpp
 * Changes: integer arithmetic, bounded row-reduction work, rectangular inputs,
 * reusable search buffers, dual solutions, and a Monge shortcut.
 *
 * BSD 2-Clause License
 *
 * Copyright (c) 2012-2025, Tomas Kazmar
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @brief ハンガリアン法（最小費用割当）
 * @docs docs/flow/hungarian.md
 */

// n <= m の完全二部グラフで、各行を異なる列へ割り当てる。
// Cost は費用・距離・双対変数・総費用の型。利用者が必要な幅を指定する。
template <class Cost>
struct Hungarian
{
  Cost cost = 0;
  vc<int> row_to_col, col_to_row;
  // row_potential[i] + col_potential[j] <= a[i][j]。割当辺では等号。
  // n < m の場合、col_potential <= 0、未割当列の col_potential = 0。
  vc<Cost> row_potential, col_potential;

  template <class Matrix>
  explicit Hungarian(const Matrix &a)
  {
    const int n = a.size(), m = n ? a[0].size() : 0;
    using Input = decay_t<decltype(a[0][0])>;
    vc<const Input *> rows(n);
    for (int i = 0; i < n; i++)
    {
      assert(int(a[i].size()) == m);
      rows[i] = data(a[i]);
    }
    solve(n, m, rows);
  }

  // 行優先の配列。stride は隣の行までの要素数（省略時 m）。
  template <class Input>
  Hungarian(int n, int m, const Input *a, int stride = -1)
  {
    assert(n >= 0 && m >= 0);
    if (stride == -1) stride = m;
    assert(stride >= m);
    vc<const Input *> rows(n);
    for (int i = 0; i < n; i++) rows[i] = a + size_t(i) * stride;
    solve(n, m, rows);
  }

private:
  static_assert(numeric_limits<Cost>::is_integer && numeric_limits<Cost>::is_signed,
                "Cost must be a signed integer type");
  using U = make_unsigned_ext_t<Cost>;

  // 相殺を含む加減算は同じ幅の符号なし型で計算する。
  static Cost difference(Cost x, Cost y) { return Cost(U(x) - U(y)); }

  // 2項の和を、符号付き型の範囲外でも正確に比較する。
  static int sum_order(Cost a, Cost b, Cost c, Cost d)
  {
    constexpr U sign = U(1) << (numeric_limits<U>::digits - 1);
    const U x = U(a) ^ sign, y = U(b) ^ sign, z = U(c) ^ sign, w = U(d) ^ sign;
    const U left = x + y, right = z + w;
    const bool left_carry = left < x, right_carry = right < z;
    if (left_carry != right_carry) return left_carry ? 1 : -1;
    return int(left > right) - int(left < right);
  }

  template <class Input>
  static int minor_order(Input a, Input b, Input c, Input d)
  {
    if constexpr (numeric_limits<Input>::is_integer && numeric_limits<Input>::digits < numeric_limits<Cost>::digits)
    {
      const Cost left = Cost(a) + Cost(b), right = Cost(c) + Cost(d);
      return int(left > right) - int(left < right);
    }
    else return sum_order(Cost(a), Cost(b), Cost(c), Cost(d));
  }

  template <class Input>
  bool ordered_matching(int n, int m, const vc<const Input *> &a)
  {
    if (n != m) return false;
    int directions = 3; // bit 0: Monge、bit 1: 逆 Monge。
    for (int i = 1; i < n; i++) for (int j = 1; j < n; j++)
    {
      const int order = minor_order(a[i - 1][j - 1], a[i][j], a[i - 1][j], a[i][j - 1]);
      if (order > 0) directions &= 2;
      if (order < 0) directions &= 1;
      if (!directions) return false;
    }
    const bool reverse_columns = !(directions & 1);
    for (int i = 0; i < n; i++)
    {
      const int j = reverse_columns ? n - 1 - i : i;
      row_to_col[i] = j, col_to_row[j] = i;
      if (i)
      {
        const int previous = row_to_col[i - 1];
        col_potential[j] = Cost(U(col_potential[previous]) + U(Cost(a[i - 1][j]))
                                - U(Cost(a[i - 1][previous])));
      }
    }
    return true;
  }

  template <class Input>
  vc<int> initialize(int n, int m, const vc<const Input *> &a)
  {
    vc<int> free_rows;
    free_rows.reserve(n);
    if (n < m)
    {
      // 列ポテンシャルを 0 に保ち、行最小値で競合しない割当を作る。
      using ScanCost = conditional_t<numeric_limits<Input>::is_integer, Input, Cost>;
      for (int i = 0; i < n; i++)
      {
        ScanCost minimum = ScanCost(a[i][0]);
        for (int j = 1; j < m; j++) minimum = min(minimum, ScanCost(a[i][j]));
        int column = -1;
        for (int j = 0; j < m; j++) if (col_to_row[j] == -1 && Cost(a[i][j]) == minimum)
        { column = j; break; }
        if (column != -1) row_to_col[i] = column, col_to_row[column] = i;
        else free_rows.emplace_back(i);
      }
      return free_rows;
    }

    // lap の column reduction / reduction transfer。行優先で列最小値を得る。
    for (int j = 0; j < m; j++) col_potential[j] = Cost(a[0][j]), col_to_row[j] = 0;
    for (int i = 1; i < n; i++) for (int j = 0; j < m; j++)
      if (Cost(a[i][j]) < col_potential[j]) col_potential[j] = Cost(a[i][j]), col_to_row[j] = i;
    vc<unsigned char> unique(n, 1);
    for (int j = m; j-- > 0;)
    {
      const int i = col_to_row[j];
      if (row_to_col[i] == -1) row_to_col[i] = j;
      else unique[i] = 0, col_to_row[j] = -1;
    }
    for (int i = 0; i < n; i++)
    {
      const int assigned = row_to_col[i];
      if (assigned == -1) free_rows.emplace_back(i);
      else if (unique[i])
      {
        Cost slack = numeric_limits<Cost>::max();
        for (int j = 0; j < m; j++) if (j != assigned)
          slack = min(slack, difference(Cost(a[i][j]), col_potential[j]));
        col_potential[assigned] = difference(col_potential[assigned], slack);
      }
    }

    // lap の augmenting row reduction。追い出した行をすぐ再処理する。
    // 各巡回を n 回の行走査で打ち切り、初期化を O(nm) に保つ。
    vc<int> deferred;
    deferred.reserve(n);
    for (int pass = 0; pass < 2 && !free_rows.empty(); pass++)
    {
      deferred.clear();
      int head = 0, work = 0;
      while (head < int(free_rows.size()) && work++ < n)
      {
        const int i = free_rows[head++];
        int first = 0;
        Cost best = difference(Cost(a[i][0]), col_potential[0]);
        Cost runner_up = numeric_limits<Cost>::max();
        for (int j = 1; j < m; j++)
        {
          const Cost value = difference(Cost(a[i][j]), col_potential[j]);
          if (value < best)
          {
            runner_up = best, best = value, first = j;
          }
          else runner_up = min(runner_up, value);
        }
        const bool lower_price = best < runner_up;
        if (lower_price)
          col_potential[first] = Cost(U(col_potential[first]) - U(runner_up) + U(best));
        else if (col_to_row[first] != -1)
          for (int j = 0; j < m; j++)
            if (col_to_row[j] == -1 && difference(Cost(a[i][j]), col_potential[j]) == best)
            { first = j; break; }
        const int displaced = col_to_row[first];
        row_to_col[i] = first, col_to_row[first] = i;
        if (displaced != -1)
        {
          row_to_col[displaced] = -1;
          if (lower_price) free_rows[--head] = displaced;
          else deferred.emplace_back(displaced);
        }
      }
      deferred.insert(deferred.end(), free_rows.begin() + head, free_rows.end());
      free_rows.swap(deferred);
    }
    return free_rows;
  }

  template <class Input>
  void augment(int root, int m, const vc<const Input *> &a,
               vc<int> &columns, vc<int> &predecessor, vc<Cost> &distance)
  {
    for (int j = 0; j < m; j++)
      columns[j] = j, predecessor[j] = root, distance[j] = difference(Cost(a[root][j]), col_potential[j]);
    int scanned = 0, labeled = 0, sink = -1;
    Cost level = 0;
    while (sink == -1)
    {
      if (scanned == labeled)
      {
        level = distance[columns[scanned]];
        for (int k = scanned; k < m; k++)
        {
          const int j = columns[k];
          if (distance[j] > level) continue;
          if (distance[j] < level) level = distance[j], labeled = scanned, sink = -1;
          swap(columns[k], columns[labeled++]);
          if (col_to_row[j] == -1) sink = j;
        }
        if (sink != -1) break;
      }
      while (scanned < labeled && sink == -1)
      {
        const int assigned = columns[scanned++], i = col_to_row[assigned];
        const U offset = U(Cost(a[i][assigned])) - U(col_potential[assigned]) - U(level);
        for (int k = labeled; k < m; k++)
        {
          const int j = columns[k];
          const Cost candidate = Cost(U(Cost(a[i][j])) - U(col_potential[j]) - offset);
          if (candidate >= distance[j]) continue;
          distance[j] = candidate, predecessor[j] = i;
          if (candidate == level)
          {
            if (col_to_row[j] == -1) { sink = j; break; }
            swap(columns[k], columns[labeled++]);
          }
        }
      }
    }
    for (int k = 0; k < scanned; k++)
    {
      const int j = columns[k];
      col_potential[j] = Cost(U(col_potential[j]) + U(distance[j]) - U(distance[sink]));
    }
    while (sink != -1)
    {
      const int i = predecessor[sink], previous = row_to_col[i];
      row_to_col[i] = sink, col_to_row[sink] = i;
      sink = previous;
    }
  }

  template <class Input>
  void solve(int n, int m, const vc<const Input *> &a)
  {
    assert(n <= m);
    row_to_col.assign(n, -1), col_to_row.assign(m, -1);
    row_potential.assign(n, 0), col_potential.assign(m, 0);
    if (!n) return;
    if (!ordered_matching(n, m, a))
    {
      const auto free_rows = initialize(n, m, a);
      vc<int> columns(m), predecessor(m);
      vc<Cost> distance(m);
      for (int root : free_rows) augment(root, m, a, columns, predecessor, distance);
    }
    U total = 0;
    for (int i = 0; i < n; i++)
    {
      const int j = row_to_col[i];
      row_potential[i] = difference(Cost(a[i][j]), col_potential[j]);
      total += U(Cost(a[i][j]));
    }
    cost = Cost(total);
  }
};

template <class Matrix>
Hungarian(const Matrix &) -> Hungarian<decay_t<decltype(declval<Matrix>()[0][0])>>;

template <class Input>
Hungarian(int, int, const Input *, int = -1) -> Hungarian<Input>;
