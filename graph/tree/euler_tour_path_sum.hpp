#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 木のオイラーツアーでパスの総和クエリを処理
 * @docs docs/graph/tree/euler_tour_path_sum.md
 */

#include "euler_tour.hpp"
#include "ds/fenwick_tree/fenwick_tree.hpp"

// G は可換群 (可換性の要求は Fenwick Tree に由来)
// 根からのパスだけなら LCA いらない
template <
  class G,
  bool need_lca = true,
  class DS = FenwickTree<G>,
  class RMQ = BlockSparseTable<MonoidMin<ull>, 64>
>
struct PathSum : EulerTour<need_lca>
{
  using S = typename G::S;

private:
  DS fw;
  template <class T>
  void build(const vc<T> &vec)
  {
    if (vec.empty())
    {
      fw = DS(2 * this->n);
      return;
    }
    assert(SZ(vec) == this->n);
    vc<S> fw_init(2 * this->n, G::e());
    repi(i, this->n)
    {
      fw_init[this->in[i]] = vec[i];
      fw_init[this->out[i] + 1] = G::inv(vec[i]);
    }
    fw = DS(fw_init);
  }

public:
  PathSum() {}
  template <class I, class T>
  PathSum(int n, const vc<I> &par, const vc<T> &vec = {})
    : EulerTour<true>(n, par)
  {
    build(vec);
  }
  template <class P, class T>
  PathSum(int n, const vc<P> &es, int rt, const vc<T> &vec = {})
    : EulerTour<true>(n, es, rt)
  {
    build(vec);
  }

  void add(int v, const S &x)
  {
    assert(0 <= v && v < this->n);
    fw.add(this->in[v], x);
    fw.add(this->out[v] + 1, G::inv(x));
  }
  // 根から
  S sum(int v) const
  {
    assert(0 <= v && v < this->n);
    return fw.sum(this->in[v] + 1);
  }

  S get(int v) const
  {
    assert(0 <= v && v < this->n);
    if (v == this->root())
      return sum(v);
    int p = this->parent(v);
    return G::op(sum(v), G::inv(sum(p)));
  }
  void set(int v, const S &x) const
  {
    assert(0 <= v && v < this->n);
    S cur = get(v);
    S diff = G::op(x, G::inv(cur));
    add(v, diff);
  }
  S sum(int u, int v) const
  {
    assert(0 <= u && u < this->n);
    assert(0 <= v && v < this->n);
    int w = this->lca(u, v);
    S su = sum(u), sv = sum(v), isw = G::inv(sum(w));
    S res = G::op(su, sv);
    res = G::op(res, G::op(isw, isw));
    res = G::op(res, get(w));
    return res;
  }
};
