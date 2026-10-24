#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 根つき木クラス（HLD もする）
 * @docs docs/tree/rooted_tree.md
 */

//! XOR Linked Tree の考え方: https://codeforces.com/blog/entry/135239
struct RootedTree
{
protected:
  int n;
  vc<int> link, dep, pre, post, preinv;
  // link[v] < 0: v は heavy path の先頭で、~link[v] が親。
  // それ以外: link[v] は v が属する heavy path の先頭。
  int internal_head(int v) const { return link[v] < 0 ? v : link[v]; }
  int parent_of_head(int h) const { return ~link[h]; }

  template <bool xor_edges>
  void build(int rt)
  {
    // pre は残りの次数（親配列なら子の数）、link は隣接頂点の XOR（または親）。
    // 葉を除いた順を残すと、逆順では各頂点の親が先に確定する。
    constexpr int leaf_degree = xor_edges ? 1 : 0;
    pre[rt] = leaf_degree - 1;
    vc<int> removed(n - 1);
    int count = 0;
    for (int seed = 0; seed < n; ++seed)
    {
      int v = seed;
      while (pre[v] == leaf_degree)
      {
        --pre[v];
        const int p = link[v];
        removed[count++] = v;
        if constexpr (xor_edges) link[p] ^= v;
        post[p] += post[v];
        dep[p] = max(dep[p], post[v]);
        --pre[p];
        v = p;
      }
    }
    assert(count == n - 1);

    // dep に置いた最大の子部分木サイズから heavy child を選ぶ。
    for (int v : removed)
    {
      const int p = link[v];
      if (post[v] == dep[p]) link[v] = ~p, dep[p] = 0;
    }
    pre[rt] = dep[rt] = 0;
    link[rt] = ~rt, post[rt] = preinv[rt] = n;
    for (int i = n - 2; i >= 0; --i)
    {
      const int v = removed[i];
      const bool heavy = link[v] < 0;
      const int p = heavy ? ~link[v] : link[v];
      // heavy child の領域を親の直後に予約し、light child は右から詰める。
      pre[v] = heavy ? pre[p] + 1 : (preinv[p] -= post[v]);
      preinv[v] = post[v] += pre[v];
      dep[v] = dep[p] + 1;
      link[v] = heavy ? internal_head(p) : ~p;
    }
    // light child の配置に使ったカーソルを、行きがけ順の逆引きへ使い直す。
    for (int v = 0; v < n; ++v) preinv[pre[v]] = v;
  }

public:
  RootedTree() {}
  template <class I>
  RootedTree(int n, const vc<I> &parents)
      : n(n), link(n), dep(n), pre(n), post(n, 1), preinv(n)
  {
    assert(n >= 1 && SZ(parents) == n);
    int rt = -1;
    for (int v = 0; v < n; ++v)
    {
      if (parents[v] < 0 || parents[v] == v)
      {
        assert(rt == -1 && "RootedTree requires exactly one root");
        rt = v;
      }
      else
      {
        assert(0 <= parents[v] && parents[v] < n);
        ++pre[link[v] = parents[v]];
      }
    }
    assert(rt != -1 && "RootedTree requires a root");
    build<false>(rt);
  }
  template <class P>
  RootedTree(int n, const vc<P> &es, int rt)
      : n(n), link(n), dep(n), pre(n), post(n, 1), preinv(n)
  {
    assert(n >= 1 && SZ(es) == n - 1);
    assert(0 <= rt && rt < n);
    for (const auto &[u, v] : es)
    {
      ++pre[u], ++pre[v];
      link[u] ^= v, link[v] ^= u;
    }
    build<true>(rt);
  }

  // 頂点数を返す
  template <class I = ll>
  I size() const { return n; }
  // 根を返す
  int root() const { return preinv[0]; }

  // v の深さ
  template <class I = ll>
  I depth(int v) const
  {
    assert(0 <= v && v < n);
    return dep[v];
  }

  // v を根とする部分木のサイズ (v も含む)
  template <class I = ll>
  I subtree_size(int v) const
  {
    assert(0 <= v && v < n);
    return post[v] - pre[v];
  }
  // 辺 (u, v) が存在するとして、これを切ったときの
  // u 側、v 側それぞれの連結成分サイズ
  template <class I = ll>
  pair<I, I> cut_and_subtree_size(int u, int v) const
  {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    if (dep[u] < dep[v])
      return {n - subtree_size(v), subtree_size(v)};
    else
      return {subtree_size(u), n - subtree_size(u)};
  }

  // v の親を返す。v は根であってはいけない
  int parent(int v) const
  {
    assert(0 <= v && v < n);
    assert(v != root());
    return link[v] < 0 ? ~link[v] : preinv[pre[v] - 1];
  }

  // 頂点番号順の親配列を返す。根の親は -1。
  vc<int> parents() const
  {
    vc<int> res(n, -1);
    const int rt = root();
    for (int v = 0; v < n; ++v) if (v != rt) res[v] = parent(v);
    return res;
  }

  // v が属する heavy path の先頭を返す
  int head(int v) const
  {
    assert(0 <= v && v < n);
    return internal_head(v);
  }

  // v の preorder
  int preorder(int v) const
  {
    assert(0 <= v && v < n);
    return pre[v];
  }
  // v の postorder
  int postorder(int v) const
  {
    assert(0 <= v && v < n);
    return post[v];
  }
  // preorder が i の頂点
  int preorder_select(int i) const
  {
    assert(0 <= i && i < n);
    return preinv[i];
  }

  // v から根までのパスで通る頂点の列
  vc<int> path_to_root(int v) const
  {
    assert(0 <= v && v < n);
    vc<int> res = {v};
    res.reserve(dep[v] + 1);
    int rt = root();
    while (v != rt)
    {
      int p = parent(v);
      res.eb(p);
      v = p;
    }
    return res;
  }
  // u から v までのパスで通る頂点の列
  vc<int> path(int u, int v) const
  {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    vc<int> pu = {u}, pv = {v};
    while (u != v)
    {
      if (dep[u] > dep[v])
      {
        int p = parent(u);
        pu.eb(p);
        u = p;
      }
      else
      {
        int p = parent(v);
        pv.eb(p);
        v = p;
      }
    }
    pv.pop_back();
    return concat(pu, reversed(pv));
  }

  // 根に近い頂点ほど先に来る順序で並べた頂点の列をひとつ返す
  const vc<int> &top_down_vertices() const { return preinv; }
  // 深い頂点ほど先に来る順序で並べた頂点の列を返す
  vc<int> bottom_up_vertices() const { return reversed(top_down_vertices()); }

  // bp: 長さ 2(n-1) の括弧列
  // vs: 長さ 2(n-1)+1 の頂点列
  // 0 <= i < 2(n-1) に対し、i 回目の移動は bp[i] の向きで vs[i] から vs[i+1] へ
  pair<string, vc<int>> dfs_ordered_vertices() const
  {
    string bp(2 * (n - 1), '?');
    vc<int> vs(2 * (n - 1) + 1);
    int rt = root();
    vs[0] = rt;
    repi(v, n)
    {
      if (v == rt)
        continue;
      int p = parent(v);
      int in_idx = 2 * pre[v] - dep[v] - 1;
      int out_idx = 2 * post[v] - dep[v] - 2;
      bp[in_idx] = '(', vs[in_idx + 1] = v;
      bp[out_idx] = ')', vs[out_idx + 1] = p;
    }
    return {bp, vs};
  }

  class ChildIterator;
  class ChildRange;

  class ChildIterator
  {
  private:
    const RootedTree* t;
    int cur;
    friend class RootedTree;
    friend class ChildRange;
    ChildIterator(const RootedTree* t, int cur) : t(t), cur(cur) {}

  public:
    using iterator_category = std::input_iterator_tag;
    using value_type        = int;
    using difference_type   = std::ptrdiff_t;
    using pointer           = int*;
    using reference         = int;
    int operator*() const { return t->preinv[cur]; }
    void operator++() { cur = t->post[t->preinv[cur]]; }
    bool operator!=(const ChildIterator& r) const { return cur != r.cur; }
    bool operator==(const ChildIterator& r) const { return cur == r.cur; }
  };

  class ChildRange
  {
  private:
    const RootedTree* t;
    int cur, end_pos;
    friend class RootedTree;
    ChildRange(const RootedTree* t, int cur, int end_pos) : t(t), cur(cur), end_pos(end_pos) {}

  public:
    ChildIterator begin() const { return ChildIterator(t, cur); }
    ChildIterator end() const { return ChildIterator(t, end_pos); }
    bool empty() const { return cur == end_pos; }
    vc<int> to_v() const { return vc<int>(begin(), end()); }
  };

  // v の heavy child (なければ -1)
  int heavy_child(int v) const
  {
    assert(0 <= v && v < n);
    if (post[v] - pre[v] == 1)
      return -1;
    return preinv[pre[v] + 1];
  }
  ChildRange children(int v) const
  {
    assert(0 <= v && v < n);
    return {this, pre[v] + 1, post[v]};
  }
  ChildRange light_children(int v) const
  {
    assert(0 <= v && v < n);
    int cur = pre[v] + 1;
    if (cur < post[v])
    {
      cur = post[preinv[cur]];
    }
    return {this, cur, post[v]};
  }

  // 辺 (u, v) が存在するとして、(親側, 子側)
  pair<int, int> parent_child(int u, int v) const
  {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    return dep[u] < dep[v] ? pair{u, v} : pair{v, u};
  }
  // (u, v, value) の列を頂点番号順に変換する。各辺の値を子側に置く。
  template <class I, class T>
  vc<T> edge_to_vertex_values(const vc<tuple<I, I, T>> &es, const T &root_value = T{}) const
  {
    assert(SZ(es) == n - 1);
    vc<T> res(n, root_value);
    fec([u, v, value] : es)
    {
      int child = parent_child(u, v).second;
      res[child] = value;
    }
    return res;
  }

  int lca(int u, int v) const
  {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    int hu = internal_head(u), hv = internal_head(v);
    while (hu != hv)
    {
      if (dep[hu] < dep[hv]) v = parent_of_head(hv), hv = internal_head(v);
      else u = parent_of_head(hu), hu = internal_head(u);
    }
    return dep[u] < dep[v] ? u : v;
  }

  template <class I = ll>
  I dist(int u, int v) const
  {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    return dep[u] + dep[v] - 2 * dep[lca(u, v)];
  }

  // u が v の祖先か
  bool is_ancestor(int u, int v) const
  {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    return pre[u] <= pre[v] && pre[v] < post[u];
  }

  // v から根への道で、pred が false -> true に切り替わる境界 (子側, 親側)。
  // 境界の子側は false、親側は true。存在しない側は -1 とする。
  // v 自身が true なら (-1, v)、根まで false なら (root(), -1) を返す。
  // pred は、この道に沿って false -> true の向きに単調であること。
  template <class Pred>
  pair<int, int> first_ancestor(int v, const Pred &pred) const
  {
    assert(0 <= v && v < n);
    int rt = root(), child = -1;
    while (true)
    {
      int h = internal_head(v);
      if (pred(h))
      {
        int ok = pre[h], ng = pre[v] + 1;
        while (ng - ok > 1)
        {
          int mid = ok + (ng - ok) / 2;
          if (pred(preinv[mid])) ok = mid;
          else ng = mid;
        }
        return {ok < pre[v] ? preinv[ok + 1] : child, preinv[ok]};
      }
      if (h == rt) return {rt, -1};
      child = h;
      v = parent_of_head(h);
    }
  }

  // v の k 個上。なければ -1
  int la(int v, int k) const
  {
    assert(0 <= v && v < n);
    assert(0 <= k);
    if (k == 0) return v;
    if (k > dep[v])
      return -1;
    const int target_depth = dep[v] - k;
    while (dep[internal_head(v)] > target_depth) v = parent_of_head(internal_head(v));
    return preinv[pre[v] - dep[v] + target_depth];
  }

  // u から v に k 個進む。なければ -1
  int jump(int u, int v, int k) const
  {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    assert(0 <= k);
    if (k == 0) return u;
    int l = lca(u, v);
    int d_ul = dep[u] - dep[l];
    int d_vl = dep[v] - dep[l];
    if (k > d_ul + d_vl)
      return -1;
    if (k <= d_ul)
      return la(u, k);
    return la(v, d_ul + d_vl - k);
  }

  // costs[v] := (parent(v), v) のコスト としたときの、各頂点の深さ
  // costs[root()] は参照されない
  // edge_to_vertex_values で辺の値を頂点番号順に変換できる
  template <class T>
  vc<T> weighted_depths(vc<T> costs) const
  {
    assert(SZ(costs) == n);
    const auto &vs = top_down_vertices();
    costs[root()] = 0;
    rep(i, 1, n)
    {
      int v = vs[i], p = parent(v);
      costs[v] += costs[p];
    }
    return costs;
  }

  // 頂点データを行きがけ順に並べ替える
  template <class T>
  vc<T> reordered_vertex_values(const vc<T> &vertex_values) const
  {
    return permuted(vertex_values, preinv);
  }
  // v の部分木が行きがけ順 [l, r) の頂点であるような (l, r)
  // edge のときは辺属性 (v を除く)
  pair<int, int> subtree_interval(int v, bool edge = false) const
  {
    return {pre[v] + edge, post[v]};
  }
  class PathRange
  {
  private:
    // 各端点から根までの light edge 数は log2(n) 以下。
    static constexpr int capacity = 2 * numeric_limits<int>::digits;
    array<tuple<int, int, bool>, capacity> intervals;
    int count = 0;
    friend class RootedTree;
    PathRange() = default;

  public:
    using iterator = decltype(intervals)::const_iterator;
    iterator begin() const { return intervals.begin(); }
    iterator end() const { return intervals.begin() + count; }
    int size() const { return count; }
    bool empty() const { return count == 0; }
    vc<tuple<int, int, bool>> to_v() const { return {begin(), end()}; }
  };

  // u から v へのパスを行きがけ順の半開区間 (l, r, isrev) に分解する
  // パス順に走査でき、isrev は逆向き (r-1, ..., l の順) のとき true
  // edge のときは辺属性 (lca(u, v) を除いたもの)
  PathRange path_query(int u, int v, bool edge = false) const
  {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    PathRange res;
    int down_begin = PathRange::capacity;
    while (true)
    {
      int hu = internal_head(u), hv = internal_head(v);
      if (hu == hv)
        break;
      if (dep[hu] > dep[hv])
      {
        res.intervals[res.count++] = {pre[hu], pre[u] + 1, true};
        u = parent_of_head(hu);
      }
      else
      {
        res.intervals[--down_begin] = {pre[hv], pre[v] + 1, false};
        v = parent_of_head(hv);
      }
    }

    if (dep[u] > dep[v])
    {
      int l = pre[v] + edge;
      int r = pre[u] + 1;
      if (l < r)
        res.intervals[res.count++] = {l, r, true};
    }
    else
    {
      int l = pre[u] + edge;
      int r = pre[v] + 1;
      if (l < r)
        res.intervals[res.count++] = {l, r, false};
    }

    while (down_begin < PathRange::capacity)
      res.intervals[res.count++] = res.intervals[down_begin++];
    return res;
  }
};

#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT(RootedTree, size(), root(), parents());
#endif
