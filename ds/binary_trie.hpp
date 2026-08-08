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
  static_assert(width < numeric_limits<Key>::digits ||
                width < numeric_limits<Boundary>::digits);

  struct Node
  {
    array<int, 2> chi;
    S sum;
    Node() : chi{-1, -1}, sum(M::e()) {}
  };

  vc<Node> nodes{1};
  Key xor_lazy = 0;

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

  S sum_of(int v) const { return v == -1 ? M::e() : nodes[v].sum; }

  void pull(int v)
  {
    nodes[v].sum = M::op(sum_of(nodes[v].chi[0]), sum_of(nodes[v].chi[1]));
  }

  int find_raw(Key a) const
  {
    int v = 0;
    repi(i, width - 1, -1, -1)
    {
      v = nodes[v].chi[(a >> i) & 1];
      if (v == -1)
        break;
    }
    return v;
  }

  template <class F>
  void modify_raw(Key a, const F &f)
  {
    array<int, width + 1> path;
    int v = 0;
    path[0] = v;
    repi(i, width - 1, -1, -1)
    {
      int b = (a >> i) & 1;
      int w = nodes[v].chi[b];
      if (w == -1)
      {
        w = nodes.size();
        nodes[v].chi[b] = w;
        nodes.eb();
      }
      v = w;
      path[width - i] = v;
    }
    f(nodes[v].sum);
    repi(i, width - 1, -1, -1) pull(path[i]);
  }

  S exact_sum(Key a, Key xor_val) const
  {
    int v = find_raw(a ^ xor_lazy ^ xor_val);
    return v == -1 ? M::e() : nodes[v].sum;
  }

  template <bool right, class F>
  Key search_ng(Key bound, const F &f, Key xor_val) const
  {
    assert(in_range(bound) && in_range(xor_val));
    assert(f(M::e()));
    const Key x = xor_lazy ^ xor_val;
    S sm = M::e();
    auto append = [&](const S &s)
    {
      S nsm = M::op(sm, s);
      if (!f(nsm))
        return false;
      sm = std::move(nsm);
      return true;
    };
    auto dfs = [&](auto dfs, int v, int i, bool tight, Key y) -> Key
    {
      if (v == -1)
        return -1;
      if (!tight && append(nodes[v].sum))
        return -1;
      if (i == -1)
      {
        if (tight && append(nodes[v].sum))
          return -1;
        return y ^ xor_val;
      }
      auto go = [&](int b, bool ntight)
      {
        const int z = (x >> i) & 1;
        return dfs(dfs, nodes[v].chi[z ^ b], i - 1, ntight, y | (Key(b) << i));
      };
      if (tight)
      {
        const int b = (bound >> i) & 1;
        auto res = go(b, true);
        if (res != -1)
          return res;
        if ((right && b == 0) || (!right && b == 1))
          return go(b ^ 1, false);
        return -1;
      }
      const int b = right ? 0 : 1;
      auto res = go(b, false);
      return res != -1 ? res : go(b ^ 1, false);
    };
    return dfs(dfs, 0, width - 1, true, 0);
  }

  template <bool right, class F>
  Key element_ng(Key bound, const F &cnt, Key xor_val) const
  {
    using Count = decay_t<decltype(cnt(M::e()))>;
    return search_ng<right>(bound, [&](const S &s) { return cnt(s) == Count{}; }, xor_val);
  }

public:
  BinaryTrie() {}
  void reserve(int n) { nodes.reserve(n); }
  void clear()
  {
    nodes.assign(1, Node());
    xor_lazy = 0;
  }

  // key に対応する値を返す。未更新なら M::e() を返す
  S get(Key a) const
  {
    assert(in_range(a));
    int v = find_raw(a ^ xor_lazy);
    return v == -1 ? M::e() : nodes[v].sum;
  }

  // key の値への参照を関数 f に渡して変更する。未更新なら M::e() から始める
  template <class F>
  void modify(Key a, const F &f)
  {
    assert(in_range(a));
    modify_raw(a ^ xor_lazy, f);
  }

  // key の値を x に変更する
  void set(Key a, const S &x)
  {
    modify(a, [&](S &y) { y = x; });
  }

  // key の値に x を加算する
  void add(Key a, const S &x)
  {
    modify(a, [&](S &y) { y = M::op(y, x); });
  }

  // すべての key に対応する値の和を返す
  S all_sum() const { return nodes[0].sum; }

  // key ^ xor_val < a を満たす値の和を返す
  S lt_sum(Key a, Key xor_val = 0) const
  {
    assert(in_range(a) && in_range(xor_val));
    Key x = xor_lazy ^ xor_val;
    int v = 0;
    S res = M::e();
    repi(i, width - 1, -1, -1)
    {
      int b = (a >> i) & 1;
      int z = (x >> i) & 1;
      if (b)
      {
        res = M::op(res, sum_of(nodes[v].chi[z]));
        v = nodes[v].chi[z ^ 1];
      }
      else
        v = nodes[v].chi[z];
      if (v == -1)
        break;
    }
    return res;
  }

  // key ^ xor_val > a を満たす値の和を返す
  S gt_sum(Key a, Key xor_val = 0) const
  {
    assert(in_range(a) && in_range(xor_val));
    Key x = xor_lazy ^ xor_val;
    int v = 0;
    S res = M::e();
    repi(i, width - 1, -1, -1)
    {
      int b = (a >> i) & 1;
      int z = (x >> i) & 1;
      if (b)
        v = nodes[v].chi[z ^ 1];
      else
      {
        res = M::op(res, sum_of(nodes[v].chi[z ^ 1]));
        v = nodes[v].chi[z];
      }
      if (v == -1)
        break;
    }
    return res;
  }

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

  // max_right_ng より前にある cnt が正の最後の key。なければ -1
  template <class F, class Count>
  Boundary max_right_ok_by(Key lower, const F &f, const Count &cnt, Key xor_val = 0) const
  {
    Boundary ng = max_right_ng(lower, f, xor_val);
    Key key = ng == key_limit()
                  ? element_ng<false>(max_key(), cnt, xor_val)
                  : lt_max_by(Key(ng) ^ xor_val, cnt, xor_val);
    return key == -1 || (key ^ xor_val) < lower ? Boundary(-1) : Boundary(key);
  }

  // L=min{l | f(sum(l <= key ^ xor_val <= upper))} として L-1 に対応する key。L=0 なら -1
  template <class F>
  Boundary min_left_ng(Key upper, const F &f, Key xor_val = 0) const
  {
    return Boundary(search_ng<false>(upper, f, xor_val));
  }

  // min_left_ng より後にある cnt が正の最初の key。なければ 2^width
  template <class F, class Count>
  Boundary min_left_ok_by(Key upper, const F &f, const Count &cnt, Key xor_val = 0) const
  {
    Boundary ng = min_left_ng(upper, f, xor_val);
    Key key = ng == -1
                  ? element_ng<true>(Key(0), cnt, xor_val)
                  : gt_min_by(Key(ng) ^ xor_val, cnt, xor_val);
    return key == -1 || (key ^ xor_val) > upper ? key_limit() : Boundary(key);
  }

  // ll cnt(S val): 値 val から「個数」の情報をとってくる関数
  // cnt を個数として key ^ xor_val の順で k 番目の key を返す
  template <class Count, class F>
  Key kth_by(Count k, const F &cnt, Key xor_val = 0) const
  {
    assert(Count{} <= k && k < cnt(all_sum()));
    Boundary res = max_right_ng(Key(0), [&](const S &s) { return cnt(s) <= k; }, xor_val);
    assert(res != key_limit());
    return Key(res);
  }

  // ll cnt(S val): 値 val から「個数」の情報をとってくる関数
  // cnt が正である key のうち key ^ xor_val が最小のものを返す
  template <class F>
  Key min_element_by(const F &cnt, Key xor_val = 0) const
  {
    Key res = element_ng<true>(Key(0), cnt, xor_val);
    assert(res != -1);
    return res;
  }

  // ll cnt(S val): 値 val から「個数」の情報をとってくる関数
  // cnt が正である key のうち key ^ xor_val が最大のものを返す
  template <class F>
  Key max_element_by(const F &cnt, Key xor_val = 0) const
  {
    Key res = element_ng<false>(max_key(), cnt, xor_val);
    assert(res != -1);
    return res;
  }

  // ll cnt(S val): 値 val から「個数」の情報をとってくる関数
  // key ^ xor_val < a かつ cnt が正である最大の key を返す。なければ -1
  template <class F>
  Key lt_max_by(Key a, const F &cnt, Key xor_val = 0) const
  {
    assert(in_range(a) && in_range(xor_val));
    return a == 0 ? -1 : element_ng<false>(a - 1, cnt, xor_val);
  }

  // ll cnt(S val): 値 val から「個数」の情報をとってくる関数
  // key ^ xor_val <= a かつ cnt が正である最大の key を返す。なければ -1
  template <class F>
  Key leq_max_by(Key a, const F &cnt, Key xor_val = 0) const
  {
    return element_ng<false>(a, cnt, xor_val);
  }

  // ll cnt(S val): 値 val から「個数」の情報をとってくる関数
  // key ^ xor_val >= a かつ cnt が正である最小の key を返す。なければ -1
  template <class F>
  Key geq_min_by(Key a, const F &cnt, Key xor_val = 0) const
  {
    return element_ng<true>(a, cnt, xor_val);
  }

  // ll cnt(S val): 値 val から「個数」の情報をとってくる関数
  // key ^ xor_val > a かつ cnt が正である最小の key を返す。なければ -1
  template <class F>
  Key gt_min_by(Key a, const F &cnt, Key xor_val = 0) const
  {
    assert(in_range(a) && in_range(xor_val));
    return a == max_key() ? -1 : element_ng<true>(a + 1, cnt, xor_val);
  }

  // すべての key に x を XOR する
  void xor_all(Key x)
  {
    assert(in_range(x));
    xor_lazy ^= x;
  }
};
