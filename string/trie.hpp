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
  struct ChildRow : array<int, CHAR_NUM>
  {
    ChildRow() { this->fill(-1); }
  };
  vc<ChildRow> chi;

  template <class C, class Emit>
  int insert_impl(const vc<C> &s, int v, const Emit &emit)
  {
    assert(0 <= v && v < num_of_vertices());
    emit(v);
    auto it = s.begin();
    for (; it != s.end(); ++it)
    {
      auto c = *it;
      assert(0 <= c && c < CHAR_NUM);
      int next = chi[v][c];
      if (next == -1) break;
      v = next;
      emit(v);
    }
    if (it != s.end())
    {
      int required = num_of_vertices<int>() + (s.end() - it);
      if (required > int(chi.capacity()))
      {
        int capacity = chi.capacity();
        while (capacity < required) capacity *= 2;
        reserve(capacity);
      }
      for (; it != s.end(); ++it)
      {
        auto c = *it;
        assert(0 <= c && c < CHAR_NUM);
        int next = num_of_vertices<int>();
        chi[v][c] = next;
        par.eb(v);
        chi.eb();
        v = next;
        emit(v);
      }
    }
    return v;
  }

public:
  Trie()
  {
    par = {-1};
    chi.resize(1);
  }

  void reserve(int len) { par.reserve(len), chi.reserve(len); }

  // 頂点数を返す
  template <class I = ll>
  I num_of_vertices() const { return par.size(); }

  // 頂点 v の親を返す。根では nullopt。
  template <class I = ll>
  optional<I> parent(int v) const
  {
    assert(0 <= v && v < num_of_vertices());
    if (par[v] == -1) return nullopt;
    return I(par[v]);
  }
  // 頂点 v の文字 c 方向への子を返す。存在しなければ nullopt。
  template <class I = ll>
  optional<I> child(int v, int c) const
  {
    assert(0 <= v && v < num_of_vertices());
    assert(0 <= c && c < CHAR_NUM);
    if (chi[v][c] == -1) return nullopt;
    return I(chi[v][c]);
  }

  // 頂点 v (デフォルトでは根) から始めて s を挿入
  // 途中で訪れた頂点列を返す (長さ |s| + 1, 最初は v)
  // もともと s があるなら中身は変化しない (s に行くための関数として使える)
  template <class I = ll, class C>
  vc<I> insert(const vc<C> &s, int v = 0)
  {
    vc<I> res;
    res.reserve(s.size() + 1);
    insert_impl(s, v, [&](int u) { res.eb(u); });
    return res;
  }

  // 挿入後の終点だけを返す。
  template <class I = ll, class C>
  I insert_last(const vc<C> &s, int v = 0)
  {
    return insert_impl(s, v, [](int) {});
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
      repi(c, CHAR_NUM) if (chi[p][c] == v)
      {
        res.eb(c);
        break;
      }
      v = p;
    }
    reverse(ALL(res));
    return res;
  }
};
