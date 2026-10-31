#pragma once

#include "polygon.hpp"

/**
 * @brief 最近点対・凸多角形の直径
 * @docs docs/geometry/point_set.md
 */

template <class T> optional<pair<Point<T>, Point<T>>> closest_pair(vector<Point<T>> p)
{
  if (p.size() < 2) return nullopt;
  sort(p.begin(), p.end());
  for (size_t i = 1; i < p.size(); ++i) if (p[i - 1] == p[i]) return pair{p[i - 1], p[i]};
  using W = geometry_wide_t<T>;
  pair<Point<T>, Point<T>> ans{p[0], p[1]};
  W best = distance2(ans.first, ans.second);
  vector<Point<T>> buffer(p.size()), strip;
  auto compare_y = [](const Point<T> &a, const Point<T> &b)
  { return a.y < b.y || (!(b.y < a.y) && a.x < b.x); };
  auto update = [&](const Point<T> &a, const Point<T> &b)
  {
    W d = distance2(a, b);
    if (d < best) { best = d; ans = {a, b}; }
  };
  auto solve = [&](auto &&self, size_t l, size_t r) -> void
  {
    if (r - l <= 3)
    {
      for (size_t i = l; i < r; ++i) for (size_t j = i + 1; j < r; ++j) update(p[i], p[j]);
      sort(p.begin() + l, p.begin() + r, compare_y);
      return;
    }
    size_t m = (l + r) / 2;
    T middle = p[m].x;
    self(self, l, m); self(self, m, r);
    merge(p.begin() + l, p.begin() + m, p.begin() + m, p.begin() + r, buffer.begin() + l, compare_y);
    copy(buffer.begin() + l, buffer.begin() + r, p.begin() + l);
    strip.clear();
    for (size_t i = l; i < r; ++i)
    {
      W dx = W(p[i].x) - W(middle);
      if (dx * dx >= best) continue;
      for (size_t j = strip.size(); j > 0; --j)
      {
        W dy = W(p[i].y) - W(strip[j - 1].y);
        if (dy * dy >= best) break;
        update(p[i], strip[j - 1]);
      }
      strip.push_back(p[i]);
    }
  };
  solve(solve, 0, p.size());
  return ans;
}

template <class T> optional<pair<Point<T>, Point<T>>> convex_diameter(const vector<Point<T>> &input)
{
  vector<Point<T>> p;
  for (const auto &q : input) if (p.empty() || p.back() != q) p.push_back(q);
  if (p.size() > 1 && p.front() == p.back()) p.pop_back();
  if (p.empty()) return nullopt;
  if (p.size() == 1) return pair{p[0], p[0]};
  if (signed_area2(p) == geometry_wide_t<T>(0))
  {
    auto mm = minmax_element(p.begin(), p.end());
    return pair{*mm.first, *mm.second};
  }
  vector<Point<T>> corners;
  for (size_t i = 0; i < p.size(); ++i)
    if (cross(p[(i + p.size() - 1) % p.size()], p[i], p[(i + 1) % p.size()]) != geometry_wide_t<T>(0))
      corners.push_back(p[i]);
  p.swap(corners);
  pair<Point<T>, Point<T>> ans{p[0], p[1]};
  auto best = distance2(ans.first, ans.second);
  auto update = [&](size_t i, size_t j)
  {
    auto d = distance2(p[i], p[j]);
    if (best < d) { best = d; ans = {p[i], p[j]}; }
  };
  size_t j = 1, n = p.size();
  for (size_t i = 0; i < n; ++i)
  {
    size_t next = (i + 1) % n;
    auto height = [&](size_t k) { return abs(cross(p[i], p[next], p[k])); };
    while (height(j) < height((j + 1) % n)) j = (j + 1) % n;
    update(i, j); update(next, j);
    if (height(j) == height((j + 1) % n)) { update(i, (j + 1) % n); update(next, (j + 1) % n); }
  }
  return ans;
}
