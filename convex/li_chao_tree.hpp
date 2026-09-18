#pragma once

#include "../template/template_all_but_modint.hpp"

#include "../utils/is_integral_ext.hpp"
#include "../utils/larger_int.hpp"
#include "../utils/make_unsigned_ext.hpp"
#include "../utils/resolved_infty.hpp"

/**
 * @brief Li Chao Tree（動的・座標圧縮）
 * @docs docs/convex/li_chao_tree.md
 */

template <class T>
struct LiChaoTreeLine
{
  T a{}, b{};
  int id = -1;
  CPP_DUMP_DEFINE_DATA(a, b, id);

  friend bool operator==(const LiChaoTreeLine &x, const LiChaoTreeLine &y)
  {
    return x.a == y.a && x.b == y.b && x.id == y.id;
  }
};

namespace li_chao_tree_detail
{
  template <class T, class U, class Compare, auto inftyT, auto inftyU>
  struct Traits
  {
    static_assert(!is_integral_ext<T> || is_signed_ext<T>);
    static_assert(!is_integral_ext<U> || is_signed_ext<U>);
    static_assert(is_same_v<Compare, less<>> || is_same_v<Compare, less<T>> ||
                  is_same_v<Compare, greater<>> || is_same_v<Compare, greater<T>>);
    using Line = LiChaoTreeLine<T>;
    static constexpr bool minimize = is_same_v<Compare, less<>> || is_same_v<Compare, less<T>>;

    static U eval(const Line &f, const T &x) { return U(f.a) * U(x) + U(f.b); }
    static bool better(const U &x, const U &y)
    {
      if constexpr (minimize) return x < y;
      else return y < x;
    }
    static bool better(const Line &f, const Line &g, const T &x)
    {
      return better(eval(f, x), eval(g, x));
    }
    static pair<U, Line> empty()
    {
      T t = resolved_infty<T, inftyT>();
      U u = resolved_infty<U, inftyU>();
      if constexpr (!minimize) t = -t, u = -u;
      return {u, {T(0), t, -1}};
    }
    static void update(pair<U, Line> &ans, const Line &f, const T &x)
    {
      if (f.id == -1) return;
      U value = eval(f, x);
      if (ans.second.id == -1 || better(value, ans.first)) ans = {value, f};
    }
  };
}

// [xmin, xmax) の範囲を指定する
template <class T, class U = larger_int_t<T>, class Compare = less<>,
          auto inftyT = nullptr, auto inftyU = nullptr>
struct LiChaoTree
{
  static_assert(is_integral_ext<T> && is_signed_ext<T>);
  using Line = LiChaoTreeLine<T>;

private:
  using Traits = li_chao_tree_detail::Traits<T, U, Compare, inftyT, inftyU>;
  struct Node
  {
    Line line;
    int chi[2] = {-1, -1};
  };
  T xmin, xmax;
  vc<Node> nodes;
  int add_cnt = 0;

  static T middle(T l, T r)
  {
    using W = make_unsigned_ext_t<T>;
    W width = W(r) - W(l);
    return l + T(width / 2);
  }
  int child(int p, int dir)
  {
    if (nodes[p].chi[dir] == -1)
    {
      int i = nodes.size();
      nodes.emplace_back();
      nodes[p].chi[dir] = i;
    }
    return nodes[p].chi[dir];
  }
  void insert(Line f, int p, T l, T r)
  {
    while (true)
    {
      if (nodes[p].line.id == -1)
      {
        nodes[p].line = f;
        return;
      }
      T m = middle(l, r);
      if (Traits::better(f, nodes[p].line, m)) swap(f, nodes[p].line);
      if (l == r - 1) return;
      if (Traits::better(f, nodes[p].line, l)) p = child(p, 0), r = m;
      else if (Traits::better(f, nodes[p].line, T(r - 1))) p = child(p, 1), l = m;
      else return;
    }
  }
  void insert_segment(Line f, T ql, T qr, int p, T l, T r)
  {
    if (ql <= l && r <= qr)
    {
      insert(f, p, l, r);
      return;
    }
    T m = middle(l, r);
    if (ql < m) insert_segment(f, ql, qr, child(p, 0), l, m);
    if (m < qr) insert_segment(f, ql, qr, child(p, 1), m, r);
  }

public:
  LiChaoTree(T xmin, T xmax) : xmin(xmin), xmax(xmax), nodes(1)
  {
    assert(xmin < xmax);
  }

  void reserve(int n)
  {
    assert(n >= 0);
    nodes.reserve(n);
  }

  void add_line(T a, T b) { add_line(a, b, add_cnt); }
  void add_line(T a, T b, int id)
  {
    assert(id >= 0);
    ++add_cnt;
    insert({a, b, id}, 0, xmin, xmax);
  }

  // [l, r) に y = ax + b を追加
  void add_segment(T l, T r, T a, T b) { add_segment(l, r, a, b, add_cnt); }
  void add_segment(T l, T r, T a, T b, int id)
  {
    assert(l <= r && id >= 0);
    ++add_cnt;
    l = max(l, xmin), r = min(r, xmax);
    if (l < r) insert_segment({a, b, id}, l, r, 0, xmin, xmax);
  }

  pair<U, Line> query(T x) const
  {
    assert(xmin <= x && x < xmax);
    auto ans = Traits::empty();
    T l = xmin, r = xmax;
    int p = 0;
    while (p != -1)
    {
      Traits::update(ans, nodes[p].line, x);
      if (l == r - 1) break;
      T m = middle(l, r);
      if (x < m) p = nodes[p].chi[0], r = m;
      else p = nodes[p].chi[1], l = m;
    }
    return ans;
  }

  void clear()
  {
    nodes.clear();
    nodes.emplace_back();
    add_cnt = 0;
  }
};

// クエリ先読み
template <class T, class U = larger_int_t<T>, class Compare = less<>,
          auto inftyT = nullptr, auto inftyU = nullptr>
struct LiChaoTreeCompressed
{
  using Line = LiChaoTreeLine<T>;

private:
  using Traits = li_chao_tree_detail::Traits<T, U, Compare, inftyT, inftyU>;
  vc<T> xs;
  vc<Line> dat;
  int add_cnt = 0;

  void insert(Line f, int p, int l, int r)
  {
    while (true)
    {
      if (dat[p].id == -1)
      {
        dat[p] = f;
        return;
      }
      int m = (l + r) / 2;
      if (Traits::better(f, dat[p], xs[m])) swap(f, dat[p]);
      if (r - l == 1) return;
      if (Traits::better(f, dat[p], xs[l])) p *= 2, r = m;
      else if (Traits::better(f, dat[p], xs[r - 1])) p = 2 * p + 1, l = m;
      else return;
    }
  }
  void insert_segment(Line f, int ql, int qr, int p, int l, int r)
  {
    if (ql <= l && r <= qr)
    {
      insert(f, p, l, r);
      return;
    }
    int m = (l + r) / 2;
    if (ql < m) insert_segment(f, ql, qr, 2 * p, l, m);
    if (m < qr) insert_segment(f, ql, qr, 2 * p + 1, m, r);
  }
  template <bool Indexed>
  pair<U, Line> query_impl(const T &x, int k = -1) const
  {
    auto ans = Traits::empty();
    int p = 1, l = 0, r = xs.size();
    while (true)
    {
      Traits::update(ans, dat[p], x);
      if (r - l == 1) break;
      int m = (l + r) / 2;
      bool left;
      if constexpr (Indexed) left = k < m;
      else left = x < xs[m];
      if (left) p *= 2, r = m;
      else p = 2 * p + 1, l = m;
    }
    assert(xs[l] == x);
    return ans;
  }

public:
  explicit LiChaoTreeCompressed(vc<T> xs) : xs(std::move(xs))
  {
    sort(this->xs.begin(), this->xs.end());
    this->xs.erase(unique(this->xs.begin(), this->xs.end()), this->xs.end());
    dat.resize(4 * this->xs.size());
  }

  const vc<T> &coordinates() const { return xs; }
  int lower_bound(const T &x) const { return std::lower_bound(xs.begin(), xs.end(), x) - xs.begin(); }

  void add_line(T a, T b) { add_line(a, b, add_cnt); }
  void add_line(T a, T b, int id)
  {
    assert(id >= 0);
    ++add_cnt;
    if (!xs.empty()) insert({a, b, id}, 1, 0, xs.size());
  }

  // 座標 [l, r) に y = ax + b を追加
  void add_segment(T l, T r, T a, T b) { add_segment(l, r, a, b, add_cnt); }
  void add_segment(T l, T r, T a, T b, int id)
  {
    assert(l <= r);
    add_segment_index(lower_bound(l), lower_bound(r), a, b, id);
  }

  // 圧縮後の添字 [l, r) に y = ax + b を追加
  void add_segment_index(int l, int r, T a, T b) { add_segment_index(l, r, a, b, add_cnt); }
  void add_segment_index(int l, int r, T a, T b, int id)
  {
    assert(0 <= l && l <= r && r <= int(xs.size()) && id >= 0);
    ++add_cnt;
    if (l < r) insert_segment({a, b, id}, l, r, 1, 0, xs.size());
  }

  // x は座標
  pair<U, Line> query(const T &x) const
  {
    assert(!xs.empty());
    return query_impl<false>(x);
  }
  // k は圧縮後の添字
  pair<U, Line> query_index(int k) const
  {
    assert(0 <= k && k < int(xs.size()));
    return query_impl<true>(xs[k], k);
  }

  void clear()
  {
    fill(dat.begin(), dat.end(), Line{});
    add_cnt = 0;
  }
};
