#define PROBLEM "https://judge.yosupo.jp/problem/segment_add_get_min"

#include "convex/li_chao_tree.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, q;
  cin >> n >> q;
  struct Segment { ll l, r, a, b; };
  vc<Segment> lines(n);
  for (auto &f : lines) cin >> f.l >> f.r >> f.a >> f.b;
  struct Query { int type; ll l, r, a, b; };
  vc<Query> queries(q);
  vc<ll> xs;
  for (auto &op : queries)
  {
    cin >> op.type >> op.l;
    if (op.type == 0) cin >> op.r >> op.a >> op.b;
    else xs.push_back(op.l);
  }
  LiChaoTreeCompressed<ll> tree(xs);
  for (auto f : lines) tree.add_segment(f.l, f.r, f.a, f.b);
  for (auto op : queries)
  {
    if (op.type == 0) tree.add_segment(op.l, op.r, op.a, op.b);
    else
    {
      auto [value, line] = tree.query(op.l);
      if (line.id == -1) cout << "INFINITY\n";
      else cout << value << '\n';
    }
  }
}
