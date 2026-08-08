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
  // 長さ n、各点の初期値 M::e() の列を作る
  SparseSegmentTree(ll n) : n(n), nodes(1, {-1, M::e()})
  {
    assert(n >= 0);
  }

  // 少なくとも n ノード分の領域をあらかじめ確保する
  void reserve(int n) { nodes.reserve(n); }

  // p 番目の値を参照で受け取る関数 f を適用する。未登録なら M::e() から始める
  template <class F>
  void modify(ll p, const F &f)
  {
    assert(0 <= p && p < n);
    S x = M::e();
    bool target_is_pending = true;
    auto materialize_target = [&]()
    {
      if (target_is_pending)
      {
        f(x);
        target_is_pending = false;
      }
    };
    auto dfs = [&](auto dfs, ll a, ll b, int i) -> void
    {
      if (nodes[i].p == p)
      {
        if (target_is_pending)
        {
          f(nodes[i].val);
          target_is_pending = false;
        }
        else
          nodes[i].val = x;
        update(i);
        return;
      }
      ll c = a + (b - a) / 2;
      if (p < c)
      {
        if (i != 0 && p > nodes[i].p)
        {
          materialize_target();
          swap(p, nodes[i].p), swap(x, nodes[i].val);
        }
        int &ni = nodes[i].chi[0];
        if (ni == -1)
        {
          materialize_target();
          ni = nodes.size();
          nodes.eb(p, x);
        }
        else
          dfs(dfs, a, c, ni);
      }
      else
      {
        if (i != 0 && nodes[i].p > p)
        {
          materialize_target();
          swap(p, nodes[i].p), swap(x, nodes[i].val);
        }
        int &ni = nodes[i].chi[1];
        if (ni == -1)
        {
          materialize_target();
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

  void set(ll p, const S &x)
  {
    modify(p, [&](S &y) { y = x; });
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
      ll c = a + (b - a) / 2;
      if (p < c)
        i = nodes[i].chi[0], b = c;
      else
        i = nodes[i].chi[1], a = c;
    }
    return M::e();
  }

  // [l, r)
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
      ll c = a + (b - a) / 2;
      S sml = dfs(dfs, a, c, nodes[i].chi[0]);
      S smm = l <= nodes[i].p && nodes[i].p < r ? nodes[i].val : M::e();
      S smr = dfs(dfs, c, b, nodes[i].chi[1]);
      return M::op(M::op(sml, smm), smr);
    };
    return dfs(dfs, 0, n, 0);
  }
  S all_prod() const { return nodes[0].prod; }

  // g(prod(l, r)) が true となる最大の r を返す
  template <class G>
  ll max_right_ok(ll l, const G &g) const
  {
    assert(0 <= l && l <= n);
    assert(g(M::e()));
    S sm = M::e();
    auto append = [&](const S &x)
    {
      S nsm = M::op(sm, x);
      if (!g(nsm))
        return false;
      sm = std::move(nsm);
      return true;
    };
    auto dfs = [&](auto dfs, ll a, ll b, int i) -> ll
    {
      if (i == -1 || b <= l)
        return -1;
      if (l <= a && append(nodes[i].prod))
        return -1;
      ll c = a + (b - a) / 2;
      ll res = dfs(dfs, a, c, nodes[i].chi[0]);
      if (res != -1)
        return res;
      if (nodes[i].p >= l && !append(nodes[i].val))
        return nodes[i].p;
      return dfs(dfs, c, b, nodes[i].chi[1]);
    };
    ll res = dfs(dfs, 0, n, 0);
    return res == -1 ? n : res;
  }

  // g(prod(l, r)) が true となる最小の l を返す
  template <class G>
  ll min_left_ok(ll r, const G &g) const
  {
    assert(0 <= r && r <= n);
    assert(g(M::e()));
    S sm = M::e();
    auto prepend = [&](const S &x)
    {
      S nsm = M::op(x, sm);
      if (!g(nsm))
        return false;
      sm = std::move(nsm);
      return true;
    };
    auto dfs = [&](auto dfs, ll a, ll b, int i) -> ll
    {
      if (i == -1 || r <= a)
        return -1;
      if (b <= r && prepend(nodes[i].prod))
        return -1;
      ll c = a + (b - a) / 2;
      ll res = dfs(dfs, c, b, nodes[i].chi[1]);
      if (res != -1)
        return res;
      if (nodes[i].p != -1 && nodes[i].p < r && !prepend(nodes[i].val))
        return nodes[i].p;
      return dfs(dfs, a, c, nodes[i].chi[0]);
    };
    ll res = dfs(dfs, 0, n, 0);
    return res == -1 ? 0 : res + 1;
  }

  // 登録された添字と値を map で返す。デバッグ用。
  map<ll, S> content() const
  {
    map<ll, S> res;
    fec(node : nodes) if (node.p != -1) res[node.p] = node.val;
    return res;
  }
};
