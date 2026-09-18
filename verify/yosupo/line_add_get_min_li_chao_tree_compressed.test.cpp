#define PROBLEM "https://judge.yosupo.jp/problem/line_add_get_min"

#include "convex/li_chao_tree.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, q;
  cin >> n >> q;
  vc<pair<ll, ll>> lines(n);
  for (auto &[a, b] : lines) cin >> a >> b;
  struct Query { int type; ll a, b; };
  vc<Query> queries(q);
  vc<ll> xs;
  for (auto &op : queries)
  {
    cin >> op.type >> op.a;
    if (op.type == 0) cin >> op.b;
    else xs.push_back(op.a);
  }
  LiChaoTreeCompressed<ll> tree(xs);
  for (auto [a, b] : lines) tree.add_line(a, b);
  for (auto op : queries)
  {
    if (op.type == 0) tree.add_line(op.a, op.b);
    else cout << tree.query_index(tree.lower_bound(op.a)).first << '\n';
  }
}
