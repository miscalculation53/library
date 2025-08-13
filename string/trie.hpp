#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief trie
 * @docs docs/string/trie.md
 */

// 文字列は最初から [0, CHAR_NUM) の vector に変換して考える
template <int CHAR_NUM>
struct Trie
{
private:
  vc<int> par;
  vc<array<int, CHAR_NUM>> chi;

public:
  Trie()
  {
    par = {-1};
    chi = {{}};
    fill(ALL(chi[0]), -1);
  }

  void reserve(int len) { par.reserve(len), chi.reserve(len); }

  // 頂点数を返す
  template <class I = ll>
  I num_of_vertices() const { return par.size(); }

  // 頂点 v の親 (なければ -1) を返す
  template <class I = ll>
  I parent(int v) const
  {
    assert(0 <= v && v < num_of_vertices());
    return par[v];
  }
  // 頂点 v の文字 c 方向への子 (なければ -1) を返す
  template <class I = ll>
  I child(int v, int c) const
  {
    assert(0 <= v && v < num_of_vertices());
    assert(0 <= c && c < CHAR_NUM);
    return chi[v][c];
  }

  // 頂点 v (デフォルトでは根) から始めて s を挿入
  // 途中で訪れた頂点列を返す (長さ |s| + 1, 最初は v)
  // もともと s があるなら中身は変化しない (s に行くための関数として使える)
  template <class I = ll, class C>
  vc<I> insert(const vc<C> &s, int v = 0)
  {
    assert(0 <= v && v < num_of_vertices());
    vc<I> res = {v};
    res.reserve(s.size() + 1);
    fe(c : s)
    {
      assert(0 <= c && c < CHAR_NUM);
      if (chi[v][c] == -1)
      {
        chi[v][c] = num_of_vertices();
        par.eb(v);
        chi.eb();
        fill(ALL(chi.back()), -1);
      }
      v = chi[v][c];
      res.eb(v);
    }
    return res;
  }

  // 頂点 v が表す文字列 (vector) を返す
  template <class C = ll>
  vc<C> to_string(int v)
  {
    assert(0 <= v && v < num_of_vertices());
    vc<C> res;
    while (v != 0)
    {
      int p = par[v];
      repi(c, CHAR_NUM) if (chi[p][c] == v) res.eb(c);
      v = p;
    }
    return reversed(res);
  }
};
