#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "../../algebra/affine_function.hpp"

#include <optional>
#include <type_traits>
#include <utility>

namespace internal_sparse_point_fold {

template <class T, class = void>
struct has_equality : std::false_type {};
template <class T>
struct has_equality<T, std::void_t<decltype(static_cast<bool>(
    std::declval<const T&>() == std::declval<const T&>()))>> : std::true_type {};

// An optional predicate or an already available equality proves identity.
// Types without either operation retain the general initial-value path.
template <class M, class = void>
struct identity_value {
    static bool test(const typename M::S& value) {
        if constexpr (has_equality<typename M::S>::value) {
            const auto identity = M::e();
            return static_cast<bool>(value == identity);
        } else
            return false;
    }
};
template <class M>
struct identity_value<M, std::void_t<decltype(static_cast<bool>(
    M::is_identity(std::declval<const typename M::S&>())))>> {
    static bool test(const typename M::S& value) {
        return static_cast<bool>(M::is_identity(value));
    }
};

// These adapters use only the documented coefficient representation and e().
// Equality remains optional for the coefficient type as well.
template <class T>
struct identity_value<GroupAffineFunction<T>, void> {
    static bool test(const typename GroupAffineFunction<T>::S& value) {
        if constexpr (has_equality<T>::value) {
            const auto identity = GroupAffineFunction<T>::e();
            return static_cast<bool>(value.a == identity.a) &&
                   static_cast<bool>(value.b == identity.b);
        } else
            return false;
    }
};
template <class T>
struct identity_value<MonoidAffineFunction<T>, void> {
    static bool test(const typename MonoidAffineFunction<T>::S& value) {
        if constexpr (has_equality<T>::value) {
            const auto identity = MonoidAffineFunction<T>::e();
            return static_cast<bool>(value.b == identity.b) &&
                   static_cast<bool>(value.c == identity.c);
        } else
            return false;
    }
};
template <class M>
struct identity_value<OppositeMonoid<M>, void> : identity_value<M> {};
template <class M>
struct identity_value<OppositeGroup<M>, void> : identity_value<M> {};

template <class M>
class unit_avl {
public:
    using S = typename M::S;

private:
    struct Node {
        ll key;
        S own, aggregate, left_own, own_right;
        int left = -1, right = -1;
        unsigned char height = 1;
        Node(ll key, const S& value)
            : key(key), own(value), aggregate(value),
              left_own(value), own_right(value) {}
    };
    std::vector<Node> nodes;
    int root = -1, first = -1, last = -1;

    int height(int i) const { return i < 0 ? 0 : nodes[i].height; }
    void pull(int i) {
        Node& v = nodes[i];
        v.height = static_cast<unsigned char>(
            1 + std::max(height(v.left), height(v.right)));
        v.left_own = v.left < 0 ? v.own : M::op(nodes[v.left].aggregate, v.own);
        v.own_right = v.right < 0 ? v.own : M::op(v.own, nodes[v.right].aggregate);
        if (v.left < 0) v.aggregate = v.own_right;
        else if (v.right < 0) v.aggregate = v.left_own;
        else v.aggregate = M::op(v.left_own, nodes[v.right].aggregate);
    }
    int rotate_left(int i) {
        const int j = nodes[i].right;
        nodes[i].right = nodes[j].left;
        nodes[j].left = i;
        pull(i);
        pull(j);
        return j;
    }
    int rotate_right(int i) {
        const int j = nodes[i].left;
        nodes[i].left = nodes[j].right;
        nodes[j].right = i;
        pull(i);
        pull(j);
        return j;
    }
    int balance(int i) {
        pull(i);
        const int l = nodes[i].left, r = nodes[i].right;
        if (height(l) > height(r) + 1) {
            if (height(nodes[l].right) > height(nodes[l].left))
                nodes[i].left = rotate_left(l);
            return rotate_right(i);
        }
        if (height(r) > height(l) + 1) {
            if (height(nodes[r].left) > height(nodes[r].right))
                nodes[i].right = rotate_right(r);
            return rotate_left(i);
        }
        return i;
    }
    template <class F>
    int modify_impl(int i, ll p, const F& f) {
        if (i < 0) {
            S value = M::e();
            f(value);
            const int j = static_cast<int>(nodes.size());
            nodes.emplace_back(p, value);
            if (first < 0 || p < nodes[first].key) first = j;
            if (last < 0 || nodes[last].key < p) last = j;
            return j;
        }
        if (p < nodes[i].key) {
            const int child = modify_impl(nodes[i].left, p, f);
            nodes[i].left = child;
        } else if (nodes[i].key < p) {
            const int child = modify_impl(nodes[i].right, p, f);
            nodes[i].right = child;
        } else {
            f(nodes[i].own);
        }
        return balance(i);
    }
    std::optional<S> prefix(int i, ll r) const {
        std::optional<S> result;
        while (i >= 0) {
            const Node& v = nodes[i];
            if (v.key < r) {
                if (result) *result = M::op(*result, v.left_own);
                else result = v.left_own;
                i = v.right;
            } else i = v.left;
        }
        return result;
    }
    std::optional<S> suffix(int i, ll l) const {
        std::optional<S> result;
        while (i >= 0) {
            const Node& v = nodes[i];
            if (v.key < l) i = v.right;
            else {
                if (result) *result = M::op(v.own_right, *result);
                else result = v.own_right;
                i = v.left;
            }
        }
        return result;
    }
    template <class G>
    ll search_right(int i, ll l, bool whole, S& sum, const G& g) const {
        if (i < 0) return -1;
        const Node& v = nodes[i];
        if (!whole && v.key < l) return search_right(v.right, l, false, sum, g);
        if (whole) {
            S next = M::op(sum, v.aggregate);
            if (g(next)) {
                sum = std::move(next);
                return -1;
            }
        }
        const ll before = search_right(v.left, l, whole, sum, g);
        if (before >= 0) return before;
        S next = M::op(sum, v.own);
        if (!g(next)) return v.key;
        sum = std::move(next);
        return search_right(v.right, l, true, sum, g);
    }
    template <class G>
    ll search_left(int i, ll r, bool whole, S& sum, const G& g) const {
        if (i < 0) return -1;
        const Node& v = nodes[i];
        if (!whole && !(v.key < r)) return search_left(v.left, r, false, sum, g);
        if (whole) {
            S next = M::op(v.aggregate, sum);
            if (g(next)) {
                sum = std::move(next);
                return -1;
            }
        }
        const ll after = search_left(v.right, r, whole, sum, g);
        if (after >= 0) return after;
        S next = M::op(v.own, sum);
        if (!g(next)) return v.key + 1;
        sum = std::move(next);
        return search_left(v.left, r, true, sum, g);
    }
    void collect(int i, std::map<ll, S>& result) const {
        if (i < 0) return;
        collect(nodes[i].left, result);
        result.emplace_hint(result.end(), nodes[i].key, nodes[i].own);
        collect(nodes[i].right, result);
    }

public:
    void reserve(int count) { nodes.reserve(count); }
    template <class F>
    void modify(ll p, const F& f) { root = modify_impl(root, p, f); }
    S get(ll p) const {
        int i = root;
        while (i >= 0) {
            const Node& v = nodes[i];
            if (p < v.key) i = v.left;
            else if (v.key < p) i = v.right;
            else return v.own;
        }
        return M::e();
    }
    S prod(ll l, ll r) const {
        if (root < 0 || l == r) return M::e();
        if (!(nodes[first].key < r) || nodes[last].key < l) return M::e();
        const bool include_first = !(nodes[first].key < l);
        const bool include_last = nodes[last].key < r;
        if (include_first && include_last) return nodes[root].aggregate;
        if (include_first) return *prefix(root, r);
        if (include_last) return *suffix(root, l);
        int i = root;
        while (i >= 0) {
            const Node& v = nodes[i];
            if (v.key < l) i = v.right;
            else if (!(v.key < r)) i = v.left;
            else {
                S result = v.own;
                const auto before = suffix(v.left, l);
                const auto after = prefix(v.right, r);
                if (before) result = M::op(*before, result);
                if (after) result = M::op(result, *after);
                return result;
            }
        }
        return M::e();
    }
    S all_prod() const { return root < 0 ? M::e() : nodes[root].aggregate; }
    template <class G>
    ll max_right_ok(ll l, ll n, const G& g) const {
        S sum = M::e();
        const ll answer = search_right(root, l, first >= 0 && l <= nodes[first].key, sum, g);
        return answer < 0 ? n : answer;
    }
    template <class G>
    ll min_left_ok(ll r, const G& g) const {
        S sum = M::e();
        const ll answer = search_left(root, r, last >= 0 && nodes[last].key < r, sum, g);
        return answer < 0 ? 0 : answer;
    }
    std::map<ll, S> content() const {
        std::map<ll, S> result;
        collect(root, result);
        return result;
    }
};

} // namespace internal_sparse_point_fold

namespace internal_sparse_point_fold {

template <class M>
struct general_initial_tree
{
  using S = typename M::S;

private:
  ll n;
  S initial_value;
  template <class A, class = void>
  struct HasPow : false_type {};
  template <class A>
  struct HasPow<A, void_t<decltype(A::pow(declval<const S &>(), ll{}))>> : true_type {};

  static S repeat(S x, ll len)
  {
    if constexpr (HasPow<M>::value)
      return M::pow(x, len);
    else
    {
      S res = M::e();
      while (len)
      {
        if (len & 1) res = M::op(res, x);
        len >>= 1;
        if (len) x = M::op(x, x);
      }
      return res;
    }
  }

  struct Entry
  {
    // l 番目の値が val、[l + 1, r) は初期値のままで、その積が rest
    ll l, r;
    S val, rest;
    bool registered;
    Entry(ll l, ll r, const S &val, const S &rest)
        : l(l), r(r), val(val), rest(rest), registered(false) {}
  };
  struct Node
  {
    Entry entry;
    S prod;
    ll lo, hi;
    array<int, 2> chi;
    Node(const Entry &entry)
        : entry(entry), prod(M::op(entry.val, entry.rest)), lo(entry.l), hi(entry.r), chi{-1, -1} {}
  };
  vc<Node> nodes;

  void pull(int i)
  {
    int l = nodes[i].chi[0], r = nodes[i].chi[1];
    S sml = l == -1 ? M::e() : nodes[l].prod;
    S smm = M::op(nodes[i].entry.val, nodes[i].entry.rest);
    S smr = r == -1 ? M::e() : nodes[r].prod;
    nodes[i].prod = M::op(M::op(sml, smm), smr);
    nodes[i].lo = l == -1 ? nodes[i].entry.l : nodes[l].lo;
    nodes[i].hi = r == -1 ? nodes[i].entry.r : nodes[r].hi;
  }

  // 疎セグメント木の配置で、各区間の左端をキーにする。
  void insert(Entry x, ll a, ll b, int i)
  {
    ll c = a + (b - a) / 2;
    int d = x.l < c ? 0 : 1;
    if ((d == 0 && nodes[i].entry.l < x.l) || (d == 1 && x.l < nodes[i].entry.l))
      swap(x, nodes[i].entry);
    int j = nodes[i].chi[d];
    if (j == -1)
    {
      j = nodes.size();
      nodes.eb(x);
      nodes[i].chi[d] = j;
    }
    else if (d == 0)
      insert(std::move(x), a, c, j);
    else
      insert(std::move(x), c, b, j);
    pull(i);
  }

  template <class F>
  void modify_entry(ll p, const F &f, int i)
  {
    if (nodes[i].entry.l == p)
      f(nodes[i].entry);
    else
      modify_entry(p, f, nodes[i].chi[p < nodes[i].entry.l ? 0 : 1]);
    pull(i);
  }

  void split(ll p)
  {
    if (p == 0 || p == n) return;
    int i = 0, prev = -1;
    while (i != -1)
    {
      if (nodes[i].entry.l == p) return;
      if (nodes[i].entry.l < p)
        prev = i, i = nodes[i].chi[1];
      else
        i = nodes[i].chi[0];
    }
    ll l = nodes[prev].entry.l, r = nodes[prev].entry.r;
    modify_entry(l, [&](Entry &x)
    {
      x.r = p;
      x.rest = repeat(initial_value, p - l - 1);
    }, 0);
    insert(Entry(p, r, initial_value, repeat(initial_value, r - p - 1)), 0, n, 0);
  }

  S entry_prod(int i, ll l, ll r) const
  {
    const Entry &x = nodes[i].entry;
    ll a = max(l, x.l), b = min(r, x.r);
    if (b <= a) return M::e();
    if (a > x.l) return repeat(initial_value, b - a);
    return M::op(x.val, b == x.r ? x.rest : repeat(initial_value, b - a - 1));
  }

  template <class G>
  ll search_initial(ll l, ll r, S &sm, const G &g, bool from_right) const
  {
    vc<S> powers{initial_value};
    ll len = r - l, step = 1;
    while (step <= len / 2)
    {
      powers.eb(M::op(powers.back(), powers.back()));
      step *= 2;
    }
    ll pos = from_right ? r : l;
    repi(k, SZ(powers) - 1, -1, -1)
    {
      step = 1LL << k;
      if (step > (from_right ? pos - l : r - pos)) continue;
      S x = from_right ? M::op(powers[k], sm) : M::op(sm, powers[k]);
      if (g(x))
      {
        sm = std::move(x);
        pos += from_right ? -step : step;
      }
    }
    return pos;
  }

public:
  // 長さ n、各点の初期値 initial_value の列を作る
  general_initial_tree(ll n, const S &initial_value) : n(n), initial_value(initial_value)
  {
    assert(n >= 0);
    if (n > 0) nodes.eb(Entry(0, n, initial_value, repeat(initial_value, n - 1)));
  }

  // 少なくとも n ノード分の領域をあらかじめ確保する
  void reserve(int n) { nodes.reserve(n); }

  // p 番目の値を参照で受け取る関数 f を適用する。未登録なら initial_value から始める
  template <class F>
  void modify(ll p, const F &f)
  {
    assert(0 <= p && p < n);
    split(p);
    modify_entry(p, [&](Entry &x)
    {
      f(x.val);
      x.registered = true;
    }, 0);
  }

  void set(ll p, const S &x)
  {
    modify(p, [&](S &y) { y = x; });
  }

  S get(ll p) const
  {
    assert(0 <= p && p < n);
    int i = 0;
    while (true)
    {
      const Entry &x = nodes[i].entry;
      if (x.l <= p && p < x.r)
        return p == x.l ? x.val : initial_value;
      i = nodes[i].chi[p < x.l ? 0 : 1];
    }
  }

  // [l, r)
  S prod(ll l, ll r) const
  {
    assert(0 <= l && l <= r && r <= n);
    if (l == r) return M::e();
    auto dfs = [&](auto dfs, int i) -> S
    {
      if (nodes[i].hi <= l || r <= nodes[i].lo)
        return M::e();
      if (l <= nodes[i].lo && nodes[i].hi <= r)
        return nodes[i].prod;
      int li = nodes[i].chi[0], ri = nodes[i].chi[1];
      S sml = li == -1 ? M::e() : dfs(dfs, li);
      S smm = entry_prod(i, l, r);
      S smr = ri == -1 ? M::e() : dfs(dfs, ri);
      return M::op(M::op(sml, smm), smr);
    };
    return dfs(dfs, 0);
  }
  S all_prod() const { return nodes.empty() ? M::e() : nodes[0].prod; }

  // g(prod(l, r)) が true となる最大の r を返す
  template <class G>
  ll max_right_ok(ll l, const G &g) const
  {
    assert(0 <= l && l <= n);
    assert(g(M::e()));
    if (l == n) return n;
    S sm = M::e();
    auto append = [&](const S &x)
    {
      S nsm = M::op(sm, x);
      if (!g(nsm))
        return false;
      sm = std::move(nsm);
      return true;
    };
    auto dfs = [&](auto dfs, int i) -> ll
    {
      if (nodes[i].hi <= l)
        return -1;
      if (l <= nodes[i].lo && append(nodes[i].prod))
        return -1;
      int li = nodes[i].chi[0], ri = nodes[i].chi[1];
      ll res = li == -1 ? -1 : dfs(dfs, li);
      if (res != -1)
        return res;
      const Entry &x = nodes[i].entry;
      ll a = max(l, x.l), b = x.r;
      if (a < b && !append(entry_prod(i, a, b)))
      {
        if (a == x.l)
        {
          if (!append(x.val)) return a;
          a++;
        }
        return search_initial(a, b, sm, g, false);
      }
      return ri == -1 ? -1 : dfs(dfs, ri);
    };
    ll res = dfs(dfs, 0);
    return res == -1 ? n : res;
  }

  // g(prod(l, r)) が true となる最小の l を返す
  template <class G>
  ll min_left_ok(ll r, const G &g) const
  {
    assert(0 <= r && r <= n);
    assert(g(M::e()));
    if (r == 0) return 0;
    S sm = M::e();
    auto prepend = [&](const S &x)
    {
      S nsm = M::op(x, sm);
      if (!g(nsm))
        return false;
      sm = std::move(nsm);
      return true;
    };
    auto dfs = [&](auto dfs, int i) -> ll
    {
      if (r <= nodes[i].lo)
        return -1;
      if (nodes[i].hi <= r && prepend(nodes[i].prod))
        return -1;
      int li = nodes[i].chi[0], ri = nodes[i].chi[1];
      ll res = ri == -1 ? -1 : dfs(dfs, ri);
      if (res != -1)
        return res;
      const Entry &x = nodes[i].entry;
      ll a = x.l, b = min(r, x.r);
      if (a < b && !prepend(entry_prod(i, a, b)))
      {
        S rest = b == x.r ? x.rest : repeat(initial_value, b - a - 1);
        if (!prepend(rest)) return search_initial(a + 1, b, sm, g, true);
        if (!prepend(x.val)) return a + 1;
      }
      return li == -1 ? -1 : dfs(dfs, li);
    };
    ll res = dfs(dfs, 0);
    return res == -1 ? 0 : res;
  }

  // 登録された添字と値を map で返す。デバッグ用。
  map<ll, S> content() const
  {
    map<ll, S> res;
    fec(node : nodes) if (node.entry.registered) res[node.entry.l] = node.entry.val;
    return res;
  }
};

} // namespace internal_sparse_point_fold
