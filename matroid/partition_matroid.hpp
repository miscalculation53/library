#pragma once

#include "../ds/csr.hpp"
#include "linear_matroid.hpp"
#include "../algebra/from_integer.hpp"

/** 行数は全グループの min(容量, 要素数) の総和。列数は group.size()。
 *  各列の非零数は所属グループの min(容量, 要素数) 以下。 */
template <class F = FieldAddSubMulDiv<modint998244353>, class G = int, class C = int, bool Sparse = false>
auto partition_linear_representation(const vc<G> &group, const vc<C> &capacity,
                                                 const vc<typename F::S> &points)
{
  using S = typename F::S;
  const int n = group.size(), k = capacity.size();
  assert(int(points.size()) == n);
  vc<int> count(k), offset(k + 1);
  for (auto g : group)
  {
    assert(u128(g) < unsigned(k));
    count[g]++;
  }
  for (int g = 0; g < k; g++)
  {
    if constexpr (is_signed_ext<C>) assert(capacity[g] >= 0);
    offset[g + 1] = offset[g] + int(min<common_type_t<C, int>>(capacity[g], count[g]));
  }
  const int dimension = offset[k];
  internal::LinearRepresentationBuilder<F, Sparse> columns(dimension, n);
  for (int e = 0; e < n; e++)
  {
    const int g = group[e];
    S power = F::e1();
    for (int i = offset[g]; i < offset[g + 1]; i++)
      columns.set(e, i, power), power = F::mul(power, points[e]);
  }
  return std::move(columns).finish();
}

/** 行数は全グループの min(容量, 要素数) の総和。列数は group.size()。
 *  各列の非零数は所属グループの min(容量, 要素数) 以下。 */
template <class F = FieldAddSubMulDiv<modint998244353>, class G = int, class C = int, bool Sparse = false>
auto partition_linear_representation(const vc<G> &group, const vc<C> &capacity)
{
  vc<int> count(capacity.size());
  auto points = gen_vec(group.size(), [&](int e)
  {
    assert(u128(group[e]) < capacity.size());
    const int g = int(group[e]), i = count[g]++;
    auto point = from_integer<F>(i);
    assert((common_type_t<C, int>(capacity[g]) < 2 || i == 0 || point != F::e0()));
    return point;
  });
  return partition_linear_representation<F, G, C, Sparse>(group, capacity, points);
}

/**
 * @brief 分割マトロイド
 * @docs docs/matroid/partition_matroid.md
 */

// group[i]: 要素 i がどのグループに入るか
// capacity[j]: グループ j から選ばれる個数の上限
class PartitionMatroid
{
  vc<int> group, capacity, count;
  CSR<int> members;
  vc<bool> selected;

public:
  PartitionMatroid(vc<int> group, vc<int> capacity)
      : group(move(group)), capacity(move(capacity)),
        selected(this->group.size())
  {
    for (int c : this->capacity) assert(c >= 0);
    vc<int> row_sizes(this->capacity.size());
    for (int g : this->group)
    {
      assert(0 <= g && g < int(this->capacity.size()));
      row_sizes[g]++;
    }
    members = CSR<int>(row_sizes);
    count.resize(this->capacity.size());
    build({});
  }

  template <class G = int, class C = int>
  PartitionMatroid(const vc<G> &group, const vc<C> &capacity)
      : PartitionMatroid(gen_vec(group.size(), [&](int e)
        {
          assert(u128(group[e]) < capacity.size());
          return int(group[e]);
        }), gen_vec(capacity.size(), [&](int g)
        {
          if constexpr (is_signed_ext<C>) assert(capacity[g] >= 0);
          return u128(capacity[g]) < group.size() ? int(capacity[g]) : int(group.size());
        })) {}

  CPP_DUMP_DEFINE_DATA(group, capacity, selected);

  int size() const { return group.size(); }

  /** 行数は全グループの min(容量, 要素数) の総和。列数は台集合の要素数。
   *  各列の非零数は所属グループの min(容量, 要素数) 以下。 */
  template <class F = FieldAddSubMulDiv<modint998244353>>
  LinearMatroid<F> linear_representation(const vc<typename F::S> &points) const
  {
    return partition_linear_representation<F>(group, capacity, points);
  }

  /** 行数は全グループの min(容量, 要素数) の総和。列数は台集合の要素数。
   *  各列の非零数は所属グループの min(容量, 要素数) 以下。 */
  template <class F = FieldAddSubMulDiv<modint998244353>>
  LinearMatroid<F> linear_representation() const
  {
    return partition_linear_representation<F>(group, capacity);
  }

  /** 行数は全グループの min(容量, 要素数) の総和。列数は台集合の要素数。
   *  各列の非零数は所属グループの min(容量, 要素数) 以下。 */
  template <class F = FieldAddSubMulDiv<modint998244353>>
  SparseMatrix<F> sparse_linear_representation(const vc<typename F::S> &points) const
  {
    return partition_linear_representation<F, int, int, true>(group, capacity, points);
  }

  /** 行数は全グループの min(容量, 要素数) の総和。列数は台集合の要素数。
   *  各列の非零数は所属グループの min(容量, 要素数) 以下。 */
  template <class F = FieldAddSubMulDiv<modint998244353>>
  SparseMatrix<F> sparse_linear_representation() const
  {
    return partition_linear_representation<F, int, int, true>(group, capacity);
  }

  template <class I = int>
  void build(const vc<I> &s)
  {
    fill(ALL(selected), false);
    fill(ALL(count), 0);
    for (auto value : s)
    {
      assert(u128(value) < unsigned(size()));
      const int e = int(value);
      assert(!selected[e]);
      selected[e] = true;
      const int g = group[e];
      members[g][count[g]++] = e;
      assert(count[g] <= capacity[g]);
    }
  }

  optional<vc<int>> circuit(int e) const
  {
    assert(0 <= e && e < size() && !selected[e]);
    const int g = group[e];
    if (count[g] < capacity[g]) return nullopt;
    auto row = members[g];
    return vc<int>(row.begin(), row.begin() + count[g]);
  }
};
