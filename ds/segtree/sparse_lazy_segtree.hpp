#pragma once

#include "internal_sparse_interval_tree.hpp"
#include "../internal_dump_content.hpp"

/**
 * @brief 一様な区間をまとめて保持する疎遅延セグメント木
 * @docs docs/ds/segtree/sparse_lazy_segtree.md
 */

template <class AM>
struct SparseLazySegmentTree : private internal_sparse_interval_tree::Tree<AM, true>
{
  using S = typename AM::S;
  using F = typename AM::F;

private:
  using Base = internal_sparse_interval_tree::Tree<AM, true>;
  using typename Base::Entry;
  using typename Base::Node;
  using Base::n;
  using Base::nodes;

  S entry_prod(int i, ll l, ll r, const F &f) const
  {
    const Entry &x = nodes[i].entry;
    ll a = max(l, x.l), b = min(r, x.r);
    if (b <= a) return AM::e();
    return AM::mapping(f, a == x.l && b == x.r ? x.val : pow_monoid<AM>(x.unit, b - a));
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
  SparseLazySegmentTree(ll n, const S &initial_value) : Base(n, initial_value) {}

  // 少なくとも n ノード分の領域をあらかじめ確保する
  void reserve(int n) { Base::reserve(n); }

  // p 番目の値を参照で受け取る関数 g を適用する
  template <class G>
  void modify(ll p, const G &g) { Base::modify(p, g); }

  // p 番目の値を x にする
  void set(ll p, const S &x) { Base::set(p, x); }

  // p 番目の値を返す
  S get(ll p) const { return Base::get(p); }

  // p 番目に作用 f を適用する
  void apply(ll p, const F &f) { Base::apply(p, f); }

  // [l, r) に作用 f を適用する
  void apply(ll l, ll r, const F &f) { Base::apply(l, r, f); }

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
  vc<tuple<ll, ll, S>> content() const { return Base::content(); }

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

}
#endif
