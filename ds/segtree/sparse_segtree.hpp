#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 必要なところだけ作るセグメント木
 * @docs docs/ds/segtree/sparse_segtree.md
 */

template <class M>
struct SparseSegmentTree
{
  using S = typename M::S;

private:
  ll n;
  struct Node
  {
    ll l, r;
    S val;
    int par;
    array<int, 2> chi;
    Node() {}
    Node(ll l, ll r, S val, int par) : l(l), r(r), val(val), par(par), chi{-1, -1} {}
  };
  vc<Node> nodes;
  S chi_val(int i, int dir)
  {
    Node &node = nodes[i];
    if (node.chi[dir] == -1)
      return M::e();
    return nodes[node.chi[dir]].val;
  }
  void update(int i) { nodes[i].val = M::op(chi_val(i, 0), chi_val(i, 1)); }
  int visit_or_make(int i, int dir)
  {
    if (nodes[i].chi[dir] == -1)
    {
      const ll l = nodes[i].l, r = nodes[i].r;
      const ll m = (l + r) / 2;
      const ll nl = dir == 0 ? l : m;
      const ll nr = dir == 0 ? m : r;
      nodes[i].chi[dir] = nodes.size();
      nodes.eb(nl, nr, M::e(), i);
    }
    return nodes[i].chi[dir];
  }

  S prod_internal(ll l, ll r, int i) const
  {
    if (i < 0)
      return M::e();
    const Node &node = nodes[i];
    if (node.r <= l || r <= node.l)
      return M::e();
    if (l <= node.l && node.r <= r)
      return node.val;
    if (node.r - node.l <= 1)
      return M::e();
    return M::op(prod_internal(l, r, nodes[i].chi[0]), prod_internal(l, r, nodes[i].chi[1]));
  }

public:
  SparseSegmentTree() {}
  SparseSegmentTree(ll n, int reserve = 1 << 24) : n(n), nodes(1, {0, n, M::e(), -1}) { nodes.reserve(reserve); }

  void set(ll p, const S &x)
  {
    assert(0 <= p && p < n);
    ll l = 0, r = n;
    int i = 0;
    while (r - l > 1)
    {
      ll m = (l + r) / 2;
      if (p < m)
        i = visit_or_make(i, 0), r = m;
      else
        i = visit_or_make(i, 1), l = m;
    }
    nodes[i].val = x;
    while (i > 0)
      i = nodes[i].par, update(i);
  }

  S get(ll p) const
  {
    assert(0 <= p && p < n);
    ll l = 0, r = n;
    int i = 0;
    while (r - l > 1 && i >= 0)
    {
      ll m = (l + r) / 2;
      if (p < m)
        i = nodes[i].chi[0], r = m;
      else
        i = nodes[i].chi[1], l = m;
    }
    if (i == -1)
      return M::e();
    return nodes[i].val;
  }

  S prod(ll l, ll r) const
  {
    assert(0 <= l && l <= r && r <= n);
    return prod_internal(l, r, 0);
  }

  S all_prod() const { return nodes[0].val; }

  map<ll, S> content() const
  {
    map<ll, S> res;
    fec(node : nodes)
    {
      if (node.r - node.l == 1)
        res[node.l] = node.val;
    }
    return res;
  }
};
