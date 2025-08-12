#pragma once

#include "../template/template_all_but_modint.hpp"
#include "extgcd.hpp"

/**
 * @brief Stern Brocot Tree
 * @docs docs/math/sbt.md
 */

// 保持するもの
// - range: p, q, r, s
// 性質
// - num/den == (p+r)/(q+s)
// - 左の子は (p, q, p+r, q+s), 右の子は (p+r, q+s, r, s)
// - 子孫の下限は p/q, 上限は r/s
template <class T = ll>
struct SBTNode
{
  using Path = vc<pair<char, T>>;

public:
  T p, q, r, s;
  T num() const { return p + r; }
  T den() const { return q + s; }

  SBTNode() : p(0), q(1), r(1), s(0) {}
  SBTNode(T num_, T den_)
  {
    auto [g, _, __] = extgcd(num_, den_);
    num_ /= g, den_ /= g;
    p = num_, q = den_, r = 0, s = 0;
    *this = SBTNode(encode_path());
  }
  SBTNode(T p, T q, T r, T s) : p(p), q(q), r(r), s(s) {}
  SBTNode(const Path &path) : p(0), q(1), r(1), s(0) { fec([ dir, d ] : path) descend(dir, d); }

  // dir 方向に d 個潜る (dir は 'L' か 'R')
  void descend(char dir, T d)
  {
    assert(dir == 'L' || dir == 'R');
    assert(d >= 0);
    if (dir == 'L')
      r += d * p, s += d * q;
    else
      p += d * r, q += d * s;
  }
  // 親方向に d 個上がる
  void ascend(T d)
  {
    fec([dir, c] : reversed(encode_path()))
    {
      T e = min(c, d);
      if (dir == 'L')
        r -= e * p, s -= e * q;
      else
        p -= e * r, q -= e * s;
      d -= e;
    }
    assert(d == 0 && "failed: 0 <= d <= depth");
  }
  T depth() const
  {
    T res = 0;
    fec([ dir, d ] : encode_path()) res += d;
    return res;
  }

  Path encode_path() const
  {
    T f = num(), g = den();
    vc<pair<char, T>> path;
    char dir = 'R';
    if (f < g)
    {
      dir = 'L';
      swap(f, g);
    }
    while (f != 1)
    {
      path.eb(dir, f / g);
      dir ^= 'L' ^ 'R';
      f %= g;
      swap(f, g);
    }
    if (!path.empty())
    {
      if (path.back().second == 1)
        path.pop_back();
      else
        path.back().second--;
    }
    return path;
  }

  SBTNode<T> lca(const SBTNode<T> &rhs) const
  {
    SBTNode<T> node;
    auto path1 = encode_path(), path2 = rhs.encode_path();
    repi(i, min(path1.size(), path2.size()))
    {
      if (path1[i].first == path2[i].first)
      {
        char dir = path1[i].first;
        T c = min(path1[i].second, path2[i].second);
        node.descend(dir, c);
      }
      if (path1[i] != path2[i])
        break;
    }
    return node;
  }
};
#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(num(), den(), p, q, r, s);
#endif

// judge は bool(T num, T den)
// judge: [0, ∞] → {true, false}
// judge は単調性を持つ、すなわち、ある実数 α を境に true と false が切り替わる
// 分母・分子が max_value 以下の有理数のうち、α に最も近いもの (下側: p/q, 上側: r/s) を返す
template <class T = ll, class F>
SBTNode<T> sbt_search(const F &judge, T max_value)
{
  SBTNode<T> node;
  const bool judge_01 = judge(0, 1), judge_10 = judge(1, 0);
  assert(judge_01 != judge_10);
  assert(max_value >= 1);
  while (node.num() <= max_value && node.den() <= max_value)
  {
    const bool judge_now = judge(node.num(), node.den());
    const char dir = judge_now ^ judge_01 ? 'L' : 'R';
    T max_d;
    if (dir == 'L')
    {
      if (node.p == 0)
        max_d = (max_value - node.s) / node.q;
      else
        max_d = min((max_value - node.r) / node.p, (max_value - node.s) / node.q);
    }
    else
    {
      if (node.s == 0)
        max_d = (max_value - node.p) / node.r;
      else
        max_d = min((max_value - node.q) / node.s, (max_value - node.p) / node.r);
    }
    auto judge_next = [&](T d) -> bool
    {
      auto next_node = node;
      next_node.descend(dir, d);
      return judge_now == judge(next_node.num(), next_node.den());
    };
    T dl = 1, dr = 2;
    while (dr <= max_d)
    {
      if (!judge_next(dr))
        break;
      dl = dr, dr = min(2 * dr, max_d + 1);
    }
    while (dr - dl > 1)
    {
      T dm = dl + (dr - dl) / 2;
      (judge_next(dm) ? dl : dr) = dm;
    }
    node.descend(dir, dl);
  }
  return node;
}

// a/b < p/q < c/d を満たす p/q のうち q が最小のもの
// q != 1 なら一意。q == 1 なら p が最小となるようにとる
// https://atcoder.jp/contests/abc408/tasks/abc408_g/editorial
template <class T>
pair<T, T> abc408g(T a, T b, T c, T d)
{
  T n = a / b;
  a -= n * b, c -= n * d;
  if (c > d)
    return {n + 1, 1};
  auto [p, q] = abc408g(d, c, b, a);
  return {p * n + q, p};
}
