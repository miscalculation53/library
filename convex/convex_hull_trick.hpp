#pragma once

#include "../template/template_all_but_modint.hpp"

#include "../utils/is_integral_ext.hpp"

/**
 * @brief Convex Hull Trick
 * @docs docs/convex/convex_hull_trick.md
 */

template <class T>
struct ConvexHullTrickLine
{
  // ax + b
  T a, b;
  int id = -1;

  friend bool operator==(const ConvexHullTrickLine &x, const ConvexHullTrickLine &y)
  {
    return x.a == y.a && x.b == y.b && x.id == y.id;
  }
};

template <class T>
struct ConvexHullTrickSegment
{
  ConvexHullTrickLine<T> line;
  T left, right;
};

template <class T = ll, class Compare = less<>, auto infty = INF>
struct ConvexHullTrick
{
  static_assert(!is_integral_ext<T> || is_signed_ext<T>);
  static_assert(is_same_v<Compare, less<>> || is_same_v<Compare, less<T>> ||
                is_same_v<Compare, greater<>> || is_same_v<Compare, greater<T>>);

  using Line = ConvexHullTrickLine<T>;
  using Segment = ConvexHullTrickSegment<T>;

private:
  static constexpr int sgn = is_same_v<Compare, less<>> || is_same_v<Compare, less<T>> ? -1 : 1;

  struct Node
  {
    T a, b;
    mutable T r;
    int id;

    bool operator<(const Node &o) const { return a < o.a; }
    bool operator<(const T &x) const { return r < x; }
    friend bool operator<(const T &x, const Node &p) { return x < p.r; }
  };

  using It = typename multiset<Node, less<>>::iterator;

  multiset<Node, less<>> st;
  optional<It> qit;
  optional<T> last_x;
  int add_cnt = 0, qdir = 0;

  static T inf() { return T(infty); }
  static T ninf() { return -T(infty); }
  static T empty_value()
  {
    if constexpr (sgn < 0)
      return inf();
    else
      return ninf();
  }

  static Line line(const Node &p) { return {sgn * p.a, sgn * p.b, p.id}; }
  static T eval(const Node &p, const T &x) { return sgn * (p.a * x + p.b); }

  static T border(const Node &x, const Node &y)
  {
    assert(x.a < y.a);
    if constexpr (is_integral_ext<T>)
      return divfloor<T>(x.b - y.b, y.a - x.a);
    else
      return (x.b - y.b) / (y.a - x.a);
  }

  bool set_r(It x, It y)
  {
    if (y == st.end())
    {
      x->r = inf();
      return false;
    }
    if (x->a == y->a)
      x->r = x->b > y->b ? inf() : ninf();
    else
      x->r = border(*x, *y);
    return x->r >= y->r;
  }

public:
  // y = ax + b を追加
  // 直線の id は 0 始まりの追加順になる
  void add_line(T a, T b) { add_line(a, b, add_cnt); }

  // y = ax + b を追加
  // 直線の id を指定
  void add_line(T a, T b, int id)
  {
    add_cnt++;
    reset_monotone_query();
    Node p{sgn * a, sgn * b, inf(), id};
    auto same = st.lower_bound(p);
    if (same != st.end() && same->a == p.a)
    {
      if (same->b >= p.b) return;
      auto prv = same == st.begin() ? st.end() : prev(same);
      auto nxt = next(same);
      st.erase(same);
      if (prv != st.end()) set_r(prv, nxt);
    }

    auto z = st.insert(p), y = z++, x = y;
    while (set_r(y, z)) z = st.erase(z);
    if (x != st.begin() && set_r(--x, y)) set_r(x, y = st.erase(y));
    while ((y = x) != st.begin() && (--x)->r >= y->r) set_r(x, st.erase(y));
  }

  // x における最適値と、それを達成する直線を返す
  pair<T, Line> query(const T &x) const
  {
    if (st.empty()) return {empty_value(), {T(0), empty_value(), -1}};
    auto it = st.lower_bound(x);
    if (it == st.end()) --it;
    const Node &p = *it;
    return {eval(p, x), line(p)};
  }

  // 単調な x における最適値と直線を返す。増減方向は自動で判定する
  pair<T, Line> query_monotone(const T &x)
  {
    if (st.empty()) return {empty_value(), {T(0), empty_value(), -1}};
    if (!qit)
    {
      qit = st.lower_bound(x);
      if (*qit == st.end()) --*qit;
    }
    else if (*last_x < x)
    {
      assert(qdir != -1 && "query order is not monotone");
      qdir = 1;
      while (next(*qit) != st.end() && (*qit)->r < x) ++*qit;
    }
    else if (x < *last_x)
    {
      assert(qdir != 1 && "query order is not monotone");
      qdir = -1;
      while (*qit != st.begin() && !(prev(*qit)->r < x)) --*qit;
    }
    last_x = x;
    const Node &p = **qit;
    return {eval(p, x), line(p)};
  }

  // 新しい単調クエリ列を始められるように、現在位置と増減方向を消去する
  void reset_monotone_query()
  {
    qit.reset();
    last_x.reset();
    qdir = 0;
  }

  // Segment: line, left, right
  // 直線 line が最適になる x の範囲 (left, right]
  // x の昇順に返す
  vc<Segment> segments() const
  {
    vc<Segment> res;
    res.reserve(st.size());
    T l = ninf();
    for (auto it = st.begin(); it != st.end(); ++it)
    {
      auto nxt = next(it);
      T r = nxt == st.end() ? inf() : it->r;
      res.eb(Segment{line(*it), l, r});
      l = r;
    }
    return res;
  }

  void clear()
  {
    st.clear();
    add_cnt = 0;
    reset_monotone_query();
  }
};

// **追加する直線の傾き**が単調
template <class T = ll, class Compare = less<>, auto infty = INF>
struct ConvexHullTrickMonotoneSlope
{
  static_assert(!is_integral_ext<T> || is_signed_ext<T>);
  static_assert(is_same_v<Compare, less<>> || is_same_v<Compare, less<T>> ||
                is_same_v<Compare, greater<>> || is_same_v<Compare, greater<T>>);

  using Line = ConvexHullTrickLine<T>;
  using Segment = ConvexHullTrickSegment<T>;

private:
  static constexpr int sgn = is_same_v<Compare, less<>> || is_same_v<Compare, less<T>> ? -1 : 1;

  struct Node
  {
    T a, b;
    T r;
    int id;
  };

  deque<Node> dq;
  optional<T> last_a;
  optional<T> last_x;
  int add_cnt = 0, adir = 0, qpos = -1, qdir = 0;

  static T inf() { return T(infty); }
  static T ninf() { return -T(infty); }
  static T empty_value()
  {
    if constexpr (sgn < 0)
      return inf();
    else
      return ninf();
  }

  static Line line(const Node &p) { return {sgn * p.a, sgn * p.b, p.id}; }
  static T eval(const Node &p, const T &x) { return sgn * (p.a * x + p.b); }

  static T border(const Node &x, const Node &y)
  {
    assert(x.a < y.a);
    if constexpr (is_integral_ext<T>)
      return divfloor<T>(x.b - y.b, y.a - x.a);
    else
      return (x.b - y.b) / (y.a - x.a);
  }

  void push_back(Node p)
  {
    if (!dq.empty() && dq.back().a == p.a)
    {
      if (dq.back().b >= p.b) return;
      dq.pop_back();
    }
    while (dq.size() >= 2)
    {
      T r = border(dq.back(), p);
      if (dq[dq.size() - 2].r < r) break;
      dq.pop_back();
    }
    if (!dq.empty()) dq.back().r = border(dq.back(), p);
    p.r = inf();
    dq.eb(p);
  }

  void push_front(Node p)
  {
    if (!dq.empty() && dq.front().a == p.a)
    {
      if (dq.front().b >= p.b) return;
      dq.pop_front();
    }
    while (dq.size() >= 2)
    {
      T r = border(p, dq.front());
      if (r < dq.front().r) break;
      dq.pop_front();
    }
    p.r = dq.empty() ? inf() : border(p, dq.front());
    dq.emplace_front(p);
  }

public:
  // y = ax + b を追加する。id は 0 始まりの追加順になる
  void add_line(T a, T b) { add_line(a, b, add_cnt); }

  // y = ax + b を指定した id で追加する。a の増減方向は自動で判定する
  void add_line(T a, T b, int id)
  {
    add_cnt++;
    reset_monotone_query();
    Node p{sgn * a, sgn * b, inf(), id};
    if (last_a)
    {
      int d = (*last_a < p.a) - (p.a < *last_a);
      if (d)
      {
        assert((!adir || d == adir) && "slope order is not monotone");
        adir = d;
      }
    }
    last_a = p.a;
    if (adir < 0)
      push_front(p);
    else
      push_back(p);
  }

  // x における最適値と、それを達成する直線を返す。空なら infty か -infty とダミー直線を返す
  pair<T, Line> query(const T &x) const
  {
    if (dq.empty()) return {empty_value(), {T(0), empty_value(), -1}};
    auto it = lower_bound(dq.begin(), dq.end(), x,
                          [](const Node &p, const T &x) { return p.r < x; });
    if (it == dq.end()) --it;
    const Node &p = *it;
    return {eval(p, x), line(p)};
  }

  // 単調な x における最適値と直線を返す。増減方向は自動で判定する
  pair<T, Line> query_monotone(const T &x)
  {
    if (dq.empty()) return {empty_value(), {T(0), empty_value(), -1}};
    if (qpos == -1)
    {
      qpos = lower_bound(dq.begin(), dq.end(), x,
                         [](const Node &p, const T &x) { return p.r < x; }) - dq.begin();
      if (qpos == SZ(dq)) qpos--;
    }
    else if (*last_x < x)
    {
      assert(qdir != -1 && "query order is not monotone");
      qdir = 1;
      while (qpos + 1 < SZ(dq) && dq[qpos].r < x) qpos++;
    }
    else if (x < *last_x)
    {
      assert(qdir != 1 && "query order is not monotone");
      qdir = -1;
      while (qpos > 0 && !(dq[qpos - 1].r < x)) qpos--;
    }
    last_x = x;
    const Node &p = dq[qpos];
    return {eval(p, x), line(p)};
  }

  // 単調クエリの現在位置と方向を消去する
  void reset_monotone_query()
  {
    qpos = -1;
    last_x.reset();
    qdir = 0;
  }

  // 各直線と、その直線が最適になる境界区間 (left, right] を返す
  vc<Segment> segments() const
  {
    vc<Segment> res;
    res.reserve(dq.size());
    T l = ninf();
    repi(i, dq.size())
    {
      T r = i + 1 == SZ(dq) ? inf() : dq[i].r;
      res.eb(Segment{line(dq[i]), l, r});
      l = r;
    }
    return res;
  }

  void clear()
  {
    dq.clear();
    add_cnt = 0;
    last_a.reset();
    adir = 0;
    reset_monotone_query();
  }
};
