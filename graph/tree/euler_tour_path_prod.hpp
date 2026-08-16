#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 木のオイラーツアーでパスの総積クエリを処理
 * @docs docs/graph/tree/euler_tour_path_prod.md
 */

#include "euler_tour.hpp"
#include "ds/segtree/segtree.hpp"
#include "algebra/algebra_base.hpp"

// G は群
// 根からのパスだけなら LCA いらない
template <
  class G,
  bool need_lca = true,
  class DS1 = SegmentTree<G>,
  class DS2 = SegmentTree<OppositeGroup<G>>,
  class RMQ = BlockSparseTable<MonoidMin<ull>, 64>
>
struct PathProd : EulerTour<need_lca, RMQ>
{
  using S = typename G::S;

private:
  DS1 seg1;
  DS2 seg2;
  template <class T>
  void build(const vc<T> &vec)
  {
    if (vec.empty())
    {
      seg1 = DS1(2 * this->n), seg2 = DS2(2 * this->n);
      return;
    }
    assert(SZ(vec) == this->n);
    vc<S> seg_init(2 * this->n, G::e());
    repi(i, this->n)
    {
      seg_init[this->in[i]] = vec[i];
      seg_init[this->out[i] + 1] = G::inv(vec[i]);
    }
    seg1 = DS1(seg_init), seg2 = DS2(seg_init);
  }

public:
  PathProd() {}
  template <class I, class T>
  PathProd(int n, const vc<I> &par, const vc<T> &vec = {})
    : EulerTour<need_lca, RMQ>(n, par)
  {
    build(vec);
  }
  template <class P, class T>
  PathProd(int n, const vc<P> &es, int rt, const vc<T> &vec = {})
    : EulerTour<need_lca, RMQ>(n, es, rt)
  {
    build(vec);
  }

  S get(int v) const
  {
    assert(0 <= v && v < this->n);
    return seg1.get(this->in[v]);
  }
  void set(int v, const S &x)
  {
    assert(0 <= v && v < this->n);
    S ix = G::inv(x);
    seg1.set(this->in[v], x), seg1.set(this->out[v] + 1, ix);
    seg2.set(this->in[v], x), seg2.set(this->out[v] + 1, ix);
  }
  // 根から
  S prod(int v) const
  {
    assert(0 <= v && v < this->n);
    return seg1.prod(0, this->in[v] + 1);
  }
  S prod(int u, int v) const
  {
    assert(0 <= u && u < this->n);
    assert(0 <= v && v < this->n);
    int w = this->lca(u, v);
    S up = seg2.prod(this->in[w] + 1, this->in[u] + 1);
    S down = seg1.prod(this->in[w], this->in[v] + 1);
    return G::op(up, down);
  }
};
