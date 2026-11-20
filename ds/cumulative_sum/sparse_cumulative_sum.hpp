#pragma once

#include "cumulative_sum.hpp"
#include "../../algo/stable_argsort.hpp"

/**
 * @brief 疎な点集合の累積和
 * @docs docs/ds/cumulative_sum/sparse_cumulative_sum.md
 */

template <class G, class I = ll>
struct SparseCumulativeSum
{
  using S = typename G::S;

private:
  vc<I> xs;
  CumulativeSum<G> sums;

public:
  SparseCumulativeSum() : sums(vc<S>{}) {}

  // 点を座標順にまとめる。同じ座標の重みは入力順に結合する
  explicit SparseCumulativeSum(const vc<pair<I, S>> &points)
  {
    auto order = stable_argsort(points, [](const auto &point) { return point.first; });
    vc<S> weights;
    for (int i : order)
    {
      const auto &[x, w] = points[i];
      if (xs.empty() || xs.back() < x)
      {
        xs.eb(x);
        weights.eb(w);
      }
      else
        weights.back() = G::op(weights.back(), w);
    }
    sums = CumulativeSum<G>(weights);
  }

  // 座標が r 未満の重みを集約する
  S sum(const I &r) const { return sums.sum(LB<int>(xs, r)); }

  // [l, r) 内の重みを集約する
  S sum(const I &l, const I &r) const
  {
    assert(!(r < l));
    return sums.sum(LB<int>(xs, l), LB<int>(xs, r));
  }

  // 座標ごとにまとめた (座標, 重み) を座標順に返す
  vc<pair<I, S>> content() const
  {
    auto weights = sums.content();
    vc<pair<I, S>> res;
    res.reserve(xs.size());
    repi(i, xs.size()) res.eb(xs[i], std::move(weights[i]));
    return res;
  }
};
