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
    // p 番目の値は val
    // 左の子, p 番目, 右の子 の順に並ぶようにする、その積が prod
    ll p;
    S val, prod;
    array<int, 2> chi;
    Node() {}
    Node(ll p, S val) : p(p), val(val), prod(val), chi{-1, -1} {}
  };
  vc<Node> nodes;
  void update(int i)
  {
    int ni0 = nodes[i].chi[0], ni1 = nodes[i].chi[1];
    S sml = ni0 == -1 ? M::e() : nodes[ni0].prod;
    S smr = ni1 == -1 ? M::e() : nodes[ni1].prod;
    nodes[i].prod = M::op(M::op(sml, nodes[i].val), smr);
  }

public:
  SparseSegmentTree() {}
  SparseSegmentTree(ll n, int reserve = 1 << 20) : n(n), nodes(1, {-1, M::e()})
  {
    assert(n >= 0);
    nodes.reserve(reserve);
  }

  void set(ll p, S x)
  {
    assert(0 <= p && p < n);
    auto dfs = [&](auto dfs, ll a, ll b, int i) -> void
    {
      if (nodes[i].p == p)
      {
        nodes[i].val = x;
        update(i);
        return;
      }
      ll c = (a + b) / 2;
      if (p < c)
      {
        if (i != 0 && p > nodes[i].p)
          swap(p, nodes[i].p), swap(x, nodes[i].val);
        int &ni = nodes[i].chi[0];
        if (ni == -1)
        {
          ni = nodes.size();
          nodes.eb(p, x);
        }
        else
          dfs(dfs, a, c, ni);
      }
      else
      {
        if (i != 0 && nodes[i].p > p)
          swap(p, nodes[i].p), swap(x, nodes[i].val);
        int &ni = nodes[i].chi[1];
        if (ni == -1)
        {
          ni = nodes.size();
          nodes.eb(p, x);
        }
        else
          dfs(dfs, c, b, ni);
      }
      update(i);
    };
    dfs(dfs, 0, n, 0);
  }

  S get(ll p) const
  {
    assert(0 <= p && p < n);
    ll a = 0, b = n;
    int i = 0;
    while (i != -1)
    {
      if (nodes[i].p == p)
        return nodes[i].val;
      ll c = (a + b) / 2;
      if (p < c)
        i = nodes[i].chi[0], b = c;
      else
        i = nodes[i].chi[1], a = c;
    }
    return M::e();
  }

  S prod(ll l, ll r) const
  {
    assert(0 <= l && l <= r && r <= n);
    auto dfs = [&](auto dfs, ll a, ll b, int i) -> S
    {
      if (i == -1)
        return M::e();
      if (b <= l || r <= a)
        return M::e();
      if (l <= a && b <= r)
        return nodes[i].prod;
      ll c = (a + b) / 2;
      S sml = dfs(dfs, a, c, nodes[i].chi[0]);
      S smm = l <= nodes[i].p && nodes[i].p < r ? nodes[i].val : M::e();
      S smr = dfs(dfs, c, b, nodes[i].chi[1]);
      return M::op(M::op(sml, smm), smr);
    };
    return dfs(dfs, 0, n, 0);
  }

  S all_prod() const { return nodes[0].prod; }

  map<ll, S> content() const
  {
    map<ll, S> res;
    fec(node : nodes) if (node.p != -1) res[node.p] = node.val;
    return res;
  }
};
