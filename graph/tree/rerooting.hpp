#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "rooted_tree.hpp"

/**
 * @brief 全方位木 DP
 * @docs docs/graph/tree/rerooting.md
 */

// https://trap.jp/post/1702/
// 根を r として、各頂点 v について、次の res_r を計算する木 DP を考えたとき、
// すべての r について res_r[r] を計算する
// res_r[v] = putv( op[c は v の子]{pute(res_r[c], v → c)}, v )
// .
// 定義するもの:
// - M: **可換**モノイド
// - S pute(S x, int eid)
// - S putv(S x, int v)
template <class M, class Pair, class PUTE, class PUTV>
vc<typename M::S> rerooting(int n, const vc<Pair> &es, const PUTE &pute, const PUTV &putv, bool debug = true)
{
  using S = typename M::S;
  if (n == 0)
    return {};
  RootedTree gr(n, es, 0);
  vc<int> eids = gr.reordered_edge_info(es, permid<int>(n - 1));
  // 1. 普通に木 DP をする (その結果は g に入る)
  vc<S> f(n, M::e()), g(n, M::e());
  fe(v : gr.bottom_up_vertices())
  {
    if (v == gr.root())
      continue;
    int p = gr.parent(v);
    dump(p, v);
    g[v] = putv(f[v], v);
    f[p] = M::op(f[p], pute(g[v], eids[v]));
  }
  g[0] = putv(f[0], 0);
  if (debug)
    dump(g | cp::index());
  // 2. いい感じに差分を計算する
  vc<S> h(n, M::e()), res(n, M::e());
  res[0] = g[0];
  fe(v : gr.top_down_vertices())
  {
    cauto &chi = gr.children(v).to_v();
    cauto vec = GEN_VEC(chi.size(), i, pute(g[chi[i]], eids[chi[i]]));
    cauto sml = cuml<M>(vec), smr = cumr<M>(vec);
    repi(i, chi.size())
    {
      const int c = chi[i], eid = eids[c];
      h[c] = pute(putv(M::op(M::op(sml[i], smr[i + 1]), h[v]), v), eid);
      res[c] = putv(M::op(f[c], h[c]), c);
    }
  }
  return res;
}
