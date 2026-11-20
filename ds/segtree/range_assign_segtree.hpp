#pragma once

#include "../../algebra/monoid_power.hpp"
#include "segtree_range.hpp"

/**
 * @brief 区間代入・区間モノイド積のセグメント木
 * @docs docs/ds/segtree/range_assign_segtree.md
 */

template <class M>
struct RangeAssignSegmentTree
{
  using S = typename M::S;

private:
  int n;
  ll siz;
  vc<S> dat;
  vc<optional<S>> assigned;

  void set_node(ll i, const S &unit, const S &value)
  {
    dat[i] = value;
    if (i < siz) assigned[i] = unit;
  }

  void preserve_outside(ll i, int height, ll l, ll r)
  {
    ll prefix = max(0LL, l - (i << height));
    ll suffix = max(0LL, ((i + 1) << height) - r);
    const S &unit = *assigned[i];
    S power = unit;
    for (int h = 0; ; ++h)
    {
      if (prefix & 1)
      {
        l -= 1LL << h;
        set_node(l >> h, unit, power);
      }
      if (suffix & 1)
      {
        set_node(r >> h, unit, power);
        r += 1LL << h;
      }
      prefix >>= 1, suffix >>= 1;
      if ((prefix | suffix) == 0) break;
      power = M::op(power, power);
    }
  }

  bool expose(ll leaf, int top, int bottom, ll l, ll r)
  {
    for (int h = top; h >= bottom; --h)
    {
      const ll i = leaf >> h;
      if (assigned[i])
      {
        preserve_outside(i, h, l, r);
        return true;
      }
    }
    return false;
  }

  void pull(ll i)
  {
    assigned[i].reset();
    dat[i] = M::op(dat[2 * i], dat[2 * i + 1]);
  }

  static S uniform_prod(const S &unit, ll len)
  {
    return len == 1 ? unit : pow_monoid<M>(unit, len);
  }

  template <bool from_right>
  S border_prod(ll i, ll width, ll len) const
  {
    while (len < width)
    {
      if (assigned[i]) return uniform_prod(*assigned[i], len);
      width >>= 1;
      i = 2 * i + from_right;
      if (len > width)
      {
        if constexpr (from_right)
          return M::op(border_prod<true>(i - 1, width, len - width), dat[i]);
        else
          return M::op(dat[i], border_prod<false>(i + 1, width, len - width));
      }
    }
    return dat[i];
  }

  template <class X, class F>
  void apply_range(ll i, ll a, ll b, ll l, ll r, X &x, F &apply) const
  {
    if (l == a && r == b)
    {
      const S &value = dat[i];
      x = apply(x, value);
      return;
    }
    if (assigned[i])
    {
      ll len = r - l;
      if constexpr (internal::HasMonoidPow<M, ll>::value)
      {
        const S value = uniform_prod(*assigned[i], len);
        x = apply(x, value);
      }
      else
      {
        S power = *assigned[i];
        while (len > 3)
        {
          if (len & 1) x = apply(x, as_const(power));
          len >>= 1;
          power = M::op(power, power);
        }
        for (; len > 0; --len) x = apply(x, as_const(power));
      }
      return;
    }
    const ll mid = a + (b - a) / 2;
    if (l < mid) apply_range(2 * i, a, mid, l, min(r, mid), x, apply);
    if (mid < r) apply_range(2 * i + 1, mid, b, max(l, mid), r, x, apply);
  }

  template <bool from_right>
  static S join(const S &acc, const S &value)
  {
    if constexpr (from_right) return M::op(value, acc);
    else return M::op(acc, value);
  }

  template <bool from_right, class G>
  static ll search_uniform(const S &unit, ll l, ll r, S &acc, const G &g)
  {
    const int levels = bit_width(r - l);
    vc<S> powers;
    powers.reserve(levels);
    powers.eb(unit);
    for (int h = 1; h < levels; ++h) powers.eb(M::op(powers.back(), powers.back()));
    for (int h = levels - 1; h >= 0; --h)
    {
      const ll step = 1LL << h;
      if (r - l < step) continue;
      S next = join<from_right>(acc, powers[h]);
      if (g(as_const(next)))
      {
        acc = std::move(next);
        l += step;
      }
    }
    return l == r ? -1 : l;
  }

  template <bool from_right, class G>
  ll search(ll i, ll a, ll b, ll l, ll r, S &acc, const G &g) const
  {
    if (l == a && r == b)
    {
      S next = join<from_right>(acc, dat[i]);
      if (g(as_const(next)))
      {
        acc = std::move(next);
        return -1;
      }
      if (b - a == 1) return a;
    }
    if (assigned[i]) return search_uniform<from_right>(*assigned[i], l, r, acc, g);

    const ll mid = a + (b - a) / 2, first = 2 * i + from_right;
    if (l < mid)
    {
      ll stop = search<from_right>(first, a, mid, l, min(r, mid), acc, g);
      if (stop != -1) return stop;
    }
    return mid < r ? search<from_right>(first ^ 1, mid, b, max(l, mid), r, acc, g) : -1;
  }

public:
  RangeAssignSegmentTree() : RangeAssignSegmentTree(0) {}
  explicit RangeAssignSegmentTree(int n)
      : n(n), siz(SegmentTreeRange(n).leaf_size()), dat(2 * siz, M::e()), assigned(siz) {}
  RangeAssignSegmentTree(int n, const S &x) : RangeAssignSegmentTree(n) { set(0, n, x); }

  template <class T>
  explicit RangeAssignSegmentTree(const vc<T> &a) : RangeAssignSegmentTree(SZ(a))
  {
    for (int p = 0; p < n; ++p) dat[siz + p] = a[p];
    for (ll i = siz - 1; i > 0; --i) dat[i] = M::op(dat[2 * i], dat[2 * i + 1]);
  }

  int size() const { return n; }

  void set(int l, int r, const S &x)
  {
    assert(0 <= l && l <= r && r <= n);
    if (l == r) return;
    const S unit = x;
    const ll left = siz + l, right = siz + r;
    const int height = countr_zero(siz), split = bit_width(left ^ (right - 1));
    const int left_start = countr_zero(left) + 1, right_start = countr_zero(right) + 1;
    const int common_start = max(split, min(left_start, right_start));
    if (!expose(left, height, common_start, left, right))
    {
      expose(left, split - 1, left_start, left, right);
      expose(right - 1, split - 1, right_start, left, right);
    }

    S power = unit;
    for (ll a = left, b = right; ; )
    {
      if (a & 1) set_node(a++, unit, power);
      if (b & 1) set_node(--b, unit, power);
      a >>= 1, b >>= 1;
      if (a >= b) break;
      power = M::op(power, power);
    }

    for (int h = left_start; h < split; ++h) pull(left >> h);
    for (int h = right_start; h < split; ++h) pull((right - 1) >> h);
    for (int h = common_start; h <= height; ++h) pull(left >> h);
  }

  void set(int p, const S &x)
  {
    assert(0 <= p && p < n);
    set(p, p + 1, x);
  }

  S prod(int l, int r) const
  {
    assert(0 <= l && l <= r && r <= n);
    if (l == r) return M::e();
    const ll left = siz + l, right = siz + r;
    const int split = bit_width(left ^ (right - 1));
    for (int h = countr_zero(siz); h > split; --h)
      if (const auto &unit = assigned[left >> h]) return uniform_prod(*unit, r - l);

    const ll i = left >> split, width = 1LL << split;
    if (r - l == width) return dat[i];
    if (assigned[i]) return uniform_prod(*assigned[i], r - l);
    const ll half = width >> 1, mid = (i << split) + half;
    return M::op(border_prod<true>(2 * i, half, mid - left),
                 border_prod<false>(2 * i + 1, half, right - mid));
  }

  S get(int p) const
  {
    assert(0 <= p && p < n);
    const ll leaf = siz + p;
    for (int h = countr_zero(siz); h > 0; --h)
      if (const auto &unit = assigned[leaf >> h]) return *unit;
    return dat[leaf];
  }

  S all_prod() const { return dat[1]; }

  // i = l, ..., r-1 について x ← apply(x, get(i))
  // 行列ベクトル積など
  template <class X, class F>
  X prod_apply(int l, int r, X x, F &&apply) const
  {
    assert(0 <= l && l <= r && r <= n);
    if (l < r) apply_range(1, 0, siz, l, r, x, apply);
    return x;
  }

  // g(prod(l, r)) が true となる最大の r を返す
  template <class G>
  int max_right_ok(int l, const G &g) const
  {
    assert(0 <= l && l <= n);
    S acc = M::e();
    assert(g(as_const(acc)));
    if (l == n) return n;
    ll stop = search<false>(1, 0, siz, l, n, acc, g);
    return stop == -1 ? n : stop;
  }

  // g(prod(l, r)) が true となる最小の l を返す
  template <class G>
  int min_left_ok(int r, const G &g) const
  {
    assert(0 <= r && r <= n);
    S acc = M::e();
    assert(g(as_const(acc)));
    if (r == 0) return 0;
    ll stop = search<true>(1, 0, siz, siz - r, siz, acc, g);
    return stop == -1 ? 0 : siz - stop;
  }

  vc<S> content() const
  {
    vc<S> res;
    res.reserve(n);
    auto append = [&](auto &&self, ll i, ll a, ll b) -> void
    {
      if (n <= a) return;
      if (i >= siz) res.eb(dat[i]);
      else if (assigned[i]) res.insert(res.end(), b - a, *assigned[i]);
      else
      {
        const ll mid = a + (b - a) / 2;
        self(self, 2 * i, a, mid);
        self(self, 2 * i + 1, mid, b);
      }
    };
    append(append, 1, 0, siz);
    return res;
  }
};
