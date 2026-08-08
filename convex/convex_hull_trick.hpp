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
  T a, b;
  int id = -1;

  friend bool operator==(const ConvexHullTrickLine &x, const ConvexHullTrickLine &y)
  {
    return x.a == y.a && x.b == y.b && x.id == y.id;
  }
};

template <class T, class X = T>
struct ConvexHullTrickSegment
{
  ConvexHullTrickLine<T> line;
  X left, right;
};

// 傾きを任意順に追加できる Convex Hull Trick
template <class T = ll, class Compare = less<>, auto infty = INF, class X = T>
struct ConvexHullTrick
{
  static_assert(!is_integral_ext<T> || is_signed_ext<T>);
  static_assert(is_same_v<Compare, less<>> || is_same_v<Compare, less<T>> ||
                is_same_v<Compare, greater<>> || is_same_v<Compare, greater<T>>);

  using Line = ConvexHullTrickLine<T>;
  using Segment = ConvexHullTrickSegment<T, X>;
  using Value = decltype(declval<T>() * declval<X>() + declval<T>());

private:
  static constexpr int sgn = is_same_v<Compare, less<>> || is_same_v<Compare, less<T>> ? -1 : 1;

  struct Node
  {
    T a, b;
    mutable X r;
    int id;

    bool operator<(const Node &o) const { return a < o.a; }
    bool operator<(const X &x) const { return r < x; }
    friend bool operator<(const X &x, const Node &p) { return x < p.r; }
  };

  using It = typename multiset<Node, less<>>::iterator;

  multiset<Node, less<>> st;
  optional<It> qit;
  optional<X> last_x;
  int add_cnt = 0, qdir = 0;

  static Line line(const Node &p) { return {sgn * p.a, sgn * p.b, p.id}; }
  static Value eval(const Node &p, const X &x) { return sgn * (p.a * x + p.b); }

  static X border(const Node &x, const Node &y)
  {
    assert(x.a < y.a);
    if constexpr (is_integral_ext<T> && is_same_v<T, X>)
      return divfloor<T>(x.b - y.b, y.a - x.a);
    else
      return (X(x.b) - X(y.b)) / (X(y.a) - X(x.a));
  }

  bool set_r(It x, It y)
  {
    if (y == st.end())
    {
      x->r = numeric_limits<X>::max();
      return false;
    }
    if (x->a == y->a)
      x->r = x->b > y->b ? numeric_limits<X>::max() : numeric_limits<X>::lowest();
    else
      x->r = border(*x, *y);
    return x->r >= y->r;
  }

  const Node &best(const X &x) const
  {
    assert(!st.empty());
    return *st.lower_bound(x);
  }

  const Node &best_monotone(const X &x)
  {
    assert(!st.empty());
    if (!qit)
      qit = st.lower_bound(x);
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
    return **qit;
  }

public:
  // y = ax + b を追加する。id は 0 始まりの追加順になる
  void add_line(T a, T b) { add_line(a, b, add_cnt); }

  // y = ax + b を指定した id で追加する
  void add_line(T a, T b, int id)
  {
    add_cnt++;
    reset_monotone_query();
    Node p{sgn * a, sgn * b, numeric_limits<X>::max(), id};
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

  // x における最適な直線を返す。空ならダミー直線を返す
  Line query_line(const X &x) const
  {
    return st.empty() ? Line{T(0), T(-sgn * infty), -1} : line(best(x));
  }

  // x における最適値と、それを達成する直線を返す。空なら infty か -infty とダミー直線を返す
  pair<Value, Line> query_with_line(const X &x) const
  {
    if (st.empty()) return {Value(-sgn * infty), {T(0), T(-sgn * infty), -1}};
    const Node &p = best(x);
    return {eval(p, x), line(p)};
  }

  // x における最適値を返す。空なら最小化で infty、最大化で -infty を返す
  Value query(const X &x) const { return st.empty() ? Value(-sgn * infty) : eval(best(x), x); }

  // 単調な x における最適な直線を返す。増減方向は自動で判定する
  Line query_monotone_line(const X &x)
  {
    return st.empty() ? Line{T(0), T(-sgn * infty), -1} : line(best_monotone(x));
  }

  // 単調な x における最適値と直線を返す。増減方向は自動で判定する
  pair<Value, Line> query_monotone_with_line(const X &x)
  {
    if (st.empty()) return {Value(-sgn * infty), {T(0), T(-sgn * infty), -1}};
    const Node &p = best_monotone(x);
    return {eval(p, x), line(p)};
  }

  // 単調な x における最適値を返す。増減方向は自動で判定する
  Value query_monotone(const X &x)
  {
    return st.empty() ? Value(-sgn * infty) : eval(best_monotone(x), x);
  }

  // 単調クエリの現在位置と方向を消去する
  void reset_monotone_query()
  {
    qit.reset();
    last_x.reset();
    qdir = 0;
  }

  // 各直線と、その直線が最適になる境界区間 (left, right] を返す
  vc<Segment> segments() const
  {
    vc<Segment> res;
    res.reserve(st.size());
    X l = X(-infty);
    for (auto it = st.begin(); it != st.end(); ++it)
    {
      auto nxt = next(it);
      X r = nxt == st.end() ? X(infty) : it->r;
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

// 傾きを単調に追加する Convex Hull Trick。増加・減少は自動で判定する
template <class T = ll, class Compare = less<>, auto infty = INF, class X = T>
struct ConvexHullTrickMonotoneSlope
{
  static_assert(!is_integral_ext<T> || is_signed_ext<T>);
  static_assert(is_same_v<Compare, less<>> || is_same_v<Compare, less<T>> ||
                is_same_v<Compare, greater<>> || is_same_v<Compare, greater<T>>);

  using Line = ConvexHullTrickLine<T>;
  using Segment = ConvexHullTrickSegment<T, X>;
  using Value = decltype(declval<T>() * declval<X>() + declval<T>());

private:
  static constexpr int sgn = is_same_v<Compare, less<>> || is_same_v<Compare, less<T>> ? -1 : 1;

  struct Node
  {
    T a, b;
    X r;
    int id;
  };

  deque<Node> dq;
  optional<T> last_a;
  optional<X> last_x;
  int add_cnt = 0, adir = 0, qpos = -1, qdir = 0;

  static Line line(const Node &p) { return {sgn * p.a, sgn * p.b, p.id}; }
  static Value eval(const Node &p, const X &x) { return sgn * (p.a * x + p.b); }

  static X border(const Node &x, const Node &y)
  {
    assert(x.a < y.a);
    if constexpr (is_integral_ext<T> && is_same_v<T, X>)
      return divfloor<T>(x.b - y.b, y.a - x.a);
    else
      return (X(x.b) - X(y.b)) / (X(y.a) - X(x.a));
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
      X r = border(dq.back(), p);
      if (dq[dq.size() - 2].r < r) break;
      dq.pop_back();
    }
    if (!dq.empty()) dq.back().r = border(dq.back(), p);
    p.r = numeric_limits<X>::max();
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
      X r = border(p, dq.front());
      if (r < dq.front().r) break;
      dq.pop_front();
    }
    p.r = dq.empty() ? numeric_limits<X>::max() : border(p, dq.front());
    dq.emplace_front(p);
  }

  const Node &best(const X &x) const
  {
    assert(!dq.empty());
    return *lower_bound(dq.begin(), dq.end(), x,
                        [](const Node &p, const X &x) { return p.r < x; });
  }

  const Node &best_monotone(const X &x)
  {
    assert(!dq.empty());
    if (qpos == -1)
      qpos = lower_bound(dq.begin(), dq.end(), x,
                         [](const Node &p, const X &x) { return p.r < x; }) - dq.begin();
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
    return dq[qpos];
  }

public:
  // y = ax + b を追加する。id は 0 始まりの追加順になる
  void add_line(T a, T b) { add_line(a, b, add_cnt); }

  // y = ax + b を指定した id で追加する。a の増減方向は自動で判定する
  void add_line(T a, T b, int id)
  {
    add_cnt++;
    reset_monotone_query();
    Node p{sgn * a, sgn * b, numeric_limits<X>::max(), id};
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

  // x における最適な直線を返す。空ならダミー直線を返す
  Line query_line(const X &x) const
  {
    return dq.empty() ? Line{T(0), T(-sgn * infty), -1} : line(best(x));
  }

  // x における最適値と、それを達成する直線を返す。空なら infty か -infty とダミー直線を返す
  pair<Value, Line> query_with_line(const X &x) const
  {
    if (dq.empty()) return {Value(-sgn * infty), {T(0), T(-sgn * infty), -1}};
    const Node &p = best(x);
    return {eval(p, x), line(p)};
  }

  // x における最適値を返す。空なら最小化で infty、最大化で -infty を返す
  Value query(const X &x) const { return dq.empty() ? Value(-sgn * infty) : eval(best(x), x); }

  // 単調な x における最適な直線を返す。増減方向は自動で判定する
  Line query_monotone_line(const X &x)
  {
    return dq.empty() ? Line{T(0), T(-sgn * infty), -1} : line(best_monotone(x));
  }

  // 単調な x における最適値と直線を返す。増減方向は自動で判定する
  pair<Value, Line> query_monotone_with_line(const X &x)
  {
    if (dq.empty()) return {Value(-sgn * infty), {T(0), T(-sgn * infty), -1}};
    const Node &p = best_monotone(x);
    return {eval(p, x), line(p)};
  }

  // 単調な x における最適値を返す。増減方向は自動で判定する
  Value query_monotone(const X &x)
  {
    return dq.empty() ? Value(-sgn * infty) : eval(best_monotone(x), x);
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
    X l = X(-infty);
    repi(i, dq.size())
    {
      X r = i + 1 == SZ(dq) ? X(infty) : dq[i].r;
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
