#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief セグ木の点・区間・ノードの変換と列挙
 * @docs docs/ds/segtree/segtree_range.md
 */

struct SegmentTreeRange
{
private:
  int n;
  ll siz;

  void check_node(ll i) const { assert(1 <= i && i < 2 * siz); }

public:
  explicit SegmentTreeRange(int n) : n(n), siz(1)
  {
    assert(0 <= n);
    siz = bit_ceil(ll(n));
  }

  int size() const { return n; }
  ll leaf_size() const { return siz; }
  ll node_count() const { return 2 * siz - 1; }

  int depth(ll i) const
  {
    check_node(i);
    return bit_width(i) - 1;
  }
  bool is_leaf(ll i) const
  {
    check_node(i);
    return siz <= i;
  }
  ll range_length(ll i) const { return siz >> depth(i); }

  // 余白を含む完全二分木上の区間。
  pair<ll, ll> node_to_range(ll i) const
  {
    const int d = depth(i);
    const ll len = siz >> d, l = (i - (1LL << d)) * len;
    return {l, l + len};
  }
  ll range_to_node(ll l, ll r) const
  {
    assert(0 <= l && l < r && r <= siz);
    const ll len = r - l;
    assert(has_single_bit(len) && l % len == 0);
    return (siz + l) / len;
  }
  ll point_to_node(int p) const
  {
    assert(0 <= p && p < n);
    return siz + p;
  }
  int node_to_point(ll i) const
  {
    assert(siz <= i && i < siz + n);
    return i - siz;
  }

  template <class Iter>
  struct NodeRange
  {
    Iter first;
    Iter begin() const { return first; }
    Iter end() const { return {}; }
    bool empty() const { return begin() == end(); }
    vc<ll> to_v() const { return vc<ll>(begin(), end()); }
  };

  template <bool from_left>
  struct RangeIterator
  {
    using iterator_category = input_iterator_tag;
    using value_type = ll;
    using difference_type = ptrdiff_t;
    using reference = ll;
    using pointer = void;

  private:
    ll l = 0, r = 0, i = 0;

    void advance()
    {
      if constexpr (from_left)
      {
        if (l < r)
        {
          // 左端の整列条件と残りの長さから、次の最大区間を選ぶ。
          const ll len = min<ll>(lsb_mask(l), bit_floor(r - l));
          i = l / len;
          l += len;
          return;
        }
      }
      else
      {
        while (l < r)
        {
          if (l & 1)
          {
            i = l++;
            return;
          }
          if (r & 1)
          {
            i = --r;
            return;
          }
          l >>= 1, r >>= 1;
        }
      }
      l = r = i = 0;
    }

  public:
    RangeIterator() = default;
    RangeIterator(ll l, ll r) : l(l), r(r) { advance(); }
    ll operator*() const { return i; }
    RangeIterator &operator++()
    {
      advance();
      return *this;
    }
    RangeIterator operator++(int)
    {
      auto res = *this;
      ++*this;
      return res;
    }
    bool operator==(const RangeIterator &other) const
    {
      return l == other.l && r == other.r && i == other.i;
    }
    bool operator!=(const RangeIterator &other) const { return !(*this == other); }
  };

  template <bool from_top>
  struct AncestorIterator
  {
    using iterator_category = input_iterator_tag;
    using value_type = ll;
    using difference_type = ptrdiff_t;
    using reference = ll;
    using pointer = void;

  private:
    ll target = 0, i = 0;
    int shift = 0;

  public:
    AncestorIterator() = default;
    explicit AncestorIterator(ll target) : target(target), i(target)
    {
      if constexpr (from_top)
      {
        shift = bit_width(target) - 1;
        i = 1;
      }
    }
    ll operator*() const { return i; }
    AncestorIterator &operator++()
    {
      if constexpr (from_top)
        i = --shift < 0 ? 0 : target >> shift;
      else
        i >>= 1;
      return *this;
    }
    AncestorIterator operator++(int)
    {
      auto res = *this;
      ++*this;
      return res;
    }
    bool operator==(const AncestorIterator &other) const
    {
      return i == other.i && (i == 0 || target == other.target);
    }
    bool operator!=(const AncestorIterator &other) const { return !(*this == other); }
  };

  auto range_to_nodes_from_left(int l, int r) const
  {
    assert(0 <= l && l <= r && r <= n);
    return NodeRange<RangeIterator<true>>{RangeIterator<true>(siz + l, siz + r)};
  }
  auto range_to_nodes_from_bottom(int l, int r) const
  {
    assert(0 <= l && l <= r && r <= n);
    return NodeRange<RangeIterator<false>>{RangeIterator<false>(siz + l, siz + r)};
  }

  // i 自身を含む、i と根の間のノード。
  auto ancestors_from_bottom(ll i) const
  {
    check_node(i);
    return NodeRange<AncestorIterator<false>>{AncestorIterator<false>(i)};
  }
  auto ancestors_from_top(ll i) const
  {
    check_node(i);
    return NodeRange<AncestorIterator<true>>{AncestorIterator<true>(i)};
  }
  auto point_to_nodes_from_bottom(int p) const { return ancestors_from_bottom(point_to_node(p)); }
  auto point_to_nodes_from_top(int p) const { return ancestors_from_top(point_to_node(p)); }
};
