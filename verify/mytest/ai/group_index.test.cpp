#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/group_index.hpp"

// Test focus: empty input, missing values, and value_bound() with gaps in the values.
int main()
{
  GroupIndex<int> empty(vc<int>{});
  assert(empty.value_bound() == 0);

  GroupIndex<int> grp(vc<int>{0, 2, 2, 5});
  assert(grp.value_bound() == 6);
  assert(grp.idxs(1).empty());
  assert((vc<int>(grp.idxs(2).begin(), grp.idxs(2).end()) == vc<int>{1, 2}));

  cout << "Hello World" << endl;
}
