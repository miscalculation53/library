#pragma once
#include "../template/template_all_but_modint.hpp"
#include "../utils/larger_int.hpp"

/**
 * @brief Binary Trie
 * @docs docs/ds/binary_trie.md
 */

// M は可換モノイド
template <int width, class M = MonoidAdd<ll>, class Key = ll>
struct BinaryTrie
{
  using S = typename M::S;
  using Boundary = conditional_t<width == numeric_limits<Key>::digits, larger_int_t<Key>, Key>;
private:
  static_assert(is_integral_v<Key> && is_signed_v<Key>);
  static_assert(1 <= width && width <= numeric_limits<Key>::digits);
  static_assert(width < numeric_limits<Key>::digits || width < numeric_limits<Boundary>::digits);
  using UKey = make_unsigned_t<remove_cv_t<Key>>;
  static constexpr int shortcut_length = 3;
  struct Node
  {
    array<int, 2> chi;
    S sum;
    Node() : chi{-1, -1}, sum(M::e()) {}
  };
  struct Skip { Key exemplar; UKey mask; int target, next_bit; };
  struct Step { int node, bit; };
  vc<Node> nodes{1};
  vc<Skip> skips;
  vc<int> free_skips;
  // A leaf is promoted only when public modify exposes a reference to its value.
  deque<S> stable_values;
  Key lazy = 0;

  // Keep a short unused gap when a newly allocated node group crosses a boundary.
  int append_nodes(int count)
  {
    int first = nodes.size();
    const int offset = first & 63;
    if (offset + count > 63) first += 64 - offset;
    nodes.resize(first + count);
    return first;
  }

  static constexpr bool in_range(Key a)
  {
    if (a < 0)
      return false;
    if constexpr (width == numeric_limits<Key>::digits)
      return true;
    else
      return a < (Key(1) << width);
  }

  static constexpr Key max_key()
  {
    if constexpr (width == numeric_limits<Key>::digits)
      return numeric_limits<Key>::max();
    else
      return (Key(1) << width) - 1;
  }

  static constexpr Boundary key_limit() { return Boundary(1) << width; }

  S &leaf_value(int leaf)
  {
    const int slot = nodes[leaf].chi[1];
    return slot == -1 ? nodes[leaf].sum : stable_values[-2 - slot];
  }

  const S &leaf_value(int leaf) const
  {
    const int slot = nodes[leaf].chi[1];
    return slot == -1 ? nodes[leaf].sum : stable_values[-2 - slot];
  }

  const S &node_sum(int node, int bit) const
  {
    return bit < 0 ? leaf_value(node) : nodes[node].sum;
  }

  S &expose_leaf(int leaf)
  {
    if (nodes[leaf].chi[1] == -1)
    {
      stable_values.emplace_back(nodes[leaf].sum);
      nodes[leaf].chi[1] = -int(stable_values.size()) - 1;
    }
    return leaf_value(leaf);
  }

  static UKey skipped_mask(int upper, int lower)
  {
    return (UKey(1) << (upper + 1)) - (UKey(1) << (lower + 1));
  }

  static int highest_bit(UKey difference)
  {
    assert(difference != 0);
    if constexpr (sizeof(UKey) <= sizeof(ull))
      return numeric_limits<ull>::digits - 1 - __builtin_clzll(ull(difference));
    else
    {
      int bit = 0;
      while ((difference >>= 1) != 0) ++bit;
      return bit;
    }
  }

  int new_skip(Key exemplar, int target, int bit, int upper)
  {
    if (free_skips.empty())
    {
      skips.push_back({exemplar, skipped_mask(upper, bit), target, bit});
      return -int(skips.size()) - 1;
    }
    const int index = free_skips.back();
    free_skips.pop_back();
    skips[index] = {exemplar, skipped_mask(upper, bit), target, bit};
    return -index - 2;
  }

  // The target is an existing node at target_bit; build only the skipped prefix.
  int connect_segment(Key exemplar, int upper, int target_bit, int target)
  {
    const int length = upper - target_bit;
    if (length == 0) return target;
    if (length >= shortcut_length) return new_skip(exemplar, target, target_bit, upper);
    const int first = append_nodes(length);
    for (int offset = 0; offset < length; ++offset)
    {
      const int node = first + offset, bit = upper - offset;
      nodes[node].sum = M::op(M::e(), node_sum(target, target_bit));
      nodes[node].chi[(exemplar >> bit) & 1] = offset + 1 == length ? target : node + 1;
    }
    return first;
  }

  pair<int,int> new_tail(Key a, int upper)
  {
    const int leaf = append_nodes(1);
    return {connect_segment(a, upper, -1, leaf), leaf};
  }

  int trace(Key a, array<Step, width> &path, int &length) const
  {
    int node = 0, bit = width - 1;
    length = 0;
    while (bit >= 0)
    {
      path[length++] = {node, bit};
      const int edge = nodes[node].chi[(a >> bit) & 1];
      assert(edge != -1);
      if (edge >= 0) node = edge, --bit;
      else
      {
        const auto &skip = skips[-2 - edge];
        node = skip.target;
        bit = skip.next_bit;
      }
    }
    return node;
  }

  int ensure_leaf(Key a, array<Step, width> &path, int &length)
  {
    int node = 0, bit = width - 1;
    length = 0;
    while (bit >= 0)
    {
      path[length++] = {node, bit};
      const int b = (a >> bit) & 1, edge = nodes[node].chi[b];
      if (edge == -1)
      {
        const auto tail = new_tail(a, bit - 1);
        nodes[node].chi[b] = tail.first;
        return trace(a, path, length);
      }
      if (edge >= 0) node = edge, --bit;
      else
      {
        const Skip old = skips[-2 - edge];
        const int upper = bit - 1;
        const UKey difference = (UKey(a) ^ UKey(old.exemplar)) & old.mask;
        if (difference == 0)
        {
          node = old.target;
          bit = old.next_bit;
          continue;
        }
        const int split = highest_bit(difference);
        free_skips.eb(-edge - 2);
        const int branch = append_nodes(1);
        nodes[branch].sum = M::op(M::e(), node_sum(old.target, old.next_bit));
        const int old_edge = connect_segment(old.exemplar, split - 1, old.next_bit, old.target);
        const int new_edge = new_tail(a, split - 1).first;
        const int side = (a >> split) & 1;
        nodes[branch].chi[side] = new_edge;
        nodes[branch].chi[side ^ 1] = old_edge;
        nodes[node].chi[b] = connect_segment(a, upper, split, branch);
        return trace(a, path, length);
      }
    }
    return node;
  }

  const S &edge_sum(int edge, int bit) const
  {
    if (edge >= 0) return node_sum(edge, bit);
    const auto &skip = skips[-2 - edge];
    return node_sum(skip.target, skip.next_bit);
  }

  S sum_of(int edge, int bit) const
  {
    return edge == -1 ? M::e() : edge_sum(edge, bit);
  }

  // The prefix contains exactly the already selected raw-key bits.
  void follow(int edge, int &node, int &bit, UKey &prefix) const
  {
    if (edge >= 0) node = edge;
    else
    {
      const auto &skip = skips[-2 - edge];
      prefix |= UKey(skip.exemplar) & skip.mask;
      node = skip.target;
      bit = skip.next_bit;
    }
  }

  int find_raw(Key a) const
  {
    int node = 0;
    for (int bit = width - 1; bit >= 0; --bit)
    {
      const int edge = nodes[node].chi[(a >> bit) & 1];
      if (edge < 0)
      {
        if (edge == -1) return -1;
        const auto &skip = skips[-2 - edge];
        if (((UKey(a) ^ UKey(skip.exemplar)) & skip.mask) != 0) return -1;
        node = skip.target;
        bit = skip.next_bit + 1;
      }
      else node = edge;
    }
    return node;
  }

  void repair(Key a, int leaf, const array<Step, width> &path, int length)
  {
    S sum = leaf_value(leaf);
    while (length != 0)
    {
      const auto step = path[--length];
      const int sibling = nodes[step.node].chi[((a >> step.bit) & 1) ^ 1];
      if (sibling != -1) sum = M::op(std::move(sum), edge_sum(sibling, step.bit - 1));
      nodes[step.node].sum = sum;
    }
  }

  S exact_sum(Key a, Key xor_val) const
  {
    const int leaf = find_raw(a ^ lazy ^ xor_val);
    return leaf == -1 ? M::e() : leaf_value(leaf);
  }

  template <bool less>
  S strict_sum(Key bound, Key xor_val) const
  {
    assert(in_range(bound) && in_range(xor_val));
    const UKey x = UKey(lazy ^ xor_val), a = UKey(bound);
    int node = 0, bit = width - 1;
    UKey prefix = 0;
    S result = M::e();
    for (;;)
    {
      const UKey p = (prefix ^ x) >> (bit + 1), q = a >> (bit + 1);
      if (p != q)
      {
        if (less ? p < q : p > q) result = M::op(result, node_sum(node, bit));
        return result;
      }
      if (bit < 0) return result;
      const int b = (a >> bit) & 1, z = (x >> bit) & 1;
      if (b == int(less))
        result = M::op(result, sum_of(nodes[node].chi[z ^ int(!less)], bit - 1));
      const int side = b ^ z, edge = nodes[node].chi[side];
      if (edge == -1) return result;
      prefix |= UKey(side) << bit;
      --bit;
      follow(edge, node, bit, prefix);
    }
  }

  template <bool right, class F>
  Key search_ng(Key bound, const F &f, Key xor_val) const
  {
    assert(in_range(bound) && in_range(xor_val));
    assert(f(M::e()));
    const UKey x = UKey(lazy ^ xor_val), a = UKey(bound);
    S aggregate = M::e();
    auto append = [&](const S &value)
    {
      S next = M::op(aggregate, value);
      if (!f(next)) return false;
      aggregate = std::move(next);
      return true;
    };
    auto dfs = [&](auto &&self, int node, int bit, UKey prefix) -> Key
    {
      const UKey p = (prefix ^ x) >> (bit + 1), q = a >> (bit + 1);
      if (right ? p < q : p > q) return -1;
      const bool complete = p != q || bit < 0;
      if (complete && append(node_sum(node, bit))) return -1;
      if (bit < 0) return Key(prefix) ^ lazy;
      const int first = int((x >> bit) & 1) ^ int(!right);
      for (int turn = 0; turn < 2; ++turn)
      {
        const int side = first ^ turn, edge = nodes[node].chi[side];
        if (edge == -1) continue;
        int child = 0, child_bit = bit - 1;
        UKey child_prefix = prefix | (UKey(side) << bit);
        follow(edge, child, child_bit, child_prefix);
        const Key result = self(self, child, child_bit, child_prefix);
        if (result != -1) return result;
      }
      return -1;
    };
    return dfs(dfs, 0, width - 1, UKey(0));
  }

  template <bool right, class F>
  Key element_ng(Key bound, const F &cnt, Key xor_val) const
  {
    using Count = decay_t<decltype(cnt(M::e()))>;
    return search_ng<right>(bound, [&](const S &sum) { return cnt(sum) == Count{}; }, xor_val);
  }

  template <bool maximum, class F>
  Key extreme_key(const F &cnt, Key xor_val) const
  {
    using Count = decay_t<decltype(cnt(M::e()))>;
    assert(in_range(xor_val) && cnt(nodes[0].sum) != Count{});
    const UKey x = UKey(lazy ^ xor_val);
    int node = 0, bit = width - 1;
    UKey prefix = 0;
    while (bit >= 0)
    {
      int side = ((x >> bit) & 1) ^ int(maximum), edge = nodes[node].chi[side];
      if (edge == -1 || cnt(edge_sum(edge, bit - 1)) == Count{})
        side ^= 1, edge = nodes[node].chi[side];
      assert(edge != -1);
      prefix |= UKey(side) << bit;
      --bit;
      follow(edge, node, bit, prefix);
    }
    return Key(prefix) ^ lazy;
  }

public:
  BinaryTrie() {}
  void reserve(int n) { nodes.reserve(n); }
  void clear()
  {
    nodes.assign(1, Node());
    skips.clear();
    free_skips.clear();
    stable_values.clear();
    lazy = 0;
  }
  // すべての key に対応する値の和を返す
  S all_sum() const { return nodes[0].sum; }
  // すべての key に x を XOR する
  void xor_all(Key x) { assert(in_range(x)); lazy ^= x; }

  // key に対応する値を返す。未更新なら M::e() を返す
  S get(Key a) const
  {
    assert(in_range(a));
    const int leaf = find_raw(a ^ lazy);
    return leaf == -1 ? M::e() : leaf_value(leaf);
  }

  // key の値への参照を関数 f に渡して変更する。未更新なら M::e() から始める
  template <class F>
  void modify(Key a, const F &f)
  {
    assert(in_range(a));
    a ^= lazy;
    array<Step, width> path;
    int length;
    int leaf = ensure_leaf(a, path, length);
    S &value = expose_leaf(leaf);
    const auto old_size = nodes.size();
    f(value);
    if (nodes.size() != old_size) leaf = trace(a, path, length);
    repair(a, leaf, path, length);
  }

  // key の値を x に変更する
  void set(Key a, const S &x)
  {
    assert(in_range(a));
    a ^= lazy;
    array<Step, width> path;
    int length;
    const int leaf = ensure_leaf(a, path, length);
    if constexpr (is_integral_v<S>)
    {
      // Before any mutable reference is exposed, equal values preserve all aggregates.
      if (stable_values.empty() && leaf_value(leaf) == x) return;
    }
    leaf_value(leaf) = x;
    repair(a, leaf, path, length);
  }

  // key の値に x を加算する
  void add(Key a, const S &x)
  {
    assert(in_range(a));
    a ^= lazy;
    array<Step, width> path;
    int length;
    const int leaf = ensure_leaf(a, path, length);
    for (int i = 0; i < length; ++i)
      nodes[path[i].node].sum = M::op(nodes[path[i].node].sum, x);
    // Update the leaf last so x can refer to this same leaf.
    S &value = leaf_value(leaf);
    value = M::op(value, x);
  }

  // key ^ xor_val < a を満たす値の和を返す
  S lt_sum(Key a, Key xor_val = 0) const { return strict_sum<true>(a, xor_val); }
  // key ^ xor_val > a を満たす値の和を返す
  S gt_sum(Key a, Key xor_val = 0) const { return strict_sum<false>(a, xor_val); }
  // key ^ xor_val <= a を満たす値の和を返す
  S leq_sum(Key a, Key xor_val = 0) const
  {
    assert(in_range(a) && in_range(xor_val));
    return M::op(lt_sum(a, xor_val), exact_sum(a, xor_val));
  }
  // key ^ xor_val >= a を満たす値の和を返す
  S geq_sum(Key a, Key xor_val = 0) const
  {
    assert(in_range(a) && in_range(xor_val));
    return M::op(gt_sum(a, xor_val), exact_sum(a, xor_val));
  }

  // R=max{r | f(sum(lower <= key ^ xor_val < r))} に対応する key。R=2^width なら 2^width
  template <class F>
  Boundary max_right_ng(Key lower, const F &f, Key xor_val = 0) const
  {
    Key key = search_ng<true>(lower, f, xor_val);
    return key == -1 ? key_limit() : Boundary(key);
  }

  // max_right_ng より前にある cnt が正の最後の key。なければ nullopt
  template <class F, class Count>
  optional<Boundary> max_right_ok_by(Key lower, const F &f, const Count &cnt, Key xor_val = 0) const
  {
    Boundary ng = max_right_ng(lower, f, xor_val);
    Key key = ng == key_limit()
                  ? element_ng<false>(max_key(), cnt, xor_val)
                  : lt_max_by(Key(ng) ^ xor_val, cnt, xor_val).value_or(-1);
    return key == -1 || (key ^ xor_val) < lower ? nullopt : optional<Boundary>(Boundary(key));
  }

  // L=min{l | f(sum(l <= key ^ xor_val <= upper))} として L-1 に対応する key。L=0 なら -1
  template <class F>
  Boundary min_left_ng(Key upper, const F &f, Key xor_val = 0) const
  {
    return Boundary(search_ng<false>(upper, f, xor_val));
  }

  // min_left_ng より後にある cnt が正の最初の key。なければ nullopt
  template <class F, class Count>
  optional<Boundary> min_left_ok_by(Key upper, const F &f, const Count &cnt, Key xor_val = 0) const
  {
    Boundary ng = min_left_ng(upper, f, xor_val);
    Key key = ng == -1
                  ? element_ng<true>(Key(0), cnt, xor_val)
                  : gt_min_by(Key(ng) ^ xor_val, cnt, xor_val).value_or(-1);
    return key == -1 || (key ^ xor_val) > upper ? nullopt : optional<Boundary>(Boundary(key));
  }

  // ll cnt(S val): 値 val から「個数」の情報をとってくる関数
  // cnt を個数として key ^ xor_val の順で k 番目の key を返す
  template <class Count, class F>
  optional<Key> kth_by(Count k, const F &cnt, Key xor_val = 0) const
  {
    assert(in_range(xor_val));
    if (!(Count{} <= k && k < cnt(all_sum()))) return nullopt;
    const UKey x = UKey(lazy ^ xor_val);
    int node = 0, bit = width - 1;
    UKey raw = 0;
    S prefix = M::e();
    while (bit >= 0)
    {
      const int z = (x >> bit) & 1, left = nodes[node].chi[z];
      S next = M::op(prefix, sum_of(left, bit - 1));
      const bool take_right = cnt(next) <= k;
      if (take_right) prefix = std::move(next);
      const int side = z ^ int(take_right), edge = nodes[node].chi[side];
      assert(edge != -1);
      raw |= UKey(side) << bit;
      --bit;
      follow(edge, node, bit, raw);
    }
    return Key(raw) ^ lazy;
  }

  // ll cnt(S val): 値 val から「個数」の情報をとってくる関数
  // cnt が正である key のうち key ^ xor_val が最小のものを返す
  template <class F>
  optional<Key> min_element_by(const F &cnt, Key xor_val = 0) const
  {
    assert(in_range(xor_val));
    if (!(cnt(all_sum()) != decay_t<decltype(cnt(M::e()))>{})) return nullopt;
    return extreme_key<false>(cnt, xor_val);
  }

  // ll cnt(S val): 値 val から「個数」の情報をとってくる関数
  // cnt が正である key のうち key ^ xor_val が最大のものを返す
  template <class F>
  optional<Key> max_element_by(const F &cnt, Key xor_val = 0) const
  {
    assert(in_range(xor_val));
    if (!(cnt(all_sum()) != decay_t<decltype(cnt(M::e()))>{})) return nullopt;
    return extreme_key<true>(cnt, xor_val);
  }

  // ll cnt(S val): 値 val から「個数」の情報をとってくる関数
  // key ^ xor_val < a かつ cnt が正である最大の key を返す。なければ nullopt
  template <class F>
  optional<Key> lt_max_by(Key a, const F &cnt, Key xor_val = 0) const
  {
    assert(in_range(a) && in_range(xor_val));
    Key key = a == 0 ? -1 : element_ng<false>(a - 1, cnt, xor_val);
    return key == -1 ? nullopt : optional<Key>(key);
  }

  // ll cnt(S val): 値 val から「個数」の情報をとってくる関数
  // key ^ xor_val <= a かつ cnt が正である最大の key を返す。なければ nullopt
  template <class F>
  optional<Key> leq_max_by(Key a, const F &cnt, Key xor_val = 0) const
  {
    Key key = element_ng<false>(a, cnt, xor_val);
    return key == -1 ? nullopt : optional<Key>(key);
  }

  // ll cnt(S val): 値 val から「個数」の情報をとってくる関数
  // key ^ xor_val >= a かつ cnt が正である最小の key を返す。なければ nullopt
  template <class F>
  optional<Key> geq_min_by(Key a, const F &cnt, Key xor_val = 0) const
  {
    Key key = element_ng<true>(a, cnt, xor_val);
    return key == -1 ? nullopt : optional<Key>(key);
  }

  // ll cnt(S val): 値 val から「個数」の情報をとってくる関数
  // key ^ xor_val > a かつ cnt が正である最小の key を返す。なければ nullopt
  template <class F>
  optional<Key> gt_min_by(Key a, const F &cnt, Key xor_val = 0) const
  {
    assert(in_range(a) && in_range(xor_val));
    Key key = a == max_key() ? -1 : element_ng<true>(a + 1, cnt, xor_val);
    return key == -1 ? nullopt : optional<Key>(key);
  }

  // 登録されたキーと値を、xor_all を反映した map で返す。
  map<Key, S> content() const
  {
    map<Key, S> result;
    auto dfs = [&](auto &&self, int node, int bit, UKey prefix) -> void
    {
      if (bit < 0)
      {
        result.emplace_hint(result.end(), Key(prefix) ^ lazy, leaf_value(node));
        return;
      }
      const int first = (UKey(lazy) >> bit) & 1;
      for (int turn = 0; turn < 2; ++turn)
      {
        const int side = first ^ turn, edge = nodes[node].chi[side];
        if (edge == -1) continue;
        int child = 0, child_bit = bit - 1;
        UKey child_prefix = prefix | (UKey(side) << bit);
        follow(edge, child, child_bit, child_prefix);
        self(self, child, child_bit, child_prefix);
      }
    };
    dfs(dfs, 0, width - 1, UKey(0));
    return result;
  }

};
