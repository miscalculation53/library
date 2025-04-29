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
    S val;
    array<int, 2> chi;
    Node() {}
    Node(S val) : val(val), chi{-1, -1} {}
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
      nodes[i].chi[dir] = nodes.size();
      nodes.eb(M::e());
    }
    return nodes[i].chi[dir];
  }

public:
  SparseSegmentTree() {}
  SparseSegmentTree(ll n, int reserve = 1 << 24) : n(n), nodes(1, M::e()) { nodes.reserve(reserve); }

  void set(ll p, const S &x)
  {
    assert(0 <= p && p < n);
    auto dfs = [&](auto dfs, ll a, ll b, int i) -> void
    {
      if (b - a == 1)
      {
        nodes[i].val = x;
        return;
      }
      ll c = (a + b) / 2;
      if (p < c)
        dfs(dfs, a, c, visit_or_make(i, 0));
      else
        dfs(dfs, c, b, visit_or_make(i, 1));
      update(i);
    };
    dfs(dfs, 0, n, 0);
  }

  S get(ll p) const
  {
    assert(0 <= p && p < n);
    ll a = 0, b = n;
    int i = 0;
    while (b - a > 1 && i >= 0)
    {
      ll c = (a + b) / 2;
      if (p < c)
        i = nodes[i].chi[0], b = c;
      else
        i = nodes[i].chi[1], a = c;
    }
    if (i == -1)
      return M::e();
    return nodes[i].val;
  }

  S prod(ll l, ll r) const
  {
    assert(0 <= l && l <= r && r <= n);
    auto dfs = [&](auto dfs, ll a, ll b, int i) -> S
    {
      if (i < 0)
        return M::e();
      if (b <= l || r <= a)
        return M::e();
      if (l <= a && b <= r)
        return nodes[i].val;
      if (b - a == 1)
        return M::e();
      ll c = (a + b) / 2;
      return M::op(dfs(dfs, a, c, nodes[i].chi[0]), dfs(dfs, c, b, nodes[i].chi[1]));
    };
    return dfs(dfs, 0, n, 0);
  }

  S all_prod() const { return nodes[0].val; }

  map<ll, S> content() const
  {
    map<ll, S> res;
    auto dfs = [&](auto dfs, ll a, ll b, int i) -> void
    {
      if (i < 0)
        return;
      if (b - a == 1)
      {
        res[a] = nodes[i].val;
        return;
      }
      ll c = (a + b) / 2;
      dfs(dfs, a, c, nodes[i].chi[0]), dfs(dfs, c, b, nodes[i].chi[1]);
    };
    dfs(dfs, 0, n, 0);
    return res;
  }
};
