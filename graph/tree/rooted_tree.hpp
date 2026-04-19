#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 根つき木クラス（HLD もする）
 * @docs docs/graph/tree/rooted_tree.md
 */

// 参考: https://judge.yosupo.jp/submission/349844

// コンストラクタは
// - n, es, root (**es にコストの情報は入れない**)
// - n, p
// のいずれか
struct RootedTree
{
protected:
  static const int MSK = 1 << 30;
  int n;
  vc<int> p_head, dep, pre, post, preinv;
  // 構築後の p_head[v] の値:
  // - MSK が立っている場合、v は heavy path の先頭で、MSK を取ったものが親
  // - MSK が立っていない場合、v が属する heavy path の先頭

  // v が属する heavy path の先頭
  inline int internal_head(int v) const { return (p_head[v] & MSK) ? v : p_head[v]; }
  // hv が heavy path の先頭であるとして、hv の親。hv が根のときは根を返す
  inline int parent_of_head(int hv) const { return p_head[hv] & ~MSK; }

  // p_head に親配列が格納されている状態からスタート
  void build(int rt, const vc<int> &siz, vc<int> &max_chi_siz)
  {
    // 部分木サイズ順に並べる
    vc<int> ord(n);
    {
      vc<int> cnt(n + 1);
      repi(i, n) cnt[siz[i]]++;
      cnt = cumlsum(cnt);
      repi(i, n) ord[n - 1 - (cnt[siz[i]]++)] = i;
    }
    // 必要な配列の計算
    {
      p_head[rt] = rt | MSK, post[rt] = n;
      vc<int> add(n, 1);
      repi(i, 1, n)
      {
        int v = ord[i];
        int p = p_head[v];
        dep[v] = dep[p] + 1;
        pre[v] = pre[p] + add[p];
        add[p] += siz[v];
        post[v] = pre[v] + siz[v];
        if (max_chi_siz[p] == siz[v])
        {
          max_chi_siz[p] = 0;
          p_head[v] = internal_head(p_head[v]);
        }
        else
          p_head[v] |= MSK;
      }
      preinv = perminv(pre);
    }
  }

public:
  RootedTree() {}
  template <class I>
  RootedTree(int n, const vc<I> &par)
  : n(n), p_head(n), dep(n), pre(n), post(n), preinv(n)
  {
    assert(n >= 1);
    assert(SZ(par) == n);
    int rt = -1;
    vc<int> deg(n), siz(n, 1), max_chi_siz(n);
    repi(i, n)
    {
      if (par[i] < 0 || par[i] == i)
      {
        assert(rt == -1 && "There are more than two roots");
        rt = i;
      }
      else
      {
        p_head[i] = par[i];
        deg[par[i]]++;
      }
    }
    assert(rt != -1 && "There is no root");
    repi(i, n)
    {
      int v = i;
      while (v != rt && deg[v] == 0)
      {
        int p = p_head[v];
        deg[p]--, deg[v]--;
        siz[p] += siz[v];
        chmax(max_chi_siz[p], siz[v]);
        v = p;
      }
    }
    build(rt, siz, max_chi_siz);
  }
  template <class P>
  RootedTree(int n, const vc<P> &es, int rt)
  : n(n), p_head(n), dep(n), pre(n), post(n), preinv(n)
  {
    assert(n >= 1);
    assert(SZ(es) == n - 1);
    assert(0 <= rt && rt < n);

    vc<int> deg(n), siz(n, 1), max_chi_siz(n);
    fec([ u, v ] : es)
    {
      deg[u]++, deg[v]++;
      p_head[u] ^= v, p_head[v] ^= u;
    }
    deg[rt] = 0;
    repi(i, n)
    {
      int v = i;
      while (deg[v] == 1)
      {
        int p = p_head[v];
        deg[p]--, deg[v]--, p_head[p] ^= v;
        siz[p] += siz[v];
        chmax(max_chi_siz[p], siz[v]);
        v = p;
      }
    }
    build(rt, siz, max_chi_siz);
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
    int ph = p_head[v];
    if (ph & MSK)
      return ph & ~MSK;
    else
      return preinv[pre[v] - 1];
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
  // 長さ n の配列。v 番目には v が子側になる辺の情報が格納される。根は未定義。
  template <class P, class EdgeInfo>
  vc<EdgeInfo> reordered_edge_info(const vc<P> &es, const vc<EdgeInfo> &edge_info) const
  {
    assert(SZ(es) == n - 1 && SZ(edge_info) >= n - 1);
    vc<EdgeInfo> res(n);
    repi(i, n - 1)
    {
      auto [u, v] = es[i];
      int eid = dep[u] < dep[v] ? v : u;
      res[eid] = edge_info[i];
    }
    return res;
  }

  int lca(int u, int v) const
  {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    while (true)
    {
      int hu = internal_head(u), hv = internal_head(v);
      if (hu == hv)
        break;
      if (dep[hu] > dep[hv])
        swap(u, v), swap(hu, hv);
      v = parent_of_head(hv);
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

  // v の k 個上。なければ -1
  int la(int v, int k) const
  {
    assert(0 <= v && v < n);
    assert(0 <= k);
    if (k > dep[v])
      return -1;
    while (true)
    {
      int hv = internal_head(v);
      if (k <= dep[v] - dep[hv])
        break;
      k -= dep[v] - dep[hv] + 1;
      v = parent_of_head(hv);
    }
    return preinv[pre[v] - k];
  }

  // u から v に k 個進む。なければ -1
  int jump(int u, int v, int k) const
  {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    assert(0 <= k);
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
  // 使う際は reordered_edge_info 関数も適切に活用するとよい
  template <class T>
  vc<T> weighted_depths(vc<T> costs) const
  {
    vc<T> res(n, 0);
    auto vs = top_down_vertices();
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
  vc<T> reordered_vertex_info(const vc<T> &vertex_info) const
  {
    return permuted(vertex_info, preinv);
  }
  // v の部分木が行きがけ順 [l, r) の頂点であるような (l, r)
  // edge のときは辺属性 (v を除く)
  pair<int, int> subtree_interval(int v, bool edge = false) const
  {
    return {pre[v] + edge, post[v]};
  }
  // u から v へのパスが
  // 行きがけ順 [l_1, r_1), ..., [l_k, r_k) の頂点 (この順) であるとき
  // f(l_1, r_1, isrev), ..., f(l_k, r_k, isrev) を順に実行する
  // isrev は逆向きのとき
  // edge のときは辺属性 (lca(u, v) を除く)
  template <class F>
  void path_query(int u, int v, F f, bool edge = false) const
  {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    static pair<int, int> down_path[30];
    int down_cnt = 0;
    while (true)
    {
      int hu = internal_head(u), hv = internal_head(v);
      if (hu == hv)
        break;
      if (dep[hu] > dep[hv])
      {
        f(pre[hu], pre[u] + 1, true);
        u = parent_of_head(hu);
      }
      else
      {
        down_path[down_cnt++] = {pre[hv], pre[v] + 1};
        v = parent_of_head(hv);
      }
    }

    if (dep[u] > dep[v])
    {
      int l = pre[v] + edge;
      int r = pre[u] + 1;
      if (l < r)
        f(l, r, true);
    }
    else
    {
      int l = pre[u] + edge;
      int r = pre[v] + 1;
      if (l < r)
        f(l, r, false);
    }

    repi(i, down_cnt - 1, -1, -1)
    {
      auto [l, r] = down_path[i];
      f(l, r, false);
    }
  }
};
