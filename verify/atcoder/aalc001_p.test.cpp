#define PROBLEM "https://atcoder.jp/contests/aalc001/tasks/aalc001_p"

#include "algo/two_pointers.hpp"
#include "ds/interval_crossing.hpp"

int main()
{
  INT(N, Q);
  vc<pair<int, int>> intervals(N);
  for (auto &[a, b] : intervals) cin >> a >> b;
  IntervalCrossing ds(intervals);
  auto add_r = [&](int, int r) { ds.insert(r); };
  auto del_l = [&](int l, int) { ds.erase(l); };
  // 右端の区間を除く族はラミナーなので、右端の区間との交差だけを調べる。
  auto judge = [&](int l, int r) { return l == r || ds.can_insert(r - 1); };
  const auto rok = right_ok<int>(N, add_r, del_l, judge);
  repi(q, Q)
  {
    INT(l, r);
    PRINT(r <= rok[l - 1] ? "Yes" : "No");
  }
}
