#pragma once

#include "graph.hpp"
#include "tree/rooted_tree.hpp"
#include "../utils/is_integral_ext.hpp"

/**
 * @brief グラフ・木の可視化（GRAPH × GRAPH）
 * @docs docs/graph/visualize.md
 */

namespace graph_visualize_detail
{
  inline string encode(const string &text)
  {
    constexpr char hex[] = "0123456789ABCDEF";
    string res;
    for (unsigned char c : text)
    {
      if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ||
          ('0' <= c && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~')
        res += c;
      else
      {
        res += '%';
        res += hex[c >> 4];
        res += hex[c & 15];
      }
    }
    return res;
  }

  // GRAPH × GRAPH の URL Export と同じパラメータを使う。
  inline string url(const string &text, bool directed, bool weighted, bool one_indexed)
  {
    return string("https://hello-world-494ec.firebaseapp.com/?format=normal&indexed=") +
        (one_indexed ? "true" : "false") + "&weighted=" + (weighted ? "true" : "false") +
        "&directed=" + (directed ? "true" : "false") + "&data=" + encode(text);
  }
}

// N M に続けて辺を1行ずつ出力する。無向辺は1回だけ出力する。
template <bool directed, class Cost, bool erasable>
string graph_text(const Graph<directed, Cost, erasable> &g, bool one_indexed = false)
{
  ostringstream out;
  out.imbue(locale::classic());
  if constexpr (is_floating_point_v<Cost>)
    out << setprecision(numeric_limits<Cost>::max_digits10);
  out << g.size() << ' ' << g.num_of_edges() << '\n';
  repi(v, g.size()) fec(e : g.out_edges(v))
  {
    if constexpr (!directed) if (e.from > e.to) continue;
    out << ll(e.from) + one_indexed << ' ' << ll(e.to) + one_indexed;
    if constexpr (!is_void_v<Cost>)
    {
      out << ' ';
      if constexpr (is_integral_ext<Cost>) out << +e.cost;
      else out << e.cost;
    }
    out << '\n';
  }
  return out.str();
}

// 親配列から親 -> 子の辺を作る。負の値または自分自身を親に持つ頂点は根。
template <class I>
string graph_text(const vc<I> &par, bool one_indexed = false)
{
  static_assert(is_integral_ext<I>);
  const int n = par.size();
  using Index = common_type_t<I, int>;
  ostringstream edges;
  edges.imbue(locale::classic());
  int m = 0;
  repi(v, n)
  {
    Index p = par[v];
    if constexpr (is_signed_ext<Index>) if (p < 0) continue;
    if (p == Index(v)) continue;
    assert(p < Index(n));
    edges << ll(p) + one_indexed << ' ' << ll(v) + one_indexed << '\n';
    ++m;
  }
  return to_string(n) + " " + to_string(m) + "\n" + edges.str();
}

inline string graph_text(const RootedTree &tree, bool one_indexed = false)
{
  return graph_text(tree.parents(), one_indexed);
}

template <bool directed, class Cost, bool erasable>
string graph_url(const Graph<directed, Cost, erasable> &g, bool one_indexed = false)
{
  return graph_visualize_detail::url(graph_text(g, one_indexed), directed, !is_void_v<Cost>, one_indexed);
}

template <class I>
string graph_url(const vc<I> &par, bool one_indexed = false)
{
  return graph_visualize_detail::url(graph_text(par, one_indexed), true, false, one_indexed);
}

inline string graph_url(const RootedTree &tree, bool one_indexed = false)
{
  return graph_url(tree.parents(), one_indexed);
}

#ifdef LOCAL
#define dump_graph(...) ((void)(cerr << "[graph] " #__VA_ARGS__ "\n" << graph_url(__VA_ARGS__) << '\n'))
#else
#define dump_graph(...) ((void)0)
#endif
