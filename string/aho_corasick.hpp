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
  vc<array<int, CHAR_NUM>> nxt;
  vc<int> fail;

public:
  Trie<CHAR_NUM> trie;
  AhoCorasick() {}
  template <class C>
  AhoCorasick(const vvc<C> &strs)
  {
    fec(s : strs) trie.insert(s);
    n = trie.num_of_vertices();
    fail.assign(n, -1);
    nxt.resize(n);
    MyQueue<int> que;
    que.push(0);
    while (!que.empty())
    {
      int p = que.front();
      que.pop();

      repi(c, CHAR_NUM)
      {
        int v = trie.child(p, c);
        if (v != -1)
        {
          nxt[p][c] = v;
          fail[v] = p == 0 ? 0 : nxt[fail[p]][c];
          que.push(v);
        }
        else
        {
          nxt[p][c] = p == 0 ? 0 : nxt[fail[p]][c];
        }
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
