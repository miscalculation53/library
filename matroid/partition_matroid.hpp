#pragma once

#include "../ds/csr.hpp"
#include "linear_matroid.hpp"

template <class F>
LinearMatroid<F> partition_linear_representation(const vc<int> &group, const vc<int> &capacity,
                                                 const vc<typename F::S> &points)
{
  using S = typename F::S;
  const int n = group.size(), k = capacity.size();
  assert(int(points.size()) == n);
  vc<int> count(k), offset(k + 1);
  for (int g : group)
  {
    assert(0 <= g && g < k);
    count[g]++;
  }
  for (int g = 0; g < k; g++)
  {
    assert(capacity[g] >= 0);
    offset[g + 1] = offset[g] + min(capacity[g], count[g]);
  }
  const int dimension = offset[k];
  vvc<S> columns(n, vc<S>(dimension, F::e0()));
  for (int e = 0; e < n; e++)
  {
    const int g = group[e];
    S power = F::e1();
    for (int i = offset[g]; i < offset[g + 1]; i++)
      columns[e][i] = power, power = F::mul(power, points[e]);
  }
  return LinearMatroid<F>(dimension, columns);
}

/**
 * @brief 分割マトロイド
 * @docs docs/matroid/partition_matroid.md
 */
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

  int size() const { return group.size(); }

  template <class F>
  LinearMatroid<F> linear_representation(const vc<typename F::S> &points) const
  {
    return partition_linear_representation<F>(group, capacity, points);
  }

  void build(const vc<int> &s)
  {
    fill(ALL(selected), false);
    fill(ALL(count), 0);
    for (int e : s)
    {
      assert(0 <= e && e < size() && !selected[e]);
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
