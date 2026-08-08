#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief Cartesian Tree
 * @docs docs/graph/tree/cartesian_tree.md
 */

struct CartesianTree
{
private:
  int n, root_;
  struct Node
  {
    // par: 親の頂点番号
    // chi_l: 左の子の頂点番号 (ない場合 -1)
    // chi_r: 右の子の頂点番号 (ない場合 -1)
    // seg_l, seg_r: 表す区間 [l, r)
    // i が argmin となる [l, r) の条件: seg_l[i] <= l <= i and i < r <= seg_r[i]

    int par, chi_l, chi_r, seg_l, seg_r;
    Node() {}
    Node(int par, int chi_l, int chi_r, int seg_l, int seg_r)
    : par(par), chi_l(chi_l), chi_r(chi_r), seg_l(seg_l), seg_r(seg_r) {}
  };
  vector<Node> nodes;

  template <class I = ll>
  struct ChildList
  {
    I data[2];
    int sz = 0;
    const I *begin() const { return data; }
    const I *end() const { return data + sz; }
  };

public:
  CartesianTree() {}
  template <class T, class Compare = less<T>>
  CartesianTree(const vector<T> &a, Compare comp = Compare{}) : n(a.size()), nodes(n, Node(-1, -1, -1, -1, n))
  {
    vc<int> sta;  // stack には「右に伸びているやつ」を持つ
    repi(i, n)
    {
      int p = -1, c = -1;  // p: i の親、c: i の子
      while (!sta.empty())
      {
        int j = sta.back();
        if (comp(a[j], a[i]))
        {
          p = j;
          break;
        }
        c = j;
        sta.pop_back();
        nodes[c].seg_r = i;
      }
      sta.eb(i);
      nodes[i].seg_l = p + 1;
      if (p == -1)
        root_ = i;
      else
        nodes[i].par = p, nodes[p].chi_r = i;
      if (c != -1)
        nodes[c].par = i, nodes[i].chi_l = c;
    }
  }

  template <class I = ll>
  I root() const { return root_; }
  // 存在しなければ -1
  template <class I = ll>
  I parent(int i) const
  {
    assert(0 <= i && i < n);
    return nodes[i].par;
  }
  // 存在しなければ -1
  template <class I = ll>
  I left_child(int i) const
  {
    assert(0 <= i && i < n);
    return nodes[i].chi_l;
  }
  // 存在しなければ -1
  template <class I = ll>
  I right_child(int i) const
  {
    assert(0 <= i && i < n);
    return nodes[i].chi_r;
  }
  // 存在する子をイテレートする
  template <class I = ll>
  ChildList<I> children(int i) const
  {
    assert(0 <= i && i < n);
    ChildList<I> res;
    if (nodes[i].chi_l != -1) res.data[res.sz++] = nodes[i].chi_l;
    if (nodes[i].chi_r != -1) res.data[res.sz++] = nodes[i].chi_r;
    return res;
  }

  // i が argmin となる [l, r) の条件: seg_l[i] <= l <= i and i < r <= seg_r[i]
  template <class I = ll>
  pair<I, I> range(int i) const
  {
    assert(0 <= i && i < n);
    return {nodes[i].seg_l, nodes[i].seg_r};
  }

  auto content() const { return nodes; }
};
#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(par, chi_l, chi_r, seg_l, seg_r);
#endif
