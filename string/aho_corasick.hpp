#pragma once

#include "../template/template_all_but_modint.hpp"
#include "trie.hpp"
#include "../ds/my_queue.hpp"

/**
 * @brief Aho Corasick
 * @docs docs/string/aho_corasick.md
 */

template <int CHAR_NUM>
struct AhoCorasick
{
private:
  int n;
  struct TransitionRow : array<int, CHAR_NUM>
  {
    // BFS writes every entry before this row is used.
    TransitionRow() {}
  };
  vc<TransitionRow> nxt;
  vc<int> fail;

public:
  Trie<CHAR_NUM> trie;
  AhoCorasick() {}
  template <class C>
  AhoCorasick(const vvc<C> &strs)
  {
    fec(s : strs) trie.template insert_last<int>(s);
    n = trie.num_of_vertices();
    fail.assign(n, -1);
    nxt.resize(n);
    MyQueue<int> que;
    que.reserve(n);
    repi(c, CHAR_NUM)
    {
      int v = trie.template child<int>(0, c);
      nxt[0][c] = v == -1 ? 0 : v;
      if (v != -1)
      {
        fail[v] = 0;
        que.push(v);
      }
    }
    while (!que.empty())
    {
      int p = que.front();
      que.pop();
      const auto &fallback = nxt[fail[p]];
      repi(c, CHAR_NUM)
      {
        int v = trie.template child<int>(p, c);
        if (v != -1)
        {
          nxt[p][c] = v;
          fail[v] = fallback[c];
          que.push(v);
        }
        else nxt[p][c] = fallback[c];
      }
    }
  }

  // v の failure link の行き先 (根 0 に対しては -1)
  // これは v の最長 suffix
  template <class I = ll>
  I failure(int v)
  {
    assert(0 <= v && v < n);
    return fail[v];
  }

  // 文字 c を追加できない限り failure link を辿る → 初めて追加できたときにいる頂点
  template <class I = ll>
  I next(int v, int c)
  {
    assert(0 <= v && v < n);
    assert(0 <= c && c < CHAR_NUM);
    return nxt[v][c];
  }
};
