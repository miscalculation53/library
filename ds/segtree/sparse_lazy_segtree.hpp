#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "../internal_dump_content.hpp"

/**
 * @brief 一様な区間をまとめて保持する疎遅延セグメント木
 * @docs docs/ds/segtree/sparse_lazy_segtree.md
 */

template <class AM>
struct SparseLazySegmentTree
{
  using S = typename AM::S;
  using F = typename AM::F;

private:
  template <class A, class = void>
  struct HasPow : false_type {};
  template <class A>
  struct HasPow<A, void_t<decltype(A::pow(declval<const S &>(), ll{}))>> : true_type {};

  static S repeat(S x, ll len)
  {
    if constexpr (HasPow<AM>::value)
      return AM::pow(x, len);
    else
    {
      S res = AM::e();
      while (len)
      {
        if (len & 1) res = AM::op(res, x);
        len >>= 1;
        if (len) x = AM::op(x, x);
      }
      return res;
    }
  }

  struct Entry
  {
    ll l, r;
    S unit, val;
    Entry(ll l, ll r, const S &unit) : l(l), r(r), unit(unit), val(repeat(unit, r - l)) {}
  };
  struct Node
  {
    Entry entry;
    S prod;
    F lazy;
    ll lo, hi;
    array<int, 2> chi;
    bool has_lazy;
    Node(const Entry &entry)
        : entry(entry), prod(entry.val), lazy(AM::id()), lo(entry.l), hi(entry.r), chi{-1, -1}, has_lazy(false) {}
  };
  ll n;
  vc<Node> nodes;

  int make_node(const Entry &entry)
  {
    int i = nodes.size();
    nodes.eb(entry);
    return i;
  }

  void all_apply(int i, const F &f)
  {
    nodes[i].entry.unit = AM::mapping(f, nodes[i].entry.unit);
    nodes[i].entry.val = AM::mapping(f, nodes[i].entry.val);
    nodes[i].prod = AM::mapping(f, nodes[i].prod);
    if (nodes[i].has_lazy)
      nodes[i].lazy = AM::composition(f, nodes[i].lazy);
    else
    {
      nodes[i].lazy = f;
      nodes[i].has_lazy = true;
    }
  }

  void push(int i)
  {
    if (!nodes[i].has_lazy) return;
    F f = nodes[i].lazy;
    repi(d, 2)
    {
      int j = nodes[i].chi[d];
      if (j != -1) all_apply(j, f);
    }
    nodes[i].lazy = AM::id();
    nodes[i].has_lazy = false;
  }

  void pull(int i)
  {
    int l = nodes[i].chi[0], r = nodes[i].chi[1];
    S sml = l == -1 ? AM::e() : nodes[l].prod;
    S smr = r == -1 ? AM::e() : nodes[r].prod;
    nodes[i].prod = AM::op(AM::op(sml, nodes[i].entry.val), smr);
    nodes[i].lo = l == -1 ? nodes[i].entry.l : nodes[l].lo;
    nodes[i].hi = r == -1 ? nodes[i].entry.r : nodes[r].hi;
  }

  // 各ノードに一点を置く疎セグ木と同じ配置で、区間の左端をキーにする。
  void insert(Entry x, ll a, ll b, int i)
  {
    push(i);
    ll c = a + (b - a) / 2;
    int d = x.l < c ? 0 : 1;
    if ((d == 0 && nodes[i].entry.l < x.l) || (d == 1 && x.l < nodes[i].entry.l))
      swap(x, nodes[i].entry);
    int j = nodes[i].chi[d];
    if (j == -1)
    {
      j = make_node(x);
      nodes[i].chi[d] = j;
    }
    else if (d == 0)
      insert(std::move(x), a, c, j);
    else
      insert(std::move(x), c, b, j);
    pull(i);
  }

  template <class G>
  void modify_entry(ll p, const G &g, int i)
  {
    push(i);
    if (nodes[i].entry.l == p)
      g(nodes[i].entry);
    else
      modify_entry(p, g, nodes[i].chi[p < nodes[i].entry.l ? 0 : 1]);
    pull(i);
  }

  void split(ll p)
  {
    if (p == 0 || p == n) return;
    int i = 0, prev = -1;
    while (i != -1)
    {
      push(i);
      if (nodes[i].entry.l == p) return;
      if (nodes[i].entry.l < p)
        prev = i, i = nodes[i].chi[1];
      else
        i = nodes[i].chi[0];
    }
    ll l = nodes[prev].entry.l, r = nodes[prev].entry.r;
    S unit = nodes[prev].entry.unit;
    modify_entry(l, [&](Entry &x)
    {
      x.r = p;
      x.val = repeat(x.unit, p - l);
    }, 0);
    insert(Entry(p, r, unit), 0, n, 0);
  }

  void apply_impl(ll l, ll r, const F &f, ll a, ll b, int i, array<optional<Entry>, 2> &cuts)
  {
    if (nodes[i].hi <= l || r <= nodes[i].lo) return;
    if (l <= nodes[i].lo && nodes[i].hi <= r)
    {
      all_apply(i, f);
      return;
    }
    push(i);
    ll lo = nodes[i].lo, hi = nodes[i].hi;
    Entry &x = nodes[i].entry;
    if (x.l < l && l < x.r)
    {
      cuts[0].emplace(l, min(r, x.r), AM::mapping(f, x.unit));
      if (r < x.r) cuts[1].emplace(r, x.r, x.unit);
      x.r = l;
      x.val = repeat(x.unit, x.r - x.l);
    }
    else if (l <= x.l && x.l < r)
    {
      if (r < x.r)
      {
        cuts[1].emplace(r, x.r, x.unit);
        x.r = r;
        x.val = repeat(x.unit, x.r - x.l);
      }
      x.unit = AM::mapping(f, x.unit);
      x.val = AM::mapping(f, x.val);
    }
    ll c = a + (b - a) / 2;
    int li = nodes[i].chi[0], ri = nodes[i].chi[1];
    if (li != -1) apply_impl(l, r, f, a, c, li, cuts);
    if (ri != -1) apply_impl(l, r, f, c, b, ri, cuts);
    pull(i);
    // 元の部分木に属する新しい区間を、キーの座標範囲も満たす最深の祖先で挿入する。
    for (auto &cut : cuts)
      if (cut && max(a, lo) <= cut->l && cut->l < min(b, hi))
      {
        insert(std::move(*cut), a, b, i);
        cut.reset();
      }
  }

  S entry_prod(int i, ll l, ll r, const F &f) const
  {
    const Entry &x = nodes[i].entry;
    ll a = max(l, x.l), b = min(r, x.r);
    if (b <= a) return AM::e();
    return AM::mapping(f, a == x.l && b == x.r ? x.val : repeat(x.unit, b - a));
  }

  S prod_impl(ll l, ll r, int i, const F &f) const
  {
    if (nodes[i].hi <= l || r <= nodes[i].lo) return AM::e();
    if (l <= nodes[i].lo && nodes[i].hi <= r)
      return AM::mapping(f, nodes[i].prod);
    F nf = nodes[i].has_lazy ? AM::composition(f, nodes[i].lazy) : f;
    int li = nodes[i].chi[0], ri = nodes[i].chi[1];
    S sml = li == -1 ? AM::e() : prod_impl(l, r, li, nf);
    S smm = entry_prod(i, l, r, f);
    S smr = ri == -1 ? AM::e() : prod_impl(l, r, ri, nf);
    return AM::op(AM::op(sml, smm), smr);
  }

  template <class G>
  ll search_entry(ll l, ll r, S unit, S &sm, const G &g, bool from_right) const
  {
    vc<S> powers{std::move(unit)};
    ll len = r - l, step = 1;
    while (step <= len / 2)
    {
      powers.eb(AM::op(powers.back(), powers.back()));
      step *= 2;
    }
    ll pos = from_right ? r : l;
    repi(k, SZ(powers) - 1, -1, -1)
    {
      step = 1LL << k;
      if (step > (from_right ? pos - l : r - pos)) continue;
      S x = from_right ? AM::op(powers[k], sm) : AM::op(sm, powers[k]);
      if (g(x))
      {
        sm = std::move(x);
        pos += from_right ? -step : step;
      }
    }
    return pos;
  }

  template <class G>
  ll max_right_bound(ll l, const G &g) const
  {
    assert(0 <= l && l <= n);
    assert(g(AM::e()));
    if (l == n) return n;
    S sm = AM::e();
    auto dfs = [&](auto dfs, int i, const F &f) -> ll
    {
      if (nodes[i].hi <= l) return -1;
      if (l <= nodes[i].lo)
      {
        S x = AM::op(sm, AM::mapping(f, nodes[i].prod));
        if (g(x))
        {
          sm = std::move(x);
          return -1;
        }
      }
      F nf = nodes[i].has_lazy ? AM::composition(f, nodes[i].lazy) : f;
      int li = nodes[i].chi[0], ri = nodes[i].chi[1];
      ll res = li == -1 ? -1 : dfs(dfs, li, nf);
      if (res != -1) return res;
      ll a = max(l, nodes[i].entry.l), b = nodes[i].entry.r;
      if (a < b)
      {
        S x = AM::op(sm, entry_prod(i, a, b, f));
        if (!g(x))
          return search_entry(a, b, AM::mapping(f, nodes[i].entry.unit), sm, g, false);
        sm = std::move(x);
      }
      return ri == -1 ? -1 : dfs(dfs, ri, nf);
    };
    ll res = dfs(dfs, 0, AM::id());
    return res == -1 ? n : res;
  }

  template <class G>
  ll min_left_bound(ll r, const G &g) const
  {
    assert(0 <= r && r <= n);
    assert(g(AM::e()));
    if (r == 0) return 0;
    S sm = AM::e();
    auto dfs = [&](auto dfs, int i, const F &f) -> ll
    {
      if (r <= nodes[i].lo) return -1;
      if (nodes[i].hi <= r)
      {
        S x = AM::op(AM::mapping(f, nodes[i].prod), sm);
        if (g(x))
        {
          sm = std::move(x);
          return -1;
        }
      }
      F nf = nodes[i].has_lazy ? AM::composition(f, nodes[i].lazy) : f;
      int li = nodes[i].chi[0], ri = nodes[i].chi[1];
      ll res = ri == -1 ? -1 : dfs(dfs, ri, nf);
      if (res != -1) return res;
      ll a = nodes[i].entry.l, b = min(r, nodes[i].entry.r);
      if (a < b)
      {
        S x = AM::op(entry_prod(i, a, b, f), sm);
        if (!g(x))
          return search_entry(a, b, AM::mapping(f, nodes[i].entry.unit), sm, g, true);
        sm = std::move(x);
      }
      return li == -1 ? -1 : dfs(dfs, li, nf);
    };
    ll res = dfs(dfs, 0, AM::id());
    return res == -1 ? 0 : res;
  }

public:
  // 長さ n、各点の初期値 initial_value の列を作る
  SparseLazySegmentTree(ll n, const S &initial_value) : n(n)
  {
    assert(n >= 0);
    nodes.eb(Entry(0, n, initial_value));
  }

  // 少なくとも n ノード分の領域をあらかじめ確保する
  void reserve(int n) { nodes.reserve(n); }

  // p 番目の値を参照で受け取る関数 g を適用する
  template <class G>
  void modify(ll p, const G &g)
  {
    assert(0 <= p && p < n);
    split(p), split(p + 1);
    modify_entry(p, [&](Entry &x) { g(x.unit); x.val = x.unit; }, 0);
  }

  // p 番目の値を x にする
  void set(ll p, const S &x) { modify(p, [&](S &y) { y = x; }); }

  // p 番目の値を返す
  S get(ll p) const
  {
    assert(0 <= p && p < n);
    int i = 0;
    F f = AM::id();
    while (true)
    {
      if (nodes[i].entry.l <= p && p < nodes[i].entry.r)
        return AM::mapping(f, nodes[i].entry.unit);
      F nf = nodes[i].has_lazy ? AM::composition(f, nodes[i].lazy) : f;
      i = nodes[i].chi[p < nodes[i].entry.l ? 0 : 1];
      f = std::move(nf);
    }
  }

  // p 番目に作用 f を適用する
  void apply(ll p, const F &f)
  {
    assert(0 <= p && p < n);
    apply(p, p + 1, f);
  }

  // [l, r) に作用 f を適用する
  void apply(ll l, ll r, const F &f)
  {
    assert(0 <= l && l <= r && r <= n);
    if (l == r) return;
    // 分割と作用を同じ走査で処理し、新しい区間は対応する部分木の更新後に挿入する。
    array<optional<Entry>, 2> cuts;
    apply_impl(l, r, f, 0, n, 0, cuts);
  }

  // [l, r) の積を返す
  S prod(ll l, ll r) const
  {
    assert(0 <= l && l <= r && r <= n);
    if (l == r) return AM::e();
    return prod_impl(l, r, 0, AM::id());
  }

  // 全体の積を返す
  S all_prod() const { return nodes[0].prod; }

  // i = l, ..., r-1 について x ← apply(x, get(i))
  template <class X, class G>
  X prod_apply(ll l, ll r, X x, G &&apply) const
  {
    assert(0 <= l && l <= r && r <= n);
    auto dfs = [&](auto &&self, int i, const F &f) -> void
    {
      const Node &v = nodes[i];
      if (v.hi <= l || r <= v.lo) return;
      if (l <= v.lo && v.hi <= r)
      {
        const S value = AM::mapping(f, v.prod);
        x = apply(x, value);
        return;
      }
      F nf = v.has_lazy ? AM::composition(f, v.lazy) : f;
      if (v.chi[0] != -1) self(self, v.chi[0], nf);
      if (max(l, v.entry.l) < min(r, v.entry.r))
      {
        const S value = entry_prod(i, l, r, f);
        x = apply(x, value);
      }
      if (v.chi[1] != -1) self(self, v.chi[1], nf);
    };
    if (l < r) dfs(dfs, 0, AM::id());
    return x;
  }

  // 保持している区間を (左端, 右端, 一点の値) の順で返す
  vc<tuple<ll, ll, S>> content() const
  {
    vc<tuple<ll, ll, S>> res;
    if (n == 0) return res;
    res.reserve(nodes.size());
    auto dfs = [&](auto dfs, int i, const F &f) -> void
    {
      if (i == -1) return;
      const Node &x = nodes[i];
      F nf = x.has_lazy ? AM::composition(f, x.lazy) : f;
      dfs(dfs, x.chi[0], nf);
      res.eb(x.entry.l, x.entry.r, AM::mapping(f, x.entry.unit));
      dfs(dfs, x.chi[1], nf);
    };
    dfs(dfs, 0, AM::id());
    return res;
  }

  // g(prod(l, r)) が true となる最大の r を返す
  template <class G>
  ll max_right_ok(ll l, const G &g) const { return max_right_bound(l, g); }

  // g(prod(l, r)) が true となる最小の l を返す
  template <class G>
  ll min_left_ok(ll r, const G &g) const { return min_left_bound(r, g); }
};

#ifdef LOCAL
namespace cpp_dump::_detail
{
  template <class AM>
  inline string export_object_generic(
      const SparseLazySegmentTree<AM> &value, const string &indent,
      size_t last_line_length, size_t current_depth, bool fail_on_newline,
      const export_command &command)
  {
    return export_named_content<library_dump_detail::IntervalFields>(
        value, [&]() -> decltype(auto) { return (value.content()); },
        indent, last_line_length, current_depth, fail_on_newline, command);
  }

} // namespace cpp_dump::_detail
#endif
