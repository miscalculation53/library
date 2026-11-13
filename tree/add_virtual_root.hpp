#pragma once

#include "rooted_tree.hpp"

/**
 * @brief 森に仮想根を追加して根つき木を構築
 * @docs docs/tree/add_virtual_root.md
 */

template <class T>
RootedTree add_virtual_root(int n, const vc<T> &forest)
{
  assert(n >= 0);
  vc<int> par(n + 1, -1);
  if constexpr (is_integral_ext<T>)
  {
    assert(SZ(forest) == n);
    repi(v, n)
    {
      if constexpr (is_signed_ext<T>) if (forest[v] < 0) { par[v] = n; continue; }
      if (u128(forest[v]) == unsigned(v)) par[v] = n;
      else
      {
        assert(u128(forest[v]) < unsigned(n));
        par[v] = forest[v];
      }
    }
  }
  else
  {
    vvc<int> adj(n);
    for (auto [u, v] : forest)
    {
      assert(u128(u) < unsigned(n) && u128(v) < unsigned(n));
      adj[u].eb(v), adj[v].eb(u);
    }
    vc<int> que;
    que.reserve(n);
    repi(root, n) if (par[root] == -1)
    {
      par[root] = n;
      que.clear();
      que.eb(root);
      for (int i = 0; i < SZ<int>(que); i++)
      {
        int v = que[i];
        for (int to : adj[v]) if (to != par[v])
        {
          assert(par[to] == -1 && "The edges must form a forest");
          par[to] = v;
          que.eb(to);
        }
      }
    }
  }
  return RootedTree(n + 1, par);
}
