#pragma once

#include "add_virtual_root.hpp"

/**
 * @brief 木の直径と中心
 * @docs docs/tree/diameter.md
 */

template <class Cost = void>
struct Diameter
{
  using Dist = conditional_t<is_void_v<Cost>, ll, Cost>;

private:
  RootedTree tree;
  int a, b;
  vc<Dist> depths;

  int farthest() const
  {
    int res = 0;
    for (int v = 1; v < tree.size<int>(); ++v)
    {
      if constexpr (is_void_v<Cost>)
      {
        if (tree.depth(v) > tree.depth(res)) res = v;
      }
      else if (depths[v] > depths[res]) res = v;
    }
    return res;
  }

  Dist distance_from_a(int v) const
  {
    if constexpr (is_void_v<Cost>) return tree.depth(v);
    else return depths[v];
  }

  vc<Dist> distances_from_b() const
  {
    vc<Dist> res(tree.size<int>(), length());
    for (int v : tree.top_down_vertices()) if (v != a)
    {
      const int p = tree.parent(v);
      const Dist w = distance_from_a(v) - distance_from_a(p);
      if (tree.is_ancestor(v, b)) res[v] = res[p] - w;
      else res[v] = res[p] + w;
    }
    return res;
  }

  // 中点の頂点、または中点を含む辺の親側と子側を返す。
  pair<int, optional<int>> midpoint() const
  {
    if constexpr (is_void_v<Cost>)
    {
      const int d = tree.depth<int>(b);
      const int left = *tree.la(b, (d + 1) / 2);
      if (d % 2) return {left, tree.la(b, d / 2)};
      return {left, nullopt};
    }
    else
    {
      const Dist diameter = length();
      if (diameter == 0) return {0, nullopt};
      int left = b;
      optional<int> right;
      while (depths[left] + depths[left] > diameter)
      {
        right = left;
        left = tree.parent(left);
      }
      if (depths[left] + depths[left] < diameter) return {left, right};
      while (left != a && depths[tree.parent(left)] == depths[left]) left = tree.parent(left);
      int representative = left;
      vc<int> que{left};
      for (int i = 0; i < int(que.size()); ++i)
        for (int v : tree.children(que[i])) if (depths[v] == depths[que[i]])
        {
          representative = min(representative, v);
          que.eb(v);
        }
      return {representative, nullopt};
    }
  }

public:
  template <class Pair, class C = Cost, enable_if_t<is_void_v<C> && is_same_v<C, Cost>, int> = 0>
  Diameter(int n, const vc<Pair> &es)
      : tree(n, es, 0)
  {
    a = farthest();
    tree = tree.rerooted(a);
    b = farthest();
  }

  template <class Pair, class C, enable_if_t<!is_void_v<C> && is_same_v<C, Cost>, int> = 0>
  Diameter(int n, const vc<Pair> &es, const vc<C> &costs)
      : tree(n, es, 0)
  {
    assert(SZ(costs) == n - 1);
    assert(all_of(ALL(costs), [](const auto &x) { return x >= 0; }));
    auto weighted_depths = [&]()
    {
      vc<Dist> values(n, 0);
      for (int i = 0; i < n - 1; ++i)
      {
        auto [u, v] = es[i];
        values[tree.parent_child(u, v).second] = costs[i];
      }
      return tree.weighted_depths(move(values));
    };
    depths = weighted_depths();
    a = farthest();
    tree = tree.rerooted(a);
    depths = weighted_depths();
    b = farthest();
  }

  template <class I, class C, enable_if_t<!is_void_v<C> && is_same_v<C, Cost>, int> = 0>
  Diameter(int n, const vc<tuple<I, I, C>> &es)
      : Diameter(n,
                 gen_vec(SZ<int>(es), [&](int i) { return pair{get<0>(es[i]), get<1>(es[i])}; }),
                 gen_vec(SZ<int>(es), [&](int i) { return get<2>(es[i]); })) {}

  // 直径の 2 端点の一例
  pair<int, int> endpoints() const { return {a, b}; }

  // 直径の辺コスト和
  Dist length() const
  {
    if constexpr (is_void_v<Cost>) return tree.depth(b);
    else return depths[b];
  }

  CPP_DUMP_DEFINE_DATA(a, b, depths, tree);

  // 直径の一例
  vc<int> path_vertex() const { return tree.path_vertex(a, b); }

  // 各頂点からの最大距離を最小にする頂点 (番号順)
  // コストなしのとき、中心 (1 個または 2 個)
  vc<int> centers() const
  {
    if constexpr (is_void_v<Cost>)
    {
      const auto [left, right] = midpoint();
      if (!right) return {left};
      return {min(left, *right), max(left, *right)};
    }
    else
    {
      const int n = tree.size<int>();
      const auto from_b = distances_from_b();
      vc<int> res;
      Dist best = max(depths[0], from_b[0]);
      for (int v = 0; v < n; ++v)
      {
        const Dist eccentricity = max(depths[v], from_b[v]);
        if (eccentricity < best) best = eccentricity, res.clear();
        if (eccentricity == best) res.eb(v);
      }
      return res;
    }
  }

  // 異なる集合から選んだ相異なる 2 頂点が直径の両端となる分割を返す。
  // 集合内は頂点番号順、集合は最小要素の昇順。直径 0 では各頂点の単集合。
  vvc<int> endpoint_groups() const
  {
    const int n = tree.size<int>();
    const Dist diameter = length();
    if (diameter == 0)
    {
      vvc<int> res(n);
      for (int v = 0; v < n; ++v) res[v].eb(v);
      return res;
    }

    const auto from_b = distances_from_b();
    vc<bool> at_center(n);
    bool has_center = false;
    for (int v = 0; v < n; ++v)
    {
      const Dist da = distance_from_a(v);
      at_center[v] = da == from_b[v] && da + from_b[v] == diameter;
      has_center |= at_center[v];
    }

    vc<int> branch(n, a);
    if (has_center)
    {
      for (int v : tree.top_down_vertices()) if (v != a)
      {
        const int p = tree.parent(v);
        branch[v] = at_center[p] && !at_center[v] ? v : branch[p];
      }
    }
    else
    {
      for (int v = 0; v < n; ++v) branch[v] = distance_from_a(v) < from_b[v] ? a : b;
    }

    vvc<int> res;
    vc<int> index(n, 0);
    for (int v = 0; v < n; ++v) if (max(distance_from_a(v), from_b[v]) == diameter)
    {
      int &i = index[branch[v]];
      if (i == 0) i = int(res.size()) + 1, res.eb();
      res[i - 1].eb(v);
    }
    return res;
  }

  // コストなし
  // 中心が 1 個のとき: n 頂点、中心を根とする木
  // 中心が 2 個のとき: n+1 頂点、頂点 n を根とする木、頂点 n の 2 つの子が中心
  template <class C = Cost, enable_if_t<is_void_v<C> && is_same_v<C, Cost>, int> = 0>
  RootedTree virtual_center_tree() const
  {
    const auto [left, right] = midpoint();
    auto centered = tree.rerooted(left);
    if (!right) return centered;
    auto par = centered.parents();
    par[*right] = -1;
    return add_virtual_root(tree.size<int>(), par);
  }

  // コストつき
  // first:
  //   - 中点が頂点のとき: n 頂点、中心を根とする木
  //   - 中心が辺内のとき: n+1 頂点、頂点 n を根とする木、根の子はその辺の 2 端点
  // 
  // second[v]: v が子側の辺の重みの 2 倍 (根の値は 0)
  template <class C = Cost, enable_if_t<!is_void_v<C> && is_same_v<C, Cost>, int> = 0>
  pair<RootedTree, vc<Dist>> virtual_center_tree() const
  {
    const int n = tree.size<int>();
    const Dist diameter = length();
    const auto [left, right] = midpoint();
    auto centered = tree.rerooted(left);
    vc<Dist> costs2(n + bool(right), 0);
    for (int v = 0; v < n; ++v) if (v != left)
    {
      const int p = centered.parent(v);
      const Dist w = depths[v] < depths[p] ? depths[p] - depths[v] : depths[v] - depths[p];
      costs2[v] = w + w;
    }

    if (!right) return {move(centered), move(costs2)};
    auto par = centered.parents();
    par[left] = par[*right] = n;
    costs2[left] = diameter - depths[left] - depths[left];
    costs2[*right] = depths[*right] + depths[*right] - diameter;
    par.eb(-1);
    return {RootedTree(n + 1, par), move(costs2)};
  }
};

template <class Pair>
Diameter(int, const vc<Pair> &) -> Diameter<>;
template <class Pair, class Cost>
Diameter(int, const vc<Pair> &, const vc<Cost> &) -> Diameter<Cost>;
template <class I, class Cost>
Diameter(int, const vc<tuple<I, I, Cost>> &) -> Diameter<Cost>;
