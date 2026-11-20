#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "../../algebra/monoid_power.hpp"

namespace internal_sparse_interval_tree
{
template <class A, bool WithProduct>
struct Tree
{
  using S = typename A::S;
  using F = typename A::F;

protected:
  struct EmptyProduct {};
  using Product = conditional_t<WithProduct, S, EmptyProduct>;

  struct Entry
  {
    ll l, r;
    S unit;
    [[no_unique_address]] Product val;

    Entry(ll l, ll r, const S &unit) : l(l), r(r), unit(unit), val(make_product(unit, r - l)) {}

    void set_right(ll right)
    {
      r = right;
      if constexpr (WithProduct) val = pow_monoid<A>(unit, r - l);
    }

    void apply(const F &f)
    {
      unit = A::mapping(f, unit);
      if constexpr (WithProduct) val = A::mapping(f, val);
    }

    CPP_DUMP_DEFINE_DATA(l, r, unit);

  private:
    static Product make_product(const S &unit, ll len)
    {
      if constexpr (WithProduct) return pow_monoid<A>(unit, len);
      else return {};
    }
  };

  struct Node
  {
    Entry entry;
    [[no_unique_address]] Product prod;
    F lazy;
    ll lo, hi;
    array<int, 2> chi;
    bool has_lazy;
    Node(const Entry &entry)
        : entry(entry), prod(entry.val), lazy(A::id()), lo(entry.l), hi(entry.r), chi{-1, -1}, has_lazy(false) {}
    CPP_DUMP_DEFINE_DATA(entry, lazy, lo, hi, chi, has_lazy);
  };

  ll n;
  vc<Node> nodes;

private:
  int make_node(const Entry &entry)
  {
    int i = nodes.size();
    nodes.eb(entry);
    return i;
  }

  void all_apply(int i, const F &f)
  {
    Node &v = nodes[i];
    v.entry.apply(f);
    if constexpr (WithProduct) v.prod = A::mapping(f, v.prod);
    if (v.has_lazy)
      v.lazy = A::composition(f, v.lazy);
    else
    {
      v.lazy = f;
      v.has_lazy = true;
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
    nodes[i].lazy = A::id();
    nodes[i].has_lazy = false;
  }

  void pull(int i)
  {
    int l = nodes[i].chi[0], r = nodes[i].chi[1];
    if constexpr (WithProduct)
    {
      S sml = l == -1 ? A::e() : nodes[l].prod;
      S smr = r == -1 ? A::e() : nodes[r].prod;
      nodes[i].prod = A::op(A::op(sml, nodes[i].entry.val), smr);
    }
    nodes[i].lo = l == -1 ? nodes[i].entry.l : nodes[l].lo;
    nodes[i].hi = r == -1 ? nodes[i].entry.r : nodes[r].hi;
  }

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
    modify_entry(l, [&](Entry &x) { x.set_right(p); }, 0);
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
      cuts[0].emplace(l, min(r, x.r), A::mapping(f, x.unit));
      if (r < x.r) cuts[1].emplace(r, x.r, x.unit);
      x.set_right(l);
    }
    else if (l <= x.l && x.l < r)
    {
      if (r < x.r)
      {
        cuts[1].emplace(r, x.r, x.unit);
        x.set_right(r);
      }
      x.apply(f);
    }
    ll c = a + (b - a) / 2;
    int li = nodes[i].chi[0], ri = nodes[i].chi[1];
    if (li != -1) apply_impl(l, r, f, a, c, li, cuts);
    if (ri != -1) apply_impl(l, r, f, c, b, ri, cuts);
    pull(i);
    for (auto &cut : cuts)
      if (cut && max(a, lo) <= cut->l && cut->l < min(b, hi))
      {
        insert(std::move(*cut), a, b, i);
        cut.reset();
      }
  }

public:
  Tree(ll n, const S &initial_value) : n(n)
  {
    assert(n >= 0);
    nodes.eb(Entry(0, n, initial_value));
  }

  void reserve(int count) { nodes.reserve(count); }

  template <class G>
  void modify(ll p, const G &g)
  {
    assert(0 <= p && p < n);
    split(p), split(p + 1);
    modify_entry(p, [&](Entry &x)
    {
      g(x.unit);
      if constexpr (WithProduct) x.val = x.unit;
    }, 0);
  }

  void set(ll p, const S &x) { modify(p, [&](S &y) { y = x; }); }

  S get(ll p) const
  {
    assert(0 <= p && p < n);
    int i = 0;
    F f = A::id();
    while (true)
    {
      if (nodes[i].entry.l <= p && p < nodes[i].entry.r)
        return A::mapping(f, nodes[i].entry.unit);
      F nf = nodes[i].has_lazy ? A::composition(f, nodes[i].lazy) : f;
      i = nodes[i].chi[p < nodes[i].entry.l ? 0 : 1];
      f = std::move(nf);
    }
  }

  void apply(ll p, const F &f)
  {
    assert(0 <= p && p < n);
    apply(p, p + 1, f);
  }

  void apply(ll l, ll r, const F &f)
  {
    assert(0 <= l && l <= r && r <= n);
    if (l == r) return;
    array<optional<Entry>, 2> cuts;
    apply_impl(l, r, f, 0, n, 0, cuts);
  }

  vc<tuple<ll, ll, S>> content() const
  {
    vc<tuple<ll, ll, S>> res;
    if (n == 0) return res;
    res.reserve(nodes.size());
    auto dfs = [&](auto dfs, int i, const F &f) -> void
    {
      if (i == -1) return;
      const Node &x = nodes[i];
      F nf = x.has_lazy ? A::composition(f, x.lazy) : f;
      dfs(dfs, x.chi[0], nf);
      res.eb(x.entry.l, x.entry.r, A::mapping(f, x.entry.unit));
      dfs(dfs, x.chi[1], nf);
    };
    dfs(dfs, 0, A::id());
    return res;
  }
};
}
