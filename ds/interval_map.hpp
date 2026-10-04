#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 値つき区間の管理（区間代入・削除）
 * @docs docs/ds/interval_map.md
 */

template <class T, class I = ll>
struct IntervalMap
{
  struct Segment
  {
    I l, r;
    T x;
  };

private:
  struct Compare
  {
    using is_transparent = void;
    bool operator()(const Segment &a, const Segment &b) const { return a.l < b.l; }
    bool operator()(const Segment &a, const I b) const { return a.l < b; }
    bool operator()(const I a, const Segment &b) const { return a < b.l; }
  };
  std::set<Segment, Compare> segs;
  T dflt;

  template <class Add, class Del>
  void replace(I l, I r, const T *x, Add &add, Del &del)
  {
    assert(!(r < l));
    if (!(l < r)) return;

    if (x)
    {
      auto it = segs.lower_bound(l);
      if (it != segs.begin())
      {
        --it;
        if (!(it->r < l) && it->x == *x) l = it->l;
      }
      it = segs.upper_bound(r);
      if (it != segs.begin())
      {
        --it;
        if (r < it->r && it->x == *x) r = it->r;
      }
    }

    auto it = next_it(l);
    while (it != segs.end() && it->l < r)
    {
      const Segment a = *it;
      del(a.l, a.r, a.x);
      it = segs.erase(it);
      if (a.l < l)
      {
        segs.insert(it, Segment{a.l, l, a.x});
        add(a.l, l, a.x);
      }
      if (r < a.r)
      {
        it = segs.insert(it, Segment{r, a.r, a.x});
        add(r, a.r, a.x);
        break;
      }
    }
    if (x)
    {
      segs.insert(it, Segment{l, r, *x});
      add(l, r, *x);
    }
  }

public:
  using iterator = typename std::set<Segment, Compare>::const_iterator;

  explicit IntervalMap(const T &dflt = T()) : dflt(dflt) {}
  IntervalMap(const vc<T> &a, const T &dflt = T()) : dflt(dflt)
  {
    const int n = a.size();
    for (int l = 0, r; l < n; l = r)
    {
      for (r = l + 1; r < n && a[l] == a[r]; ++r) {}
      segs.insert(segs.end(), Segment{I(l), I(r), a[l]});
    }
  }

  // p を含む区間
  // 未登録なら end()
  iterator get_it(const I p) const
  {
    auto it = segs.upper_bound(p);
    if (it == segs.begin()) return segs.end();
    --it;
    return p < it->r ? it : segs.end();
  }
  // p を含む区間、またはその右側で最初の区間
  iterator next_it(const I p) const
  {
    auto it = segs.upper_bound(p);
    if (it != segs.begin() && p < prev(it)->r) return prev(it);
    return it;
  }
  T get(const I p) const
  {
    auto it = get_it(p);
    return it == segs.end() ? dflt : it->x;
  }
  T operator[](const I p) const { return get(p); }

  // [l, r) を x にする
  // 区間 [L, R), 値 X が追加されるとき add(L, R, X) を呼ぶ
  // 区間 [L, R), 値 X が削除されるとき del(L, R, X) を呼ぶ
  template <class Add, class Del>
  void set(I l, I r, T x, Add &&add, Del &&del)
  {
    replace(l, r, &x, add, del);
  }
  // [l, r) を x にする
  // 区間 [L, R), 値 X が追加されるとき add(L, R, X) を呼ぶ
  // 区間 [L, R), 値 X が削除されるとき del(L, R, X) を呼ぶ
  void set(I l, I r, T x)
  {
    set(l, r, move(x), [](const I, const I, const T &) {},
           [](const I, const I, const T &) {});
  }
  // [l, r) を未登録にする
  // 区間 [L, R), 値 X が追加されるとき add(L, R, X) を呼ぶ
  // 区間 [L, R), 値 X が削除されるとき del(L, R, X) を呼ぶ
  template <class Add, class Del>
  void erase(I l, I r, Add &&add, Del &&del)
  {
    replace(l, r, nullptr, add, del);
  }
  // [l, r) を未登録にする
  // 区間 [L, R), 値 X が追加されるとき add(L, R, X) を呼ぶ
  // 区間 [L, R), 値 X が削除されるとき del(L, R, X) を呼ぶ
  void erase(I l, I r)
  {
    erase(l, r, [](const I, const I, const T &) {},
          [](const I, const I, const T &) {});
  }

  bool empty() const { return segs.empty(); }
  iterator begin() const { return segs.begin(); }
  iterator end() const { return segs.end(); }
  
  vc<tuple<I, I, T>> content() const
  {
    vc<tuple<I, I, T>> res;
    res.reserve(segs.size());
    for (const auto &[l, r, x] : segs) res.eb(l, r, x);
    return res;
  }
};
