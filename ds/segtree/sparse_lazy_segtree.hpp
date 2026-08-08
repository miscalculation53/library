#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 必要なところだけ作る遅延セグメント木
 * @docs docs/ds/segtree/sparse_lazy_segtree.md
 */

template <class AM>
struct SparseLazySegmentTree
{
  using S = typename AM::S;
  using F = typename AM::F;

private:
  ll n;
  struct Node
  {
    S prod;
    F lazy;
    array<int, 2> chi;
    bool has_lazy;
    Node(const S &prod) : prod(prod), lazy(AM::id()), chi{-1, -1}, has_lazy(false) {}
  };
  vc<Node> nodes;
  struct InitialNode
  {
    S prod;
    array<int, 2> chi;
    InitialNode(const S &prod, int l = -1, int r = -1) : prod(prod), chi{l, r} {}
  };
  vc<InitialNode> initial_nodes;
  int initial_root;

  int make_node(int z)
  {
    int i = nodes.size();
    nodes.eb(initial_nodes[z].prod);
    return i;
  }

  int ensure_child(int i, int d, int z)
  {
    int j = nodes[i].chi[d];
    if (j == -1)
    {
      j = make_node(z);
      nodes[i].chi[d] = j;
    }
    return j;
  }

  S node_prod(int i, int z) const
  {
    return i == -1 ? initial_nodes[z].prod : nodes[i].prod;
  }

  void all_apply(int i, const F &f)
  {
    nodes[i].prod = AM::mapping(f, nodes[i].prod);
    if (nodes[i].has_lazy)
      nodes[i].lazy = AM::composition(f, nodes[i].lazy);
    else
    {
      nodes[i].lazy = f;
      nodes[i].has_lazy = true;
    }
  }

  void push(int i, int z)
  {
    if (!nodes[i].has_lazy || initial_nodes[z].chi[0] == -1)
      return;
    F f = nodes[i].lazy;
    int l = ensure_child(i, 0, initial_nodes[z].chi[0]);
    int r = ensure_child(i, 1, initial_nodes[z].chi[1]);
    all_apply(l, f), all_apply(r, f);
    nodes[i].lazy = AM::id();
    nodes[i].has_lazy = false;
  }

  void pull(int i, int z)
  {
    nodes[i].prod = AM::op(node_prod(nodes[i].chi[0], initial_nodes[z].chi[0]),
                           node_prod(nodes[i].chi[1], initial_nodes[z].chi[1]));
  }

  template <class G>
  void modify_impl(ll p, const G &g, ll a, ll b, int i, int z)
  {
    if (b - a == 1)
    {
      g(nodes[i].prod);
      nodes[i].lazy = AM::id();
      nodes[i].has_lazy = false;
      return;
    }
    push(i, z);
    ll c = a + (b - a) / 2;
    int d = p < c ? 0 : 1;
    int nz = initial_nodes[z].chi[d];
    int j = ensure_child(i, d, nz);
    if (d == 0)
      modify_impl(p, g, a, c, j, nz);
    else
      modify_impl(p, g, c, b, j, nz);
    pull(i, z);
  }

  void apply_impl(ll l, ll r, const F &f, ll a, ll b, int i, int z)
  {
    if (l <= a && b <= r)
    {
      all_apply(i, f);
      return;
    }
    push(i, z);
    ll c = a + (b - a) / 2;
    if (l < c)
    {
      int nz = initial_nodes[z].chi[0];
      int j = ensure_child(i, 0, nz);
      apply_impl(l, r, f, a, c, j, nz);
    }
    if (c < r)
    {
      int nz = initial_nodes[z].chi[1];
      int j = ensure_child(i, 1, nz);
      apply_impl(l, r, f, c, b, j, nz);
    }
    pull(i, z);
  }

  S prod_impl(ll l, ll r, ll a, ll b, int i, int z, const F &f) const
  {
    if (b <= l || r <= a)
      return AM::e();
    if (l <= a && b <= r)
      return AM::mapping(f, node_prod(i, z));
    F nf = f;
    if (i != -1 && nodes[i].has_lazy)
      nf = AM::composition(f, nodes[i].lazy);
    ll c = a + (b - a) / 2;
    int li = i == -1 ? -1 : nodes[i].chi[0];
    int ri = i == -1 ? -1 : nodes[i].chi[1];
    return AM::op(prod_impl(l, r, a, c, li, initial_nodes[z].chi[0], nf),
                  prod_impl(l, r, c, b, ri, initial_nodes[z].chi[1], nf));
  }

  template <class G>
  ll max_right_bound(ll l, const G &g) const
  {
    assert(0 <= l && l <= n);
    assert(g(AM::e()));
    if (l == n)
      return n;
    S sm = AM::e();
    auto dfs = [&](auto dfs, ll a, ll b, int i, int z, const F &f) -> ll
    {
      if (b <= l)
        return -1;
      if (l <= a)
      {
        S x = AM::op(sm, AM::mapping(f, node_prod(i, z)));
        if (g(x))
        {
          sm = std::move(x);
          return -1;
        }
        if (b - a == 1)
          return a;
      }
      F nf = f;
      if (i != -1 && nodes[i].has_lazy)
        nf = AM::composition(f, nodes[i].lazy);
      ll c = a + (b - a) / 2;
      int li = i == -1 ? -1 : nodes[i].chi[0];
      int ri = i == -1 ? -1 : nodes[i].chi[1];
      ll res = dfs(dfs, a, c, li, initial_nodes[z].chi[0], nf);
      return res == -1 ? dfs(dfs, c, b, ri, initial_nodes[z].chi[1], nf) : res;
    };
    ll res = dfs(dfs, 0, n, 0, initial_root, AM::id());
    return res == -1 ? n : res;
  }

  template <class G>
  ll min_left_bound(ll r, const G &g) const
  {
    assert(0 <= r && r <= n);
    assert(g(AM::e()));
    if (r == 0)
      return 0;
    S sm = AM::e();
    auto dfs = [&](auto dfs, ll a, ll b, int i, int z, const F &f) -> ll
    {
      if (r <= a)
        return -1;
      if (b <= r)
      {
        S x = AM::op(AM::mapping(f, node_prod(i, z)), sm);
        if (g(x))
        {
          sm = std::move(x);
          return -1;
        }
        if (b - a == 1)
          return a;
      }
      F nf = f;
      if (i != -1 && nodes[i].has_lazy)
        nf = AM::composition(f, nodes[i].lazy);
      ll c = a + (b - a) / 2;
      int li = i == -1 ? -1 : nodes[i].chi[0];
      int ri = i == -1 ? -1 : nodes[i].chi[1];
      ll res = dfs(dfs, c, b, ri, initial_nodes[z].chi[1], nf);
      return res == -1 ? dfs(dfs, a, c, li, initial_nodes[z].chi[0], nf) : res;
    };
    ll res = dfs(dfs, 0, n, 0, initial_root, AM::id());
    return res == -1 ? 0 : res + 1;
  }

public:
  // 長さ n、各点の初期値 initial_value の列を作る
  SparseLazySegmentTree(ll n, const S &initial_value) : n(n)
  {
    assert(n >= 0);
    map<ll, int> ids;
    auto build = [&](auto build, ll len) -> int
    {
      auto it = ids.find(len);
      if (it != ids.end())
        return it->second;
      int l = -1, r = -1;
      S prod = len == 0 ? AM::e() : initial_value;
      if (len >= 2)
      {
        l = build(build, len / 2);
        r = build(build, len - len / 2);
        prod = AM::op(initial_nodes[l].prod, initial_nodes[r].prod);
      }
      int z = initial_nodes.size();
      initial_nodes.eb(prod, l, r);
      ids[len] = z;
      return z;
    };
    initial_root = build(build, n);
    nodes.eb(initial_nodes[initial_root].prod);
  }

  // 少なくとも n ノード分の領域をあらかじめ確保する
  void reserve(int n) { nodes.reserve(n); }

  // p 番目の値を参照で受け取る関数 g を適用する
  template <class G>
  void modify(ll p, const G &g)
  {
    assert(0 <= p && p < n);
    modify_impl(p, g, 0, n, 0, initial_root);
  }

  // p 番目の値を x にする
  void set(ll p, const S &x)
  {
    modify(p, [&](S &y) { y = x; });
  }

  // p 番目の値を返す
  S get(ll p) const
  {
    assert(0 <= p && p < n);
    return prod_impl(p, p + 1, 0, n, 0, initial_root, AM::id());
  }

  // p 番目に作用 f を適用する
  void apply(ll p, const F &f)
  {
    assert(0 <= p && p < n);
    apply_impl(p, p + 1, f, 0, n, 0, initial_root);
  }

  // [l, r) に作用 f を適用する
  void apply(ll l, ll r, const F &f)
  {
    assert(0 <= l && l <= r && r <= n);
    if (l != r)
      apply_impl(l, r, f, 0, n, 0, initial_root);
  }

  // [l, r) の積を返す
  S prod(ll l, ll r) const
  {
    assert(0 <= l && l <= r && r <= n);
    if (l == r)
      return AM::e();
    return prod_impl(l, r, 0, n, 0, initial_root, AM::id());
  }

  // 全体の積を返す
  S all_prod() const
  {
    return nodes[0].prod;
  }

  // g(prod(l, r)) が true となる最大の r を返す
  template <class G>
  ll max_right_ok(ll l, const G &g) const
  {
    return max_right_bound(l, g);
  }

  // g(prod(l, r)) が true となる最小の l を返す
  template <class G>
  ll min_left_ok(ll r, const G &g) const
  {
    return min_left_bound(r, g);
  }
};
