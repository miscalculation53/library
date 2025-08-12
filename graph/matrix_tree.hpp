#pragma once

#include "../template/template_all_but_modint.hpp"

#include "graph.hpp"
#include "../ds/my_queue.hpp"
#include "../math/linalg/matrix.hpp"
#include "../math/modint/binomial.hpp"

/**
 * @brief 行列木定理など
 * @docs docs/graph/matrix_tree.md
 */

// 無向グラフの全域木の個数
// g には隣接行列 (辺の本数) を渡す
// 行列式の計算、O(n^3) など
template <class mint, class I>
mint count_spanning_trees_undirected(const vvc<I> &g)
{
  const int n = g.size();
  if (n == 0)
    return 1;
  repi(i, n) assert(SZ(g[i]) == n);
  Matrix<FieldAddSubMulDiv<mint>> mat(n - 1, n - 1);
  repi(i, n) repi(j, i + 1, n)
  {
    assert(g[i][j] == g[j][i]);
    if (i < n - 1 && j < n - 1)
      mat[i][j] = mat[j][i] = -g[i][j];
    if (i < n - 1)
      mat[i][i] += g[i][j];
    if (j < n - 1)
      mat[j][j] += g[i][j];
  }
  return mat.det();
}

// 有向グラフの r を根とする（根から出る）全域有向木の個数
// g には隣接行列 (辺の本数) を渡す
// 行列式の計算、O(n^3) など
template <class mint, class I>
mint count_spanning_trees_directed(const vvc<I> &g, int r)
{
  const int n = g.size();
  if (n == 0)
    return 1;
  repi(i, n) assert(SZ(g[i]) == n);
  Matrix<FieldAddSubMulDiv<mint>> mat(n - 1, n - 1);
  repi(i, r) repi(j, r)
  {
    if (i == j)
      continue;
    mat[i][j] = -g[i][j];
    mat[j][j] += g[i][j];
  }
  repi(i, r) repi(j, r + 1, n)
  {
    mat[i][j - 1] = -g[i][j];
    mat[j - 1][j - 1] += g[i][j];
  }
  repi(j, r) mat[j][j] += g[r][j];
  repi(j, r + 1, n) mat[j - 1][j - 1] += g[r][j];
  repi(i, r + 1, n) repi(j, r)
  {
    mat[i - 1][j] = -g[i][j];
    mat[j][j] += g[i][j];
  }
  repi(i, r + 1, n) repi(j, r + 1, n)
  {
    if (i == j)
      continue;
    mat[i - 1][j - 1] = -g[i][j];
    mat[j - 1][j - 1] += g[i][j];
  }
  return mat.det();
}

// 有向グラフのオイラー閉路の個数
// g には隣接行列 (辺の本数) を渡す
// 行列式の計算 O(n^3) など + 階乗前計算 O(max{入次数})
template <class mint, class I>
mint count_eularian_circuits(const vvc<I> &g)
{
  const int n = g.size();
  repi(i, n) assert(SZ(g[i]) == n);

  vc<int> indeg(n, 0), outdeg(n, 0);
  repi(i, n) repi(j, n) indeg[j] += g[i][j], outdeg[i] += g[i][j];
  // 孤立点を除去
  int k = 0;
  vc<int> id(n, -1);
  repi(i, n) if (indeg[i] != 0 || outdeg[i] != 0) id[i] = k++;
  vvc<I> h(k, vc<I>(k, 0));
  repi(i, n) repi(j, n)
  {
    if (g[i][j] == 0)
      continue;
    h[id[i]][id[j]] = g[i][j];
  }
  if (k == 0)
    return 1;
  repi(i, n) if (indeg[i] != outdeg[i]) return 0;
  // 孤立点を除いて連結でないなら 0
  int cnt_visited = 1;
  vb visited(k, false);
  visited[0] = true;
  MyQueue<int> que;
  que.push(0);
  while (!que.empty())
  {
    int i = que.front();
    que.pop();
    repi(j, k)
    {
      if (h[i][j] == 0)
        continue;
      if (visited[j])
        continue;
      visited[j] = true;
      cnt_visited++;
      if (cnt_visited == k)
        break;
      que.push(j);
    }
    if (cnt_visited == k)
      break;
  }
  if (cnt_visited < k)
    return 0;

  mint res = count_spanning_trees_directed<mint>(h, 0);
  repi(i, n) if (indeg[i] != 0) res *= Binomial<mint>::fac(indeg[i] - 1);
  return res;
}

// 有向グラフのオイラー路の個数
// g には隣接行列 (辺の本数) を渡す
// 行列式の計算 O(n^3) など + 階乗前計算 O(max{入次数})
template <class mint, class I>
mint count_eularian_trails(const vvc<I> &g)
{
  const int n = g.size();
  repi(i, n) assert(SZ(g[i]) == n);
  vc<int> indeg(n, 0), outdeg(n, 0);
  repi(i, n) repi(j, n) indeg[j] += g[i][j], outdeg[i] += g[i][j];
  const int m = SUM(indeg);
  if (m == 0)
    return 1;
  int u = -1, v = -1;
  repi(i, n)
  {
    if (indeg[i] - outdeg[i] == 1)
    {
      if (u != -1)
        return 0;
      u = i;
    }
    else if (indeg[i] - outdeg[i] == -1)
    {
      if (v != -1)
        return 0;
      v = i;
    }
    else
    {
      if (indeg[i] - outdeg[i] != 0)
        return 0;
    }
  }
  if (u == -1 && v == -1)
    return count_eularian_circuits<mint>(g) * m;
  else if (u != -1 && v != -1)
  {
    auto h = g;
    h[u][v]++;
    return count_eularian_circuits<mint>(h);
  }
  else
    return 0;
}
